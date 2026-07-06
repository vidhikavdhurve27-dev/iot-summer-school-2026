# Week 2:  Arduino Programming & Simulation

This directory contains the practical lab submissions for Questions 11 through 20. The work focuses on non-blocking programming structures, state machines, serial protocols, and foundational IoT hardware design. All projects are designed to maximize input responsiveness by replacing blocking delay loops with timestamp checking.

---

##  Hardware Components Required

*   **Microcontroller:** Arduino Uno R3 
*   **Visual Outputs:** 3x Standard LEDs (Red, Yellow, Green), 3x System State LEDs (Blue, Yellow, Green)
*   **Audio Outputs:** 1x Passive Piezo Buzzer element
*   **Sensors & Input Controls:** 1x Rotary Potentiometer (10kΩ), 6x Momentary Tactile Pushbuttons
*   **Prototyping Gear:** 1x Solderless Breadboard, 1x 220Ω resistor per LED, hookup jumper wires

---

##  Project Directory 

###  Q14: Traffic Light Controller with Pedestrian Override
https://www.tinkercad.com/things/eXgOG93Rpju-tracfficlight/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits

*   **Description:** Implements a timed automotive traffic light loop (Red 5s -> Yellow 2s -> Green 4s) using `millis()`. A crosswalk pushbutton acts as an external hardware interrupt that instantly shifts the state back to a sustained 8-second safety Red window.

###  Q15: Digital Piano using Buzzer
https://www.tinkercad.com/things/1DV0n12vsLC-digital-piano-with-buzzer/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits

*   **Description:** Generates unique standard musical notes (Do, Re, Mi, Fa) across a 4-button input grid. Features multi-press fallback tracking that overrides the sound output with a complex *Sol* chord substitute when multiple buttons are pushed down together.

###  Q16: Serial Command Interface
https://www.tinkercad.com/things/40wetrbUj6z-serial-command-interface/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits

*   **Description:** An incoming serial text interpreter that processes inbound user terminal strings (`LED_ON`, `LED_OFF`, `STATUS`, `RESET`, `BLINK_X`). Includes text scanning parsing and returns safety errors over the bus line for non-validated requests.

###  Q17: PWM Fading Night Light
https://www.tinkercad.com/things/i3KwXMis453-night-light-system/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits

*   **Description:** A smart luminary system utilizing high-speed digital pulse width switching to output a smooth fade wave (breathing effect). Uses a single mode selector button to index through standard slow breathe, fast pulse, and an emergency Morse Code SOS flash string.

###  Q18: State Machine Vending Machine
https://www.tinkercad.com/things/6kgVP164bKM-state-machine-implementation/editel?returnTo=%2Fdashboard%2Fdesigns%2Fcircuits

*   **Description:** An architecture mapping a 4-state automation matrix (`IDLE`, `COIN_INSERTED`, `ITEM_SELECTED`, `DISPENSING`). Listens to dedicated action triggers (Coin Insert, Select Item, Cancel) to manage clean visual tracking transitions across localized hardware feedback lines.

---

##  Circuit Diagram Descriptions

### Traffic Light Setup (Q14)
The grounds of three separate LEDs combine into the ground rail via current-limiting resistors. Anodes map directly to Digital Pins 11, 12, and 13. A momentary switch bridges ground to Digital Pin 7, depending on internal pull-up logic to register active depressions.

### Piano Matrix Layout (Q15)
A single passive piezo element connects between the ground rail and PWM-capable Digital Pin 3. Four pushbuttons form a horizontal row; their lower pins connect to the ground rail, while their top pins bridge independently across to Pins 4, 5, 6, and 8.

### PWM Light & FSM Array (Q17 & Q18)
The night light utilizes a high-output LED wired into PWM Pin 9 through a protective resistor block. The vending machine uses three independent LEDs linked into Pins 10, 11, and 12, while three separate pushbuttons connect directly across into Pins 4, 5, and 6 to serve as clean logical inputs.

---
