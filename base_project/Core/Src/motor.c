#include "motor.h"

extern TIM_HandleTypeDef htim9;

void speed_run(int32_t speed_left, int32_t speed_right){
	if(speed_left < -40000) speed_left = -40000;
	else if(speed_left > 40000) speed_left = 40000;
	if(speed_right < -40000) speed_right = -40000;
	else if(speed_right > 40000) speed_right = 40000;
	
	if(speed_right<0){
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
		htim9.Instance->CCR1 = -speed_right;
	}
	else{
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
		htim9.Instance->CCR1 = speed_right;
	}
	if(speed_left<0){
		HAL_GPIO_WritePin(AIN1_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(AIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
		htim9.Instance->CCR2 = -speed_left;
	}
	else{
		HAL_GPIO_WritePin(AIN1_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(AIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
		htim9.Instance->CCR2 = speed_left;
	}
}
