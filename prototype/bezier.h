/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#ifndef _BEZIER_H_
#define _BEZIER_H_
#include "common.h"

void initMap(void);
/* Initialize the control points defining the map. */

float getDesiredHeading(vector currentPos);
/* Get a desired heading, given where you currently are. */

#endif
