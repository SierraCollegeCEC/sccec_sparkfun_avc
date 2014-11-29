/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * nRF42L01+ API
 * Radio module used for telemetry information. This API uses SPI to communicate with 
 * the nRF42L01+ radio module
 * 
 * TODO: Remove Arduino dependent implementations
 * Authors: James Smith
 */
#include <SPI.h>
#include "common.h"
#include "nRF24L01.h"

uint8_t data[2];

void rf24Setup( nRF24L01* radio, uint8_t pinCE, uint8_t pinSS )
{
  if( radio )
  {
    radio->pinCE = pinCE;
    radio->pinSS = pinSS;
    radio->mode = RXMODE;
    
    pinMode( spiSCK, OUTPUT );
    pinMode( spiMISO, INPUT );
    pinMode( spiMOSI, OUTPUT );
    pinMode( radio->pinCE, OUTPUT );
    pinMode( radio->pinSS, OUTPUT );
    
    SPI.setBitOrder( MSBFIRST );
    SPI.setDataMode( SPI_MODE0 );
    SPI.setClockDivider( SPI_CLOCK_DIV2 );
    
    /* 1 = TX, 2 = RX */
    digitalWrite( radio->pinCE, HIGH );
    digitalWrite( radio->pinSS, HIGH );
    
    SPI.begin();
    
    digitalWrite( radio->pinSS, LOW );
    data[0] = SPI.transfer( CONFIG );
    data[1] = SPI.transfer( 0x00 );
    digitalWrite( radio->pinSS, HIGH );
    
    Serial.println( data[1], BIN );
  }
}
