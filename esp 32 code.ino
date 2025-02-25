#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>  // For SHT31 sensor
#include <Adafruit_ADS1X15.h> // For radiation sensor
#include <ThingSpeak.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <PubSubClient.h> // For MQTT communication

// WiFi credentials
const char* ssid = "agrotech";
const char* password = "1Afuna2gezer";

WiFiClient client;

// MQTT broker details
const char* mqtt_server = "192.168.0.102";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt-user";
const char* mqtt_password = "1234";
const char* mqttTopic = "/greenhouse/outside/irrigation/solenoid8";

PubSubClient mqttClient(client);

// Time Synchronization
const long utcOffsetInSeconds = 3600 * 2; // UTC+2
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String formattedTime;

// Sensor declarations
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_ADS1115 ads;

// Variables for sensors and calculations
const int WIND_SENSOR_PIN = A0;
float sensorValue, voltage, wind_speed;
float ET = 2.4; // AVG ET in israel (winter)
float Kc = 1.6; // Crop Coefficient for Mint
float area = 1.5; // Area in m²
float efficiency = 0.8;
float flow_rate = 4; // L/hour
float wateringTime; // Seconds
int num_drips = 2;
int lastWateringDay = -1;  // Track the day when watering was performed
unsigned long wateringStartTime = 0;
bool isWatering = false;

// ThingSpeak credentials
unsigned long channelID = 2804506;
const char* writeApiKey = "GB6Q5ZQYX6T4LATT";
const char* readApiKey = "K97I6D8WEXKL0SXQ";

// Setup MQTT connection
void setupMQTT() {
    mqttClient.setServer(mqtt_server, mqtt_port);
}

// Reconnect to MQTT broker on every loop iteration
bool reconnectMQTT() {
    Serial.println("Checking MQTT Connection...");
    if (!mqttClient.connected()) {
        Serial.println("MQTT Disconnected. Attempting to reconnect...");
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);
        if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("MQTT Connected.");
            return true;
        } else {
            Serial.println("MQTT Connection failed. Will retry in the next loop.");
            return false;
        }
    }
    Serial.println("MQTT Already Connected.");
    return true;
}
// reading ET0 from think speak
void readETFromThingSpeak() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = "http://api.thingspeak.com/channels/" + String(channelID) + "/fields/5/last?api_key=" + String(readApiKey);
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0) {
            String payload = http.getString();
            ET = payload.toFloat();
            Serial.print("ET from ThingSpeak: ");
            Serial.println(ET);
        } else {
            Serial.println("Failed to read ET from ThingSpeak. Using default value.");
        }
        http.end();
    }
}
// reading the sensor every 10 min
void readSensorsAndSendData() {
    int16_t rad_results = ads.readADC_Differential_0_1();
    float radiation = ((rad_results * 0.03125) / 0.260);
    float temperature = sht31.readTemperature();
    float humidity = sht31.readHumidity();
    sensorValue = analogRead(WIND_SENSOR_PIN);
    voltage = sensorValue * (3.3 / 4095.0);
   wind_speed=0;
     if (voltage >= 0.4) { 
       wind_speed = ((voltage - 0.4) / 1.6) * 32.4;}
     // printing data from sesnors
    Serial.print("Timestamp: "); Serial.println(formattedTime);
    Serial.print("Radiation: "); Serial.print(radiation); Serial.println(" W/m²");
    Serial.print("Temperature (°C): "); Serial.println(temperature);
    Serial.print("Humidity (%): "); Serial.println(humidity);
    Serial.print("Wind Speed (m/s): "); Serial.println(wind_speed);
    Serial.print("ET (mm/day): "); Serial.println(ET);
    Serial.print("Watering Time (s): "); Serial.println(wateringTime);
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, radiation);
    ThingSpeak.setField(4, wind_speed);
    ThingSpeak.setStatus(formattedTime);
    int httpCode = ThingSpeak.writeFields(channelID, writeApiKey);
    if (httpCode == 200) {
        Serial.println("Data successfully sent to ThingSpeak.");
    } else {
        Serial.println("Failed to send data to ThingSpeak. HTTP Code: " + String(httpCode));
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    ThingSpeak.begin(client);
    if (!sht31.begin(0x44)) {
        Serial.println("Couldn't find SHT31 sensor!");
        while (1);
    }
    ads.setGain(GAIN_FOUR);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115.");
        while (1);
    }
    timeClient.begin();
    setupMQTT();
}

void loop() {
    reconnectMQTT();
    mqttClient.loop();
    timeClient.update();
    formattedTime = timeClient.getFormattedTime();
    int currentHour = timeClient.getHours();
    int currentDay = timeClient.getDay();

    if (currentHour >= 6 && currentDay != lastWateringDay && !isWatering) {
        readETFromThingSpeak();
        if (ET <= 0) ET = 2.4;// standart for winter in israel
        wateringTime = (((ET * Kc * area) / efficiency) / (flow_rate * num_drips))*3600;
        wateringStartTime = millis();
        isWatering = true;
        mqttClient.publish(mqttTopic, "1");
        Serial.println("Published MQTT: Open Solenoid 8");
    }
//// leaving the water on while the loop running
    if (isWatering && millis() - wateringStartTime >= wateringTime * 1000) {
        mqttClient.publish(mqttTopic, "0");
        Serial.println("Published MQTT: Close Solenoid 8");
        isWatering = false;
        lastWateringDay = currentDay;
        ThingSpeak.setField(6, wateringTime);
    }

    readSensorsAndSendData();
    delay(600000);
    
}
