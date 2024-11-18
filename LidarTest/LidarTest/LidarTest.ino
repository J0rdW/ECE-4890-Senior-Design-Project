/* This code takes the closest object and prints out the angle of where it is at */

#include <RPLidar.h>
#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor (MOTOCTRL).

float minDistance = 100000;
float angleAtMinDist = 0;
int count = 0;
float angle = 0;
int lidarSection;
int section;
int displayDistance = 1;
int displayAngle = 1;
float inches;

#define NO_ACTION 0
#define STOP 2
#define BOTH 3

// LED pins for testing...
#define g1 4
#define g2 5
#define g3 6
#define r1 7
#define r2 8
#define r3 9
#define b1 10
#define b2 11

#define samples 10;
float g2Buffer[samples] = {0};
float g3Buffer[samples] = {0};
float r1Buffer[samples] = {0};
float r2Buffer[samples] = {0};
float r3Buffer[samples] = {0};
float b1Buffer[samples] = {0};
float b2Buffer[samples] = {0};
float g1Buffer[samples] = {0};
int bufferIndex = 0;

// So far just moves at 1 speed
#define SPEED3 12     // Forward
#define REVERSE3 15   // Backward

#define R_STOP 16 // Turns left
#define L_STOP 17 // Turns right

RPLidar lidar;
                      
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  // For RPLidar
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);  // set pin modes
}

// Main loop
void loop() {
  if(count < 200){
    lidarStart();
  } else{
    lidarStop();
  }
}

// Print the angle and distance away 
void printData(float angle, float distance, int section)
{
  // Count++ just so it doesn't run all the time and only 200 reads and stops
  count++;
  
  Serial.print("dist: ");
  Serial.print(distance);
  Serial.print("    angle: ");
  Serial.println(angle);
  Serial.print("    section: ");
  Serial.println(section);
}

void lidarStart(){
  
  // Wait for 1 sample
  if (IS_OK(lidar.waitPoint())) { 
    //perform data processing here:
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;  // 0-360 deg
    if(distance < 3000){
      handleLED(angle, distance);
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

void lidarStop(){
  analogWrite(RPLIDAR_MOTOR, 0);
}
float toInches(float d){
  float distanceInches = d/25.4;
  return distanceInches;
}

void handleLED(float a, float d){
    int thresh = 12;
    inches = toInches(d);
    if(inches > thresh && (a >= 180 && a <= 225)) {
      digitalWrite(g2, HIGH);
    } else if(inches < thresh && (a >= 180 && a <= 225)){
      digitalWrite(g2, LOW);
    }
    if(inches > thresh && (a >= 225 && a <= 270)) {
      digitalWrite(g3, HIGH);
    } else if(inches < thresh && (a >= 225 && a <= 270)){
      digitalWrite(g3, LOW);
    }
    if(inches > thresh && (a >= 270 && a <= 315)) {
      digitalWrite(r1, HIGH);
      Serial.println("I AM ON");
    } else if(inches < thresh && (a >= 270 && a <= 315)){
      digitalWrite(r1, LOW);
    }
    if(inches > thresh && (a >= 315 && a <= 360)) {
      digitalWrite(r2, HIGH);
    } else if(inches < thresh && (a >= 315 && a <= 360)){
      digitalWrite(r2, LOW);
    }
    // Crosses to 0 and goes from 0 to 180
    if(inches > thresh && (a >= 0 && a <= 45)) {
      digitalWrite(r3, HIGH);
    } else if(inches < thresh && (a >= 0 && a <= 45)){
      digitalWrite(r3, LOW);
    }
    if(inches > thresh && (a >= 45 && a <= 90)) {
      digitalWrite(b1, HIGH);
    } else if(inches < thresh && (a >= 45 && a <= 90)){
      digitalWrite(b1, LOW);
    }
    if(inches > thresh && (a >= 90 && a <= 135)) {
      digitalWrite(b2, HIGH);
    } else if(inches < thresh && (a >= 90 && a <= 135)){
      digitalWrite(b2, LOW);
    }
    if(inches > thresh && (a >= 135 && a <= 179)) {
      digitalWrite(g1, HIGH);
    } else if(inches < thresh && (a >= 135 && a <= 179)){
      digitalWrite(g1, LOW);
    }
}

// Update the 
void updateBuffer(float buffer[], float value){
  buffer[bufferIndex % samples] = value;
  bufferIndex++;
}

//      Serial.println("Angle: ");
//      Serial.println(a);
//      Serial.println("Distance: ");
//      Serial.println(d);
