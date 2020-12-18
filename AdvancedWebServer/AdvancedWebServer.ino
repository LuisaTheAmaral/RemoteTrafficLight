/*
 * Contol a relay over WiFi using ESP8266 NodeMCU to 
 * turn ON or OFF 2 AC bulb using 2 channel relay
 * 
 * Watch Video instrution for this code:  https://youtu.be/F7mK587OQgI
 * Full explanation of this code and wiring diagram is available at
 * my Arduino Course at Udemy.com here: http://robojax.com/L/?id=62

 * Written by Ahmad Shamshiri on Feb 22, 2020 at 15:25
 * in Ajax, Ontario, Canada. www.robojax.com
 * Best Wifi channel was created by Mael
 * Adapted by Luísa Amaral and Hugo Leal

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;

int on = 0;

int relay1Pin = D1;//define a pin for relay (D1)
int relay2Pin = D0;//define a pin for relay

int relay1State = 1;//initial state . 1 ON, 0 OFF
int relay2State = 0;//initial state . 1 ON, 0 OFF
int counter = 0;

//String button1Title1 ="Ativar luz verde";
//String button1Title2 ="Ativar luz vermelha";

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>



//#ifndef STASSID
//#define STASSID "Semaforo_USF" // your WiFi SSID
//#define STAPSK  "usfsantajoana2020" //your WiFi password
//#endif
//
//const char *ssid = STASSID;
//const char *password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() {
 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  <meta charset=\"utf-8\">\
  <title>Semáforo USF</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <style>\
  html,body{width:100%\;height:100%\;margin:0}*{box-sizing:border-box}.colorAll{background-color:#90ee90}.colorBtn{background-color:#add8e6}.angleButtdon,a{font-size:50px\;border:1px solid #ccc\;display:table-caption\;padding:7px 10px\;text-decoration:none\;cursor:pointer\;padding:5px 6px 7px 10px}a{display:block}.btn{margin:5px\;border:none\;display:inline-block\;vertical-align:middle\;text-align:center\;white-space:nowrap}";
  HTML += "#ret {width:100px;height:200px;border:1px;border-radius: 10px;background-color:#434343;padding: 20px;}";
  HTML += "#circleRED {width: 60px;height:60px;border-radius: 50px;background-color:red;padding: 20px;}";
  HTML += "#circleGREEN {width:60px;height:60px;border-radius: 50px;background-color:green;padding: 20px;}";
  HTML +="</style></head><body><h1>Semáforo USF</h1>";
  
  if(relay1State){ //verde acesso
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=off'\" style=\"background-color:red;width: 300px;height:150px;border-radius: 5px;font-size:20px;\" type=\"button\" >";
    HTML +="Ativar luz vermelha </div>";
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=on'\"  type=\"button\" style=\"display: none;\" disabled >";
    //HTML +=button1Title1; 
    HTML += "</div>";
    //HTML += "<div style=\"padding:20px; \"><div id=\"ret\"><div id=\"circleRED\" style=\"background-color: #331111;\"></div><br><div id=\"circleGREEN\" ></div></div></div>";
  }else{
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=off'\"  type=\"button\" style=\"display: none;\" disabled >";
    //HTML +=button1Title2;
    HTML += "</div>";
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=on'\" style=\"background-color:green;width: 300px;height:150px;border-radius: 5px;font-size:20px;\" type=\"button\" >";
    HTML +="Ativar luz verde </div>";
    //HTML += "<div style=\"padding:20px; \"><div id=\"ret\"><div id=\"circleRED\"></div><br><div id=\"circleGREEN\" style=\"background-color: #331111;\" ></div></div></div>"; 
  }
  HTML +="</a></div>";  
  HTML +="</a></div>";
  HTML += "</body></html>";
  server.send(200, "text/html", HTML);  
}//handleRoot()

void handleNotFound() {
 server.send(404, "text/plain", "Please Reconnect");
}//end of handleNotFound()

void setup(void) {
  Serial.println("new code");
  pinMode(relay1Pin, OUTPUT);// define a pin as output for relay
  pinMode(relay2Pin, OUTPUT);// define a pin as output for relay
  digitalWrite(relay1Pin, relay1State);//initial state either ON or OFF
  digitalWrite(relay2Pin, relay2State);//initial state either ON or OFF
  Serial.begin(115200);//initialize the serial monitor
  connect_network();

  //Relay control ON OFF by Robojax.com

//  Code for a connection to another wifi    
  

}//end of setup

void loop(void) {
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
//
//  // Wait until the client sends some data
//  while (!client.available()) {
//    delay(1);
//  }
  //Serial.println("Hello MIgas");
  counter +=1;
  if (counter % 600 == 0){
    //Serial.println("passed");
    connect_network();
    counter=1;
  }
  server.handleClient();
  //MDNS.update();
  
  if(relay1State ==1)
  {
    digitalWrite(relay1Pin, LOW);
  }else
  {
    digitalWrite(relay1Pin, HIGH);
  }

  if(relay2State ==1)
  {
    digitalWrite(relay2Pin, LOW);
  }else
  {
    digitalWrite(relay2Pin, HIGH);
  }

   delay(100);
   ////////////Serial.print("Relay1:");Serial.print (relay1State);
   //Serial.print(" Relay2:");//Serial.print(relay2State);   
}//end of loop

/*
 * relayControl()
 * updates the value of "relayState" varible to 1 or zero 
 * returns nothing
 * written by Ahmad Shamshiri
 * on Wednesday Feb 22, 2020 at 16:20 in Ajax, Ontario, Canada
 * www.robojax.com
 */
