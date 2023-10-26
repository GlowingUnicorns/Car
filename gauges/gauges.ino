/*
 An example showing 'ring' analogue meter on a HX8357 TFT
 colour screen

 Needs Fonts 2, 4 and 7 (also Font 6 if using a large size meter)
 */
#include <FastLED.h>
#define NUM_LEDS 10
#define DATA_PIN 9
CRGB leds[NUM_LEDS];

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6

#define IN1 7
#define IN2 8



// #include <avr/dtostrf.h> // Only needed for Due

#include <TFT_HX8357.h> // Hardware-specific library

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library

#define HX8357_GREY 0x3104 // Dark grey 16 bit colour

uint32_t runTime = -99999;       // time for next update

int reading = 0; // Value to be displayed
int d = 0; // Variable used for the sinewave test waveform
boolean alert = 0;
int8_t ramp = 1;
int xpos = 480/2 - 140, ypos = (320-140*2), gap = 15, radius = 140;
void setup(void) {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  FastLED.clear();

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);

  pinMode(IN1,INPUT_PULLUP);
  pinMode(IN2,INPUT_PULLUP);

  tft.begin();
  Serial.begin(9600);
  Serial3.begin(1200);

  tft.setRotation(1);

  tft.fillScreen(HX8357_BLACK);
  //tft.fillCircle(480/2-1, 320/2+ypos/2, radius+12, TFT_WHITE);
  tft.fillCircle(480/2-1, 320/2+ypos/2, radius+9, TFT_BLACK);
  tft.fillRect(0,300,480,300,TFT_BLACK);
  //Serial3.setTimeout(10);


}
uint8_t receivedData[5];
uint8_t currentData[5];
uint16_t counter = 0; 

void loop() {

  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  digitalWrite(LED5,HIGH);
  waitForData();
  Serial.print("Received data[");
  for (int i = 0; i < 5; i++) {
    // Print received values to the Serial Monitor
    Serial.print(receivedData[i]);
  }
  Serial.println("]: ");
  for(int i = 0; i < 4;i++){
    if(currentData[i]!=receivedData[i]){
      int j = abs(currentData[i]-receivedData[i])/2 + 1;
      if(currentData[i]<receivedData[i])currentData[i] += j; 
      else if (currentData[i]>receivedData[i]) currentData[i] -= j; 
    }
  }
  currentData[1] = 0;
  FastLED.clear();
  for (int i = 0; i < map(30, 0, 100, 0, NUM_LEDS); i++) {
    leds[i] = CRGB::Purple;
  }
  FastLED.show();

  reading +=(ramp);
  if (reading>98) ramp = -10;
  if (reading<0) ramp = 10;
  ringMeter(currentData[1],0,100, xpos,ypos,radius," mph",3);
  lineMeter(currentData[0],0,102, 15, 55, 10, "amps");
  lineMeter(88,0,90, 480-55-15, 55, 10, "volts");

}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################

