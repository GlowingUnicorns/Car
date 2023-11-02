# Electric Car Software System

This GitHub repository contains the software for my electric car control system. The purpose of this project is to create various software programs that manage aspects of the car including battery management, motor control, temperature control, cameras, sensors, communication lines, redundancy, and user interface.

## Table of Contents

- [Overview](#overview)
- [Auxiliary Systems](#auxiliary-systems)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

![alt text](https://github.com/GlowingUnicorns/Car/blob/main/Images/Diag5.png)


These programs are collectively responsible for:

- **Battery Management:** Monitor and manage the electric car's battery system, including charging, discharging, and maintaining battery health.

- **Motor Control:** Control the electric motor for acceleration, deceleration, and regenerative braking.

- **Temperature Control:** Control fans and pumps for inverter, battery, and motor.

- **Sensor Data:** Detects temperatures, speed, proximities, gyroscopic info, and battery info.

- **Auxiliary Systems:** Controls lights, screens, and displays.

- **User Interface:** Provide a user-friendly interface for the driver to see system information, via a large screen.

- **Safety Systems:** Implement safety features like redundancy and system diagnostics.

The five main modules are the Battery system, Motor System, Auxiliary System, Secondary System, and Interface System, which are responsible for battery regulation, motor control, main data display, secondary data display, and a centralized monitor respectively.

![alt text](https://github.com/GlowingUnicorns/Car/blob/main/Images/Diag4.png)


## Battery Management

The BatteryECU consists of a set of sensors and drivers that communicate battery data and regulate cooling based on external settings determined by CAN-BUS data. The BatteryECU is one of two main data senders which can be easily found by masking and filtering the first 3 bits of the 11-bit address. 

```
  canMsg1.data[0] = int(bms.get.packVoltage);
  canMsg1.data[1] = decimal(bms.get.packVoltage);
  canMsg1.data[2] = int(bms.get.packSOC);
  canMsg1.data[3] = decimal(bms.get.packSOC);
  canMsg1.data[4] = int(bms.get.packCurrent * 0.1);
  canMsg1.data[5] = int(int(bms.get.packCurrent* 0.1) * 100 - bms.get.packCurrent * 100);
  canMsg1.data[6] = bms.get.resCapacitymAh;
  canMsg1.data[7] = inbuf[0];
  for(int i = 0; i < 8; i++){
    canMsg2.data[i] = int(bms.get.cellVmV[i]) * 0.1 - 200;
  }
  for(int i = 0; i < 8; i++){
    canMsg3.data[i] = int(bms.get.cellVmV[i+8]) * 0.1 - 200;
  }
  for(int i = 0; i < 5; i++){
    canMsg4.data[i] = int(bms.get.cellVmV[i+16]) * 0.1 - 200;
  }
```

## Motor Control

The inverter is controlled by an internal 32-bit ARM processor and is externally controlled by another 32-bit ARM processor in the form of RP2040. The internal processor's purpose is simply to regulate the frequency of the 3-phase motor power with data from various sensors. The pedal can transfer the signal directly to the internal processor or to the external processor which regulates the power based on external parameters and displays the data being transferred via CAN-BUS. This system introduces redundancy and allows the car to still function in a degraded state even in the case of complete system failure.

```
  limit = analogRead(pot); 
  limitPercent = int(limit / 1023.0 * 100);
  throttle = analogRead(INSIG);
  throttlePercent = int(throttle / 1023.0 * 100);
  output = analogRead(OUTSIG);
  outputPercent = int(output / 1023.0 * 100);
  switch(gear){
    case 0:
      digitalWrite(PB3,0);
      break;
    case 1:
      digitalWrite(PB3,1);
      dac.setVoltage(throttle * 0.5, false);
      if(analogRead(OUTSIG) > (throttle * 0.5 + 100)){ digitalWrite(PB3,0) };
      break;
    case 2:
      digitalWrite(PB3,1);
      if(throttle > limit) throttle = limit; 
      dac.setVoltage(throttle * 4, false);
      break;
  }
```


## Sensor Data

Data is collected from an array of thermistors, ultrasonic sensors, and ADCs, which is sent over the CAN-BUS to be interpretted by other computers.
```
  SonarSensor(trigPin1, echoPin1);
  Sensor1 = distance;
  SonarSensor(trigPin2, echoPin2);
  Sensor2 = distance;
  SonarSensor(trigPin3, echoPin3);
  Sensor3 = distance;
  SonarSensor(trigPin4, echoPin4);
  Sensor4 = distance;
  canMsg1.data[0] = (Sensor2 >> 8) & 0xFF;
  canMsg1.data[1] = Sensor2 & 0xFF;
  canMsg1.data[2] = (Sensor1 >> 8) & 0xFF;
  canMsg1.data[3] = Sensor1 & 0xFF;
  canMsg1.data[4] = (Sensor4 >> 8) & 0xFF;
  canMsg1.data[5] = Sensor4 & 0xFF;
  canMsg1.data[6] = (Sensor3 >> 8) & 0xFF;
  canMsg1.data[7] = Sensor3 & 0xFF;

```

## Auxiliary Systems

Auxiliary systems control the dashboard and its 7 LED displays, 5 LED strips, 6 LCDs, 9 inputs, and the 8 main signal lights and headlights. This system of 1 RP2040, 3 ATMEGA328PBs, 1 ESP32, and 1 ATMEGA2560 communicates with I2c, UART, and CAN bus. 

![alt text](https://github.com/GlowingUnicorns/Car/blob/main/Images/Diag3.png)

The RP2040 uses an intermediate Atmega chip through I2c as a buffer as it sends data to update the screens and potentiometer inputs which control the temperatures. 

The main auxiliary board communicates to a peripheral Atmega through I2c to update the LED strips from decoded CAN-BUS data, while controlling the main digital displays.

```
case 0x330:
        rpm = (canMsg.data[5] << 8) + canMsg.data[4];
        rpm = rpm*18;
        mph = rpm*3.14*2*60/5280;
        break;
      case 0x271:
        v = (canMsg.data[1]*255 + canMsg.data[0]) * 0.0625;
        i = (canMsg.data[4] * 255 + canMsg.data[3]) * 0.0625;
        p = i*v;
        if (i < 0) i = 0; 
        break;
      case 0x411:
        t = canMsg.data[6]|(canMsg.data[7] << 8);
        break;

```
The LED strip peripheral smooths the LED movement based on the change detected by the sensors.

```
  uint8_t x = 3;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 3) leds[x][i] = CRGB::Green;
      else if( i < 6) leds[x][i] = CRGB::Blue;
      else if( i < 9) leds[x][i] = CRGB::Purple;
      else leds[x][i] = CRGB::Red;
    }
    else leds[x][i] = CRGB::Black;
  }  
```

The Atmega 2560 receives data from an ESP32 to display data on a large TFT LCD through Serial Communications, and graphs it as a ring display. 
```
  uint8_t startByte = 0xFF;
  uint8_t stopByte = 0xFE;
  while (Serial3.available() == 0 || Serial3.read() != startByte);
  for (int i = 0; i < 5; i++) {
    while (Serial3.available() == 0);
    receivedData[i] = Serial3.read();
  }
  while (Serial3.available() == 0 || Serial3.read() != stopByte);
  uint8_t x = 3;
  for(int i = 0; i < nums[x];i++){
    if(i < map(vals[x],0,255,0,nums[x])){
      if( i < 3) leds[x][i] = CRGB::Green;
      else if( i < 6) leds[x][i] = CRGB::Blue;
      else if( i < 9) leds[x][i] = CRGB::Purple;
      else leds[x][i] = CRGB::Red;
    }
    else leds[x][i] = CRGB::Black;
  }
  for(int i = 0; i < 4;i++){
    if(currentData[i]!=receivedData[i]){
      int j = abs(currentData[i]-receivedData[i])/2 + 1;
      if(currentData[i]<receivedData[i])currentData[i] += j; 
      else if (currentData[i]>receivedData[i]) currentData[i] -= j; 
    }
  }
```

The ESP32 itself is also responsible for triggering the lights around the cars based on the hand levers and displaying any diagnostic data based on data from the CAN-BUS. 




1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/electric-car-control.git
   cd electric-car-control
## License

[MIT](https://choosealicense.com/licenses/mit/)
