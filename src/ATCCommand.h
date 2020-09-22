// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef ATCCommand_h
	#define ATCCommand_h

	/*
	 * COMMANDS
	 * --------
	 * HELLO			used for maintaining communication / checking whether Aircraft is alive
	 * MAINTAIN			instruction to maintain throttle, attitude, etc.; works same as HELLO
	 * LEVEL			used to cancel all roll and pitch
	 * CANCEL_ROLL		used to cancel any left/right roll
	 * CANCEL_PITCH		used to cancel any up/down pitch
	 * FLAPS_RETRACT	used to retract flaps
	 * FLAPS_HALF		used to deploy half flaps
	 * FLAPS_FULL		used to deploy full flaps
	 * SPOILERS_ON		used to deploy spoilers
	 * SPOILERS_OFF		used to retract spoilers
	 * ROLL_RIGHT		used to inrease roll by 10 degrees
	 * ROLL_LEFT		used to decrease roll by 10 degrees
	 * CLIMB			used to increase pitch by 10 degrees
	 * DESCEND			used to decrease pitch by 10 degrees
	 * THROTTLE_UP		used to increase throttle by 30%
	 * THROTTLE_DOWN	used to decrease throttle by 30%
	 * YAW_LEFT			used to decrease yaw by 10 degrees
	 * YAW_RIGHT		used to increase yaw by 10 degrees
	 * DROP_PACKAGE		used to drop package
	 * RETURN_TO_BASE	used to direct the aircraft to land at the designated airport
	 * LAND_NOW			used to direct the aircraft to start landing procedure immediately: this levels the aircraft and starts landing
	 * 
	 * Note:			Once landing procedure begins, the aircraft is deaf to any more commands
	 */

	#define ATCC_NOCOMM -1
	#define ATCC_HELLO 0
	#define ATCC_MAINTAIN 1
	#define ATCC_LEVEL 2
	#define ATCC_CANCEL_ROLL 3
	#define ATCC_CANCEL_PITCH 4
	#define ATCC_FLAPS_RETRACT 5
	#define ATCC_FLAPS_HALF 6
	#define ATCC_FLAPS_FULL 7
	#define ATCC_SPOILERS_ON 8
	#define ATCC_SPOILERS_OFF 9
	#define ATCC_ROLL_LEFT 10
	#define ATCC_ROLL_RIGHT 11
	#define ATCC_CLIMB 12
	#define ATCC_DESCEND 13
	#define ATCC_THROTTLE_UP 14
	#define ATCC_THROTTLE_DOWN 15
	#define ATCC_YAW_LEFT 16
	#define ATCC_YAW_RIGHT 17
	#define ATCC_DROP_PACKAGE 18
	#define ATCC_RETURN_TO_BASE 19
	#define ATCC_LAND_NOW 20
	#define ATCC_TEST_CONTROLS 21
	#define ATCC_START_MISSION 22
	#define ATCC_REPORT_LOCATION_AND_FUEL 23
	#define ATCC_MARK_CURRENT_LOCATION_AS_AIRPORT 24
	#define ATCC_ADD_CURRENT_LOCATION_AS_WAYPOINT 25
	#define ATCC_INSERT_CURRENT_LOCATION_AS_WAYPOINT 26
	#define ATCC_ADD_CURRENT_LOCATION_AS_DELIVERY_POINT 27
	#define ATCC_REMOVE_WAYPOINT 28
	#define ATCC_CLEAR_FLIGHTPLAN 29
	#define ATCC_REPORT_FLIGHTPLAN 30
	#define ATCC_REQUEST_COMMAND 31
	#define ATCC_OPEN_DOOR 32
	#define ATCC_CLOSE_DOOR 33
	#define ATCC_START_DEMO_MISSION 34
#endif
