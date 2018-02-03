
/**
 * Control the shelf lighting.
 * NB: White LEDs only
 */

#define NUMPIXELS 152
#define STRIP_MAX_VALUE 765 // about 45 watts = 9 amps at 5v

#include "config.h" 
#include <SPI.h>
#include <stdint.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>
#include "ws_html.h"

#define PIN_ENCODER_A D2
#define PIN_ENCODER_B D3

#define SPI_FREQ 8000000L
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0

const char* ssid = WIFI_SSID;
const char* password = WIFI_PWD;
const byte DEBUG_LED = 16;

ESP8266WebServer webserver(80);
WebSocketsServer websocket = WebSocketsServer(81);

// LED values (3 bytes each)
uint8_t pixels[NUMPIXELS][3];

volatile uint16_t brightness = 3;
volatile uint8_t brightness_changed = 0;


void setup() {
  pinMode(PIN_ENCODER_A, INPUT_PULLUP); 
  pinMode(PIN_ENCODER_B, INPUT_PULLUP);
  
  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, LOW);  // LOW = ON
  
  stripSetup();
  
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
  websocketSetup();

  // Brighten the strip on completion of setup.
  stripSetAllPixels(10);
  stripShow();

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), encoder_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), encoder_ISR, CHANGE);

}


void loop() {
  ArduinoOTA.handle();
  webserver.handleClient();
  websocket.loop();

  if (brightness_changed) {
    brightness_changed = 0;
    stripSetAllPixels(brightness);
    stripShow();
  }
}

