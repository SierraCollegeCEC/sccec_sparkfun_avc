/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * PID Controller
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>

float lastHeadingDiff, totalErr;

float pidAdjust(float headingDiff)
{
  /*
    Takes a value and applies the PID controller to it.
    Currently only does a proportional correction.
  */
  float newHeading;
  float kp, kd, ki;
  
  kp = 3.f;
  kd = 2.f;
  ki = 0.1f;
  
  /*
    Maybe this is a good value. Maybe it's not! Who knows!
  */

  totalErr += headingDiff; 

  newHeading = headingDiff * kp
    /* Proportional Correction */
    + (headingDiff - lastHeadingDiff) * kd / dt
    /* Derivative Correction */
    + (totalErr * ki);
  /* Integral Correction */

  lastHeadingDiff = headingDiff;

  return newHeading;
}
