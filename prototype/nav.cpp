/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Navigation Subsystem
 * Authors: Kristoffer Semelka
 */
#include "common.h"
#include "nav.h"
#include "sensors.h"
#include <stdlib.h>
#include <math.h>
#include "pid.h"
#include "bezier.h"

motionData MotionData;

void initNav()
{
  /*
    Nothing goes here yet.
    For future milestones, include Bezier curve data here.
  */
}

float findCorrection(float current, float desired)
{
  /*
    Takes the current heading and desired heading, and
    finds the (shortest) difference between the two.
  */
  float tempAngle, correction;
  
  tempAngle = desired - current;

  if ( abs(tempAngle) > 180)
    {
      /*
       Use the other angle, in the other direction
     */

      if ( tempAngle > 0)
        {
          correction = (360.0f - tempAngle) * -1;
        }
      else
        {
          correction = (-360.0f - tempAngle) * -1;
        }
    }

  else
    { correction = tempAngle; }
  
  return correction;
}

void updateNav()
{
  /*
    This function:
     - gets the navigation data from sensors (heading, position);
     - gets desired values from Bezier Curves (position);
     - finds difference between desired position and real position
       to obtain desired heading;
     - find difference between desired heading and real heading
     - applies PID to heading differences;
     - creates instructions for motion module;
     - and pushes instructions to MotionData
       on every update.
  */
  navData* NavData;
  float desiredHeading;
  float headingDiff;
  float adjustedHeading;

  NavData = getNavData();
  desiredHeading = getDesiredHeading();
  /* This NEEDS to accept NavData->pos as an argument, but I can't till
     it's defined in sensors.h. Returning stub value in the meantime for
     purposes of this commit.
  */

  headingDiff = findCorrection(NavData->heading, desiredHeading);
  adjustedHeading = pidAdjust(headingDiff);
  
  MotionData.heading = adjustedHeading;
  
}

motionData* getMotionData()
{
  return &MotionData;
}
