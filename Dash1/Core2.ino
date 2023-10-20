

void setup1(){
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();
  Serial.begin(115200);
}
void loop1(){
  recvBMS();
  delay(10);
  recvBMS();
  delay(10);
  sendLEDs();
  delay(10);
  sendBMS();
  delay(10);
  
}

void sendLEDs(){
  Wire1.beginTransmission(0x08);
  Wire1.write(235);//bool
  Wire1.write(231);//speed
  Wire1.write(120);//temp
  Wire1.write(100);//curr
  Wire1.write(122);//volt
  Wire1.endTransmission();
}

void recvBMS(){
  Wire1.requestFrom(0x10,30,false);
  
  uint8_t i = 0;
  uint8_t c = Wire1.read();
  while(0 < Wire1.available()){
    if(c == 0xA1){
      buf1[i++] = Wire1.read();
    }
    else if(c == 0xA2){
      buf2[i++] = Wire1.read();
    }
    else break;
    if(i >= 30)break;
  }
  temp1[1] = buf1[28];
  temp2[1] = buf1[29];
  temp3[1] = buf2[0];
}


void sendBMS(){
  Wire1.beginTransmission(0x10);
  Wire1.write(cont1[1]);
  Wire1.write(cont2[1]);
  Wire1.write(cont3[1]);
  Wire1.write(cont1[1]);
  Wire1.endTransmission();
}



void sendComp(){
  
  Wire1.beginTransmission(0x69);
  Wire1.write(0xB1);
  for(int i = 0; i <30;i++){
    Wire1.write(buf1[i]);
  }
  Wire1.endTransmission();
  Wire1.beginTransmission(0x69);
  Wire1.write(0xB2);
  for(int i = 0; i <30;i++){
    Wire1.write(buf2[i]);
  }
  Wire1.endTransmission();
  Wire1.beginTransmission(0x69);
  Wire1.write(0xB3);
  for(int i = 0; i <30;i++){
    Wire1.write(buf3[i]);
  }
  Wire1.endTransmission();

}
