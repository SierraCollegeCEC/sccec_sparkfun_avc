/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Navigation Subsystem
 * Authors: Kristoffer Semelka
 */
#include "common.h"
#include "nav.h"
#include "sensors.h"

motionData MotionData;

void initNav()
{
  /*
    Nothing goes here yet.
    For future milestones, include Bezier curve data here.
  */
}

void updateNav()
{
  /* This function:
     - gets the navigation data from sensors;
     - "gets" (constant for now) desired heading;
     - finds difference;
     - applies PID (just P for now);
     - creates instructions for motion module;
     - and pushes instructions to MotionData
     on every update.
  */
  navData* NavData;
  float desiredHeading;
  float headingDiff;
  
  NavData = getNavData();
  desiredHeading = M1HEADING;

  headingDiff = findDifference(NavData->heading, M1HEADING);

  MotionData.heading = headingDiff;
  
}

motionData* getMotionData()
{
  return &MotionData;
}
