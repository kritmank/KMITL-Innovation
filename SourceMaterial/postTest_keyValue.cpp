#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Set the POST data
  String postData = "deviceName=device00&sensor00=50&sensor01=50";

  // Send the POST request
  int httpResponseCode = http.POST(postData);

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
