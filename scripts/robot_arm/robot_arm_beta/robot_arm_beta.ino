// ESP32
// Robot Arm Script ALPHA: Reciever
// https://github.com/BurntWywrm/L2-robot-arm
// Script Reference: https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

/*
ALPHA: Tests the ESP-NOW protocol so that
it can recieve information from the controller.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/* Global Variables */

// -- Robot Arm Variables --

#define FREQUENCY 50
const int MAX_ANGLE = 180;

// Servo pulse length
const int SERVO_MIN  = 120; // Minimum pulse length count of 4096
const int SERVO_MAX = 500; // Minimum pulse length count of 4096

// Create object to represent PCA9685 at your I2C address
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Set I2C Pins for the ESP32, default: SDA = 21, SCL = 22.
const int I2C_SDA = 21;
const int I2C_SCL = 22;

// PCA9685 board Servo Pins
const int servoL1Pin = 15;
const int servoL2Pin = 14;
const int servoL3Pin = 13;
const int servoL4Pin = 12;

// End Effector Variables
const int gripper = 11;
String gripper_state = "";

// -- Controller Variables --

// potNameAngle Storage
// L(X) dictates the pot heirarchy from base to wrist
int potL1Angle = 0;
int potL2Angle = 0;
int potL3Angle = 0;
int potL4Angle = 0;

/* ESP_NOW Variables */
// Structure example to recieve data
// Must match sender Structure
typedef struct struct_message{
    int L1AngleData;
    int L2AngleData;
    int L3AngleData;
    int L4AngleData;
    char button_state_data[8];
} struct_message;

// Create a struct_message called MyData;
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    potL1Angle = myData.L1AngleData;
    potL2Angle = myData.L2AngleData;
    potL3Angle = myData.L3AngleData;
    potL4Angle = myData.L4AngleData;
    gripper_state = String(myData.button_state_data);
}   

void setup()
{
    Serial.begin(115200); // Starts serial monitor with a baud rate of 115200
    WiFi.mode(WIFI_STA); // Set device as a wifi Station
    Wire.begin(I2C_SDA, I2C_SCL);

    // Initialize PWM Servo Libarary
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);

    // Initialize ESP_NOW
    if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
    set_angle(potL1Angle, servoL1Pin);
    set_angle(potL2Angle, servoL2Pin);
    set_angle(potL3Angle, servoL3Pin);
    set_angle(potL4Angle, servoL4Pin);

    // Open/Closes Gripper
    if (gripper_state.equalsIgnoreCase("Open")){
        set_angle(180, gripper);
    } else if (gripper_state.equalsIgnoreCase("Closed")){
        set_angle(0, gripper);
    }
}

// manually_set_angle(desired_angle, servoPin);
void set_angle(int desired_angle, int servoPin){
    int pulse_width = map(desired_angle, 0, MAX_ANGLE, SERVO_MIN, SERVO_MAX); // Determines pwm pulse width
    pwm.setPWM(servoPin, 0, pulse_width); // Write to servo monitor

    // Locates the servo name for reference
    String servoName = "";
    switch(servoPin){
        case 15: servoName = "L1"; break;
        case 14: servoName = "L2"; break;
        case 13: servoName = "L3"; break;
        case 12: servoName = "L4"; break;
    }

    // Displays updated servo angle
    Serial.println("Updated " + (servoName) + ": " + (desired_angle));    
}

void reset_arm(){
    delay(2500); // Delay so the robot arm doesn't overload power supply
    set_angle(90, servoL1Pin); // Default postion: 90°
    delay(2500);
    set_angle(0, servoL2Pin); // Default position: 0°
    delay(2500);
    set_angle(180, servoL3Pin); // Default position: 180°
    delay(2500);
    set_angle(90, servoL4Pin); // Default postion: 90°
}