/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "bezier.h"

vector desiredHeading;

void updateDesiredHeading(){

  desiredHeading[0]= 0f;
  desiredHeading[1]= 0f;

}
