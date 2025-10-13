// ESP32
// Controller Script BETA: Sender
// https://github.com/BurntWywrm/L2-robot-arm
// Script Reference: https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/


/*
BETA: Tests the ESP-NOW protocol so that
it can properly send information to the
main body, the robot arm.
*/

#include <esp_now.h>
#include <WiFi.h>

/* Global Variables */
// replace with your maximum servo angle
const int MAX_ANGLE = 270; 

// potNamePin
// L(X) dictates the pot heirarchy from base to wrist
const int potL1Pin = 34;
const int potL2Pin = 35;
const int potL3Pin = 32;
const int potL4Pin = 33;

// potNameAngle Storage
int potL1Angle = 0;
int potL2Angle = 0;
int potL3Angle = 0;
int potL4Angle = 0;

// Button variables
const int buttonPin = 25;
int buttonState = "Closed";

/* ESP_NOW Variables */
// Change with your reciever MAC address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

// Structure example to send data
// Must match reciever Structure
typedef struct struct_message{
    int L1AngleData;
    int L2AngleData;
    int L3AngleData;
    int L4AngleData;
    String button_state_data;
} struct_message;

// Create a struct_message called MyData;
struct_message MyData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    Serial.begin(115200); // Starts serial monitor with a baud rate of 115200
    Wifi.mdoe(WIFI_STA); // Set device as a Wifi Station
    
    // Initialize Button
    pinMode(buttonPin, INPUT_PULLUP); // Set button mode as pullup
    buttonState = "Closed";

    // Initialize ESP_NOW
    if (esp_now_init() != ESP_OK){
        Serial.println("Error Initializing ESP_NOW");
        return;
    }
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peerInfo
    memcpy(peerInfo.peer_adr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void loop(){
    get_angle(potL1Pin);
    get_angle(potL2Pin);
    get_angle(potL3Pin);
    get_angle(potL4Pin);
    get_button_state();
    send_controllerInfo();
}

void get_button_state(){
  int buttonVal = digitalRead(buttonPin); // Reads the button signal

  if (buttonVal == LOW){
    buttonState = "Closed";
  }
  else{
    buttonState = "Open";
  }
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

   // Switch statement to store the angle names
   switch(potPin){
    case 34: potL1Angle = potAngle; break; 
    case 35: potL2Angle = potAngle; break;
    case 32: potL3Angle = potAngle; break;
    case 33: potL4Angle = potAngle; break;
   } 
}

void send_controllerInfo(){
    // Set values to send
    myData.L1AngleData = potL1Angle;
    myData.L2AngleData = potL2Angle;
    myData.L3AngleData = potL3Angle;
    myData.L4AngleData = potL4Angle;
    myData.button_state_data = buttonState;
    
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
    Serial.println("Error sending the data");
    }
}