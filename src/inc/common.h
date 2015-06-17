/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Common header file for any definitions we might need in each module
 * Authors: James Smith
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdlib.h>

/*** Configuration ***/
/* These are the primary configurable values.
 * C's linkage defaults are different than C++'s.
 * Declare all global constants static to avoid
 * redefinitions.
 */
static float map[] =
{
	/* Two coordinates make a point.
	   Four points make a curve. */

	/* x     y */
	0.1f, 0.1f,
	0.2f, 0.2f,
	0.3f, 0.3f,
	0.4f, 0.4f,

	0.5f, 0.5f,
	0.6f, 0.6f,
	1.2f, 1.2f,
	2.0f, 2.0f
};

// Run the code at 10Hz for now
static const float dt = 100.0f;
static const float M_PI = 3.14159f;

/*
 * I/O Pin Definitions
 * These arent needed in all files but it's nice to have them in one place so we can avoid pin collisions
 */
static const uint8_t escPin = 10;
static const uint8_t strPin = 9;
static const uint8_t CLK = 7;
static const uint8_t EN = 11;
static const uint8_t DATA = 12;

static const int spiSCK = 13;
static const int spiMISO = 12;
static const int spiMOSI = 11;
static const int spiRF24 = 6; // SS
static const int spiRF24_mode = 7; // CE

/*** Typedefs ***/
/* Formatting */
typedef enum formats {BIN, HEX, DEC} format;

/* Vector Datatype */
typedef struct s_vector {
  float x;
  float y;
} vector;

/* Prototypes */
float findAngle(vector vec);
float norm (vector point);
vector diff(vector, vector);
uint8_t max(uint8_t, uint8_t);
uint8_t min(uint8_t, uint8_t);

/* Parsing */
float *parseToArray(char *string, uint8_t length);
float *parseToArrayOfUnknownSize(char *string);

/* These two functions will ONLY accept the commas being
 * right after the numbers, i.e.
 * "10.0f, 10E2, .0002, 20" is a valid string, but
 * "10.0f , 20E20 , 2" is not. The last element
 * should not have a comma, either.
 */

/* UNIMPLEMENTED PROTOTYPES */
void delay(uint8_t);
void delayMicroseconds(uint8_t);
uint32_t millis();
/* serial port functions; should use calls from spi.h */
void print(char*);
void printChar(char);
uint8_t available();
void readBytes(char*, uint8_t);
void printFormat(uint8_t, format);
char transmitByte(char byte); /* See: spiTransmitData(...) in spi.h */
#define HIGH 1
#define LOW 0
void digitalWrite(uint8_t,uint8_t);
void spiSetBitOrder( uint8_t );
void spiSetDataMode( uint8_t );
void spiSetClockDivider( uint8_t );
#define MSBFIRST 0
#define SPI_MODE0 0
#define SPI_CLOCK_DIV4 0
#define INPUT 0x00
#define OUTPUT 0x00
void pinMode(uint8_t, uint8_t);
#define _BV(val) ( 1<<(val) )

#endif

