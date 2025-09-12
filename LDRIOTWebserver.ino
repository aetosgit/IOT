#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <time.h>

// WiFi credentials
const char* ssid = "TIS";
const char* password = "Tischool@2023";

// NTP server and time zone settings
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST (GMT+5:30)
const int daylightOffset_sec = 0;

// Web server on port 80
ESP8266WebServer server(80);

// LDR sensor pin
const int ldrPin = A0;

// Maximum number of records to store
#define MAX_RECORDS 10

// Data structure for a single observation
struct LDRRecord {
  String timestamp;
  int ldrValue;
  String meaning;
};

// Circular buffer of observations
LDRRecord records[MAX_RECORDS];
int recordIndex = 0;

// Convert raw LDR reading to a weather-like label
String getMeaning(int val) {
  if (val > 900) return "☀️ Sunny";
  else if (val > 700) return "🌤️ Partly Cloudy";
  else if (val > 500) return "☁️ Cloudy";
  else if (val > 300) return "🌧️ Gloomy";
  else return "🌑 Night";
}

// Get current time from NTP and format as string
String getCurrentTime() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

// Add a new record to the circular buffer
void addLDRRecord() {
  int value = analogRead(ldrPin);
  String meaning = getMeaning(value);
  String timestamp = getCurrentTime();

  LDRRecord rec;
  rec.timestamp = timestamp;
  rec.ldrValue = value;
  rec.meaning = meaning;

  records[recordIndex] = rec;
  recordIndex = (recordIndex + 1) % MAX_RECORDS;
}

// Generate HTML page with observation table
String generateHTML() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='5'/>";
  html += "<title>LDR Weather Observation</title>";
  html += "<style>";
  html += "body{font-family:Arial;background-color:#f9f9f9;padding:20px;}";
  html += "table{border-collapse:collapse;width:100%;max-width:600px;}";
  html += "th,td{border:1px solid #ccc;padding:8px;text-align:center;}";
  html += "th{background-color:#f2f2f2;}";
  html += "h2{font-size:24px;color:#333;}";
  html += "</style></head><body>";
  
  html += "<h2>🌤️ LDR Weather Observation Table</h2>";
  html += "<table>";
  html += "<tr><th>Date & Time</th><th>LDR Value</th><th>Weather</th></tr>";

  for (int i = 0; i < MAX_RECORDS; i++) {
    int index = (recordIndex + i) % MAX_RECORDS;
    if (records[index].timestamp == "") continue;

    html += "<tr>";
    html += "<td>" + records[index].timestamp + "</td>";
    html += "<td>" + String(records[index].ldrValue) + "</td>";
    html += "<td>" + records[index].meaning + "</td>";
    html += "</tr>";
  }

  html += "</table>";
  html += "<p style='font-size:12px;margin-top:10px;'>Page auto-refreshes every 5 seconds.</p>";
  html += "</body></html>";

  return html;
}

// Web server root handler
void handleRoot() {
  addLDRRecord();
  server.send(200, "text/html", generateHTML());
}

// Initialize NTP time sync
void setupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Syncing time via NTP");
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTime synced!");
}

// Main setup
void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setupTime();  // Sync time from NTP server

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
}

// Main loop
void loop() {
  server.handleClient();
}
