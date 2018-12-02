#include <ESP8266WiFi.h>

// Station properties -----------------------------
const char ssid[] = "Bico";
const char pwd[] = "12345678";

IPAddress ip(192, 168, 0, 69);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
// Station properties -----------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Turn on station ----------------------------------------------------
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  
  Serial.println();

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pwd);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.printf("Connected to %s \r\n", ssid);
  Serial.printf("Static IP: %s\r\n", ip.toString().c_str());
  // Turn on station ----------------------------------------------------
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
