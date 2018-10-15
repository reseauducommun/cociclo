///////// DHT Température et humidité ///////////
 
 // Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

 void dhtLecture(){
  delay(2000);
   h = dht.readHumidity();
   // Read temperature as Celsius (the default)
   t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
 if (isnan(h) || isnan(t)) {
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

 }
  

