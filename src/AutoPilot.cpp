// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "AutoPilot.h"

AutoPilot::AutoPilot(const Aircraft *a)
{
	aircraft = a;

	isTargetLocationValid=false;
	isTargetAltitudeValid=false;
	targetLocation = NULL;
	targetAltitude = 0;
}

void AutoPilot::takeOff() const
{
	if(!aircraft->isLandingGearDeployed()) return;
	if(!aircraft->areControlsInitialized()) return;
	if(!aircraft->areEnginesPowered()) aircraft->powerUpEngines();

	// initiate take-off sequence
	setCruiseMode();
	aircraft->setFlaps(FSR_FLAPS_FULL);
	aircraft->setEngineThrottle(30);								// don't go full throttle on startup as it may damage engine
	delay(1000);
	aircraft->setEngineThrottleFull();

	// wait for take-off velocity and then attempt to get airborne
	while(aircraft->getVelocity() < TAKEOFF_VELOCITY) 
	{
		aircraft->refresh();
		delay(1000);
	}
	aircraft->setElevators(135);

	// wait till actually airborne
	while(aircraft->getAltitude() == 0) 
	{
		aircraft->refresh();
		delay(500);
	}

	// retract flaps and landing-gear
	aircraft->retractLandingGear();
	aircraft->setFlaps(FSR_FLAPS_RETRACT);

	// Handover charge to reach desired altitude; once reached, decrease throttle to cruise
	setAltitude(CRUISE_ALTITUDE);
	
	setCruiseMode();
	aircraft->setEngineThrottle(60);
}

void AutoPilot::setCourse(const Location &destination)
{
	if(isTargetLocationValid) delete targetLocation;
	targetLocation = new Location(destination);
	isTargetLocationValid = true;
	aircraft->setEngineThrottle(70);
	keepOnCourse(0);
	aircraft->setEngineThrottle(60);
}

void AutoPilot::setAltitude(const float alt)
{
	bool climbing = false;
	if(isTargetAltitudeValid && alt > targetAltitude) climbing = true;
	if(climbing) aircraft->setEngineThrottle(90);		// increase throttle for climbing
	targetAltitude = alt;
	isTargetAltitudeValid = true;
	maintainAltitude(0);
	if(climbing) aircraft->setEngineThrottle(60);
}

float AutoPilot::getETA() const
{
	if(!isTargetLocationValid) return -1;

	Location current = aircraft->getLocation();
	float dist = targetLocation->distanceTo(current);
	return (dist / aircraft->getVelocity());
}

bool AutoPilot::keepOnCourse(const int preemptMillis = 0) const
{
	if(aircraft->isLandingGearDeployed()) return;
	if(!isTargetLocationValid) return;

	int failCount = 0;
	while(!aircraft->isLocationValid()) 
	{
		failCount++;
		delay(100);
		if(failCount > MAX_GPS_ATTEMPTS) return false;
	}
		
	Location current = aircraft->getLocation();
	float targetBearing = getDesiredBearing(current, *targetLocation);
	return setBearing(targetBearing, preemptMillis);
}

bool AutoPilot::setBearing(const float targetBearing, const int preemptMillis=0) const			// target bearing must be unipolar (0-360)
{
	float initialBearing = aircraft->getBearing();
	float deltaBearing = unipolarToBipolarBearing(targetBearing - initialBearing);
	
	float deltaBearingAbs = fabs(deltaBearing);
	float currentBearing = initialBearing;
		
	if(deltaBearingAbs==0)
	{
		stabilizeRoll();
		return true;
	}

	int timer = 0;
	if(deltaBearingAbs < RUDDER_KICK_IN_ANGLE)
	{
		// Use rudder
		do {
			deltaBearing = unipolarToBipolarBearing(targetBearing - aircraft->getBearing());
			deltaBearingAbs = fabs(deltaBearing);	
			
			float rudderDelta = map(deltaBearingAbs,0,5,0,20);
			aircraft->setRudder(deltaBearing < 0 ? 90-rudderDelta : 90 + rudderDelta);
			aircraft->refresh();
			
			delay(AUTOPILOT_ADJUSTMENT_DELAY);
		} while(deltaBearingAbs >= 0.25);
		
		aircraft->setRudder(90);
		return true;
	} else {
		// Use ailerons
		do {
			deltaBearing = (targetBearing - aircraft->getBearing());
			if(fabs(deltaBearing) > 180) 
			{
				if(deltaBearing > 0)
					deltaBearing -= 360;
				else
					deltaBearing += 360;
			}
			
			if(!setAilerons(deltaBearing)) break;
			aircraft->refresh();
			delay(AUTOPILOT_ADJUSTMENT_DELAY);

			if(preemptMillis > 0)
			{
				timer+=AUTOPILOT_ADJUSTMENT_DELAY;			
				if(timer > preemptMillis) return false;
			}
		} while(1);
	}

	stabilizeRoll();
	return true;
}

