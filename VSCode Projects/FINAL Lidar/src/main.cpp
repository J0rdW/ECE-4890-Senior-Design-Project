///////// RX
#include <SPI.h>
#include <RPLidar.h>

// LIDAR
#define RPLIDAR_MOTOR 3

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

int thresh = 50;  // Threshold in inches for when the LED should light up.
int maxFluctuation = 1; // The maximum fluctuation in inches before being able to light an LED.

float minDistance = 0;
int currentRangeStart = -1;

RPLidar lidar;

void processLidar();  // Lidar code that contains the below functions
void processData(float angle, float distance);

// LIDAR output digital pins for motor arduino
#define FRONT 5
#define BACK 6
#define LEFT 7
#define RIGHT 8

void setup() {
  
  Serial.begin(115200);//Set up comm with the IDE serial monitor
  Serial1.begin(115200);
  lidar.begin(Serial1);

  // Pins to output to the motor arduino
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  pinMode(FRONT, OUTPUT);
  pinMode(BACK, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
}

void loop() {
  processLidar();
}

void processLidar(){
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
       if ( distance > 300 &&  distance < minDistance) {
          minDistance = distance;
          angleAtMinDist = angle;
       }
      }
  }else {
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

void processData(float angle, float distance)
{
  Serial.print("dist: ");
  Serial.print(distance);
  Serial.print("    angle: ");
  Serial.println(angle);
  if((angle <= 225) && (angle >= 150) && (angle < 210)){
    digitalWrite(FRONT, HIGH);
    digitalWrite(BACK, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(RIGHT, LOW);
    Serial.println("can't go forward");
  }
  if((angle >= 240) && (angle < 300)){
    digitalWrite(BACK, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(FRONT, LOW);
    digitalWrite(RIGHT, HIGH);
    Serial.println("can't go right");
  }
  if((angle >= 330) || (angle < 30)){
    digitalWrite(BACK, HIGH);
    digitalWrite(LEFT, LOW);
    digitalWrite(FRONT, LOW);
    digitalWrite(RIGHT, LOW);
    Serial.println("can't go back");
  }
  if((angle >= 60) && (angle < 120)){
    digitalWrite(LEFT, HIGH);
    digitalWrite(FRONT, LOW);
    digitalWrite(RIGHT, LOW);
    digitalWrite(BACK, LOW);
    Serial.print("can't go left");
  }
}
