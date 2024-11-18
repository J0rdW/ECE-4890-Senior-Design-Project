///////// RX
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
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
 
 
bool motor_fw = false;
bool motor_bw = false;
bool motor_fw2 = false;
bool motor_bw2 = false;
bool motor_fw3 = false;
bool motor_bw3 = false;
bool motor_stop = false; // both motors stop
bool motor_right_stop = false; // right motor stops
bool motor_left_stop = false; // left motor stops
void setup() {
  Serial.begin(9600);//Set up comm with the IDE serial monitor
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
}
unsigned long motor_code=NO_ACTION;
void loop()
{
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
//  if(motor_fw)
//  {
//      // Set motors to maximum speed
//      // For PWM maximum possible values are 0 to 255
//      analogWrite(F_PWM1, 100);
//      analogWrite(R_PWM1, 100);
//      // Turn on motor A & B
//      digitalWrite(F_EN1, HIGH);
//      digitalWrite(R_EN1, LOW);
//      delay(100);
////      motor_fw = false;  // Stop so servo doesn't keep going
//  }
//  if(motor_fw2)
//  {
//      // Set motors to maximum speed
//      // For PWM maximum possible values are 0 to 255
//      analogWrite(F_PWM1, 170);
//      analogWrite(R_PWM1, 170);
//      // Turn on motor A & B
//      digitalWrite(F_EN1, HIGH);
//      digitalWrite(R_EN1, LOW);
//      delay(100);
//  }
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
//  if(motor_bw)
//  {
//      // Set motors to maximum speed
//      // For PWM maximum possible values are 0 to 255
//      analogWrite(F_PWM1, 100);
//      analogWrite(R_PWM1, 100);
//      // Turn on motor A & B
//      digitalWrite(F_EN1, LOW);
//      digitalWrite(R_EN1, HIGH);
//      delay(100);
//  }
//  if(motor_bw2)
//  {
//      // Set motors to maximum speed
//      // For PWM maximum possible values are 0 to 255
//      analogWrite(F_PWM1, 170);
//      analogWrite(R_PWM1, 170);
//      // Turn on motor A & B
//      digitalWrite(F_EN1, LOW);
//      digitalWrite(R_EN1, HIGH);
//      delay(100);
//  }
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
