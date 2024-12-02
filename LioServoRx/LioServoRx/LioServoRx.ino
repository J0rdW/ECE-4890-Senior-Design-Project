//************** RECEIEVER CODE FOR SERVO CONTROL (ARDUINO NANO PINS)  **************//

#include <SPI.h>
#include <Servo.h>
#include "RF24.h" // This is the RF24 library that may need to be installed through the Manage Libraries feature in the IDE.

#include <Servo.h>//Include and create a servo object for controlling the servo motor
//joystick1
Servo servo1;
Servo servo2;

//joystick2
Servo servo3;
Servo servo4;

//joystick3
Servo servo5;
Servo servo6;


RF24 radio(9, 10);//Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; //These are just arbitrary 64bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; //They must be the same on both ends of the communciations

//Codes for actions
#define NO_ACTION 0
#define BOTH 19

//joystick1 action
//motor1 x-direction
#define motor1X 1
#define motor1Y 2
#define servo1stop 3

//motor2 y-direction
#define motor2X 4
#define motor2Y 5
#define servo2stop 6

//joystick2 action
//motor2 x-direction
#define motor3X 7
#define motor3Y 8
#define servo3stop 9

//motor2 y-direction
#define motor4X 10
#define motor4Y 11
#define servo4stop 12

//joystcik3 action
//motor3 motor5
#define motor5X 13
#define motor5Y 14
#define servo5stop 15

//servo3 motor6
#define motor6X 16
#define motor6Y 17
#define servo6stop 18



// Define buttons and joystick pins on ARDUINO NANO
#define SERVO1_PIN 3 //good
#define SERVO2_PIN 5 //good
#define SERVO3_PIN 4 //good
#define SERVO4_PIN 7 //good
#define SERVO5_PIN 11 //good
#define SERVO6_PIN 12 //tbd

// Variables for servo
bool Motor1X_F1= false;
bool Motor1X_R1= false;

bool Motor1Y_F2= false;
bool Motor1Y_R2= false;

bool Motor2X_F3= false;
bool Motor2X_R3= false;

bool Motor2Y_F4= false;
bool Motor2Y_R4= false;

bool Motor3X_F5= false;
bool Motor3X_R5= false;

bool Motor3Y_F6= false;
bool Motor3Y_R6= false;

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
  
  servo1.attach(SERVO1_PIN);//Spool up the servo -pin3
  servo2.attach(SERVO2_PIN);//Spool up the servo -pin5
  
  servo3.attach(SERVO3_PIN);//Spool up the servo -pin4
  servo4.attach(SERVO4_PIN);//Spool up the servo -pin7
  
  servo5.attach(SERVO5_PIN);//Spool up the servo -pin11
  servo6.attach(SERVO6_PIN);//Spool up the servo -pin12


}

