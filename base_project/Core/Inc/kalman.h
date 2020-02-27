#ifndef _KALMAN_H_
#define _KALMAN_H_
#include <math.h>
#include "main.h"

typedef struct kalman_typedef_t{
  /* Kalman filter variables */
    float Q_angle; // Process noise variance for the accelerometer
    float Q_bias; // Process noise variance for the gyro bias
    float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

    float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
}kalman_type;
// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float getAngle(kalman_type *p_k, float newAngle, float newRate, float dt);
void setAngle(kalman_type *p_k, float angle); // Used to set angle, this should be set as the starting angle
void kalman_init(kalman_type *p_k, float Q_angle, float Q_bias, float R_mea);
#endif
