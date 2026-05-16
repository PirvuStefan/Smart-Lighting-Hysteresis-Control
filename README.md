
# Arduino LDR Light Control (Sistem LDR)

This project is a small Arduino-based light sensing and control system that reads a photoresistor (LDR) and controls an LED based on ambient light. It also shows status and readings on a 16x2 I2C LCD and allows switching between two operating modes using a push button.

The code in `sketch.ino` implements a hysteresis-based controller with two modes:
- NOAPTE (night): the LED turns ON when it gets dark below a lower threshold and turns OFF when it becomes bright above an upper threshold.
- ZI (day): the LED behavior is inverted (LED ON when bright).

The project is suitable for learning about sensors, simple control logic, and reading/writing to small displays. It can be used as a learning toy, a simple ambient-light reactive lamp, or as a starting point for more advanced home-automation projects.

----

Quick non-developer explanation
--------------------------------
If you are not a developer, here is what this device does in plain words:
- A light sensor (the LDR / photoresistor) measures how bright the environment is.
- The Arduino reads that sensor and decides whether to turn the LED on or off.
- The small screen (LCD) shows the current light reading, whether the LED is ON or OFF, and the current mode (DAY or NIGHT).
- A button lets you switch the mode. When you press it, the screen briefly shows "MOD SCHIMBAT" and the new mode.

So: press the button to change how the LED reacts to light, and look at the LCD to see what the system is reading and doing.

----

Features
--------
- Analog light measurement using an LDR (photoresistor).
- Hysteresis control to avoid rapid on/off switching (uses separate low/high thresholds).
- Two modes: Night (LED on when dark) and Day (LED on when bright).
- 16x2 I2C LCD for readable feedback.
- Button to toggle modes with a 1.5 second splash message.

----

Physical components (BOM)
------------------------
- 1x Arduino board (Uno, Nano, or compatible)
- 1x LDR (photoresistor)
- 1x fixed resistor for the LDR voltage divider (recommended 10 kΩ)
- 1x LED (any color) + 1x 220 Ω resistor
- 1x pushbutton (momentary)
- 1x 16x2 I2C LCD module (with PCF8574/I2C backpack) — code uses address 0x27
- Breadboard and jumper wires
- USB cable to program the Arduino and provide power

Optional
- 1x enclosure or mounting board

----

Wiring (simple table)
----------------------
- LDR: form a voltage divider with the LDR and a 10 kΩ resistor. Connect one end of the LDR to 5V, the other end to the Arduino analog input A0 and to one end of the fixed resistor. Connect the other end of the fixed resistor to GND. The analog pin reads the voltage across the fixed resistor (junction).
- LED: LED anode -> Pin 13 (digital), LED cathode -> resistor 220 Ω -> GND. (Pin 13 is used in the sketch.)
- Button: one side -> Pin 2 (digital), other side -> GND. The sketch enables the pin's internal pull-up resistor, so the button should pull the pin to GND when pressed.
- LCD I2C: SDA -> SDA pin on your board (for Uno A4), SCL -> SCL pin on your board (for Uno A5), VCC -> 5V, GND -> GND. Typical I2C address used: 0x27 (the sketch sets this in LiquidCrystal_I2C initialization).

Pin summary (as used in code)
- A0: LDR analog input
- 13: LED output
- 2: pushbutton input (uses INPUT_PULLUP)
- I2C (SDA/SCL): LCD display

----

How to assemble
----------------
1. Place the Arduino on a flat surface or breadboard.
2. Build the LDR voltage divider on the breadboard: 5V -> LDR -> junction -> fixed resistor (10 kΩ) -> GND. Connect the junction to A0.
3. Connect the LED with its series resistor to pin 13 and GND.
4. Wire the pushbutton between pin 2 and GND (the sketch uses the internal pull-up).
5. Connect the I2C LCD's SDA/SCL/VCC/GND lines to the Arduino.
6. Connect the Arduino to your computer with the USB cable to power and upload code.

----

Using the project (non-developer)
---------------------------------
1. Power the Arduino with the USB cable.
2. The LCD will show readings and status. The first screen message at startup is "Sistem LDR".
3. Press the pushbutton to switch between "NOAPTE" (night) and "ZI" (day) modes. A brief message will display the new mode.
4. Watch the LCD for "Lumina: <value>" where <value> is a number from 0–1023 (analog reading). Lower numbers mean darker, higher mean brighter.
5. The LED will turn on or off according to the selected mode and the ambient light.

----

Configuration for developers
----------------------------
Open `sketch.ino` in the Arduino IDE to configure these values:
- PRAG_LOW (default 400): lower threshold used for hysteresis.
- PRAG_HIGH (default 700): upper threshold used for hysteresis.
- PIN definitions (if you want different pins): `PIN_LDR`, `PIN_LED`, `PIN_BUTON`.
- I2C LCD address and dimensions are set in the LiquidCrystal_I2C initialization (currently 0x27, 16x2).

Tips:
- If the LED keeps toggling rapidly, increase the gap between PRAG_LOW and PRAG_HIGH to add more hysteresis.
- If the LCD does not show text, check the I2C address (0x27 is common but some modules use 0x3F). Use an I2C scanner sketch to detect the address.

----

Troubleshooting
---------------
- No display on LCD: check SDA/SCL wiring, VCC/GND, contrast potentiometer on the I2C backpack, and the I2C address.
- Button not working: ensure the button is wired to GND and the sketch uses INPUT_PULLUP (so the button closes to GND).
- Sensor values always high/low: double-check the LDR voltage divider wiring and the fixed resistor value.
- LED never lights: verify LED polarity and series resistor; test the pin by temporarily writing HIGH in a small test sketch.
 - LED never lights: verify LED polarity and series resistor; test the pin by temporarily writing HIGH in a small test sketch.

Illustrated wiring diagram
--------------------------
An SVG wiring diagram is included in this repository to help visualize the connections. Open `wiring_diagram.svg` in the repository or view it in a browser or image viewer. The diagram shows the LDR voltage divider to A0, LED on D13 (with 220Ω resistor), the button wired to D2 (to GND), and the I2C LCD on SDA/SCL.

If you prefer a Fritzing diagram or a higher-resolution PNG, tell me which format you want and I will add it.

----

Uploading the sketch
---------------------
1. Install the Arduino IDE (or Arduino CLI).
2. Open `sketch.ino` in the IDE.
3. Select the correct board and serial port.
4. Click Upload. The sketch will start automatically when the Arduino resets.

----

Remarks
-------
This repository contains a compact example of reading an analog sensor, using a simple state machine with hysteresis, and providing user feedback on an I2C LCD. It is intended as an educational project and a foundation for more complex behaviors (logging, remote reporting, multiple lights, etc.).

If you want, I can add a simple wiring diagram image, an I2C scanner sketch, or instructions to adapt the project for other boards (e.g., Arduino Nano, ESP32) — tell me which you'd prefer.

License
-------
This project is provided as-is for educational use. Feel free to reuse and modify the code and hardware ideas.