bool AutoPilot::setAilerons(const int deg) const				// deg must be bipolar
{
	int absDeg = abs(deg);
	if(absDeg <= BEARING_TOLERANCE) return false;
	
	// aileron positions: 0 deg = full-up, 90 deg = flat, 180 deg = full down
	if(deg < 0)
	{
		// turn LEFT: set left-aileron UP, set right-aileron DOWN
		if(absDeg > EASE_OUT_ANGLE)
		{
			aircraft->setLeftAileron(0);
			aircraft->setRightAileron(180);
		} else {
			aircraft->setLeftAileron((int)easeOut(EASE_OUT_ANGLE-absDeg, EASE_OUT_ANGLE, 0, 90));
			aircraft->setLeftAileron((int)easeOut(EASE_OUT_ANGLE-absDeg, EASE_OUT_ANGLE, 180, 90));
		}
	} else {
		// turn RIGHT: set right-aileron UP, set left-aileron DOWN
		if(absDeg > EASE_OUT_ANGLE)
		{
			aircraft->setLeftAileron(180);
			aircraft->setRightAileron(0);
		} else {
			aircraft->setLeftAileron((int)easeOut(EASE_OUT_ANGLE-absDeg, EASE_OUT_ANGLE, 180, 90));
			aircraft->setLeftAileron((int)easeOut(EASE_OUT_ANGLE-absDeg, EASE_OUT_ANGLE, 0, 90));
		}
	}
	return true;
}

bool AutoPilot::setElevators(const int delta) const
{
	int deltaAbs = abs(delta);
	if(deltaAbs <= ALTITUDE_TOLERANCE) return false;
		
	int degree = (deltaAbs > EASE_OUT_ALTITUDE ? 90 : (deltaAbs * 90)/EASE_OUT_ALTITUDE); 
		
	if(delta < 0)
	{
		// descend: set elevators UP
		aircraft->setElevators(90-degree);
	} else {
		// climb: set elevators DOWN
		aircraft->setElevators(90+degree);
	}

	return true;
}

bool AutoPilot::maintainAltitude(const int preemptMillis = 0) const
{
	if(aircraft->isLandingGearDeployed()) return;
	if(!isTargetAltitudeValid) return;

	float currentAltitude, delta, deltaAbs;
	int degree = 0;

	int timer=0;
	do {
		currentAltitude = aircraft->getAltitude();
		delta = targetAltitude - currentAltitude;
		
		if(!setElevators(delta)) break;
		aircraft->refresh();		
		delay(AUTOPILOT_ADJUSTMENT_DELAY);

		if(preemptMillis > 0)
		{
			timer+=AUTOPILOT_ADJUSTMENT_DELAY;
			if(timer > preemptMillis) return false;			// return without flattening elevators
		}
	} while(1);

	aircraft->setElevators(90);
	return true;
}

