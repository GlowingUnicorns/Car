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
void loop() {
//  Serial.println("test"); 
  if (can2.read(msg)) {
//    Serial.print("CAN2 "); 
//    Serial.print("MB: "); Serial.print(msg.mb);
//    Serial.print("  ID: 0x"); Serial.print(msg.id, HEX );
//    Serial.print("  EXT: "); Serial.print(msg.flags.extended );
//    Serial.print("  LEN: "); Serial.print(msg.len);
//    Serial.print(" DATA: ");
    if(msg.id == 0b10100110001){
      for ( uint8_t i = 0; i < 8; i++ ) {
        Serial.print(msg.buf[i]); Serial.print(" ");
        arr[i+1] = msg.buf[i];
      }
      Serial.println("");
    }
  }
  //delay(100);
}

void myFunc() {
  uint16_t i = 0;
  while ( mySPI.active() ) {
    if (mySPI.available()) {
//      if (i++ < sizeof(arr))
        mySPI.pushr(arr[i++]);
        mySPI.popr();
//      else{
//        mySPI.pushr(255);
//        break; 
//      }
    }
  }
}
