// Counts the ticks of each motor and adds the error value to the slower motor
// tickGoal is calculated based off of how far the motors go in one rotation and how many ticks there are on the encoder wheel (16)

#include "Grove_I2C_Motor_Driver.h"
 
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

// We need to use D2 and D3 here for the encoders
// because these pins are specifically for interrupts
const int encoderL = 3;
const int encoderR = 2;

int buttonPin = 5;

long lcount = 0;
long rcount = 0;

// interrupt vectors that correspond to pins 2 and 3
volatile unsigned int encoderLPos = 1;
volatile unsigned int encoderRPos = 0;

// change these values to match your motor orientation
unsigned char left = MOTOR2;
unsigned char right = MOTOR1;

void setup()
{
Motor.begin(I2C_ADDRESS);
pinMode(buttonPin, INPUT);
pinMode(encoderL,INPUT);
pinMode(encoderR,INPUT);

// Initializes interrupt for left and right encoder
// so that the main program can run in between ticks
attachInterrupt(1, countLeft, CHANGE);
attachInterrupt(0, countRight, CHANGE);

Serial.begin(9600);
}
 
void loop()
{
  // Wait for button to be pressed
  if (digitalRead(buttonPin) == HIGH) {
    delay(1000);
     moveDistance(30, 50); 
  }
}

// function to move robot forward or backward
void moveDistance(int inches, int motorPower) {

  // variables to save tick counts
  long prevLcount, prevRcount;
  long lDiff, rDiff;

  int leftPower = motorPower;
  int rightPower = motorPower;

  // the distance to travel / circumference * number of ticks on encoder wheel (16)
  int tickGoal = (inches / 8.54) * 16;

  // CHANGE THIS TO YOUR ERROR VALUE
  int error = 3;

  // Reset the tick counts to zero each time
  lcount = 0;
  rcount = 0;

  // Wait .1 seconds
  delay(100);

  Motor.speed(left, motorPower);
  Motor.speed(right, motorPower);

  // Counts the ticks for each wheel, saves that number as the previous count
  // and adjusts each motor's speed based off of the difference
  while (rcount < tickGoal || lcount < tickGoal) {

    Motor.speed(left, leftPower);   
    Motor.speed(right, rightPower);
    
    // Calculates the difference in previous and current tick counts
    lDiff = (lcount - prevLcount);
    rDiff = (rcount - prevRcount);

    // Saves the current tick count
    prevLcount = lcount;
    prevRcount = rcount;

      // If right motor is slow, it adds the error to the right motor power
      // and slows down the left motor
      if (lDiff > rDiff) {
        leftPower = leftPower - error;
        rightPower = rightPower + error;
      }

      // If left motor is slow, it adds the error to the left motor power
      // and slows down the right motor
      else if (lDiff < rDiff) {
        leftPower = leftPower + error;
        rightPower = rightPower - error;
      }

     // prints the counts for each wheel
     Serial.print(lcount);
     Serial.print("\t");
     Serial.print(rcount);
     Serial.print("\t");
     Serial.println(tickGoal);

      // Give the motors a chance to respond   
      delay(50);
  }

 // Stop the motors  
 Motor.stop(MOTOR1);
 Motor.stop(MOTOR2);
   
 delay(500);
}

// When the interrupt for one of the encoders
// is triggered, it adds one to the  count
void countLeft() {
  lcount++;
}

void countRight() {
  rcount++;
}