int lineMeter(int value, int vmin, int vmax, int x, int w, int h, char *units){
  int minHeight = 20;int i = 200;
  uint16_t colour= rainbow(map(0,vmin,vmax,0,127));

  for(int i = vmin; i< vmax; i += (vmax-vmin)/10){
    int Y1 = map(i,vmin,vmax,240,20);

    if(i<=value){
      colour= rainbow(map(i,vmin,vmax,0,127));
      tft.fillRect(x,Y1,w,h,colour);
    } 
    else{
    tft.fillRect(x,Y1,w,h,HX8357_GREY);
    }
  }


  char buf[10];
  byte len = 2;
  if(vmax < 100)len = 2;
  else len =3;
  if (value > 999) len = 5;
  dtostrf(value, len, 0, buf);
  buf[len] = ' '; buf[len+1] = 0; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  
  tft.setTextColor(colour, HX8357_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  tft.setTextPadding(0); // Allow for 3 digits each 14 pixels wide
  tft.drawString(buf, x+32, 280, 7); // Value in middle
  tft.setTextPadding(0);
  // Print units, if the meter is large then use big font 4, othewise use 2
  //tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.drawString(units, x+50, 310, 2); // Units display

}


int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option
  
  x += r; y += r;   // Calculate coords of centre of ring
  
  int w = r / 4;    // Width of outer ring is 1/4 of radius
  
  int angle = 150;  // Half the sweep angle of meter (300 degrees)

  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

  byte seg = 3; // Segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 7; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Variable to save "value" text colour from scheme and set default
  int colour = HX8357_BLUE;
  uint16_t textColor = HX8357_WHITE;

  // Draw colour blocks every inc degrees
  for (int i = -angle+inc/2; i < angle-inc/2; i += inc) {
    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * 0.0174532925);
    float SY = sin((i - 90) * 0.0174532925);
    uint16_t X0 = sx * (r - w) + x;
    uint16_t Y0 = SY * (r - w) + y;
    uint16_t X1 = sx * r + x;
    uint16_t Y1 = SY * r + y;

    // Calculate pair of coordinates for segment end
    float sX2 = cos((i + seg - 90) * 0.0174532925);
    float sY2 = sin((i + seg - 90) * 0.0174532925);
    int X2 = sX2 * (r - w) + x;
    int Y2 = sY2 * (r - w) + y;
    int X3 = sX2 * r + x;
    int Y3 = sY2 * r + y;
    if (i < v) { // Fill in coloured segments with 2 triangles
      switch (scheme) {
        case 0: colour = HX8357_RED; break; // Fixed colour
        case 1: colour = HX8357_GREEN; break; // Fixed colour
        case 2: colour = HX8357_BLUE; break; // Fixed colour
        case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
        case 4: colour = rainbow(map(i, -angle, angle, 70, 127)); break; // Green to red (high temperature etc)
        case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
        default: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Fixed colour
      }
      textColor = colour; 
    }
    else // Fill in blank segments
    {
      colour = HX8357_GREY;
    }
    tft.fillTriangle(X0, Y0, X1, Y1, X2, Y2, colour);
    tft.fillTriangle(X1, Y1, X2, Y2, X3, Y3, colour);
  }
  char buf[10];
  byte len = 3; if (value > 999) len = 5;
  dtostrf(value, len, 0, buf);
  buf[len] = ' '; buf[len+1] = 0; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  tft.setTextSize(1);

  if (value<vmin || value>vmax) {
    drawAlert(x,y+90,50,1);
  }
  else {
    drawAlert(x,y+90,50,0);
  }
  y -= 10;

  tft.setTextColor(textColor, HX8357_BLACK);
  tft.setTextDatum(MC_DATUM);
      tft.setTextSize(1);

  // Print value, if the meter is large then use big font 8, othewise use 4
  if (r > 84) {

    tft.setTextPadding(55*3); // Allow for 3 digits each 55 pixels wide
    tft.drawString(buf, x, y, 7); // Value in middle
  }
  else {
    tft.setTextPadding(3 * 14); // Allow for 3 digits each 14 pixels wide
    tft.drawString(buf, x, y, 4); // Value in middle
  }
  tft.setTextSize(1);
  tft.setTextPadding(0);
  // Print units, if the meter is large then use big font 4, othewise use 2
  //tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  if (r > 84) tft.drawString(units, x, y + 40, 4); // Units display
  else tft.drawString(units, x, y + 15, 2); // Units display

  // Calculate and return right hand side x coordinate
  return x + r;
}

void drawAlert(int x, int y , int side, boolean draw)
{
  if (draw && !alert) {
    tft.fillTriangle(x, y, x+30, y+47, x-30, y+47, rainbow(95));
    tft.setTextColor(HX8357_BLACK);
    tft.drawCentreString("!", x, y + 6, 4);
    alert = 1;
  }
  else if (!draw) {
    tft.fillTriangle(x, y, x+30, y+47, x-30, y+47, HX8357_BLACK);
    alert = 0;
  }
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    if(value%32 < 16){
      blue = 0;
      green = 62;
      red =0 ;
    }
    else{
      blue = (value%32-16)*2;
      green = 62;
      red = 0; 
    }
  }
  if (quadrant == 1) {
    if(value%32 < 16){
      blue = 31;
      green = 62-value%32*4;
      red = 0;      
    }
    else{
      blue = 31;
      green = 0;
      red = (value%32-15)*2;
      if(red > 31) red = 31; 
    }
  }
  if (quadrant == 2) {
    blue = 31-value%32;
    green = 0;
    red = 31;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 0;
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
void waitForData(){
  uint8_t startByte = 0xFF;
  uint8_t stopByte = 0xFE;
  while (Serial3.available() == 0 || Serial3.read() != startByte);
  for (int i = 0; i < 5; i++) {
    while (Serial3.available() == 0);
    receivedData[i] = Serial3.read();
  }
  while (Serial3.available() == 0 || Serial3.read() != stopByte);
}

// #########################################################################
// Return a value in range -1 to +1 for a given phase angle in degrees
// #########################################################################
float sineWave(int phase) {
  return sin(phase * 0.0174532925);
}
