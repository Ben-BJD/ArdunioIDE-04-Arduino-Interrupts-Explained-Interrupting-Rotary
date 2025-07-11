#include <Arduino.h>
#include <Servo.h>
 
#define LED_PIN 6
#define BUTTON_PIN 2 // Interrupt Pin 0
#define ENCODER_CLK 3  // Interrupt Pin 1
#define ENCODER_DT  5
 
#define SERVO_PIN  9
#define SERVO_STEP 5

//variables toggled via interrupt
volatile int ledState = LOW;
volatile unsigned long lastButtonTime = 0;
volatile int servoPos = 90;
 
Servo servoMotor;// Create a Servo object
 
// ISR for the push button
void onPushButtonInterrupt() {
  if (millis() - lastButtonTime > 50) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastButtonTime = millis();
  }
}
 
// ISR for the rotary encoder
void onEncoderInterrupt() {
  if (digitalRead(ENCODER_DT) == HIGH) {
    servoPos += SERVO_STEP; // Clockwise
  } else {
    servoPos -= SERVO_STEP; // Counterclockwise
  }
 
  // Constrain the servo position
  servoPos = constrain(servoPos, 0, 180);
  servoMotor.write(servoPos);
}
 
void setup() {
  // Button and LED setup
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
 
  // Encoder setup
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
 
  // Servo setup
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(servoPos);
 
  // Attach BOTH interrupts to their respective pins and ISRs
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onPushButtonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), onEncoderInterrupt, FALLING);
 
  Serial.begin(9600);
}
 
// The loop is still busy, but it doesn't matter!
void timeIntensiveTask() {
  Serial.println("Time intensive running ...");
  
  // Simulate a time-intensive task
  for (long i = 0; i < 100000; i++) {
    // Wasting time...
    Serial.println("Time intensive task still running ...");
  }
  Serial.println("Time intensive task finished.");
}
 
void loop() {
  timeIntensiveTask();
  delay(1000); // We can even add long delays here.
}