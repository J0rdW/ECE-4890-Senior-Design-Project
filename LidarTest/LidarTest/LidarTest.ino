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
float maxDistance = 0;
int currentRangeStart = -1;

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

void lidarStop(){
  analogWrite(RPLIDAR_MOTOR, 0);
}
float toInches(float d){
  float distanceInches = d/25.4;
  return distanceInches;
}

// Sets the range to see
void setRangeDist(float a, float d){
    int thresh = 12;
    inches = toInches(d);
    // If in 45-90 angle section, means it will be the 45 degree section (see updateLED function)
    int rangeStart = (int(a) / 45)*45;

    // If it's finally out of the range, process the maximum distance lidar saw
    if(rangeStart != currentRangeStart){
      // >= 0 to make sure lidar is reading >=0 angles and proper data
      if(currentRangeStart >= 0){
        updateLED(currentRangeStart, maxDistance, thresh);
      }
      // Reset for the next range...
      currentRangeStart = rangeStart;
      maxDistance = 0;
    }

    // Starts determining the maximum distance it saw within the current range
    if(inches > maxDistance){
      maxDistance = inches;
    }
}

// If the max distance is higher than thresh, write HIGH to proper LED
void updateLED(int rangeStart, float maxDistance, int thresh){
  // Check the range
  if(rangeStart == 180){
    if(maxDistance > thresh){
      digitalWrite(g2, HIGH);
    } else{
      digitalWrite(g2, LOW);
    }
  }
  if(rangeStart == 225){
    if(maxDistance > thresh){
      digitalWrite(g3, HIGH);
    } else{
      digitalWrite(g3, LOW);
    }
  }
  if(rangeStart == 270){
    if(maxDistance > thresh){
      digitalWrite(r1, HIGH);
    } else{
      digitalWrite(r1, LOW);
    }
  }
  if(rangeStart == 315){
    if(maxDistance > thresh){
      digitalWrite(r2, HIGH);
    } else{
      digitalWrite(r2, LOW);
    }
  }
  if(rangeStart == 0){
    if(maxDistance > thresh){
      digitalWrite(r3, HIGH);
    } else{
      digitalWrite(r3, LOW);
    }
  }
  if(rangeStart == 45){
    if(maxDistance > thresh){
      digitalWrite(b1, HIGH);
    } else{
      digitalWrite(b1, LOW);
    }
  }
  if(rangeStart == 90){
    if(maxDistance > thresh){
      digitalWrite(b2, HIGH);
    } else{
      digitalWrite(b2, LOW);
    }
  }
  if(rangeStart == 135){
    if(maxDistance > thresh){
      digitalWrite(g1, HIGH);
    } else{
      digitalWrite(g1, LOW);
    }
  }
}

//// Update corresponding buffers
//void updateBuffer(float buffer[], float inches){
//  buffer[bufferIndex % samples] = inches;
//  bufferIndex++;
//}
//
//float getAverage(float buffer[]){
//  float sum = 0;
//  for(int i=0; i < samples; i++){
//    sum += buffer[i];
//  }
//  return sum/samples;
//}

//   if(a >= 180 && a <= 225) {
//      updateBuffer(g2Buffer, inches);
//      if(getAverage(g2Buffer) > thresh){
//        digitalWrite(g2, HIGH);
//      } else{
//        digitalWrite(g2, LOW);
//      }
//    }
//    if(a >= 225 && a <= 270) {
//      updateBuffer(g3Buffer, inches);
//      if(getAverage(g3Buffer) > thresh){
//        digitalWrite(g3, HIGH);
//      } else{
//        digitalWrite(g3, LOW);
//      }
//    }
//    if(a >= 270 && a <= 315) {
//      updateBuffer(r1Buffer, inches);
//      if(getAverage(r1Buffer) > thresh){
//        digitalWrite(r1, HIGH);
//      } else{
//        digitalWrite(r1, LOW);
//      }
//    }
//    if(a >= 315 && a <= 360) {
//      updateBuffer(r2Buffer, inches);
//      if(getAverage(r2Buffer) > thresh){
//        digitalWrite(r2, HIGH);
//      } else{
//        digitalWrite(r2, LOW);
//      }
//    }
//    // Crosses to 0 and goes from 0 to 180
//    if(a >= 0 && a <= 45) {
//      updateBuffer(r3Buffer, inches);
//      if(getAverage(r3Buffer) > thresh){
//        digitalWrite(r3, HIGH);
//      } else{
//        digitalWrite(r3, LOW);
//      }
//    }
//    if(a >= 45 && a <= 90) {
//      updateBuffer(b1Buffer, inches);
//      if(getAverage(b1Buffer) > thresh){
//        digitalWrite(b1, HIGH);
//      } else{
//        digitalWrite(b1, LOW);
//      }
//    }
//    if(a >= 90 && a <= 135) {
//      updateBuffer(b2Buffer, inches);
//      if(getAverage(b2Buffer) > thresh){
//        digitalWrite(b2, HIGH);
//      } else{
//        digitalWrite(b2, LOW);
//      }
//    }
//    if(a >= 135 && a <= 179) {
//      updateBuffer(g1Buffer, inches);
//      if(getAverage(g1Buffer) > thresh){
//        digitalWrite(g1, HIGH);
//      } else{
//        digitalWrite(g1, LOW);
//      }
//    }

//      Serial.println("Angle: ");
//      Serial.println(a);
//      Serial.println("Distance: ");
//      Serial.println(d);
