**Agrotech Lab final project: Automated Irrigation System Using Evapotranspiration Data**

<img width="246" alt="esp32" src="https://github.com/user-attachments/assets/5f7b359c-5499-44b1-b602-41057d490905" />



**Introduction:**

This project aims to develop an automated irrigation system that leverages evapotranspiration (ET) data to optimize water usage efficiently.
The system collects data from a meteorological station equipped with a PAR radiation sensor, anemometer, temperature gauge, and hygrometer. 
This data is transmitted to the ThingSpeak cloud platform, where the ET is calculated and stored for further analysis.
Using the ET values, the system determines the required irrigation duration and controls the greenhouse irrigation solenoid via the MQTT protocol.

**Hardware and assembly:**

**Sketch:** 

<img width="951" alt="SENSORS" src="https://github.com/user-attachments/assets/5455995b-8eac-4985-8a76-5bf714dd532c" />

* In the Sketch there is no ESP 32 connection for electricity. The ESP 32 is connected to 5V through vcc and GND.
  

**Hardware:**
* [ESP 32](https://www.espressif.com/en/products/socs/esp32)
* [RY-EBN-1 PAR Sensor, Sensitivity of 260.03 μV/μ*mol*m2*s ](https://www.compactweathersensor.com/solar-radiation-sensors/ry-ebn-1-par-sensor.html)
* [SHT 31](https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385)
* [WIND]()
* [ADS 1115 ](https://www.adafruit.com/product/1083)
* [12V Power source](https://www.iec.co.il/home)
* Electric box
* Electric wires
* Solder breadboard


<div style="display: flex; justify-content: center; align-items: center; gap: 20px;">
  <a href="https://github.com/user-attachments/assets/26b3903f-d668-451e-8018-17017d28efcf" target="_blank">
    <img src="https://github.com/user-attachments/assets/26b3903f-d668-451e-8018-17017d28efcf" alt="PAR" width="250" style="border: 2px solid #000; border-radius: 5px;"/>
  </a>
  
  <a href="https://github.com/user-attachments/assets/8976c230-f755-48e7-b685-c276087f597b" target="_blank">
    <img src="https://github.com/user-attachments/assets/8976c230-f755-48e7-b685-c276087f597b" alt="SHT" width="250" style="border: 2px solid #000; border-radius: 5px;"/>
  </a>

  <a href="https://github.com/user-attachments/assets/7429dfe7-0586-4658-b7b3-06668f8e92d7" target="_blank">
    <img src="https://github.com/user-attachments/assets/7429dfe7-0586-4658-b7b3-06668f8e92d7" alt="WIND" width="250" style="border: 2px solid #000; border-radius: 5px;"/>
  </a>

  <a href="https://github.com/user-attachments/assets/e241bb1f-98f5-4c57-9f31-6679799dc703" target="_blank">
    <img src="https://github.com/user-attachments/assets/e241bb1f-98f5-4c57-9f31-6679799dc703" alt="ALL" width="250" style="border: 2px solid #000; border-radius: 5px;"/>
  </a>
</div>






**Code:**

We have two separate code files:

[esp 32 Code](https://github.com/omribooton/finalepro/blob/main/esp%2032%20code):

This code runs on the ESP32 to manage sensors, control the irrigation system via MQTT, and interact with ThingSpeak for data logging and retrieval.

**Required Python Libraries:** 
requests, pandas, pyet 

**If you are using the code, make the following adjustments:** 

<details>
<summary>Click to toggle adjustments <code>code</code></summary>

```cpp
// Wi-Fi Adjustment
const char* ssid = "your_wifi_name"; // Change to the new WiFi network name
const char* password = "your_wifi_password"; // Change to the new WiFi password

// Time Zone Adjustment
const long utcOffsetInSeconds = 3600 * new_offset; // Set the appropriate UTC offset

// Irrigation Calculation Parameters Adjustment
float ET = 2.4; // Default evapotranspiration value, adjust based on climate conditions
float Kc = 1.6; // Crop coefficient, change according to the specific crop type
float area = 1.5; // The area of the irrigation zone in square meters, modify as needed
float efficiency = 0.9; // Irrigation system efficiency (e.g., 0.9 for 90% efficiency)
float flow_rate = 4; // Flow rate of the irrigation system in liters per hour per dripper
int num_drips = 2; // Number of drippers in the irrigation system, update accordingly

// MQTT Setup
const char* mqtt_server = "your_mqtt_broker_ip"; // Update with your MQTT broker IP
const int mqtt_port = 1883; // Update the broker port if needed
const char* mqtt_user = "your_mqtt_username"; // Update with your MQTT username
const char* mqtt_password = "your_mqtt_password"; // Update with your MQTT password
const char* mqttTopic = "/new/location/irrigation/solenoid"; // Update the MQTT topic

// ThingSpeak Setup
unsigned long channelID = your_channel_id; // Update with your ThingSpeak channel ID
const char* writeApiKey = "your_write_api_key"; // Update with your write API key
const char* readApiKey = "your_read_api_key"; // Update with your read API key
```

</details>


---

[et_penman.py](https://github.com/omribooton/finalepro/blob/main/et_penman.py):

This Python script calculates the Penman evapotranspiration (ET) value using data retrieved from ThingSpeak and uploads the calculated ET value back to ThingSpeak.

**Required Arduino Libraries:**  
WiFi.h, Wire.h, Adafruit_SHT31.h, Adafruit_ADS1X15.h, ThingSpeak.h, NTPClient.h, HTTPClient.h, and PubSubClient.h


**If you are using the code, make the following adjustments:** 

<details>
<summary>Click to toggle adjustments</summary>

```python
# Penman calculation Parameters (Elevation, Latitude and Pressure) Adjustment
elevation, latitude = 31.0, pyet.utils.deg_to_rad(32.0073)  # Replace with local elevation and latitude
pressure = 101.3  # Replace with local pressure

# ThingSpeak Setup
channel_id = "your_channel_id"  # Replace with your channel ID
read_api_key = "your_read_api_key"  # Replace with your read API key
write_api_key = "your_write_api_key"  # Replace with your write API key
```

</details>

---





  
