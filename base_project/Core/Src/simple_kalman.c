#include "simple_kalman.h"

void kalman_init_simple(simple_kalman_type *p_k, float mea_e, float est_e, float q){
	p_k->err_measure = mea_e;
	p_k->err_estimate = est_e;
	p_k->q = q;
}

float updateEstimate(float mea, simple_kalman_type *p_k)
{
  p_k->kalman_gain = p_k->err_estimate/(p_k->err_estimate + p_k->err_measure);
  p_k->current_estimate = p_k->last_estimate + p_k->kalman_gain * (mea - p_k->last_estimate);
  p_k->err_estimate =  (1.0 - p_k->kalman_gain)*p_k->err_estimate \
												+ fabs(p_k->last_estimate-p_k->current_estimate)*p_k->q;
  p_k->last_estimate=p_k->current_estimate;
  return p_k->current_estimate;
}
