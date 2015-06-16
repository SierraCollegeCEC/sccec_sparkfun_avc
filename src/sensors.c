/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: Alex Norell, Kosuke Fujimoto
 * 
 */

#include "common.h"
#include "sensors.h"
#include <math.h>
#include "kalman.h"

#ifndef MAG
void mag_reset();
float mag_read()
{
	return 0.f;
}
#endif

/* Drivers */


/* Debugging Flag */
const int DEBUGGING = 0;

INSData INS;
sensorData SensorData;

void initSensors()
{
	mag_reset();
}

void integrateGyro(float reading)
{
	INS.yaw += reading * dt;
}

void integrateAccel(vector readings)
{
	INS.change_vel_x += readings.x * pow(dt,2) / 2;
	INS.change_vel_y += readings.y * pow(dt,2) / 2;

	INS.disp_x += INS.change_vel_x * dt;
	INS.disp_y += INS.change_vel_y * dt;
}

void resetINS()
{
	INS.disp_x = 0.f;
	INS.disp_y = 0.f;
	INS.change_vel_x = 0.f;
	INS.change_vel_y = 0.f;
	INS.yaw = 0.f;
}

void updateSensors()
{
	SensorData.heading = mag_read();
}

sensorData* getSensorData()
{
  return &SensorData;
}

INSData* getINSData()
{
	return &INS;
}

navData* getNavData()
{
	return getCurrentEstimate();
}
