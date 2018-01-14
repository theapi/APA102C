
#include <SPI.h>
#include <stdint.h>

#define SPI_FREQ 8000000L
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0

#define INDEX_RED 0
#define INDEX_GREEN 1
#define INDEX_BLUE 2

#define NUMPIXELS 60
#define NUMPIXEL_BYTES NUMPIXELS * 3

#define LFSR_TAP_1 0
#define LFSR_TAP_2 2
#define LFSR_TAP_3 3
#define LFSR_TAP_4 5

// 64 bit linear feedback shift register.
// Seeded with 1, as it shifts right;
uint64_t lfsr = (uint64_t) 1 << NUMPIXELS;

// LED RGB values (3 bytes eaach)
uint8_t pixels[NUMPIXEL_BYTES];

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
  do {
    // For each pixel...
    // Pixel start, 3 bits of 1, then 5 bits of brightness.
    spi_return = SPI.transfer(0xFF);
    for (i = 0; i < 3; i++) {
      // Write R,G,B
      spi_return = SPI.transfer(pixels[n + i]);
    }
  } while(--n);
  
  // End frame
  for(i = 0; i < ((NUMPIXELS + 15) / 16); i++) {
    spi_return = SPI.transfer(0xFF);
  }
}

// Set pixel color, separate R,G,B values (0-255 each)
void setPixelColor (uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if (n < NUMPIXELS) {
    uint8_t *p = &pixels[n * 3];
    p[INDEX_RED] = r;
    p[INDEX_GREEN] = g;
    p[INDEX_BLUE] = b;
  }
}

void setPixels() {
  uint8_t state = 0;
  for (uint8_t i = NUMPIXELS; i > 0; i--) {
    //setPixelColor(i, 110, 100, 110);
    state = bitRead(lfsr, i); Serial.print(state);
    if (state) {
      setPixelColor (i, 0, 100, 0);
    }
    else {
      setPixelColor (i, 0, 0, 0);
    }
  }
  Serial.println();
}

void shift() {
  lfsr = lfsr >> 1;

  // Four taps, exclusive ORed together.
  uint8_t bit = (
    (lfsr >> LFSR_TAP_1) ^ (lfsr >> LFSR_TAP_2) ^ (lfsr >> LFSR_TAP_3) ^ (lfsr >> LFSR_TAP_4) 
  ) & 1;

  //uint8_t bit = bitRead(lfsr, 0);

  // Not using bitWrite() as it cannot use 64bit 
  if (bit) {
    lfsr |= (uint64_t) 1 << NUMPIXELS;
  }
  else {
    lfsr &= ~((uint64_t) 1 << NUMPIXELS);
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  // Fill the pixel data with zeros
  memset(pixels, 0 , NUMPIXEL_BYTES);
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, SPI_ORDER, SPI_MODE));
}

void loop() {
  showPixels();
  shift();
  setPixels();
  
  delay(250);
  
}
