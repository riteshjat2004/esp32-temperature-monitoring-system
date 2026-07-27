/*
 * Temperature Sensor Project (ESP32 + DHT22 + ThingSpeak)
 * -------------------------------------------------------
 * - Reads temperature & humidity
 * - Uploads data to ThingSpeak
 * - Controls a fan/LED based on temperature threshold
 */

#include <WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

// ================= User Configuration =================

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

// ================= DHT Sensor =================

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

// ================= Actuator Configuration =================

const int ACTUATOR_PIN = 12;
const float TEMP_THRESHOLD = 30.0;

// ThingSpeak allows updates every 15 seconds on free accounts.
// Using 20 seconds to stay within limits.
const unsigned long UPDATE_INTERVAL_MS = 20000;
unsigned long lastUploadTime = 0;

// ==========================================================

void setup() {

  Serial.begin(115200);
  delay(100);

  pinMode(ACTUATOR_PIN, OUTPUT);
  digitalWrite(ACTUATOR_PIN, LOW);

  // Initialize DHT Sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  unsigned long wifiTimeout = millis() + 15000;

  while (WiFi.status() != WL_CONNECTED && millis() < wifiTimeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected");
    ThingSpeak.begin(client);
  } else {
    Serial.println("WiFi connection failed. Will retry later.");
  }
}

void loop() {

  unsigned long now = millis();

  // Read Sensor Values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Validate Sensor Readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Display Values
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.print(" °C");

  Serial.print(" | Humidity : ");
  Serial.print(humidity);
  Serial.println(" %");

  // ================= Actuator Control =================

  if (temperature >= TEMP_THRESHOLD) {

    digitalWrite(ACTUATOR_PIN, HIGH);
    Serial.println("Actuator : ON");

  } else {

    digitalWrite(ACTUATOR_PIN, LOW);
    Serial.println("Actuator : OFF");
  }

  // ================= ThingSpeak Upload =================

  if (now - lastUploadTime >= UPDATE_INTERVAL_MS) {

    // Reconnect Wi-Fi if disconnected
    if (WiFi.status() != WL_CONNECTED) {

      Serial.println("WiFi disconnected. Attempting reconnect...");

      WiFi.begin(ssid, password);

      unsigned long timeout = millis() + 10000;

      while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
        delay(500);
        Serial.print(".");
      }

      Serial.println();
    }

    if (WiFi.status() == WL_CONNECTED) {

      ThingSpeak.setField(1, temperature);
      ThingSpeak.setField(2, humidity);
      ThingSpeak.setField(3, digitalRead(ACTUATOR_PIN));

      int response = ThingSpeak.writeFields(channelID, writeAPIKey);

      if (response == 200) {
        Serial.println("Successfully uploaded to ThingSpeak");
      } else {
        Serial.print("ThingSpeak upload failed. Response Code: ");
        Serial.println(response);
      }

    } else {

      Serial.println("WiFi still not connected. Upload skipped.");
    }

    lastUploadTime = now;
  }

  delay(2000);
}