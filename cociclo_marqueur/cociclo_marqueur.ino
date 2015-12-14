
/// C'est une carte Leonardo
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

int CO = 0;

  void setup() {
    Serial.begin(9600);
    Serial.println("16 channel Servo test!");
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
    }

  void setServoPulse(uint8_t n, double pulse) {
    double pulselength;
  
    pulselength = 1000000;   // 1,000,000 us per second
    pulselength /= 60;   // 60 Hz
    // Serial.print(pulselength); Serial.println(" us per period"); 
    pulselength /= 4096;  // 12 bits of resolution
    // Serial.print(pulselength); Serial.println(" us per bit"); 
    pulse *= 1000;
    pulse /= pulselength;
    //  Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
    }

  void loop() {
    CO = analogRead(2);
    CO = map(CO,80, 400, 200,666);
    Serial.print("CO : ");
    Serial.println(CO);
    Serial.print("A1 : ");
    Serial.println(analogRead(1));
    Serial.print("A0 : ");
    Serial.println(analogRead(0));
    //geiger(); // il faudrait mettre l'appel aux moteurs dans un interrupt car la vitesse de réponse est trop basse avec geiger()

      if(CO<184 && analogRead(0)<355){  
        masGisBlanca();
        }
        else {
        paraGisBlanca(); 
      }

    if(CO>=184 && analogRead(1)<237){
        masGisAmarilla();
        }
        else {
        paraGisAmarilla();
      }
 
    //delay(200);
    //pwm.setPWM(0, 0, 2000); // premier servo // anti-horaire rapide, 1000 à 450 aussi
    //pwm.setPWM(0,0,320); // horaire lent 
    //pwm.setPWM(1, 0, 430); // anti-horaire lent 400, 390 = jitter
    //Serial.println("recule");
    //delay(5000);
    // pwm.setPWM(0, 0, 400); // horaire lent 370, 200. 100, 50, 10 aussi
    //pwm.setPWM(0, 0, 0); // arrête tout
    Serial.println("------------");
  }

  
  void masGisBlanca(){
    Serial.println("AVANCE BLANC");
    digitalWrite(13, HIGH);
    pwm.setPWM(0, 0, 430); // anti-horaire lent 400, 390 = jitter
    pwm.setPWM(1,0,330); // horaire lent
    delay(100);
    }

  void paraGisBlanca(){ 
    Serial.println("ARRET BLANC");
    pwm.setPWM(0, 0, 0); // stop
    pwm.setPWM(1, 0, 0); // stop
    digitalWrite(13, LOW);
    delay(100);
    }

  void masGisAmarilla(){
    Serial.println("AVANCE Amarilla");
    digitalWrite(12, HIGH);
    pwm.setPWM(2, 0, 430); // anti-horaire lent 400, 390 = jitter
    pwm.setPWM(3,0,330); // horaire lent
    delay(100);
    }

  void paraGisAmarilla(){ 
    Serial.println("ARRET Amarilla");
    pwm.setPWM(2, 0, 0); // stop
    pwm.setPWM(3, 0, 0); // stop
    digitalWrite(12, LOW);
    delay(100);
    }

  void geiger(){
    if (CO < 230)
    {
      tone(5,CO,400); // sortida (PWM), variable, duración del 'pulse'
      delay(50);
      noTone(5);
      delay(500);
      }
      else if (CO >= 230 && CO < 366)
      {
        for (int i=0;i<2;i++)
        {
        tone(5,CO,400); 
        delay(75);
        noTone(5);
        delay(200);
        }
        delay(200);
    }
    else if (CO >= 366 && CO < 666 )
    {
    for (int i=0;i<3;i++)
        {
        tone(5,CO,400);
        delay(65);
        noTone(5);
        delay(80);
              } 
              delay(400);
        }
      else
        {
        tone(5,CO,400); 
        delay(75);
        noTone(5);
        delay(75);
      }
  }

