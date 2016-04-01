#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

////////////// Configuration du capteur de température et humidité ////////////
#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
//#define DHTTYPE DHT11   // DHT 11 (couleur bleue)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
///////////////////////////////////////////////////////////////////////////////

const char* host = "www.cociclo.io"; // le serveur qui reçoit les données

//define your default values here, if there are different values in config.json, they are overwritten.
char ID[30];  /// un tableau de 30 caractères 
char latitude[12] = "42";
char longitude[12] = "42";

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

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
  //////wifiManager.addParameter(&custom_mqtt_server);
  //////wifiManager.addParameter(&custom_mqtt_port);
  //////wifiManager.addParameter(&custom_blynk_token);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

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
  Serial.println("connected...yeey :)");

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

   dht.begin();
}

int value = 0;
int sensorPin = A0;    // select the input pin for the MQ7
int CO = 0;  // variable to store the value coming from the sensor

void loop() {
  // put your main code here, to run repeatedly:
    delay(5000);
  ++value;

/*
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
*/

 ////////////////// Lecture du MQ-7 //////////////////

  CO = analogRead(sensorPin); 
  
  Serial.print("CO : ");
  Serial.println(CO);  
  

 ///////////////////////// Envoi au serveur ////////////////////
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
  //String url = "/php/connect2.php";
  //////String url = "/php/connect2.php?ID=huzzah&CO=42&Temp=42&Hum=42&Latitude=2&Longitude=3";

  String myID = ID;
  String mylatitude = latitude;
  String mylongitude = longitude;
  
  String url = "/php/connect2.php?ID=" + myID + "&CO=" + CO + "&t=" + 42 + "&h=" + 42 + "&La=" + mylatitude + "&Lo=" + mylongitude;
  /////////String url = "/php/connect2.php?ID=" + myID + "&CO=" + CO + "&t=" + t + "&h=" + h + "&La=" + mylatitude + "&Lo=" + mylongitude;
  //String url = "/php/connect2.php?ID=" + myID + "&CO=" + CO + "&Temp=" + t + "&Hum=" + h + "&Latitude=" + mylatitude + "&Longitude=" + mylongitude;
  Serial.print("Requesting URL: ");  
  Serial.println(url);

  Serial.print("ID: ");
  Serial.println(ID);

  Serial.print("Latitude: ");
  Serial.println(latitude);

  Serial.print("longitude: ");
  Serial.println(longitude);


  
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


}
