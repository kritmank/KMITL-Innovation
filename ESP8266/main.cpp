#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MAKONOI"; //WiFi Name
const char* password = "12345678"; //WiFi Password
int delayTime = 20 * 1000; //10s

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, password); //Connected WiFi
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("--------- Start ------------");

  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    String temp_str = String(temp);
    String humid_str = String(humid);

    HTTPClient http;
  // Set the URL of the server
    http.begin("http://161.246.52.20/expo/import.php");

    // Set the content type of the request
    http.addHeader("Content-Type", "application/json");

    // Create a JSON document
    DynamicJsonDocument jsonDoc(1024);

    // Add data to the JSON document
    jsonDoc["deviceName"] = "device00";
    jsonDoc["sensor00"] = temp_str;
    jsonDoc["sensor01"] = humid_str;

    Serial.println("Temperature : " + temp_str);
    Serial.println("Humidity: " + humid_str);

    // Serialize the JSON document to a string
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    // Send the POST request
    int httpResponseCode = http.POST(jsonString);

    // Check for errors
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending POST request: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();

    // Wait for some time before sending the next request
    delay(1000);
  }
  delay(delayTime);
}