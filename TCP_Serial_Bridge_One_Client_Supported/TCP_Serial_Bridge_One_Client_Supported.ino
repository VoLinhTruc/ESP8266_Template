#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

WiFiServer my_server(502);
ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(2000);

#define MAX_PARA_LENGTH 20

#define SSID_MAX_LENGTH MAX_PARA_LENGTH
#define PASSWORD_MAX_LENGTH MAX_PARA_LENGTH
#define IP_MAX_LENGTH MAX_PARA_LENGTH
#define GATEWAY_MAX_LENGTH MAX_PARA_LENGTH
#define SUBNET_MAX_LENGTH MAX_PARA_LENGTH

#define START_SEEK 0

  char station_ssid[SSID_MAX_LENGTH];
  char station_password[PASSWORD_MAX_LENGTH];
  char station_ip[IP_MAX_LENGTH];
  char station_gateway[GATEWAY_MAX_LENGTH];
  char station_subnet[SUBNET_MAX_LENGTH];
  uint16_t station_port;

  uint16_t seek = START_SEEK;
  EEPROM.get(seek, station_ssid);
  seek += sizeof(station_ssid);
  EEPROM.get(seek, station_password);
  seek += sizeof(station_password);
  EEPROM.get(seek, station_ip);
  seek += sizeof(station_ip);
  EEPROM.get(seek, station_gateway);
  seek += sizeof(station_gateway);
  EEPROM.get(seek, station_subnet);
  seek += sizeof(station_subnet);
  EEPROM.get(seek, station_port);
  seek += sizeof(station_port);

  // Config via Web page if pin is active -----------------------------------------------------------------------------------------------------------------
