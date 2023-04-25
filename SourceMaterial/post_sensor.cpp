#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN D4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MAKONOI"; //WiFi Name
const char* password = "12345678"; //WiFi Password
const char* destURL = "http://161.246.52.20/expo/import.php"; //Server URL
int delayTime = 20 * 1000; //20s
String deviceName = "device00";
float sensor0 = 20;
float sensor1 = 10;

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
    // Receive data from sensor
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    if (isnan(temp) || isnan(humid)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
      }
    // Create the POST data
    String postdata = "deviceName=" + deviceName + "&sensor00=" + String(temp) + "&sensor01=" + String(humid);

    // Send the POST request
    WiFiClient client;
    HTTPClient http;
    http.begin(client, destURL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(postdata);

    // Check for errors
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("Server response: " + response);
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
