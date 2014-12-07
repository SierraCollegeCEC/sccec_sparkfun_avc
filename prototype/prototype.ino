/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Prototype Project
 */
#include <SPI.h>
#include "common.h"
#include "sensors.h"
#include "nav.h"
#include "motion.h"
#include "telemetry.h"

void setup()
{
	Serial.begin( 9600 );
	initSensors();
	initNav();
	initMotion();
	initTelemetry();
}

void loop()
{
	updateSensors();
	updateNav();
	updateMotion();
	updateTelemetry();

	delay( dt );
}

