#include "I2Cdev.h"

#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>


const float RADIANS_TO_DEG = 180.0 / M_PI;
const float DEG_TO_RADIANS = M_PI / 180.0;


MPU6050 mpu;

bool dmpReady = false;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Initializing MPU6050");
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Connection failed!");
    while (1);
  }

  Serial.println("Connection established!");

  devStatus = mpu.dmpInitialize();

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
    dmpReady = true;
    Serial.println("DMP ready");
  } else {
    Serial.println("DMP failed");
    while (1);
  }
}

void loop() {
  if (!dmpReady) return;

  fifoCount = mpu.getFIFOCount();

  // overflow protection
  if (fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println("FIFO overflow!");
    return;
  }

  if (fifoCount < packetSize) return;

  // drain FIFO properly
  while (fifoCount >= packetSize) {
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
  }

  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  Serial.print("Yaw: ");
  Serial.print(ypr[0] * RADIANS_TO_DEG);
  Serial.print("  Pitch: ");
  Serial.print(ypr[1] * RADIANS_TO_DEG);
  Serial.print("  Roll: ");
  Serial.println(ypr[2] * RADIANS_TO_DEG);

  delay(5);
}