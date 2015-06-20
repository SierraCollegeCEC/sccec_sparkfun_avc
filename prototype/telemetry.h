/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

#include <string.h> /* Any file that adds a telemetry handler will need strcmp */

/*
 * This is the typedef for function pointers that will respond to events from the telemetry
 * system. These events are fired when messages are recieved from the reciever
 */
typedef void (*telemetryEventHandler)(char*,char*);

void initTelemetry();
void updateTelemetry();
void addTelemetryEventHandler( telemetryEventHandler func );
void removeTelemetryEventHandler( telemetryEventHandler func );
#endif

