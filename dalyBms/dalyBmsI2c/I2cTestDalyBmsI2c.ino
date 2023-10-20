#include <daly-bms-uart.h>
#define BMS_SERIAL Serial
Daly_BMS_UART bms(BMS_SERIAL);
#include<Wire.h>
#define LED 2
#define FAN 3
void setup() {
  // put your setup code here, to run once:
  Wire.begin(0x10);
  Wire.onRequest(req);
  Wire.onReceive(recv);
  pinMode(LED,OUTPUT);
  pinMode(FAN,OUTPUT);
  TCCR2B = TCCR2B & B11111000 | B00000001;
  bms.Init();
  bms.setBmsReset();
  //Serial.begin(115200);

}
volatile uint8_t inbuf[10];

uint8_t buf1[30];
uint8_t buf2[30];

int temp1 = 50;
int temp2 = 45;
int temp3 = 40;
int temp4 = 50;
void loop() {
  // put your main code here, to run repeatedly:
  bms.update();
  buf1[0] = int(bms.get.packVoltage);
  buf1[1] = int(((float)(bms.get.packVoltage)-(int)(bms.get.packVoltage))*100);
  buf1[2] = int(bms.get.packSOC);
  buf1[3] = int(bms.get.packCurrent)/10;
  buf1[4] = (float(bms.get.packCurrent)-int(bms.get.packCurrent))*100;
  for(int i = 5; i < 26;i++){
    buf1[i]= int(bms.get.cellVmV[i-5])/10-200;
  }
  buf1[26] = int(bms.get.tempAverage);
  buf1[27] = 
            int(bms.get.chargeFetState)<<4 + 
            int(bms.get.disChargeFetState)<<3 + 
            int(bms.get.chargeState)<<2 + 
            int(bms.get.loadState)<<1 + 
            int(bms.get.cellBalanceActive);
            
  buf1[28] = temp1;
  buf1[29] = temp2;
  buf2[0] = temp3;
  buf2[1] = temp4;

  buf2[2] = int(bms.alarm.levelOneCellVoltageTooHigh)<<7 + 
            int(bms.alarm.levelTwoCellVoltageTooHigh)<<6 + 
            int(bms.alarm.levelOneCellVoltageTooLow)<<5 + 
            int(bms.alarm.levelTwoCellVoltageTooLow)<<4 + 
            
            int(bms.alarm.levelOnePackVoltageTooHigh)<<3 + 
            int(bms.alarm.levelTwoPackVoltageTooHigh)<<2 + 
            int(bms.alarm.levelOnePackVoltageTooLow)<<1 + 
            int(bms.alarm.levelTwoPackVoltageTooLow); 
            
  buf2[3] = int(bms.alarm.levelOneChargeTempTooHigh)<<7 + 
            int(bms.alarm.levelTwoChargeTempTooHigh)<<6 + 
            int(bms.alarm.levelOneChargeTempTooLow)<<5 + 
            int(bms.alarm.levelTwoChargeTempTooLow)<<4 + 
            
            int(bms.alarm.levelOneDischargeTempTooHigh)<<3 + 
            int(bms.alarm.levelTwoDischargeTempTooHigh)<<2 + 
            int(bms.alarm.levelOneDischargeTempTooLow)<<1 + 
            int(bms.alarm.levelTwoDischargeTempTooLow); 
            
  buf2[4] = int(bms.alarm.levelOneChargeCurrentTooHigh)<<7 + 
            int(bms.alarm.levelTwoChargeCurrentTooHigh)<<6 + 
            int(bms.alarm.levelOneDischargeCurrentTooHigh)<<5 + 
            int(bms.alarm.levelTwoDischargeCurrentTooHigh)<<4 + 
            
            int(bms.alarm.levelOneStateOfChargeTooHigh)<<3 + 
            int(bms.alarm.levelTwoStateOfChargeTooHigh)<<2 + 
            int(bms.alarm.levelOneStateOfChargeTooLow)<<1 + 
            int(bms.alarm.levelTwoStateOfChargeTooLow);             

  buf2[5] = int(bms.alarm.levelOneCellVoltageDifferenceTooHigh)<<3 + 
            int(bms.alarm.levelTwoCellVoltageDifferenceTooHigh)<<2 + 
            int(bms.alarm.chargeFETTemperatureTooHigh)<<1 + 
            int(bms.alarm.dischargeFETTemperatureTooHigh); 
  buf2[6] = inbuf[0];
  buf2[7] = inbuf[1];
  buf2[8] = inbuf[2];
  if(inbuf[0] == 0){
    digitalWrite(LED,LOW);
    analogWrite(FAN,0); 
  }
  delay(200);

}
bool first_buf = false;
void req(){
  if(first_buf){
    Wire.write(0xA1);
    for(int i= 0; i < 30;i++){
      Wire.write(buf1[i]);
    } 
  }
  else{
    Wire.write(0xA2);
    for(int i= 0; i < 30;i++){
      Wire.write(buf2[i]);
    } 
  }
  first_buf = !first_buf;


}
void recv(){
  uint16_t i = 0; 
  while(0<Wire.available()){
    inbuf[i++] = Wire.read();
    if(i >= 10)break;

  }
  if(inbuf[0] == 0){
    digitalWrite(LED,LOW);
    analogWrite(FAN,0);
  }
  else{
    digitalWrite(LED,HIGH);
    analogWrite(FAN,map(inbuf[0],0,100,0,255));
    TCCR2B = TCCR2B & B11111000 | B00000001;
  }
}  
