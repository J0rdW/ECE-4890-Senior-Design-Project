#include <RPLidar.h>
#define RPLIDAR_MOTOR 3 // The PWM pin for controlling the speed of RPLIDAR's motor (MOTOCTRL).

RPLidar lidar;
float distances[360]; // Array to store distances for each angle from 0 to 359 degrees

const int THRESHOLD = 0; // Minimum number of non-zero values needed to print

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);  // For RPLidar
  lidar.begin(Serial1);
  pinMode(RPLIDAR_MOTOR, OUTPUT);  // Set pin mode
  analogWrite(RPLIDAR_MOTOR, 255); // Start the RPLidar motor
  lidar.startScan();
}

void loop() {
  lidarStart();
}

// Function to print all distances once the threshold is met
void printAllDistances() {
  for (int i = 0; i < 360; i++) {
    Serial.print("Angle: ");
    Serial.print(i);
    Serial.print("    Distance: ");
    Serial.println(distances[i]);
  }
  Serial.println(); // Separate scans with a blank line
}

void lidarStart() {
  if (IS_OK(lidar.waitPoint())) { // Wait for 1 sample
    // Get the current distance and angle
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;

    // New scan
    if (lidar.getCurrentPoint().startBit) {
      int nonZeroCount = 0;
      
      // Count non-zero values in the distances array
      for (int i = 0; i < 360; i++) {
        if (distances[i] > 1) {
          nonZeroCount++;
        }
      }

      // Only print if the threshold is met
      if (nonZeroCount >= THRESHOLD) {
        printAllDistances();  // Print distances if enough non-zero values are present
        Serial.println("Count: ");
        Serial.println(nonZeroCount);
        delay(500);
      }

//      memset(distances, 0, sizeof(distances)); // Reset distances array for the next scan
    }

    // Store the distance in the appropriate angle slot (0 to 359 degrees)
    if (angle >= 0 && angle < 360) {
      int index = (int)angle;  // Convert angle to an integer index
      distances[index] = distance; // Update the distance for this angle
    }
  } 
  else {
    analogWrite(RPLIDAR_MOTOR, 0); // Stop the RPLidar motor if not responding

    // Try to detect and restart the RPLidar
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
}

void lidarStop() {
  analogWrite(RPLIDAR_MOTOR, 0);
}
