//// Pour la plaquette Cociclo 0.3
//// Testing! testing des capteurs, intégration d'une DEL 'dotstar' aux autres capteurs.

int CO;

////////////// Configuration du capteur de température et humidité ////////////
#include "DHT.h"
#define DHTPIN D7     // what digital pin we're connected to the wemos
///#define DHTPIN 2    // what digital pin we're connected to huzzah
#define DHTTYPE DHT11   // DHT 11 (couleur bleue)
////////#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
///////////////////////////////////////////////////////////////////////////////

// DOTSTAR ///////////////////////////////////////////////
#include <Adafruit_DotStar.h>
#include <SPI.h>    // pour les dotstars
#define NUMPIXELS 1 // Number of LEDs in strip
#define DATAPIN    D2
#define CLOCKPIN   D3
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
 
 // uint32_t color1 = 0xCC3300; // naranja
 uint32_t color1 = 0x0000FF; // azul
 uint32_t color2 = 0xFF0000; // rojo
 
void setup() {

    Serial.begin(115200);
    ////////  DOTSTAR /////////////////////////////////////
    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP
    ////////////////////////////////////////// DOTSTAR ////

    dht.begin();  // intialisación del sensor de temperatura y humedad
}

void loop() {

    CO = analogRead(A0);
  // put your main code here, to run repeatedly:
  toneDeaf(D1,CO*2, 500);
  //noTone(D3);
  delay(500);
  toneDeaf(D1,CO*3, 500);
  //noTone(D3);
  delay(500);
  Serial.println("yep");
  
  // put your main code here, to run repeatedly:
        for(int i=0;i<NUMPIXELS+1;i++){
        strip.setBrightness(127); 
        strip.setPixelColor(i, color1); 
        strip.show(); }
        delay(25);
        for(int j=0;j<NUMPIXELS+1;j++){
        strip.setBrightness(127); 
        strip.setPixelColor(j, color2); 
        strip.show(); } 
          delay(25); 

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
}


void toneDeaf(uint8_t _pin, unsigned int frequency, unsigned long duration) {
pinMode (_pin, OUTPUT );
analogWriteFreq(frequency);
analogWrite(_pin,500);
delay(duration);
analogWrite(_pin,0);
}
