/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Motion Subsystem
 * author: James Smith
 */

#include <stdio.h>
#include <stdint.h>

/* TI Drivers */
#include "sys_common.h"
#include "rti.h"

/* AVC */
#include "schedule.h"
#include "sensors.h"
#include "nav.h"
#include "motion.h"

int counter = 0;
int main( void )
{
	schInit();
	rtiInit();
	rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
	_enable_IRQ();
	rtiStartCounter(rtiCOUNTER_BLOCK0);

	sensorsInit();
	navigationInit();
	motionInit();

	schAddTask( sensorsUpdate, 0, 10 );
	schAddTask( navigationUpdate, 0, 30 );
	schAddTask( motionUpdate, 0, 30 );

	schStart();
	while( 1 )
	{
		/* TODO: the update call will move to a hardware timer eventually */
		schUpdate();
		schDispatchTasks();
	}
}

void rtiNotification(uint32 notification)
{
	//printf("Timer\n");
	counter++;
}
