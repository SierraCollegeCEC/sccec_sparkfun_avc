/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Kalman Filter
 * Authors: Kristoffer Semelka, Austin
 */

#ifndef _KALMAN_H_
#define _KALMAN_H_

void initFilter();
void updateFilter();

typedef enum navData_fields {pos_x, pos_y, vel_x, vel_y, heading} navData_field;

typedef float* navData;

/*
typedef struct s_navData
{
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	float heading;
} navData;
*/

navData* getNavData();

#endif
