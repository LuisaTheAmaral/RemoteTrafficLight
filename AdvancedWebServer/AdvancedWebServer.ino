/*
 * Contol a relay over WiFi using ESP8266 NodeMCU, D1 Mini to 
 * turn ON or OFF 2 AC bulb or 2 fan or other load using 2 channel relay
 * 
 * Watch Video instrution for this code:  https://youtu.be/F7mK587OQgI
 * Full explanation of this code and wiring diagram is available at
 * my Arduino Course at Udemy.com here: http://robojax.com/L/?id=62

 * Written by Ahmad Shamshiri on Feb 22, 2020 at 15:25
 * in Ajax, Ontario, Canada. www.robojax.com
 * Adapted by Luísa Amaral and Hugo Leal
 * 
 * 
 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63

or make donation using PayPal http://robojax.com/L/?id=64

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
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

int relay1Pin = D4;//define a pin for relay (D1)
int relay2Pin = D0;//define a pin for relay

int relay1State = 1;//initial state . 1 ON, 0 OFF
int relay2State = 0;//initial state . 1 ON, 0 OFF

String button1Title1 ="Ativar luz verde";
String button1Title2 ="Ativar luz vermelha";

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>



#ifndef STASSID
#define STASSID "wifi name" // your WiFi SSID
#define STAPSK  "wifi password" //your WiFi password
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

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
    HTML +=button1Title2 + "</div>";
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=on'\"  type=\"button\" style=\"display: none;\" disabled >";
    //HTML +=button1Title1; 
    HTML += "</div>";
    HTML += "<div style=\"padding:20px; \"><div id=\"ret\"><div id=\"circleRED\" style=\"background-color: #331111;\"></div><br><div id=\"circleGREEN\" ></div></div></div>";
  }else{
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=off'\"  type=\"button\" style=\"display: none;\" disabled >";
    //HTML +=button1Title2;
    HTML += "</div>";
    HTML +="<div class=\"btn\"><button onclick=\"location.href='/control?r1=on'\" style=\"background-color:green;width: 300px;height:150px;border-radius: 5px;font-size:20px;\" type=\"button\" >";
    HTML +=button1Title1 + "</div>";
    HTML += "<div style=\"padding:20px; \"><div id=\"ret\"><div id=\"circleRED\"></div><br><div id=\"circleGREEN\" style=\"background-color: #331111;\" ></div></div></div>"; 
  }
  HTML +="</a></div>";  
  HTML +="</a></div>";
  HTML += "</body></html>";
  server.send(200, "text/html", HTML);  
}//handleRoot()

void handleNotFound() {

  String message = "File Not Found";
  message += "URI: ";
  message += server.uri();
  message += "Method: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "Arguments: ";
  message += server.args();
  message += "";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "";
  }

  server.send(404, "text/plain", message);

}//end of handleNotFound()

void setup(void) {
  pinMode(relay1Pin, OUTPUT);// define a pin as output for relay
  pinMode(relay2Pin, OUTPUT);// define a pin as output for relay
  digitalWrite(relay1Pin, relay1State);//initial state either ON or OFF
  digitalWrite(relay2Pin, relay2State);//initial state either ON or OFF
  
  Serial.begin(115200);//initialize the serial monitor

  //Relay control ON OFF by Robojax.com

    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxESP8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/control", HTTP_GET, relayControl);         
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

}//end of setup

void loop(void) {
  
  server.handleClient();
  MDNS.update();

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
   //Serial.print("Relay1:");Serial.print (relay1State);
   //Serial.print(" Relay2:");Serial.println(relay2State);   
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
