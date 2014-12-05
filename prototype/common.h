/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Common header file for any definitions we might need in each module
 * Authors: 
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/* Platform definitions */
#define UNO
//#define MEGA
//#define MSP430

// Since this is const it doesnt need to be extern.
// Run the code at 10Hz for now
const float dt = 100.0f;

/*
 * I/O Pin Definitions
 * These arent needed in all files but it's nice to have them in one place so we can avoid pin collisions
 */

#ifdef UNO
const int spiSCK = 13;
const int spiMISO = 12;
const int spiMOSI = 11;
const int spiRF24 = 6; // SS
const int spiRF24_mode = 7; // CE
#else
#ifdef MEGA
const int spiSCK = 52;
const int spiMISO = 50;
const int spiMOSI = 51;
const int spiRF24 = 45; // SS
const int spiRF24_mode = 44; // CE
#else
#ifdef MSP430
const int spiSCK = 0;
const int spiMISO = 0;
const int spiMOSI = 0;
const int spiRF24 = 0; // SS
const int spiRF24_mode = 0; // CE
#endif /* MSP430 */
#endif /* MEGA */
#endif /* UNO */

#endif

