#include <Arduino_GFX_Library.h>
#include <TM1637Display.h>
#include "LedControl.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <daly-bms-uart.h>
#define BMS_SERIAL Serial1
Daly_BMS_UART bms(BMS_SERIAL);

#define GFX_BL DF_GFX_BL 

#if defined(DISPLAY_DEV_KIT)
  Arduino_GFX *gfx = create_default_Arduino_GFX();
#else
  Arduino_DataBus *bus = new Arduino_HWSPI(16 /* DC */, 11 /* CS */);
  Arduino_GFX *gfx = new Arduino_GC9A01(bus, DF_GFX_RST, 1 /* rotation */, true /* IPS */);
  
  Arduino_DataBus *bus2 = new Arduino_HWSPI(16 /* DC */, 10 /* CS */);
  Arduino_GFX *gfx2 = new Arduino_GC9A01(bus2, DF_GFX_RST, 1 /* rotation */, true /* IPS */);

  Arduino_DataBus *bus3 = new Arduino_HWSPI(16 /* DC */, 9 /* CS */);
  Arduino_GFX *gfx3 = new Arduino_GC9A01(bus3, DF_GFX_RST, 1 /* rotation */, true /* IPS */);

  
#endif
LedControl lc2=LedControl(14,15,13,1);
LedControl lc1=LedControl(14,15,12,2);


TM1637Display disp3(20,21);
TM1637Display disp5(20,4);
TM1637Display disp4(20,5);
TM1637Display disp6(20,6);
TM1637Display disp1(20,7);
TM1637Display disp2(20,8);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
uint16_t temp1[2] = {0,100};
uint16_t temp2[2] = {0,100};
uint16_t temp3[2] = {0,100};

uint16_t cont1[2] = {0,100};
uint16_t cont2[2] = {0,100};
uint16_t cont3[2] = {0,100};

uint8_t tempMax = 100;
uint8_t tempMin = 20; 

#define BATPOT 28
#define MOTPOT 27
#define DRVPOT 26
volatile uint8_t buf1[31] = {1,2,3,4,5,6,7,8,9,10,11};
volatile uint8_t buf2[31] = {1,2,3,4,5,6,7,8,9,10,11};
volatile uint8_t buf3[31] = {1,2,3,4,5,6,7,8,9,10,11};

void setup() {

  pinMode(BATPOT,INPUT);
  pinMode(MOTPOT,INPUT);
  pinMode(DRVPOT,INPUT);
//
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Test");
  
  lc1.shutdown(0,false);
  lc2.shutdown(0,false);

  lc1.setIntensity(0,8);
  lc2.setIntensity(0,8);
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);


  lc1.setChar(0,7,'5',false);
  lc1.setChar(0,6,'o',false);
  lc1.setChar(0,5,'C',false);

  lc2.setChar(0,7,'s',false);
  lc2.setChar(0,6,'y',false);
  lc2.setChar(0,5,'s',false);
  //lc2.setChar(0,4,'',false);


  disp1.setBrightness(7);
  disp2.setBrightness(7);
  disp3.setBrightness(7);
  disp4.setBrightness(7);
  disp5.setBrightness(7);
  disp6.setBrightness(7);
  disp1.clear();
  disp2.clear();
  disp3.clear();
  disp4.clear();
  disp5.clear();
  disp6.clear();
  
  #ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
  #endif
  //Serial.begin(9600);
  if(!gfx->begin());//Serial.println("gfx->begin() failed");
  if(!gfx2->begin());//Serial.println("gfx2->begin() failed");
  if(!gfx3->begin());//Serial.println("gfx3->begin() failed");
  
  gfx->fillScreen(BLACK);
  gfx2->fillScreen(BLACK);
  gfx3->fillScreen(BLACK);



  ringMeter(gfx ,120,120,80,110,120,420,10,5,10,temp1[0],tempMin,tempMax);
  ringMeter(gfx2,120,120,80,110,120,420,10,5,10,temp2[0],tempMin,tempMax);
  ringMeter(gfx3,120,120,80,110,120,420,10,5,10,temp3[0],tempMin,tempMax);

  ringMeter(gfx ,120,120,60,70,120,420,5,0,6,cont1[0],0,100);
  ringMeter(gfx2,120,120,60,70,120,420,5,0,7,cont2[0],0,100);
  ringMeter(gfx3,120,120,60,70,120,420,5,0,8,cont3[0],0,100);

  numMeter(gfx ,125,110,temp1[0],tempMin,tempMax,10,248,4,0);
  numMeter(gfx2,125,110,temp2[0],tempMin,tempMax,10,248,4,0);
  numMeter(gfx3,125,110,temp3[0],tempMin,tempMax,10,248,4,0);

  numMeter(gfx ,125,140,cont1[0],0,100,6,37,2,0);
  numMeter(gfx2,125,140,cont2[0],0,100,7,37,2,0);
  numMeter(gfx3,125,140,cont3[0],0,100,8,37,2,0);

  labelMeter(gfx ,125,210,6,"Battery",2);
  labelMeter(gfx2,125,210,7,"Engine" ,2);
  labelMeter(gfx3,125,210,8,"Control",2);
  

