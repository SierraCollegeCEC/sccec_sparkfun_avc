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

uint8_t temp[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

void initTelemetry()
{
  rf24Setup( &radio, spiRF24_mode, spiRF24 );
  rf24WritePayload( &radio, temp, 32 );
  Serial.println( rf24ReadRegister( &radio, FIFO_STATUS ), BIN );
  Serial.println( rf24ReadRegister( &radio, CONFIG ), BIN );
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
      rf24Send( &radio, &idx, 1 );
      rf24Send( &radio, &toSend, 1 );
      
      for( int i = 0; i < toSend; ++i )
      {
        /* TODO: Bundle all telemetry data into larger packets. Should this happen here or on the radio side? */
        rf24Send( &radio, &telemetryData[idx][(i + telemetryIndexLast[idx]) % TELEMETRY_MAX], 1 );
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
