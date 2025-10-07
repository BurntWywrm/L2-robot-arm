// ESP32
// Individual Actuator [Servo] Script Testing
// https://github.com/BurntWywrm/L2-robot-arm

/* 
Tests if the actuator can 
sweep from 0 and to 270 degrees
*/

#include <Wire.h> // I2C Wire Library
#include <Adafruit_PWMServoDriver.h> // Adafruit PWM Servo Library

// MAXIMUM and MINIMUM number of "ticks" for the servo motors.
// Configure as necessary
#define MIN 120 // Minimum pulse length count out of 4096
#define MAX 500 // Maximum pulse length count out of 4096

// Create object to represent PCA9685 at your I2C address
// if you don't know your I2C address, run an I2C scanner sketch
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo variables
// Configure as necessary
const int servoPin = 15;
#define FREQUENCY 50

// Set I2C Pins for the ESP32, default: SDA = 21, SCL = 22.
const int I2C_SDA = 21;
const int I2C_SCL = 22;

void setup()
{
    Serial.begin(115200); // Starts serial monitor with a baud rate of 115200
    Serial.println("Individual Actuator Testing");
    Wire.begin(I2C_SDA, I2C_SCL);

    // Initialize PWM Servo Library
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
}

void loop()
{
  /* Sweeps the servo back and forth */
  for (int posDegrees = 0; posDegrees <= 270; posDegrees += 5){
    int pulse_width = map(posDegrees, 0, 270, MIN, MAX); // Determine pwm pulse width
    pwm.setPWM(servoPin, 0, pulse_width); // Write to Servo
    // Print to serial monitor
    Serial.print("Servo: ");
    Serial.println(posDegrees);
    delay(30);
  }
  delay(250); // quick pause
  for (int posDegrees = 270; posDegrees >= 0; posDegrees -= 5){
    int pulse_width = map(posDegrees, 0, 270, MIN, MAX); // Determine pwm pulse width
    pwm.setPWM(servoPin, 0, pulse_width); // Write to Servo
    // Print to serial monitor
    Serial.print("Servo: ");
    Serial.println(posDegrees);
    delay(30);
  }
  delay(250); // quick pause
}