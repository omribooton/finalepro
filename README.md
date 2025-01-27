**Agrotech Lab finan project: Automated Irrigation System Using Evapotranspiration Data**

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
* [RY-EBN-1 PAR Sensor](https://www.compactweathersensor.com/solar-radiation-sensors/ry-ebn-1-par-sensor.html)
* [SHT 31](https://wiki.dfrobot.com/SHT31_Temperature_Humidity_Sensor_Weatherproof_SKU_SEN0385)
* [WIND]()
* [ADS 1115 ](https://www.adafruit.com/product/1083)
* [12V Power source](https://www.iec.co.il/home)
* Electric box
* Electric wires
* Solder breadboard



**Code:**

We have two separate code files:

[esp 32 Code](https://github.com/omribooton/finalepro/blob/main/esp%2032%20code): This runs on the ESP32 to manage sensors, control the irrigation system via MQTT, and interact with ThingSpeak for data logging and retrieval.

Required Python Libraries: requests, pandas, pyet 

[et_penman.py](https://github.com/omribooton/finalepro/blob/main/et_penman.py):
 This Python script calculates the Penman evapotranspiration (ET) value using data retrieved from ThingSpeak and uploads the calculated ET value back to ThingSpeak.
 
Required Arduino Libraries: WiFi.h, Wire.h, Adafruit_SHT31.h, Adafruit_ADS1X15.h, ThingSpeak.h, NTPClient.h, HTTPClient.h, and PubSubClient.h


  
