#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for WiFiManager library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson


////////////// Configuration du capteur de température et humidité ////////////
#include "DHT.h"
#define DHTPIN D7     // what digital pin we're connected to the wemos
///#define DHTPIN 2    // what digital pin we're connected to huzzah
#define DHTTYPE DHT11   // DHT 11 (couleur bleue)
////////#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
///////////////////////////////////////////////////////////////////////////////

//////  warm up du capteur ////////
// int lapause = 366600; // 6 minutes, 6 secondes et 6 millièmes de seconde de pause 

int lapause =  1380000;  // 23 minutes x 60 secondes x 1000 millièmes de secondes = 1 380 000) nécessaire pour tous les modèles de capteurs afin d'éviter les fausses lectures.
//int lapause = 0;   // pour tester rapidement

const char* host = "www.cociclo.io"; // le serveur qui reçoit les données

//define your default values here, if there are different values in config.json, they are overwritten.
char ID[30] = "";  /// un tableau de 30 caractères, valeur de cociclo par défaut
char latitude[12] = "";
char longitude[12] = "";

//////////////// Pour obtenir l'adresse MAC ///////////

uint8_t MAC_array[6];  
char MAC_char[18];

//////////// Pour le calcul du maximmum et des moyennes de CO /////

float moyCO;
float maxCO;
int lectures=100;
int i = 0;
float totalCO = 0;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

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


void setup() {
   pixels.begin(); // This initializes the NeoPixel library.
  
  Serial.begin(115200);
  delay(100);
  
  Serial.println();

//clean FS, for testing ////// CUIDADO  ///////
// SPIFFS.format();
//////////////////////////////

  //read configuration from FS json
  Serial.println("mounting FS...");
  

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");


       strcpy(ID, json["ID"]);
       strcpy(latitude, json["latitude"]);
       strcpy(longitude, json["longitude"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  

///////////// Print de l'adresse MAC

WiFi.macAddress(MAC_array);
    for (int i = 0; i < sizeof(MAC_array); ++i){
      sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
    }
  
    Serial.println(MAC_char);

  /////////////////////////////////////////////
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length

  WiFiManagerParameter custom_ID("ID", "ID", ID, 30);
  WiFiManagerParameter custom_latitude("latitude", "latitude", latitude, 12);
  WiFiManagerParameter custom_longitude("longitude", "longitude", longitude, 12);
  

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_ID);
  wifiManager.addParameter(&custom_latitude);
  wifiManager.addParameter(&custom_longitude);

 ////// pas de serveur /////
 Serial.println("indication couleur");
  pixels.setPixelColor(0, pixels.Color(10,10,255)); // azul
   pixels.show(); // This sends the updated pixel color to the hardware.

   delay(1000);
     pixels.setPixelColor(0, pixels.Color(10,10,150)); // azul
   pixels.show(); // This sends the updated pixel color to the hardware.

delay(1000);
/*
   pixels.setPixelColor(1, pixels.Color(150,10,150)); // azul
   pixels.show(); // This sends the updated pixel color to the hardware.

  
delay(1000); 

  pixels.setPixelColor(2, pixels.Color(150,150,10)); // azul
   pixels.show(); // This sends the updated pixel color to the hardware.

  
delay(1000);

 pixels.setPixelColor(3, pixels.Color(100,50,150)); // azul
   pixels.show(); // This sends the updated pixel color to the hardware.

  
delay(1000);
*/
//////////////
   
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(300);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("cociclo", "placecommune")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);

  
  }


  //if you get here you have connected to the WiFi
  Serial.println("connected...bravo!! :)");

 /////////////////////////////////////////////////////
 ////// Feedback neopixel, devrait changer à vert ////
 ////// Tenemos una connexión a el servidor

   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,60,123)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(60,60,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }

  /////////

  //read updated parameters
  
  strcpy(ID, custom_ID.getValue());
  strcpy(latitude, custom_latitude.getValue());
  strcpy(longitude, custom_longitude.getValue());
  
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
   
   json["ID"] = ID;
   json["latitude"] = latitude;
   json["longitude"] = longitude;
   

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

   dht.begin();  // intialisación del sensor de temperatura y humedad
}

