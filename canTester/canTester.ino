#include <SPI.h>
#include <mcp2515.h>
#define LED 2
#define FAN 3
uint8_t temp1 = 40;
uint8_t temp2 = 45;
uint8_t temp3 = 50;
uint8_t temp4 = 55;

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;
struct can_frame canMsg5;
struct can_frame canMsg6;
struct can_frame canMsg7;
struct can_frame canMsg8;

struct can_frame canMsg;

MCP2515 mcp2515(10);


void setup(){
  Serial.begin(115200);
  pinMode(A6,INPUT);
  canMsg1.can_id  = 0b10100000001;
  canMsg1.can_dlc = 8;
  canMsg2.can_id  = 0b10100000010;
  canMsg2.can_dlc = 8;
  canMsg3.can_id  = 0b10100000011;
  canMsg3.can_dlc = 8;
  canMsg4.can_id  = 0b10100000100;
  canMsg4.can_dlc = 8;
  canMsg5.can_id  = 0b10100000101;
  canMsg5.can_dlc = 8;
  canMsg6.can_id  = 0b10100000110;
  canMsg6.can_dlc = 8;
  canMsg7.can_id  = 0b10100000111;
  canMsg7.can_dlc = 8;
  canMsg8.can_id  = 0b10100001000;
  canMsg8.can_dlc = 8;

  //while (!Serial);
  //Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_16MHZ);
  mcp2515.setNormalMode();
  
//  canMsg1.data[0] = 1;
//  canMsg1.data[1] = 2;
//  canMsg1.data[2] = 3;
//  canMsg1.data[3] = 4;
//  canMsg1.data[4] = 5;
//  canMsg1.data[5] = 6;
//  canMsg1.data[6] = 7;
//  canMsg1.data[7] = 8;
}
volatile uint8_t inbuf[10];
void loop() {

  updateBuffer();
  
  mcp2515.sendMessage(&canMsg1);
  delay(3);
  mcp2515.sendMessage(&canMsg2);
  delay(3);
  mcp2515.sendMessage(&canMsg3);
  delay(3);
  mcp2515.sendMessage(&canMsg4);
  delay(3);
  mcp2515.sendMessage(&canMsg5);
  delay(3);
  mcp2515.sendMessage(&canMsg6);
  delay(3);
  mcp2515.sendMessage(&canMsg7);
  delay(3);
  mcp2515.sendMessage(&canMsg8);
  delay(3);

  
}

void updateBuffer(){
  double packCurrent = analogRead(A6)/10.0;
  canMsg1.data[0] = int(0.0);
  canMsg1.data[1] = decimal(81.5);
  canMsg1.data[2] = int(0.0);
  canMsg1.data[3] = decimal(50.0);
  canMsg1.data[4] = int(packCurrent/10);
  canMsg1.data[5] = int(int(packCurrent/10)*100-packCurrent*100);
  canMsg1.data[6] = 100;
  canMsg1.data[7] = 0;

  //canMsg1.data[7] = bms.get.bmsCycles;

  for(int i = 0; i < 8;i++){
    canMsg2.data[i] = int(4200)/10-200;
  }
  for(int i = 0; i < 8;i++){
    canMsg3.data[i] = int(3600)/10-200;
  }
  for(int i = 0; i < 5;i++){
    canMsg4.data[i] = int(2500)/10-200;
  }

  canMsg4.data[5] = int(25);
  canMsg4.data[6] = 0x00;
  canMsg4.data[7] = 0xA0;

  canMsg5.data[0] = int(4200/10)-200;
  canMsg5.data[1] = int(5);
  canMsg5.data[2] = int(2500/10)-200;
  canMsg5.data[3] = int(19);
  canMsg5.data[4] = int(100);
  canMsg5.data[5] = 0x00;
  canMsg5.data[6] = 0x00;
  canMsg5.data[7] = 0xA0;

  canMsg6.data[0] = 25;
  canMsg6.data[1] = 25;
  canMsg6.data[2] = 25;
  canMsg6.data[3] = 25;
  canMsg6.data[4] = 0x01;
  canMsg6.data[5] = 0x00;
  canMsg6.data[6] = 0x00;
  canMsg6.data[7] = 0xA0;

  canMsg7.data[0] = 
            int(true)<<7 + 
            int(false)<<6 + 
            int(true)<<5 + 
            int(true)<<4 + 
            
            int(true)<<3 + 
            int(false)<<2 + 
            int(true)<<1 + 
            int(true); 
  canMsg7.data[1] = 
            int(true)<<7 + 
            int(false)<<6 + 
            int(true)<<5 + 
            int(true)<<4 + 
            
            int(true)<<3 + 
            int(false)<<2 + 
            int(true)<<1 + 
            int(true); 
  canMsg7.data[2] = 
            int(true)<<7 + 
            int(false)<<6 + 
            int(true)<<5 + 
            int(true)<<4 + 
            
            int(true)<<3 + 
            int(false)<<2 + 
            int(true)<<1 + 
            int(true); 
  canMsg7.data[3] = 
            int(true)<<3 + 
            int(false)<<2 + 
            int(true)<<1 + 
            int(true); 
  canMsg7.data[4] = 0x01;
  canMsg7.data[5] = 0x00;
  canMsg7.data[6] = 0x00;
  canMsg7.data[7] = 0xA0;

  canMsg8.data[0] = 0x0E;
  canMsg8.data[1] = 0x00;
  canMsg8.data[2] = 0x00;
  canMsg8.data[3] = 0x08;
  canMsg8.data[4] = 0x01;
  canMsg8.data[5] = 0x00;
  canMsg8.data[6] = 0x00;
  canMsg8.data[7] = 0xA0;
}

int decimal(double input){
  int a = int(input)*100;
  int b = int(100*input);
  return b-a;  
}
