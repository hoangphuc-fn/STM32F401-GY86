#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor.h"
#include "pid.h"
#include "hal_i2c_mpu6050.h"
#include "hal_i2c_hmc5883l.h"
#include "math.h"
#include "simple_kalman.h"
#include <stdio.h>
#include <stdlib.h>
#include "kalman.h"


void SystemClock_Config(void);
/* For KALMAN filter */
simple_kalman_type k;
float real_value = 125;
float real_2;
float measured_value;
float estimated_value;
/* For the encoder */
volatile int32_t encA;
volatile int32_t encB;
/* For the PID */
pid_position_type pidA;
pid_position_type pidB;

pid_speed_type pidLW;
/* For the MPU sensor */
int16_t global[3];
float acce_result[3] = {0,0,0};
float gyro_result[3] = {0,0,0};
uint8_t check;
uint8_t test[3];
#define M_PI 3.141592654
#define RAD2DEG  180/M_PI

int16_t compass_i[3] = {0,0,0};
float compass[3] = {0,0,0};
float compass_filted[3] = {0,0,0};
double headingDegrees, fixedHeadingDegrees;
double declinationAngle;
double heading;
float base;
float diff;
float accXnorm;
float accYnorm;
float pitch;
float roll;
float magXcomp;
float magYcomp;
float norm;
float mx,my,mz;
float yaw_gyro;
float pre_yaw_gyro_deg;
float yaw_gyro_deg;
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float Norm(float a, float b, float c){
	return sqrt(a*a + b*b + c*c);
}
volatile short temp;
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM9_Init();
  MX_I2C1_Init();
	MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1|TIM_CHANNEL_2);
	/* Do not use OR bitwise operator - TIM_CHANNEL_1|TIM_CHANNEL_2 */
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	/*_____________ Init value _____________*/
	pidA.setPoint = 10000;
	pidA.kP = 100;
	pidA.kI = 0.005;
	pidA.kD = 10;
	pidB.setPoint = 0;
	pidB.kP = 100;
	pidB.kI = 1000;
	
	pidLW.setPoint = 0;
	pidLW.kP = 2000;
	pidLW.kI = 200;
	pidLW.kD = 0;
	/*_____________ Init kalman _____________*/
	kalman_init_simple(&k,1,1,0.01);
	/* Init MPU */
	check = MPU6050_Init(&hi2c1,MPU6050_DataRate_1KHz,MPU6050_Accelerometer_2G,MPU6050_Gyroscope_500s);
	HAL_Delay(500);
	if(HMC5883L_initialize()){
		setRange(HMC5883L_RANGE_8_1GA);
		setMeasurementMode(HMC5883L_CONTINOUS);
		setDataRate(HMC5883L_DATARATE_30HZ);
		setSamples(HMC5883L_SAMPLES_8);
		//setOffset(0, 0);
	}
  while (1){	
		//speed_run(pidA.output, pidB.output);
		while(!handle_straight(10000,encA,pidA.output,pidB.output));
		reset_run(500);
		while(!handle_angle(90,yaw_gyro_deg));
		reset_run(500);
		while(!handle_straight(-5000,encA,pidA.output,pidB.output));
		reset_run(500);
		while(!handle_angle(-45,yaw_gyro_deg));
		reset_run(500);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_SET);
		while(1);
		
		/*
		readNormalize(compass);
		heading = atan2(compass[1],compass[0]);
		// Set declination angle on your location and fix heading
		// You can find your declination on: http://magnetic-declination.com/
		// (+) Positive or (-) for negative
		// For Bytom / Poland declination angle is 4'26E (positive)
		// Formula: (deg + (min / 60.0)) / (180 / M_PI);
		declinationAngle = (0 + (37.0 / 60.0)) / (180 / M_PI);
		heading -= declinationAngle;
		HAL_Delay(10);
		if (heading < 0){
			heading += 2 * M_PI;
		}
		else if (heading > 2 * M_PI){
			heading -= 2 * M_PI;
		}
		headingDegrees = heading * 180/M_PI; 
		if (headingDegrees >= 0 && headingDegrees < 270)
		{
			fixedHeadingDegrees = map (headingDegrees * 100, 0, 270 * 100, 0, 179 * 100) /100.00;
		}
		else {
		if (headingDegrees >= 270){
			fixedHeadingDegrees = map (headingDegrees * 100, 270 * 100, 360 * 100, 180 * 100, 360 * 100) /100.00;
		}
		}
		
		*/
		
		/*
		
		MPU6050_Read_Gyro(&hi2c1,gyro_result,MPU6050_GYRO_SENS_500);
		readNormalize(compass);
		MPU6050_Read_Accel(&hi2c1,acce_result,MPU6050_ACCE_SENS_2);
//		accXnorm = acce_result[0]/(sqrt(acce_result[0]*acce_result[0] +
//																		acce_result[1]*acce_result[1] + 
//																		acce_result[2]*acce_result[2]));
//		accYnorm = acce_result[1]/(sqrt(acce_result[0]*acce_result[0] +
//																		acce_result[1]*acce_result[1] + 
//																		acce_result[2]*acce_result[2]));
//		pitch = asin(accXnorm)*RAD2DEG;
//		roll = asin(accYnorm/cos(pitch))*RAD2DEG;
		norm = Norm(acce_result[0],acce_result[1],acce_result[2]);
		roll = atan2(acce_result[1],acce_result[2]);
		pitch = atan2(acce_result[0],sqrt(acce_result[1]*acce_result[1] + 
																			 acce_result[2]*acce_result[2]));
//		pitch = -asin(acce_result[0]/norm)*RAD2DEG;
//		roll = asin(acce_result[1]/cos(pitch)/norm)*RAD2DEG;
		norm = Norm(compass[0],compass[1],compass[2]);
		mx = compass[0]/norm;
		my = compass[1]/norm;
		mz = compass[2]/norm;
		
		magXcomp = mx*cos(pitch) + mz*sin(pitch);
		magYcomp = mx*sin(roll)*sin(pitch) + my*cos(roll) - 
							 mz*sin(roll)*cos(pitch);
		heading = atan2(magYcomp,magXcomp)*RAD2DEG;
		if(heading>360) heading-=360;
		else if(heading<0) heading+=360;
		if (heading >= 0 && heading < 270){
			fixedHeadingDegrees = map (heading * 100, 0, 270 * 100, 0, 179 * 100) /100.00;
		}
		else if(heading >= 270){
			fixedHeadingDegrees = map (heading * 100, 270 * 100, 360 * 100, 180 * 100, 360 * 100) /100.00;
		}
		*/
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
