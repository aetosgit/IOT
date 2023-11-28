int LED = 32;
int Sensor_input = 4;
void setup() 
{
Serial.begin(115200);
pinMode(LED,OUTPUT);
}

void loop() 
{
int sensor_Aout = analogRead(Sensor_input);
Serial.print("Gas Sensor: ");
Serial.print(sensor_Aout);
Serial.print("\t");
Serial.print("\t");
if (sensor_Aout > 1800)
{
  Serial.println("Alert:: Gas Leak Detected!!!");
  digitalWrite(LED,HIGH);
}
else
{
  Serial.println("No Gas Leak Detected...");
  digitalWrite(LED,LOW);
}
delay(1000);
}
