#include "pid.h"

int32_t pid_position_compute(float p_kP, float p_kI, float p_kD, int32_t p_error, int32_t p_preError){
	int32_t l_output;
	int32_t l_PPart;
	int32_t l_IPart;
	int32_t l_DPart;
	l_PPart = p_kP * p_error;
	l_IPart += (p_kI*(p_error));
	l_DPart = p_kD*(p_error-p_preError);
	l_output = l_PPart + l_IPart + l_DPart;
	return l_output;
}

int32_t pid_speed_compute(float p_kP, float p_kI, float p_kD, int32_t p_error, int32_t p_preError){
	int32_t l_output;
	int32_t l_PPart;
	int32_t l_IPart;
	int32_t l_DPart;
	l_PPart = p_kP * p_error;
	l_IPart += (p_kI*(p_error));
	l_DPart = p_kD*(p_error-p_preError);
	l_output += l_PPart + l_IPart + l_DPart;
	return l_output;
}
