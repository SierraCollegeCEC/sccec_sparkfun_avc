/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: Alex Norell, Kosuke Fujimoto
 * 
 * Sensor: Hitachi HM55B
 * Board: Paralax HM55B REV B 29123
 *	
 *           ╒══════════════════╕
 *           │ ▀▀▀   ▄▄▄▄   ▀▀▀ │ 
 *           │ ▄▄▄   ▀▀▀▀   ▄▄▄ │ 
 *    Din (1)│██▄█▌        ▐█▄██│(6) Vcc
 *           │████▌▐██████▌▐████│ 
 *   Dout (2)│██▄█▌▐██████▌▐█▄██│(5) /EN
 *           │████▌▐██████▌▐████│ 
 *    GND (3)│██▄█▌▐██████▌▐█▄██│(4) CLK
 *           │ ▄▄▄    ▄▄▄   ▄▄▄ │
 *           ╘══════════════════╛
 * 
 * PIN	Name	Function
 * 1	Din		Serial data input
 * 2 	Dout	Serial data output
 * 3   	GND		Ground -> 0V
 * 4  	CLK		Synchronous clock input
 * 5	/EN		Active-low device enable
 * 6	Vcc		+5V power input
 *
 */
#include "common.h"
#include "sensors.h"
#include <math.h>
#include <Arduino.h>

/* Drivers */
#include "drivers/hm55b.h" /* Magenetometer */

/* Debugging Flag */
const int DEBUGGING = 0;

sensorData SensorData;
INSData INS;

void initSensors()
{
	pinMode( EN, OUTPUT );
	pinMode( CLK, OUTPUT );
	pinMode( DATA, INPUT );

	mag_reset();
	int filter_array[10];
}

void integrateGyro(float reading)
{
	INS[yaw] += reading * dt;
}

void integrateAccel(vector readings)
{
	INS[change_vel_x] += readings.x * dt^2 / 2;
	INS[change_vel_y] += readings.y * dt^2 / 2;

	INS[displ_x] += change_vel_x * dt;
	INS[displ_y] += change_vel_y * dt;
}

void resetIntegrators()
{
	for(int i = 0; i < INSDATA_FIELDS; i++){
		INS[i] = 0.f;
	}
}

void updateSensors()
{
	SensorData[heading] = mag_read();
}

sensorData getSensorData()
{
  return &SensorData;
}

INSData getINSData()
{
	return &INSData;
}
