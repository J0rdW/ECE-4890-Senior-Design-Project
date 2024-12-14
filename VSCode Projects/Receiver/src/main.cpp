///////// RX
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RPLidar.h>
RF24 radio(9, 10); // Create a commuications object for talking to the NRF24L01
const uint64_t send_pipe=0xB01DFACECEL; // These are just arbitrary 64-bit numbers to use as pipe identifiers
const uint64_t recv_pipe=0xDEADBEEFF1L; // They must be the same on both ends of the communciations

// define codes for actions
#define NO_ACTION 0
#define SPEED1 10
#define SPEED2 11
#define SPEED3 12
#define STOP 2
#define BOTH 3
#define REVERSE 13
#define REVERSE2 14
#define REVERSE3 15
#define RIGHTMOTORSTOP 16
#define LEFTMOTORSTOP 17

// define Motor A connections
#define F_PWM1 3
#define R_PWM1 2
 
#define F_EN1 22
#define R_EN1 24
// define Motor B connections
#define F_PWM2 5
#define R_PWM2 4
 
#define F_EN2 26
#define R_EN2 28

// LIDAR
#define RPLIDAR_MOTOR 6

// LIDAR send codes (if wanted)
#define G2_ON 30
#define G2_OFF 31
#define G3_ON 32
#define G3_OFF 33
#define R1_ON 34
#define R1_OFF 35
#define R2_ON 36
#define R2_OFF 37
#define R3_ON 38
#define R3_OFF 39
#define B1_ON 40
#define B1_OFF 41
#define B2_ON 42
#define B2_OFF 43
#define G1_ON 44
#define G1_OFF 45

 
bool motor_fw = false;
bool motor_bw = false;
bool motor_fw2 = false;
bool motor_bw2 = false;
bool motor_fw3 = false;
bool motor_bw3 = false;
bool motor_stop = false; // both motors stop
bool motor_right_stop = false; // right motor stops
bool motor_left_stop = false; // left motor stops

// Lidar variables
float angleAtMinDist = 0;
int count = 0;
float angle = 0;
int lidarSection;
int section;
int displayDistance = 1;
int displayAngle = 1;
float inches;
int maxDistance = 0;

int thresh = 24;  // Threshold in inches for when the LED should light up.
int maxFluctuation = 2; // The maximum fluctuation before being able to light an LED.

#define samples 10
float g2Buffer[samples] = {0};
float g3Buffer[samples] = {0};
float r1Buffer[samples] = {0};
float r2Buffer[samples] = {0};
float r3Buffer[samples] = {0};
float b1Buffer[samples] = {0};
float b2Buffer[samples] = {0};
float g1Buffer[samples] = {0};
int bufferIndex = 0;
float minDistance = 0;
int currentRangeStart = -1;

RPLidar lidar;

void checkMotor();  // Check motor codes
void processLidar();  // Lidar code that contains the below functions
void printData();
void lidarStart();
void lidarStop();
void setRangeDist(float angle, float distance);


void setup() {
  Serial.begin(115200);//Set up comm with the IDE serial monitor
  Serial1.begin(115200);
  //Serial.println("Ready for commands");
  radio.begin();//Start up the radio object
  radio.setRetries(15,15);//This will improve reliability of the module if it encounters interference
  radio.setPALevel(RF24_PA_LOW);//This sets the power low. This will reduce the range. RF24_PA_MAX would increase the range
  radio.openWritingPipe(send_pipe);//Thses are the reverse of the transmit code.
  radio.openReadingPipe(1,recv_pipe);
  radio.startListening();//Give the module a kick
  //Set all the motor control pins to outputs
  pinMode(F_PWM1, OUTPUT);
  pinMode(R_PWM1, OUTPUT);
  pinMode(F_PWM2, OUTPUT);
  pinMode(R_PWM2, OUTPUT);
  pinMode(F_EN1, OUTPUT);
  pinMode(R_EN1, OUTPUT);
  pinMode(F_EN2, OUTPUT);
  pinMode(R_EN2, OUTPUT);
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}

unsigned long motor_code=NO_ACTION;
unsigned long send_code=NO_ACTION;

void loop() {
  checkMotor();
  processLidar();
}

void processLidar(){
    // Wait for 1 sample
  if (IS_OK(lidar.waitPoint())) { 
    //perform data processing here:
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;  // 0-360 deg
    if(distance < 3000){
      setRangeDist(angle, distance);
    }
  }
  // Otherwise stop the LIDAR and retry
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

float toInches(float d){
  float distanceInches = d/25.4;
  return distanceInches;
}

void setRangeDist(float a, float d){
   // If reading is available (doesn't have 0 distance) then convert to inches and process the data
  if(d != 0){
    inches = toInches(d);
  }
  // If in 45-90 angle section, means it will be the 45 degree section (see updateLED function)
  int rangeStart = (int(a) / 45)*45;

  // If it's finally out of the range, process the minimum distance lidar saw
  if(rangeStart != currentRangeStart){
    // >= 0 to make sure lidar is reading >=0 angles and proper data
    if(currentRangeStart >= 0){
      updateLED(currentRangeStart, maxDistance, minDistance, thresh, maxFluctuation);
    }
    // Reset for the next range...
    currentRangeStart = rangeStart;
    minDistance = 10000;
    maxDistance = 0;
  }

  // Starts determining the minimum distance it saw within the current range
  if(inches < minDistance){
    minDistance = inches;
  }
  if(inches > maxDistance){
    maxDistance = inches;
  }
}

// If the max distance is higher than thresh, write HIGH to proper LED
void updateLED(int rangeStart, float maxDistance, float minDistance, int thresh, int maxFluctation){
  int variation = maxDistance - minDistance;
  
  // Check the range
  if((rangeStart == 180) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = G2_ON;
    } else{
      send_code = G2_OFF;
    }
  }
  if((rangeStart == 225) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = G3_ON;
    } else{
      send_code = G3_OFF;
    }
  }
  if((rangeStart == 270) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = R1_ON;
    } else{
      send_code = R1_OFF;
    }
  }
  if((rangeStart == 315) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = R2_ON;
    } else{
      send_code = R2_OFF;
    }
  }
  if((rangeStart == 0) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = R3_ON;
    } else{
      send_code = R3_OFF;
    }
  }
  if((rangeStart == 45) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = B1_ON;
    } else{
      send_code = B1_OFF;
    }
  }
  if((rangeStart == 90) && (variation < maxFluctation)){
    if(minDistance > thresh){
      send_code = B2_ON;
    } else{
      send_code = B2_OFF;
    }
  }
  if((rangeStart == 135) && (variation < maxFluctation)){
    Serial.println(minDistance);
    if(minDistance > thresh){
      send_code = G1_ON;
    } else{
      send_code = G1_OFF;
    }
  }
}

// Handle motor codes
void checkMotor(){
  if( radio.available()) //Keep checking on each loop to see if any data has come in
  {
      radio.read(&motor_code, sizeof(unsigned long)); //Stuff the incoming packet into the motor_code variable
      //Serial.println(motor_code);
    // Check each "motor_code" 
    if(motor_code==RIGHTMOTORSTOP){
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
    if(motor_code==LEFTMOTORSTOP){
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
    if(motor_code==SPEED1)
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
    if(motor_code==SPEED2)
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
    if(motor_code==SPEED3)
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

 
    else if(motor_code==REVERSE)
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
    else if(motor_code==REVERSE2)
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
    else if(motor_code==REVERSE3)
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