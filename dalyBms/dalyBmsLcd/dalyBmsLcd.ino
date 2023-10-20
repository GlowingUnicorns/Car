#include "LedControl.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <daly-bms-uart.h> 

#define BMS_SERIAL Serial 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




Daly_BMS_UART bms(BMS_SERIAL);

void setup()
{
  bms.Init(); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(15,25);
  display.fillRect(0,0,128,30, WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.print("blep");
  display.display();

}



void loop()
{

  //bms.update();

  //for (size_t i = 0; i < size_t(bms.get.numberOfCells); i++)
  //{
    //Serial.println("Remaining Capacity mAh:      " + (String)bms.get.cellVmV[i]);
  //}
  

   //delay(1000);
}
