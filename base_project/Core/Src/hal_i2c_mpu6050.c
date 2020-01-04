#include "hal_i2c_mpu6050.h"

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c, MPU6050_DataRate_TypeDef dataRate, MPU6050_Acce_Range_Typedef acceRange, MPU6050_Gyro_Range_Typedef gyroRange){
	uint8_t Lcheck;
    /*!< First, check if the sensor is responding - Should return 0x68 */
	HAL_I2C_Mem_Read (hi2c, MPU6050_ADDRESS,WHO_AM_I_MPU6050, 1, &Lcheck, 1, 1000);
    if(Lcheck == 0x68){
		/*<! Second, wake the sensor up */
		uint8_t LData = 0;
		HAL_I2C_Mem_Write(hi2c, MPU6050_ADDRESS, PWR_MGMT_1, 1,&LData, 1, 1000);
		/*!< Set DATA RATE by writing SMPLRT_DIV register*/
		LData = dataRate;
		HAL_I2C_Mem_Write(hi2c, MPU6050_ADDRESS, SMPLRT_DIV, 1, &LData, 1, 1000);
		/*!< Set accelerometer configuration in ACCEL_CONFIG Register*/
		LData = acceRange;
		HAL_I2C_Mem_Write(hi2c, MPU6050_ADDRESS, ACCEL_CONFIG, 1, &LData, 1, 1000);
		// Set Gyroscopic configuration in GYRO_CONFIG Register
		LData = gyroRange;
		HAL_I2C_Mem_Write(hi2c, MPU6050_ADDRESS, GYRO_CONFIG, 1, &LData, 1, 1000);
		//return MPU6050_Result_Ok;
    }
    //return MPU6050_Result_Error;
	return Lcheck;
}

MPU6050_Result MPU6050_Read_Accel (I2C_HandleTypeDef *hi2c, float A[], MPU6050_Acce_Sensitivities_Typedef acceSen){
	uint8_t tempResult[6] = {0,0,0,0,0,0};	
	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDRESS, ACCEL_XOUT_H, 1, tempResult, 6, 1000);
	int16_t Accel_X_RAW = (int16_t)(tempResult[0] << 8 | tempResult [1]);
	int16_t Accel_Y_RAW = (int16_t)(tempResult[2] << 8 | tempResult [3]);
	int16_t Accel_Z_RAW = (int16_t)(tempResult[4] << 8 | tempResult [5]);
	A[0] = Accel_X_RAW*1.0/acceSen;
	A[1] = Accel_Y_RAW*1.0/acceSen;
	A[2] = Accel_Z_RAW*1.0/acceSen;
	if(tempResult[0]!=0||tempResult[1]!=0||tempResult[2]!=0||tempResult[3]!=0||tempResult[4]!=0||tempResult[5]!=0)	return MPU6050_Result_Ok;
	else	return MPU6050_Result_Error; 
}
MPU6050_Result MPU6050_Read_Gyro (I2C_HandleTypeDef *hi2c, float A[], MPU6050_Gyro_Sensitivities_Typedef gyroSen){
	uint8_t tempResult[6] = {0,0,0,0,0,0};	
	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDRESS, GYRO_XOUT_H, 1, tempResult, 6, 1000);
	int16_t Gyro_X_RAW = (int16_t)(tempResult[0] << 8 | tempResult [1]);
	int16_t Gyro_Y_RAW = (int16_t)(tempResult[2] << 8 | tempResult [3]);
	int16_t Gyro_Z_RAW = (int16_t)(tempResult[4] << 8 | tempResult [5]);
	A[0] = Gyro_X_RAW*1.0;
	A[1] = Gyro_Y_RAW*1.0;
	A[2] = Gyro_Z_RAW*1.0;
	if(tempResult[0]!=0||tempResult[1]!=0||tempResult[2]!=0||tempResult[3]!=0||tempResult[4]!=0||tempResult[5]!=0)	return MPU6050_Result_Ok;
	else	return MPU6050_Result_Error; 
}
