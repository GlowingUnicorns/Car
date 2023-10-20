#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <mcp2515.h>
MCP2515 mcp2515(10);

#define trigPin1 3
#define echoPin1 4
#define trigPin2 5
#define echoPin2 6
#define trigPin3 A0
#define echoPin3 A1
#define trigPin4 A2
#define echoPin4 A3
LiquidCrystal_I2C lcd(0x27,20,4);
long duration, distance;
uint16_t Sensor1,Sensor2,Sensor3,Sensor4;
uint32_t timer = 0; 

struct can_frame canMsg1;


  
void setup()
{
  Serial.begin (9600);
  lcd.init();
  lcd.backlight();
  canMsg1.can_id  = 0b10100110001;
  canMsg1.can_dlc = 8;
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);


  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  timer = millis();
}
void loop() {
  if(millis()-timer > 300){
    mcp2515.sendMessage(&canMsg1);
    timer = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Sensor1);
    lcd.setCursor(8,0);
    lcd.print(Sensor2);
    lcd.setCursor(0,1);
    lcd.print(Sensor3);
    lcd.setCursor(8,1);
    lcd.print(Sensor4);
    
  }
  SonarSensor(trigPin1, echoPin1);
  Sensor1 = distance;
  SonarSensor(trigPin2, echoPin2);
  Sensor2 = distance;
  SonarSensor(trigPin3, echoPin3);
  Sensor3 = distance;
  SonarSensor(trigPin4, echoPin4);
  Sensor4 = distance;
  canMsg1.data[0] = (Sensor2 >> 8) & 0xFF;
  canMsg1.data[1] = Sensor2 & 0xFF;
  canMsg1.data[2] = (Sensor1 >> 8) & 0xFF;
  canMsg1.data[3] = Sensor1 & 0xFF;
  canMsg1.data[4] = (Sensor4 >> 8) & 0xFF;
  canMsg1.data[5] = Sensor4 & 0xFF;
  canMsg1.data[6] = (Sensor3 >> 8) & 0xFF;
  canMsg1.data[7] = Sensor3 & 0xFF;
  Serial.print(Sensor1);
  Serial.print(" - ");
  Serial.print(Sensor2);
  Serial.print(" - ");
  Serial.print(Sensor3);
  Serial.print(" - ");
  Serial.println(Sensor4);


  

}

void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

}