//  for(int i = 0; i < 8;i++){
//    lc1.setDigit(0,i,random(9),false);
//  }
//  for(int i = 0; i < 8;i++){
//    lc2.setDigit(0,i,random(9),false);
//  }
  



}


void loop() {

  disp1.showNumberDec((double(buf1[0]) + double(buf1[1]) *0.01)*100 ,false);
  disp2.showNumberDec(buf1[2],false);
  disp3.showNumberDec(double(buf1[3] * 10) +double( buf1[4])*0.1,false);
  disp4.showNumberDec(buf1[6],false);
  disp5.showNumberDec(buf1[7],false);
  disp6.showNumberDec(buf1[8],false);
//  disp1.showNumberDec(1,false);
//  disp2.showNumberDec(2,false);
//  disp3.showNumberDec(3,false);
//  disp4.showNumberDec(4,false);
//  disp5.showNumberDec(5,false);
//  disp6.showNumberDec(6,false);
  
  //temp1[1] = lim(temp1[1],20,80);
  //temp2[1] = lim(temp2[1],20,80);
  //temp3[1] = lim(temp3[1],20,80);
  cont1[1] = map(analogRead(BATPOT),0,1023,100,0); if(cont1[1] < 5)cont1[1] = 0;if(cont1[1] > 95)cont1[1] = 100;
  cont2[1] = map(analogRead(MOTPOT),0,1023,100,0); if(cont2[1] < 5)cont2[1] = 0;if(cont2[1] > 95)cont2[1] = 100;
  cont3[1] = map(analogRead(DRVPOT),0,1023,100,0); if(cont3[1] < 5)cont3[1] = 0;if(cont3[1] > 95)cont3[1] = 100;
  updateRing(gfx ,120,120,80,110,120,420,10,5,10,temp1[1],tempMin,tempMax,temp1[0]);
  temp1[0] = temp1[1];

  updateRing(gfx2,120,120,80,110,120,420,10,5,10,temp2[1],tempMin,tempMax,temp2[0]);
  temp2[0] = temp2[1];

  updateRing(gfx3,120,120,80,110,120,420,10,5,10,temp3[1],tempMin,tempMax,temp3[0]);
  temp3[0] = temp3[1];

  
  updateRing(gfx ,120,120,60,70,120,420,5,0,6,cont1[1],0,100,cont1[0]);
  updateRing(gfx2,120,120,60,70,120,420,5,0,7,cont2[1],0,100,cont2[0]);
  updateRing(gfx3,120,120,60,70,120,420,5,0,8,cont3[1],0,100,cont3[0]);

  numMeter(gfx ,125,110,temp1[1],tempMin,tempMax,10,248,4,temp1[0]);

  numMeter(gfx2,125,110,temp2[1],tempMin,tempMax,10,248,4,temp2[0]);

  numMeter(gfx3,125,110,temp3[1],tempMin,tempMax,10,248,4,temp3[0]);


  numMeter(gfx ,125,140,cont1[1],0,100,6,37,2,cont1[0]);
  numMeter(gfx2,125,140,cont2[1],0,100,7,37,2,cont2[0]);
  numMeter(gfx3,125,140,cont3[1],0,100,8,37,2,cont3[0]);
  
  cont1[0] = cont1[1];
  cont2[0] = cont2[1];
  cont3[0] = cont3[1];
  //delay(100);
}

int lim(int x, int mini, int maxi){
  x+= random(-1,2);
  if(x > maxi) x = maxi;
  if(x < mini) x = mini;
  return x;
}
