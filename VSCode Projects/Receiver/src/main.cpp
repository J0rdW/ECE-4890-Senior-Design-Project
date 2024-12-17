///////// RX
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RPLidar.h>
#include <Servo.h>

//joystick1 controls two motors (1 & 2)
Servo servo1;
Servo servo2;

//joystick2 controls two motors (3 & 4)
Servo servo3;
Servo servo4;

//joystick3 copntrols two motors (5 & 6)
Servo servo5;
Servo servo6;

RF24 radio(9, 10); // Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; // These are just arbitrary 64-bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; // They must be the same on both ends of the communciations

// MOTOR STUFF
// define codes for actions
#define NO_ACTION 19
#define SPEED1 22
#define SPEED2 23
#define SPEED3 24
#define STOP 20
#define BOTH 3
#define REVERSE 25
#define REVERSE2 26
#define REVERSE3 27
#define R_STOP 28
#define L_STOP 29

// define Motor A connections (pin numbers so repeats are ok)
#define F_PWM1 3
#define R_PWM1 2
#define F_EN1 22
#define R_EN1 24

// define Motor B connections
#define F_PWM2 5
#define R_PWM2 4
#define F_EN2 26
#define R_EN2 28

// define LIDAR input digital pins & variables
#define FRONT 30
#define BACK 31
#define LEFT 32
#define RIGHT 33
int goForward = 0;
int goBack = 0;
int goLeft = 0;
int goRight = 0;

// Motor bools
bool motor_fw = false;
bool motor_bw = false;
bool motor_fw2 = false;
bool motor_bw2 = false;
bool motor_fw3 = false;
bool motor_bw3 = false;
bool motor_stop = false; // both motors stop
bool motor_right_stop = false; // right motor stops
bool motor_left_stop = false; // left motor stops

/************************* SERVO STUFF ********************** */
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
#define SERVO1_PIN 13 //good
#define SERVO2_PIN 8 //good
#define SERVO3_PIN 6 //good
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

float position1=60;  // Starts at 60 degrees
float position2=45;  // Starts at 110 degrees
float position3=80;  // Starts at 120 degrees
float position4=70;  // Starts at 70 degrees
float position5=140;  // Starts at 140 degrees
float position6=90;  // Starts at- 90 degrees

int direction=1;

// Function prototypes
void checkMotor();
void checkLidar();
void checkServo();

void setup() {
  Serial.begin(115200);//Set up comm with the IDE serial monitor
  Serial.println("Ready for commands");
  radio.begin();//Start up the radio object
  radio.setRetries(15,15);//This will improve reliability of the module if it encounters interference
  radio.setPALevel(RF24_PA_LOW);//This sets the power low. This will reduce the range. RF24_PA_MAX would increase the range
  radio.openWritingPipe(send_pipe);//Thses are the reverse of the transmit code.
  radio.openReadingPipe(1,recv_pipe);
  radio.startListening();//Give the module a kick

  // Set the LIDAR input pins
  pinMode(FRONT, INPUT);
  pinMode(BACK, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  //Set all the motor control pins to outputs
  pinMode(F_PWM1, OUTPUT);
  pinMode(R_PWM1, OUTPUT);
  pinMode(F_PWM2, OUTPUT);
  pinMode(R_PWM2, OUTPUT);
  pinMode(F_EN1, OUTPUT);
  pinMode(R_EN1, OUTPUT);
  pinMode(F_EN2, OUTPUT);
  pinMode(R_EN2, OUTPUT);

  // SERVO STUFF
  servo1.attach(SERVO1_PIN);//Spool up the servo -pin3
  servo2.attach(SERVO2_PIN);//Spool up the servo -pin5
  
  servo3.attach(SERVO3_PIN);//Spool up the servo -pin4
  servo4.attach(SERVO4_PIN);//Spool up the servo -pin7
  
  servo5.attach(SERVO5_PIN);//Spool up the servo -pin11
  servo6.attach(SERVO6_PIN);//Spool up the servo -pin12

  servo2.write(position2); // initializing the position for servo 2
  delay(200); 
  servo1.write(position1); // initializing the position for servo 1
  delay(200);
  servo3.write(position3); // initializing the position for servo 3 
  delay(200); 
  servo4.write(position4); // initializing the position for servo 4
  servo5.write(position5); // initializing the position for servo 5
  servo6.write(position6); // initializing the position for servo 6


  delay(5000);
}

unsigned long motor_code = NO_ACTION;
unsigned long servo_code = NO_ACTION;  // Unused, might use later for servos?

void loop() {
  checkServo();
  checkLidar();
  checkMotor();
}

void checkServo(){
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
      position5 = constrain(position5, 105, 180);  // Ensure position is within bounds
      servo5.write(position5);
      motor5X_F5 = false;  // Stop so servo doesn't keep going
      Serial.println(position5); // shows the position on serial mon
  }
  if(motor5X_R5)
  {
      position5 -= 5.0f;   // Subtract 5 degrees to initial position
      position5 = constrain(position5, 105, 180);  // Ensure position is within bounds
      servo5.write(position5);
      motor5X_R5 = false;  // Stop so servo doesn't keep going
      Serial.println(position5); // shows the position on serial mon
  }
  if(motor6Y_F6)
  {
      position6 += 10.0f;   // Subtract 5 degrees to initial position
      position6 = constrain(position6, 0, 90);  // Ensure position is within bounds
      servo6.write(position6);
      motor6Y_F6 = false;  // Stop so servo doesn't keep going
      Serial.println(position6); // shows the position on serial mon
  }
  if(motor6Y_R6)
  {
      position6 -= 10.0f;   // Subtract 5 degrees to initial position
      position6 = constrain(position6, 0, 90);  // Ensure position is within bounds
      servo6.write(position6);
      motor6Y_R6 = false;  // Stop so servo doesn't keep going
      Serial.println(position6); // shows the position on serial mon
  }
}

