#include "WiFiEsp.h"
#include "SoftwareSerial.h"
#include "Grove_I2C_Motor_Driver.h"

//Create Serial communication Object
SoftwareSerial Serial1(8, 9); // RX, TX

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

//Wifi settings
char ssid[] = "My WiFi Robot";    // your network SSID (name)
char pass[] = "password";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

//Wifi server object
WiFiEspServer server(80);

// use a ring buffer to increase speed and reduce memory allocation
//RingBuffer buf(8);

void setup()
{
  //Start Serial Communication with computer and WiFi module
  Serial.begin(9600);   // initialize serial for debugging
  Serial1.begin(9600);    // initialize serial for ESP module
  WiFi.init(&Serial1);    // initialize ESP module

  Motor.begin(I2C_ADDRESS);    //start motor driver
  
  //Print out debugging messages
  Serial.print("Attempting to start AP ");
  Serial.println(ssid);

  // start access point
  status = WiFi.beginAP(ssid);

  // start the web server on port 80
  server.begin();
  IPAddress ip = WiFi.localIP();
  Serial.println("Server started");
  Serial.print("To see this page in action, connect to ");
  Serial.print(ssid);
  Serial.print(" and open a browser to http://");
  Serial.println(ip);
  Serial.println();
}


void loop()
{

  //Wait for a device to connect to the WiFi
  WiFiEspClient client = server.available();  // listen for incoming clients

    // if you get a client,
  if (client) {
    // print a message out the serial port
    Serial.println("New client");
      // loop while the client's connected
    while (client.connected()) {
      // if there's bytes to read from the client,
      if (client.available()) {      
          String request = client.readStringUntil('\r');
          client.flush();
           
          // Match the request
         
          if (request.indexOf("/fwd") != -1) {
            Motor.speed(MOTOR1, 100);
            Motor.speed(MOTOR2, 100);
            delay(1500);    // turn on both motors for 1.5 seconds
            Motor.stop(MOTOR1);
            Motor.stop(MOTOR2);
          } 
          if (request.indexOf("/bwd") != -1){
            Motor.speed(MOTOR1, -100);
            Motor.speed(MOTOR2, -100);
            delay(1500);    // turn on both motors in reverse for 1.5 seconds
            Motor.stop(MOTOR1);
            Motor.stop(MOTOR2);
          }
          if (request.indexOf("/left") != -1) {
            Motor.speed(MOTOR2, 100);
            delay(500);     // turn on right motor for .5 sec
            Motor.stop(MOTOR2);
          } 
          if (request.indexOf("/right") != -1){
            Motor.speed(MOTOR1, 100);
            delay(500);     // turn on left motor for .5 sec
            Motor.stop(MOTOR1);
          }
         
          // Return the response
          
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n");
          client.println();

          // CSS to style the on/off buttons
            
          client.print(F(
            "<!DOCTYPE HTML>\r\n"
            "<html>\r\n"
            "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n"
            "<link rel=\"icon\" href=\"data:,\">\r\n"
            "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n"
            ".button { background-color: #ff6666; border: none; color: black; padding: 16px 40px;\r\n"
            "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\r\n"
            ".button:hover {background: #eee;}\r\n"
            ".forward {background-color: #6699ff;}\r\n"
            ".backward {background-color: #6699ff;}\r\n"
            ".left {background-color: #6699ff;}\r\n"
            ".right {background-color: #6699ff;}</style></head>\r\n"
            "<h2>Controls</h2>\r\n"));
            
          client.print(F(
            "<button class=\"button forward\" onClick=location.href='/fwd'>forward</button>\r\n"
            "<p><button class=\"button left\" onClick=location.href='/left'>left</button>\r\n"
            "<button class=\"button right\" onClick=location.href='/right'>right</button></p>\r\n"
            "<button class=\"button backward\" onClick=location.href='/bwd'>backward</button>\r\n"));


          client.println("</body></html>");
          break;
      }
    }

    // give the web browser time to receive the data
    delay(10);
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

