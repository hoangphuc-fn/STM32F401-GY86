#ifndef _HAL_I2C_HMC5883L_H_
#define _HAL_I2C_HMC5883L_H_
//#include "i2c.h"
#include <stdbool.h>
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
/******************************************************************************/
/*                          I2C Device Address		            	      				*/
/******************************************************************************/
#define HMC5883L_ADDRESS 0x3C
/******************************************************************************/
/*                          HMC5883L Register		            	      					*/
/******************************************************************************/
#define HMC5883L_RA_CONFIG_A        0x00
#define HMC5883L_RA_CONFIG_B        0x01
#define HMC5883L_RA_MODE            0x02
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAX_L         0x04
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAZ_L         0x06
#define HMC5883L_RA_DATAY_H         0x07
#define HMC5883L_RA_DATAY_L         0x08
#define HMC5883L_RA_STATUS          0x09
#define HMC5883L_RA_ID_A            0x0A
#define HMC5883L_RA_ID_B            0x0B
#define HMC5883L_RA_ID_C            0x0C

#define HMC5883L_CRA_AVERAGE_BIT    6
#define HMC5883L_CRA_AVERAGE_LENGTH 2
#define HMC5883L_CRA_RATE_BIT       4
#define HMC5883L_CRA_RATE_LENGTH    3
#define HMC5883L_CRA_BIAS_BIT       1
#define HMC5883L_CRA_BIAS_LENGTH    2

#define HMC5883L_AVERAGING_1        0x00
#define HMC5883L_AVERAGING_2        0x01
#define HMC5883L_AVERAGING_4        0x02
#define HMC5883L_AVERAGING_8        0x03

#define HMC5883L_RATE_0P75          0x00
#define HMC5883L_RATE_1P5           0x01
#define HMC5883L_RATE_3             0x02
#define HMC5883L_RATE_7P5           0x03
#define HMC5883L_RATE_15            0x04
#define HMC5883L_RATE_30            0x05
#define HMC5883L_RATE_75            0x06

#define HMC5883L_BIAS_NORMAL        0x00
#define HMC5883L_BIAS_POSITIVE      0x01
#define HMC5883L_BIAS_NEGATIVE      0x02

#define HMC5883L_CRB_GAIN_BIT       7
#define HMC5883L_CRB_GAIN_LENGTH    3

#define HMC5883L_GAIN_1370          0x00
#define HMC5883L_GAIN_1090          0x01
#define HMC5883L_GAIN_820           0x02
#define HMC5883L_GAIN_660           0x03
#define HMC5883L_GAIN_440           0x04
#define HMC5883L_GAIN_390           0x05
#define HMC5883L_GAIN_330           0x06
#define HMC5883L_GAIN_220           0x07

#define HMC5883L_MODEREG_BIT        1
#define HMC5883L_MODEREG_LENGTH     2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_STATUS_LOCK_BIT    1
#define HMC5883L_STATUS_READY_BIT   0

typedef enum
{
    HMC5883L_RANGE_8_1GA     = 7,
    HMC5883L_RANGE_5_6GA     = 6,
    HMC5883L_RANGE_4_7GA     = 5,
    HMC5883L_RANGE_4GA       = 4,
    HMC5883L_RANGE_2_5GA     = 3,
    HMC5883L_RANGE_1_9GA     = 2,
    HMC5883L_RANGE_1_3GA     = 1,
    HMC5883L_RANGE_0_88GA    = 0
} hmc5883l_range_t;

typedef enum
{
    HMC5883L_IDLE          = 2,
    HMC5883L_SINGLE        = 1,
    HMC5883L_CONTINOUS     = 0
} hmc5883l_mode_t;

typedef enum
{
    HMC5883L_DATARATE_75HZ       = 6,
    HMC5883L_DATARATE_30HZ       = 5,
    HMC5883L_DATARATE_15HZ       = 4,
    HMC5883L_DATARATE_7_5HZ      = 3,
    HMC5883L_DATARATE_3HZ        = 2,
    HMC5883L_DATARATE_1_5HZ      = 1,
    HMC5883L_DATARATE_0_75_HZ    = 0
} hmc5883l_dataRate_t;

typedef enum
{
    HMC5883L_SAMPLES_8     = 3,
    HMC5883L_SAMPLES_4     = 2,
    HMC5883L_SAMPLES_2     = 1,
    HMC5883L_SAMPLES_1     = 0
} hmc5883l_samples_t;

extern float mgPerDigit;
extern int32_t v[3];
extern int xOffset, yOffset;
extern int xValue, yValue;

bool HMC5883L_initialize(void);
bool HMC5883L_testConnection(void);
void setRange(hmc5883l_range_t range);
void setMeasurementMode(hmc5883l_mode_t mode);
void setDataRate(hmc5883l_dataRate_t dataRate);
void setSamples(hmc5883l_samples_t samples);
void setOffset(int xo, int yo);

void readRaw(int16_t data_read[]);
void readNormalize(float data_read[]);

#endif /*_HAL_I2C_HMC5883L_H_*/
