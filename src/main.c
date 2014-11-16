/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Motion Subsystem
 * author: James Smith
 */

#include <stdio.h>
#include <stdint.h>

/* TI Drivers */
/*#include "sys_common.h"*/
/*#include "rti.h"*/

/* AVC */
#include "sensors.h"
#include "navigation.h"
#include "motion.h"

int counter = 0;
int main( void )
{
	sensorsInit();
	navigationInit();
	motionInit();

	while( 1 )
	{
		sensorsUpdate();
		navigationUpdate();
		motionUpdate();
	}
}
