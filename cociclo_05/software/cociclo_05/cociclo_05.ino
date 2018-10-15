#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <ESP8266HTTPClient.h> // for geolocalisation

#include <DNSServer.h> //needed for WiFiManager library
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson



//////  warm up du capteur ////////

// int lapause = 366600; // 6 minutes, 6 secondes et 6 millièmes de seconde de pause 
// int lapause =  1380000;  // 23 minutes x 60 secondes x 1000 millièmes de secondes = 1 380 000) nécessaire pour tous les modèles de capteurs afin d'éviter les fausses lectures.
int lapause = 10000;   // pour tester rapidement

///////////////////////////////////



///////////// SERVER HOST ////////////////////

const char* host = "www.cociclo.io"; 

/////////////////////////////////////////////


///////////// WIFIMANAGER //////////////////

bool shouldSaveConfig = false; //flag for saving data

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//define your default values here, if there are different values in config.json, they are overwritten.
char ID[30] = "";  /// un tableau de 30 caractères, valeur de cociclo par défaut
char storeLatitude[12] = "";
char storeLongitude[12] = "";

////////////////////////////////////////////



////////// DHT11  capteur de température et humidité /////

#include "DHT.h"
#define DHTPIN D7     // what digital pin we're connected to the wemos
#define DHTTYPE DHT11   // DHT 11 (capteur de couleur bleue)
DHT dht(DHTPIN, DHTTYPE);

float h = 42;
float t = 42;

//////////////////////////////////////////////////////////



////////////// PMS 3000 ///////////////////////////////

#include <Arduino.h>
#include <SoftwareSerial.h>

#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
//#define LENG 23   //0x42 + 23 bytes equal to 24 bytes
unsigned char buf[LENG];

int PM01Value=42;    //define PM1.0 value of the air detector module
int PM2_5Value=42;   //define PM2.5 value of the air detector module
int PM10Value=42;    //define PM10 value of the air detector module

SoftwareSerial PMSerial(D5, D6); // RX, TX

/////////////////////////////////////////////////////////


//////// Pour obtenir l'adresse MAC /////////

uint8_t MAC_array[6];  
char MAC_char[18];

/////////////////////////////////////////////



//////////////// MQ7 CO /////////////////////

float moyCO;
float maxCO;
//int lectures=50;
int lectures=2;
int i = 0;
float totalCO = 0;
int value = 0;
int reponse = 42;
int sensorPin = A0;    // select the input pin for the MQ7
int CO = 0;  // variable to store the value coming from the sensor

/////////////////////////////////////////////



////////////// Neopixel ////////////////////

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            D2
#define NUMPIXELS      1  /// 4
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 750; // delay for half a second

/////////////////////////////////////////



/////////// GEOLOCALISATION ///////////////////////

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyDXj6CqtjLdjzhUWLwQ-8bojvBYrdEfQ1w";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 0; // set to 1 for more debug output

////////////////////////////////////////////////////
