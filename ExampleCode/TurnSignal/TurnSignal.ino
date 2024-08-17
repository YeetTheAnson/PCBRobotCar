#include <Adafruit_NeoPixel.h>

#define PIN 4 //neopixel is connected to pin 4 of the car
#define NUMPIXELS 16  // because there 16 pixels on the car (i know cuz i designed it duh)

// l98n motor driver pin
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 15
#define ENA 18
#define ENB 17

// creating class
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool leftSignalActive = false;
bool rightSignalActive = false;
bool leftTurnActive = false;
bool rightTurnActive = false;

void setup() {
  Serial.begin(9600); // set baud rate to 9600 in serial monitor
  pixels.begin();
  pixels.show();

  // set pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // test motor
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255); 
}

void loop() {
  if (leftSignalActive) {
    leftSignal();
  } else if (rightSignalActive) {
    rightSignal();
  }

  if (leftTurnActive) {
    leftTurn();
  } else if (rightTurnActive) {
    rightTurn();
  }

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // remove empty space on serial read

    if (command == "left") {
      leftSignalActive = true;
      rightSignalActive = false;
      leftTurnActive = false;
      rightTurnActive = false;
    } else if (command == "stopleft") {
      leftSignalActive = false;
      turnOffLeftSignal();
    } else if (command == "right") {
      rightSignalActive = true;
      leftSignalActive = false;
      leftTurnActive = false;
      rightTurnActive = false;
    } else if (command == "stopright") {
      rightSignalActive = false;
      turnOffRightSignal();
    } else if (command == "leftturn") {
      leftSignalActive = false;
      rightSignalActive = false;
      leftTurnActive = true;
      rightTurnActive = false;
    } else if (command == "rightturn") {
      leftSignalActive = false;
      rightSignalActive = false;
      leftTurnActive = false;
      rightTurnActive = true;
    }
  }
}

void leftSignal() {
  for (int i = 7; i >= 0; i--) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 0)); 
    pixels.setPixelColor(i + 8, pixels.Color(0, 0, 0)); 
    pixels.show();
    delay(60);
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
  delay(250);
}

void rightSignal() {
  for (int i = 8; i < 16; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 0)); 
    pixels.setPixelColor(i - 8, pixels.Color(0, 0, 0)); 
    pixels.show();
    delay(60);
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
  delay(250);
}

void turnOffLeftSignal() {
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
}

void turnOffRightSignal() {
  for (int i = 8; i < 16; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
}

void moveForward(int duration) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(duration);
  stopMotors();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void leftTurn() {
  moveForward(1000); // move forward for 1 second
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { left signal while turning for 5 secs
    leftSignal();
  }
  turnOffLeftSignal();
  moveForward(2000); // moving straight again for 2 seconds
  stopMotors();
}

void rightTurn() {
  moveForward(1000); 
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) 
    rightSignal();
  }
  turnOffRightSignal();
  moveForward(2000); 
  stopMotors();
}
