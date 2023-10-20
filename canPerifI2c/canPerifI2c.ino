#include <SPI.h>
#include <mcp2515.h>
#include<Wire.h>

struct can_frame canMsg;
struct can_frame outMsg;

MCP2515 mcp2515(10);

volatile uint8_t buf[10];
void setup() {
  outMsg.can_id = 0x021;
  outMsg.can_dlc = 8;
  Serial.begin(115200);

  Wire.begin(0x10);
  Wire.onRequest(req);
  Wire.onReceive(recv);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    switch(canMsg.can_id){
    case 0x011:
      for(int i = 0; i < 8;i++){
        buf[i] = canMsg.data[i];
//        Serial.print(String(buf[i]) + " ");
      }
//      Serial.println("");
      break;
    }
  }
}
volatile uint8_t test[10] = {1,2,3,4,5,6,7,8,9,10};
void req(){
  Wire.write('c');
  for(int i = 0; i < sizeof(buf);i++){
    Wire.write(buf[i]); 
//    Serial.print(buf[i]);
  }
//  Serial.println("");
}


void recv(){
  bool startFound = false; 
  uint8_t i = 0;
  int temp = 0; 
  while(Wire.available()){
    temp = Wire.read();
    Serial.print(temp);
    Serial.print(" ");
    outMsg.data[i++] = temp;
  }
  Serial.println("");
  mcp2515.sendMessage(&outMsg);
}
