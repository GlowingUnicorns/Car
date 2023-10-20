#include <TM1637Display.h>
#include <SPI.h>
#include<Wire.h>
#include <mcp2515.h>
#include "LedControl.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct can_frame canMsg;
MCP2515 mcp2515(10);
LedControl lc1=LedControl(A1,A0,A2,1);
LedControl lc2=LedControl(A1,A0,A3,2);
LiquidCrystal_I2C lcd(0x27, 20, 4);

TM1637Display disp3(9,8);
TM1637Display disp5(9,7);
TM1637Display disp4(9,6);
TM1637Display disp6(9,5);
TM1637Display disp1(9,4);
TM1637Display disp2(9,3);

volatile uint8_t arr[100] = {1,2,3,4,5,6,7,8,9,10};


void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(A6,INPUT);
  disp1.setBrightness(0x0f);
  disp2.setBrightness(0x0f);
  disp3.setBrightness(0x0f);
  disp4.setBrightness(0x0f);
  disp5.setBrightness(0x0f);
  disp6.setBrightness(0x0f);

  lc1.shutdown(0,false);
  lc2.shutdown(0,false);
  lc1.setIntensity(0,8);
  lc2.setIntensity(0,8);
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  lcd.init();
  lcd.backlight();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
//  display.display();
  display.print("test");
  display.display();
}
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };


uint16_t timer = 0; 
uint16_t timer2 = 0; 
uint16_t timer3 = 0; 

uint16_t rpm = 0;
uint16_t mph = 0; 
double v = 88; 
double i = 0;
uint16_t t = 24; 
uint16_t p = 0;


void loop() {
  if(timer++ > 500){
    timer = 0; 
    disp1.showNumberDec(v,false);
    disp2.showNumberDec(i,false);
    disp3.showNumberDec(p,false);
    disp4.showNumberDec(mph,false);
    disp5.showNumberDec(rpm,false);
    disp6.showNumberDec(t,false); 
//  disp1.showNumberDec(1,false);
//  disp2.showNumberDec(2,false);
//  disp3.showNumberDec(3,false);
//  disp4.showNumberDec(4,false);
//  disp5.showNumberDec(5,false);
//  disp6.showNumberDec(6,false);

  }

  if (timer2++ > 2000){
    timer2 = 0; 
    lc1.setChar(0,7,'5',false);
    lc1.setChar(0,6,'o',false);
    lc1.setChar(0,5,'C',false);
  
    lc2.setChar(0,7,'c',false);
    lc2.setChar(0,6,'A',false);
    lc2.setChar(0,5,'P',false);

    lc1.setDigit(0,3,8,false);
    lc1.setDigit(0,2,5,true);
    lc1.setDigit(0,1,8,false);
    lc1.setDigit(0,0,0,false);

    lc2.setDigit(0,3,2,false);
    lc2.setDigit(0,2,2,false);
    lc2.setDigit(0,1,9,false);
    lc2.setDigit(0,0,4,false);
    lcd.setCursor(0,0);
    lcd.print("inactive");
  }
  if (timer3++>150){sendLEDs();}
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    int bufStart = -1; 
    switch(canMsg.can_id){
      case 0x011:bufStart = 0;timer += 400;break;
      case 0x012:bufStart = 8;break;
      case 0x013:bufStart = 16;break;
      case 0x014:bufStart = 24;break;
      case 0x015:bufStart = 32;break;
      case 0x016:bufStart = 40;break;
      case 0x017:bufStart = 48;break;
      case 0x018:bufStart = 56;break;
      case 0x019:bufStart = 64;break;
      case 0x330:
        rpm = (canMsg.data[5] << 8) + canMsg.data[4];
        rpm = rpm*18;
        mph = rpm*3.14*2*60/5280;
        break;
      case 0x271:
        v = (canMsg.data[1]*255 + canMsg.data[0]) * 0.0625;
        i = (canMsg.data[4] * 255 + canMsg.data[3]) * 0.0625;
        p = i*v;
        if (i < 0) i = 0; 
        break;
      case 0x411:
        t = canMsg.data[6]|(canMsg.data[7] << 8);
        break;
    }
    if (bufStart >= 0){
      for (uint8_t i = bufStart; i < bufStart + 8; i++ ) {
        arr[i] = canMsg.data[i-bufStart];
        Serial.print(arr[i]);
      }
    }
    else{

    }
    Serial.println("");

  }
  
}


void sendLEDs(){
  Wire.beginTransmission(0x08);
  Wire.write(255);//bool
  Wire.write(map2(i,0,50,0,255));//current
  Wire.write(map2(t,20,30,0,255));//temp
  Wire.write(map2(mph,0,150,0,255));//speed
  Wire.write(map2(0,65,92,0,255));//volt
  Wire.endTransmission();
}

uint16_t map2(uint16_t value, uint16_t min,uint16_t max, int newMin,int newMax){
  if (value<min)return newMin;
  if (value>max)return newMax;
  return map(value,min,max,newMin,newMax);
}
