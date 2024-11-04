#include <Servo.h>
// written by: Ahmad Saeed
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

int potpin4 = A4;
int potpin5 = A5;
int val;

void setup()
{
  myservo1.attach(3);
  myservo2.attach(5);
  myservo3.attach(9);
  myservo4.attach(10);
  myservo5.attach(11);
  myservo6.attach(6);
}

void loop()
{
  val = analogRead(VRX_1);
  val = map(val, 0, 1023, 0, 180);
  myservo1.write(val);
  delay(15);

  val = analogRead(VRY_1);
  val = map(val, 0, 1023, 0, 180);
  myservo2.write(val);
  delay(15);

  val = analogRead(VRX_2);
  val = map(val, 0, 1023, 0, 180);
  myservo3.write(val);
  delay(15);

  val = analogRead(VRY_2);
  val = map(val, 0, 1023, 0, 180);
  myservo4.write(val);
  delay(15);

  val = analogRead(potpin4);
  val = map(val, 0, 1023, 0, 250);
  myservo5.write(val);
  delay(15);

  val = analogRead(potpin5);
  val = map(val, 0, 1023, 0, 250);
  myservo6.write(val);
  delay(15);
}
