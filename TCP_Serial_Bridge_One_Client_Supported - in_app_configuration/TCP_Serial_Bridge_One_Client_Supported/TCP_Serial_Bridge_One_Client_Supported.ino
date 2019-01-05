#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "ArduinoJson-master/ArduinoJson.h"

#define MAX_CONFIGURATION_STRING_SIZE 512

// Station properties -----------------------------
String ssid;
String pwd;

IPAddress ip;
IPAddress gateway;
IPAddress subnet;
uint32_t port = 502;
// Station properties -----------------------------

WiFiServer my_server(port);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(MAX_CONFIGURATION_STRING_SIZE);
  EEPROM.begin(MAX_CONFIGURATION_STRING_SIZE);

  String configuration_string = "";
  Serial.println("Previous parameter: ");
  for(uint16_t i = 0; i < MAX_CONFIGURATION_STRING_SIZE; i++)
  {
    char temp_val = EEPROM.read(i);
    if(temp_val != ';')
    {
      configuration_string += String(temp_val);
    }
    else
    {
      break;
    }
  }
  Serial.println(configuration_string);
  
  Serial.println();
  Serial.println("Ready for configuration");

  bool configuration_condition = true;
  while(configuration_condition)
  {
    #define CONFIGURATION_TIMEOUT 5000
    static uint32_t last_time = millis();
    if((millis() - last_time) > CONFIGURATION_TIMEOUT)
    {
      configuration_condition = false;
    }
    delay(1); //prevent from Soft WDT

    if(Serial.available())
    {
      configuration_string = Serial.readString();
      for(uint16_t i = 0; i < configuration_string.length(); i++)
      {
        EEPROM.write(i, configuration_string[i]);
      }
      EEPROM.write(configuration_string.length(), ';');
      EEPROM.commit();
      
      configuration_condition = false;
      Serial.println("Configuration Done");
    }
  }

  StaticJsonBuffer<MAX_CONFIGURATION_STRING_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(configuration_string);
  
  String SSID_data = root["SSID_data"];
  String password_data = root["password_data"];
  String static_IP_data = root["static_IP_data"];
  String gateway_data = root["gateway_data"];
  String subnet_data = root["subnet_data"];
  uint32_t port_data = root["port_data"];

  ssid = SSID_data;
  pwd = password_data;
  ip.fromString(static_IP_data);
  gateway.fromString(gateway_data);
  subnet.fromString(subnet_data);
  port = port_data;
  

  // Turn on station ----------------------------------------------------
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  
  Serial.println();

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
