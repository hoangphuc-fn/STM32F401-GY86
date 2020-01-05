#ifndef		_pid_h_
#define		_pid_h_
#include "main.h"

typedef struct pid_typedef{
	float kP;
	float kI;
	float kD;
	int32_t setPoint;
	int32_t error;
	int32_t preError;
	int32_t output;
}pid_type;

int32_t pid_compute(float p_kP, float p_kI, float p_kD, int32_t p_error, int32_t p_preError);

#endif	/*!< _pid_h_ */
