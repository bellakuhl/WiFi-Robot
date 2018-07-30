#include "WiFiEsp.h"
#include "SoftwareSerial.h"

//Create Serial communication Object
SoftwareSerial Serial1(8, 9); // RX, TX

int ledPin1 = 2;
String ledState1 = "OFF";
int ledPin2 = 3;
String ledState2 = "OFF";

//Wifi settings
char ssid[] = "Weather Node";    // your network SSID (name)
char pass[] = "password";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

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
  
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);

  //Print out debugging messages
  Serial.print("Attempting to start AP ");
  Serial.println(ssid);

  // start access point
  status = WiFi.beginAP(ssid);

  // start the web server on port 80
  server.begin();
  Serial.println("Server started");
}


void loop()
{

  //Wait for a device to connect to the WiFi
  WiFiEspClient client = server.available();  // listen for incoming clients

    // if you get a client,
  if (client) {
    // print a message out the serial port
    Serial.println("New client");
// initialize the circular buffer
    //buf.init();
      // loop while the client's connected
    while (client.connected()) {
      // if there's bytes to read from the client,
      if (client.available()) {
        // read a byte, then
        //char c = client.read();
        // push it to the ring buffer
        //buf.push(c);
        
          String request = client.readStringUntil('\r');
          //Serial.println(request);
          client.flush();
           
          // Match the request
         
          if (request.indexOf("/led1ON") != -1) {
            digitalWrite(ledPin1, HIGH);
            ledState1 = "ON";
          } 
          if (request.indexOf("/led1OFF") != -1){
            digitalWrite(ledPin1, LOW);
            ledState1 = "OFF";
          }
          if (request.indexOf("/led2ON") != -1) {
            digitalWrite(ledPin2, HIGH);
            ledState2 = "ON";
          } 
          if (request.indexOf("/led2OFF") != -1){
            digitalWrite(ledPin2, LOW);
            ledState2 = "OFF";
          }
         
          // Return the response
          
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            // the connection will be closed after completion of the response
            "Connection: close\r\n"
             // refresh the page automatically every .1 sec
            "Refresh: .1\r\n"
            "\r\n");

          // CSS to style the on/off buttons
            
          client.print(F(
            "<!DOCTYPE HTML>\r\n"
            "<html>\r\n"
            "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n"
            "<link rel=\"icon\" href=\"data:,\">\r\n"
            "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n"
            ".button { background-color: #ff6666; border: none; color: white; padding: 16px 40px;\r\n"
            "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\r\n"
            ".button2 {background-color: #6699ff;}</style></head>\r\n"));

          // Display current state, and ON/OFF buttons for LED 1
          
            client.println("<p>LED1 is currently " + ledState1 + "</p>");
            
            // If the LED1  is off, it displays the ON button  
                 
            if (ledState1 == "OFF") {
              client.println(F("<p><a href=\"/led1ON\"><button class=\"button\">Turn 1 ON</button></a></p>"));
            } else {
              client.println(F("<p><a href=\"/led1OFF\"><button class=\"button button2\">Turn 1 OFF</button></a></p>"));
            } 
           
           // Display current state, and ON/OFF buttons for LED 2  
           
            client.println("<p>LED2 is currently " + ledState2 + "</p>");
            
            // If LED 2 is off, it displays the ON button    
               
            if (ledState2 == "OFF") {
              client.println(F("<p><a href=\"/led2ON\"><button class=\"button\">Turn 2 ON</button></a></p>"));
            } else {
              client.println(F("<p><a href=\"/led2OFF\"><button class=\"button button2\">Turn 2 OFF</button></a></p>"));
            }

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

