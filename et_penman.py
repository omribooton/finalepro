import requests
import pandas as pd
import time
import pyet

# Configuration
channel_id = "2804506"
read_api_key = "K97I6D8WEXKL0SXQ"
write_api_key = "GB6Q5ZQYX6T4LATT"
base_url = f"https://api.thingspeak.com/channels/{channel_id}/feeds.json"
update_url = "https://api.thingspeak.com/update.json"

# Field descriptions
fields = {
    "field1": "Temperature (°C)",
    "field2": "Relative Humidity (%)",
    "field3": "Daily Radiation (MJ/m²/day)",
    "field4": "Wind Speed (m/s)"
}

def fetch_data():
    try:
        response = requests.get(base_url, params={"api_key": read_api_key, "results": 100})
        response.raise_for_status()
        return response.json().get("feeds", [])
    except requests.RequestException as e:
        print(f"Error fetching data: {e}")
        return []

def save_and_process_data(data):
    df = pd.DataFrame([{**{"Timestamp": entry["created_at"]}, **{fields[key]: entry.get(key, "") for key in fields}} for entry in data])
    df.to_csv("thingspeak_data.csv", mode='a', header=not bool(pd.read_csv("thingspeak_data.csv", nrows=1).empty), index=False)
    
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])
    df.set_index('Timestamp', inplace=True)
    df = df.apply(pd.to_numeric, errors='coerce')
    
    daily_mean = df.resample('24H').mean().fillna(method='ffill')
    daily_mean.to_csv("thingspeak_24h_mean.csv")
    
    return daily_mean.tail(1)  # Get the most recent entry for Penman calculation

def calculate_and_upload_penman(daily_mean):
    if daily_mean.empty: return
    
    # Penman calculation
    elevation, latitude = 31.0, pyet.utils.deg_to_rad(32.0073)
    penm = pyet.combination.penman(
        tmean=daily_mean["Temperature (°C)"],
        wind=daily_mean["Wind Speed (m/s)"],
        pressure=101.3,
        elevation=elevation,
        rh=daily_mean["Relative Humidity (%)"],
        rs=daily_mean["Daily Radiation (MJ/m²/day)"],
        lat=latitude
    )

    # Upload Penman result to ThingSpeak
    requests.post(update_url, data={"api_key": write_api_key, "field5": penm[0]})
    print(f"Uploaded Penman ET: {penm[0]}")

def main():
    while True:
        data = fetch_data()
        if data:
            daily_mean = save_and_process_data(data)
            calculate_and_upload_penman(daily_mean)
        else:
            print("No data fetched or an error occurred.")
        time.sleep(360)

if __name__ == "__main__":
    main()
