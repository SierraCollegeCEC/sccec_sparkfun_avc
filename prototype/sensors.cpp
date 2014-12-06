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
 *           │ ▄▄▄   ▀▀▀▀   ▄▄▄ │ 
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
#include <math.h>
#include <Arduino.h>

/* Debugging Flag */
const int DEBUGGING = 0;

navData NavData;

/* HM55B Commands */
int hm55b_shift_in( int bits )
{
	pinMode( DATA, INPUT );
	int result = 0;
	
	if ( DEBUGGING )
	{
		Serial.print( "Shifting in ");
		Serial.print( bits );
		Serial.print( " bits. Recieved: " );
	}
	
	for( int i = bits; i >= 0; i-- )
	{
		digitalWrite( CLK, HIGH );
		delayMicroseconds(1);
		if ( digitalRead( DATA ) == HIGH )
		{
			result = ( result << 1 ) + 1;
			if ( DEBUGGING )
			{
				Serial.print( "1" );
			}
		}
		else
		{
			result = result << 1;
			if ( DEBUGGING )
			{
				Serial.print( "0" );
			}
		}
		digitalWrite( CLK, LOW );
		delayMicroseconds(1);
	}
	if ( DEBUGGING )
	{
		Serial.print( "\n" );
	}
	
	if ( ( result & 1 << 11 ) == 1 << 11 )
	{
		result = ( B11111000 << 8 ) | result;
	}
	return result;
}

void hm55b_shift_out ( int value, int bits )
{
	if ( DEBUGGING )
	{
		Serial.print( "Shifting out " );
		Serial.print( bits );
		Serial.print( " bits. Sending " ); 
		Serial.print( value );
		Serial.print(" Sent: " );
	}
	for( int i = bits; i >= 0; i-- )
	{
		digitalWrite( CLK, LOW );
		if ( ( value & 1 << i ) == ( 1 << i ) )
		{
			digitalWrite( DATA, HIGH );
			if ( DEBUGGING )
			{
				Serial.print( "1" );
			}
		}
		else
		{
			digitalWrite( DATA, LOW );
			if ( DEBUGGING )
			{
				Serial.print( "0" );
			}
		}
		digitalWrite( CLK, HIGH );
		delayMicroseconds(1);
	}
	if ( DEBUGGING )
	{
		Serial.print( "\n" );
	}
	
}

void hm55b_reset()
{
	pinMode( DATA, OUTPUT );
	digitalWrite( EN, LOW );
	hm55b_shift_out( B0000, 3 );
	digitalWrite( EN, HIGH);
	if ( DEBUGGING )
	{
		Serial.print( "Reset HM55B\n" );
	}
}

int hm55b_read()
{
	pinMode( DATA, OUTPUT );
	digitalWrite( EN, LOW );
	hm55b_shift_out( B1000, 3 );
	digitalWrite( EN, HIGH );
	delay(40);
	
	pinMode( DATA, OUTPUT );
	digitalWrite( EN, LOW );
	hm55b_shift_out( B1100, 3 );
	int read_command_result = hm55b_shift_in( 3 );
	
	int x_value = hm55b_shift_in( 11 );
	int y_value = hm55b_shift_in( 11 );
	
	digitalWrite( EN, HIGH );
	
	int angle = 180 * ( atan2( ( -1 * y_value ) , x_value ) / M_PI );
	if ( DEBUGGING )
	{
  
		Serial.print( "Angle: " );
		Serial.print( angle );
		Serial.print( "\n" );
	}
	return angle;
}


void initSensors()
{
	pinMode( EN, OUTPUT );
	pinMode( CLK, OUTPUT );
	pinMode( DATA, INPUT );

	hm55b_reset();
	int filter_array[10];
}

void updateSensors()
{
  float heading = (float)hm55b_read();
  if( heading < 0 )
  {
    heading = abs(heading);
  }
  else
  {
    heading = 360.0f - heading;
  }
  Serial.println( heading );
  NavData.heading = heading;
}

navData* getNavData()
{
  return &NavData;
}
