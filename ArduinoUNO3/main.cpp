#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MAKONOI"; //WiFi Name
const char* password = "12345678"; //WiFi Password
int delayTime = 20 * 1000; //10s

char server[] = "161.246.52.20"; //Server IP address
WiFiClient client;

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

    // Connect to the server
    if (client.connect(server, 80)) {
      // Make a HTTP POST request
      client.println("POST /expo/import.php HTTP/1.1");
      client.println("Host: 161.246.52.20");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(jsonString.length());
      client.println();
      client.println(jsonString);
    } else {
      Serial.println("Connection failed");
    }

    // Wait for some time before sending the next request
    delay(1000);

    // Disconnect from the server
    client.stop();
  }
  delay(delayTime);
}
