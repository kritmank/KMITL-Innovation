#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Connect to Wi-Fi network
  WiFi.begin("your_SSID", "your_PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  // Create an HTTPClient object
  HTTPClient http;

  // Set the URL of the server
  http.begin("http://161.246.52.20/expo/import.php");

  // Set the content type of the request
  http.addHeader("Content-Type", "application/json");

  // Create a JSON document
  DynamicJsonDocument jsonDoc(1024);

  // Add data to the JSON document
  jsonDoc["deviceName"] = "device00";
  jsonDoc["sensor00"] = 50;
  jsonDoc["sensor01"] = 50;

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