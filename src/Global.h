// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Global_h
	#define Global_h

	#include <Arduino.h>
	
	// ----------------- Hard Parameters -------------------
	#define MAX_ENGINES 				4
	#define MAX_CHAMBERS 				2
	#define BATTERY_CELL_COUNT 			3
	#define BATTERY_CELL_FULL_THRESHOLD 3.7
	#define HAS_FLAPS 					true
	#define HAS_SPOILERS 				true
	#define HAS_LIGHTS					true	
	// -----------------------------------------------------


	// ----------------- Pins -------------------------
	#define AIRCRAFT_SERVO_LANDING_GEAR_PIN	1
	
	#define FSR_SERVO_FLAP_PIN 				2
	#define FSR_SERVO_ELEVATOR_PIN 			3
	#define FSR_SERVO_RUDDER_PIN 			4
	#define FSR_SERVO_SPOILER_PIN 			5
	#define FSR_SERVO_LEFT_AILERON_PIN 		6
	#define FSR_SERVO_RIGHT_AILERON_PIN 	7
	
	#define TRACKER_HCSR04_TRIG_PIN 		8
	#define TRACKER_HCSR04_ECHO_PIN 		9
	
	#define TRACKER_GPS_RX_PIN 				10
	#define TRACKER_GPS_TX_PIN 				11
	
	#define WT_NRF24L01_PIN1 				12
	#define WT_NRF24L01_PIN2 				13

	#define AIRCRAFT_LIGHTS_PIN				20
	
	const int ENGINE_PINS[MAX_ENGINES]     ={ 14, 15, 16, 17 };		// connected to the ESCs
	const int CHAMBER_PINS[MAX_CHAMBERS]   ={ 18, 19 };				// connected to servos controlling the package-chamber doors
	const int CELL_PINS[BATTERY_CELL_COUNT]={ A0, A1, A2 };			// connected to the (+) of each cell
	// ------------------------------------------------


	// ----------------- Soft Parameters -------------------
	#define CRUISE_ALTITUDE 			4				// set cruise-altitude = 4 metres above ground
	#define ALTITUDE_TOLERANCE 			0.0508			// set altitude measurement error = 2 inches
	#define GPS_ALTITUDE_TOLERANCE 		0.3				// set GPS altitude measurement error = 1 feet
	#define TAKEOFF_VELOCITY 			1.0				// set takeoff velocity to be 1 m/s (experiment and update this value as required)

	#define RTB_ON_LOSS_OF_TELEMETRY	true
	#define RTB_ON_LOW_FUEL				true
	
	#define BATTERY_CELL_LOW_THRESHOLD 	3.4
	
	#define FSR_FLAPS_RETRACT 			90
	#define FSR_FLAPS_HALF 				135
	#define FSR_FLAPS_FULL 				180
	#define FSR_FLAPS_BRAKE 			0

	#define MAX_GYRO_ATTEMPTS 			15
	#define MAX_GPS_ATTEMPTS 			15
	#define MAX_ATC_COMM_RETRIES		15
	#define WT_TIMEOUT_PERIOD 			200000			// in microseconds

	#define GPS_STALE_AGE 				5000			// max. allowed time (in miliseconds) since last GPS satellite lock

	#define BEARING_TOLERANCE 			0				// deviation allowed = 0deg
	#define ALTITUDE_TOLERANCE 			0.3				// deviation allowed = 0.3m
	#define EASE_OUT_ALTITUDE 			35				// max deviation in altitude above which elevators are set to FULL
	#define AUTOPILOT_ADJUSTMENT_DELAY 	10				// milliseconds between flight-control adjustments

	#define FINAL_APPROACH_DISTANCE 	10				// final approach = 30 feet
	#define TURNING_RADIUS 				4				// turning radius = 12 feet
	#define EASE_OUT_ANGLE 				90				// max. angle at which ease-out function kicks in
	#define RUDDER_KICK_IN_ANGLE		5				// angle below which rudder (instead of ailerons) will be used to adjust bearing

	#define LIGHTS_ON_DURATION			250
	#define LIGHTS_OFF_DURATION			750
	// -----------------------------------------------------


	// ------------------ Functions ------------------------
				
	// -----------------------------------------------------
#endif
