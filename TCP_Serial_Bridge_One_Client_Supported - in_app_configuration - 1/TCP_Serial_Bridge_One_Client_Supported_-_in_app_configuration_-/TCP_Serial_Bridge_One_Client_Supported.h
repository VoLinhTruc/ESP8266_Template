#ifndef TCP_SERIAL_BRIDGE_ONE_CLIENT_SUPPORTED_H
#define TCP_SERIAL_BRIDGE_ONE_CLIENT_SUPPORTED_H

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "TCP_Serial_Bridge_One_Client_Supported.h"
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

// Serial properties ------------------------------
uint32_t baudrate;
SerialConfig serial_mode = SERIAL_8N1;
// Serial properties ------------------------------

// Others properties ------------------------------
uint32_t time_life;
// Others properties ------------------------------

WiFiServer my_server(port);

SerialConfig convertSerialModeFromStringToMacro(String serial_mode_data);
String convertSerialModeFromMacroToString(SerialConfig serial_mode);


String getConfigurationStringFromEEPROM()
{
  String temp_str = "";
  for(uint16_t i = 0; i < MAX_CONFIGURATION_STRING_SIZE; i++)
  {
    char temp_val = EEPROM.read(i);
    if(temp_val != ';')
    {
      temp_str += String(temp_val);
    }
    else
    {
      break;
    }
  }
  return temp_str;
}

void updateDataFromConfigurationString(String temp_str)
{
  StaticJsonBuffer<MAX_CONFIGURATION_STRING_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(temp_str);

  if(root.containsKey("SSID_data"))
  {
    String SSID_data = root["SSID_data"];
    ssid = SSID_data;
  }
  if(root.containsKey("password_data"))
  {
    String password_data = root["password_data"];
    pwd = password_data;
  }
  if(root.containsKey("static_IP_data"))
  {
    String static_IP_data = root["static_IP_data"];
    ip.fromString(static_IP_data);
  }
  if(root.containsKey("gateway_data"))
  {
    String gateway_data = root["gateway_data"];
    gateway.fromString(gateway_data);
  }
  if(root.containsKey("subnet_data"))
  {
    String subnet_data = root["subnet_data"];
    subnet.fromString(subnet_data);
  }
  if(root.containsKey("port_data"))
  {
    String port_data = root["port_data"];
    port = port_data.toInt();
  }
  if(root.containsKey("baudrate_data"))
  {
    String baudrate_data = root["baudrate_data"];
    baudrate = baudrate_data.toInt();
  }
  if(root.containsKey("serial_mode_data"))
  {
    String serial_mode_data = root["serial_mode_data"];
    serial_mode = convertSerialModeFromStringToMacro(serial_mode_data);
  }
  if(root.containsKey("time_life_data"))
  {
    String time_life_data = root["time_life_data"];
    time_life = time_life_data.toInt();
  }
}

String buildJsonStringFromData()
{
  String temp_str = "";
  
  StaticJsonBuffer<MAX_CONFIGURATION_STRING_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["SSID_data"] = ssid;
  root["password_data"] = pwd;
  root["static_IP_data"] = ip.toString();
  root["gateway_data"] = gateway.toString();
  root["subnet_data"] = subnet.toString();
  root["port_data"] = String(port);
  root["baudrate_data"] = String(baudrate);
  root["serial_mode_data"] = convertSerialModeFromMacroToString(serial_mode);
  root["time_life_data"] = String(time_life);
  
  root.prettyPrintTo(temp_str);
  return temp_str;
}

void setStringToEEPROM(String temp_str)
{
  for(uint16_t i = 0; i < temp_str.length(); i++)
  {
    EEPROM.write(i, temp_str[i]);
  }
  EEPROM.write(temp_str.length(), ';');
  EEPROM.commit();  
}

void configurationRoutine()
{
  Serial.println("Previous parameter: ");
  String configuration_string = getConfigurationStringFromEEPROM();
  Serial.println(configuration_string);

  updateDataFromConfigurationString(configuration_string);
  
  Serial.println();
  Serial.println("Ready for configuration");
  
  bool configuration_condition = true;
  while(configuration_condition)
  {
    #define CONFIGURATION_TIMEOUT 3000
    static uint32_t last_time = millis();
    if((millis() - last_time) > CONFIGURATION_TIMEOUT)
    {
      configuration_condition = false;
    }
    delay(1); //prevent from Soft WDT

    if(Serial.available())
    {
      configuration_string = Serial.readString();
      updateDataFromConfigurationString(configuration_string);
      configuration_string = buildJsonStringFromData();
      setStringToEEPROM(configuration_string);
     /////////////////////////////////////////////////////////////////////////////////////
      Serial.println("Configuration Done");
      Serial.println(configuration_string);

      last_time = millis();
    }
  } 
}




