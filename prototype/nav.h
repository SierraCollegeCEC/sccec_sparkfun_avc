/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Navigation Subsystem
 * Authors: James Smith, Kristoffer Semelka
 */

#ifndef _NAV_H_
#define _NAV_H_

#define M1HEADING 280

/*
  I'm assuming true north for this milestone.
*/

typedef struct s_motionData
{
	float heading;
	float speed;
} motionData;

motionData* getMotionData();
void initNav();
void updateNav();

#endif

