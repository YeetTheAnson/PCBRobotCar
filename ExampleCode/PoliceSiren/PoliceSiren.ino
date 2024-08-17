#include <Adafruit_NeoPixel.h>

#define PIN 4
#define NUMPIXELS 16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); 
  pixels.show(); 
}

void loop() {
  policeSiren(255, 0, 0, 0, 0, 255, 100); 

void policeSiren(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, int wait) {
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(r1, g1, b1));  
    pixels.setPixelColor(i + 8, pixels.Color(r2, g2, b2)); 
  }
  pixels.show();
  delay(wait);

  for (int i = 0; i < 16; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
  delay(wait);

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(r1, g1, b1));
      pixels.setPixelColor(i + 8, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(wait);

    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + 8, pixels.Color(r2, g2, b2));
    }
    pixels.show();
    delay(wait);
  }

  for (int k = 0; k < 6; k++) {
    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(r1, g1, b1)); 
    }
    pixels.show();
    delay(wait / 2);

    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(wait / 2);

    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(r2, g2, b2));
    }
    pixels.show();
    delay(wait / 2);

    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(wait / 2);
  }
}
