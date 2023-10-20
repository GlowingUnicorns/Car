// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up three NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with, only this time they're going
// to be all parts of an array of arrays.

#include <FastLED.h>

#define NUM_STRIPS A0
#define NUM_LEDS_PER_STRIP 13
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on pin 2
  FastLED.addLeds<NEOPIXEL, 2>(leds[0], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 3
  FastLED.addLeds<NEOPIXEL, 3>(leds[1], NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 4
  FastLED.addLeds<NEOPIXEL, 4>(leds[2], NUM_LEDS_PER_STRIP);

  FastLED.addLeds<NEOPIXEL, 5>(leds[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds[4], NUM_LEDS_PER_STRIP);
}
uint16_t vals[5] = {150,150,150,150,150};
uint8_t nums[5] = {4,13,9,6,6};

void loop() {
  for(int i = 0; i < 13;i++){
    for(int x = 0; x < 5; x++){
      vals[x] += random(-10,10);
      if(vals[x]<0||vals[x]>=255){
        vals[x] = 125;
      }



      if(x == 0){
        if(i < map(vals[x],0,255,0,nums[x])){
          leds[x][i] = CRGB::Red;
        }
  
        else leds[x][i] = CRGB::Black;
        
        if(i>3){if(3 < map(vals[x],0,255,0,nums[x])){
          leds[x][3] = CRGB::Red;}
          else{
            leds[x][3]=CRGB::Black;
          }
        }
      }
      
      FastLED.show();
      if(i == nums[x]-1)break;

    }
  }
  
}
