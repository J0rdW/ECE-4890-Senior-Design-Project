#include <Servo.h>
//https://www.instructables.com/DIY-Robotic-Arm/

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int VRX_1 = A0; // Joystick 1
int VRY_1 = A1;
int VRX_2 = A2; // Joystick 2
int VRY_2 = A3;

int X1_Pos;
int Y1_Pos;
int X2_Pos;
int Y2_Pos;

int angle1 = 90;
int angle2 = 90;
int angle3 = 90;
int angle4 = 90;

//int potpin4 = A4;
//int potpin5 = A5;
//int val;

void setup()
{
  Serial.begin(115200);
  myservo1.attach(3);
  myservo2.attach(5);
  myservo3.attach(9);
  myservo4.attach(10);
  myservo5.attach(11);
  myservo6.attach(6);

  // initialize servo angle
  myservo1.write(angle1);
  myservo2.write(angle2);
  myservo3.write(angle3);
  myservo4.write(angle4);
}

void loop()
{
  // SERVO 1
  X1_Pos = analogRead(VRX_1);
  X1_Pos = map(X1_Pos, 0, 1023, 0, 180);
  
  if(X1_Pos > 95){
    angle1 += 3.0f; // Add some degrees to the initial position
  } else if(X1_Pos < 85){
    angle1 -= 3.0f; // Go other direction from initial position
  }
  
  if(angle1 > 1 && angle1 < 265){ // Limit the angle so it doesn't go crazy
    myservo1.write(angle1);
  }
  delay(15);

  // SERVO 2
  Y1_Pos = analogRead(VRY_1);
  Y1_Pos = map(Y1_Pos, 0, 1023, 0, 180);
  if(Y1_Pos > 95){
    angle2 += 3.0f; // Add some degrees to the initial position
  } else if(Y1_Pos < 85){
    angle2 -= 3.0f; // Go other direction from initial position
  }

  if(angle2 > 1 && angle2 < 265){ // Limit the angle so it doesn't go crazy
    myservo1.write(angle2);
  }
  delay(15);

  // SERVO 3
  X2_Pos = analogRead(VRX_2);
  X2_Pos = map(X2_Pos, 0, 1023, 0, 180);
  if(X2_Pos > 95){
    angle3 += 3.0f; // Add some degrees to the initial position
  } else if(X2_Pos < 85){
    angle3 -= 3.0f; // Go other direction from initial position
  }
  
  if(angle3 > 1 && angle3 < 265){ // Limit the angle so it doesn't go crazy
    myservo1.write(angle3);
  }
  delay(15);

  // SERVO 4
  Y2_Pos = analogRead(VRY_2);
  Y2_Pos = map(Y2_Pos, 0, 1023, 0, 180);
  if(Y2_Pos > 95){
    angle4 += 3.0f; // Add some degrees to the initial position
  } else if(Y2_Pos < 85){
    angle4 -= 3.0f; // Go other direction from initial position
  }
    
  if(angle4 > 1 && angle4 < 265){ // Limit the angle so it doesn't go crazy
    myservo1.write(angle4);
  }
  delay(15);

//  val = analogRead(potpin4);
//  val = map(val, 0, 1023, 0, 250);
//  myservo5.write(val);
//  delay(15);
//
//  val = analogRead(potpin5);
//  val = map(val, 0, 1023, 0, 250);
//  myservo6.write(val);
//  delay(15);
}
