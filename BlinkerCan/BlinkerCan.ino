#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <mcp2515.h>
struct can_frame canMsg;
MCP2515 mcp2515(10);
LiquidCrystal_I2C lcd(0x27,20,4);

#define horn 3
#define leftTurn 4
#define rightTurn 5
#define brakePress 6
#define reverseLight 7
#define leftLight 9
#define rightLight 8
void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  lcd.init();
  lcd.backlight();
//  mcp2515.setFilterMask(0,0,0b00111000000);
//  mcp2515.setFilter(0,0,    0b00010000000);
  pinMode(horn,INPUT);
  pinMode(leftTurn,INPUT);
  pinMode(rightTurn,INPUT);
  pinMode(brakePress,INPUT);
  pinMode(reverseLight,OUTPUT);
  pinMode(leftLight,OUTPUT);
  pinMode(rightLight,OUTPUT);
  digitalWrite(leftTurn,LOW);
  digitalWrite(rightTurn,LOW);
  digitalWrite(reverseLight,LOW);
  Serial.begin(1200);
}
uint16_t counter = 0;
uint16_t counter2 = 0;
uint16_t rpm = 0;
double v = 0; 
double i = 0;
uint16_t t = 0; 
uint16_t timer = 0; 
uint8_t list[5] = {1,2,3,4,5};
bool asc = true; 
void loop() {
  counter2++;
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      switch(canMsg.can_id){
      case 0x330:
        rpm = (canMsg.data[5] << 8) + canMsg.data[4];
        break;
      case 0x271:
        v = (canMsg.data[1]*255 + canMsg.data[0]) * 0.0625;
        i = (canMsg.data[4] * 255 + canMsg.data[3]) * 0.0625;
        if (i < 0) i = 0; 
        break;
    }    
  }
  volatile bool a = digitalRead(leftTurn);
  volatile bool b = digitalRead(rightTurn);
  if(a||b){
    counter++;
    if(counter < 500){
      if(a)digitalWrite(leftLight,HIGH);
      if(b)digitalWrite(rightLight,HIGH);
    }
    else if(counter < 1000){
      digitalWrite(leftLight,LOW);
      digitalWrite(rightLight,LOW);
    }
    else counter = 0; 
  }
  else{
    digitalWrite(leftLight,LOW);
    digitalWrite(rightLight,LOW); 
  }
  if(counter2 >= 100){
    
    if(asc)list[1]+=random(0,10);
    else list[1]-=random(0,10);
    if(list[1]<1){asc = true;list[1] =0;}
    else if (list[1] >99){asc = false;list[1]=100;}
    
    uint8_t startByte = 0xFF;
    uint8_t stopByte = 0xFE;
    Serial.write(startByte);
    delay(10);
    for (int i = 0; i < 5; i++) {
      Serial.write(list[i]);
      delay(20); 
    }
    Serial.write(stopByte);
    
    counter2 = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    if(a)lcd.write("Left");
    if(b)lcd.write("Right");
  }
}
