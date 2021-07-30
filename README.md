# drizzle
### watering automation

This project allows you to build an automated watering system for your plants so they do not just die.

Using the hardware listed below, it functions as follows:
Every 10 seconds the microcontroller (mc) reads the analog signal give from the external
moisture sensor and determines the current earth moisture level. The level gets checked
against the set limit every hour. If it's too low, the mc causes the MOSFET to switch on
the valve, allowing some water to flow into the plant pot. After some seconds the mc drops
voltage on the MOSFET and the valve closes again.
This process gets repeated every hour.

Serial monitor displays the currently measured earth humidity.

LEDs indicate as followed:
1. orange flashing:    serial connection active
2. red static:         eath humidity below limit
3. green static:       valve open

Night mode:
Red, green and power led flash once everey measuring inervall intead of static glowing.

Used hardware:
- Arduino Nano 33 BLE
- Joy-it SEN-Moisture
- RMV-158-NC valve
- STMicroelectronics STP16NF06L MOSFET
- 10kOhm resistor
- power supply with output 12V, 0.6A

(you may use some other stuff if it works as well)


*I do not take responsibility if you flood your home or anything else.*