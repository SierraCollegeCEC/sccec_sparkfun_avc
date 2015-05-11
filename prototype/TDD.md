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
information to its map, which represented as a series of continuous
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
is. The new state estimation is sent to the navigation subsystem
inside a navData struct. 
		
### Navigation Subsystem
The navigation subsystem begins by reading the filtered heading,
position, and velocity data sent to it by the sensors subsystem in the
navData struct. The current position is used by the bezier curve
implementation to find the closest point on the curve, and then some
point farther ahead on the curve that the vehicle should be traveling
toward (the details of both finding the closest point and the
destination are left for section five). The displacement vector from the
current position to the desired one is the desired heading vector. The
current heading and desired heading are converted to compass directions,
with  0 degrees being true North (the Earth's magnetic declination is
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
drivers, and the hardware responds. The relevant files are motion.cpp/h
and pid.cpp, and their header files.

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
Kalman filter equations specific to this project and their specific
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