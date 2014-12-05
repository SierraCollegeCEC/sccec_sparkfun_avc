/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Navigation Subsystem
 * Authors: James Smith, Kristoffer Semelka
 */

#ifndef _NAV_H_
#define _NAV_H_

#define M1HEADING 0

/*
  I'm assuming true north for this milestone.
*/

typedef struct s_motionData
{
  /* In degrees */
  float heading;
  /* In some yet to be determined unit */
  float speed;
} motionData;

void initNav();
void updateNav();
motionData* getMotionData();

#endif