// JOYSTICK1 Servo1
void loop()
{
  unsigned long motor_code=NO_ACTION;

  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check ArmServoCodeTX 
    if(motor_code==servo1stop)
    {
      Serial.println("Servo MOTOR1 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    } 
    else if(motor_code==motor1X)
    {
      Serial.println("Servo MOTOR1 forward X-direction");
      MoveX_F1= true;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
     
    //REMOVED motor_code==BOTH
    
    else if(motor_code==motor1Y)
    {
      Serial.println("Servo MOTOR1 reverse in X-direction");
      MoveX_F1= false;
      MoveX_R1= true;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }

    else if(motor_code==servo2stop)
    {
      Serial.println("Servo MOTOR2 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }

    else if(motor_code==motor2X)
    {
      Serial.println("Servo MOTOR2 forward in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= true;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    
    else if(motor_code==motor2Y)
    {
      Serial.println("Servo MOTOR2 reverse in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= true;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    ////////
    ///End of Joystick1
    ////////
    else if(motor_code==servo3stop)
    {
      Serial.println("Servo MOTOR3 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor3X)
    {
      Serial.println("Servo MOTOR3 forward in X-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= true;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor3Y)
    {
      Serial.println("Servo MOTOR3 reverse in X-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= true;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==servo4stop)
    {
      Serial.println("Servo MOTOR4 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor4X)
    {
      Serial.println("Servo MOTOR4 forward in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= true;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor4Y)
    {
      Serial.println("Servo MOTOR4 reverse in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= true;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==servo5stop)
    {
      Serial.println("Servo MOTOR5 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motot5X)
    {
      Serial.println("Servo MOTOR5 forward in X-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= true;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor5Y)
    {
      Serial.println("Servo MOTOR5 reverse in X-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= true;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==servo6stop)
    {
      Serial.println("Servo MOTOR6 has stopped");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= false;
    }
    else if(motor_code==motor6X)
    {
      Serial.println("Servo MOTOR6 forward in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= true;
      MoveY_R6= false;
    }
    else if(motor_code==motor6Y)
    {
      Serial.println("Servo MOTOR6 reverse in Y-direction");
      MoveX_F1= false;
      MoveX_R1= false;
      
      MoveY_F2= false;
      MoveY_R2= false;
    
      MoveX_F3= false;
      MoveX_R3= false;
    
      MoveY_F4= false;
      MoveY_R4= false;
    
      MoveX_F5= false;
      MoveX_R5= false;
    
      MoveY_F6= false;
      MoveY_R6= true;
    }
  }

  
  
  // MOTOR1 forward x-direction (button1)
  if(MoveX_F1)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo1.write(position);
      MoveX_F1 = false;  // Stop so servo doesn't keep going
  }
  
  // MOTOR1 reverse x-direction (button2)
  if(MoveX_R1)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo1.write(position);
      MoveX_R1 = false;  // Stop so servo doesn't keep going
  }
  // MOTOR2 forward y-direction
  if(MoveY_F2)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo2.write(position);
      MoveY_F1 = false;  // Stop so servo doesn't keep going
  }
  
  // MOTOR2 reverse y-direction (button2)
  if(MoveY_R2)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo2.write(position);
      MoveY_R1 = false;  // Stop so servo doesn't keep going
  }
  if(MoveX_F3)
  {
      position += 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo3.write(position);
      MoveX_F3 = false;  // Stop so servo doesn't keep going
  }
  if(MoveX_R3)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo3.write(position);
      MoveY_R1 = false;  // Stop so servo doesn't keep going
  }
  if(MoveY_R4)
  {
      position += 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo4.write(position);
      MoveY_R4 = false;  // Stop so servo doesn't keep going
  }
  if(MoveY_R4)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo4.write(position);
      MoveY_R1 = false;  // Stop so servo doesn't keep going
  }
  if(MoveX_F5)
  {
      position += 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo5.write(position);
      MoveX_F5 = false;  // Stop so servo doesn't keep going
  }
  if(MoveX_R5)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo5.write(position);
      MoveX_R5 = false;  // Stop so servo doesn't keep going
  }
  if(MoveY_F6)
  {
      position += 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo6.write(position);
      MoveY_F6 = false;  // Stop so servo doesn't keep going
  }
  if(MoveY_R6)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo6.write(position);
      MoveY_R6 = false;  // Stop so servo doesn't keep going
  }

  // // JOYSTICK1 Servo2
  /*if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo2YF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveY_F2=true;
      MoveY_R2=false;
    }
    else if(motor_code==Servo2stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveY_F2=false;
      MoveY_R2=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveY_F2=false;
      MoveY_R2=false;
    }
    else if(motor_code==Servo2YR)
    {
      Serial.println("In reverse");
      MoveY_R2=true;
      MoveY_F2=false;
    }
  }
  
  // FORWARD (button1)
  if(MoveY_F2)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo2.write(position);
      MoveY_F2 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveY_R2)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo2.write(position);
      MoveY_R2 = false;  // Stop so servo doesn't keep going
  }

// Joystick2 Servo3
  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo3XF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveX_F3=true;
      MoveX_R3=false;
    }
    else if(motor_code==Servo3stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveX_F3=false;
      MoveX_R3=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveX_F3=false;
      MoveX_R3=false;
    }
    else if(motor_code==Servo3XR)
    {
      Serial.println("In reverse");
      MoveX_R3=true;
      MoveX_F3=false;
    }
  }
  // FORWARD (button1)
  if(MoveX_F3)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo3.write(position);
      MoveX_F3 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveX_R3)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo3.write(position);
      MoveX_R3 = false;  // Stop so servo doesn't keep going
  }


// Joystick 2 Servo 4

  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo4YF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveY_F4=true;
      MoveY_R4=false;
    }
    else if(motor_code==Servo4stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveY_F4=false;
      MoveY_R4=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveY_F4=false;
      MoveY_R4=false;
    }
    else if(motor_code==Servo4YR)
    {
      Serial.println("In reverse");
      MoveY_R4=true;
      MoveY_F4=false;
    }
  }
  // FORWARD (button1)
  if(MoveY_F4)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo4.write(position);
      MoveY_F4 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveY_R4)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo4.write(position);
      MoveY_R4 = false;  // Stop so servo doesn't keep going
  }


  // Joystick 3 Servo 5


  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo5XF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveX_F5=true;
      MoveX_R5=false;
    }
    else if(motor_code==Servo5stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveX_F5=false;
      MoveX_R5=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveX_F5=false;
      MoveX_R5=false;
    }
    else if(motor_code==Servo5XR)
    {
      Serial.println("In reverse");
      MoveX_R5=true;
      MoveX_F5=false;
    }
  }
  // FORWARD (button1)
  if(MoveX_F5)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo5.write(position);
      MoveX_F5 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveX_R5)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo5.write(position);
      MoveX_R5 = false;  // Stop so servo doesn't keep going
  }



  // JOYSTICK 3 Servo 6
  if( radio.available())//Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long));//Stuff the incoming packet into the motor_code variable

    // Check motor_code 
    if(motor_code==Servo6YF)
    {
      Serial.println("THE MOTOR HAS STARTED");
      MoveY_F6=true;
      MoveY_R6=false;
    }
    else if(motor_code==Servo6stop)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      MoveY_F6=false;
      MoveY_R6=false;
    }
    else if(motor_code==BOTH)
    {
      Serial.println("Both buttons pushed");
      MoveY_F6=false;
      MoveY_R6=false;
    }
    else if(motor_code==Servo6YR)
    {
      Serial.println("In reverse");
      MoveY_R6=true;
      MoveY_F6=false;
    }
  }
  // FORWARD (button1)
  if(MoveY_F6)
  {
      position += 3.0f;   // Add 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo6.write(position);
      MoveY_F6 = false;  // Stop so servo doesn't keep going
  }
  
  // REVERSE (button2)
  if(MoveY_R6)
  {
      position -= 3.0f;   // Subtract 5 degrees to initial position
      position = constrain(position, 0, 270);  // Ensure position is within bounds
      servo6.write(position);
      MoveY_R6 = false;  // Stop so servo doesn't keep going
  }
  */
}
