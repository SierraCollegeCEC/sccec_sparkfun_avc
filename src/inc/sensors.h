/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: 
 */

#ifndef _SENSORS_H_
#define _SENSORS_H_

void initSensors();
void updateSensors();

#define INSDATA_FIELDS 5
typedef enum INSData_fields {disp_x, disp_y, change_vel_x, change_vel_y,
														 yaw} INSData_field;
typedef float INSData[INSDATA_FIELDS];

#define SENSOR_DATA_FIELDS 5
typedef enum sensorData_fields {pos_x, pos_y, vel_x, vel_y, yaw} sensorData_field;
typedef float sensorData[SENSOR_DATA_FIELDS];

INSData getINSData();
sensorData getSensorData();

#endif

