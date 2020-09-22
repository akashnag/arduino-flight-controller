// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

/*
 * 		Arduino On-Board Autonomous Flight Controller for Multi-Propeller Fixed-Wing Cargo Aircraft
 * 		--------------------------------------------------------------------------------------------------
 * 		Normal operation requires:
 * 		1.	Arduino Mega
 * 		2.	BLDC motors
 * 		3.	ESCs
 * 		4.	LiPo battery pack
 * 		5.	Servo motors (for flaps, rudder, ailerons, spoilers, landing-gear, package doors)
 * 		6.	3-Axis Gyroscope + Accelerometer
 * 		7.	GPS Module
 * 		8.	NRF24L01+ / HC-12 Module
 * 		9.	Barometer
 * 		10.	Magnetometer
 * 		11.	Landing-Gears x 3-sets
 * 		12.	Propellers as required
 * 		13.	Connectors for connecting battery with ESCs
 * 		14.	AA-battery pack/holder for powering Arduino
 */

#include "FlightPlan.h"
#include "Aircraft.h"
#include "AirTrafficControl.h"
#include "ATCCommand.h"
#include "Global.h"
#include "AutoPilot.h"

bool airportDesignated = false;
Airport *airport = NULL;
FlightPlan flightPlan;

Aircraft aircraft;
AirTrafficControl atc;
AutoPilot autoPilot(&aircraft);

void setup() 
{
	atc.establishCommunication();
	aircraft.initControls();
	autoPilot.testFlightControls();
}

bool startMission()
{
	if(!airportDesignated) return false;

	atc.inform("Initiating take-off procedure...");
	autoPilot.takeOff();

	//atc.inform("Commencing training procedure...");
	//autoPilot.train();
	//atc.inform("Training complete");

	atc.inform("Attempting to reach cruise altitude...");
	autoPilot.setAltitude(CRUISE_ALTITUDE);

	int maxWaypoints = flightPlan.getWaypointCount();

	bool altFlag = false, courseFlag = false;

	Waypoint *currentWaypoint;
	if(maxWaypoints > 0)
	{
		bool emergency = false;
		for(int i=0; i<maxWaypoints; i++)
		{
			currentWaypoint = flightPlan.getWaypoint(i);

			atc.inform("Setting course to Waypoint #" + i);
			autoPilot.setCourse(currentWaypoint->getLocation());
			while(autoPilot.getETA() > 2) 
			{
				if(RTB_ON_LOW_FUEL && aircraft.isLowOnFuel()) 
				{
					atc.inform("Low fuel warning! Executing emergency RTB procedure...");
					emergency = true;
					break;
				} else if(RTB_ON_LOSS_OF_TELEMETRY && !atc.withinRange()) {
					atc.inform("Outside ATC coverage area! Executing emergency RTB procedure...");
					emergency = true;
					break;
				}

				do {
					courseFlag = autoPilot.keepOnCourse(70);
					altFlag = autoPilot.maintainAltitude(70);
				} while(!courseFlag || !altFlag);
				
				delay(500);
			}
	
			if(emergency) break;
			if(currentWaypoint->isDeliveryPoint()) 
			{
				atc.inform("Nearing drop location, preparing to drop package...");
				aircraft.dropPackage(currentWaypoint->getPackageID());
			} else {
				atc.inform("Waypoint reached.");
			}
		}
	}

	atc.inform("Initiating landing procedure...");
	autoPilot.land(*airport);

	return true;
}

void startDemoMission()
{
	if(!aircraft.isLandingGearDeployed()) return;

	// TURN --> CRUISE --> DROP --> repeat
	const int turns[] 		= { 0, -45, -90, -45, 0, -90, -45 };
	const int delays[] 		= { 5,  5,   5,   5,  5,  5,   0  };				// delays (in seconds) AFTER the corresponding turn has been completed
	const int dropPoints[] 	= { -1, -1, -1,   0, -1, -1,  -1  };				// drop packages (-1:no, >=0:yes) AFTER the cruise has been completed
	const int maxWaypoints 	= 7;

	// TAKEOFF
	const float alt = aircraft.getAltitude();
	atc.inform("Initiating take-off procedure...");
	autoPilot.takeOff();

	// CLIMB
	atc.inform("Attempting to reach cruise altitude...");
	autoPilot.setAltitude(CRUISE_ALTITUDE);

	String x;
	for(int i=0; i<maxWaypoints; i++)
	{
		// TURN
		if(turns[i] != 0)
		{
			x = String("Turning ");
			x = x + abs(turns[i]) + " degrees " + (turns[i] < 0 ? "left" : "right") + "...";
			atc.inform(x);
			autoPilot.turn(turns[i]);
		}

		// CRUISE
		int timer=delays[i];
		while(timer>0)
		{
			x = String("Cruising for ");
			x = x + timer + " more seconds...";
			atc.inform(x);
			timer--;
			aircraft.refresh();
			delay(1000);
		}

		// DROP
		if(dropPoints[i] > -1)
		{
			x = String("Dropping package #");
			x = x + dropPoints[i] + "...";
			atc.inform(x);
			aircraft.dropPackage(dropPoints[i]);
		}
	}

	// LAND
	atc.inform("Preparing to land...");
	autoPilot.landImmediately(alt);
}

