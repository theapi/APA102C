
/**
 * Control the shelf lighting.
 */

#include "config.h" 
#include <SPI.h>
#include <stdint.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PWD;

#define SPI_FREQ 8000000L
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0



#define NUMPIXELS 144

const byte DEBUG_LED = 16;

ESP8266WebServer server = ESP8266WebServer(80);

// LED values (3 bytes each)
uint8_t pixels[NUMPIXELS][3];

// Junk from the spi response; 
uint8_t spi_return;

void serverRoot() {
  uint8_t set = 0;
  for (uint8_t i = 0; i < server.args(); i++ ) {
    if (server.argName(i).indexOf('b') == 0) {
       String d = server.argName(i).substring(1);
       int requested_val = server.arg(i).toInt();
       //int val = constrain(requested_val, 0, 762);
       int val = constrain(requested_val, 0, 300);
       set = setPixels(val);
       showPixels();
    }
  }
  
  // Respond with json
  String message = "{";
  message += "\"set\": \"" + String(set) + "\"";
  message += "}\n";
  
  server.send(200, "application/json", message);
}

// Send the pixel data to the led strip.
void showPixels(void) {
  uint8_t i;
  uint16_t n = NUMPIXELS; 
  
  // 4 bytes of 0 as the start frame
  for (i = 0; i < 4; i++) {
    spi_return = SPI.transfer(0x00);
  }

  // Pixel frame
  for (n = 0; n < NUMPIXELS; n++) {
    // For each pixel...
    // Pixel start, 3 bits of 1, then 5 bits of brightness.
    spi_return = SPI.transfer(0xFF);
    spi_return = SPI.transfer(pixels[n][0]);
    spi_return = SPI.transfer(pixels[n][1]);
    spi_return = SPI.transfer(pixels[n][2]);
  }

  // End frame
  for(i = 0; i < ((NUMPIXELS + 15) / 16); i++) {
    spi_return = SPI.transfer(0xFF);
  }
}

void setPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if (n < NUMPIXELS) {
    pixels[n][0] = r;
    pixels[n][1] = g;
    pixels[n][2] = b;
  }
}

uint8_t setPixels(int val) {
  uint8_t w;

  w = val / 3;
  Serial.print("Got: "); Serial.print(val);
  Serial.print(" setting: "); Serial.println(w);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    setPixel(i, w, w, w);
  }

  return w;
}


void setup() {
  // Fill the pixel data with zeros
  memset(pixels, 0 , NUMPIXELS);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  showPixels();
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting OTA");
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  pinMode(DEBUG_LED, OUTPUT);
  digitalWrite(DEBUG_LED, LOW);  // LOW = ON
  
  ArduinoOTA.setHostname("shelves");
  ArduinoOTA.onStart([]() {
    digitalWrite(DEBUG_LED, HIGH);
    //Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });
  ArduinoOTA.begin();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  setPixels(30);
  showPixels();

  server.on("/", serverRoot);
  server.begin();
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}

