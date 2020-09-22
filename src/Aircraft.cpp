// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Aircraft.h"

Aircraft::Aircraft()
{
	landingGearDeployed = false;
	controlsInitialized = false;
	enginesPowered = false;

	for(int i=0; i<MAX_CHAMBERS; i++) chamberDoorsOpen[i] = false;

	if(HAS_LIGHTS) 
	{
		pinMode(AIRCRAFT_LIGHTS_PIN, OUTPUT);
		digitalWrite(AIRCRAFT_LIGHTS_PIN, LOW);
	}
	
	lightsOn = false;
	lastRefresh = millis();
}

Location Aircraft::initControls()
{
	flightControls.init();
	tracker.init();
	powerUnit.init();
	controlsInitialized = true;

	landingGear.attach(AIRCRAFT_SERVO_LANDING_GEAR_PIN);
	landingGear.write(90);
	landingGearDeployed = true;

	for(int i=0; i<MAX_CHAMBERS; i++) 
	{
		chambers[i].attach(CHAMBER_PINS[i]);
		chambers[i].write(0);
	}

	aircraftGroundAltitude = tracker.getAltitude();

	// to lock to, and establish communication with GPS satellites; also to dynamically get location of airport
	return getLocation();
}

Location Aircraft::getLocation() const
{
	return tracker.getGPSCoordinates();
}

float Aircraft::getBearing() const
{
	return tracker.getGPSBearing();
}

float Aircraft::getAltitude() const
{
	float alt = tracker.getAltitude();
	return(alt <= aircraftGroundAltitude + ALTITUDE_TOLERANCE ? 0 : alt);
}

bool Aircraft::isLandingGearDeployed() const
{
	return landingGearDeployed;
}

void Aircraft::retractLandingGear()
{
	if(landingGearDeployed)
	{
		for(int i=90; i>=0; i--) 
		{
			landingGear.write(i);
			delay(15);
		}
		landingGearDeployed = false;
	}
}

void Aircraft::deployLandingGear()
{
	if(!landingGearDeployed)
	{
		for(int i=0; i<=90; i++)
		{
			landingGear.write(i);
			delay(15);
		}
		landingGearDeployed = true;
	}
}

void Aircraft::powerUpEngines()
{
	for(int i=0; i<MAX_ENGINES; i++) engines[i].startEngine(ENGINE_PINS[i], (i==MAX_ENGINES-1 ? true : false));
	enginesPowered = true;
}

void Aircraft::powerDownEngines()
{
	for(int i=0; i<MAX_ENGINES; i++) engines[i].stopEngine();
	enginesPowered = false;
}

void Aircraft::setEngineThrottle(const int percent) const
{
	for(int i=0; i<MAX_ENGINES; i++) engines[i].setThrottle(percent);
}

void Aircraft::setEngineThrottleFull() const
{
	for(int i=0; i<MAX_ENGINES; i++) engines[i].fullThrottle();
}

void Aircraft::dropPackage(const int chamber) const
{
	if(chamber<0 || chamber>=MAX_CHAMBERS) return;
	chambers[chamber].write(90);
	delay(1500);
	chambers[chamber].write(0);
}

void Aircraft::openDoor(const int chamber)
{
	if(chamber<0 || chamber>=MAX_CHAMBERS || chamberDoorsOpen[chamber]) return;
	for(int i=0; i<=90; i++)
	{
		chambers[chamber].write(i);
		delay(20);
	}
	chamberDoorsOpen[chamber] = true;
}

void Aircraft::closeDoor(const int chamber)
{
	if(chamber<0 || chamber>=MAX_CHAMBERS || !chamberDoorsOpen[chamber]) return;
	for(int i=90; i>=0; i--)
	{
		chambers[chamber].write(i);
		delay(20);
	}
	chamberDoorsOpen[chamber] = false;
}

bool Aircraft::isLowOnFuel() const
{
	return powerUnit.isLow();
}

int Aircraft::getFuel() const
{
	return powerUnit.getPercentage();
}

float Aircraft::getPitch() const
{
	return tracker.getPitch();
}

float Aircraft::getRoll() const
{
	return tracker.getRoll();
}

float Aircraft::getYaw() const
{
	return tracker.getYaw();
}

void Aircraft::setFlaps(const int f) const
{
	flightControls.setFlaps(f);
}

void Aircraft::setRudder(const int r) const
{
	flightControls.setRudder(r);
}

void Aircraft::setElevators(const int e) const
{
	flightControls.setElevators(e);
}

void Aircraft::setLeftAileron(const int a) const
{
	flightControls.setLeftAileron(a);
}

void Aircraft::setRightAileron(const int a) const
{
	flightControls.setRightAileron(a);
}

float Aircraft::getVelocity() const
{
	return tracker.getGPSVelocity();
}

void Aircraft::deploySpoilers() const
{
	flightControls.deploySpoilers();
}

void Aircraft::retractSpoilers() const
{
	flightControls.retractSpoilers();
}

bool Aircraft::isLocationValid() const
{
	bool f1 = tracker.measureGPSLocation();
	bool f2 = (tracker.fixAge <= GPS_STALE_AGE ? true : false);

	return(f1 && (landingGearDeployed || f2));
}

bool Aircraft::areControlsInitialized() const
{
	return controlsInitialized;
}

bool Aircraft::areEnginesPowered() const
{
	return enginesPowered;
}

void Aircraft::refresh()
{
	long currentRefresh = millis();
	if(HAS_LIGHTS)
	{
		if(lightsOn)
		{
			if(currentRefresh - lastRefresh >= LIGHTS_ON_DURATION)
			{
				digitalWrite(AIRCRAFT_LIGHTS_PIN, LOW);
				lightsOn = false;
			}
		} else {
			if(currentRefresh - lastRefresh >= LIGHTS_OFF_DURATION)
			{
				digitalWrite(AIRCRAFT_LIGHTS_PIN, HIGH);
				lightsOn = true;
			}
		}
	}
	lastRefresh = currentRefresh;
}
