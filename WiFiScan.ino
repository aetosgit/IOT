#include "ESP8266WiFi.h"  // For ESP8266 boards

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Scanning for available WiFi networks...");
}

void loop() {
  int networks = WiFi.scanNetworks();
  Serial.println("Scan complete.");
  if (networks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.println("Networks found:");
    for (int i = 0; i < networks; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm) ");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "[Open]" : "[Secured]");
      delay(10);
    }
  }
  delay(15000); // Scan every 15 seconds
}
