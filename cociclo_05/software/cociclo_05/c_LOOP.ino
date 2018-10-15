void loop() {

////////////// Délai nécessaire pour que les valeurs du capteur soient valides (débute à 1023) ////
Serial.println("on fait une pause pour réchauffer le capteur");
/// FAIRE LES CHECKS DE POSITION ICI !! /////////

geolocalisation(); // fait une lecture de géolocalisation 

/// SAUVEGARDER LES VALEURS DS LAT ET LONG //////

////////// WARM UP !! //////

 delay(lapause);
 
///////////////////////////


 while(reponse==42) {  ///// autrement dit toujours
    
    delay(5815);  // pour arriver à un délai de 6 secondes
  ++value;


///// LECTURE DU  PMS 3000 /////

pmsSensorRead();

////////////////////////////////
    

///// Lecture du MQ-7 //////////

lectureCO();


///////// DHT LECTURE ////////////

dhtLecture();


//////// LED FEEDBACK //////////

ledIndicator(2); // varie selon les valeurs de CO


 //////////// Envoi au serveur /////////////

  if(i >= lectures){ /// après n lectures envoyer au serveur

   Serial.print("lectures : ");
   Serial.println(i);

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
  String mylatitude = String(latitude,7);
  String mylongitude = String(longitude,7);
  //double mylatitude = latitude;
  //double mylongitude = longitude;
  
  String myMAC_char = MAC_char;  /// doit convertir en string pour l'envoi
  // String url = "/php/connect2.php?MAC_char="+myMAC_char+"&ID=" + myID + "&CO=" + moyCO + "&MCO=" + maxCO + "&t=" + t + "&h=" + h + "&La=" + mylatitude + "&Lo=" + mylongitude;
  String url = "/php/connect4.php?MAC_char="+myMAC_char+"&ID=" + myID + "&CO=" + moyCO + "&MCO=" + maxCO + "&PM01=" + PM01Value + "&PM25=" + PM2_5Value + "&PM10=" + PM10Value + "&t=" + t + "&h=" + h + "&La=" + mylatitude + "&Lo=" + mylongitude;

  // String url = "/php/connect2.php?ID=" + myID + "&CO=" + moyCO + "&t=" + 42 + "&h=" + 42 + "&La=" + mylatitude + "&Lo=" + mylongitude;

  ///// ce qui suit est nécessaire pour tester mais pas pendant l'exécution du programme //////
 
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
    Serial.println(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

///////// Feedback neopixel données envoyées ////////

  ledIndicator(3);

////////////////////////////////

  
  i= 0;  /// remise à zéro du compteur
  totalCO = 0; //// remise à zéro du total 
  maxCO = 0;  /// remise du max à zéro
    
    } /// fin du if pour l'envoi

    
  }  /// fin du while

} /// fin du loop
