#include "SPISlave_T4.h"
SPISlave_T4<&SPI, SPI_8_BITS> mySPI;
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;
CAN_message_t msg;

volatile uint8_t arr[30] = {99,2,3,4,5,6,7,8,9,10};

void setup() {
  
  Serial.begin(115200);
  //while(!Serial);
  can2.begin();
  can2.setBaudRate(500000);
  mySPI.begin();
  mySPI.onReceive(myFunc);
}
uint16_t bufStart = 0;
void loop() {
//  Serial.println("test"); 
/*  if (can2.read(msg)) {
//    Serial.print("CAN2 "); 
//    Serial.print("MB: "); Serial.print(msg.mb);
//    Serial.print("  ID: 0x"); Serial.print(msg.id, HEX );
//    Serial.print("  EXT: "); Serial.print(msg.flags.extended );
//    Serial.print("  LEN: "); Serial.print(msg.len);
//    Serial.print(" DATA: ");
    switch(msg.id){
      case 0b10100000001:bufStart = 0;break;
      case 0b10100000010:bufStart = 8;break;
      case 0b10100000011:bufStart = 16;break;
      case 0b10100000100:bufStart = 24;break;
      case 0b10100000101:bufStart = 32;break;
      case 0b10100000110:bufStart = 40;break;
      case 0b10100000111:bufStart = 48;break;
      case 0b10100001000:bufStart = 56;break;
//      case 0b10100000010:bufStart = 64;break;
    }
    
    for ( uint8_t i = bufStart; i < bufStart + 8; i++ ) {
      Serial.print(msg.buf[i-bufStart]); Serial.print(" ");
      arr[i] = msg.buf[i-bufStart];
    }
    Serial.println("");
  }*/
  
  arr[0] = int(88.0);
  arr[1] = (5);
  arr[2] = int(85.0);
  arr[3] = 8;
  arr[4] = int(0/10);
  arr[5] = int(int(0/10)*100-0*100);
  arr[6] = 2294;
  arr[7] = 0;

  //arr[7] = bms.get.bmsCycles;

  for(int i = 0; i < 8;i++){
    arr[i+8] = int(4190)/10-200;
  }
  for(int i = 0; i < 8;i++){
    arr[i+16] = int(4190)/10-200;
  }
  for(int i = 0; i < 5;i++){
    arr[i+24] = int(4190)/10-200;
  }

  arr[29] = int(25);
  arr[30] = 0x00;
  arr[31] = 0xA0;

  arr[0+32] = int(4190/10)-200;
  arr[1+32] = int(5);
  arr[2+32] = int(4190/10)-200;
  arr[3+32] = int(4);
  arr[4+32] = int(0);
  arr[5+32] = 0x00;
  arr[6+32] = 0x00;
  arr[7+32] = 0xA0;
}

void myFunc() {
  uint16_t i = 0;
  bool first = true; 
  while ( mySPI.active() ) {
    if (mySPI.available()) {
//      if (i++ < sizeof(arr))
        if(first){
          mySPI.pushr(99);
          mySPI.popr();
          first = false; 
        }
        else{
          mySPI.pushr(arr[i++]);
          mySPI.popr();
        }
//      else{
//        mySPI.pushr(255);
//        break; 
//      }
    }
  }
}
