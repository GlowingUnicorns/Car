
#include <FastLED.h>
#include <Wire.h>

#define NUM_STRIPS 5
#define NUM_LEDS_PER_STRIP 15
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

void strip1();
void strip2();
void strip3();
void strip4();
void strip5();

void setup() {

  Wire.begin(0x08);
  Wire.onReceive(recv);                     
//  lcd.print("test");                  
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, 3>(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, A3>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds[2], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds[3], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds[4], NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(20);
}
uint8_t nums[5] = {4,15,9,15,5}; 
volatile uint16_t vals[10] = {0,0,0,0,0};
volatile uint16_t future[5] = {0,0,0,0,0};
uint16_t counter = 0;
uint16_t counter2 = 0;

void loop() {
  if(++counter >=300)counter = 0;
  strip1(counter);//loading
  strip2();//current
  strip3();//temp
  strip4();//velocity
  strip5();//voltage
  FastLED.show();
  for(int i = 1; i < 5;i++){
    if(future[i] >vals[i])vals[i]+=2;
    if(future[i] <vals[i])vals[i]-=2;
  }
  
  
}

void recv(){
  uint16_t i = 0;
  while(Wire.available()>0){
    future[i++] = Wire.read();
    //Wire.read();
  }
}
