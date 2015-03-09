/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * PID Controller
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "pid.h"

float lastHeadingDiff, totalErr;

void setErrParams(errParams * p, float kp, float kd, float ki, float maxInt){
  
	/* Constructs an errParams struct and sets defaults. */

	p->kp = kp;
	p->kd = kd;
	p->ki = ki;
	p->maxInt = maxInt;
	p-> lastErr = 0.f;
	p-> totalErr = 0.f;
}

float pidAdjust(float err, errParams * p)
{
	/*
	  Takes a value and its associated parameters, and applies the PID controller to it.
	  We use a pointer so we don't have to copy all these floats everywhere.
	*/
  
	float correction;
  
	p->totalErr += err;
  
	correction = err * p->kp
		/* Proportional Correction */
		+ (err - p->lastErr) * p->kd / dt
		/* Derivative Correction */
		+ fmax(p->totalErr, p->maxInt) * p->ki;
	/* Integral Correction */

	p->lastErr = correction;

	return correction;
}
