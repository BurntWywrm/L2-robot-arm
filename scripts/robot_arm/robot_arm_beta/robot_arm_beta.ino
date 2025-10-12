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

/* Global Variables */

// Controller Variables

// potNameAngle Storage
// L(X) dictates the pot heirarchy from base to wrist
int potL1Angle = 0;
int potL2Angle = 0;
int potL3Angle = 0;
int potL4Angle = 0;

// Button Storage
String buttonState = "Closed";

/* ESP_NOW Variables */
// Structure example to recieve data
// Must match sender Structure
typedef struct struct_message{
    int L1AngleData;
    int L2AngleData;
    int L3AngleData;
    int L4AngleData;
    String button_state_data;
} struct_message;

// Create a struct_message called MyData;
struct_message MyData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    potL1Angle = MyData.L1AngleData;
    potL1Angle = MyData.L2AngleData;
    potL1Angle = MyData.L3AngleData;
    potL1Angle = MyData.L4AngleData;
    buttonState = MyData.button_state_data;
}   

void setup()
{
    Serial.begin(115200); // Starts serial monitor with a baud rate of 115200
    WiFi.mode(WIFI_STA); // Set device as a wifi Station
    
    // Initialize ESP_NOW
    if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
    }
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop()
{
}