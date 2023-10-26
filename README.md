# Electric Car Software System

## Overview

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

Auxiliary systems control the dashboard and its 7 LED displays, 5 LED strips, 6 LCDs, 9 inputs, and the 8 main signal lights and headlights. This system of 1 RP2040, 3 ATMEGA328PBs, 1 ESP32, and 1 ATMEGA2560 communicates with I2c, SPI, UART, and CAN bus. 

The RP2040 uses an intermediate Atmega chip through I2c as a buffer as it sends data to update the screens and potentiometer inputs which control the temperatures. 




1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/electric-car-control.git
   cd electric-car-control
## License

[MIT](https://choosealicense.com/licenses/mit/)
