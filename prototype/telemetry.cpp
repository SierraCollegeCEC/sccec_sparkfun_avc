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
  rf24Setup( &radio, spiRF24_mode, spiRF24 );  
}

void updateTelemetry()
{
}

