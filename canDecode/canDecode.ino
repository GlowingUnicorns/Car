#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}
double rpm = 0;
double v = 0; 
double i = 0;
uint16_t t = 0; 
uint16_t timer = 0; 
void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
//    Serial.print(canMsg.can_id, HEX); // print ID
//    Serial.print(" "); 
//    Serial.print(canMsg.can_dlc, HEX); // print DLC
//    Serial.print(" ");
//    
//    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
//      Serial.print(canMsg.data[i],HEX);
//      Serial.print(" ");
//    }
    switch(canMsg.can_id){
      case 0x330:
        rpm = (canMsg.data[5] << 8) + canMsg.data[4];
        break;
      case 0x271:
        v = (canMsg.data[1]*255 + canMsg.data[0]) * 0.0625;
        i = (canMsg.data[4] * 255 + canMsg.data[3]) * 0.0625;
        if (i < 0) i = 0; 
        break;
      case 0x411:
        t = canMsg.data[6]|(canMsg.data[7] << 8);
        break;
    }    
  }
  if(timer++ > 2000){
    Serial.println("V: " + String(v) + " I: " + String(i) + " RPM: " + String(rpm*1.8) + " t: " + String(t) + " mph: "+ String(rpm*1.8*3.14*2*60*10/5280) + "pow: " + String(v*i));
    timer = 0; 
  }
}
