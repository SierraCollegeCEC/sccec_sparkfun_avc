/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * nRF24L01+ API
 * Radio module used for telemetry information. This API uses SPI to communicate with 
 * the nRF24L01+ radio module
 * 
 * TODO: Remove Arduino dependent implementations
 * Authors: James Smith
 */
#include <SPI.h>
#include "common.h"
#include "nRF24L01.h"

 #define BIT(offset) (1<<offset)

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

		/* The datasheet mentions needed a 1.5ms startup time for the crystal, give it a little extra time */
		delay( 5 );

		/* Automatically re-transmit packets */
		rf24WriteRegister( radio, SETUP_RETR, (B0100 << ARD) | (B1111 << ARC) );
	}
}

uint8_t rf24ReadRegister( nRF24L01* radio, uint8_t reg )
{
	int value = 0;
	if( radio )
	{
		digitalWrite( radio->pinSS, LOW );
		/* TODO: Does this return an error code? */
		SPI.transfer( R_REGISTER | (REGISTER_MASK & reg) );
		value = SPI.transfer( 0xFF );
		digitalWrite( radio->pinSS, HIGH );
	}
	return value;
}

void rf24WriteRegister( nRF24L01* radio, uint8_t reg, uint8_t value )
{
	if( radio )
	{
		digitalWrite( radio->pinSS, LOW );
		SPI.transfer( W_REGISTER | (REGISTER_MASK * reg) );
		SPI.transfer( value );
		digitalWrite( radio->pinSS, HIGH );
	}
}

void rf24WritePayload( nRF24L01* radio, uint8_t* buffer, uint8_t length )
{
	if( radio )
	{
		rf24WriteRegister( radio, CONFIG, rf24ReadRegister( radio, CONFIG ) | BIT(PWR_UP) & ~(BIT(PRIM_RX)) );
		
		uint8_t padding = PAYLOAD_SIZE - length;
		digitalWrite( radio->pinSS, LOW );
		SPI.transfer( W_TX_PAYLOAD );
		while( length > 0 )
		{
			SPI.transfer( *buffer );
			buffer++;
			length--;
		}
		while( padding > 0 )
		{
			SPI.transfer( 0x00 );
			padding--;
		}
		digitalWrite( radio->pinSS, HIGH );

		Serial.println( rf24ReadRegister( radio, CONFIG ), BIN );
	}
}

void rf24ReadPayload( nRF24L01* radio, uint8_t* buffer, uint8_t length )
{
	if( radio )
	{
		uint8_t padding = PAYLOAD_SIZE - length;
		digitalWrite( radio->pinSS, LOW );
		SPI.transfer( R_RX_PAYLOAD );
		while( length > 0 )
		{
			*buffer = SPI.transfer( 0xFF );
			buffer++;
			length--;
		}
		while( padding > 0 )
		{
			SPI.transfer( 0x00 );
			padding--;
		}
		digitalWrite( radio->pinSS, HIGH );
	}
}

void rf24Send( nRF24L01* radio, uint8_t* buffer, uint8_t length )
{
	if( radio )
	{
	}
}

void rf24Recieve( nRF24L01* radio, uint8_t* buffer, uint8_t length )
{
	if( radio )
	{
	}
}
