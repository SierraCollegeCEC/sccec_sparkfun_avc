/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Prototype Project
 */
#include "common.h"
#include "sensors.h"
#include "nav.h"
#include "motion.h"
#include <Servo.h>

void setup()
{
  Serial.begin( 9600 );
  initSensors();
  initNav();
  initMotion();
}

void loop()
{
  updateSensors();
  updateNav();
  updateMotion();
  
  delay( dt );
}

