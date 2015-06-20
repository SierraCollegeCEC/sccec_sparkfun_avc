/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Telemetry Subsystem
 * Authors: James Smith
 */
#include "spi.h"
#include "common.h"
#include "telemetry.h"
#include "radio.h"

#define ENABLE_TELEMETRY

#ifdef ENABLE_TELEMETRY

#define TELEMETRY_DEBUG

Radio radio;

uint8_t packet[MAX_PACKET_SIZE];
uint8_t strBuffer[MAX_PACKET_SIZE+2];

/* Linked List containing all of the event handlers that will handle messages coming from the reciever */
typedef struct s_handlerNode
{
	telemetryEventHandler func;
	struct s_handlerNode* next;
} handlerNode;

handlerNode* handlerList;

void printPacket( uint8_t* data, uint8_t length )
{
	print( "[" );
	for( int i = 0; i < length; ++i )
	{
		printChar( (char)data[i] );
	}
	print( "]" );
}

#ifdef TELEMETRY_DEBUG
void evtHandler( char* key, char* val )
{
	print( "Recieved message, key: " );
	print( key );
	print( ", val: " );
	print( val );
	print("\n");
}
#endif

void initTelemetry()
{
	print( "Starting radio...\n" );
	radioSetup( &radio, spiRF24_mode, spiRF24 );

#ifdef TELEMETRY_DEBUG
	/* This is mostly here as an example */
	addTelemetryEventHandler( &evtHandler );
#endif
}

void updateTelemetry()
{
	/* Listens on the serial port for a string command to send to the reciever */
	if( available() > 0 )
	{
		/* This delay allows the serial buffer on the sending device to fill up. Pretty ugly hack
		   Alternative option: Build the packet up over several frames, wait for \n, then procede  */
		delay(15);

		/* First, send the length of the message so we know how much to decode. We could alternatively 
		   send this null-terminated, but its possible that null is a valid value for some telemetry 
		   transmissions */
		packet[0] = available() - 1;

		/* Read serial data into the memory address just after the packet length */
		char* dest = (char*)(packet + 1);
		readBytes( dest, packet[0] + 1 );
		
#ifdef TELEMETRY_DEBUG
		printPacket( packet, packet[0] + 1 );
		print("\n");
#endif
		
		/* Should use the result to print a status to the debug channel */
		bool result = radioSend( &radio, packet, packet[0] + 1 );
	}

	/* Listens for commands over the radio to send to the various subsystems.
	 * The system processes two types of commands for now:
	 * - key:value commands let you listen for a key in your event handler, and set some variable to a new value. 
	 * 		These values default to float but we may end up passing void* in the future
	 * - messages are effectively key:value commands with no value. Listen for the message and ignore the value 
	 * 		in the event handler, then call whatever method or set whatever value you want
	 */
	if( radioHasData( &radio ) )
	{
		radioRecieve( &radio, packet, 32 );

#ifdef TELEMETRY_DEBUG
		print( "Recieved: " );
		printPacket( packet, packet[0] + 1 );
		print("\n");
#endif

		/* Loop through the recieved packet looking for a colon character to determine if this is a key:value */
		uint8_t len = packet[0];
		uint8_t* start = packet + 1;
		uint8_t* idx = start;
		uint8_t* colon = NULL;
		while( (idx < (start + len)) && !colon )
		{
			if( *idx == ':' )
			{
				colon = idx;
			}
			idx++;
		}

		/* Write the key/message to the allocated space */
		int keyLen = len;
		if( colon )
		{
			keyLen = (colon - start);
		}
		memcpy( strBuffer, start, keyLen );
		/* Null-terminated strings */
		strBuffer[keyLen] = 0;

		char* key = (char*)strBuffer;
		char* val;

		/* If this is a key:value command, create a string for the value(s) and pass it along */
		if( colon )
		{
			int valLen = len - keyLen - 1;
			memcpy( strBuffer + keyLen + 1, start + keyLen + 1, valLen );
			strBuffer[keyLen + valLen + 1] = 0;
			val = (char*)(strBuffer + keyLen + 1);
		}

#ifdef TELEMETRY_DEBUG
		print( key );
		print("\n");
		print( val );
		print("\n");
		printPacket( strBuffer, 32 );
		print("\n");
#endif

		/* Run through the event handler list and call any events that have registered with us */
		handlerNode* node = handlerList;
		while( node != NULL )
		{
			node->func( key, val );
			node = node->next;
		}
	}
}

/*
 * Adds a function to the event handler linked list that is called when we recieve a message over the telemetry system
 */
void addTelemetryEventHandler( telemetryEventHandler func )
{
	/* C version: handlerNode* newNode = malloc( sizeof(handlerNode) ); */
	handlerNode* newNode = new handlerNode;
	if( newNode )
	{
		newNode->func = func;
		newNode->next = NULL;

		/* Insert into list */
		if( handlerList == NULL )
		{
			handlerList = newNode;
		}
		else
		{
			handlerNode* node = handlerList;
			while( node->next != NULL )
			{
				node = node->next;
			}
			node->next = newNode;
		}
	}
}

/*
 * Removes all instances of the given event handler from the event handler list
 */
void removeTelemetryEventHandler( telemetryEventHandler func )
{
	handlerNode* node = handlerList;
	handlerNode* last = NULL;

	while( node != NULL )
	{
		if( node->func == func )
		{
			if( last == NULL )
			{
				/* This is the head node - special case, just delete the head node and try again */
				handlerList = node->next;
				free( node );
				node = handlerList;
				continue;
			}
			else
			{
				last->next = node->next;
			}

			free( node );
			node = last;
		}
		
		last = node;
		node = node->next;
	}
}

#else
void initTelemetry()
{
}
void updateTelemetry()
{
}
void addTelemetryData( TelemetryType type, uint8_t value )
{
}
void addTelemetryEventHandler( telemetryEventHandler func )
{
}
void removeTelemetryEventHandler( telemetryEventHandler func )
{
}
#endif
