/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "bezier.h"

float desiredHeading[3];

float * getDesiredHeading(void){
  
  desiredHeading[0]= 0.f;
  desiredHeading[1]= 0.f;
  
  return desiredHeading;

}
