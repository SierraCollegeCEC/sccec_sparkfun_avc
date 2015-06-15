/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: 
 */

#ifndef _SENSORS_H_
#define _SENSORS_H_

void initSensors();
void updateSensors();

typedef struct s_INSData
{	/* Data structure for Accelerometer and Gyroscope data */
	float disp_x;       /* Displacement in x direction */
	float disp_y;       /* Displacement in y direction */
	float change_vel_x; /* Change of velocity in x */
	float change_vel_y; /* Change of velocity in y */
	float yaw;          /* Directional displacement in xy-plane (compass direction) */
} INSData;

typedef struct s_SensorData
{	/* Data structure for GPS and Magnetometer Data */
	float pos_x;   /* Global x coordinate (from GPS) */
	float pos_y;   /* Global y coordinate (from GPS) */
	float vel_x;   /* Velocity in x (from GPS) */
	float vel_y;   /* Velocity in y (from GPS) */
	float heading; /* Direction in xy-plane (from Mag) */
} SensorData;

typedef struct s_NavData
{	/* Data structure for GPS and Magnetometer Data */
	float pos_x;   /* Global x coordinate */
	float pos_y;   /* Global y coordinate */
	float vel_x;   /* Velocity in x */
	float vel_y;   /* Velocity in y */
	float heading; /* Direction in xy-plane (compass direction) */
} NavData;

INSData* getINSData();
sensorData* getSensorData();
NavData* getNavData();

#endif

