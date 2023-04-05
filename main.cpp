#include <Arduino.h>
#include <WiFiClient.h>
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
  Serial.println("---------------------");

  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();

    WiFiClient client;
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=0Q0VYIFXC41WJUDA";
    url += "&field1=" + String(temp);
    url += "&field2=" + String(humid);
    Serial.println("URL : " + url);
    Serial.println("Temperature : " + String(temp));
    Serial.println("Humidity : " + String(humid));
    http.begin(client, url);
    // http.addHeader("Content-Type", "application/json");

    // String json = "{\"api_key\":\"0Q0VYIFXC41WJUDA\",\"field1\":\"" + String(x) + "\",\"field2\":\"" + String(y) + "\"}";
    int httpResponseCode = http.POST("");
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  delay(10000); 
}
