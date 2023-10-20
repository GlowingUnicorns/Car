#include <Arduino_GFX_Library.h>
#include "FreeMono8pt7b.h"
#include "FreeSansBold10pt7b.h"
#include "FreeSerifBoldItalic12pt7b.h"

#include <SPI.h>



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

void test();

void setup1(){

  SPI1.begin();
  SPI1.setRX(12);
  SPI1.setCS(13);
  SPI1.setSCK(14);
  SPI1.setTX(15);
  SPI1.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE1));
  digitalWrite(13,LOW);
  SPI1.transfer(0x69);
  SPI1.transfer(0x69);
  SPI1.transfer(0x69);
  SPI1.transfer(0x69);
  SPI1.transfer(0x69);
  SPI1.transfer(0x05);
  SPI1.transfer(0x06);
  SPI1.transfer(0x07);
  SPI1.transfer(0x08);
  SPI1.transfer(0x09);
  SPI1.transfer(0x0A);
  SPI1.transfer(0x0C);
  digitalWrite(13,HIGH);
  
}

void setup() {
  
  #ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
  #endif
  Serial.begin(9600);
  if(!gfx->begin())Serial.println("gfx->begin() failed");
  if(!gfx2->begin())Serial.println("gfx2->begin() failed");
  if(!gfx3->begin())Serial.println("gfx3->begin() failed");
  
  gfx->fillScreen(BLACK);
  gfx2->fillScreen(BLACK);
  gfx3->fillScreen(BLACK);

  uint16_t temp1 = 100;
  uint16_t temp2 = 48;
  uint16_t temp3 = 56;

  uint16_t cont1 = 50;
  uint16_t cont2 = 64;
  uint16_t cont3 = 23;

  uint8_t tempMax = 100;
  uint8_t tempMin = 20; 

  ringMeter(gfx ,120,120,80,110,120,420,15,5,10,temp1,tempMin,tempMax);
  ringMeter(gfx2,120,120,80,110,120,420,10,5,10,temp2,tempMin,tempMax);
  ringMeter(gfx3,120,120,80,110,120,420,10,5,10,temp3,tempMin,tempMax);

  ringMeter(gfx,120,120,60,70,120,420,2,0,6,cont1,0,100);
  ringMeter(gfx2,120,120,60,70,120,420,2,0,7,cont2,0,100);
  ringMeter(gfx3,120,120,60,70,120,420,2,0,8,cont3,0,100);

  numMeter(gfx,125,110,temp2,tempMin,tempMax,10,248,4);
  numMeter(gfx2,125,110,temp1,tempMin,tempMax,10,248,4);
  numMeter(gfx3,125,110,temp1,tempMin,tempMax,10,248,4);

  numMeter(gfx,125,140,cont1,0,100,6,37,2);
  numMeter(gfx2,125,140,cont1,0,100,7,37,2);
  numMeter(gfx3,125,140,cont1,0,100,8,37,2);

  labelMeter(gfx,125,210,6,"Battery",2);
  labelMeter(gfx2,125,210,7,"Engine",2);
  labelMeter(gfx3,125,210,8,"Control",2);

}


void loop() {
  // put your main code here, to run repeatedly:

}
