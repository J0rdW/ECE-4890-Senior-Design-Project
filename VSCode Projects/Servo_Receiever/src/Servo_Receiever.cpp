//************** RX CODE FOR SERVO CONTROL (ARDUINO NANO PINS)  **************//

#include <SPI.h>
#include <Servo.h>
#include "RF24.h" // This is the RF24 library that may need to be installed through the Manage Libraries feature in the IDE.
#include <Servo.h>//Include and create a servo object for controlling the servo motor

//joystick1 controls two motors (1 & 2)
Servo servo1;
Servo servo2;

//joystick2 controls two motors (3 & 4)
Servo servo3;
Servo servo4;

//joystick3 copntrols two motors (5 & 6)
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
#define motor1XF 1
#define motor1XR 2
#define servo1stop 3

//motor2 y-direction
#define motor2YF 4
#define motor2YR 5
#define servo2stop 6

//joystick2 action
//motor2 x-direction
#define motor3XF 7
#define motor3XR 8
#define servo3stop 9

//motor2 y-direction
#define motor4YF 10
#define motor4YR 11
#define servo4stop 12

//joystcik3 action
//motor3 motor5
#define motor5XF 13
#define motor5XR 14
#define servo5stop 15

//servo3 motor6
#define motor6YF 16
#define motor6YR 17
#define servo6stop 18



// Define buttons and joystick pins on ARDUINO NANO
#define SERVO1_PIN 3 //good
#define SERVO2_PIN 5 //good
#define SERVO3_PIN 4 //good
#define SERVO4_PIN 7 //good
#define SERVO5_PIN 11 //good
#define SERVO6_PIN 12 //tbd

// Variables for servo
bool motor1X_F1= false;
bool motor1X_R1= false;

bool motor2Y_F2= false;
bool motor2Y_R2= false;

bool motor3X_F3= false;
bool motor3X_R3= false;

bool motor4Y_F4= false;
bool motor4Y_R4= false;

bool motor5X_F5= false;
bool motor5X_R5= false;

bool motor6Y_F6= false;
bool motor6Y_R6= false;

