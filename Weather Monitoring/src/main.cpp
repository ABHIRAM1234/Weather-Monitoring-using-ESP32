
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include "ThingSpeak.h"

#define SECRET_SSID "mjvrangan"		
#define SECRET_PASS "ra8026582625"	
const int SensorPin = 34;
#define SECRET_CH_ID 1446876		//channel ID	
#define SECRET_WRITE_APIKEY "110W63NQ0O3JS2PM"  //API Key

char ssid[] = SECRET_SSID;   // my network SSID (name) 
char pass[] = SECRET_PASS;   // my network password
WiFiClient  client;

#define DHTPIN 15          // PIN 15 GPIO
#define DHTTYPE DHT22     // DHT 22

DHT_Unified dht(DHTPIN, DHTTYPE); //Class that stores state and functions for interacting with DHT_Unified.

uint32_t delayMS;
unsigned long myChannelNumber = SECRET_CH_ID; //storing channel ID as unsigned lont int
const char * myWriteAPIKey = SECRET_WRITE_APIKEY; //storing API Key as char

void setup(){

  Serial.begin(9600); //initialize device
  dht.begin(); //setup DHT sensor

  Serial.println(F("DHT22 test"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000; //setting delay between sensor readings
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); //initialize thingspeak
  
}

void loop(){

  //checking the status of WiFi Connection
  if(WiFi.status() != WL_CONNECTED){
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    }    
    Serial.println("\nConnected.");
  }

  delay(delayMS);
  //Connect or reconnect to WiFi
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temp = event.temperature;

  dht.humidity().getEvent(&event);
  float humidity = event.relative_humidity;
  float sensorValue = analogRead(SensorPin);

  ThingSpeak.setField(1,temp); //select field and value to write to channel
  ThingSpeak.setField(2,humidity);
  ThingSpeak.setField(3,sensorValue);
  //writing to Thingspeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); //delay between updating the channel */
}
