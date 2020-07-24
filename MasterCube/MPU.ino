#include "MPU9250.h" // https://github.com/hideakitai/MPU9250

MPU9250 mpu;

int lastMPUUpdate = 0;
int lastMPURead = 0;
int lastRoll = 0;
int lastPitch = 0;
int lastYaw = 0;
byte lastDirection = 0;

void MPU_Init() {
  Wire.begin(MPU_SDA, MPU_SCL);
  mpu.setup();

  // calibrate anytime you want to
  Display_SetAll(0, 0, 255);
  Display_Force_Update();
  mpu.calibrateAccelGyro();
  
  Display_SetAll(0, 100, 255);
  Display_Force_Update();  
  mpu.calibrateMag();
  
  Display_Clear();
  Display_Force_Update();
}

void MPU_Update() {
  if (millis() - lastMPUUpdate > MPU_UPDATE_RATE) {
    lastMPUUpdate = millis();
    mpu.update();
  }

  if (millis() - lastMPURead > MPU_READ_RATE) {
    lastMPURead = millis();
    MPU_Read();
  }
}

byte MPU_GetDirection() {
  return lastDirection;
}

byte MPU_GetRoll() {
  return lastRoll;
}

byte MPU_GetPitch() {
  return lastPitch;
}

byte MPU_GetYaw() {
  return lastYaw;
}

void MPU_Read() {      
  lastRoll = mpu.getRoll();
  lastPitch = mpu.getPitch();
  lastYaw = mpu.getYaw();
  lastDirection = 9;
  
  if (MPU_Reading_In_Range(lastPitch, 1)) {
    lastDirection = BACK;
  } else if (MPU_Reading_In_Range(lastPitch, 3)) {
    lastDirection = FRONT;
  } else  if (MPU_Reading_In_Range(lastRoll, 2)) {
    lastDirection = TOP;
  } else if (MPU_Reading_In_Range(lastRoll, 1)) {
    lastDirection = LEFT;
  } else if (MPU_Reading_In_Range(lastRoll, 3)) {
    lastDirection = RIGHT;
  } else if (MPU_Reading_In_Range(lastRoll, 0)) {
    lastDirection = BOTTOM;
  }
}

bool MPU_Reading_In_Range(int val, byte rangeIndex) {
  switch (rangeIndex) {
    case 0:
      return (val >= 135 || val < -135);
    case 1:
      return (val >= -135 && val < -45);
    case 2:
      return (val >= -45 && val < 45);
    case 3:
      return (val >= 45 && val < 135);
  }
}
