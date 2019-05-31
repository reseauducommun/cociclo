/****************************************
Example Sound Level Sketch for the
Adafruit Microphone Amplifier
****************************************/
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
void setup()
{
Serial.begin(9600);
}
void loop()
{
unsigned long startMillis= millis(); // Start of sample window
unsigned int peakToPeak = 0; // peak-to-peak level
unsigned int signalMax = 0;
unsigned int signalMin = 1024;
// collect data for 50 mS
while (millis() - startMillis < sampleWindow)
{
sample = analogRead(0);
if (sample < 1024) // toss out spurious readings
{
if (sample > signalMax)
{
signalMax = sample; // save just the max levels
}
else if (sample < signalMin)
{
signalMin = sample; // save just the min levels
}
}
}
//peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitudeSerial.
signalMax = signalMax-300; // dummy
signalMax = map(signalMax,0,130,60,95); // more dummy

Serial.print("signalMax : ");
Serial.println(signalMax);
//double volts = (peakToPeak * 5.0) / 1024; // convert to volts
//Serial.println(volts);
}
