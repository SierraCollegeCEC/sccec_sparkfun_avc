/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Sensors Subsystem
 * Authors: Alex Norell, Kosuke Fujimoto
 * 
 * Sensor: Hitachi HM55B
 * Board: Paralax HM55B REV B 29123
 *
 *           ╒══════════════════╕
 *           │ ▀▀▀   ▄▄▄▄   ▀▀▀ │ 
 *           │▄███   ▀▀▀▀   ███▄│ 
 *    Din (1)│██▄█▌        ▐█▄██│(6) Vcc
 *           │████▌▐██████▌▐████│ 
 *   Dout (2)│██▄█▌▐██████▌▐█▄██│(5) /EN
 *           │████▌▐██████▌▐████│ 
 *    GND (3)│██▄█▌▐██████▌▐█▄██│(4) CLK
 *           │ ▄▄▄    ▄▄▄   ▄▄▄ │
 *           ╘══════════════════╛
 * 
 * PIN	Name	Function
 * 1	Din		Serial data input
 * 2 	Dout	Serial data output
 * 3   	GND		Ground -> 0V
 * 4  	CLK		Synchronous clock input
 * 5	/EN		Active-low device enable
 * 6	Vcc		+5V power input
 *
 */
#include "common.h"
#include "sensors.h"

navData NavData;

/* Pins */
const byte CLK = ;
const byte EN = ;
const byte DATA =;

void initSensors()
{
	

}

void updateSensors()
{
}

navData* getNavData()
{
  return &NavData;
}
