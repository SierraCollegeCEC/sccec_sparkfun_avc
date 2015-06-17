/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "bezier.h"
#include "telemetry.h"

#define POINTS_PER_CURVE 4 /* These are quadratic bezier curves. */

typedef struct s_curve
{
	vector points[POINTS_PER_CURVE]; /* A curve is an array of control points. */
	struct s_curve *next; /* A curve knows where the next segment is */
} curve;

curve *current; /* Current curve. */
curve *previous; /* In case we need to go back. */
float lastT;
/* For finding closest point on curve, keep track of last t we used and assume
   t is increasing and minimally changing. See findDesiredPos(currentPos).
*/

/* Forward declarations */
void setMap(float*);
curve* makeMapFromArray(float*);
vector findDesiredPos(vector);
float findClosestT(vector);
float offset(float);
vector bezier(float);
float bezierHelper(float, float, float, float, float);
void setMapHandler(char*, char*);

void initMap(float *mapValues)
{
	setMap(mapValues);
	addTelemetryEventHandler(setMapHandler);
}

void setMap(float *mapValues)
{	/* Takes an array of control points representing a map,
 	 * and resets the map accordingly. Also resets the current node
 	 * and the "lastT" values.
 	 *
 	 * A valid array of control points should be formatted accordingly:
 	 * Map: [Curve, Curve, Curve, . . . ]
 	 * Curve: [Point, Point, Point, Point . . . ]
 	 * Point: [x, y]
 	 * Thus, a map should have a total size of 2 * POINTS_PER_CURVE * SEGMENTS * sizeof(float).
 	 */

 	current = makeMapFromArray(mapValues);
 	previous = current;
 	lastT = 0.f;
}

float getDesiredHeading(vector currentPos)
{
	float desiredHeading;
	vector desiredPos;

	desiredPos = findDesiredPos(currentPos);
	desiredHeading = findAngle(diff(desiredPos, currentPos));
	
	return desiredHeading;
}

vector findDesiredPos(vector currentPos)
{
	/* Given where we are, where do we want to eventually be? Rabbit-chase.*/

	float tOfClosest, tDesired;
	vector desiredPos;
	
	tOfClosest = findClosestT(currentPos); /* Where are we close to? */
	tDesired = tOfClosest + offset(tOfClosest); /* Offset by some ammount */

	desiredPos = bezier(tDesired); /* Find out where we want to be with new t. */

	return desiredPos;
}

float findClosestT(vector currentPos)
{
	/* This function determines from a current position the nearest point on the
	   curve.

	1. We find the distance we are from where we last were on the curve. We
	set that as the distance we're at.
	2. We then try two points around a neighborhood of that t. t + I is more likely
	than t - I (why would we go backwards?).
	3. If the distance to +/- I is more than the distance to t, cut I in half.
	4. If the distance to +/- I is less, go to whichever one worked.
	5. Try again until I < .01 (3 iterations). Subject to tuning.

	Return whatever distance we get. Make sure to save last value of t.

	*/
	float t, right, left, interval, distancet, distanceLeft, distanceRight;

	t = lastT; /* Our last t is a pretty good guess. */
	
	interval = .10f; /* Not as great of a guess */
	

	while(interval > .01)
	{
		/* Set neighborhood. */
		left = t - interval / 2; 
		right = t + interval / 2;

		/* Compute distances. */
		distancet = norm(diff(bezier(t), currentPos));
		distanceLeft = norm(diff(bezier(left), currentPos));
		distanceRight = norm(diff(bezier(right), currentPos));

		/* Compare distances. Set to left and right, OR halve interval. */
		
		if ( ( distanceLeft < distancet ) && ( distanceLeft < distanceRight ) )
		{
			t = left;
		}
		else if ( ( distanceRight < distancet ) && ( distanceRight < distanceLeft ) )
		{
			t = right;
		}
		else
		{
			interval = interval / 2;
		}
	}

	/* If we're nearing the end of a curve, go on to the next one. */
	if ( (1 - t) < .95 )
	{
		previous = current;
		current = current->next;
		t = 0;
	}
	
	lastT = t; /* Start at this point later, to save iterations. */
	
	return t;
}

float offset(float tOfClosest)
{
	/* INCOMPLETE. This function should dynamically determine what value of "t"
	   we should be chasing. I did some thinking, and I think that this should
	   be a function of the proportion of the curve we wish to cover in that cycle
	   divided by the current desired velocity (distance / velocity = dt).

	   The velocity we wish to go should also be inversely proportional to the
	   average curvature over the upcoming distance, and proportional to the
	   arclength that we wish to traverse over that same distance.
	   (ds/dt = constant * distance /mean-kappa). As curvature approaches zero,
	   speed becomes infinite (value clamped by controller).
	   As curvature increases, speed decreases.
	   As "desired covered distance" decreases, then so does speed.
	   The constant would be tuned.

	   Setting to dummy value for now. No idea how this will perform.
	*/
	/* Inch 1/100th of a t */
	return 0.01f;
}

vector bezier(float t)
{
	/* Takes some parameter t between 0 and 1 and a set of control points and
	 * gives you the corresponding point on the curve defined by those points.
	 * This is a *cubic* bezier curve. Extra complexity is no problem;
	 * the math takes about 142 cycles to compute, excluding fetches.
	 */

	vector point;
	vector *cpoints = current->points;

	/* Crank out the x, then the y */
	point.x = bezierHelper(t, cpoints[0].x, cpoints[1].x, cpoints[2].x, cpoints[3].x);
	point.y = bezierHelper(t, cpoints[0].y, cpoints[1].y, cpoints[2].y, cpoints[3].y);

	return point;
}

curve* makeMapFromArray(float* arr)
{
	/* Makes a linked list from an array of values. */
	/* Todo: Implement. */
	return (curve*) 0;
}

float bezierHelper(float t, float P0, float P1, float P2, float P3)
{
	/* grunt work of function */

	float val  = P0 * pow(1 - t, 3) + P1 * 3 * pow(1-t, 2) * t + P2
	             * 3 * (1-t) * pow(t, 2) + P3 * pow(t, 3);
	
	return val;
}

/* Event Handlers */

void setMapHandler(char *key, char* mapValues)
{
	if( strcmp(key, "setMap") == 0 )
	{
		float* mapArray = parseToArrayOfUnknownSize(mapValues);
		setMap(mapArray);
		free(mapArray);
	}
}
