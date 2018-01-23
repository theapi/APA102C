
/**
 * Control the shelf lighting.
 */

#define NUMPIXELS 144


#include "config.h" 
#include <SPI.h>
#include <stdint.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#define SPI_FREQ 8000000L
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0

const char* ssid = WIFI_SSID;
const char* password = WIFI_PWD;
const byte DEBUG_LED = 16;

ESP8266WebServer webserver(80);

// LED values (3 bytes each)
uint8_t pixels[NUMPIXELS][3];


void setup() {
  stripSetup();
  
  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, HIGH);  // LOW = ON
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting");
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  otaSetup();
  webserverSetup();
  stripSetAllPixels(30);
  stripShow();
}


void loop() {
  ArduinoOTA.handle();
  webserver.handleClient();
}