int value = 0;
int  reponse = 42;
int sensorPin = A0;    // select the input pin for the MQ7
int CO = 0;  // variable to store the value coming from the sensor

void loop() {
  // put your main code here, to run repeatedly:
    
    ////////////// Délai nécessaire pour que les valeurs du capteur soient valides (débute à 1023) ////

 delay(lapause);
  ////////////////////

 while(reponse==42) {  ///// autrement dit toujours
    
    delay(5815);  // pour arriver à un délai de 6 secondes
  ++value;

 ////////////////// Lecture du MQ-7 //////////////////

  CO = analogRead(sensorPin); // on prends les valeurs ici!!
    
  Serial.print("CO : ");
  Serial.println(CO); 


//////// NEOPIXEL FEEDBACK /////

if (CO<45)
   {  
    Serial.println("verde");
    pixels.setPixelColor(0, pixels.Color(0,120,0)); // Moderately bright green color.
     pixels.show(); // This sends the updated pixel color to the hardware.
   }
   else if (CO>=45 && CO <= 75) {
     Serial.println("amarillo");
     pixels.setPixelColor(0, pixels.Color(170,170,0)); // Moderately bright green color.
       pixels.show(); // This sends the updated pixel color to the hardware.
    }

    else  {
  Serial.println("rojo");
     pixels.setPixelColor(0, pixels.Color(170,20,0)); // Moderately bright green color.
       pixels.show(); // This sends the updated pixel color to the hardware.
    }


  maxCO = _max(CO,maxCO); // le "_" devant le max est nécessaire parce que la fonction max interfère avec une autre fonction de c++ sur le esp8266
  
  totalCO = CO+totalCO; // Calcul de la moyenne 
  moyCO = totalCO/lectures;

  i++;

 ///////////////////////// Envoi au serveur ////////////////////

  if(i >= lectures){ /// après 100 lectures envoyer au serveur

  

 //////////////////////// DHT Température et humidité ///////////////
 // Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidité: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Température: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println();
 /////////////////////////////////////////////////////////////// 
    
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request

  String myID = ID;
  String mylatitude = latitude;
  String mylongitude = longitude;
  String myMAC_char = MAC_char;  /// doit convertir en string pour l'envoi
  
   

  String url = "/php/connect2.php?MAC_char="+myMAC_char+"&ID=" + myID + "&CO=" + moyCO + "&MCO=" + maxCO + "&t=" + t + "&h=" + h + "&La=" + mylatitude + "&Lo=" + mylongitude;
  // String url = "/php/connect2.php?ID=" + myID + "&CO=" + moyCO + "&t=" + 42 + "&h=" + 42 + "&La=" + mylatitude + "&Lo=" + mylongitude;
  /////////String url = "/php/connect2.php?ID=" + myID + "&CO=" + CO + "&t=" + t + "&h=" + h + "&La=" + mylatitude + "&Lo=" + mylongitude;
  //String url = "/php/connect2.php?ID=" + myID + "&CO=" + CO + "&Temp=" + t + "&Hum=" + h + "&Latitude=" + mylatitude + "&Longitude=" + mylongitude;

 ///// ce qui suit est nécessaire pour tester mais pas pendant l'exécution du programme //////x
 
  Serial.print("Requesting URL: ");  
  Serial.println(url);

  Serial.print("MAC address: ");
  Serial.println(MAC_char);

  Serial.print("ID: ");
  Serial.println(ID);

  Serial.print("maxCO: ");
  Serial.println(maxCO);  

  Serial.print("Latitude: ");
  Serial.println(latitude);

  Serial.print("longitude: ");
  Serial.println(longitude);
  

////////////////////////////
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  ///////// Feedback neopixel données envoyées ////////

  pixels.setPixelColor(0, pixels.Color(20,127,80)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
  pixels.setPixelColor(0, pixels.Color(127,127,80)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
  

  i= 0;  /// remise à zéro du compteur
  totalCO = 0; //// remise à zéro du total 
  maxCO = 0;  /// remise du max à zéro
    
    } /// fin du if pour l'envoi

    
  }  /// fin du while

} /// fin du loop
