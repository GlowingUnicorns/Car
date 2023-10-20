#include <TM1637Display.h>
#include <SPI.h>
#include <FastLED.h>

#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 6
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

TM1637Display disp1(9,8);
TM1637Display disp2(9,7);
uint8_t nums[2] = {6,6}; 
volatile uint16_t vals[2] = {0,0};
volatile uint16_t future[2] = {200,50};

void setup() {
  FastLED.addLeds<NEOPIXEL, A1>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, A0>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(20);
  disp1.setBrightness(0x0f);
  disp2.setBrightness(0x0f);
  disp1.showNumberDec(5,false);
  disp2.showNumberDec(22,false);
}

void loop() {
  strip1();//current
  strip2();//voltage
  for(int i = 0; i < 2;i++){
    if(future[i] >vals[i])vals[i]+=1;
    if(future[i] <vals[i])vals[i]-=1;
  }
  FastLED.show();

}
