void setup() {
  Serial.begin(115200); // Start serial monitor
}

void loop() {
  int ldrValue = analogRead(A0); // Read analog value from LDR
  Serial.print("LDR Value: ");
  Serial.println(ldrValue); // Print value to Serial Monitor
  delay(500);
}
