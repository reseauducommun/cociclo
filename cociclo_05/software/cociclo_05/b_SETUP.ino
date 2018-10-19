
void setup() {

  Serial.begin(115200);
  delay(100);


///// Format FS, uncomment for testing ////

 // SPIFFS.format();
 //Serial.println("formattage du FS");
  
///////////////////////////////////////////


///// read configuration from FS json //////

  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file config.json");
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
          strcpy(storeLatitude, json["latitude"]);
          strcpy(storeLongitude, json["longitude"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }

    else {Serial.println("config.json n'existe pas");
    }
    
  } else {
    Serial.println("failed to mount FS");
  }
//////// end filesystem read //////



////////  DOTSTAR /////////////////////////////////////
strip.begin(); // Initialize pins for output
strip.show();  // Turn all LEDs off ASAP
//////////////////////////////////////// DOTSTAR ////
  


////////  CONFIG  PMS 3000 /////////////

  // pinMode(D8,OUTPUT);
  // digitalWrite(D8,HIGH); // to put it in constant operation
  PMSerial.begin(9600);   
  PMSerial.setTimeout(1500);   
   
///////////////////////////////////////


///////////// Print de l'adresse MAC //////////

WiFi.macAddress(MAC_array);
    for (int i = 0; i < sizeof(MAC_array); ++i){
      sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
    }
  
    Serial.println(MAC_char);

////////////////////////////////////////////////


////////// WiFiManager /////////////////////////
  
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length

  WiFiManagerParameter custom_ID("ID", "ID", ID, 30);
  WiFiManagerParameter custom_latitude("latitude", "latitude", storeLatitude, 12);
  WiFiManagerParameter custom_longitude("longitude", "longitude", storeLongitude, 12);
  

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
   
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

////// pas de serveur /////

 ledIndicator(0); /// faire une fonction pour afficher qu'on a pas de connection au serveur

//////////////
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(300);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("cociclo")) {
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

ledIndicator(1);  // Tenemos una connexión al servidor

 /////////

  //read updated parameters
  
  strcpy(ID, custom_ID.getValue());
  strcpy(storeLatitude, custom_latitude.getValue());
  strcpy(storeLongitude, custom_longitude.getValue());
  
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("************ saving config ************");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
   
   json["ID"] = ID;
   json["latitude"] = latitude;
   json["longitude"] = longitude;
   

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("!!!!! failed to open config file for writing !!!!!");
    } 
    
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    
  } //end save

  // Serial.println("local ip");
  // Serial.println(WiFi.localIP());

   dht.begin();  // intialisación del sensor de temperatura y humedad
}

