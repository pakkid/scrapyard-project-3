#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// Wi-Fi Credentials
const char *ssid = "TankBot";
const char *password = "robotics123";

// Motor Driver Pins (Keystudio L298N)
#define MOTOR_A_IN1 16
#define MOTOR_A_IN2 17
#define MOTOR_A_IN3 18
#define MOTOR_A_IN4 19
#define MOTOR_A_ENA 21  // Motor A speed (PWM)

#define MOTOR_B_IN1 22
#define MOTOR_B_IN2 23
#define MOTOR_B_IN3 25
#define MOTOR_B_IN4 26
#define MOTOR_B_ENB 27  // Motor B speed (PWM)

// Ultrasonic Sensor (HC-SR04)
#define TRIG_PIN 5
#define ECHO_PIN 18

// Web Server
AsyncWebServer server(80);


// Motor speed variables
int motorASpeed = 0;
int motorBSpeed = 0;

// Set up motors
void setupMotors() {
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_A_IN3, OUTPUT);
  pinMode(MOTOR_A_IN4, OUTPUT);
  pinMode(MOTOR_A_ENA, OUTPUT);

  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
  pinMode(MOTOR_B_IN3, OUTPUT);
  pinMode(MOTOR_B_IN4, OUTPUT);
  pinMode(MOTOR_B_ENB, OUTPUT);
}

// Control motor speed
void setMotorSpeed(int motorA, int motorB) {
  motorASpeed = constrain(motorA, -100, 100);
  motorBSpeed = constrain(motorB, -100, 100);

  digitalWrite(MOTOR_A_IN1, motorASpeed > 0);
  digitalWrite(MOTOR_A_IN2, motorASpeed < 0);
  digitalWrite(MOTOR_A_IN3, motorASpeed > 0);
  digitalWrite(MOTOR_A_IN4, motorASpeed < 0);
  analogWrite(MOTOR_A_ENA, abs(motorASpeed) * 2.55);

  digitalWrite(MOTOR_B_IN1, motorBSpeed > 0);
  digitalWrite(MOTOR_B_IN2, motorBSpeed < 0);
  digitalWrite(MOTOR_B_IN3, motorBSpeed > 0);
  digitalWrite(MOTOR_B_IN4, motorBSpeed < 0);
  analogWrite(MOTOR_B_ENB, abs(motorBSpeed) * 2.55);
}

// Ultrasonic Sensor Function
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;  // Convert to cm
}


// Reverse on Obstacle Detection
void obstacleCheck() {
  while (true) {
    float distance = getDistance();
    if (distance < 30) {
      Serial.println("Obstacle detected! Reversing...");
      setMotorSpeed(-100, -100);
      delay(1000);
      setMotorSpeed(0, 0);
    }
    delay(100);
  }
}

// Web UI
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>TankBot Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #121212;
      color: white;
      margin: 0;
      padding: 0;
    }
    h1 {
      margin-top: 20px;
    }
    .control-grid {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      gap: 10px;
      max-width: 300px;
      margin: 20px auto;
    }
    button {
      font-size: 18px;
      padding: 15px;
      border: none;
      cursor: pointer;
      border-radius: 8px;
      background: linear-gradient(135deg, #ff7b00, #ff3d00);
      color: white;
      transition: transform 0.1s ease, background 0.3s ease;
    }
    button:hover {
      background: linear-gradient(135deg, #ff5200, #e60000);
    }
    button:active {
      transform: scale(0.9);
    }
    .full {
      grid-column: span 3;
    }
  </style>
  <script>
    function sendControl(left, right) {
      fetch('/control?left=' + left + '&right=' + right);
    }
  </script>
</head>
<body>
  <h1>TankBot Control</h1>
  <div class="control-grid">
    <button onclick="sendControl(100, 100)">⬆️</button>
    <button class="full" onclick="sendControl(0, 0)">⏹ Stop</button>
    <button onclick="sendControl(-100, -100)">⬇️</button>
    <button onclick="sendControl(100, -100)">⬅️</button>
    <button onclick="sendControl(-100, 100)">➡️</button>
  </div>
</body>
</html>
)rawliteral";

// Web Server Handlers
void setupWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("left") && request->hasParam("right")) {
      int left = request->getParam("left")->value().toInt();
      int right = request->getParam("right")->value().toInt();
      setMotorSpeed(left, right);
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void setup() {
  Serial.begin(115200);
  setupMotors();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.softAP(ssid, password);
  Serial.println("Wi-Fi AP Started");

  setupWebServer();
  xTaskCreatePinnedToCore(obstacleCheck, "ObstacleCheck", 4096, NULL, 1, NULL, 0);
}