/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Bezier Curve Implementation and Corresponding Utilities
 * Authors: Kristoffer Semelka
 */

#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "bezier.h"

#define POINTS_PER_CURVE 4

vector map[10][POINTS_PER_CURVE];
/* A map is an array of 4-tuple point-sets, representing curves.*/

vector * curve;
/* Current curve. A curve is a set of control points. points[i] represents the ith Point.*/

float lastT;
/* For finding closest point on curve, keep track of last t we used and assume
   t is increasing and minimally changing. See findDesiredPos(currentPos).
*/

void initMap(){
  curve = map[0];
  lastT = 0;
  /*
    This function should eventually set the map.
    Hopefully someone figures out our control points :)
    Unfortunately, this year they're not providing gps data.
    Have to use google maps.
  */
}

float bezierHelper(float t, float P0, float P1, float P2, float P3){
  /* grunt work of function */

  float val  = P0 * pow(1 - t, 3) + P1 * 3 * pow(1-t, 2) * t + P2
    * 3 * (1-t) * pow(t, 2) + P3 * pow(t, 3);
  
  return val;
}

vector bezier(float t){
  /* Takes some parameter t between 0 and 1 and a set of control points and
     gives you the corresponding point on the curve defined by those points.
     This is a *cubic* bezier curve. Extra complexity is no problem;
     the math takes about 142 cycles to compute, excluding fetches. */

  vector point;

  /* Crank out the x, then the y */
  point.x = bezierHelper(t, curve[0].x, curve[1].x, curve[2].x, curve[3].x);
  point.y = bezierHelper(t, curve[0].y, curve[1].y, curve[2].y, curve[3].y);

  return point;

}

float norm (vector point){
  /* standard norm of a vector */

  return sqrt(pow(point.x, 2) + pow(point.y, 2));
}

vector diff(vector P1, vector P0){
  /* standard vector difference P1 - P0
   Maybe this should be in common.cpp*/
  
  vector difference;
  difference.x = P1.x - P0.x;
  difference.y = P1.y - P0.y;

  return difference;
}  

float offset(float tOfClosest){
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

float findClosestT(vector currentPos){
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
    curve += POINTS_PER_CURVE; /* I think that's how this works? */
    t=0;
  }
  
  lastT = t; /* Start at this point later, to save iterations. */
  
  return t;
}

vector findDesiredPos(vector currentPos){
  /* Given where we are, where do we want to eventually be? Rabbit-chase.*/

  float tOfClosest, tDesired;
  vector desiredPos;
  
  tOfClosest = findClosestT(currentPos); /* Where are we close to? */
  tDesired = tOfClosest + offset(tOfClosest); /* Offset by some ammount */

  desiredPos = bezier(tDesired); /* Find out where we want to be with new t. */

  return desiredPos;
}
  

vector getDesiredHeading(vector currentPos){
  
  vector desiredHeading;
  vector desiredPos;

  desiredPos = findDesiredPos(currentPos);

  desiredHeading = diff(desiredPos, currentPos);
  return desiredHeading;
}
