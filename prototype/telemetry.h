/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

/*
 * If you want to add a new telemetry type, be sure you increase the TELEMETRY_TYPES define so
 * that memory is allocated. Also add an entry to the telemetryNames table in telemetry.cpp
 */
typedef enum
{
	MOTION_TURNING,
	NAVIGATION_ERROR,
	SENSORS_MAG
} TelemetryType;

/* If new types are added or removed from the enum, this needs to be changed to reflect that */
#define TELEMETRY_TYPES 3
#define TELEMETRY_MAX 16

/* Open up telemetry.cpp and add a name for any new categories you add */
/* TODO: This is only used on the RX side, which is not currently implemented
extern const char* telemetryNames[TELEMETRY_TYPES];
*/

void initTelemetry();
void updateTelemetry();

void addTelemetryData( TelemetryType type, int32_t value );
#endif

