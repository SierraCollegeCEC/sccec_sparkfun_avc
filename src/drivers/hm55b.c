#include "sensors.h"

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

pvoid mag_reset()
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

vector mag_read()
{

	vector heading;
	pinMode( DATA, OUTPUT );
	digitalWrite( EN, LOW );
	hm55b_shift_out( B1000, 3 );
	digitalWrite( EN, HIGH );
	delay(40);
	
	pinMode( DATA, OUTPUT );
	digitalWrite( EN, LOW );
	hm55b_shift_out( B1100, 3 );
	int read_command_result = hm55b_shift_in( 3 );
	
	heading.x = hm55b_shift_in( 11 );
	heading.y = hm55b_shift_in( 11 );
	
	digitalWrite( EN, HIGH );
	
	if ( DEBUGGING )
		{
			float angle =  180 * ( atan2( ( -1 * heading.y ) , heading.x ) / M_PI );
			Serial.print( "Angle: " );
			Serial.print( angle );
			Serial.print( "\n" );
		}
	return heading;
}
