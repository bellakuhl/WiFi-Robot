#include "Grove_I2C_Motor_Driver.h"
#include <Average.h>

//IR sensor INPUT
const int dataIN = 3;

// To store time
unsigned long prevmillis;

// To store time difference
unsigned long duration;

int rpm;

// CHANGE THIS VALUE TO EITHER MOTOR1 OR MOTOR2
unsigned char motor = MOTOR1;

// initalizes array to get mode of 200 rpm values
Average<int> RPMavg(200);

// initalize counting of RPM values
int i = 0;

// current state of IR input scan
boolean currentstate;

// state of IR sensor in previous scan
boolean prevstate;

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

void setup()
{
  Motor.begin(I2C_ADDRESS);

  // initialize line finder as input
  pinMode(dataIN,INPUT);

  // start timer off at 0 and previous state at LOW
  prevmillis = 0;
  prevstate = LOW;

  // start Serial Communication
  Serial.begin(9600);
}

void loop()
{
  // read IR sensor state
  currentstate = digitalRead(dataIN);

  // begin motor
  Motor.speed(motor, 80);

  // check to see if there is change in input
 if( prevstate != currentstate)
   {
     // if input only changes from LOW to HIGH
     if( currentstate == HIGH )
       {
         // time difference between wedges times eight
         // because the detection goes from HIGH to LOW eight times per rotation
         duration = ( micros() - prevmillis ) * 8;

         // rpm = (1/ time millis)*1000*1000*60;
         rpm = (60000000/duration);

         // store time for next revolution calculation
         prevmillis = micros();

         // add rpm to array
         RPMavg.push(rpm);

         // increase count for each rpm value added
         i++;
       }
   }

  // store this new data for next scan
  prevstate = currentstate;

  // if 200 values are collected, print the mode of the array of values
  if (i == 200) {
    Serial.print("RPM for Motor "); Serial.print(motor); Serial.print(" : ");
    Serial.println(RPMavg.mode()); //display mode of rpm values

    // reset count
    i = 0;

    // clear saved rpm values
    RPMavg.clear();
  }
}
