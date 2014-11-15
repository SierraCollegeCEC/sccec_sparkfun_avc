/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Prototype Project
 */
#include "sensors.h"
#include "nav.h"
#include "motion.h"

void setup()
{
  initSensors();
  initNav();
  initMotion();
}

void loop()
{
  updateSensors();
  updateNav();
  updateMotion();
}

