/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * nRF24L01+ API
 * Radio module used for telemetry information. This API uses SPI to communicate with 
 * the nRF24L01+ radio module
 * 
 * TODO: Remove Arduino dependent implementations
 * TODO: Iron out the bandwidth/retry stuff
 * TODO: Setup ADDR_WIDTH properly
 * TODO: move SPI init to a common area
 * Authors: James Smith
 */
#include <SPI.h>
#include "common.h"
#include "radio.h"

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5      5
#define DPL_P4      4
#define DPL_P3      3
#define DPL_P2      2
#define DPL_P1      1
#define DPL_P0      0
#define EN_DPL      2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

#define BIT(offset) (1<<offset)
const uint16_t FREQ_MIN = 2400;
const uint16_t FREQ_MAX = 2525;
const uint16_t FREQ = 2475;
/*
 * Address width values:
 * 0x01 - 3 bytes
 * 0x10 - 4 bytes
 * 0x11 - 5 bytes
 */
const uint8_t ADDR_WIDTH = 0x11;
const uint8_t ADDR[] = { 0xDE, 0xAD, 0xFF, 0x12, 0x34 };

const uint8_t PAYLOAD_SIZE = MAX_PACKET_SIZE;

/* Forward Declarations */
void writeRegister( Radio* radio, uint8_t reg, uint8_t value );
void writeRegister( Radio* radio, uint8_t reg, const uint8_t* value, uint8_t length );
uint8_t readRegister( Radio* radio, uint8_t reg );
void readRegister( Radio* radio, uint8_t reg, uint8_t* buffer, uint8_t length );
void setFrequency( Radio* radio, uint16_t freq );
void flushBuffers( Radio* radio );
void printDebug( Radio* radio );

/* 
 * Setup the nRF24L01+
 * The radio operates internally on a state machine. There are a couple of ways that we could
 * run the radio, but for now what we will do is power it up, put it into PRX mode (Primary Recieve)
 * and wait for any communication from the other radio. When it is time to send data to the other
 * radio, we will enter PTX mode (Primary Transmit), transmit the packet, and then return to PRX mode.
 * 
 * For now, we will run the radio in a configuration that only allows for bi directional communication
 * with one other radio module. It is possible to communicate with up to 6 modules with the nRF24L01+
 * but we will not need this feature for our project. In the future it would be nice to support this
 */
void radioSetup( Radio* radio, uint8_t pinCE, uint8_t pinSS )
{
	if( radio )
	{
		radio->pinCE = pinCE;
		radio->pinSS = pinSS;

		/* Pin Setup */
		pinMode( spiSCK, OUTPUT );
		pinMode( spiMISO, INPUT );
		pinMode( spiMOSI, OUTPUT );
		pinMode( radio->pinCE, OUTPUT );
		pinMode( radio->pinSS, OUTPUT );

		SPI.setBitOrder( MSBFIRST );
		SPI.setDataMode( SPI_MODE0 );
		SPI.setClockDivider( SPI_CLOCK_DIV4 );

		digitalWrite( radio->pinCE, LOW );
		digitalWrite( radio->pinSS, HIGH );

		SPI.begin();

		/*
		 * Automatically re-transmit packets
		 * ARD: Auto-Retransmit Delay: How long between retransmits (see datasheet for values)
		 * ARC: Auto-Retransmit Count: How many times to retransmit. 0-15
		 * TODO: Clean up these magic numbers
		 */
		writeRegister( radio, SETUP_RETR, (B0100 << ARD) | (15 << ARC) );

		/* RF SETUP */
		writeRegister( radio, RF_SETUP, readRegister( radio, RF_SETUP ) & ~(BIT(RF_PWR_LOW) | BIT(RF_PWR_HIGH)) | BIT(RF_PWR_HIGH) );

		setFrequency( radio, FREQ );

		/* Power up and go into PRX mode */
		writeRegister( radio, CONFIG, readRegister(radio,CONFIG) | BIT(PWR_UP) | BIT(PRIM_RX) );
		writeRegister( radio, STATUS, BIT(RX_DR) | BIT(TX_DS) | BIT(MAX_RT) );

		/* Flush FIFO buffers, this clears up bugs when simply resetting the device (without fully powering down) */
		flushBuffers( radio );

		//writeRegister( radio, SETUP_AW, (ADDR_WIDTH << AW) );
		writeRegister( radio, TX_ADDR, ADDR, 5 );

		/* Listening address */
		writeRegister( radio, RX_ADDR_P1, ADDR, 5);
		writeRegister( radio, EN_RXADDR, readRegister(radio,EN_RXADDR) | BIT(ERX_P1) );
		writeRegister( radio, RX_PW_P1, PAYLOAD_SIZE );

		digitalWrite( radio->pinCE, HIGH );
		
		/* 
		 * Startup times (from datasheet)
		 *   Power Down -> Standby: 4.5ms (max)
		 *   Standby -> TX/RX: 130μs (max)
		 * Let the device have enough time to get into whatever state it wants
		 */
		delay( 5 );
		printDebug( radio );
	}
}

