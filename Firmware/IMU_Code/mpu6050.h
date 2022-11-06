#include <Wire.h>

#define G_PER_LSB_CFG   (float)0.000244
#define DEG_PER_LSB_CFG (float)0.061

uint8_t gyro_address = 0x68;               //The I2C address of the MPU-6050 is 0x68 in hexadecimal form.

int16_t gyroX_correction = -39;             //values received from calibration
int16_t gyroY_correction = 21;              //values received from calibration
int16_t gyroZ_correction = -6;             //values received from calibration

float acc_x, acc_y, acc_z; 
float gyro_x, gyro_y, gyro_z;
bool imu_available = false;

void mpu6050_set_dlpf_freq();
void mpu6050_set_sample_rate();
void mpu6050_set_gyro_range();
void mpu6050_set_accel_range();
void mpu6050_set_clock_source();
void mpu6050_interrupt_disable();
void mpu6050_reset();
void mpu6050_disable_sleep();
void read_imu(bool use_calibration_values);
bool mpu6050_test_connection();


void mpu6050_init(){

  Wire.begin();
  Wire.setClock(400000L);
  
  mpu6050_reset();
  
  for (int i = 0; i < 10; i++){
    if (mpu6050_test_connection()) {
      imu_available = true;
      break;
    }
    delay(100);
  }
  
  if (!imu_available) {
    while(1) Serial.println("IMU Error!!!");
  }
  
  Serial.println("IMU Initialized"); 
  
  //mpu6050_temp_sensor_enable();
  //mpu6050_interrupt_disable();
  mpu6050_set_clock_source();
  mpu6050_set_gyro_range();
  mpu6050_set_accel_range();
  //mpu6050_set_sample_rate();
  mpu6050_set_dlpf_freq();
  mpu6050_disable_sleep();
  read_imu(true);
}

void mpu6050_set_dlpf_freq(){
  //set 94Hz DLPF
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x1A);                                            
  Wire.write(0x02);                                            
  Wire.endTransmission();
  delay(50); 
}

void mpu6050_set_sample_rate(){
  //set 500Hz sample rate
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x19);                                            
  Wire.write(0x01);                                            
  Wire.endTransmission();
  delay(50); 
}

void mpu6050_set_gyro_range(){
  //set 2000dps
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x1B);                                            
  Wire.write(0x18);                                            
  Wire.endTransmission();
  delay(50); 
}

void mpu6050_set_accel_range(){
  //set 8G
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x1C);                                            
  Wire.write(0x10);                                            
  Wire.endTransmission();
  delay(50); 
}

void mpu6050_set_clock_source(){
  //set clock source to gyro x reference
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x6B);                                            
  Wire.write(0x01);                                           
  Wire.endTransmission();                                      
  delay(50);
}

void mpu6050_interrupt_disable(){
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x38);                                            
  Wire.write(0x00);                                           
  Wire.endTransmission();                                      
  delay(50);
}

void mpu6050_reset(){
  //Reset Power Management Register
  Wire.beginTransmission(gyro_address);                        
  Wire.write(0x6B);                                            
  Wire.write(0x80);                                           
  Wire.endTransmission();                                      
  delay(150);
}

bool mpu6050_test_connection(){

  Wire.beginTransmission(gyro_address);                       //Start communication with the gyro.
  Wire.write(0x75);                                           //Start reading @ register 43h and auto increment with every read.
  Wire.endTransmission();                                     //End the transmission.

  Wire.requestFrom(gyro_address, 1);

  uint8_t whoami = Wire.read();

  if (whoami == gyro_address) return true;
  else return false;
}

void mpu6050_disable_sleep(){
  Wire.beginTransmission(gyro_address);                       
  Wire.write(0x6B);                                            
  Wire.write(0x00);                                            
  Wire.endTransmission();                                      
  delay(50);
}


void read_imu(bool use_calibration_values){
  Wire.beginTransmission(gyro_address);                       //Start communication with the gyro.
  Wire.write(0x3B);                                           //Start reading @ register 43h and auto increment with every read.
  Wire.endTransmission();                                     //End the transmission.

  //MPU6050 axes are 90 degree clock wise rotated than the drone body frame
  uint8_t buff[14];
  Wire.requestFrom(gyro_address, 14);                         //Request 14 bytes from the MPU 6050.

  for (int x = 0; x < 14; x++)  buff[x] = Wire.read();
  
  int16_t ax = (((int16_t) buff[0]) << 8) | buff[1];                   
  int16_t ay = (((int16_t) buff[2]) << 8) | buff[3];                   
  int16_t az = (((int16_t) buff[4]) << 8) | buff[5];                   

  int16_t gx = (((int16_t) buff[8]) << 8) | buff[9];               
  int16_t gy = (((int16_t) buff[10]) << 8) | buff[11];                
  int16_t gz = (((int16_t) buff[12]) << 8) | buff[13];

  //if calibrated values are requierd
  if (use_calibration_values){
    gyro_x = (gx - gyroX_correction)*DEG_PER_LSB_CFG;
    gyro_y = (gy - gyroY_correction)*DEG_PER_LSB_CFG;
    gyro_z = (gz - gyroZ_correction)*DEG_PER_LSB_CFG;

    acc_x = (ax*G_PER_LSB_CFG);
    acc_y = (ay*G_PER_LSB_CFG);
    acc_z = (az*G_PER_LSB_CFG);
  }
    else{
    gyro_x = gx;
    gyro_y = gy;
    gyro_z = gz;
    acc_x = ax;
    acc_y = ay;
    acc_z = az;
  }
}
