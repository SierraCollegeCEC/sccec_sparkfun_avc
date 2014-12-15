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
 * Vector Datatype (will handle 2D and 3D algorithms).
 * If you're using only 2 dimensions, only use 2 spots!
 */

typedef float vector[3];
const uint8_t dimensions = 2;

/*
 * Simply use "vector" as a type, and access it as an array,
 * e.g. vector[i]
 * Be careful! Arrays are passed by reference in C, so do not
 * write " &vector " in your code when you are trying to share vectors,
 * since a pointer is already given.
 *
 * If we ever use this code for a quadcopter, simply change
 * "dimensions" to 3. Remember to reference dimensions in code
 * that iterates through vectors.
 */

#endif

