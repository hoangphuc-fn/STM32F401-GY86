#ifndef		_motor_h_
#define		_motor_h_
#include "main.h"

void speed_run(int32_t speed_left, int32_t speed_right);
uint8_t handle_angle(int16_t angle_target, int16_t angle_fb);
uint8_t handle_straight(int32_t dis_target, int32_t dis_fb, int32_t speed_left, int32_t speed_right);
void reset_run(uint16_t time);
#endif	/*!< _motor_h_ */
