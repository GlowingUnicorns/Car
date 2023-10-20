#include <RotaryEncoder.h>
#define PIN_IN1 8
#define PIN_IN2 7
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define relay 5
#define BUT 9
#define ACT 6

#define LED1 A0
#define LED2 A1
#define LED3 A2

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

void setup() {
  pinMode(ACT,INPUT_PULLUP);
  pinMode(BUT,INPUT_PULLUP);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(relay,OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  display.clearDisplay();
  delay(1000);
  Serial.begin(9600);
  
}
boolean PRES[2] = {0,0};
boolean ACTIVATE[2] = {0,0};
boolean active = false;
boolean surge = false; 
int onTime = 10; 
boolean trig = false; 
unsigned long start; 
void loop() {
  encoder.tick();
  
  digitalWrite(LED1,LOW);digitalWrite(LED2,LOW);digitalWrite(LED3,LOW);digitalWrite(relay,LOW);
  static int pos = 0;
  int newPos = encoder.getPosition();
  PRES[0] = digitalRead(BUT);
  ACTIVATE[0] = digitalRead(ACT);
  if (ACTIVATE[0] != ACTIVATE[1]){if(ACTIVATE[0] == HIGH){active = !active;}trig = true;}
  else trig = false; 
  
  if (pos != newPos||((PRES[0] == 0 && PRES[1] == 1))||trig){
    trig = false;     
  
    if(PRES[0] == 0 && PRES[1] == 1) surge = true;
    else{
      surge = false;
    }
    if(surge && active){
      start = millis();
      runSurge(onTime,false);
      while(digitalRead(BUT) == LOW){
        if(millis() - start > 0)digitalWrite(LED1,HIGH);
        if(millis() - start > 500)digitalWrite(LED2,HIGH);
        if(millis() - start > 1000)digitalWrite(LED3,HIGH);
        if(millis() - start > 1500){digitalWrite(relay,HIGH);
        runSurge(onTime,true);

        delay(onTime);
        
        digitalWrite(relay,LOW);
        break;
        }
      }
    }
    pos = newPos;
    onTime += 5*(int)(encoder.getDirection());
    bleh(onTime);

    
  }
  PRES[1] = PRES[0];
  ACTIVATE[1] = ACTIVATE[0];
}

void runSurge(int ms, boolean uh){
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(15,25);
  display.fillRect(0,20,128,40, WHITE);
  display.setTextColor(SSD1306_BLACK);
  if(!uh)
  display.print(" CLEAR");
  else 
  display.print("STNDBY");
  display.display();
  Serial.println("surge");
}
void bleh(int ms){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10,3);
  if(active){
    display.fillRect(0,0,128,30, WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.print("ACTIVE");

  }
  else{
    display.setTextColor(SSD1306_WHITE)  ;  
    display.print("INACTIVE");

  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5,35);
  display.print("Millis: ");
  display.print(ms);
  display.display();
}
