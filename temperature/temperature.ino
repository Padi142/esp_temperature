#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"
#include <Arduino_JSON.h>

// Define what type of temperature sensor are we using
#define DHTTYPE DHT11   // DHT 11

// Set the credentials to your local network
const char* ssid = "ssid";  // Enter SSID here
const char* password = "password";  //Enter Password here

WebServer server(80);

// DHT Sensor
uint8_t DHTPin = 27; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local  network
  WiFi.begin(ssid, password);

  //Wait untill the device connect
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  // Define webserver routes
  server.on("/", handleOnDefaultRoute);
  server.on("/temp", handleOnTemperatureRead);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  
  server.handleClient();
  
}

void handleOnDefaultRoute() {
  server.send(200, "application/text", "ok"); 
}

void handleOnTemperatureRead() {

 Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  JSONVar myArray;
  myArray[0] = Temperature;
  myArray[1] = Humidity;

  // Makes json from data
  String jsonString = JSON.stringify(myArray);
  server.send(200, "application/json", jsonString); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
