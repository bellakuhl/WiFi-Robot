#include "Grove_I2C_Motor_Driver.h"
#include <Average.h>

const int dataIN = 4; //IR sensor INPUT
unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference

int rpm; // RPM value
unsigned char motor = MOTOR1; // CHANGE THIS VALUE TO EITHER MOTOR1 OR MOTOR2
Average<int> RPMavg(10); // Initalizes array to get mode of 10 rpm values
int i = 0; // initalize counting of RPM values

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

void setup()
{
  Motor.begin(I2C_ADDRESS);
  pinMode(dataIN,INPUT);       
  prevmillis = 0;
  prevstate = LOW;  
  Serial.begin(9600);
}

void loop()
{
  
  currentstate = digitalRead(dataIN); // Read IR sensor state
  Motor.speed(motor, 50);
  
 if( prevstate != currentstate) // If there is change in input
   {
     //if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       //{
         duration = ( micros() - prevmillis ) * 16; // Time difference between wedges times four because the color alternates 4 times each rotation
         rpm = (60000000/duration); // rpm = (1/ time millis)*1000*1000*60;
         prevmillis = micros(); // store time for next revolution calculation
         RPMavg.push(rpm); // add rpm to array
         i++; // increase count for each rpm value added
       //}
   }
  prevstate = currentstate; // store this new data for next scan

  if (i == 10) {
    Serial.print("RPM for Motor "); Serial.print(motor); Serial.print(" : "); 
    Serial.println(RPMavg.mode()); //display mode of rpm values
    i = 0; // reset count
    RPMavg.clear(); // clear saved rpm values
    delay(5000);
  }
}
