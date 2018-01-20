
/**
 * Read serial for a number between 0 and 762 to set he value of ALL pixels.
 * Use to see how much power the strip uses.
 */

#include <SPI.h>
#include <stdint.h>

#define SPI_FREQ 8000000L
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0



#define NUMPIXELS 240



// LED values (3 bytes each)
uint8_t pixels[NUMPIXELS][3];

// Junk from the spi response; 
uint8_t spi_return;

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

void setPixels(int val) {
  uint8_t w;

  w = val / 3;
  Serial.print("Got: "); Serial.print(val);
  Serial.print(" setting: "); Serial.println(w);
  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    setPixel(i, w, w, w);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  // Fill the pixel data with zeros
  memset(pixels, 0 , NUMPIXELS);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
  showPixels();

  setPixels(30);
  showPixels();
}


void loop() {
  while (Serial.available() > 0) {
    int serval = Serial.parseInt();
    Serial.print("Serial val: "); Serial.println(serval);
    if (Serial.read() == '\n') {
      //int val = constrain(serval, 0, 762); // 3.1 amps a6 5v
      int val = constrain(serval, 0, 300); // Just over 2.3 amps at 5v
      setPixels(val);
      showPixels();
    }
  }
}

