# drizzle
### watering automation
This mini project allows you to build an automated watering system for your plants so they do not just die.

## Concept
Every 20 minutes the microcontroller reads the analog signal give from the external moisture sensor and determines the current earth moisture level. The level gets checked against the set limit every hour. If it's too low, the controller causes the valve to switch, allowing some water to flow into the plant pot. After some seconds the controller closes the valve again.

## Hardware
- Arduino Nano 33 IoT
- power supply with output 12V, 0.6A

### v1
- MakerFactory MF-6402132 Sensor
- RMV-158-NC valve
- STMicroelectronics STP16NF06L MOSFET
- 10kOhm resistor

### v2
- KYES516 Sensor
- ONPIRA 11490 Valve
- Elegoo Relay Module GE-EL-SM-006 
- Voltage Controller M-SD-LM317

_(of course other combinations are possible as well)_

## Details
### Serial monitor

### LED indicators

## Disclaimer
Although this should not happen, I encourge you to test the code under safe circumstances to ensure it works as desired. If you somehow flood your home or damage something else, that is your responsibility.

Have fun.