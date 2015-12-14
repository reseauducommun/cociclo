
/* Adapté de WiFi UDP Send and Receive String
 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort
 Circuit:
 * WiFi shield attached
 created 30 December 2012
 by dlf (Metodo2 srl) */

#include <SPI.h>
#include <ESP8266WiFi.h> // changement pour pointer à la bonne librairie et pas la lib wifi du arduino
#include <WiFiUdp.h>

const char* ssid = "ovni";
const char* password = "alexestmondieu";
//const char* ssid = "Egils iPhone";
//const char* password = "egilbegil";
//const char* ssid = "Edvard";
//const char* password = "finefensalfin";
unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[50] = "42";       // a string to send back

String COS = "entree A0";
int CO = 13;

WiFiUDP Udp;

void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // delay(10000);
  
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {

  // Check if module is still connected to WiFi.
  if (WiFi.status() != WL_CONNECTED) {

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  
    // Print the new IP to Serial.
    printWiFiStatus();
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
   // Serial.println("Contents:");
   // Serial.println(packetBuffer);

  //send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    CO = analogRead(A0); // lecture du capteur
    Serial.println(CO);
  //  CO = map(CO,30,500,100,1000);
  //  Serial.println(CO);
    //geiger();
    //tone(0,CO,100);
    COS = CO; //conversion à string
    COS.toCharArray(ReplyBuffer, 50); // conversion à un buffer
    Udp.write(ReplyBuffer); // envoi du buffer
    Udp.endPacket();
 //   Serial.println("termine");
  }
}

void printWiFiStatus() {

  Serial.println("");
  Serial.print("Connecté à: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

  //// bips
      void tone(uint8_t _pin, unsigned int frequency, unsigned long duration) {
      analogWriteFreq(frequency);     //(frequency);
      analogWrite(_pin,500);
      //delay(duration);
      analogWrite(_pin,0);
      }
   

void geiger(){
 // Serial.println("geiger");
 // Serial.println(CO);

    if (CO < 150)
    {
      tone(0,CO,400); // sortida (PWM), variable, duración del 'pulse'
      delay(500);
    }
   else if (CO >= 150 && CO < 666)
      {
        for (int i=0;i<2;i++)
        {
        tone(0,CO,400); 
        delay(100);
        //tone(0,0,0);
        //delay(300);
        }
      }
} 
      //tone(0,0,0);
      //delay(1000);
   /*   
      
      else if (CO >= 230 && CO < 366)
      {
        for (int i=0;i<2;i++)
        {
        tone(0,CO,400); 
        delay(100);
        tone(0,0,0);
        delay(300);
        }
        delay(200);
    }
    else if (CO >= 366 && CO < 666 )
    {
    for (int i=0;i<3;i++)
        {
        tone(0,CO,400);
        delay(65);
        tone(0,0,0);
        delay(80);
              } 
              delay(400);
        }
      else
        {
        tone(0,CO,400); 
        delay(75);
        tone(0,0,0);
        delay(75);
      }
}
*/

