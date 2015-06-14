/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * PID Controller
 * Authors: Kristoffer Semelka
 */

#ifndef _PID_H_
#define _PID_H_

typedef struct s_errParams {
	float kp;
	float kd;
	float ki;
	float maxInt; /* Clamping integration error prevents saturation. */
	float lastErr;
	float totalErr;

} errParams;

void setErrParams(errParams * p, float kp, float kd, float ki, float maxInt);

float pidAdjust(float err, errParams * p);


#endif
