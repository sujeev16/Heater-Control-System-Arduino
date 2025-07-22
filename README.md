# Heater Control System (Arduino UNO)

## Overview
This project is a heater control system that:
- Monitors temperature using an NTC Thermistor
- Tracks system state: Idle, Heating, Stabilizing, Target Reached, Overheat
- Controls LED indicators and Buzzer for feedback

## Components
- Arduino UNO
- NTC Thermistor
- LEDs (Heater and Status)
- Buzzer

## Pin Connections
| Component        | Arduino Pin |
|------------------|-------------|
| NTC Thermistor   | A0           |
| Heater LED       | D4           |
| Status LED       | D5           |
| Buzzer           | D3           |

## Usage
1. Upload the `HeaterControl.ino` to Arduino UNO
2. Monitor via Serial Monitor at 9600 baud
3. Adjust thermistor temperature to see state transitions

## Bonus
Wokwi simulation JSON is included if you want to simulate the system.
