/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */
#include <SPI.h>
#include "common.h"
#include "telemetry.h"
#include "radio.h"

#define ENABLE_TELEMETRY

#ifdef ENABLE_TELEMETRY
Radio radio;
uint8_t telemetryData[TELEMETRY_TYPES][TELEMETRY_MAX];
uint8_t telemetryIndex[TELEMETRY_TYPES];
uint8_t telemetryIndexLast[TELEMETRY_TYPES];

/* TODO: This is only used on the RX side, which is not currently implemented
const char* telemetryNames[TELEMETRY_TYPES] = 
{
	"Motion: Turning",
	"Navigation: Error",
	"Sensors: Magnetometer"
};
*/

uint8_t temp[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
				   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

void initTelemetry()
{
	radioSetup( &radio, spiRF24_mode, spiRF24 );
	radioSend( &radio, temp, 32 );
}

void updateTelemetry()
{
	//Serial.println( rf24ReadRegister( &radio, FIFO_STATUS ), BIN );
	for( uint8_t idx = 0; idx < TELEMETRY_TYPES; ++idx )
	{
		uint8_t toSend = 0;
		if( telemetryIndex[idx] < telemetryIndexLast[idx] )
		{
	 		toSend = (TELEMETRY_MAX - telemetryIndexLast[idx]) + telemetryIndex[idx];
		}
		else
		{
			toSend = telemetryIndex[idx] - telemetryIndexLast[idx];
		}

		if( toSend > 0 )
		{
			radioSend( &radio, &idx, 1 );
			radioSend( &radio, &toSend, 1 );

			for( int i = 0; i < toSend; ++i )
			{
				/* TODO: Bundle all telemetry data into larger packets. Should this happen here or on the radio side? */
				radioSend( &radio, &telemetryData[idx][(i + telemetryIndexLast[idx]) % TELEMETRY_MAX], 1 );
			}

			telemetryIndexLast[idx] = telemetryIndex[idx];
		}
	}
}

void addTelemetryData( TelemetryType type, uint8_t value )
{
	telemetryData[type][telemetryIndex[type]] = value;

	telemetryIndex[type]++;
	if( telemetryIndex[type] >= TELEMETRY_MAX )
	{
		telemetryIndex[type] = 0;
	}
}
#else
void initTelemetry()
{
}
void updateTelemetry()
{
}
void addTelemetryData( TelemetryType type, uint8_t value )
{
}
#endif
