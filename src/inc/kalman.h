/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Kalman Filter
 * Authors: Kristoffer Semelka, Austin
 */

#include "sensors.h"
#ifndef _KALMAN_H_
#define _KALMAN_H_

void initFilter();
void updateFilter();

#define SENSOR_DATA_FIELDS 5
#define NAV_DATA_FIELDS 5

typedef KalmanGain float[SENSOR_DATA_FIELDS][NAV_DATA_FIELDS]; 

navData* getCurrentEstimate();

#endif
