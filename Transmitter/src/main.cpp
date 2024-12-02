//************************* TX *************************//
// Library from https://github.com/nRF24/RF24 
#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"
RF24 radio(9, 10); // CE, CSN
const uint64_t send_pipe=0xB01DFACECEL; // This will be this device TX
const uint64_t recv_pipe=0xDEADBEEFF1L; // This will be the other device RX
 
/**************** Servo Joystick ****************/
// Joystick1
#define VRX1_PIN A0
#define VRY1_PIN A1

// Joystick SW
#define BUTTON_PIN 8
int buttonState = 0;
int buttonIncrement = 0;

/**************** Motor Control Joystick ****************/


void setup()
{
  Serial.begin(9600); // Set up comms with serial monitor
  Serial.println("Start");
  radio.begin(); // Turn on comms
  radio.openWritingPipe(recv_pipe);
  radio.openReadingPipe(1, send_pipe);
  radio.setRetries(15,15);  // This will improve reliability
  radio.setPALevel(RF24_PA_MIN);  // Sets the power level of the transciever

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

#define NO_ACTION 0
#define BOTH 19
#define STOP 20

//Joystick 1 controls servo motor1 when input is +/- X-direction
//Joystick 1 controls servo motor2 when input is +/- Y-direction
#define motor1XF 1
#define motor1XR 2
#define servo1stop 3

#define motor2YF 4
#define motor2YR 5
#define servo2stop 6
//Joystick 2 controls servo motor3 when input is +/- X-direction
//Joystick 2 controls servo motor4 when input is +/- Y-direction
#define motor3XF 7
#define motor3XR 8
#define servo3stop 9

#define motor4YR 10
#define motor4YF 11
#define servo4stop 12
//Joystick 3 controls servo motor5 when input is +/- X-direction
//Joystick 3 controls servo motor6 when input is +/- Y-direction
#define motor5XF 13
#define motor5XR 14
#define servo5stop 15

#define motor6YF 16
#define motor6YR 17
#define servo6stop 18

void swapServos();

unsigned long button_code=NO_ACTION; // Set to default
void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == 0){
    swapServos();
  }
  delay(100);
  Serial.println(buttonState);
  // Read analog x and y values (10-bit ADC reads 0-1023, 0V-5V)
  int yValue1 = analogRead(VRY1_PIN);
  int xValue1 = analogRead(VRX1_PIN);
  

 /* int yValue2 = analogRead(VRY2_PIN);
  int xValue2 = analogRead(VRX2_PIN);

  int yValue3 = analogRead(VRY3_PIN);
  int xValue3 = analogRead(VRX3_PIN); */
  
  int y_joy_value1 = map(yValue1, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value1 = map(xValue1, 0, 1023, 0, 180);  // For x movement of joystick
  // Serial.println(x_joy_value1);
  // Serial.println(y_joy_value1);


 /* int y_joy_value2 = map(yValue2, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value2 = map(xValue2, 0, 1023, 0, 180);  // For x movement of joystick
  
  int y_joy_value3 = map(yValue3, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value3 = map(xValue3, 0, 1023, 0, 180);  // For x movement of joystick */

  
/************************* State0 values control motors 1 & 2 *************************/
  if(buttonIncrement == 0){
    if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
    //  Serial.println("Joystick not moving");
    button_code = servo2stop;
    }
    
    if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
    //  Serial.println("Joystick not moving");
    button_code = servo1stop;
    }  
    
    if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
      Serial.println("Servo MOTOR2 forward");
      button_code = motor2YF;
    }
    
    if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
      Serial.println("Servo MOTOR2 reverse");
      button_code = motor2YR;
    }

    if(x_joy_value1 > 179){
      Serial.println("Servo MOTOR1 forward");
      button_code = motor1XF;
    }
    if(x_joy_value1 < 5){
      Serial.println("Servo MOTOR1 reverse");
      button_code = motor1XR;
    }
  }
  
/************************* State 1 values control motors 3 & 4 *************************/
if(buttonIncrement == 1){
  if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
  //  Serial.println("Joystick not moving");
   button_code = servo4stop;
  }
  
  if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
  //  Serial.println("Joystick not moving");
   button_code = servo3stop;
  }  
  if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
    Serial.println("Servo MOTOR4 forward");
    button_code = motor4YF;
  }
  
  if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
    Serial.println("Servo MOTOR4 reverse");
    button_code = motor4YR;
  }

  if(x_joy_value1 > 179){
    Serial.println("Servo MOTOR5 forward");
    button_code = motor3XF;
  }
  if(x_joy_value1 < 5){
    Serial.println("Servo MOTOR5 reverse");
    button_code = motor3XR;
  }
}
/************************* Joystick3 values control motors 5 & 6 *************************/
if(buttonIncrement == 2){
  if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
  //  Serial.println("Joystick not moving");
   button_code = servo6stop;
  }
  
  if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
  //  Serial.println("Joystick not moving");
   button_code = servo5stop;
  }    
  if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
    Serial.println("Servo MOTOR6 forward");
    button_code = motor6YF;
  }
  
  if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
    Serial.println("Servo MOTOR6 reverse");
    button_code = motor6YR;
  }

  if(x_joy_value1 > 179){
    Serial.println("Servo MOTOR1 forward");
    button_code = motor5XF;
  }
  if(x_joy_value1 < 5){
    Serial.println("Servo MOTOR1 reverse");
    button_code = motor5XR;
  } 
}
/************************* Fin(the end) *************************/
  // Finally, send the code to the receiver
  if(button_code!=NO_ACTION)
  {
    radio.stopListening();  // Make sure it becomes a transmitter
    radio.write(&button_code, sizeof(unsigned long));
    delay(10);
  }

  
} // End void loop

void swapServos(){
  buttonIncrement++;
  Serial.println(buttonIncrement);
  if(buttonIncrement == 3){
    buttonIncrement = 0;
  }
}