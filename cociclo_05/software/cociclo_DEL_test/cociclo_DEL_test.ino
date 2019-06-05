// DOTSTAR ///////////////////////////////////////////////
#include <Adafruit_DotStar.h>
#include <SPI.h>    // pour les dotstars
#define NUMPIXELS 1 // Number of LEDs in strip
#define DATAPIN    D2
#define CLOCKPIN   D3
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() {
  
  Serial.begin(115200);
    ////////  DOTSTAR /////////////////////////////////////
    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP
    ////////////////////////////////////////// DOTSTAR ////
}

void loop() {
        strip.setPixelColor(0,255,255,255);  // blanco
        strip.show(); 
         delay(50);       
}
  
