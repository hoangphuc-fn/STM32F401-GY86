#ifndef _SIMPLE_KALMAN_H_
#define _SIMPLE_KALMAN_H_
#include <math.h>
#include "main.h"

typedef struct simple_kalman_typedef_t{
  float err_measure;
  float err_estimate;
  float q;
  float current_estimate;
  float last_estimate;
  float kalman_gain;
}simple_kalman_type;

float updateEstimate(float mea, simple_kalman_type *p_k);
void kalman_init_simple(simple_kalman_type *p_k, float mea_e, float est_e, float q);

#endif /*_SIMPLE_KALMAN_H_*/
