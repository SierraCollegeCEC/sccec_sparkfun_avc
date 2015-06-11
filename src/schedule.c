/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Scheduler
 * author: James Smith
 * TODO: Add watchdog timer to scheduler
 */

#include <stdint.h>
#include "rti.h"
#include "schedule.h"

/* Stores all of the tasks currently running */
schTask schTasks[SCH_MAX_TASKS];

/* Error Codes */
uint8_t schError;
#define ERROR_SCH_TOO_MANY_TASKS 1
#define ERROR_SCH_CANNOT_DELETE_TASK 2

void schReportStatus();
void schSleep();

void schInit()
{
	uint8_t index;
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		schDeleteTask( index );
	}
	schError = 0;
}

uint8_t schAddTask( void (*function)(), const uint16_t DELAY, const uint16_t PERIOD )
{
	uint8_t index = 0;

	while( (schTasks[index].taskFunction != 0) && (index < SCH_MAX_TASKS) )
	{
		index++;
	}

	if( index == SCH_MAX_TASKS )
	{
		/* Task list is full */
		schError = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}

	schTasks[index].taskFunction = function;
	schTasks[index].delay = DELAY;
	schTasks[index].period = PERIOD;
	schTasks[index].runMe = 0;
	return index;
}

uint8_t schDeleteTask( const uint8_t TASK_INDEX )
{
	uint8_t returnCode;
	if( schTasks[TASK_INDEX].taskFunction == 0 )
	{
		schError = ERROR_SCH_CANNOT_DELETE_TASK;
		returnCode = 1;
	}
	else
	{
		returnCode = 0;
	}

	schTasks[TASK_INDEX].taskFunction = 0x0000;
	schTasks[TASK_INDEX].delay = 0;
	schTasks[TASK_INDEX].period = 0;
	schTasks[TASK_INDEX].runMe = 0;

	return returnCode;
}

void schUpdate()
{
	uint8_t index;

	/* NOTE: calculations are in *TICKS* (not milliseconds) */
	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		if( schTasks[index].taskFunction )
		{
			if( schTasks[index].delay == 0 )
			{
				/* The task is due to run */
				schTasks[index].runMe += 1;
				if( schTasks[index].period )
				{
					/* Schedule periodic tasks to run again */
					schTasks[index].delay = schTasks[index].period;
				}
			}
			else
			{
				schTasks[index].delay -= 1;
			}
		}
	}
}

void schDispatchTasks()
{
	uint8_t index;

	for( index = 0; index < SCH_MAX_TASKS; index++ )
	{
		if( schTasks[index].runMe > 0 )
		{
			(*schTasks[index].taskFunction)();
			schTasks[index].runMe -= 1;

			/* If this is a 'one shot' task, remove it from the array */
			if( schTasks[index].period == 0 )
			{
				schDeleteTask( index );
			}
		}
	}
	/* TODO: Possibly add hardware sleeping to reduce power consumption */
	schReportStatus();
	schSleep();
}

void schStart()
{
	/* TODO: Add hardware interrupt support */
}

void schReportStatus()
{
	/* TODO: Report error code in some way. Maybe turn on a board LED */
}

void schSleep()
{
	/* TODO: add hardware sleep? */
}
