 int ledIndicator(int x){

////////// pas de connection au serveur ///////
  if (x==0) {
 Serial.println("indication : pas de serveur ");
  pixels.setPixelColor(0, pixels.Color(10,10,255)); // azul
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(1000);
  }

  else if (x==1) {  /// Tenemos una connexión al servidor
   
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

  }

  else if (x==2) {

    if (CO<45) {  
      Serial.println("verde");
      pixels.setPixelColor(0, pixels.Color(0,120,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      }
      else if (CO>=45 && CO <= 75) {
        Serial.println("amarillo");
        pixels.setPixelColor(0, pixels.Color(170,170,0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        else {
              Serial.println("rojo");
              pixels.setPixelColor(0, pixels.Color(170,20,0)); // Moderately bright green color.
              pixels.show(); // This sends the updated pixel color to the hardware.
              }
    
    }

    else if (x==3){
        pixels.setPixelColor(0, pixels.Color(20,127,80)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).
        pixels.setPixelColor(0, pixels.Color(127,127,80)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).
        }
  
 } 