SerialConfig convertSerialModeFromStringToMacro(String serial_mode_data)
{
  SerialConfig serial_mode;
  if(serial_mode_data == "SERIAL_5N1")
  {
    serial_mode = SERIAL_5N1;
  }
  else if(serial_mode_data == "SERIAL_6N1")
  {
    serial_mode = SERIAL_6N1;
  }
  else if(serial_mode_data == "SERIAL_7N1")
  {
    serial_mode = SERIAL_7N1;
  }
  else if(serial_mode_data == "SERIAL_8N1")
  {
    serial_mode = SERIAL_8N1 ;
  }
  else if(serial_mode_data == "SERIAL_5N2")
  {
    serial_mode = SERIAL_5N2;
  }
  else if(serial_mode_data == "SERIAL_6N2")
  {
    serial_mode = SERIAL_6N2;
  }
  else if(serial_mode_data == "SERIAL_7N2")
  {
    serial_mode = SERIAL_7N2;
  }
  else if(serial_mode_data == "SERIAL_8N2")
  {
    serial_mode = SERIAL_8N2;
  }
  else if(serial_mode_data == "SERIAL_5E1")
  {
    serial_mode = SERIAL_5E1;
  }
  else if(serial_mode_data == "SERIAL_6E1")
  {
    serial_mode = SERIAL_6E1;
  }
  else if(serial_mode_data == "SERIAL_7E1")
  {
    serial_mode = SERIAL_7E1;
  }
  else if(serial_mode_data == "SERIAL_8E1")
  {
    serial_mode = SERIAL_8E1;
  }
  else if(serial_mode_data == "SERIAL_5E2")
  {
    serial_mode = SERIAL_5E2;
  }
  else if(serial_mode_data == "SERIAL_6E2")
  {
    serial_mode = SERIAL_6E2;
  }
  else if(serial_mode_data == "SERIAL_7E2")
  {
    serial_mode = SERIAL_7E2;
  }
  else if(serial_mode_data == "SERIAL_8E2")
  {
    serial_mode = SERIAL_8E2;
  }
  else if(serial_mode_data == "SERIAL_5O1")
  {
    serial_mode = SERIAL_5O1;
  }
  else if(serial_mode_data == "SERIAL_6O1")
  {
    serial_mode = SERIAL_6O1;
  }
  else if(serial_mode_data == "SERIAL_7O1")
  {
    serial_mode = SERIAL_7O1;
  }
  else if(serial_mode_data == "SERIAL_8O1")
  {
    serial_mode = SERIAL_8O1;
  }
  else if(serial_mode_data == "SERIAL_5O2")
  {
    serial_mode = SERIAL_5O2;
  }
  else if(serial_mode_data == "SERIAL_6O2")
  {
    serial_mode = SERIAL_6O2;
  }
  else if(serial_mode_data == "SERIAL_7O2")
  {
    serial_mode = SERIAL_7O2;
  }
  else if(serial_mode_data == "SERIAL_8O2")
  {
    serial_mode = SERIAL_8O2;
  }

  return serial_mode;
}


String convertSerialModeFromMacroToString(SerialConfig serial_mode)
{
  String serial_mode_data;
  if(serial_mode == SERIAL_5N1)
  {
    serial_mode_data = "SERIAL_5N1";
  }
  else if(serial_mode == SERIAL_6N1)
  {
    serial_mode_data = "SERIAL_6N1";
  }
  else if(serial_mode == SERIAL_7N1)
  {
    serial_mode_data = "SERIAL_7N1";
  }
  else if(serial_mode == SERIAL_8N1 )
  {
    serial_mode_data = "SERIAL_8N1 ";
  }
  else if(serial_mode == SERIAL_5N2)
  {
    serial_mode_data = "SERIAL_5N2";
  }
  else if(serial_mode == SERIAL_6N2)
  {
    serial_mode_data = "SERIAL_6N2";
  }
  else if(serial_mode == SERIAL_7N2)
  {
    serial_mode_data = "SERIAL_7N2";
  }
  else if(serial_mode == SERIAL_8N2)
  {
    serial_mode_data = "SERIAL_8N2";
  }
  else if(serial_mode == SERIAL_5E1)
  {
    serial_mode_data = "SERIAL_5E1";
  }
  else if(serial_mode == SERIAL_6E1)
  {
    serial_mode_data = "SERIAL_6E1";
  }
  else if(serial_mode == SERIAL_7E1)
  {
    serial_mode_data = "SERIAL_7E1";
  }
  else if(serial_mode == SERIAL_8E1)
  {
    serial_mode_data = "SERIAL_8E1";
  }
  else if(serial_mode == SERIAL_5E2)
  {
    serial_mode_data = "SERIAL_5E2";
  }
  else if(serial_mode == SERIAL_6E2)
  {
    serial_mode_data = "SERIAL_6E2";
  }
  else if(serial_mode == SERIAL_7E2)
  {
    serial_mode_data = "SERIAL_7E2";
  }
  else if(serial_mode == SERIAL_8E2)
  {
    serial_mode_data = "SERIAL_8E2";
  }
  else if(serial_mode == SERIAL_5O1)
  {
    serial_mode_data = "SERIAL_5O1";
  }
  else if(serial_mode == SERIAL_6O1)
  {
    serial_mode_data = "SERIAL_6O1";
  }
  else if(serial_mode == SERIAL_7O1)
  {
    serial_mode_data = "SERIAL_7O1";
  }
  else if(serial_mode == SERIAL_8O1)
  {
    serial_mode_data = "SERIAL_8O1";
  }
  else if(serial_mode == SERIAL_5O2)
  {
    serial_mode_data = "SERIAL_5O2";
  }
  else if(serial_mode == SERIAL_6O2)
  {
    serial_mode_data = "SERIAL_6O2";
  }
  else if(serial_mode == SERIAL_7O2)
  {
    serial_mode_data = "SERIAL_7O2";
  }
  else if(serial_mode == SERIAL_8O2)
  {
    serial_mode_data = "SERIAL_8O2";
  }

  return serial_mode_data;
}

#endif
