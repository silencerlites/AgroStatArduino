int sensorValue = 0;
int percentValue = 0;

#include "DHT.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial ArduinoUno(3,2);

#define DHTPIN 7    // digital pin 2

#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  pinMode(A0,INPUT);
  Serial.begin(9600);
  ArduinoUno.begin(4800);
  // set up the LCD's number of columns and rows: 
  dht.begin();
}

void loop() {

  StaticJsonBuffer<1000>jsonBuffer;
  JsonObject& dataSen = jsonBuffer.createObject();
  sensorValue =analogRead(A0);
  dataSen["AnalogValue"] = sensorValue;
  percentValue = map(sensorValue, 675, 300, 0, 150);
  dataSen["PercentageValue"] = percentValue + "%";


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  dataSen["Humidity"] = h;
  dataSen["Temperature"] = t;
  dataSen["HeatindexC"] = hic;
  dataSen["HeatindexF"] = hif;

  
if(ArduinoUno.available()>0){
  dataSen.printTo(ArduinoUno);
}
     delay(5000);

  
 
  
}
