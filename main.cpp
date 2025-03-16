#include <Arduino.h>

// Motor Driver Pins (Keystudio L298N)
#define MOTOR_A_IN1 16
#define MOTOR_A_IN2 17
#define MOTOR_A_IN3 18
#define MOTOR_A_IN4 19

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set motor pins as outputs
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_A_IN3, OUTPUT);
  pinMode(MOTOR_A_IN4, OUTPUT);

  // Print status to serial monitor
  Serial.println("Stepper motor is set to move forward");
}

void loop() {
  // Alternate between MOTOR_A_IN1 and MOTOR_A_IN2 to move forward
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_A_IN3, LOW);
  digitalWrite(MOTOR_A_IN4, LOW);
  delay(10);  // Adjust delay for speed control

  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_A_IN3, LOW);
  digitalWrite(MOTOR_A_IN4, LOW);
  delay(10);  // Adjust delay for speed control

  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_A_IN3, HIGH);
  digitalWrite(MOTOR_A_IN4, LOW);
  delay(10);  // Adjust delay for speed control

  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_A_IN3, LOW);
  digitalWrite(MOTOR_A_IN4, HIGH);
  delay(10);  // Adjust delay for speed control
}