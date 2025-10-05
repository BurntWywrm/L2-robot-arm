// ESP32
// Indivudal Potentiometer Testing
// https://github.com/BurntWywrm/L2-robot-arm

int potPin = 35; // Signal pin of the Potentiometer

void setup()
{
    Serial.begin(115200); // Starts Serial Monitor with a baud rate of 115200
}

void loop()
{
    int potVal = analogRead(potPin); // Reads the signal value

    /*Replace '270' with your maximum servo angle*/
    int degrees = map (potVal, 0, 4095, 0, 270); // Maps & stores the potentiometer analog value into degrees
    Serial.println(degrees); // Prints output to the serial monitor
}