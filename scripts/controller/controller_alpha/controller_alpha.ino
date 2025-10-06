// ESP32
// Controller Testing ALPHA
// https://github.com/BurntWywrm/L2-robot-arm

/*
ALPHA: Tests if the potentiometers
properly sends signals to the ESP32.
As well as the ESP32 properly converting
the signals to the correct angle.
*/

// variables
const int MAX_ANGLE = 270; // replace with your maximum servo angle

// Potetiometer variables
// L(X) dictates the pot heirarchy from base to wrist
int potL1Pin = 34;
int potL2Pin = 35;
int potL3Pin = 32;
int potL4Pin = 33;

// Button variables
int buttonPin = 25;
String buttonState = "";

void setup()
{
    Serial.begin(115200); // Starts Serial Monitor with a baud rate of 115200
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
    get_angle(potL1Pin);
    get_angle(potL2Pin);
    get_angle(potL3Pin);
    get_angle(potL4Pin);
    get_button_state();
}

void get_angle(int potPin){
    int potVal = analogRead(potPin); // Reads the signal value
    int potAngle = map(potVal, 0, 4095, 0, MAX_ANGLE); // Maps & stores the potentiometer analog value into degrees

    /* Simple switch statement to assign the correct potentiometer name */
    String potName = ""; // Stores pot name
    switch(potPin){
    case 34: potName = "Joint L1"; break;
    case 35: potName = "Joint L2"; break;
    case 32: potName = "Joint L3"; break;
    case 33: potName = "Joint L4"; break;
    }

    /* Displays current pot angle */
    Serial.print(potName);
    Serial.print(": ");
    Serial.print(potAngle);
    Serial.println(" Degrees");
}

void get_button_state(){
  int buttonVal = digitalRead(buttonPin); // Reads the button signal

  if (buttonVal == LOW){
    buttonState = "Close";
  }
  else{
    buttonState = "Open";
  }
}