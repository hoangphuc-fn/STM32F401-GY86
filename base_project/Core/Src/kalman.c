#include "kalman.h"

float getAngle(kalman_type *p_k, float newAngle, float newRate, float dt){
	/* Step 1 */
	p_k->rate = newRate - p_k->bias;
	p_k->angle += dt*p_k->rate;
	/* Step 2 */
	p_k->P[0][0] += dt * (dt*p_k->P[1][1] - p_k->P[0][1] - p_k->P[1][0] + p_k->Q_angle);
	p_k->P[0][1] -= dt * p_k->P[1][1];
	p_k->P[1][0] -= dt * p_k->P[1][1];
	p_k->P[1][1] += p_k->Q_bias * dt;
	/* Step 4 */
	float S = p_k->P[0][0] + p_k->R_measure;
	/* Step 5 */
	float K[2];
	K[0] = p_k->P[0][0]/S;
	K[1] = p_k->P[1][0]/S;
	/* Step 3 */
	float y = newAngle - p_k->angle;
	/* Step 6 */
	p_k->angle += K[0]*y;
	p_k->bias += K[1]*y;
	float P00_temp = p_k->P[0][0];
	float P01_temp = p_k->P[0][1];
	
	p_k->P[0][0] -= K[0]*P00_temp;
	p_k->P[0][1] -= K[0]*P01_temp;
	p_k->P[1][0] -= K[1]*P00_temp;
	p_k->P[1][1] -= K[1]*P01_temp;
	return p_k->angle;
}

void kalman_init(kalman_type *p_k, float Q_angle, float Q_bias, float R_mea){
	p_k->Q_angle = Q_angle;
	p_k->Q_bias = Q_bias;
	p_k->R_measure = R_mea;
	
	p_k->angle = 0.0f;
	p_k->bias = 0.0f;
	
	p_k->P[0][0] = 0.0f;
	p_k->P[0][1] = 0.0f;
	p_k->P[1][0] = 0.0f;
	p_k->P[1][1] = 0.0f;
}
