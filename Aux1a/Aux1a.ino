#include "SPI.h"
#include "TFT_22_ILI9225.h" // Use the appropriate TFT library for your display
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  7  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 0
#include "LedControl.h"

#define TFT_BRIGHTNESS 200

#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>

const int centerX = ILI9225_LCD_HEIGHT / 2-20;
const int centerY = ILI9225_LCD_WIDTH/ 2;

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

int16_t x=0, y=0, w, h;

LedControl lc1=LedControl(4,3,5,1);
LedControl lc2=LedControl(4,3,6,2);

void setup() {  
  lc1.shutdown(0,false);
  lc2.shutdown(0,false);
  lc1.setIntensity(0,8);
  lc2.setIntensity(0,8);
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);
  
  lc1.setDigit(0,7,0,false);
  lc1.setDigit(0,6,0,true);
  lc1.setDigit(0,5,0,false);

  lc1.setDigit(0,3,0,false);
  lc1.setDigit(0,2,0,true);
  lc1.setDigit(0,1,0,false);

  lc2.setChar(0,7,'-',false);
  lc2.setDigit(0,6,0,false);
  lc2.setDigit(0,5,0,true);
  lc2.setDigit(0,4,4,false);

//  lc2.setChar(0,3,'-',false);
  lc2.setDigit(0,2,0,false);
  lc2.setDigit(0,1,0,true);
  lc2.setDigit(0,0,2,false);

  tft.begin();
  tft.setOrientation(1);  // Adjust the rotation as needed
  tft.setBackgroundColor(COLOR_BLACK);
  tft.drawCircle(centerX,centerY,85,COLOR_WHITE);
  for (int angle = 90; angle <= 270; angle += 30) {
    // Calculate the coordinates of the marking
    uint8_t radius = 85;
    float angleRadians = angle * (PI / 180.0);
    int x1 = int(centerX + (radius) * cos(angleRadians));
    int y1 = int(centerY + (radius) * sin(angleRadians));
    int x2 = int(centerX + (radius - 15) * cos(angleRadians));
    int y2 = int(centerY + (radius - 15) * sin(angleRadians));

    // Draw the marking line
    tft.drawLine(x1, y1, x2, y2, COLOR_WHITE);
  }
  tft.fillRectangle(centerX+1,0,centerX*2,centerY*2,COLOR_BLACK);
  String s1 = "6789 pitch";
  tft.setGFXFont(&FreeSans12pt7b); // Set current font
  tft.getGFXTextExtent(s1, x, y, &w, &h); // Get string extents
  y=h;


//  tft.drawText(centerX,centerY,s1,COLOR_RED);
}

void drawRotatedIrregularPentagon(int cx, int cy, int vertices[][2], float angleDegrees, uint16_t color) {
  float angleRadians = angleDegrees * (PI / 180.0);
  float cosA = cos(angleRadians);
  float sinA = sin(angleRadians);

  for (int i = 0; i < 5; i++) {
    int x1 = int(cx + (vertices[i][0]) * cosA - (vertices[i][1]) * sinA);
    int y1 = int(cy + (vertices[i][0]) * sinA + (vertices[i][1]) * cosA);

    int x2 = int(cx + (vertices[(i + 1) % 5][0]) * cosA - (vertices[(i + 1) % 5][1]) * sinA);
    int y2 = int(cy + (vertices[(i + 1) % 5][0]) * sinA + (vertices[(i + 1) % 5][1]) * cosA);

    tft.drawLine(x1, y1, x2, y2, color);
  }
}
float angleDegrees = 0.0;
bool increasing = true; 
int vertices[5][2] = {
  {-40, 0},
  {8, -16},
  {56, -16},
  {56, 16},
  {-56, 16}
};
  
void loop() {
  // Define the coordinates of the vertices of the irregular pentagon
  String s1 = "pitch:"+String(-0.4);
  tft.drawGFXText(centerX, 2*y, s1, COLOR_RED); // Print string
  String s2 = "roll:"+String(0.2);
  tft.drawGFXText(centerX+20, ILI9225_LCD_WIDTH-y, s2, COLOR_RED); // Print string
  drawRotatedIrregularPentagon(centerX, centerY, vertices, angleDegrees, COLOR_WHITE);
  delay(220);
//  drawRotatedIrregularPentagon(centerX, centerY, vertices, angleDegrees, COLOR_BLACK);
//  tft.drawGFXText(centerX, 2*y, s1, COLOR_BLACK); // Print string
//  tft.drawGFXText(centerX+20, ILI9225_LCD_WIDTH-y, s2, COLOR_BLACK); // Print string

//  if(increasing){
//    angleDegrees++; 
//    if(angleDegrees >= 45)increasing = false;
//  }
//  else{
//    angleDegrees--;
//    if(angleDegrees<= -45) increasing = true; 
//  }

}
