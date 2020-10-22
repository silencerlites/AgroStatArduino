/*
 * HTTP Client POST Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */ 
//SOIL
int sensorValue = 0;
int percentValue = 0;

#include "DHT.h"
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


SoftwareSerial NodeMCU(D2,D3);

/* Set these to your desired credentials. */
const char *ssid = "Silencer";  //ENTER YOUR WIFI SETTINGS
const char *password = "qwerty123";

//Web/Server address to read/write from 
const char *host = "192.168.43.128";   //https://circuits4you.com website or IP address of server

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(9600);
  NodeMCU.begin(4800);
  while (!Serial) continue;
  
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& dataSen = jsonBuffer.parseObject(NodeMCU);
  if (dataSen == JsonObject::invalid())
    return;
  HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, postData;
  int adcvalue=analogRead(A0);  //Read Analog value of LDR
  ADCData = String(adcvalue);   //String to interger conversion
  station = "A";

  //Post Data
  int AnalogValue=dataSen["AnalogValue"];
  int PercentageValue=dataSen["PercentageValue"];
  int Humidity=dataSen["Humidity"];
  int Temperature=dataSen["Temperature"];
  int HeatindexC=dataSen["HeatindexC"];
  int HeatindexF=dataSen["HeatindexF"];
  postData = "status=" + ADCData + "&station=" + station + "&anaV=" + AnalogValue + "&perV=" + PercentageValue + "&humV=" + Humidity + + "&Temperature=" + Temperature + "&HeatInC=" + HeatindexC + "&HeatInF=" + HeatindexF;
  
  http.begin("http://192.168.43.199:3000/api/remote");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println("status: "+ httpCode);   //Print HTTP return code
  Serial.println("payload: "+ payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
//=======================================================================
