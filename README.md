# Electric Car Software System

This GitHub repository contains the software for my electric car control system. The purpose of this project is to create various software programs that manage aspects of the car including battery management, motor control, temperature control, cameras, sensors, communication lines, redundancy, and user interface.

## Table of Contents

- [Overview](#overview)
- [Auxiliary Systems](#auxiliary-systems)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

These programs are collectively responsible for:

- **Battery Management:** Monitor and manage the electric car's battery system, including charging, discharging, and maintaining battery health.

- **Motor Control:** Control the electric motor for acceleration, deceleration, and regenerative braking.

- **Temperature Control:** Control fans and pumps for inverter, battery, and motor.

- **Sensor Data:** Detects temperatures, speed, proximities, gyroscopic info, and battery info.

- **Auxiliary Systems:** Controls lights, screens, and displays.

- **User Interface:** Provide a user-friendly interface for the driver to see system information, via a large screen.

- **Safety Systems:** Implement safety features like redundancy and system diagnostics.


## Auxiliary Systems

Auxiliary systems control the dashboard and its 7 LED displays, 5 LED strips, 6 LCDs, 9 inputs, and the 8 main signal lights and headlights. This system of 1 RP2040, 3 ATMEGA328PBs, 1 ESP32, and 1 ATMEGA2560 communicates with I2c, UART, and CAN bus. 


The RP2040 uses an intermediate Atmega chip through I2c as a buffer as it sends data to update the screens and potentiometer inputs which control the temperatures. 

   ```
  cont1[1] = map(analogRead(BATPOT),0,1023,100,0); if(cont1[1] < 5)cont1[1] = 0;if(cont1[1] > 95)cont1[1] = 100;
  cont2[1] = map(analogRead(MOTPOT),0,1023,100,0); if(cont2[1] < 5)cont2[1] = 0;if(cont2[1] > 95)cont2[1] = 100;
  cont3[1] = map(analogRead(DRVPOT),0,1023,100,0); if(cont3[1] < 5)cont3[1] = 0;if(cont3[1] > 95)cont3[1] = 100;
  updateRing(gfx ,120,120,80,110,120,420,10,5,10,temp1[1],tempMin,tempMax,temp1[0]);
  temp1[0] = temp1[1];

  updateRing(gfx2,120,120,80,110,120,420,10,5,10,temp2[1],tempMin,tempMax,temp2[0]);
  temp2[0] = temp2[1];

  updateRing(gfx3,120,120,80,110,120,420,10,5,10,temp3[1],tempMin,tempMax,temp3[0]);
  temp3[0] = temp3[1];
   ``` 

The buffer code decodes the data and sends it to the second core of the RP2040.

The main auxiliary board communicates to a peripheral Atmega through I2c to update the LED strips from decoded CAN-BUS data, while controlling the main digital displays.

```case 0x330:
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
  for(int i = 1; i < 5;i++){
    if(future[i] >vals[i])vals[i]+=2;
    if(future[i] <vals[i])vals[i]-=2;
  }
}
void recv(){
  uint16_t i = 0;
  while(Wire.available()>0){
    future[i++] = Wire.read();
  }
}
```
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
```
  volatile bool a = digitalRead(leftTurn);
  volatile bool b = digitalRead(rightTurn);
  if(a||b){
    counter++;
    if(counter < 500){
      if(a)digitalWrite(leftLight,HIGH);
      if(b)digitalWrite(rightLight,HIGH);
    }
    else if(counter < 1000){
      digitalWrite(leftLight,LOW);
      digitalWrite(rightLight,LOW);
    }
    else counter = 0; 
  }
```


1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/electric-car-control.git
   cd electric-car-control
## License

[MIT](https://choosealicense.com/licenses/mit/)
