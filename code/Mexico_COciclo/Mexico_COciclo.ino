int CO = 0;
int CO2 = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

CO = (analogRead(A7));
Serial.println(CO);
CO2 = map(CO,100, 400, 150,666); // remap de l'ancienne valeur à quelque chose d'audible
Serial.println(CO2); //nuevo, le CO2 est une chistosa

if (CO2 < 250)
  {
    tone(5,CO2,400); // output pin (PWM), variable, duration of the pulse
    delay(100);
    noTone(5);
    delay(1000);// do Thing A
    }
   else if (CO2 >= 250 && CO2 < 666)
    {
      for (int i=0;i<2;i++)
        {
        tone(5,CO2,400); // output pin (PWM), variable, duration of the pulse
        delay(100);
        noTone(5);
        delay(300);// do Thing B
        }
        delay(200);
    }
    else if (CO2 >= 666 && CO2 < 900)
    {
    for (int i=0;i<3;i++)
        {
        tone(5,CO2,400); // output pin (PWM), variable, duration of the pulse
        delay(75);
        noTone(5);
        delay(100);
              } 
              delay(400);
      }
else
{
    tone(5,CO2,400); // output pin (PWM), variable, duration of the pulse
    delay(75);
    noTone(5);
    delay(75);// do Thing C
}

/////////////////////// Para la version Android ///////////////////////////////
/*
tone(5,CO2,400); // output pin (PWM), variable, duration of the pulse
delay(100);
noTone(5);
delay(500);
*/

}
