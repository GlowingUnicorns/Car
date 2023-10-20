#include <Arduino.h>
#include <daly-bms-uart.h>
#define BMS_SERIAL Serial
Daly_BMS_UART bms(BMS_SERIAL);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  bms.Init();
  bms.setBmsReset();

  lcd.init();
  lcd.backlight();
  
  // put your setup code here, to run once:

}

void loop() {
  bms.update();
  // put your main code here, to run repeatedly:
  uint8_t t = int(bms.get.cellVmV[0])/10-200;

  lcd.print("test");
  lcd.setCursor(0,0);
  delay(1000);
}
