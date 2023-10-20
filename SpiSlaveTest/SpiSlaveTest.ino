#include "SPISlave_T4.h"
SPISlave_T4<&SPI, SPI_8_BITS> mySPI;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  mySPI.begin();
  mySPI.onReceive(myFunc);
}

void loop() {
  Serial.print("millis: "); Serial.println(millis());
  delay(1000);
}

void myFunc() {
  Serial.println("START: ");
  volatile uint8_t arr[10] = {1,2,3,4,5,6,7,8,9,10};
  uint8_t i = 0;
  while ( mySPI.active() ) {
    if (mySPI.available()) {
      if ( i++ > sizeof(arr) ) i = 0;
      mySPI.pushr(random(10));
      Serial.print("VALUE: ");
      Serial.print(mySPI.popr());
    }
  }
//  Serial.println("\nEND");
}
