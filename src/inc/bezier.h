/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#ifndef _BEZIER_H_
#define _BEZIER_H_
#include "common.h"

extern curve map; /* A linked list of curves. Configurable. */
void initMap(float* map);
/* Initialize the control points defining the map. */

vector getDesiredHeading(vector currentPos);
/* Get a desired heading, given where you currently are. */

#endif
