#include <Arduino.h>
const int ledPin[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//Interrupt is mapped to pin 2
const int leftPin = 0; //Analog 0
const int rightPin = 1; //Analog 1
int leftVol = 0; // Voltage of the left channel
int leftAudio = 0; //Calibrated volume of the left channel
int startTime = 0; // Time variables used for callibration
int currentTime = 0;
int sumVols = 0; //Sum of the voltages (over 1s interval) Used for calibration
int totalVols = 0; //Total voltage samlpes, to calculate average.
int avgVols = 0; //verage voltage. Used for calibration

volatile float tempCalibration = 2; // Temporary Calibration factor.  
volatile float calibration = 2; // Volume calibration factor
int lowCounter = 0;

void setup() {
  
  attachInterrupt(0, Interrupt, HIGH); //Interrupt 0 is mapped to pin 2, signal an interrupt on a change to pin 2
  
  for (int i=0; i<10;i++){
    pinMode(ledPin[i], OUTPUT);   
  }
  
  Serial.begin(9600);
}
void loop() {
  // Ensure all LEDs work
  /*for (int i=0; i<10;i++){
    digitalWrite(ledPin[i], HIGH);   
  }*/
  //Serial.println (leftAudio);
  
  // Read & print input
  leftVol = analogRead(leftPin);

  //Calibrate leftAudio for output
  leftAudio = floor(leftVol/calibration);
 
  //Illuminate the appropriate LEDs
  for (int i=0; i<=leftAudio; i++){
    digitalWrite(ledPin[i], HIGH);
  }
 
  //Serial.println(calibration);
  
  //turn off lights above
  for (int i=leftAudio; i<10; i++){
    digitalWrite(ledPin[i], LOW);   
  }
  
   // Implement Callibration algorithm;
 //--
    setCalibration();
 //--
  
}

//------------------------------------------------------------------

void setCalibration() {
  
  if (totalVols == 0) {
   startTime = millis(); 
  }
  
  currentTime = millis();
  totalVols++; //Total voltage samples
  sumVols += leftVol; //Total sum
  
  // Calculate the average voltage every second
  if  (currentTime - startTime >= 1000)
  {
    avgVols = (float)sumVols/(float)totalVols;
    
    // We want this average voltage to be Level 6 on the VU Meter
    tempCalibration = avgVol/6;
  
    //Reset total & sum vols
    totalVols = 0;
    sumVols = 0;
  }
}
//------------------------------------------------------------------

void Interrupt() {
  calibration = tempCalibration;
}
