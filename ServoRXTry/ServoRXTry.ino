//************** RECEIEVER CODE FOR SERVO CONTROL (ARDUINO NANO PINS)  **************//

#include <SPI.h>
#include <Servo.h>
#include "RF24.h" // This is the RF24 library that may need to be installed through the Manage Libraries feature in the IDE.

#include <Servo.h>//Include and create a servo object for controlling the servo motor
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;


RF24 radio(9, 10);//Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; //These are just arbitrary 64bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; //They must be the same on both ends of the communciations

// Codes for actions
#define NO_ACTION 21
#define BOTH 20
// Servos
#define Servo1XF 1
#define Servo1XR 2
#define Servo1stop 3
#define Servo2YF 5
#define Servo2YR 6
#define Servo2stop 7
#define Servo3XF 8
#define Servo3XR 9
#define Servo3stop 10
#define Servo4YF 11
#define Servo4YR 12
#define Servo4stop 13
#define Servo5XF 14
#define Servo5XR 15
#define Servo5stop 16
#define Servo6YF 17
#define Servo6YR 18
#define Servo6stop 19



// Define buttons and joystick pins on ARDUINO NANO
#define SERVO1_PIN 3
#define SERVO2_PIN 5
#define SERVO3_PIN 4
#define SERVO4_PIN 7
#define SERVO5_PIN 11
#define SERVO6_PIN 12

// Variables for servo
bool MoveX_F1=false;
bool MoveX_R1 = false;
float position=90;  // Starts at 90 degrees
int direction=1;

void setup()
{
  Serial.begin(9600);//Set up comm with the IDE serial monitor
  Serial.println("Ready for commands");
  radio.begin();//Start up the radio object
  radio.setRetries(15,15);//This will improve reliability of the module if it encounters interference
  radio.setPALevel(RF24_PA_LOW);//This sets the power low. This will reduce the range. RF24_PA_MAX would increase the range
  radio.openWritingPipe(send_pipe);//Thses are the reverse of the transmit code.
  radio.openReadingPipe(1,recv_pipe);
  radio.startListening();//Give the module a kick
  servo1.attach(SERVO1_PIN);//Spool up the servo
  servo2.attach(SERVO2_PIN);//Spool up the servo
  servo3.attach(SERVO3_PIN);//Spool up the servo
  servo4.attach(SERVO4_PIN);//Spool up the servo
  servo5.attach(SERVO5_PIN);//Spool up the servo
  servo6.attach(SERVO6_PIN);//Spool up the servo


}

// Joystick1 Servo1
void loop()
{
  unsigned long motor_code=NO_ACTION;

  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo1XF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveX_F1=true;
      MoveX_R1=false;
    }
    else if(motor_code==Servo1stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveX_F1=false;
      MoveX_R1=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveX_F1=false;
      MoveX_R1=false;
    }
    else if(motor_code==Servo1XR)
    {
      Serial.println("In reverse");
      MoveX_R1=true;
      MoveX_F1=false;
    }
  }
  // FORWARD (button1)
  if(MoveX_F1)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo1.write(position);
      MoveX_F1 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveX_R1)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo1.write(position);
      MoveX_R1 = false;  // Stop so servo doesn't keep going
  }

}

