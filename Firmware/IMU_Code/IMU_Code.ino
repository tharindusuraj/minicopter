#include "mpu6050.h"

void setup() {
  mpu6050_init();
}

void loop() {
  read_imu(true);

  Serial.print("Ax: ");    Serial.print(acc_x);
  Serial.print(" Ay: ");   Serial.print(acc_y);
  Serial.print(" Az: ");   Serial.print(acc_z);
  Serial.print(" Gx: ");   Serial.print(gyro_x);
  Serial.print(" Gy: ");   Serial.print(gyro_y);
  Serial.print(" Gz: ");   Serial.print(gyro_z);
  Serial.println();

}
