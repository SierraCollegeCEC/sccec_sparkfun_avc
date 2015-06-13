/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Navigation Subsystem
 * Authors: Kristoffer Semelka
 */
#include "common.h"
#include "nav.h"
#include "sensors.h"
#include <stdlib.h>
#include <math.h>
#include "pid.h"
#include "bezier.h"
#include "string.h"
#include "telemetry.h"

motionData MotionData;
errParams headingParams, speedParams;
float magneticDeclination;

void initNav()
{

	/*initmap(); Hi! Implement me!*/

	setErrParams(&headingParams, 3.f, 2.f, 0.1f, .1f); /* Heading PID values. */

	/* Event Handlers */
	addTelemetryEventHandler(setHeadingPID);
	addTelemetryEventHandler(setMotionPID);
  
}

float findAngle(vector vec){

	float angle =  180 * ( atan2( ( -1 * vec.y ) , vec.x ) / M_PI );
	/* Dear James: Copy and pasted from sensors.cpp. Why is there a -1 on the y component?
	   Was that causing our last direction mishap? I'm too lazy to ask Alex.
	   -- Kris
	*/
	return angle;
  
}  

float findCorrection(vector current, vector desired)
{
	/*
	  Takes the current heading and desired heading, and
	  finds the (shortest, signed) difference between the two.
	*/

	float correction;
  
	correction = findAngle(desired) - findAngle(current);

	if ( fabs(correction) > 180)
		{ /* Angle too large? Use the other angle, in the other direction */

			if ( correction > 0)
				/* Were we going left (positive theta)? Go right instead. */
				{ correction -= 360.f; }
			else
				/* Were we going right (negative theta)? Go left instead. */
				{ correction += 360.f; }
		}

	return correction;
}

void updateNav()
{
	/*
	  This function:
	  - gets the navigation data from sensors (heading, position)
	  - gets desired values from Bezier Curves (position, velocity)
	  - finds difference between desired position and real position
	  to obtain desired heading;
	  - find difference between desired heading and real heading
	  - applies PID to heading differences and speed
	  - creates instructions for motion module;
	  - and pushes instructions to MotionData
	  on every update.
	*/
	navData* NavData;
	float  headingDiff;
	float adjustedHeading;

	NavData = getNavData();
  
	headingDiff = findCorrection(NavData[heading], getDesiredHeading(NavData[position]);
  
	adjustedHeading = pidAdjust(headingDiff, &headingParams);
  
	MotionData.heading = adjustedHeading;
  
}

motionData* getMotionData()
{
	return &MotionData;
}

/* Event handlers */

void setHeadingPID(char *key, char *paramsString)
{	/* Event handler for the "setHeadingPID" key. */
  
 	if ( strcmp(key, "setHeadingPID") == 0 )
	{
		float *params = parseToArray(paramsString, 4);
		setErrParams( &headingParams, params[0], params[1], params[2], params[3] );
	}	
}

void setMotionPID(char *key, char *paramsString)
{	/* Event handler for the "setMotionPID" key. */
	
	if ( strcmp(key, "setMotionPID") == 0 )
	{
		float *params = parseToArray(paramsString, 4);
		setErrParams( &headingParams, params[0], params[1], params[2], params[3] );
	}	
}