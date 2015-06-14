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

#define NAV_DATA_FIELDS 5
typedef enum navData_fields {pos_x, pos_y, vel_x, vel_y, heading} navData_field;

typedef float navData[NAV_DATA_FIELDS];

typedef KalmanGain float[SENSOR_DATA_FIELDS][NAV_DATA_FIELDS]; 

navData getNavData();

#endif
