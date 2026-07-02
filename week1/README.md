# Week 1: Potentiometer-Controlled LED Blink

## Hardware Required
* Arduino Uno R3 
* 1x Breadboard
* 1x Light Emitting Diode (LED)
* 1x 220-ohm Resistor (for LED current limiting)
* 1x 10k-ohm Rotary Potentiometer
* Jumper wires

## Circuit Diagram Description
The cathode (short leg) of the LED connects to the Arduino GND pin through a 220-ohm resistor. The anode (long leg) connects to digital Pin 13. The potentiometer's outer pins are wired to Arduino 5V and GND, respectively. The middle wiper pin connects directly to Analog Pin A0 to feed variable voltage values.

## How to Upload Code
1. Install the Arduino IDE on your computer.
2. Clone this repository or copy the code from `led_blink.ino`.
3. Open the file in the Arduino IDE.
4. Connect your Arduino board to your computer using a USB cable.
5. Go to **Tools > Board** and select your hardware as Arduino Uno.
6. Go to **Tools > Port** and select your active serial port.
7. Click the **Upload** arrow icon in the top-left toolbar.

## Expected Output
The onboard LED will flash continuously. Turning the potentiometer knob fully clockwise increases the delay up to 1023ms, slowing the flash cycle down. Turning it counterclockwise drops the delay toward 0ms, making it blink hyper-fast. The Arduino Serial Monitor (set to 9600 baud) prints an incrementing running count of every full cycle completed.

## Troubleshooting Tips
* **LED is completely off:** Verify that the LED legs are not reversed. Long leg must point toward Pin 13.
* **Serial Monitor shows garbled text:** Ensure the baud rate dropdown menu in the bottom-right corner of your Serial Monitor window matches exactly 9600 baud.
* **Blink rate stays constant:** Check that the center pin of your potentiometer is securely plugged into Analog pin **A0**, rather than a digital pin or power rail.
