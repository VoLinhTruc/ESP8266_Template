#include <ESP8266WiFi.h>

// Station properties -----------------------------
const char ssid[] = "Network";
const char pwd[] = "12345678";

IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
// Station properties -----------------------------

WiFiServer my_server(502);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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


  my_server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = my_server.available();
  if(client)
  {
    // Clean Serial --------------------------------
    while(Serial.available())
    {
      Serial.read();
    }
    // Clean Serial --------------------------------
    
    #define CONNECTION_TIMEOUT 5000    
    uint32_t end_point = millis();
    
    while(client.connected())
    {
      // Pass TCP to Serial ---------------------
      if(client.available())
      {
        while(client.available())
        {
          uint8_t data = client.read();
          Serial.write(data);
        }

        end_point = millis();
      }
      // Pass TCP to Serial ---------------------

      // Pass Serial to TCP ---------------------
      if(Serial.available())
      {
        while(Serial.available())
        {
          uint8_t data = Serial.read();
          client.write(data);
        }

        end_point = millis();
      }
      // Pass Serial to TCP ---------------------
      
      if(millis() - end_point > CONNECTION_TIMEOUT)
      {
        end_point = millis();
        break;
      }
    }
    client.stop();
  }
}
