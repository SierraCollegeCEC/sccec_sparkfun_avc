# Technical Design Document
This document outlines and specifies the design of the Sierra College Sparkfun
Automated Vehicle Competition software project. This includes:
- The ovearching design, including its subsystems and modules
- Elaboration of the responsibilities of its subsystems and modules
- Justification for any design choices, such as separation of concerns
and algorithms used
- Detailed explanation of any equations or models used, such as the
Kalman filter and Bezier curve implementations
- Intended hardware requirements
- Recommendations for modification or extension
- Any acknowledgements to contributers, including both members and
nonmembers of our organization

## Table of Contents
1. [Design](#design)
2. [Technical Specification](#Technical Specification)
	 1. [Sensors Subsystem] (#Sensors Subsystem)
	 2. [Navigation Subsystem] (#Navigation Subsystem)
	 3. [Motion Subsystem] (#Motion Subsystem)
3. [Kalman Filter Implementation](#Kalman Filter Implementation)
4. [Bezier Curve Equations](#Bezier Curve Equations)
5. [Hardware Used] (#Hardware)
6. [How to Use our Code] (#How to Use our Code)
7. [Acknowledgements] (#Acknowledgements)
8. [License] (#License)

## Design