/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: 
 */

#ifndef _SENSORS_H_
#define _SENSORS_H_

typedef struct s_navData
{
	/* In degrees of compass */
	float heading;
} navData;

void initSensors();
void updateSensors();
navData* getNavData();

#endif