void relayControl() {
  if(server.arg("r1") == "on")
  {
    relay1State = 1;// set state of relay1 to ON
    relay2State = 0;
  }else if(server.arg("r1") == "off"){
    relay1State = 0;  // set state of relay1 to OFF
    relay2State = 1;
  }
  

  handleRoot();
}//relayControl() end


void connect_network(){
  WiFi.mode(WIFI_AP);
  //Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open

  byte best_channel = channel_chooser();

  while (best_channel == 255) {         // while the scan isn't finished
    best_channel = channel_chooser();
    delay(0);                           // needed to avoid a WDT trigger, yields the process time to background esp tasks
  }

  
  WiFi.softAP("redeSemaforo","senha", best_channel);
  //Serial.print("best");
  //Serial.print(best_channel);
  //Serial.print("channel");
  //Serial.print(WiFi.channel());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  //Serial.print(IP);

  // Print ESP8266 Local IP Address
  //Serial.print(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/control", HTTP_GET, relayControl);
  server.onNotFound(handleNotFound);
  server.begin();
  //Serial.print("HTTP server started");
}

// This fuction was created by Mael 
// http://maelpoureau.com/ESP8266_channel_chooser/
// We
byte channel_chooser() {
  int scan_result = WiFi.scanComplete();

  if (scan_result < 0) {       //  if not scanning return 255 and launch scan
    WiFi.scanNetworks(true);
    return 255;

  } else if (scan_result == 0) { // return 1 if found no networks

    return 1;

  } else {

    // create the list of channel weights

    int channels[14][2];

    for (byte a = 1; a < 14; a++) {
      channels[a][0] = a;
      channels[a][1] = 0;
    }

    for (byte a = 0; a < scan_result; a++) {
      channels[WiFi.channel(a)][0] = WiFi.channel(a);
      channels[a][1] += 100 + WiFi.RSSI(a);
    }

    // adding adgacent channels to channel weight

    for (byte a = 1; a < 14; a++) {
      if (a > 1) {
        channels[a][1] += channels[a - 1][1] / 2;
      }
      if (a > 2) {
        channels[a][1] += channels[a - 2][1] / 4;
      }
      if (a < 13) {
        channels[a][1] += channels[a + 1][1] / 2;
      }
      if (a < 12) {
        channels[a][1] += channels[a + 2][1] / 4;
      }
    }

    // bubble sort the list by weight

    bool swapped = 1;
    while (swapped) {
      for (byte a = 1; a < 13; a++) {
        if (a == 1) {
          swapped = 0;
        }
        if (channels[a][1] > channels[a + 1][1]) {
          byte buf = channels[a][0];
          channels[a][0] = channels[a + 1][0];
          channels[a + 1][0] = buf;

          buf = channels[a][1];
          channels[a][1] = channels[a + 1][1];
          channels[a + 1][1] = buf;
          swapped = 1;
        }
      }
    }

    // print the sorted results
//
//    for (byte a = 1; a < 14; a++) {
//      Serial.print(channels[a][0]);
//      Serial.print(" - ");
//      /Serial.print(channels[a][1]);
//    }

    WiFi.scanDelete();

    return channels[1][0];
  }
} 
