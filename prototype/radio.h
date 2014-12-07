/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Generic Radio API
 * This is a generic interface for a radio module. Each different chip would need to implement these functions
 * as well as any internal functions it would need. This API assumed point to point radio, and does not support
 * a network of radios.
 * 
 * Authors: James Smith
 */

#ifndef _RADIO_H_
#define _RADIO_H_
#include <stdint.h>

typedef struct s_Radio
{
	uint8_t pinCE;
	uint8_t pinSS;
} Radio;

void radioSetup( Radio* radio, uint8_t pinCE, uint8_t pinSS );
void radioSend( Radio* radio, uint8_t* buffer, uint8_t length );
bool radioHasData( Radio* radio );
void radioRecieve( Radio* radio, uint8_t* buffer, uint8_t length );

#endif
