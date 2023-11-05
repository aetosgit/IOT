/*
* Ultrasonic Sensor HC-SR04 interfacing with  ESP32 
*/

/* Define the names for ESP32 pin for HC-SR04*/
#define trigger_pin 5
#define Echo_pin 18
#define LED 2

long duration;
int distance;

void setup() {
pinMode(trigger_pin, OUTPUT); 
pinMode(LED, OUTPUT); 
pinMode(Echo_pin, INPUT); 
Serial.begin(9600); 
}

void loop() {
  
digitalWrite(trigger_pin, LOW); 
delayMicroseconds(2);

digitalWrite(trigger_pin, HIGH);  
delayMicroseconds(10);            
digitalWrite(trigger_pin, LOW);   

duration = pulseIn(Echo_pin, HIGH); 
distance= duration*0.034/2; 

if ( distance < 15)
digitalWrite(LED, HIGH);
else
digitalWrite(LED, LOW);
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
delay(1000);
}