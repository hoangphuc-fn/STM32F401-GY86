#include "motor.h"
#include "pid.h"
#include "tim.h"

extern volatile int32_t encA;
extern volatile int32_t encB;
extern TIM_HandleTypeDef htim9;
extern volatile short temp;
extern float yaw_gyro;
extern pid_position_type pidA;

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

uint8_t handle_angle(int16_t angle_target, int16_t angle_fb){
	if((angle_target - angle_fb) < 2 && (angle_target - angle_fb) > -2){
		speed_run(0,0);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_RESET);
		return 1;
	}
	else if((angle_target - angle_fb) >= 2){
		speed_run(10000,-10000);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_SET);
		return 0;
	}
	else {
		speed_run(-10000,10000);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_SET);
		return 0;
	}
}

uint8_t handle_straight(int32_t dis_target, int32_t dis_fb, int32_t speed_left, int32_t speed_right){
	pidA.setPoint = dis_target;
	if((dis_target - dis_fb) < 200 && (dis_target - dis_fb) > -200){
		speed_run(0,0);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_RESET);
		return 1;
	}
	else {
		speed_run(speed_left,speed_right);
		HAL_GPIO_WritePin(BZ_GPIO_Port,BZ_Pin,GPIO_PIN_SET);
		return 0;
	}
}
	
void reset_run(uint16_t time){
	HAL_Delay(time);
	yaw_gyro = 0;
	temp = 0;
	encA = 0;
	encB = 0;
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_SET_COUNTER(&htim4,0);
}

