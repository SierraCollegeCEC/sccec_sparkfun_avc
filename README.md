sccec_sparkfun_avc
==================

Sierra College Computer Engineering Club: Sparkfun AVC Competition

## Release Notes
* 0.2
  * Sensors
    * Reading compass heading data from HM55B magnetometer
    * Setup interface to pass heading data to the nav system
  * Navigation
    * First pass on PID controller, includes proportional, integral, and derivative corrections
    * Setup interface to pass data to the motion system
    * Sending the results from the PID controller to motion
  * Motion
    * Added interface for setting speed on the esc
    * Added interface for setting turn angle on steering servo
    * Set steering value to whatever the navigation system has determined
* 0.1
  * Added prototype framework for arduino project
