# Technical Design Document
This document outlines and specifies the design of the software written by
the Sierra College Computer Engineering Club for use in 2015 Sparkfun
Automated Vehicle competition. This includes:
- The ovearching design, including its subsystems and modules
- Elaboration of the responsibilities of its subsystems and modules
- Justification of the design choices made such as the choice of
module boundaries/interfaces and the algorithms used
- Detailed explanation of any equations or models used, such as the
Kalman filter and Bezier curve implementations
- Intended hardware requirements
- Recommendations for modification or extension
- Any acknowledgements to contributers, which includes both members and
nonmembers of our organization

## Table of Contents
1. [Design](#design)
2. [Technical Specification](#technical-specification)
	 1. [Sensors Subsystem] (#sensors-subsystem)
	 2. [Navigation Subsystem] (#navigation-subsystem)
	 3. [Motion Subsystem] (#motion-subsystem)
3. [Kalman Filter Implementation](#kalman-filter-implementation)
4. [Bezier Curve Equations](#bezier-curve-equations)
5. [Hardware Used] (#hardware)
6. [How to Use our Code] (#how-to-use-our-code)
7. [Acknowledgements] (#acknowledgements)
8. [License] (#license)

## Design
This software is divided into three major components: the
[Sensors](#Sensors Subsystem), [Navigation](#Navigation Subsystem), and
[Motion](#Motion Subsystem) subsystems. The software operates a TI
microcontroller mounted on to an RC car and is attached to some
[sensors](#Hardware). A single flow of control through the entire system
starts with the collection of GPS sensor data, magnetometer data, 10 samples of
accelerometer and gyroscope information from a strapdown intertial
navigation unit. The sensors subsystem organizes this information and
sends it to a Kalman filter module, which uses statistical sensor fusion
and a model of the vehicle's physics to extrapolate a location and
heading. The sensors subsystem then takes this information and pushes it
to the navigation subsystem. The navigation subsystem compares this
information to its map, which is represented as a continuous series of
[Bezier Curves](#Bezier Curve Equations). Then, it figures out the
difference between the desired heading and the current heading, as well as the
difference between the desired velocity and current velocity. The
heading is converted into a steering angle in degrees. These
values are pushed into a PID controller that adjusts the values
according to its internal, configurable gain values. The new adjusted
values are returned, and the new heading and velocity corrections are
sent to the motion subsystem so that the ECS and steering can be
adjusted accordingly.

##Technical Specification

### Sensors Subsystem
TK, SENSOR READINGS
After the sensor data is read and packed into a sensorData struct, this
information is sent into a kalman filter. The filter uses a measurement
model to map the sensor readings to a new observed state. The filter
calculates a predicted observation by applying a system model to the
previous record state estimation. The kalman filter then compares the
observed state to the predicted state, and applies a gain value to
this difference to determine a weighted average between the
two. Lastly, the filter updates what it thinks its accuracy currently
is. These details are expounded upon in the section on the Kalman
filter. The new state estimation is sent to the navigation subsystem
inside a navData struct. The relevant files are sensors.cpp and
kalman.cpp, and their header files.
		
### Navigation Subsystem
The navigation subsystem begins by reading the filtered heading,
position, and velocity data sent to it by the sensors subsystem in the
navData struct. The current position is used by the bezier curve
implementation to find both the closest point on the curve and also some
point farther ahead on the curve that the vehicle should be traveling
toward (the details of both finding the closest point and the
destination are left for section five; this is referred to as a
"rabbit-chasing" algorithm). The displacement vector from the
current position to the desired one is the desired heading vector. The
current heading and desired heading are converted to compass directions,
with 0 degrees being true North (the Earth's magnetic declination is
accounted for and configurable), a negative angle representing East, a
positive West, and +/- 180 degrees being South.  The current heading is
then subtracted from the desired to find the steering angle required to
get the vehicle oriented in the right direction. The desired velocity is
encoded in the map, and the desired throttling is calculated from the
difference between the desired and current velocity. These two pieces of
information are stored in a motionData struct and sent to the motion
subsystem. The relevant files are nav.cpp and bezier.cpp, and their
header files.

### Motion Subsystem
The motion subsystem extracts the heading and velocity corrections and
applies a PID controller to both of these values. The gains are
configurable. The returned values are merely sent to the servo and ecs
drivers, and the hardware responds. The relevant files are motion.cpp
and pid.cpp, and their header files.

## Kalman Filter Implementation
The general Continuous Kalman Filter is the optimal estimator for
linear, dynamic processes affected by Guassian white noise. The
Discrete Time Kalman Filter is the discrete approximation of this
filter, and is the most often used variant of the Kalman filter in
control systems (a notable exception to this role is missile guidance
systems, which either use the Kalman-Bucy filter or a hybrid between
the Discrete Time and Continuous variants).
The
[Wikipedia article](http://en.wikipedia.org/wiki/Kalman_Filter) is
especially illuminating in this regard.

In short, there are only a few major components of a Kalman
filter. The first is the vector of state variables, s, which represent
the "true" state of the system at any point in time. In our use-case,
the vector of state variables is simply composed of:
- the absolute x and y coordinates, where y is in the direction of
North and x East, and
- the velocities dx/dt and dy/dt, which are the standard time
derivatives
- and the heading, which is the direction the vehicle is facing.

s = [x, y, dx/dt, dy/dt, theta]

The second component is some rule for transitioning from one state to
another, assuming no input into the system, written in the form of a
matrix of coefficients F. If the system is nonlinear, this is a Jacobian
of partials. Our matrix is a linear 5 x 5:

R1: [1  0  dt  0   0]  
R2: [0  1  0   dt  0]  
R3: [0  0  1   0   0]  
R4: [0  0  0   1   0]  
R5: [0  0  0   0   1]  

These simply represent the rather trivial equations x(or y) = 1 + dt *
dx/dt (or dy/dt), the velocity is assumed to remain constant, and the
heading is assumed to remain constant.

The third component is some input vector u. This is traditionally the
"control" applied to a system - it operates on its own accord until
you move it in some manner to a more optimal state. A very naive
approach to modeling control in our case would be to let u be the
vector [u_t, u_theta], where u_t is the throttle and u_theta is the
angle of steering. This choice of control makes everything modeled
nonlinearly, and you have to resort to awful Jacobians. Instead of
that, we employ an Alternative Direct Kalman Filter. The typical
direct Kalman Filter models everything in the most straightforward
way: a gyroscope is a sensor, and must be measured. The input to the
system is the actual control, namely the steering and throttle. So our
control would be throttle and steering and the sensors would be
gyroscope, accelerometer, magnetometer, and gps module. The
alternative direct kalman filter assumes that gyroscope and
accelerometer data are already very good estimations for the control
applied to a system: these, then, become your "input" to the
system. These Kalman filter then essentially "checks" the inertial
navigation units against the less accurate but drift-stable
magnetometers and gps units. The Kalman filter becomes linear,
computational resources are saved, and accuracy actually becomes
better because an attempt to model all of the dynamics of a rattling,
racing rc car on pavement would introdue error purely from
oversimplification in the model.

So, with those comments aside, we are justified in using a different
control. The accelerometer and gyroscope data are integrated
numerically across the 10 samples from each. The gyroscope is
integrated once to get angular displacement. The accelerometer data is
integrated once to get change in velocity, and then the history of
measured velocities is integrated to get a final displacement in
position. The change of velocity and the displacement are relative to
the body frame of the vehicle: these are resolved by using the
gyroscope data to rotate the vectors into the global frame.

Our control vector is thus: u = [dx, dy, d^2x, d^2y, dtheta], namely
displacement, change in velocity, and change in angle. An input rule B
has to he applied to convert the control into changes in state.

TK: Control matrix

Kalman filters account for a guassian white noise v that accompanies a
process. If v has n elements, then v also has an nxn covariance matrix
Q, where the ith,jth element represents the statistical covariance
between the ith and jth elements of v. Elements along the diagonal
represents the variance of the ith element of v. There are as many
elements of v as there are elements of our state vector s. In our
case, we have confused our process with a couple of our sensors, so we
incorrectly model our INS noise as process noise.

We can now write the equation for our model.

s_k+1 = F s_k + B u_k + v_k

This says nothing more than "the next state should be the last state
after a single state transition, plus whatever control was applied,
plus some noise."

There are only 3 more components: those related to actual measurement.

We need a vector z that represents the measurement we observe at time
k. This will typically just be every sensor you have installed. Ours
is quite tame:

z = [x, y, dx/dt, dy/dt, theta]

The first four measurements are provided by a GPS sensor; the last is
provided by a magnetometer. 

We typically say that the measurement z is the result of mapping the
state s to the measurement rule H. Our H is just a 5x5 identity
matrix; tame indeed, given our choice of control. If we had gone down
the nonlinear route, there would be some very pernicious partial
derivatives here. Lastly, there is a measurement noise vector w, which
is defined in the same manner as v, except its covariance is R and it
has precisely the same number of elements as z has (each sensor
reading has its own noise).

The "measurement model", quite naturally, is:

z_k = H s_k + w_k.

These two equations are not used in practice, but everything has been
defined for a very clear explanation of how the Kalman filter itself
actually works.

A high level overview:
-The Kalman filter starts with its last state estimate. It predicts what
the next state should be by evaluating F s_k + B u_k, since the last
input was known. This is known as the *a priori* ("before the facts") estimate.
-The filter puts its estimate into the measurement model and compares
that to the measurement z by taking the difference. This is known as
the residual.
-Then, the filter calculates how much it should correct between
whatever it observes and what it estimated (remember that the filter
provides the best guess given those two; this is called the "Kalman
gain")
-The Kalman gain is applied to the residual and adds it to the a
priori estimate, resulting in the *a posteriori* ("after the facts")
estimate.
-The Kalman filter then figures out its new "error" matrix, which is
its best guess for how well it's doing as an estimator. This is used in the
calculation of the Kalman gain.

The most interesting thing about the filter is that it does not
actually matter what values you observe. This is because your belief
in your sensors is invariant, and shouldn't change! The Kalman Gain
thus converges to a value, can be precalculated, and then inlined
appropriately.

These are the equations of the Kalman filter:

## How to Use our Code
This code is [licensed](#license) under the MIT License, which gives
any potential user the freedom to rewrite, repurpose, and redistribute
this code without any assumption of warranty, so long as the license
is redistributed as well. If you want to use our code
with your own hardware, you need to write drivers for both the sensors
and the motion units. A driver merely needs to implement the
methods specified in the hardware section. Beyond this, however, the
Kalman filter equations need to be reformulated. This includes adding
an extra row to the measurement matrix for every sensor added, as well
as adding another row and column to the measurement covariance matrix
to represent the variance of the sensor added (its covariance is
assumed to be independent of all of the other sensors). Simulating the
matrix multiplication of those rows in code is necessary as
well. An example of a sensor that we did not use but perhaps you might
is a wheel encoder. Lastly, the scheduler and integrator need to be
reconfigured to take full advantage of the sampling frequency of your
sensors.  We recommend you precompute the Kalman gain and inline it
just as we have, because actual matrix multiplication is an
unnecessary expense in this domain, and the gain converges to a
constant matrix. Lastly, you might want to adjust the PID values we
have set, although we quite like our values and think they are
unlikely to change from one ECS/steering configuration to another.


## Acknowledgements
[James
Smith](https://github.com/jamesdsmith/) is the lead developer for this
project. He is responsible for much of the
overarching design of this software, the primary author of over half
of the code written, the author of some of the initial tooling and the
telemetry system for easier testing and remote configurability, the
one who proposed using bezier curves, pid controllers, and kalman
filters, and an invaluable resource due to his decade of experience
in professional software development.

[Alex Norell](https://github.com/AlexNorell/) is
responsible for the selection and purchase of the majority of the
hardware, the writing of the sensors subsystem, the assembly of the vehicle
itself, and the progenitor of this entire project. His sheer knowledge
of computer hardware components and low level design was indispensable
throughout this project.

[Kristoffer Semelka](https://github.com/soderstroff/) is responsible
for the navigation subsystem, the bezier curve implementation, the
Kalman filter equations specific to this project and their actual
implementation, and the writing of this documentation.

## License
The MIT License (MIT)

Copyright (c) 2014 James Smith, Alex Norell, Kristoffer Semelka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.