/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Motion Subsystem
 * Authors: Brad Morton
 */
#include "common.h"
#include "motion.h"
#include "nav.h"
#include <Servo.h>
#include <Arduino.h>

Servo servoStr;
Servo servoEsc;

void initMotion()
{
	servoStr.attach(strPin);
	servoEsc.attach(escPin);
}
void setVel(int v)
{
  servoEsc.write(v);
}
void setTurn(float turn)
{
  float radTurn=(turn/180)*2;
  servoStr.writeMicroseconds(1250+radTurn*(350));
}
int speedCalc(float v)
{
  int v1 = 0;
  if(v>0)
  {
    v1 = 90 - (v*90);
  }
  else
  {
    v1= 90 + (v*90);
  }
  return v1;
}
void updateMotion()
{
	float a=0.3;
	motionData *MotionData = getMotionData();
	float angle= MotionData->heading;
	
	int v;
	v=speedCalc(a);
	setVel(v);
	setTurn(angle);
}