void loop() 
{
	aircraft.refresh();
	
	String message;
	int fuel, n;
	float lat, lon;
	int param = 0;
	int command = atc.receiveCommand(param);
	
	switch(command)
	{
		case ATCC_NOCOMM:
			break;
		
		case ATCC_HELLO:
			atc.inform("Command received: ATCC_HELLO | Hello");
			break;

		case ATCC_TEST_CONTROLS:
			atc.inform("Command received: ATCC_TEST_CONTROLS | Initiating test of flight controls...");
			autoPilot.testFlightControls();
			atc.inform("Flight-controls test complete");
			break;

		case ATCC_START_MISSION:
			atc.inform("Command received: ATCC_START_MISSION | Starting mission...");
			bool flag = startMission();
			atc.inform((flag ? "Mission complete" : "Mission failure: no airport designated"));
			break;

		case ATCC_REPORT_LOCATION_AND_FUEL:
			fuel = aircraft.getFuel();
			Location loc = aircraft.getLocation();
			message = String("Command received: ATCC_REPORT_LOCATION_AND_FUEL | Fuel remaining: ");
			message = message + fuel + "%, Coordinates=" + loc.toString();
			atc.inform(message);
			break;

		case ATCC_MARK_CURRENT_LOCATION_AS_AIRPORT:
			if(airportDesignated) delete airport;
			airport = new Airport(&aircraft.getLocation(), aircraft.getBearing(), aircraft.getAltitude());
			airportDesignated = true;
			message = String("Command received: ATCC_MARK_CURRENT_LOCATION_AS_AIRPORT | Airport marked at: ");
			message = message + airport->location->toString();
			atc.inform(message);
			break;

		case ATCC_ADD_CURRENT_LOCATION_AS_WAYPOINT:
			Waypoint wp(Location(aircraft.getLocation()), false, -1);
			flightPlan.appendWaypoint(wp);
			atc.inform("Command received: ATCC_ADD_CURRENT_LOCATION_AS_WAYPOINT | Waypoint marked");
			break;

		case ATCC_INSERT_CURRENT_LOCATION_AS_WAYPOINT:
			Waypoint wp1(Location(aircraft.getLocation()), false, -1);
			flightPlan.insertWaypoint(param,wp1);
			atc.inform("Command received: ATCC_INSERT_CURRENT_LOCATION_AS_WAYPOINT | Waypoint inserted");
			break;
			
		case ATCC_ADD_CURRENT_LOCATION_AS_DELIVERY_POINT:
			Waypoint wp2(Location(aircraft.getLocation()), true, param);
			flightPlan.appendWaypoint(wp2);
			atc.inform("Command received: ATCC_ADD_CURRENT_LOCATION_AS_DELIVERY_POINT | Delivery point marked");
			break;

		case ATCC_REMOVE_WAYPOINT:
			flightPlan.removeWaypoint(param);
			atc.inform("Command received: ATCC_REMOVE_WAYPOINT | Waypoint removed");
			break;

		case ATCC_CLEAR_FLIGHTPLAN:
			flightPlan.clearAll();
			atc.inform("Command received: ATCC_CLEAR_FLIGHTPLAN | Flightplan cancelled");
			break;

		case ATCC_REPORT_FLIGHTPLAN:
			atc.inform("Command received: ATCC_REPORT_FLIGHTPLAN | Retrieving flight-plan...");
			atc.inform(airportDesignated ? "Airport coordinates: " + airport->location->toString() : "No airport designated");
			n = flightPlan.getWaypointCount();
			if(n==0)
				atc.inform("Flight-plan empty.");
			else {
				atc.inform("Waypoint count: " + n);
				atc.inform("#\tDel\tPID\tCoordinates\n--\t---\t---\t----------------------------");
				for(int i=0; i<n; i++) atc.inform(String(i) + "\t" + flightPlan.getWaypoint(i)->toString());
				atc.inform("--------------------------------------------------------------------");
			}
			break;

		case ATCC_OPEN_DOOR:
			atc.inform("Command received: ATCC_OPEN_DOOR | Opening door...");
			aircraft.openDoor(param);
			break;
			
		case ATCC_CLOSE_DOOR:
			atc.inform("Command received: ATCC_CLOSE_DOOR | Closing door...");
			aircraft.closeDoor(param);
			break;

		case ATCC_START_DEMO_MISSION:
			atc.inform("Command received: ATCC_START_DEMO_MISSION | Starting demo mission...");
			startDemoMission();
			atc.inform("Demo Mission complete");
			break;

		default:
			message = String("Command not recognized | Received: ");
			message = message + command + " with parameter " + param;
			atc.inform(message);
			break;
	}
	atc.clearedToSend();
}
