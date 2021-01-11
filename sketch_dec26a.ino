#include <ESP8266WiFi.h>
#define NAME "ESProj"
#define PASSWORD "malf1234"

IPAddress staticIP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(8080);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  setupWiFi();
  server.begin();
  client = server.available();
}

void loop() {
  while (!client.connected()){
    if (WiFi.softAPgetStationNum() == 0){
      ESP.restart();
    }
    client = server.available();
  }
  
  if (client.connected()){
    while (Serial.available()){
      char serial_read = Serial.read();
      client.print(serial_read);
      while (client.available()){
        char client_read = client.read();
        Serial.print(client_read);
      }
    }
    while (client.available()){
        char client_read = client.read();
        Serial.print(client_read);
    }
  }

}

void setupWiFi(){
  WiFi.mode(WIFI_AP);
  
  boolean result, configChange;

  while(1){                                              //setting AP
    result = WiFi.softAP(NAME, PASSWORD);
    if(result){
      configChange = WiFi.softAPConfig(staticIP, gateway, subnet);
      if(configChange){
        break;
      }
    }
  }

  while(WiFi.softAPgetStationNum() != 1){         //waiting for station (client) to connect
    delay(900);
  }
}
