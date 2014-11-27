/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: James Smith
 */
#include "nRF24L01.h"

void rf24Setup( nRF24L01* radio, uint8_t pinCE, uint8_t pinCSN )
{
  if( radio )
  {
    radio->pinCE = pinCE;
    radio->pinCSN = pinCSN;
  }
}
