// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "FlightControlSurfaces.h"

FlightControlSurfaces::FlightControlSurfaces()
{
	
}

void FlightControlSurfaces::init()
{
	if(HAS_FLAPS) flaps.attach(FSR_SERVO_FLAP_PIN);
	elevators.attach(FSR_SERVO_ELEVATOR_PIN);
	rudder.attach(FSR_SERVO_RUDDER_PIN);
	leftAileron.attach(FSR_SERVO_LEFT_AILERON_PIN);
	rightAileron.attach(FSR_SERVO_RIGHT_AILERON_PIN);
	if(HAS_SPOILERS) spoilers.attach(FSR_SERVO_SPOILER_PIN);
}

void FlightControlSurfaces::retractSpoilers()
{
	if(HAS_SPOILERS)
	{
		spoilers.write(0);
		spoilersDeployed = false;
	}
}

void FlightControlSurfaces::deploySpoilers()
{
	if(HAS_SPOILERS)
	{
		spoilers.write(90);
		spoilersDeployed = true;
	}
}

void FlightControlSurfaces::setFlaps(int mode)
{
	if(HAS_FLAPS)
	{
		flaps.write(mode);
		flapMode = mode;
	} else {
		// use ailerons as flaps
		leftAileron.write(mode);
		rightAileron.write(mode);
		flapMode = mode;
	}
}

void FlightControlSurfaces::setElevators(int degree)
{
	elevators.write(degree);
	elevatorPosition = degree;
}

int FlightControlSurfaces::getElevatorPosition()
{
	return elevatorPosition;
}

void FlightControlSurfaces::setRudder(int degree)
{
	rudder.write(degree);
	rudderPosition = degree;
}

int FlightControlSurfaces::getRudderPosition()
{
	return rudderPosition;
}

void FlightControlSurfaces::setLeftAileron(int degree)
{
	leftAileron.write(degree);
	leftAileronPosition = degree;
}

int FlightControlSurfaces::getLeftAileronPosition()
{
	return leftAileronPosition;
}

void FlightControlSurfaces::setRightAileron(int degree)
{
	rightAileron.write(degree);
	rightAileronPosition = degree;
}

int FlightControlSurfaces::getRightAileronPosition()
{
	return rightAileronPosition;
}
