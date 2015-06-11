/*
 * schedule.h
 *
 *  Created on: Nov 1, 2014
 *      Author: james
 */

#ifndef _SPARKFUN_SCHEDULE_H_
#define _SPARKFUN_SCHEDULE_H_

#define SCH_MAX_TASKS (3)

typedef struct
{
	void (* taskFunction)(void);
	uint16_t delay;
	uint16_t period;
	uint8_t runMe;
} schTask;

void schInit();
uint8_t schAddTask(void (* function)(), const uint16_t DELAY, const uint16_t PERIOD);
uint8_t schDeleteTask( uint8_t i );
void schUpdate();
void schDispatchTasks();
void schStart();

#endif
