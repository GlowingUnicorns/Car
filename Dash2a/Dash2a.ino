#include <Wire.h>
#include <Arduino_GFX_Library.h>
#define GFX_BL DF_GFX_BL 
#define BATPOT 26
#define MOTPOT 27
#define DRVPOT 28
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
uint16_t temp1[2] = {0,0};
uint16_t temp2[2] = {0,0};
uint16_t temp3[2] = {0,0};

uint16_t cont1[2] = {0,0};
uint16_t cont2[2] = {0,0};
uint16_t cont3[2] = {0,0};

uint8_t tempMax = 100;
uint8_t tempMin = 20; 
volatile uint8_t buf[10];
void setup(){
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();
  Serial.begin(115200);

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
  
}
void loop(){
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

  

  //recvBMS();
  buf[0] = 28;
  buf[1] = 25;
  buf[2] = 21;

  sendBMS();
  temp1[1] = buf[0];
  temp2[1] = buf[1];
  temp3[1] = buf[2];

  delay(100);
  
}


void recvBMS(){
  Wire1.requestFrom(0x10,11,true);
  uint8_t i = 0; 
  bool startFound = false; 
  uint8_t temp = 0; 
  int j = 0; 
  while(0 < Wire1.available()){
    temp = Wire1.read(); 
    if(!startFound && temp == 99){
      startFound = true; 
    }
    else if(startFound){
      buf[j++] = temp; 
      Serial.print(temp);
      Serial.print(" ");

    }
    if(i++ >= 8)break;
  }
  Serial.println("");
}


void sendBMS(){
  Wire1.beginTransmission(0x10);
  Wire1.write(cont1[1]);
  Wire1.write(cont2[1]);
  Wire1.write(cont3[1]);
  Wire1.write(10);
  Wire1.write(11);
  Wire1.write(12);
  Wire1.write(13);

  Wire1.endTransmission();
}
