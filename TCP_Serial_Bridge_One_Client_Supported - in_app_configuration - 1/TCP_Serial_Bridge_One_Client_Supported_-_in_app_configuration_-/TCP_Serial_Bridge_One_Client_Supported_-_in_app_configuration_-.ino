#include "TCP_Serial_Bridge_One_Client_Supported.h"

void setup() {
  // put your setup code here, to run once:
  //--------------------------------------------------
  Serial.begin(38400);
  Serial.setTimeout(MAX_CONFIGURATION_STRING_SIZE);
  EEPROM.begin(MAX_CONFIGURATION_STRING_SIZE);

  configurationRoutine(); 

  Serial.end();
  //--------------------------------------------------

  Serial.begin(baudrate);
  // Turn on station ----------------------------------------------------
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  
  Serial.println();

  Serial.println("Connecting to ");
  Serial.println(ssid.c_str());
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid.c_str(), pwd.c_str());
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.printf("Connected to %s \r\n", ssid.c_str());
  Serial.printf("Static IP: %s\r\n", ip.toString().c_str());
  // Turn on station ----------------------------------------------------


  my_server.begin(port);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Nomal performing -----------------------------------------------------------------
  WiFiClient client = my_server.available();
  if(client)
  {
    // Clean Serial --------------------------------
    while(Serial.available())
    {
      Serial.read();
    }
    // Clean Serial --------------------------------
       
    uint32_t end_point = millis();
    
    while(client.connected())
    {
      // Pass TCP to Serial ---------------------
      if(client.available())
      {
        while(client.available())
        {
          Serial.write(client.read());
        }

        end_point = millis();
      }
      // Pass TCP to Serial ---------------------

      // Pass Serial to TCP ---------------------
      if(Serial.available())
      {
        static uint8_t data_buffer[1000];
        static int data_buffer_length;
        data_buffer_length = 0;
        while(Serial.available())
        {
          data_buffer[data_buffer_length++] = Serial.read();
          delayMicroseconds(1000000*10/baudrate);
        }
        client.write((uint8_t*)data_buffer, data_buffer_length);

        end_point = millis();
      }
      // Pass Serial to TCP ---------------------
      
      if(millis() - end_point > time_life)
      {
        end_point = millis();
        break;
      }
    }
    client.stop();
  }
  // Nomal performing -----------------------------------------------------------------

  // Check Wifi status and reconnect if it has lost -----------------------------------
  if(WiFi.status() != WL_CONNECTED)
  {    
    Serial.println("Connecting to ");
    Serial.println(ssid.c_str());
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid.c_str(), pwd.c_str());
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print('.');
      delay(500);
    }
    Serial.println();
    Serial.printf("Connected to %s \r\n", ssid.c_str());
    Serial.printf("Static IP: %s\r\n", ip.toString().c_str());
  }
  // Check Wifi status and reconnect if it has lost -----------------------------------
}
