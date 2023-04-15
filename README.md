# drizzle
### watering automation
This project allows you to build an automated watering system for your plants so they do not just die.

## Concept
Every 20 minutes the microcontroller reads the analog signal give from the external moisture sensor and determines the current earth moisture level. The level gets checked against the set limit every hour. If it's too low, the controller causes the MOSFET to switch on the valve, allowing some water to flow into the plant pot. After some seconds the controller drops voltage on the MOSFET and the valve closes.

## Hardware
- Arduino Nano 33 IoT
- MakerFactory MF-6402132 Sensor
- RMV-158-NC valve
- STMicroelectronics STP16NF06L MOSFET
- 10kOhm resistor
- power supply with output 12V, 0.6A

(you may use similar components if they work as well)

## Details
### Serial monitor
The serial monitor outputs log messages

### LED indicators
- Status LED:
    - static:      valve open

## Disclaimer
Although this should not happen, I encourge you to test the code under safe circumstances to ensure it works as desired. If you somehow flood your home or damage something else, that is your responsibility.

Have fun.