#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "PageIndex.h"; //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file
#define ServoPort D1   //--> Defining Servo Port

const char* ssid = "AETOS";  // your SSID
const char* password = "9176673062"; //WIFI Password

Servo myservo;  //--> create servo object to control a servo
ESP8266WebServer server(80);  //--> Server on port 80

void handleRoot() 
{
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

//Procedure for handling servo control

void handleServo()
{
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane","");
}


void setup() 
{
  Serial.begin(115200);
  delay(500);
  myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo object

  WiFi.begin(ssid, password);
  Serial.print("Connect your wifi laptop/mobile phone to this NodeMCU Access Point : ");
  Serial.println(ssid);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println(".");
  }
  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  //Initialize Webserver
  server.on("/",handleRoot);  //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS",handleServo); //--> Sets servo position from Web request
  server.begin();  
  Serial.println("HTTP server started");
}

void loop() 
{
 server.handleClient();
}