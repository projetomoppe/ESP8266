/**
 * @example TCPServer.ino
 * @brief The TCPServer demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.02
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ESP8266.h>

#define SSID        "moppe_wireless"
#define PASSWORD    "Moppe123JAC12"

const int S1 = 1;
const int S2 = 2;
const char Disp[2] = "01";
const char Ultra[5] = "10cm";

ESP8266 wifi(Serial1, 115200);

void setup(void)
{
    Serial.begin(115200);
    Serial.print("setup begin\r\n");

//    wifi.kick();
//    wifi.restart();
      
    if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");
        Serial.println(wifi.getLocalIP().c_str());    
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.enableMUX()) {
        Serial.print("multiple ok\r\n");
    } else {
        Serial.print("multiple err\r\n");
    }
    
    if (wifi.startTCPServer(8090)) {
        Serial.print("start tcp server ok\r\n");
    } else {
        Serial.print("start tcp server err\r\n");
    }
    
    if (wifi.setTCPServerTimeout(10)) { 
        Serial.print("set tcp server timout 10 seconds\r\n");
    } else {
        Serial.print("set tcp server timout err\r\n");
    }
    
    Serial.print("setup end\r\n");
}
 
void loop(void)
{
  uint8_t buffer[512] = {0};
  String webpage = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\n";
    webpage += "<!DOCTYPE html>\r\n<meta http-equiv=\"refresh\" content=\"10\">\r\n";
    webpage += "<html><head>";
    webpage += "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">";
    webpage += "<title>MOPPE - Demonstracao do Prototipo</title>";
    webpage += "<style>body{font-size:16px;background-color:#F2F2F2;font-family:Arial, Helvetica, sans-Serif;color:#585858;} .centraliza{width:60%;heigth:600px;margin:60px auto 0 auto;background-color:#DDD;padding:15px;text-align:center;} h1{font-size:112.5%;margin:0 0 20px} p{font-size:100%;margin:5px 0;}</style>";
    webpage += "</head><body><div class=\"centraliza\">";
    webpage += "<h1>MOPPE - Demonstracao do Prototipo</h1>";
    webpage += "<p>ID do Dispositivo: ";
    webpage += Disp;
    webpage += "</p><p>Sensor ICOS Inferior - Leitura: ";
    webpage += S1;
    webpage += "</p><p>Sensor ICOS Superior - Leitura: ";
    webpage += S2;
    webpage += "</p><p>Sensor Ultrassonico - Leitura: ";
    webpage += Ultra;
    webpage += "</p>";
    webpage += "</div></body></html>";

    //Serial.println(webpage.length());

    char webpage2[webpage.length()];
    webpage.toCharArray(webpage2,webpage.length());
    
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    uint32_t len1 = sizeof(webpage2);
    
    if (len > 0) {
        Serial.print("Status:[");
        Serial.print(wifi.getIPStatus().c_str());
        Serial.println("]");
        
        Serial.print("Received from :");
        Serial.print(mux_id);
        Serial.print("[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
        
//        if(wifi.send(mux_id, buffer, len)) {
//            Serial.print("send back ok\r\n");
//        } else {
//            Serial.print("send back err\r\n");
//        }

        wifi.send(mux_id, webpage2, len1);
        
        if (wifi.releaseTCP(mux_id)) {
            Serial.print("release tcp ");
            Serial.print(mux_id);
            Serial.println(" ok");
        } else {
            Serial.print("release tcp");
            Serial.print(mux_id);
            Serial.println(" err");
        }
        
        Serial.print("Status:[");
        Serial.print(wifi.getIPStatus().c_str());
        Serial.println("]");
    }
}
