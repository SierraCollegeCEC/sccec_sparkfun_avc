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

uint8_t data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
				   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

void initTelemetry()
{
	Serial.println( "Booting" );
	radioSetup( &radio, spiRF24_mode, spiRF24 );
	//radioSend( &radio, temp, 32 );
}

void updateTelemetry()
{
	if( PTX )
	{
		radioSend( &radio, data, 32 );
	}
	else
	{
		if( radioHasData( &radio ) )
		{
			Serial.println( "Recieving" );
			radioRecieve( &radio, data, 32 );

			Serial.print("Got buttons ");
			int i = 32;
			for( int i = 0; i < 32; ++i )
			{
				if ( data[i] )
				{
					Serial.print( data[i] );
					Serial.print( " " );
				}
			}
			Serial.println();
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
