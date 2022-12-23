#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"
  
const char* ssid = "ssid";
const char* password =  "passwd";

#define DHTTYPE DHT11  
uint8_t DHTPin = 27; 

DHT dht(DHTPin, DHTTYPE);  

float Temperature;
 
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

  // Handle clients connecting to web socket
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  
  if(type == WS_EVT_CONNECT){
  
    Serial.println("Websocket client connection received");
     
  } else if(type == WS_EVT_DISCONNECT){
 
    Serial.println("Client disconnected");
  
  }
}
  
void setup(){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);

   // Connect to local network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  pinMode(DHTPin, INPUT);

  dht.begin();    
  
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  
  server.begin();
}
  
void loop(){
  // Read temperature every 4 seconds and send it to clients
  Temperature = dht.readTemperature();
  String temp = String(Temperature);
  ws.textAll(temp);
 
  delay(4000);
}
