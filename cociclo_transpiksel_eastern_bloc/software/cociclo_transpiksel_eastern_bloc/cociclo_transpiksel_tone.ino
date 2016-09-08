void setup() {
  Serial.begin(115200);
}

void loop() {

  int CO = 13;

  CO = analogRead(A0); // lecture du capteur
  Serial.print("CO : ");
  Serial.println(CO);
  CO = map(CO,20,300,100,1000);
 
  Serial.print("CO remapped : ");
  Serial.println(CO);
 
////////////// GEIGER !!! //////////////

    if (CO < 120)
    {
      tone(2,CO,50); // sortida (PWM), variable freq, duración del 'pulse'
      delay(15);
      noTone(2);
      delay(4);
      CO = CO+50;
      tone(2,CO,70); // sortida (PWM), variable, duración del 'pulse'
      delay(15);
      noTone(2);
      delay(4);
      
      }
      else if (CO >= 120 && CO < 266)
      {
        for (int i=0;i<2;i++)
        {
          CO = CO*3;
        tone(2,1000,CO); //// était CO
        delay(75);
        noTone(2);
        tone(2,CO,400); 
        delay(35);
        noTone(2);
        
        delay(20); /// était 200
            /////////////////// MAKE ///// SOME ////// NOISE ////////
            for (int i=0;i<25;i++)
            {
            tone(2,70,40); // sortida (PWM), variable, duración del 'pulse'
            delay(15);
            noTone(2);
            delay(4);
            //CO = CO+50;
            tone(2,40,20); // sortida (PWM), variable, duración del 'pulse'
            delay(15);
            noTone(2);
            delay(4);
            
            tone(2,60,30); // sortida (PWM), variable, duración del 'pulse'
            delay(12);
            noTone(2);
            delay(4);
            }
            ///////////////////////////////////////////////////////
        }
        delay(20); //// était 200
    }
    else if (CO >= 266 && CO < 666 )
    {
    for (int i=0;i<3;i++)
        {
        tone(2,1200,CO);
        delay(65);
        noTone(2);
        delay(80);
          tone(2,900,30); 
        delay(25);
        noTone(2);
              } 

  /*  tone(2,CO,50); // sortida (PWM), variable, duración del 'pulse'
      delay(15);
      noTone(2);
      delay(4);
CO = CO+50;
     tone(2,CO,70); // sortida (PWM), variable, duración del 'pulse'
      delay(15);
      noTone(2);
      delay(4);
      */
              
              delay(400);
        }
      else
        {
        tone(2,2000,400); 
        delay(75);
        noTone(2);
        delay(50);
      }
 
 }  /// fin du loop



