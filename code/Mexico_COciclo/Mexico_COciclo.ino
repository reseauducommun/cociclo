int CO = 0;
int CO2 = 0;

void setup() {

Serial.begin(9600);
}

void loop() {

CO = (analogRead(A7)); leer el valor del sensor
Serial.print("CO : ");
Serial.println(CO);

CO2 = map(CO,80, 400, 150,666); // sustituir los valores en un nuevo intervalo para escuchar
Serial.print("CO2 : ");
Serial.println(CO2); //nuevo, 'CO2' es une chistosa
Serial.println();

////////////////////////////////////////////////////////////////////////
//                     Para la version Android                        //
////////////////////////////////////////////////////////////////////////

tone(5,CO2,400); // sortida (PWM 5 en este caso), variable, duración del pulse
delay(100);


////////////////////////////////////////////////////////////////////////
//                     Para la version con piezo                      //
////////////////////////////////////////////////////////////////////////
/*
 if (CO2 < 250)
  {
    tone(5,CO2,400); // sortida (PWM), variable, duración del 'pulse'
    delay(100);
    noTone(5);
    delay(1000);
    }
   else if (CO2 >= 250 && CO2 < 666)
    {
      for (int i=0;i<2;i++)
        {
        tone(5,CO2,400); 
        delay(100);
        noTone(5);
        delay(300);
        }
        delay(200);
    }
    else if (CO2 >= 666 && CO2 < 900)
    {
    for (int i=0;i<3;i++)
        {
        tone(5,CO2,400);
        delay(75);
        noTone(5);
        delay(100);
              } 
              delay(400);
        }
  else
    {
    tone(5,CO2,400); 
    delay(75);
    noTone(5);
    delay(75);
}
*/

}