void AutoPilot::landImmediately(const float airportAltitude) const
{
	if(aircraft->isLandingGearDeployed()) return;			// maybe already on ground: rectify: check velocity too
	
	setCruiseMode();
	aircraft->setEngineThrottle(85);
	setAltitude(3 + airportAltitude);							// 9 feet
	aircraft->setEngineThrottle(70);
	setAltitude(2 + airportAltitude);							// 6 feet
	aircraft->setFlaps(FSR_FLAPS_HALF);
	aircraft->setEngineThrottle(50);
	setAltitude(1 + airportAltitude);							// 3 feet
	aircraft->setEngineThrottle(30);
	aircraft->deployLandingGear();
	aircraft->setFlaps(FSR_FLAPS_FULL);
	aircraft->setElevators(45);
	delay(50);
	aircraft->setEngineThrottle(20);
	
	while(aircraft->getAltitude() > 0.15 + ALTITUDE_TOLERANCE + airportAltitude) delay(15);		// wait till 0.5 feet above ground
	aircraft->deploySpoilers();
	aircraft->setLeftAileron(0);
	aircraft->setRightAileron(0);
	aircraft->setElevators(140);

	aircraft->setEngineThrottle(10);
	while(aircraft->getAltitude() > ALTITUDE_TOLERANCE + airportAltitude) delay(15);
	aircraft->powerDownEngines();
	setCruiseMode();
}

void AutoPilot::land(const Airport &airport) const
{
	Location finalApproachLocation = getLocationBeforeLocation(*airport.location, airport.bearing, FINAL_APPROACH_DISTANCE);
	
	float sideStepBearing = airport.bearing - 90;
	if(sideStepBearing < 0) sideStepBearing += 360;
	
	Location sideStepLocation = getLocationBeforeLocation(finalApproachLocation, sideStepBearing, TURNING_RADIUS);

	setCourse(sideStepLocation);
	do
	{
		keepOnCourse(100);
	} while(getETA() > 2);
	setBearing(sideStepBearing);
	
	setCourse(finalApproachLocation);
	do
	{
		keepOnCourse(100);
	} while(getETA() > 2);
	setBearing(airport.bearing);

	landImmediately(airport.altitude);	
}

void AutoPilot::stabilizeRoll() const
{
	// TO DO: improve this code to use Easing function to adjust ailerons
	int roll;
	aircraft->setRudder(90);
	while((roll=(int)aircraft->getRoll()) != 0)
	{
		// convert to bipolar
		int rbp = (roll <= 180 ? roll : roll-360);
		setAilerons(-1 * rbp);							// reverse polarity to adjust
		aircraft->refresh();
		delay(AUTOPILOT_ADJUSTMENT_DELAY);
	}
	aircraft->setLeftAileron(90);
	aircraft->setRightAileron(90);
}

void AutoPilot::stabilizePitch() const
{
	// TO DO: improve this code to use Easing function to adjust elevators
	int pitch;
	while((pitch=(int)aircraft->getPitch()) != 0)
	{
		// convert to bipolar
		int pbp = (pitch <= 180 ? pitch : pitch-360);
		setElevators(-1 * pbp);							// reverse polarity to adjust
		aircraft->refresh();
		delay(AUTOPILOT_ADJUSTMENT_DELAY);
	}
}

void AutoPilot::setCruiseMode() const
{
	aircraft->retractSpoilers();
	aircraft->setFlaps(FSR_FLAPS_RETRACT);
	aircraft->setRudder(90);
	
	if(!aircraft->isLandingGearDeployed())
	{
		stabilizeRoll();
		stabilizePitch();
	}

	aircraft->setElevators(90);
	aircraft->setLeftAileron(90);
	aircraft->setRightAileron(90);
}

void AutoPilot::testFlightControls() const
{
	if(!aircraft->isLandingGearDeployed()) return;			// cannot perform test mid-air!
	
	setCruiseMode();
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	
	aircraft->setFlaps(FSR_FLAPS_HALF);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setFlaps(FSR_FLAPS_FULL);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setFlaps(FSR_FLAPS_RETRACT);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	
	aircraft->deploySpoilers();
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->retractSpoilers();
	
	aircraft->setElevators(45);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setElevators(135);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setElevators(90);

	aircraft->setRudder(45);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setRudder(135);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setRudder(90);

	aircraft->setLeftAileron(45);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setLeftAileron(135);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setLeftAileron(90);

	aircraft->setRightAileron(45);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setRightAileron(135);
	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	aircraft->setRightAileron(90);

	delay(AUTOPILOT_ADJUSTMENT_DELAY);
	setCruiseMode();
}

void AutoPilot::turn(const float degree) const				// degree must be bipolar (-180 to +180) (negative: LEFT, positive: RIGHT)
{
	float currentBearing = aircraft->getBearing();
	setBearing(unipolarToBipolarBearing(currentBearing + degree), 0);
}
