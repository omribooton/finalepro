**Agrotech Lab finan project: Automated Irrigation System Using Evapotranspiration Data**

**Introduction:**

This project aims to develop an automated irrigation system that leverages evapotranspiration (ET) data to optimize water usage efficiently.
The system collects data from a meteorological station equipped with a PAR radiation sensor, anemometer, temperature gauge, and hygrometer. 
This data is transmitted to the ThingSpeak cloud platform, where the ET is calculated and stored for further analysis.
Using the ET values, the system determines the required irrigation duration and controls the greenhouse irrigation solenoid via the MQTT protocol.

**Hardware and assembly:**
**Sketch:** 
<img width="951" alt="SENSORS" src="https://github.com/user-attachments/assets/5455995b-8eac-4985-8a76-5bf714dd532c" />

Hardware:
* ESP 32  https://www.espressif.com/en/products/socs/esp32
