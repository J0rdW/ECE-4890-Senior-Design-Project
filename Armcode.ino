#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int potpin = A0;
int potpin1 = A1;
int potpin2 = A2;
int potpin3 = A3;
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
  val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 250);
  myservo1.write(val);
  delay(15);

  val = analogRead(potpin1);
  val = map(val, 0, 1023, 0, 250);
  myservo2.write(val);
  delay(15);

  val = analogRead(potpin2);
  val = map(val, 0, 1023, 0, 250);
  myservo3.write(val);
  delay(15);

  val = analogRead(potpin3);
  val = map(val, 0, 1023, 0, 250);
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
