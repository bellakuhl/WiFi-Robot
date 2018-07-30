#include "Grove_I2C_Motor_Driver.h"
 
//#define abs(X) ((X < 0) ? -1 * X : X)
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

const int encoderL = 4; //IR sensor INPUT
const int encoderR = 3;
int encoderTicks[] = {0,0};

boolean currentstateL; 
boolean currentstateR; 
boolean prevstateL; 
boolean prevstateR;

int masterPower = 100;
int slavePower = 100;

void setup()
{
Motor.begin(I2C_ADDRESS);
pinMode(encoderL,INPUT);
pinMode(encoderR,INPUT);
prevstateL = digitalRead(encoderL);
prevstateR = digitalRead(encoderR);
Serial.begin(9600);
}
 
void loop()
{
  //Distances specified in tenths of an inch.
 
  driveStraightDistance(30,60); 
  delay(1000);              //Stop in between to prevent momentum causing wheel skid.
  //driveStraightDistance(15,-100);
  //delay(500);
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  delay(500);
}

void driveStraightDistance(int inches, int masterPower)
{
  int tickGoal = (187 * inches) / 100;
  //This will count up the total encoder ticks despite the fact that the encoders are constantly reset.
  int totalTicks = 0;
 
  //Initialise slavePower as masterPower - 5 so we don't get huge error for the first few iterations. The
  //-5 value is based off a rough guess of how much the motors are different, which prevents the robot from
  //veering off course at the start of the function.
  int slavePower = masterPower + 5; 
 
  int error = 0;
 
  int kp = 3;
 
 
  //Monitor 'totalTicks', instead of the values of the encoders which are constantly reset.
  while(abs(totalTicks) < tickGoal)
  {
    //Proportional algorithm to keep the robot going straight.
    Motor.speed(MOTOR1, masterPower);
    Motor.speed(MOTOR2, slavePower);
    //Serial.print("masterpower: "); Serial.println(masterPower);
    //Serial.print("slavepower: "); Serial.println(slavePower);
    tickcount(encoderTicks, masterPower, slavePower);
    int leftEncoder = encoderTicks[0];
    Serial.print("lcount: "); Serial.println(leftEncoder);
    int rightEncoder = encoderTicks[1];
    Serial.print("rcount: "); Serial.println(rightEncoder);
 
    error = leftEncoder - rightEncoder;
 
    slavePower += error / kp;
 
    delay(100);
 
    //Add this iteration's encoder values to totalTicks.
    totalTicks+= leftEncoder;
  }
 Motor.stop(MOTOR1);
 Motor.stop(MOTOR2);  
 delay(500);
}

void tickcount(int encoderTicks[], int masterpower, int slavepower) {
  int lcount = 0;
  int rcount = 0;
  encoderTicks[0] = 0;
  encoderTicks[1] = 0;
  unsigned long start = millis();
  unsigned long endtime = start;
  while ((endtime - start) <= 500) {
    currentstateL = digitalRead(encoderL);
    currentstateR = digitalRead(encoderR);
    Motor.speed(MOTOR1, masterpower);
    Motor.speed(MOTOR2, slavepower);
   if( prevstateL != currentstateL)
     {
      encoderTicks[0] = lcount++;
      prevstateL = currentstateL;
     }
   if( prevstateR != currentstateR)
     {
      encoderTicks[1] = rcount++;
      prevstateR = currentstateR;
     }
  endtime = millis();
  }
}

