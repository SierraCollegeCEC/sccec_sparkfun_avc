/*
 * Sierra College Computer Engineering Club: Sparkfun AVC
 * Kalman Filter
 * Authors: Kristoffer Semelka, Austin
 */

#include "common.h"
#include "sensors.h"
#include "telemetry.h"

extern navData current;         /* The current state. */

static INSData INS;             /* Inertial Navigation System data, which
                                 * consists of acceleromter and gyro data. */

static sensorDataArr prediction;   /* A prediction of the next state, in array form. */
static sensorDataArr measurement;  /* The GPS and Magnetometer data, in array form. */
static sensorDataArr error;        /* Difference between error and measurement. */

extern KalmanGain kalman = {	/* Kalman gain, which is the linear correction
                                 * factor applied to the error term. */
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01},
	{.01, .01, .01, .01, .01}
}

/* Array representation of structs, for internal use. */
enum sensorData_field {gps_pos_x, gps_pos_y, gps_vel_x, gps_vel_y, mag_heading};
typedef float sensorDataArr[SENSOR_DATA_FIELDS];

enum navData_field {pos_x, pos_y, vel_x, vel_y, heading};
typedef float navDataArr[NAV_DATA_FIELDS];

/* Forward Declarations */
void predict();
void compare();
void correct();
float step(float, float, float);
float dot(float i[], float j[], int);
telemetryEventHandler setKalman;
telemetryEventHandler setKalmanRow;

void initFilter()
{
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
	subsystem, and converted to arrays. Arrays make the code readable/extensible.
	Then the error between the predicted readings
	and the actual readings is calculated. Finally, the
	update stage of the Kalman filter runs, which applies
	the Kalman gain to the error as an optimal correction
	factor. 
	*/

	INS = getINSData(); /* Retrieve INS Data */
	predict(); /* Predict next state */
	convertSensorToArray(getSensorData()); /* Retrieve GPS/Mag Data */
	compare(); /* Compare predicted observation with actual */
	correct(); /* Correct by applying linear multiple of error to prediction */
	
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
	prediction[pos_x] = step(current.pos_x, current.vel_x,
	                        INS.displ_x);
	
	prediction[pos_y] = step(current.pos_y, current.vel_y,
	                        INS.displ_y);

	prediction[vel_x] = current.vel_x + INS.change_vel_x;
	prediction[vel_y] = current.vel_y + INS.change_vel_y;

	prediction[heading] = current.heading + INS.yaw;
}

float step(float initial, float velocity, float displacement)
{
	return initial + velocity * dt + displacement; 
}

void compare()
{
	/* error = measured - predicted */

	for(uint8_t i = 0; i < SENSOR_DATA_FIELDS; i++)
	{
		error[i] = measurement[i] - prediction[i];
	}
}

void correct()
{
	/* current = prediction + Kalman * error

	The kalman gain matrix is a constant defined at filescope.
	dotting a matrix[i] with a vector is the equivalent of
	the dot product of the ith row of the matrix with the
	vector; this is standard matrix multiplication.
	*/

	float currentArr[NAV_DATA_FIELDS];

	for(uint8_t i = 0; uint8_t < NAV_DATA_FIELDS, i++)
	{
		currentArr[i] = prediction[i] + dot(kalman[i] * error);
	}

	convertCurrentToStruct(currentArr);
}

float dot(float matrixRow[], float vector[], uint8_t length)
{
	/* Standard dot product */

	float accum = 0.f;

	for(uint8_t i = 0; i < length; i++)
	{
		accum += matrixRow[i] * vector[i];
	}

	return accum;
}

void convertSensorToArray(SensorData *data)
{
	/* Converts a SensorData struct to a SensorDataArr,
	   and assigns to sensorDataArr */
	sensorDataArr[gps_pos_x] = data->pos_x;
	sensorDataArr[gps_pos_y] = data->pos_y;
	sensorDataArr[gps_vel_x] = data->vel_x;
	sensorDataArr[gps_vel_y] = data->vel_y;
	sensorDataArr[mag_heading] = data->heading;
}

navData* getCurrentEstimate()
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