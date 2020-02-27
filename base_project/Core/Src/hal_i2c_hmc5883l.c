#include "hal_i2c_hmc5883l.h"

float mgPerDigit;
int32_t v[3];
int xOffset, yOffset;
int xValue, yValue;

bool HMC5883L_initialize(void){
	uint8_t f_tempData[3];
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_ID_A, 1, f_tempData, 3, 1000);
	if(	f_tempData[0] != 0x48
	||	f_tempData[1] != 0x34
	||	f_tempData[2] != 0x33){
		return false;
	}
	return true;
}
void setRange(hmc5883l_range_t range){
	switch(range){
	case HMC5883L_RANGE_0_88GA:
	    mgPerDigit = 0.073f;
	    break;

	case HMC5883L_RANGE_1_3GA:
	    mgPerDigit = 0.92f;
	    break;

	case HMC5883L_RANGE_1_9GA:
	    mgPerDigit = 1.22f;
	    break;

	case HMC5883L_RANGE_2_5GA:
	    mgPerDigit = 1.52f;
	    break;

	case HMC5883L_RANGE_4GA:
	    mgPerDigit = 2.27f;
	    break;

	case HMC5883L_RANGE_4_7GA:
	    mgPerDigit = 2.56f;
	    break;

	case HMC5883L_RANGE_5_6GA:
	    mgPerDigit = 3.03f;
	    break;

	case HMC5883L_RANGE_8_1GA:
	    mgPerDigit = 4.35f;
	    break;
	default:
	    break;
    }
	uint8_t temp = range<<5;
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_B, 1, &temp, 1, 1000);
}
void setMeasurementMode(hmc5883l_mode_t mode){
	uint8_t temp;
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_MODE, 1, &temp, 1, 1000);
	temp &= 0xfc;
	temp |= mode;
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_MODE, 1, &temp, 1, 1000);
}
void  setDataRate(hmc5883l_dataRate_t dataRate){
	uint8_t temp;
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_A, 1, &temp, 1, 1000);
	temp &= 0xe3;
	temp |= (dataRate<<2);
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_MODE, 1, &temp, 1, 1000);
}
void  setSamples(hmc5883l_samples_t samples){
	uint8_t temp;
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_A, 1, &temp, 1, 1000);
	temp &= 0x9f;
	temp |= (samples<<5);
	HAL_I2C_Mem_Write(&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_CONFIG_A, 1, &temp, 1, 1000);
}
void  setOffset(int xo, int yo){
	xOffset = xo;
  yOffset = yo;
}

void readRaw(int16_t data_read[]){
	uint8_t tempResult[6] = {0,0,0,0,0,0};
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_DATAX_H, 1, tempResult, 6, 1000);
	data_read[0] = ((int16_t)(tempResult[0] << 8 | tempResult [1])- xOffset);	// x
	data_read[1] = ((int16_t)(tempResult[4] << 8 | tempResult [5])- yOffset);	// y
	data_read[2] = ((int16_t)(tempResult[2] << 8 | tempResult [3]));	// z
}

void readNormalize(float data_read[])
{
	uint8_t tempResult[6] = {0,0,0,0,0,0};
	HAL_I2C_Mem_Read (&hi2c1, HMC5883L_ADDRESS, HMC5883L_RA_DATAX_H, 1, tempResult, 6, 1000);
	xValue = ((int16_t)(tempResult[0] << 8 | tempResult [1])- xOffset);
	yValue = ((int16_t)(tempResult[4] << 8 | tempResult [5])- yOffset);
  data_read[0] = xValue*mgPerDigit;	// x
	data_read[1] = yValue*mgPerDigit;	// y
	data_read[2] = ((int16_t)(tempResult[2] << 8 | tempResult [3]))*mgPerDigit;	// z
}

