///////// RX
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RPLidar.h>
RF24 radio(9, 10); // Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; // These are just arbitrary 64-bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; // They must be the same on both ends of the communciations

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

// Function prototypes
void checkMotor();
void checkLidar();

void setup() {
  Serial.begin(115200);//Set up comm with the IDE serial monitor
  //Serial.println("Ready for commands");
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
}

unsigned long motor_code = NO_ACTION;
unsigned long servo_code = NO_ACTION;  // Unused, might use later for servos?

void loop() {
  checkLidar();
  checkMotor();
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