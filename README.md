# Runaway Chair

This project is a chair that runs away from you when you try to sit on it. It uses an ultrasonic sensor to detect when someone tries to sit on it and moves using four stepper motors. The main board used is an ESP32.



## Features

- Detects when someone tries to sit on the chair using an ultrasonic sensor.
- Moves away using four stepper motors.
- Controlled by an ESP32 board.

## Hardware Requirements

- ESP32 board
- 4 Stepper motors
- Motor driver (Keystudio L298N)
- Ultrasonic sensor
- Power supply
- Wires and connectors

## Software Requirements

- Arduino IDE
- ESP32 board support for Arduino IDE

## Wiring

- Connect the stepper motors to the motor driver.
- Connect the motor driver to the ESP32 board.
- Connect the ultrasonic sensor to the ESP32 board.
- Power the ESP32 board and motor driver.