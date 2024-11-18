/************************* TX *************************/
// Library from https://github.com/nRF24/RF24 
#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"
RF24 radio(9, 10); // CE, CSN
const uint64_t send_pipe=0xB01DFACECEL; // This will be this device
const uint64_t recv_pipe=0xDEADBEEFF1L; // This will be the other device
 
// Joystick
#define VRX_PIN A0
#define VRY_PIN A1

void setup()
{
  Serial.begin(9600); // Set up comms with serial monitor
  Serial.println("Start");
  radio.begin(); // Turn on comms
  radio.openWritingPipe(recv_pipe);
  radio.openReadingPipe(1, send_pipe);
  radio.setRetries(15,15);  // This will improve reliability
  radio.setPALevel(RF24_PA_MIN);  // Sets the power level of the transciever
}

#define NO_ACTION 0
#define STOP 2
#define BOTH 3
 
#define SPEED1 10
#define SPEED2 11
#define SPEED3 12
 
#define REVERSE1 13
#define REVERSE2 14
#define REVERSE3 15

#define R_STOP 16
#define L_STOP 17

unsigned long button_code=NO_ACTION; // Set to default
void loop() {
  // Read analog x and y values (10-bit ADC reads 0-1023, 0V-5V)
  int yValue = analogRead(VRY_PIN);
  int xValue = analogRead(VRX_PIN);
  int y_joy_value = map(yValue, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value = map(xValue, 0, 1023, 0, 180);  // For x movement of joystick

// Checking values on serial monitor
//  Serial.println(y_joy_value);
//  Serial.println(x_joy_value);
  
  /************************* Joystick conditions for BOTH motors forward and reverse *************************/
  if((y_joy_value > 89) && (y_joy_value < 92)){
//    Serial.println("Joystick not moving");
    button_code = STOP;
  }  
  
  if((y_joy_value > 95) && (y_joy_value < 120)){
    Serial.println("REVERSE1");
    button_code = REVERSE3;
  }
  if((y_joy_value > 120) && (y_joy_value < 150)){
    Serial.println("REVERSE2");
    button_code = REVERSE3;
  }
  if((y_joy_value > 150) && (y_joy_value < 181)){                                    
    Serial.println("REVERSE3");
    button_code = REVERSE3;
  }
  
  if((y_joy_value > 60) && (y_joy_value < 89)){
    Serial.println("SPEED1");
    button_code = SPEED3;
  }
  if((y_joy_value > 30) && (y_joy_value < 60)){
    Serial.println("SPEED2");
    button_code = SPEED3;
  }
  if((y_joy_value > -1) && (y_joy_value < 30)){
    Serial.println("SPEED3");
    button_code = SPEED3;
  }

  /************************* Implement turning here *************************/
  if(x_joy_value > 179){
    Serial.println("Turing right");
    button_code = L_STOP;
  }
  if(x_joy_value < 5){
    Serial.println("Turning left");
    button_code = R_STOP;
  }

  
  // Button status will be needed later
  int button1_status = digitalRead(button1_pin);
  if(button1_status == 1)
  {
    Serial.println("Button pushed, STOPPING");
    button_code=STOP;
  }


  // Finally, send the code to the receiver
  if(button_code!=NO_ACTION)
  {
    radio.stopListening();  // Make sure it becomes a transmitter
    radio.write(&button_code, sizeof(unsigned long));
    delay(10);
  }

  
} // End void loop
