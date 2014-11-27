/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */
#include <SPI.h>
#include "common.h"
#include "telemetry.h"
#include "nRF24L01.h"

nRF24L01 radio;

void initTelemetry()
{
  SPI.begin();
  rf24Setup( &radio, 9, 10 );
}

void updateTelemetry()
{
}

