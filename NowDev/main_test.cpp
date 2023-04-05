#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "MAKONOI"; //WiFi Name
const char* password = "12345678"; //WiFi Password
const char* key = "0Q0VYIFXC41WJUDA";
int delayTime = 20 * 1000; //10s
int x = 0;
int y = 0;

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


    WiFiClient client;
    HTTPClient http;
    String url = "http://api.thingspeak.com/update";

    StaticJsonDocument<200> doc;
    doc["api_key"] = key;
    doc["field1"] = x;
    doc["field2"] = y;

    String json;
    serializeJson(doc, json);
    Serial.println("URL : " + url);
    Serial.println("Json : " + json);
    Serial.println("X : " + String(x));
    Serial.println("Y : " + String(y));
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    x++;
    y--;

    // String json = "{\"api_key\":\"0Q0VYIFXC41WJUDA\",\"field1\":\"" + String(x) + "\",\"field2\":\"" + String(y) + "\"}";
    int httpResponseCode = http.POST(json);
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