void checkLidar(){
  goForward = digitalRead(FRONT);
  goBack = digitalRead(BACK);
  goRight = digitalRead(RIGHT);
  goLeft = digitalRead(LEFT);
}

// Handle motor codes
void checkMotor(){
  if( radio.available()) //Keep checking on each loop to see if any data has come in
  {
    radio.read(&motor_code, sizeof(unsigned long)); //Stuff the incoming packet into the motor_code variable
    //Serial.println(motor_code);
    // Check each "motor_code" 
    if((motor_code==R_STOP) && (goLeft == 0)){
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = false;
      motor_bw2 = false;
      motor_fw3 = false;
      motor_bw3 = false;
      motor_stop = false;
      motor_right_stop = true;
      motor_left_stop = false;
    }
    if((motor_code==L_STOP) && (goRight == 0)){
      Serial.println("LEFT Go vrrroomm/RIGHT Go mmmmm");
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = false;
      motor_bw2 = false;
      motor_bw3 = false;
      motor_fw3 = false;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = true;
    }
    if((motor_code==SPEED1) && (goForward == 0))
    {
      Serial.println("SPEED1");
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = true;
      motor_bw2 = false;
      motor_fw3 = false;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
    if((motor_code==SPEED2) && (goForward == 0))
    {
      Serial.println("SPEED2");
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = true;
      motor_bw2 = false;
      motor_fw3 = false;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
    if((motor_code==SPEED3) && (goForward == 0))
    {
      Serial.println("SPEED3");
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = false;
      motor_bw2 = false;
      motor_fw3 = true;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
    else if(motor_code==STOP)
    {
      Serial.println("THE MOTOR HAS STOPPED");
      motor_fw=false;
      motor_bw=false;
      motor_fw2 = false;
      motor_bw2 = false;
      motor_bw3 = false;
      motor_fw3 = false;
      motor_stop = true;
      motor_right_stop = false;
      motor_left_stop = false;
    }

 
    else if((motor_code==REVERSE) && (goBack == 0))
    {
      Serial.println("In reverse");
      motor_bw=true;
      motor_fw=false;
      motor_fw2=false;
      motor_bw2=false;
      motor_fw3=false;
      motor_bw3=false;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
    else if((motor_code==REVERSE2) && (goBack == 0))
    {
      Serial.println("In reverse2");
      motor_bw=false;
      motor_fw=false;
      motor_fw2=false;
      motor_bw2=true;
      motor_fw3=false;
      motor_bw3=false;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
    else if((motor_code==REVERSE3) && (goBack == 0))
    {
      Serial.println("In reverse3");
      motor_bw=false;
      motor_fw=false;
      motor_fw2=false;
      motor_bw2=false;
      motor_fw3=false;
      motor_bw3=true;
      motor_stop = false;
      motor_right_stop = false;
      motor_left_stop = false;
    }
  }
  // Turns left by turning left motor
  if(motor_right_stop){
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(F_PWM1, 0);
    analogWrite(R_PWM1, 255);
    analogWrite(F_PWM2, 0);
    analogWrite(R_PWM2, 255);
    // Turn on motor A & B
    digitalWrite(F_EN1, HIGH);
    digitalWrite(R_EN1, HIGH);
    digitalWrite(F_EN2, HIGH);
    digitalWrite(R_EN2, HIGH);
    delay(100);
  }
  if(motor_left_stop){
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(F_PWM1, 255);
    analogWrite(R_PWM1, 0);
    analogWrite(F_PWM2, 255);
    analogWrite(R_PWM2, 0);
    // Turn on motor A & B
    digitalWrite(F_EN1, HIGH);
    digitalWrite(R_EN1, HIGH);
    digitalWrite(F_EN2, HIGH);
    digitalWrite(R_EN2, HIGH);
    delay(100);
  }
  ////////// FORWARD
  if(motor_fw3)
  {
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(F_PWM1, 255);
    analogWrite(R_PWM1, 0);
    analogWrite(F_PWM2, 0);
    analogWrite(R_PWM2, 255);
    // Turn on motor A & B
    digitalWrite(F_EN1, HIGH);
    digitalWrite(R_EN1, HIGH);
    digitalWrite(F_EN2, HIGH);
    digitalWrite(R_EN2, HIGH);
    delay(100);
  }
  /////////// REVERSE
  if(motor_bw3)
  {
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(F_PWM1, 0);
    analogWrite(R_PWM1, 255);
    analogWrite(F_PWM2, 255);
    analogWrite(R_PWM2, 0);
    // Turn on motor A & B
    digitalWrite(F_EN1, HIGH);
    digitalWrite(R_EN1, HIGH);

    digitalWrite(F_EN2, HIGH);
    digitalWrite(R_EN2, HIGH);
    delay(100);
  }
  ////// Stop both motors
  if(motor_stop){
    analogWrite(F_PWM1, 0);
    analogWrite(R_PWM1, 0);
 
    analogWrite(F_PWM2, 0);
    analogWrite(R_PWM2, 0);
    // Turn on motor A & B
    digitalWrite(F_EN1, LOW);
    digitalWrite(R_EN1, LOW);
 
    digitalWrite(F_EN2, LOW);
    digitalWrite(R_EN2, LOW);
    //digitalWrite(in3, LOW);
    //digitalWrite(in4, LOW);
    delay(100);
  }
}

/*
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RPLidar.h>

RF24 radio(9, 10); // Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; // These are just arbitrary 64-bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; // They must be the same on both ends of the communciations

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor (MOTOCTRL).
 
float minDistance = 100000;
float angleAtMinDist = 0;
int count = 0;
 
#define NO_ACTION 0
#define STOP 2
#define BOTH 3
 
// So far just moves at 1 speed
#define SPEED3 12     // Forward
#define REVERSE3 15   // Backward
 
#define R_STOP 16 // Turns left
#define L_STOP 17 // Turns right

// LIDAR send codes
#define R1_ON 30
#define R1_OFF 31
#define R2_ON 32
#define R2_OFF 33
#define B1_ON 34
#define B1_OFF 35
#define B2_ON 36
#define B2_OFF 37
#define G1_ON 38
#define G1_OFF 39
#define G2_ON 40
#define G2_OFF 41

unsigned long controller_code = NO_ACTION;
 
RPLidar lidar;
void lidarStart();
void lidarStop();

void processData(float angle, float distance);
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  // For RPLidar
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);  // set pin modes

  // RF stuff
  radio.begin();//Start up the radio object
  radio.setRetries(15,15);//This will improve reliability of the module if it encounters interference
  radio.setPALevel(RF24_PA_LOW);//This sets the power low. This will reduce the range. RF24_PA_MAX would increase the range
  radio.openWritingPipe(send_pipe);//Thses are the reverse of the transmit code.
  radio.openReadingPipe(1,recv_pipe);
  radio.startListening();//Give the module a kick
}
 
void loop() {
  if(count < 200){
    lidarStart();
  } else{
    lidarStop();
  }
}
 
void processData(float angle, float distance)
{
  distance = distance/25.4;
  Serial.print("dist: ");
  Serial.print(distance);
  Serial.print("    angle: ");
  Serial.println(angle);
  if((angle >= 180) && (angle < 240)){
    controller_code = R1_OFF;
  }
  if((angle >= 240) && (angle < 300)){
    controller_code = R2_OFF;
  }
  if((angle >= 300) && (angle < 360)){
    controller_code = B1_OFF;
  }
  if((angle >= 0) && (angle < 60)){
    controller_code = B2_OFF;
  }
  if((angle >= 60) && (angle < 120)){
    controller_code = G1_OFF;
  }
  if((angle >= 120) && (angle < 180)){
    controller_code = G2_OFF;
  }
  // Finally, receiver -> transmitter for controller to see LEDs
  radio.stopListening();  // Make sure it becomes a transmitter
  radio.write(&controller_code, sizeof(unsigned long));
  delay(10);
}
 
void lidarStart(){
    if (IS_OK(lidar.waitPoint())) {
    //perform data processing here:
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;  // 0-360 deg
    if (lidar.getCurrentPoint().startBit) {
       // a new scan, display the previous data...
       processData(angleAtMinDist, minDistance);
       minDistance = 100000;
       angleAtMinDist = 0;
    } else {
       if ( distance > 0 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
       }
    }
  }
  else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    // Try to detect RPLIDAR
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // Detected
       lidar.startScan();
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}
 
*/