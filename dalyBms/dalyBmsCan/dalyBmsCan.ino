#include <SPI.h>
#include <mcp2515.h>
#include <daly-bms-uart.h>
#define BMS_SERIAL Serial
Daly_BMS_UART bms(BMS_SERIAL);
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


void setup() {
  pinMode(LED,OUTPUT);
  pinMode(FAN,OUTPUT);
  TCCR2B = TCCR2B & B11111000 | B00000001;

  bms.Init();
  bms.setBmsReset();
  
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
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  //Serial.println("Example: Write to CAN");
}
volatile uint8_t inbuf[10];
void loop() {
  for(int i = 0; i < 50;i++){
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK && canMsg.can_id == 0x021){
      for(int i = 0; i < canMsg.can_dlc;i++){
        inbuf[i] = canMsg.data[i];
      }
      break;
    }
  }
  if(inbuf[0] == 0){
    digitalWrite(LED,LOW);
    analogWrite(FAN,0);
  }
  else{
    digitalWrite(LED,HIGH);
    analogWrite(FAN,inbuf[0]);
  }
  bms.update();
  updateBuffer();
  mcp2515.sendMessage(&canMsg1);
  delay(1);
  mcp2515.sendMessage(&canMsg2);
  delay(1);
  mcp2515.sendMessage(&canMsg3);
  delay(1);
  mcp2515.sendMessage(&canMsg4);
  delay(1);
  mcp2515.sendMessage(&canMsg5);
  delay(2);
  mcp2515.sendMessage(&canMsg6);
  delay(2);
  mcp2515.sendMessage(&canMsg7);
  delay(2);
  mcp2515.sendMessage(&canMsg8);
  delay(2);

  //Serial.println("Messages sent");
  
  delay(100);
}

void updateBuffer(){
  canMsg1.data[0] = int(bms.get.packVoltage);
  canMsg1.data[1] = decimal(bms.get.packVoltage);
  canMsg1.data[2] = int(bms.get.packSOC);
  canMsg1.data[3] = decimal(bms.get.packSOC);
  canMsg1.data[4] = int(bms.get.packCurrent/10);
  canMsg1.data[5] = int(int(bms.get.packCurrent/10)*100-bms.get.packCurrent*100);
  canMsg1.data[6] = bms.get.resCapacitymAh;
  canMsg1.data[6] = inbuf[0];

  //canMsg1.data[7] = bms.get.bmsCycles;

  for(int i = 0; i < 8;i++){
    canMsg2.data[i] = int(bms.get.cellVmV[i])/10-200;
  }
  for(int i = 0; i < 8;i++){
    canMsg3.data[i] = int(bms.get.cellVmV[i+8])/10-200;
  }
  for(int i = 0; i < 5;i++){
    canMsg4.data[i] = int(bms.get.cellVmV[i+16])/10-200;
  }

  canMsg4.data[5] = int(bms.get.tempAverage);
  canMsg4.data[6] = 0x00;
  canMsg4.data[7] = 0xA0;

  canMsg5.data[0] = int(bms.get.maxCellmV/10)-200;
  canMsg5.data[1] = int(bms.get.maxCellVNum);
  canMsg5.data[2] = int(bms.get.minCellmV/10)-200;
  canMsg5.data[3] = int(bms.get.minCellVNum);
  canMsg5.data[4] = int(bms.get.cellDiff);;
  canMsg5.data[5] = 0x00;
  canMsg5.data[6] = 0x00;
  canMsg5.data[7] = 0xA0;

  canMsg6.data[0] = temp1;
  canMsg6.data[1] = temp2;
  canMsg6.data[2] = temp3;
  canMsg6.data[3] = temp4;
  canMsg6.data[4] = 0x01;
  canMsg6.data[5] = 0x00;
  canMsg6.data[6] = 0x00;
  canMsg6.data[7] = 0xA0;

  canMsg7.data[0] = 
            int(bms.alarm.levelOneCellVoltageTooHigh)<<7 + 
            int(bms.alarm.levelTwoCellVoltageTooHigh)<<6 + 
            int(bms.alarm.levelOneCellVoltageTooLow)<<5 + 
            int(bms.alarm.levelTwoCellVoltageTooLow)<<4 + 
            
            int(bms.alarm.levelOnePackVoltageTooHigh)<<3 + 
            int(bms.alarm.levelTwoPackVoltageTooHigh)<<2 + 
            int(bms.alarm.levelOnePackVoltageTooLow)<<1 + 
            int(bms.alarm.levelTwoPackVoltageTooLow); 
  canMsg7.data[1] = 
            int(bms.alarm.levelOneChargeTempTooHigh)<<7 + 
            int(bms.alarm.levelTwoChargeTempTooHigh)<<6 + 
            int(bms.alarm.levelOneChargeTempTooLow)<<5 + 
            int(bms.alarm.levelTwoChargeTempTooLow)<<4 + 
            
            int(bms.alarm.levelOneDischargeTempTooHigh)<<3 + 
            int(bms.alarm.levelTwoDischargeTempTooHigh)<<2 + 
            int(bms.alarm.levelOneDischargeTempTooLow)<<1 + 
            int(bms.alarm.levelTwoDischargeTempTooLow); 
  canMsg7.data[2] = 
            int(bms.alarm.levelOneChargeCurrentTooHigh)<<7 + 
            int(bms.alarm.levelTwoChargeCurrentTooHigh)<<6 + 
            int(bms.alarm.levelOneDischargeCurrentTooHigh)<<5 + 
            int(bms.alarm.levelTwoDischargeCurrentTooHigh)<<4 + 
            
            int(bms.alarm.levelOneStateOfChargeTooHigh)<<3 + 
            int(bms.alarm.levelTwoStateOfChargeTooHigh)<<2 + 
            int(bms.alarm.levelOneStateOfChargeTooLow)<<1 + 
            int(bms.alarm.levelTwoStateOfChargeTooLow);  
  canMsg7.data[3] = 
            int(bms.alarm.levelOneCellVoltageDifferenceTooHigh)<<3 + 
            int(bms.alarm.levelTwoCellVoltageDifferenceTooHigh)<<2 + 
            int(bms.alarm.chargeFETTemperatureTooHigh)<<1 + 
            int(bms.alarm.dischargeFETTemperatureTooHigh);
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
