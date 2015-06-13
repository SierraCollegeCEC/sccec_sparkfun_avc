/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Common header file for any definitions we might need in each module
 * Authors: James Smith
 */

#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdint.h>

/*** Configuration ***/
/* These are the primary configurable values. */

const float ** map = 
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
}

// Since this is const it doesnt need to be extern.
// Run the code at 10Hz for now
const float dt = 100.0f;

/*
 * I/O Pin Definitions
 * These arent needed in all files but it's nice to have them in one place so we can avoid pin collisions
 */
const uint8_t escPin = 10;
const uint8_t strPin = 9;
const uint8_t CLK = 7;
const uint8_t EN = 11;
const uint8_t DATA = 12;

const int spiSCK = 13;
const int spiMISO = 12;
const int spiMOSI = 11;
const int spiRF24 = 6; // SS
const int spiRF24_mode = 7; // CE

/*** Typedefs ***/

/* Vector Datatype */
typedef struct s_vector {
  float x;
  float y;
} vector;

/* Booleans */
typedef uint8_t bool;
/* Prototypes */
float **parseto2DArray(char *string);
float *parsetoArray(char *string);
float findAngle(vector vec);
float norm (vector point);
vector diff(vector P1, vector P0)

#endif

