#include <Arduino.h>
#include "LedControl.h"
LedControl lc=LedControl(12,11,10,1);

#include <daly-bms-uart.h> // This is where the library gets pulled in

#define BMS_SERIAL Serial // Set the serial port for communication with the Daly BMS
                           // Set the Serial Debug port

// To print debug info from the inner workings of the library, see the README

// Construct the BMS driver and passing in the Serial interface (which pins to use)
Daly_BMS_UART bms(BMS_SERIAL);

void setup()
{
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  // Used for debug printing
  //Serial.begin(9600); // Serial interface for the Arduino Serial Monitor

  bms.Init(); // This call sets up the driver
}

void loop()
{
  // Print a message and wait for input from the user
  //Serial.println("Press any key and hit enter to query data from the BMS...");
  //while (Serial.available() == 0)
  //{
  //}
  // TODO: Could these both be reduced to a single flush()?
  // Right now there's a bug where if you send more than one character it will read multiple times
  //Serial.read(); // Discard the character sent
  //Serial.read(); // Discard the new line

  // This .update() call populates the entire get struct. If you only need certain values (like
  // SOC & Voltage) you could use other public APIs, like getPackMeasurements(), which only query
  // specific values from the BMS instead of all.
  bms.update();

  // And print them out!
  /*Serial.println("Basic BMS Data:              " + (String)bms.get.packVoltage + "V " + (String)bms.get.packCurrent + "I " + (String)bms.get.packSOC + "\% ");
  Serial.println("Package Temperature (C):     " + (String)bms.get.tempAverage);
  Serial.println("Highest Cell Voltage:        #" + (String)bms.get.maxCellVNum + " with voltage " + (String)(bms.get.maxCellmV / 1000));
  Serial.println("Lowest Cell Voltage:         #" + (String)bms.get.minCellVNum + " with voltage " + (String)(bms.get.minCellmV / 1000));
  Serial.println("Number of Cells:             " + (String)bms.get.numberOfCells);
  Serial.println("Number of Temp Sensors:      " + (String)bms.get.numOfTempSensors);
  Serial.println("BMS Chrg / Dischrg Cycles:   " + (String)bms.get.bmsCycles);
  Serial.println("BMS Heartbeat:               " + (String)bms.get.bmsHeartBeat); // cycle 0-255
  Serial.println("Discharge MOSFet Status:     " + (String)bms.get.disChargeFetState);
  Serial.println("Charge MOSFet Status:        " + (String)bms.get.chargeFetState);
  Serial.println("Remaining Capacity mAh:      " + (String)bms.get.resCapacitymAh);
  Serial.println(String(bms.getPackMeasurements()));
  //... any many many more data

  for (size_t i = 0; i < size_t(bms.get.numberOfCells); i++)
  {
    Serial.println("Remaining Capacity mAh:      " + (String)bms.get.cellVmV[i]);
  }
*/
  // Alarm flags
  // These are boolean flags that the BMS will set to indicate various issues.
  // For all flags see the alarm struct in daly-bms-uart.h and refer to the datasheet
  //Serial.println("Level one Cell V to High:    " + (String)bms.alarm.levelOneCellVoltageTooHigh);

  /**
   * Advanced functions:
   * bms.setBmsReset(); //Reseting the BMS, after reboot the MOS Gates are enabled!
   * bms.setDischargeMOS(true); Switches on the discharge Gate
   * bms.setDischargeMOS(false); Switches off thedischarge Gate
   * bms.setChargeMOS(true); Switches on the charge Gate
   * bms.setChargeMOS(false); Switches off the charge Gate
   */
  lc.clearDisplay(0);
  //print4SegDec((double)(bms.get.packVoltage),-1);
  print4SegDec((double)(0.22),-1);

   delay(1000);
}


void print4Seg(uint32_t num){
  uint32_t digit;
  for(int i =0; i <8;i++){
    digit = num%10;
    if(num == 0){
      break;
    }
    num /= 10;
    lc.setDigit(0,i,digit,false);
  }
}

void print4SegDec(double num,int* offset){
  uint32_t digit;
  uint32_t temp;
  if(offset == -1){
    double copy;
    copy = num;
    *offset = 0;
    while (copy / (int)(copy) != 1.0)
    {
        *offset = *offset + 1;
        (copy) *= 10;
      Serial.println(String((copy)/(int)(copy)));

    }
    print4SegDec(num, offset);
    return;
  }

  
  temp = num * pow(10,*offset);
  for(int i = 0; i < *offset;i++){ 
    digit = temp%10;
    temp /= 10;
    lc.setDigit(0,i,digit,false);
  }
  if (temp == 0){
    lc.setDigit(0,*offset,0,true);
    return;
  }
  bool bleh = true;
 
   for(int i =*offset; i <8;i++){
    digit = temp%10;
    if(temp == 0){
      break;
    }
    temp /= 10;
    lc.setDigit(0,i,digit,bleh);
    bleh = false;

  } 
    //lc.setChar(0,4,'.',false);

  
}
