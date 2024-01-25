#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "AndroidAP017D";        // Your WiFi SSID
const char* password = "12345678";    // Your WiFi password

SoftwareSerial serialNodeMCU(D5, D6); // RX (D5), TX (D6)
//String receivedLetter = "modayaaaa";
WiFiClient client; // Create a WiFiClient instance
HTTPClient http;

void setup() {
  Serial.begin(9600);
  serialNodeMCU.begin(9600);
  WiFi.begin(ssid, password);
  /*while (WiFi.status() != WL_CONNECTED) {
    //delay(1000);
    //Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");*/
  
 
}

void loop() {
  if (serialNodeMCU.available()) {
   //if (1) {
    
    char receivedLetter = serialNodeMCU.read();
    Serial.print("Received letter: ");
    Serial.println(receivedLetter);

    //if (WiFi.status() == WL_CONNECTED) {

      // Specify the server and path for the GET request
      String url = "http://192.168.43.188:3000/api/data"; // Replace with your server URL
      Serial.print("Sending POST request...");
      http.begin(client, url); // Use the client instance as the first parameter
      http.addHeader("Content-Type", "application/json");
      String data = "{\"letter\":\"" + String(receivedLetter) + "\"}";
      int httpCode = http.POST(data); // Send the GET request
  
  //    if (httpCode > 0) {
  //      String response = http.getString(); // Get the response payload
  //      Serial.print("HTTP Response Code: ");
  //      Serial.println(httpCode);
  //      Serial.print("Response: ");
  //      Serial.println(response);
  //    } else {
  //      Serial.println("Error on HTTP request");
  //    }
  
      //http.end(); // Close the connection
  
      //delay(2000); // Wait for 5 seconds before sending the next request
    //} 
  }
}
