#define INT 2
#define redSwitch 3
#define whiteLED 4
#define slowSwitch 5
#define RELAY 6

#define Switch1 7 //toggle
#define Switch2 8
#define Switch3 9 

#define Dir1 A1
#define Dir2 A2
#define pot A3
#define INSIG A6
#define OUTSIG A7
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

void setup() {
  digitalWrite(RELAY,LOW);
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("test");
  
  dac.begin(0x60);
  dac.setVoltage(0,false);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();

  
  pinMode(redSwitch,INPUT_PULLUP);
  pinMode(whiteLED,OUTPUT);
  pinMode(slowSwitch,INPUT);
  pinMode(RELAY,OUTPUT);
  
  pinMode(Switch1,INPUT);
  pinMode(Switch2,INPUT);
  pinMode(Switch3,INPUT);

  pinMode(Dir1,INPUT);
  pinMode(Dir2,INPUT);
  pinMode(pot,INPUT);
  pinMode(INSIG,INPUT);
  pinMode(OUTSIG,INPUT);

  

}

uint8_t gear = 0; //0 none, 1 slow, 2 fast
int8_t Direction = 0;// -1 back, 0 neut, 1 forward

int8_t gearDisp = 0; //-2 highback, -1 back, 0 neutral, 1 low, 2 high, 3 raw
int8_t lastGear = 0; 
uint16_t limit = 1023; 
uint8_t limitPercent = 0; 
uint16_t throttle = 0;
uint8_t throttlePercent = 0;
uint16_t output = 0;
uint8_t outputPercent = 0; 

uint16_t counter = 0; 
void loop() {
  if(counter++ > 300){
    counter = 0; 
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PED:");
    lcd.print(throttlePercent);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("OUT:");
    lcd.print(outputPercent);
    lcd.print("% / ");
    lcd.print(limitPercent);
    lcd.print("%");
  }
  bool a = (analogRead(Dir1) > 100);
  bool b = (analogRead(Dir2) > 100);
  if(a==b)Direction = 0;
  else if (a)Direction = 1;
  else Direction = -1;

  if(digitalRead(redSwitch) == HIGH){
    if(digitalRead(slowSwitch) == HIGH) gear = 2;
    else gear = 1;
  }
  else gear = 0; 


  limit = analogRead(pot); 
  limitPercent = int(limit/1023.0*100);
  throttle = analogRead(INSIG);
  throttlePercent = int(throttle/1023.0 * 100);
  output = analogRead(OUTSIG);
  outputPercent = int(output/1023.0*100);
  
  switch(gear){
    case 0:
      digitalWrite(RELAY,LOW);
      break;
    case 1:
      digitalWrite(RELAY,HIGH);
      dac.setVoltage(throttle/2,false);
      if(analogRead(OUTSIG) > (throttle/2 + 100)) digitalWrite(RELAY,LOW);
      break;
    case 2:
      digitalWrite(RELAY,HIGH);
      if(throttle > limit) throttle = limit; 
      dac.setVoltage(throttle*4, false);
      break;
  }
  
  if(Direction == 0){ 
    gearDisp = 0; 
    digitalWrite(whiteLED,HIGH);
    dac.setVoltage(0,false);
  }
  else{
    digitalWrite(whiteLED,LOW);
    if(Direction == 1 && gear == 0) gearDisp = 3;
    else if(Direction == 1 && gear == 1) gearDisp = 1;
    else if(Direction == 1 && gear == 2) gearDisp = 2;
    else if(Direction == -1 && gear == 0) gearDisp = -2;
    else gearDisp = -1;
  }
  if(lastGear != gearDisp){
    display.clearDisplay();
    display.setTextSize(5);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15,0);
    switch(gearDisp){
      case -2:
        display.print("RR");
        break;
      case -1:
        display.print("R");
        break;
      case 0:
        display.print("N");
        break;
      case 1:
        display.print("L");
        break;
      case 2:
        display.print("H");
        break;
      case 3:
        display.print("D");
        break; 
    }
    display.display();
  }

 
  lastGear = gearDisp; 
  
  

}
