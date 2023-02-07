#include "SoftwareSerial.h"
#include "WiFiEsp.h"
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial EspSerial(10,11);
#define ONE_WIRE_BUS 2
WiFiEspServer server(80);
int status = WL_IDLE_STATUS;
OneWire oneWire(ONE_WIRE_BUS);
#define ONE_WIRE_BUS 2
DallasTemperature sensors(&oneWire);
char msgBuffer_temp[20];

void setup() {
   sensors.begin();
  Serial.begin(9600);
  EspSerial.begin(9600);
  WiFi.init(&EspSerial);
  status = WiFi.beginAP("IoT");
  IPAddress local_IP(192, 168, 0, 190);
  WiFi.configAP(local_IP);
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() { 

    WiFiEspClient client = server.available();
    if (client)
      {
         Serial.println("A client has connected");
  
          while (client.connected()){
        
          if (client.available()){
            sensors.requestTemperatures();
            String temp = dtostrf(sensors.getTempCByIndex(0), 6, 2, msgBuffer_temp);
             client.print(
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n"
              "Access-Control-Allow-Origin: http://localhost:8080\r\n"
              "Connection: close\r\n"  
              "Refresh: 20\r\n"        
              "\r\n");
             Serial.println(temp);
            client.print(temp);
            
            client.stop();
          }
               
        }
      } 
}
