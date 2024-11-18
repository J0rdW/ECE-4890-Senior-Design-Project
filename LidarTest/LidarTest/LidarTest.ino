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

#define NO_ACTION 0
#define STOP 2
#define BOTH 3

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
    if(distance > 0 && (angle >= 175 && angle <= 185)) {
      Serial.println("Angle: ");
      Serial.println(angle);
      Serial.println("Distance: ");
      Serial.println(distance);
      delay(100);
      count++;
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

// Splitting up angles into different sections
int angleSelection(float angle){
  int section;
  
  if((angle > 0) && (angle < 45)){
    section = 1;
  }
  if((angle > 45) && (angle < 90)){
    section = 2;
  }
  if((angle > 90) && (angle < 135)){
    section = 3;
  }
  if((angle > 135) && (angle < 180)){
    section = 4;
  }
  if((angle > 180) && (angle < 225)){
    section = 5;
  }
  if((angle > 225) && (angle < 270)){
    section = 6;
  }
  if((angle > 270) && (angle < 315)){
    section = 7;
  }
  if((angle > 315) && (angle < 360)){
    section = 8;
  }
  return section;
}