/* 
 * Sending out data on the nRF24L01+ is fairly strait forward. First we write the payload, which is the
 * packet that will be sent over the radio. Then we turn off the PRX bit, which will tell the chip to 
 * check if there is a payload waiting to be sent. Since there is (we just wrote one), it will transmit 
 * the packet, and then wait for a response from the reciever. After that we return to PRX mode
 */
bool radioSend( Radio* radio, uint8_t* buffer, uint8_t length )
{
	if( radio )
	{
		/* Power up PTX */
		digitalWrite( radio->pinCE, LOW );
		writeRegister( radio, CONFIG, readRegister( radio, CONFIG ) & ~(BIT(PRIM_RX)) );
		delayMicroseconds( 130 );

		/* 
		 * Write the payload. They must be a specific size, specified by PAYLOAD_SIZE, so if we arent
		 * going to fill up a packet, just fill the rest of the payload with 0x00
		 */
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

		digitalWrite( radio->pinCE, HIGH );
		delayMicroseconds( 15 );
		digitalWrite( radio->pinCE, LOW );

		/* This is a bad way to do this but for now it will work. This should be in an interrupt */
		bool waitingForAck = true;
		uint32_t sent_at = millis();
		uint32_t timeout = 500;
		while( waitingForAck && (timeout > (millis() - sent_at)) )
		{
			//Serial.print( "Sending " );
			//Serial.println( readRegister( radio, STATUS ), BIN );
			waitingForAck = !(readRegister( radio, STATUS ) & (BIT(TX_DS) | BIT(MAX_RT)));
		}

		//Serial.print( "status: " );
		//Serial.println( readRegister( radio, STATUS ), BIN );

		/* Send is finished, go back to listening mode */
		bool result = (readRegister( radio, STATUS ) & BIT(TX_DS));
		writeRegister( radio, STATUS, BIT(RX_DR) | BIT(TX_DS) | BIT(MAX_RT) );
		
		/* Return to PRX */
		digitalWrite( radio->pinCE, HIGH );
		writeRegister( radio, CONFIG, readRegister( radio, CONFIG ) | _BV(PWR_UP) | _BV(PRIM_RX) );

		/* Since we are changing modes, it doesnt hurt to flush the buffers */
		flushBuffers( radio );
		
		delayMicroseconds( 130 );

		return result;
	}
	return false;
}

/* 
 * Just checks the status register to see if we have data ready to be read
 */
bool radioHasData( Radio* radio )
{
	if( radio )
	{
		uint8_t status;
		digitalWrite( radio->pinSS, LOW );
		status = SPI.transfer( NOP );
		digitalWrite( radio->pinSS, HIGH );

		bool result = (status & BIT(RX_DR));

		if( result )
		{
			/* Reset recieve bit */
			writeRegister( radio, STATUS, BIT(RX_DR) );
		}

		return result;
	}
	return false;
}

/* 
 * This should only be called after a call to radioHasData has returned true
 * Read the specified amount from the RX payload, put it into the desired buffer
 */
void radioRecieve( Radio* radio, uint8_t* buffer, uint8_t length )
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

/* 
 * nRF24L01 specific functions
 * All of these functions still accept the radio struct as a parameter
 * Because of this, I should technically be null checking them, but since they are the private interface
 * and I am already null checking them in the public interface, there should be no problems
 */

/* 
 * Writes to a register on the nRF24L01+
 * This can write to multi byte registers, just pass in the address to the bytes you want
 * to write and how many of them there are
 */
void writeRegister( Radio* radio, uint8_t reg, const uint8_t* value, uint8_t length )
{
	digitalWrite( radio->pinSS, LOW );
	SPI.transfer( W_REGISTER | (REGISTER_MASK & reg) );
	while( length > 0 )
	{
		SPI.transfer( *value );
		value++;
		length--;
	}
	digitalWrite( radio->pinSS, HIGH );
}

/* 
 * Specialized version of writeRegister that writes to 1 byte registers
 * Since most registers on the nRF24L01+ are 1 byte registers, this is more convenient
 * because we can just pass new register values by value
 */