float position1=60;  // Starts at 90 degrees
float position2=110;  // Starts at 90 degrees
float position3=120;  // Starts at 90 degrees
float position4=70;  // Starts at 90 degrees
float position5=90;  // Starts at 90 degrees
float position6=90;  // Starts at 90 degrees

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


  servo1.write(position1); // initializing the position for servo 1
  servo2.write(position2); // initializing the position for servo 2
  servo3.write(position3); // initializing the position for servo 3 
  servo4.write(position4); // initializing the position for servo 4
  servo5.write(position5); // initializing the position for servo 5
  servo6.write(position6); // initializing the position for servo 6


  delay(5000);
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
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    } 
    else if(motor_code==motor1XF)
    {
      Serial.println("Servo MOTOR1 forward X-direction");
      motor1X_F1= true;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
     
    //REMOVED motor_code==BOTH
    
    else if(motor_code==motor1XR)
    {
      Serial.println("Servo MOTOR1 reverse in X-direction");
      motor1X_F1= false;
      motor1X_R1= true;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }

    else if(motor_code==servo2stop)
    {
      Serial.println("Servo MOTOR2 has stopped");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }

    else if(motor_code==motor2YF)
    {
      Serial.println("Servo MOTOR2 forward in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= true;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    
    else if(motor_code==motor2YR)
    {
      Serial.println("Servo MOTOR2 reverse in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= true;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    
    else if(motor_code==servo3stop)
    {
      Serial.println("Servo MOTOR3 has stopped");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;

      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor3XF)
    {
      Serial.println("Servo MOTOR3 forward in X-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= true;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor3XR)
    {
      Serial.println("Servo MOTOR3 reverse in X-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= true;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==servo4stop)
    {
      Serial.println("Servo MOTOR4 has stopped");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor4YF)
    {
      Serial.println("Servo MOTOR4 forward in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= true;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor4YR)
    {
      Serial.println("Servo MOTOR4 reverse in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= true;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==servo5stop)
    {
      Serial.println("Servo MOTOR5 has stopped");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor5XF)
    {
      Serial.println("Servo MOTOR5 forward in X-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= true;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor5XR)
    {
      Serial.println("Servo MOTOR5 reverse in X-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= true;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==servo6stop)
    {
      Serial.println("Servo MOTOR6 has stopped");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= false;
    }
    else if(motor_code==motor6YF)
    {
      Serial.println("Servo MOTOR6 forward in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= true;
      motor6Y_R6= false;
    }
    else if(motor_code==motor6YR)
    {
      Serial.println("Servo MOTOR6 reverse in Y-direction");
      motor1X_F1= false;
      motor1X_R1= false;
      
      motor2Y_F2= false;
      motor2Y_R2= false;
    
      motor3X_F3= false;
      motor3X_R3= false;
    
      motor4Y_F4= false;
      motor4Y_R4= false;
    
      motor5X_F5= false;
      motor5X_R5= false;
    
      motor6Y_F6= false;
      motor6Y_R6= true;
    }
  }

  
  
  // MOTOR1 forward x-direction (button1)
  if(motor1X_F1)
  {
      position1 += 5.0f;   // Add 5 degrees to initial position
      position1 = constrain(position1, 0, 130);  // Ensure position is within bounds
      servo1.write(position1);
      motor1X_F1 = false;  // Stop so servo doesn't keep going
      Serial.println("moving F help...........");
      Serial.println(position1);
      delay(30);  
      
  }
  
  // MOTOR1 reverse x-direction (button2)
  if(motor1X_R1)
  {
      position1 -= 5.0f;   // Subtract 5 degrees to initial position
      position1 = constrain(position1, 0, 130);  // Ensure position is within bounds
      servo1.write(position1);
      motor1X_R1 = false;  // Stop so servo doesn't keep going
      Serial.println("moving R help...........");
      Serial.println(position1);
      delay(30);
  }
  // MOTOR2 forward y-direction
  if(motor2Y_F2)
  {
      position2 += 5.0f;   // Add 5 degrees to initial position
      position2 = constrain(position2, 45, 175);  // Ensure position is within bounds
      servo2.write(position2);
      motor2Y_F2 = false;  // Stop so servo doesn't keep going
      Serial.println(position2); // shows the position on serial mon 
  }
  
  // MOTOR2 reverse y-direction (button2)
  if(motor2Y_R2)
  {
      position2 -= 5.0f;   // Subtract 5 degrees to initial position
      position2 = constrain(position2, 45, 175);  // Ensure position is within bounds
      servo2.write(position2);
      motor2Y_R2 = false;  // Stop so servo doesn't keep going
      Serial.println(position2); // shows the position on serial mon 
  }
  if(motor3X_F3)
  {
      position3 += 5.0f;   // Subtract 5 degrees to initial position
      position3 = constrain(position3, 30, 190);  // Ensure position is within bounds
      servo3.write(position3);
      motor3X_F3 = false;  // Stop so servo doesn't keep going
      Serial.println(position3); // shows the position on serial mon
  }
  if(motor3X_R3)
  {
      position3 -= 5.0f;   // Subtract 5 degrees to initial position
      position3 = constrain(position3, 30, 190);  // Ensure position is within bounds
      servo3.write(position3);
      motor3X_R3 = false;  // Stop so servo doesn't keep going
      Serial.println(position3); // shows the position on serial mon
  }
  if(motor4Y_F4)
  {
      position4 += 5.0f;   // Subtract 5 degrees to initial position
      position4 = constrain(position4, 40, 150);  // Ensure position is within bounds
      servo4.write(position4);
      motor4Y_F4 = false;  // Stop so servo doesn't keep going
      Serial.println(position4); // shows the position on serial mon
  }
  if(motor4Y_R4)
  {
      position4 -= 5.0f;   // Subtract 5 degrees to initial position
      position4 = constrain(position4, 40, 150);  // Ensure position is within bounds
      servo4.write(position4);
      motor4Y_R4 = false;  // Stop so servo doesn't keep going
      Serial.println(position4); // shows the position on serial mon
  }
  if(motor5X_F5)
  {
      position5 += 5.0f;   // Subtract 5 degrees to initial position
      position5 = constrain(position5, 0, 270);  // Ensure position is within bounds
      servo5.write(position5);
      motor5X_F5 = false;  // Stop so servo doesn't keep going
      Serial.println(position5); // shows the position on serial mon
  }
  if(motor5X_R5)
  {
      position5 -= 5.0f;   // Subtract 5 degrees to initial position
      position5 = constrain(position5, 0, 270);  // Ensure position is within bounds
      servo5.write(position5);
      motor5X_R5 = false;  // Stop so servo doesn't keep going
      Serial.println(position5); // shows the position on serial mon
  }
  if(motor6Y_F6)
  {
      position6 += 5.0f;   // Subtract 5 degrees to initial position
      position6 = constrain(position6, 0, 270);  // Ensure position is within bounds
      servo6.write(position6);
      motor6Y_F6 = false;  // Stop so servo doesn't keep going
      Serial.println(position6); // shows the position on serial mon
  }
  if(motor6Y_R6)
  {
      position6 -= 5.0f;   // Subtract 5 degrees to initial position
      position6 = constrain(position6, 0, 270);  // Ensure position is within bounds
      servo6.write(position6);
      motor6Y_R6 = false;  // Stop so servo doesn't keep going
      Serial.println(position6); // shows the position on serial mon
  }

  
}
