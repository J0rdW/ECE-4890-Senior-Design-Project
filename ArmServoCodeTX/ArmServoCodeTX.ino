/************************* TX *************************/
// Library from https://github.com/nRF24/RF24 
#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"
RF24 radio(9, 10); // CE, CSN
const uint64_t send_pipe=0xB01DFACECEL; // This will be this device TX
const uint64_t recv_pipe=0xDEADBEEFF1L; // This will be the other device RX
 
// Joystick1
#define VRX1_PIN A0
#define VRY1_PIN A1

// Joystick2
#define VRX2_PIN A2 //Arbitrary A pin
#define VRY2_PIN A3 //Arbitrary A pin

// Joystick3
#define VRX3_PIN A4 // Arbitrary A pin
#define VRY3_PIN A5 // Arbitrary A pin

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

//Joystick 1 controls servo motor1 when input is +/- X-direction
//Joystick 1 controls servo motor2 when input is +/- Y-direction
#define motor1X 1
#define motor1Y 2

#define motor2X 3
#define motor2Y 4

//Joystick 2 controls servo motor3 when input is +/- X-direction
//Joystick 2 controls servo motor4 when input is +/- Y-direction
#define motor3X 5
#define motor3Y 6

#define motor4X 7
#define motor4Y 8

//Joystick 3 controls servo motor5 when input is +/- X-direction
//Joystick 3 controls servo motor6 when input is +/- Y-direction
#define motor5X 9
#define motor5Y 10

#define motor6X 11
#define motor6Y 12

unsigned long button_code=NO_ACTION; // Set to default
void loop() {
  // Read analog x and y values (10-bit ADC reads 0-1023, 0V-5V)
  int yValue1 = analogRead(VRY1_PIN);
  int xValue1 = analogRead(VRX1_PIN);

  int yValue2 = analogRead(VRY2_PIN);
  int xValue2 = analogRead(VRX2_PIN);

  int yValue3 = analogRead(VRY3_PIN);
  int xValue3 = analogRead(VRX3_PIN);
  
  int y_joy_value1 = map(yValue1, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value1 = map(xValue1, 0, 1023, 0, 180);  // For x movement of joystick

  int y_joy_value2 = map(yValue2, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value2 = map(xValue2, 0, 1023, 0, 180);  // For x movement of joystick
  
  int y_joy_value3 = map(yValue3, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value3 = map(xValue3, 0, 1023, 0, 180);  // For x movement of joystick



// Checking values on serial monitor
// Serial.println(y_joy_value);
// Serial.println(x_joy_value);
  
/************************* Joystick1 values control motors 1 & 2 *************************/
  if((y_joy_value > 89) && (y_joy_value < 92)){
// Serial.println("Joystick not moving");
    button_code = STOP;
  }  
  
  if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
    Serial.println("Servo MOTOR2 forward");
    button_code = motor1X;
  }
  
  if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
    Serial.println("Servo MOTOR2 reverse");
    button_code = motor1Y;
  }

  if(x_joy_value1 > 179){
    Serial.println("Servo MOTOR1 forward");
    button_code = motor2X;
  }
  if(x_joy_value1 < 5){
    Serial.println("Servo MOTOR1 reverse");
    button_code = motor2Y;
  }
/************************* Joystick2 values control motors 3 & 4 *************************/
  if((y_joy_value2 > 150) && (y_joy_value2 < 181)){                                    
    Serial.println("Servo MOTOR3 forward");
    button_code = motor3X;
  }
  
  if((y_joy_value2 > -1) && (y_joy_value2 < 30)){
    Serial.println("Servo MOTOR3 reverse");
    button_code = motor3Y;
  }

  if(x_joy_value2 > 179){
    Serial.println("Servo MOTOR4 forward");
    button_code = motor4X;
  }
  if(x_joy_value2 < 5){
    Serial.println("Servo MOTOR4 reverse");
    button_code = motor4Y;
  }
/************************* Joystick1 values control motors 5 & 6 *************************/  
  if((y_joy_value3 > 150) && (y_joy_value3 < 181)){                                    
    Serial.println("Servo MOTOR2 forward");
    button_code = motor5X;
  }
  
  if((y_joy_value3 > -1) && (y_joy_value3 < 30)){
    Serial.println("Servo MOTOR2 reverse");
    button_code = motor5Y;
  }

  if(x_joy_value3 > 179){
    Serial.println("Servo MOTOR1 forward");
    button_code = motor6X;
  }
  if(x_joy_value3 < 5){
    Serial.println("Servo MOTOR1 reverse");
    button_code = motor6Y;
  }
/************************* Fin(the end) *************************/
  
  // Button status will be needed later
  //int button1_status = digitalRead(button1_pin);
  //if(button1_status == 1)
  //{
    //Serial.println("Button pushed, STOPPING");
    //button_code=STOP;
  //}


  // Finally, send the code to the receiver
  if(button_code!=NO_ACTION)
  {
    radio.stopListening();  // Make sure it becomes a transmitter
    radio.write(&button_code, sizeof(unsigned long));
    delay(10);
  }

  
} // End void loop
