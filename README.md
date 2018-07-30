## WiFi Enabled Robot
### This project is a WiFi enabled robot that uses Arduino and an ESP8266 to create a web server that can control the robot. 
This is my first project using an ESP8266. The code in this repo is used with a WiFi enabled robot that uses two DC motors attached to a laser cut chassis. The motors each have an encoder wheel and an IR module that acts as an optical encoder. The modules and the motor driver are from Seeed studios.

#### Average RPM
This code uses the optical encoders to calculate the time it takes for the wheels to rotate on click and then multiples that number by 16 because there are 16 ticks on each encoder wheel. This number is then saved into an array and when 50 values are collected it uses the mode (using the Average library) to output the RPM of the motor onto the Serial monitor. This allows the user to see the RPM of each motor.

#### Accounting for motor differences
This code uses interrupts to count the ticks of each motor. It then compares the previous tick count with the current tick count and adjusts the motor power of each motor to make the robot drive straight by either adding or subtracting a set error value. Some experimentation is required to get the best error value. The count of the left motor, right motor and tick goal are displayed on the serial monitor. The tick goal is gotten by dividing the distance you want your motors to go (in inches) by the circumference and multiplying by 16. Basically it calculates how far one tick will move the robot and then uses that to see how many ticks are needed to travel the inputted distance.

#### Control LEDs over WiFi with CSS
This code is a test sketch to add buttons to a webserver and control if an LED is on or off wirelessly. This is my first attempt at creating buttons on a webserver using CSS.

#### Control motors webserver
This code allows the user to control their robot over wifi. It uses an ESP866 to create an access point and then the user uses an internet enabled device to connect to the AP, naviagte to the IP address, and control the motors on the robot via WiFi

#### Run motors straight tickCount
This code was an attempt to drive the motors straight without using interrupts. It did not work.
