/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

/*
 * This is the typedef for function pointers that will respond to events from the telemetry
 * system. These events are fired when messages are recieved from the reciever
 */
typedef void (*telemetryEventHandler)(char*,float);

/*
 * Utility macro for defining event handlers. Creates a referenceable function pointer that dispatches only
 * on "key", and binds the value of the message to "vals" (this is to avoid hidden bindings).

 * This macro must be followed by an expansion of
 * END_EVENT_HANDLER. Luckily, your program will not compile if you do not, and it will be obvious.
 *
 * Usage Example:
 * BEGIN_EVENT_HANDLER(handleMissilesLaunched, "missiles_launched", destination)
 * {
 *     char* dest = findDestFromLocation(destination);
 *     if(populated(dest)){
 *         printf("We have to evacuate everyone from %s!", dest);
 *         evacuate(dest);
 *     }
 *     else{
 *         printf("No one lives there anyway.");
 *     }
 * }
 * END_EVENT_HANDLER
 *
 * // later ...
 * initDefcon(){
 *	addTelemetryEventHandler(handleMissilesLaunched);
 * }
 *
 * exitDefcon(){
 *   removeTelemetryEventHandler(handleMissilesLaunched);
 * }
 */

#define BEGIN_EVENT_HANDLER( name, key, vals ) void (*name)(char* keyreceived, char* vals)\
 {\
	if(strcmp(key, keyreceived) == 0)

#define END_EVENT_HANDLER }


void initTelemetry();
void updateTelemetry();
void addTelemetryEventHandler( telemetryEventHandler func );
void removeTelemetryEventHandler( telemetryEventHandler func );
#endif

