/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Kalman Filter
 * Authors: Kristoffer Semelka, Austin
 */

#include "common.h"
#include "sensors.h"
#include "telemetry.h"

extern navData current;         /* The current state. */
static navData prediction;      /* A prediction of the next state. */
static sensorData measurement;  /* The GPS and Magnetometer data. */
static INSData INS;             /* Inertial Navigation System data, which
								 * consists of acceleromter and gyro data. */
static sensorData error;        /* Difference between error and measurement. */
extern KalmanGain kalman = {	/* Kalman gain, which is the linear correction
								 * factor applied to the error term. */
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01}
}        

/* Forward Declarations */
void predict();
void compare();
void correct();
float step(float, float, float);
float dot(float*, float*, int);
telemetryEventHandler setKalman;
telemetryEventHandler setKalmanRow;



void initFilter()
{
	/* Initializes the Kalman filter.	*/
	INS = getINSData();
	measurement = getSensorData();

	addTelemetryEventHandler(setKalman);
	addTelemetryEventHandler(setKalmanRow);
}

void updateFilter()
{
	/* Updates the filter a single time step, which updates
		 NavData.

		 The subroutines that are called make heavy use of
		 precalculated constants, so please read the
		 documentation!

		 First, the prediction stage of the Kalman filter is
		 run. This only requires the last best estimate. Then,
		 the sensor readings are retrieved from the Sensors
		 subsystem. Then the error between the predicted readings
		 and the actual readings is calculated. Finally, the
		 update stage of the Kalman filter runs, which applies
		 the Kalman gain to the error as an optimal correction
		 factor.
	 */

	predict(); /* Prediction */
	INS = getINSData();
	measurement = getSensorData();
	compare(); /* Comparison */
	correct(); /* Correction */
	
}

void predict()
{
	/* Apply a state transition model to current readings,
	 * adds displacement measured from INS unit, and assigns to
	 * prediction.
	 * 
	 * While this could be a terser matrix multiplication, I've
	 * chosen  to hand unroll the dot products because much of
	 * the state transition matrix is 0. (Sacrifice in
	 * readability is minimal.)
 	 */
	prediction[pos_x] = step(current[pos_x], current[vel_x],
													 INS[displ_x]);
	
	prediction[pos_y] = step(current[pos_y], current[vel_y],
													 INS[displ_y]);

	prediction[vel_x] = current[vel_x] + INS[vel_x];
	prediction[vel_y] = current[vel_y] + INS[vel_y];

	prediction[heading] = current[heading] + INS[yaw];
}

float step(float initial, float velocity, float displacement)
{
	return initial + velocity * dt + displacement; 
}

#define navData_iterate( var ) for( navData_field var = 0; var < NAV_DATA_FIELDS; var++ )
/* Declares and initializes a variable for use within a
	 loop over a navData array. You need to add braces!
*/

void compare()
{
	/* error = measured - predicted */

	navData_iterate(i){
		error[i] = measurement[i] - prediction[i];
	}
}

float dot( float *matrixRow, float *vector, int length ){
	/* Standard dot product */

	float accum = 0;

	for(int i = 0; i < length; i++){
		accum += matrixRow[i] * vector[i];
	}

	return accum;
}

void correct()
{
	/* current = Kalman * error

		 The kalman gain matrix is a constant defined elsewhere.
		 dotting a matrix[i] with a vector is the equivalent of
		 the dot product of the ith row of the matrix with the
		 vector; this is standard matrix multiplication.
	*/
	
	navData_iterate(i){
		current[i] = dot( kalman[i], error, NAV_DATA_FIELDS );
	}
}

navData* getNavData()
{
	return &current;
}

/* Telemetry Event Handlers */

void setKalman(char *key, char *valuesString)
{ 	/* Event handler for the "setKalman" key.
	 * Parse valuesString into a NAV_DATA_FIELDS by SENSOR_DATA_FIELDS
	 * sized array, where valuesString is comma-separated, with 20 values.
	 */
	
	if( strmcmp(key, "setKalman") == 0 )
	{
		kalman = parseto2DArray(valuesString);
	}
}

void setKalmanRow(char *key, char *valuesString)
{
	/* Event handler for the "setKalmanRow" key.
	 * Parse valuesString into a row number i, and 5 values.
	 * Place the 5 values into the ith row of the kalman matrix.
	 */

	 if( strmcmp( key, "setKalmanRow") == 0 )
	{
		uint8_t row = getNextInt(valuesString);
		uint8_t comma = findComma(valuesString);
		kalman[row] = parsetoArray(substring(valuesString, comma));
	}
}