#define SOFTAP_ENABLE_PIN 2
  pinMode(SOFTAP_ENABLE_PIN, INPUT_PULLUP);
  if (!digitalRead(SOFTAP_ENABLE_PIN))
  {
    Serial.println("Start Soft_AP ");
    WiFi.softAPConfig(IPAddress(192, 168, 4, 2), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP("ESPsoftAP_01");
    server.on("/", [&]()
    {
      if (server.hasArg("ssid") && server.hasArg("password") && server.hasArg("static_ip") && server.hasArg("gateway") && server.hasArg("subnet") && server.hasArg("port"))
      {
        String temp_ssid = server.arg("ssid");
        String temp_password = server.arg("password");
        String temp_static_ip = server.arg("static_ip");
        String temp_gateway = server.arg("gateway");
        String temp_subnet = server.arg("subnet");
        String temp_port = server.arg("port");

        memcpy(station_ssid, temp_ssid.c_str(), SSID_MAX_LENGTH);
        memcpy(station_password, temp_password.c_str(), PASSWORD_MAX_LENGTH);
        memcpy(station_ip, temp_static_ip.c_str(), IP_MAX_LENGTH);
        memcpy(station_gateway, temp_gateway.c_str(), GATEWAY_MAX_LENGTH);
        memcpy(station_subnet, temp_subnet.c_str(), SUBNET_MAX_LENGTH);
        station_port = (uint16_t)(temp_port.toInt());

        Serial.println(station_ssid);
        Serial.println(station_password);
        Serial.println(station_ip);
        Serial.println(station_gateway);
        Serial.println(station_subnet);
        Serial.println(station_port);

        uint16_t seek = START_SEEK;
        EEPROM.put(seek, station_ssid);
        seek += sizeof(station_ssid);
        EEPROM.put(seek, station_password);
        seek += sizeof(station_password);
        EEPROM.put(seek, station_ip);
        seek += sizeof(station_ip);
        EEPROM.put(seek, station_gateway);
        seek += sizeof(station_gateway);
        EEPROM.put(seek, station_subnet);
        seek += sizeof(station_subnet);
        EEPROM.put(seek, station_port);
        seek += sizeof(station_port);
        EEPROM.commit();

        seek = START_SEEK;
        EEPROM.get(seek, station_ssid);
        seek += sizeof(station_ssid);
        EEPROM.get(seek, station_password);
        seek += sizeof(station_password);
        EEPROM.get(seek, station_ip);
        seek += sizeof(station_ip);
        EEPROM.get(seek, station_gateway);
        seek += sizeof(station_gateway);
        EEPROM.get(seek, station_subnet);
        seek += sizeof(station_subnet);
        EEPROM.get(seek, station_port);
        seek += sizeof(station_port);
      }

      String web_page = String("<!DOCTYPE html>")
                        + "<html lang=\"en\">"
                        + "<title>ESP8266 BRIDGE CONFIGURATION</title>"
                        + "<meta charset=\"UTF-8\">"
                        + "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"

                        + "<style>"
                        + "body {font-family: \"Lato\", sans-serif}"
                        + ".mySlides {display: none}"

                        + "input {"
                        + "    padding: 6px 10px;"
                        + "    margin: 1%;"
                        + "    box-sizing: border-box;"
                        + "    border: 4px solid #ccc;"
                        + "    -webkit-transition: 0.5s;"
                        + "    transition: 0.5s;"
                        + "    outline: none;"
                        + "    font-size:30px;"
                        + "    width:70% ;"
                        + "}"

                        + "input:focus {"
                        + "    border: 6px solid #000;"
                        + "}"
                        + "</style>"

                        + "<body>"

                        + "  <div style=\"width:100%;overflow:hidden;color:#fff!important;background-color:#000!important;\">"
                        + "    <h1 align=\"center\">BRIDGE CONFIGURATION</h1>"
                        + "  </div>"

                        + "  <form method=\"post\">"
                        + "    <div align=\"center\" >"
                        + "      <input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"SSID\" value=\"" + String(station_ssid) + "\">"
                        + "    </div>"
                        + "    <div align=\"center\" >"
                        + "      <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"PASSWORD\" value=\"" + String(station_password) + "\">"
                        + "    </div>"

                        + "    <div align=\"center\" >"
                        + "      <input type=\"text\" id=\"static_ip\" name=\"static_ip\" placeholder=\"Static IP\" value=\"" + String(station_ip) + "\">"
                        + "    </div>"
                        + "    <div align=\"center\" >"
                        + "      <input type=\"text\" id=\"gateway\" name=\"gateway\" placeholder=\"Gateway\" value=\"" + String(station_gateway) + "\">"
                        + "    </div>"
                        + "    <div align=\"center\" >"
                        + "      <input type=\"text\" id=\"subnet\" name=\"subnet\" placeholder=\"Subnet\" value=\"" + String(station_subnet) + "\">"
                        + "    </div>"

                        + "    <div align=\"center\" >"
                        + "      <div style=\"width:70%;display: flex; flex-direction: row;\">"
                        + "        <input style=\"width:60%;\" type=\"text\" id=\"port\" name=\"port\" placeholder=\"Port\" value=\"" + String(station_port) + "\">"
                        + "        <button style=\"width:40%;font-size:100%;margin: 1%;border:none;display:inline-block;vertical-align:middle;overflow:hidden;text-decoration:none;color:inherit;background-color:inherit;text-align:center;cursor:pointer;white-space:nowrap;color:#fff!important;background-color:#000!important;\">"
                        + "        Submit"
                        + "        </button>"
                        + "      </div>"
                        + "    </div>"
                        + "  </form>"

                        + "</body>"

                        + "</html>";

      server.send(200, "text/html", web_page);
    }
             );
    server.begin();
  }
  while (!digitalRead(SOFTAP_ENABLE_PIN))
  {
    server.handleClient();
  }
  // Config via Web page if pin is active -----------------------------------------------------------------------------------------------------------------


  // Turn on station ----------------------------------------------------
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.println();

  IPAddress ip;
  IPAddress gateway;
  IPAddress subnet;
  ip.fromString(station_ip);
  gateway.fromString(station_gateway);
  subnet.fromString(station_subnet);

  Serial.printf("Connecting to %s \r\n", station_ssid);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(station_ssid, station_password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.printf("Connected to %s \r\n", station_ssid);
  Serial.printf("Static IP: %s\r\n", ip.toString().c_str());
  // Turn on station ----------------------------------------------------

  // Start TCP Server ---------------------------------------------------------
  my_server.begin(station_port);
  Serial.printf("Server started at port %d\r\n", station_port);
  // Start TCP Server ---------------------------------------------------------
}

void loop() {

  // put your main code here, to run repeatedly:
  WiFiClient client = my_server.available();
  if (client)
  {
    // Clean Serial --------------------------------
    while (Serial.available())
    {
      Serial.read();
    }
    // Clean Serial --------------------------------

#define CONNECTION_TIMEOUT 5000
    uint32_t end_point = millis();

    while (client.connected())
    {
      // Pass TCP to Serial ---------------------
      if (client.available())
      {
        while (client.available())
        {
          uint8_t data = client.read();
          Serial.write(data);
        }

        end_point = millis();
      }
      // Pass TCP to Serial ---------------------

      // Pass Serial to TCP ---------------------
      if (Serial.available())
      {
        while (Serial.available())
        {
          uint8_t data = Serial.read();
          client.write(data);
        }

        end_point = millis();
      }
      // Pass Serial to TCP ---------------------

      if (millis() - end_point > CONNECTION_TIMEOUT)
      {
        end_point = millis();
        break;
      }
    }
    client.stop();
  }
}
