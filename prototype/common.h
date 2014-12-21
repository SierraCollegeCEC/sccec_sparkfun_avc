/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Common header file for any definitions we might need in each module
 * Authors: James Smith
 */

#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdint.h>

// Since this is const it doesnt need to be extern.
// Run the code at 10Hz for now
const float dt = 100.0f;

/*
 * Pin definitions
 */
const uint8_t escPin = 10;
const uint8_t strPin = 9;
const uint8_t CLK = 7;
const uint8_t EN = 11;
const uint8_t DATA = 12;

/*
 * Vector Datatype
 */

typedef struct s_vector {
  float x;
  float y;
} vector;

#endif

