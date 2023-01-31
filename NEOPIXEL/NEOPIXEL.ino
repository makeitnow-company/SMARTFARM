#include <Adafruit_NeoPixel.h>

#define PIN 19 //GPIO19번 핀에 식물 LED 연결
#define NUMPIXELS 9

void setup() {
  pixels.begin();
}

void loop() {
  for(int i=0; i<NUMPIXELS; i+=2){
    pixels.setPixelColor(i, pixels.Color(255,0,0));
  }
  
  for(int i=1; i<NUMPIXELS; i+=2){
    pixels.setPixelColor(i, pixels.Color(0,0,255));
  }
  pixels.show();
}
