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

    // Set gripper to 90 Degrees (close)
    pwm.setPWM(gripper, 0, 90);
    gripper_state = "close";

    Serial.println("Robot Arm: ALPHA");
    Serial.println("Quick Controls: ");
    Serial.println("Input only accepts 'MAX/MIN':'INT'");
}

void loop()
{
    configure_pulse_count();
}

// Manually configure the MAX/MIN within the Serial Monitor live
void configure_pulse_count(){
    if (Serial.available() > 0){
        String input = Serial.readStringUntil('\n'); // Read Input Line
        input.trim(); // Removes whitespace and new line

        int seperatorIndex = input.indexOf(":") 

        if (seperatorIndex > 0 ){
            String pulse_countKEY = input.substring(0, seperatorIndex);
            String valueSTR = input.substring(seperatorIndex + 1);
            valueSTR.trim();
            value = valueSTR.toInt(); // Converts to integer

            // Matches key and assigns value
            if (pulse_countKEY.equalsIgnoreCase("MAX")){
                MAX = value;
                Serial.println("Updated MAX to " + String(MAX));
            }
            else if (pulse_countKEY.equalsIgnoreCase("MIN")){
                MIN = value;
                Serial.println("Updated MAX to " + String(MIN));
            }
            else{
                Serial.println("Unknown input: " + pulse_countKEY);
            }
        }
    }
}