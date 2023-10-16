#include <WiFi.h>
#include "time.h"

const char* ssid       = "TIS";
const char* password   = "Tischool@2023";

// Old sketch illustrates how to set IST by offseting seconds for the variable gmtOffset_sec
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = 3600;
//const int   daylightOffset_sec = 3600;

// New sketch illustrates how to get the IST from the zone server
const char* ntpServer = "in.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 19800; //GMT+5:30

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
