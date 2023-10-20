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
  canMsg1.can_id  = 0x080;
  canMsg1.can_dlc = 0;
  canMsg2.can_id  = 0x391;
  canMsg2.can_dlc = 8;
  canMsg3.can_id  = 0x330;
  canMsg3.can_dlc = 8;
  canMsg4.can_id  = 0x454;
  canMsg4.can_dlc = 5;
  canMsg5.can_id  = 0x411;
  canMsg5.can_dlc = 8;
  canMsg6.can_id  = 0x271;
  canMsg6.can_dlc = 7;
  canMsg7.can_id  = 0x017;
  canMsg7.can_dlc = 8;
  canMsg8.can_id  = 0x018;
  canMsg8.can_dlc = 8;

  //while (!Serial);
  //Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_16MHZ);
  mcp2515.setNormalMode();
  
  //Serial.println("Example: Write to CAN");
}
volatile uint8_t inbuf[10];
void loop() {

  updateBuffer();
  
  mcp2515.sendMessage(&canMsg1);
  //delay(3);
  mcp2515.sendMessage(&canMsg2);
//  delay(3);
  mcp2515.sendMessage(&canMsg3);
//  delay(3);
  mcp2515.sendMessage(&canMsg4);
//  delay(3);
  mcp2515.sendMessage(&canMsg5);
//  delay(3);
  mcp2515.sendMessage(&canMsg6);
//  delay(3);
  mcp2515.sendMessage(&canMsg7);
//  delay(3);
  mcp2515.sendMessage(&canMsg8);
//  delay(3);

  
}

void updateBuffer(){
  canMsg8.data[0] = int(analogRead(A6)/100);
  canMsg8.data[1] = decimal(double(analogRead(A6))/100.0);
  canMsg8.data[2] = 12;
  canMsg8.data[3] = 1;
  canMsg8.data[4] = 1;
  canMsg8.data[5] = 5;
  Serial.println(analogRead(A6));
  uint16_t list2[8] = {0x0, 0x0, 0x0, 0x0, 0xE, 0x0, 0xF, 0x0};
  uint16_t list3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5F, 0x5};
  uint16_t list4[5] = {0x1, 0x0, 0x0, 0x0, 0x0};
  uint16_t list5[8] = {0xD4, 0x7D, 0xDA, 0x0, 0xA, 0x2, 0x19, 0x0};
  uint16_t list6[7] = {0x71, 0x5, 0x18, 0x0, 0x0, 0x0, 0x0};
  for(int i = 0; i < 8; i++){
    canMsg2.data[i] = list2[i];
    canMsg3.data[i] = list3[i];
    canMsg5.data[i] = list5[i];
  }
  for(int i = 0 ; i < 5;i++){
    canMsg4.data[i] = list4[i];
  }
  for(int i = 0 ; i < 7;i++){
    canMsg6.data[i] = list6[i];
  }


}

int decimal(double input){
  int a = int(input)*100;
  int b = int(100*input);
  return b-a;  
}
