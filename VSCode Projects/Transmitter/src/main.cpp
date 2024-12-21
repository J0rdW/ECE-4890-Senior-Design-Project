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
#define VRX1_PIN A2
#define VRY1_PIN A3

// Joystick SW
#define BUTTON_PIN 8
int buttonState = 0;
int buttonIncrement = 0;

/**************** Motor Control Joystick ****************/
#define MotorJoyX_PIN A0
#define MotorJoyY_PIN A1

#define FRONT 7
#define BACK 6
#define LEFT 5
#define RIGHT 4

void setup()
{
  Serial.begin(115200); // Set up comms with serial monitor
  Serial.println("Start");
  radio.begin(); // Turn on comms
  radio.openWritingPipe(recv_pipe);
  radio.openReadingPipe(1, send_pipe);
  radio.setRetries(15,15);  // This will improve reliability
  radio.setPALevel(RF24_PA_MIN);  // Sets the power level of the transciever

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FRONT, INPUT_PULLUP);
  pinMode(BACK, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
}

/**************** Motor Control Codes ****************/
#define NO_ACTION_MOTOR 19
#define STOP_MOTOR 20
#define BOTH 21
#define SPEED1 22
#define SPEED2 23
#define SPEED3 24
 
#define REVERSE1 25
#define REVERSE2 26
#define REVERSE3 27

#define R_STOP 28
#define L_STOP 29

/**************** Servo Control Codes ****************/
#define NO_ACTION_SERVO 0
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

// Function prototype
void swapServos();

unsigned long button_code=NO_ACTION_SERVO; // Set to default
void loop() {
  /********************** Motor joystick values ********************/
  int motorJoyY = analogRead(MotorJoyY_PIN);
  int motorJoyX = analogRead(MotorJoyX_PIN);
  int yValueMotor = map(motorJoyY, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int xValueMotor = map(motorJoyX, 0, 1023, 0, 180);  // For x movement of joystick

  /********************* Servo joystick values and button logic ********************/
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == 0){
    swapServos();
    delay(200);
  }
  // Read analog x and y values (10-bit ADC reads 0-1023, 0V-5V)
  int yValue1 = analogRead(VRY1_PIN);
  int xValue1 = analogRead(VRX1_PIN);
  int y_joy_value1 = map(yValue1, 0, 1023, 0, 180);  // Remaps 0-1023 ADC value to 0-180, for y movement of joystick
  int x_joy_value1 = map(xValue1, 0, 1023, 0, 180);  // For x movement of joystick
  
  /************************* Joystick conditions for BOTH motors forward and reverse *************************/
  if((yValueMotor > 85) && (yValueMotor < 92)){
    // Serial.println("Joystick not moving");
    button_code = STOP_MOTOR;
  }  
  if((yValueMotor > 95) && (yValueMotor < 120)){
    Serial.println("REVERSE1");
    button_code = REVERSE3;
  }
  if((yValueMotor > 120) && (yValueMotor < 150)){
    Serial.println("REVERSE2");
    button_code = REVERSE3;
  }
  if((yValueMotor > 150) && (yValueMotor < 181)){                                    
    Serial.println("REVERSE3");
    button_code = REVERSE3;
  }
  
  if((yValueMotor > 60) && (yValueMotor < 89)){
    Serial.println("SPEED1");
    button_code = SPEED3;
  }
  if((yValueMotor > 30) && (yValueMotor < 60)){
    Serial.println("SPEED2");
    button_code = SPEED3;
  }
  if((yValueMotor > -1) && (yValueMotor < 30)){
    Serial.println("SPEED3");
    button_code = SPEED3;
  }

  /************************* Implement turning here *************************/
  if(xValueMotor > 179){
    Serial.println("Turing right");
    button_code = L_STOP;
  }
  if(xValueMotor < 5){
    Serial.println("Turning left");
    button_code = R_STOP;
  }

  /************************* State0 values control servos 1 & 2 *************************/
  if(buttonIncrement == 0){
    // if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
    // //  Serial.println("Joystick not moving");
    // button_code = servo2stop;
    // }
    
    // if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
    // //  Serial.println("Joystick not moving");
    // button_code = servo1stop;
    // }  
    
    if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
      Serial.println("Servo MOTOR2 reverse");
      button_code = motor2YR;
    }
    
    if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
      Serial.println("Servo MOTOR2 forward");
      button_code = motor2YF;
    }

    if(x_joy_value1 > 150){
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
    // if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
    // // Serial.println("Joystick not moving");
    // button_code = servo4stop;
    // }
    
    // if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
    // //  Serial.println("Joystick not moving");
    // button_code = servo3stop;
    // }  
    if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
      Serial.println("Servo MOTOR4 reverse");
      button_code = motor4YR;
    }
    
    if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
      Serial.println("Servo MOTOR4 forward");
      button_code = motor4YF;
    }

    if(x_joy_value1 > 150){
      Serial.println("Servo MOTOR3 reverse");
      button_code = motor3XR;
    }
    if(x_joy_value1 < 5){
      Serial.println("Servo MOTOR3 forward");
      button_code = motor3XF;
    }
  }
  /************************* Joystick3 values control motors 5 & 6 *************************/
  if(buttonIncrement == 2){
    // if((y_joy_value1 > 85) && (y_joy_value1 < 95)){
    // //  Serial.println("Joystick not moving");
    // button_code = servo6stop;
    // }
    
    // if((x_joy_value1 > 85) && (x_joy_value1 < 95)){
    // //  Serial.println("Joystick not moving");
    // button_code = servo5stop;
    // }    
    if((y_joy_value1 > 150) && (y_joy_value1 < 181)){                                    
      Serial.println("Servo MOTOR6 reverse");
      button_code = motor6YR;
    }
    
    if((y_joy_value1 > -1) && (y_joy_value1 < 30)){
      Serial.println("Servo MOTOR6 forward");
      button_code = motor6YF;
    }

    if(x_joy_value1 > 150){
      Serial.println("Servo MOTOR5 forward");
      button_code = motor5XF;
    }
    if(x_joy_value1 < 5){
      Serial.println("Servo MOTOR5 reverse");
      button_code = motor5XR;
    } 
  }

  /************************* Fin(the end) *************************/
  // Finally, send the corresponding code to the receiver
  if(button_code!=NO_ACTION_SERVO)
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