void writeRegister( Radio* radio, uint8_t reg, uint8_t value )
{
	writeRegister( radio, reg, &value, 1 );
}

/*
 * Read from a register on the nRF24L01+
 * Returns the value as a byte
 * TODO: Support for reading from registers that are longer than 1 byte!
 */
void readRegister( Radio* radio, uint8_t reg, uint8_t* buffer, uint8_t length )
{
	digitalWrite( radio->pinSS, LOW );
	SPI.transfer( R_REGISTER | (REGISTER_MASK & reg) );
	while( length > 0 )
	{
		*buffer = SPI.transfer( 0xFF );
		buffer++;
		length--;
	}
	digitalWrite( radio->pinSS, HIGH );
}

/*
 * Read from a register on the nRF24L01+
 * Returns the value as a byte
 * TODO: Support for reading from registers that are longer than 1 byte!
 */
uint8_t readRegister( Radio* radio, uint8_t reg )
{
	uint8_t value = 0;
	readRegister( radio, reg, &value, 1 );
	return value;
}

/*
 * Set the RF channel frequency used by this chip.
 * The nRFLS01+ operates at a range from 2.400-2.525 GHz
 * freq - desired frequency in MHz. Expected range: [2400, 2525]
 */
void setFrequency( Radio* radio, uint16_t freq )
{
	/* First clamps freq between FREQ_MIN and FREQ_MAX, then takes the remainder after a division with FREQ_MIN, 
	   since this is what is needed in the register. We can use 1 byte for this because the range is 0-127 for the
	   register, but based on our clamping it will only range from 0-125 */
	freq = min( FREQ_MAX, max( freq, FREQ_MIN ) );
	uint8_t value = freq % FREQ_MIN;
	writeRegister( radio, RF_CH, value );
}

/*
 * Flush the IO buffers on the nRF24L01+
 * This is useful when switching between modes so that you can make sure the mode will change
 * (Some mode changes require empty buffers)
 */
void flushBuffers( Radio* radio )
{
	digitalWrite( radio->pinSS, LOW );
	SPI.transfer( FLUSH_RX );
	digitalWrite( radio->pinSS, HIGH );

	digitalWrite( radio->pinSS, LOW );
	SPI.transfer( FLUSH_TX );
	digitalWrite( radio->pinSS, HIGH );
}

void printDebug( Radio* radio )
{
	// Print status register
	uint8_t status;
	digitalWrite( radio->pinSS, LOW );
	status = SPI.transfer( NOP );
	digitalWrite( radio->pinSS, HIGH );
	Serial.print( "STATUS:     " );
	Serial.println( status, BIN );

	uint8_t addr[5] = {0,0,0,0,0};
	
	Serial.print( "RX_ADDR_P0: " );
	readRegister( radio, RX_ADDR_P0, addr, 5 );
	Serial.print( addr[0], HEX );
	Serial.print( addr[1], HEX );
	Serial.print( addr[2], HEX );
	Serial.print( addr[3], HEX );
	Serial.println( addr[4], HEX );
	
	Serial.print( "RX_ADDR_P1: " );
	readRegister( radio, RX_ADDR_P1, addr, 5 );
	Serial.print( addr[0], HEX );
	Serial.print( addr[1], HEX );
	Serial.print( addr[2], HEX );
	Serial.print( addr[3], HEX );
	Serial.println( addr[4], HEX );

	Serial.print( "TX_ADDR:    " );
	readRegister( radio, TX_ADDR, addr, 5 );
	Serial.print( addr[0], HEX );
	Serial.print( addr[1], HEX );
	Serial.print( addr[2], HEX );
	Serial.print( addr[3], HEX );
	Serial.println( addr[4], HEX );

	Serial.print( "RX_PW_P0:   " );
	Serial.println( readRegister( radio, RX_PW_P0 ), DEC );
	Serial.print( "RX_PW_P1:   " );
	Serial.println( readRegister( radio, RX_PW_P1 ), DEC );

	Serial.print( "EN_AA:      " );
	Serial.println( readRegister( radio, EN_AA ), BIN );
	Serial.print( "EN_RXADDR:  " );
	Serial.println( readRegister( radio, EN_RXADDR ), BIN );
	Serial.print( "RF_CH:      " );
	Serial.println( readRegister( radio, RF_CH ), BIN );
	Serial.print( "RF_SETUP:   " );
	Serial.println( readRegister( radio, RF_SETUP ), BIN );
	Serial.print( "CONFIG:     " );
	Serial.println( readRegister( radio, CONFIG ), BIN );
	Serial.print( "DYNPD:      " );
	Serial.println( readRegister( radio, DYNPD ), BIN );
}
