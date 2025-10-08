// ESP32
// Robot Arm Script ALPHA
// https://github.com/BurntWywrm/L2-robot-arm

/*
ALPHA: Tests if 5 of the actuators are properly 
rotating according to user input via serial monitor.
With a servo constrait with a minimum of 0 
degrees to a maximum of 180 degrees.
*/

#include <Wire.h> // I2C Wire Library
#include <Adafruit_PWMServoDriver.h> // Adafruit PWM Servo Library

// Configure in Serial Monitor during testing
int MIN = 120; // Minimum pulse length count out of 4096
int MAX = 500; // Maximum pulse length count out of 4096

// Create object to represent PCA9685 at your I2C address
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Set I2C Pins for the ESP32, default: SDA = 21, SCL = 22.
const int I2C_SDA = 21;
const int I2C_SCL = 22;

// Actuator variables
// Configure as necessary
#define FREQUENCY 50
const int MAX_ANGLE = 180;

const int servoL1Pin = 15;
const int servoL2Pin = 14;
const int servoL3Pin = 13;
const int servoL4Pin = 12;

// End Effector variables
const int gripper = 11;
String gripper_state = "";

void setup()
{
    Serial.begin(115200); // Starts serial monitor with a baud rate of 115200
    Wire.begin(I2C_SDA, I2C_SCL);

    // Initialize PWM Servo Library
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);

    // Set gripper to 0 Degrees (close)
    int pulse_width = map(0, 0, MAX_ANGL, MIN, MAX);
    pwm.setPWM(gripper, 0, pulse_width);
    gripper_state = "Close";

    Serial.println("Robot Arm: ALPHA");
    Serial.println("Quick Controls: ");
    Serial.println("Configure MAX/MIN -> 'MAX/MIN':'INT'");
    Serial.println("Configure Servo Angle -> 'servoname(L1):'0-180'");
    Serial.println("Configure Gripper state -> 'Gripper: 1 or 0'");

    // Resets actuators to default positions
    reset_arm(); 
}

void loop()
{
    input_configuration();
}

void reset_arm(){
    set_angle(90, servoL1Pin); // Default postion: 90°
    set_angle(0, servoL2Pin); // Default position: 0°
    set_angle(180, servoL3Pin); // Default position: 180°
    set_angle(90, servoL4Pin); // Default postion: 90°
}

void input_configuration(){
    if (Serial.available() > 0){
        String input = Serial.readStringUntil('\n'); // Read Input Line
        input.trim(); // Removes whitespace and new line

        int seperatorIndex = input.indexOf(":") 

        if (seperatorIndex > 0 ){
            String key = input.substring(0, seperatorIndex);
            String valueSTR = input.substring(seperatorIndex + 1);
            valueSTR.trim();
            value = valueSTR.toInt(); // Converts to integer

            // Configures the MAX/MIN
            if (key.equalsIgnoreCase("MAX")){
                MAX = value;
                Serial.println("Updated MAX to " + String(MAX));
            }
            else if (key.equalsIgnoreCase("MIN")){
                MIN = value;
                Serial.println("Updated MAX to " + String(MIN));
            }

            // Configures servo angle
            else if(key.equalsIgnoreCase("L1")){
                if (value > 180){
                    Serial.println("Value exceeds Servo specifications");
                }
                else if (value < 0){
                    Serial.println("Value exceeds Servo specifications");
                }
                else{
                    set_angle(value, servoL1Pin);
                }
            }

            else if(key.equalsIgnoreCase("L2")){
                if (value > 180){
                    Serial.println("Value exceeds Servo specifications");
                }
                else if (value < 0){
                    Serial.println("Value exceeds Servo specifications");
                }
                else{
                    set_angle(value, servoL2Pin);
                }
            }

            else if(key.equalsIgnoreCase("L3")){
                if (value > 180){
                    Serial.println("Value exceeds Servo specifications");
                }
                else if (value < 0){
                    Serial.println("Value exceeds Servo specifications");
                }
                else{
                    set_angle(value, servoL3Pin);
                }
            }

            else if(key.equalsIgnoreCase("L4")){
                if (value > 180){
                    Serial.println("Value exceeds Servo specifications");
                }
                else if (value < 0){
                    Serial.println("Value exceeds Servo specifications");
                }
                else{
                    set_angle(value, servoL4Pin);
                }
            }

            else if(key.equalsIgnoreCase("Gripper")){
                // Opens gripper
                if(value == 1){
                    int open_gripper = map(180, 0, MAX_ANGLE, MIN, MAX);
                    pwm.setPWM(gripper, 0, open_gripper);
                    gripper_state = "Open";
                }
                // Closes gripper
                else if (value == 0){
                    int close_gripper = map(0, 0, MAX_ANGLE, MIN, MAX);
                    pwm.setPWM(gripper, 0, close_gripper);
                    gripper_state = "Close";
                }
            }
            
            // Error Handler
            else{
                Serial.println("Unknown input: " + key);
            }
        }
    }
}

// set_angle(desired_angle, servo_pin);
void set_angle(int userVal, int servoPin){
    int pulse_width = map(userVal, 0, MAX_ANGLE, MIN, MAX); // Determines pwm pulse width
    pwm.setPWM(servoPin, 0, pulse_width) // write to servo
    delay(30);
    
    // Locates the servo name for reference
    String servoName = "";
    switch(servoPin){
        case 15: servoName = "L1"; break;
        case 14: servoName = "L2"; break;
        case 13: servoName = "L3"; break;
        case 12: servoName = "L4"; break;
    }

    // Displays updated servo angle
    Serial.println("Updated " + (servoName) + ": " + (userVal));
}