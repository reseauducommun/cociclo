/* Cociclo es un circuito simple de sensor medioambiental
Cuidado!! Version 0.1 !! El circuito en fritzing tiene errores y me voy a hacerlo en Eagle.

copyright Alexandre Castonguay
License GPL 2.0

Puedes connectar un tri color DEL (pins 9,10,11) y un piezo elemento

*/

void setup() { 
    pinMode(9,OUTPUT); // on
    pinMode(10,INPUT); // mas de 500
    pinMode(11,INPUT);  // mas de 750   
}

void loop() {
  // read the sensor:
  int sensorValue = analogRead(A0); // très simple pour lire la valeur du senseur
  if (sensorValue < 100)
  {
   digitalWrite(9, HIGH);    // affiche une couleur différente selon le degré de pollution 
   digitalWrite(10, HIGH); 
   digitalWrite(11, HIGH); 
   
  }
else if (sensorValue > 300)
  {
   digitalWrite(9, HIGH); 
   digitalWrite(10, LOW); 
   digitalWrite(11, HIGH); 
  }
else
  {
  digitalWrite(9, HIGH); 
  digitalWrite(10, HIGH); 
  digitalWrite(11, LOW); 
  }  
  
  // Fait correspondre le sensovalue (0,1023) à la gamme de fréquences en sortie (200,6000)
  int thisPitch = map(sensorValue, 0, 1023, 200, 6000);
  // Jouer la fréquence
  tone(3, thisPitch, 1000);
  // Test, envoyer un 'la' 440Hz
  // tone(3, 440, 1000);

  delay(100);        // delai entre les lectures pour une stabilité
}


