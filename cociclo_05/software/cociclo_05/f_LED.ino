 
 int ledIndicator(int x){

////////// pas de connection au serveur ///////
  if (x==0) {

    Serial.println("pas de serveur ");
   /* while(){
      nonBlockingOsXAnimation();
    }*/  
  for (int i=0;i<255;i=i+10){
  strip.setBrightness(i); 
  strip.setPixelColor(0, color0); // blanco
  strip.show(); 
  delay(50); 
  }
  for (int i=255;i>0;i=i-10){
  strip.setBrightness(i); 
  strip.setPixelColor(0, color0); // blanco
  strip.show(); 
  delay(50); 
  }
   
    delay(1000);
  }
    

  else if (x==1) {  /// Tenemos una connexión al servidor
   
  Serial.println("connectés ");
  strip.setBrightness(200); 
  strip.setPixelColor(0, color1); // azul?
  strip.show(); 
  delay(1000);

  }

  else if (x==2) {

    if (CO<45) {  
      Serial.println("verde");
      strip.setBrightness(200); 
      strip.setPixelColor(0, color4); 
      strip.show(); 
  delay(1000);
      }
      else if (CO>=45 && CO <= 75) {
        Serial.println("amarillo");
        strip.setBrightness(200); 
        strip.setPixelColor(0, color5); 
        strip.show(); 
        }
        else {
              Serial.println("rojo");
              strip.setBrightness(200); 
              strip.setPixelColor(0, color3); 
              strip.show(); 
              }
    
    }

    else if (x==3){
      Serial.println("naranja");
      strip.setBrightness(200); 
      strip.setPixelColor(0, color2); 
      strip.show(); 
        }
  
 } 

  void nonBlockingOsXAnimation() { // merci Thomas

      byte grey = floor(sin(millis() * 0.005 - PI * 0.5) * 32 + 32); // half intensity
      strip.setBrightness(grey); 
      strip.setPixelColor(0, color0); // blanco
      strip.show(); 

    }
