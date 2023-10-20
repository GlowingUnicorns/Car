#include "SPI.h"
#include "TFT_22_ILI9225.h" // Use the appropriate TFT library for your display
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  7  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 0

#define TFT_BRIGHTNESS 200

#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>

const int centerX = ILI9225_LCD_HEIGHT / 2;
const int centerY = ILI9225_LCD_WIDTH/ 2;
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
int16_t x=0, y=0, w, h;

void setup() {
  
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
  String s1 = "6789";
  tft.setGFXFont(&FreeSans12pt7b); // Set current font
  tft.getGFXTextExtent(s1, x, y, &w, &h); // Get string extents
  y=h;
  tft.drawGFXText(centerX+20, 2*y, s1, COLOR_RED); // Print string
  tft.drawGFXText(centerX+20, ILI9225_LCD_WIDTH-y, s1, COLOR_RED); // Print string

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
void loop() {
  // Define the coordinates of the vertices of the irregular pentagon
  int vertices[5][2] = {
    {-40, 0},
    {8, -16},
    {56, -16},
    {56, 16},
    {-56, 16}
  };


  // Draw the rotated irregular pentagon
  drawRotatedIrregularPentagon(centerX, centerY, vertices, angleDegrees, COLOR_WHITE);
  delay(220);
  drawRotatedIrregularPentagon(centerX, centerY, vertices, angleDegrees, COLOR_BLACK);
  if(increasing){
    angleDegrees++; 
    if(angleDegrees >= 45)increasing = false;
  }
  else{
    angleDegrees--;
    if(angleDegrees<= -45) increasing = true; 
  }

}
