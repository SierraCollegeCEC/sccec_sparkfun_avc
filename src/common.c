/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Common header file for any definitions we might need in each module
 * Includes utility and math functions.
 * Authors: James Smith, Kristoffer Semelka
 */
#include "common.h"
#include <math.h>

float **parseto2DArray(char *string)
{	/* To-do.
	 * Parses a two-dimensional array out of a string of values. 
	 */
	return (float**) 0;
}

float *parsetoArray(char *string)
{	/* To-do
	 * Parses an array out of a string of values.
	 */
	return (float*) 0;
}

float findAngle(vector vec)
{	/* Takes a vector, and finds its angle with respect to the positive x-axis. */
	float angle =  180 * ( atan2( ( -1 * vec.y ) , vec.x ) / M_PI );
	return angle;
}

float norm (vector point)
{
	/* standard norm of a vector */

	return sqrt(pow(point.x, 2) + pow(point.y, 2));
}

vector diff(vector P1, vector P0)
{
	/* standard vector difference P1 - P0 */
	
	vector difference;
	difference.x = P1.x - P0.x;
	difference.y = P1.y - P0.y;

	return difference;
}
