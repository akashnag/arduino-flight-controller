// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Aircraft_h
	#define Aircraft_h

	#include "Location.h"
	#include "Engine.h"
	#include "FlightControlSurfaces.h"
	#include "Battery.h"
	#include "Tracker.h"
	#include "ATCCommand.h"
	#include "Global.h"

	class AutoPilot;
	
	class Aircraft
	{
		private:
			Engine engines[MAX_ENGINES];
			FlightControlSurfaces flightControls;
			Battery powerUnit;
			Tracker tracker;
			Servo landingGear;
			Servo chambers[MAX_CHAMBERS];

			float aircraftGroundAltitude;

			bool landingGearDeployed;
			bool controlsInitialized;
			bool enginesPowered;
			bool chamberDoorsOpen[MAX_CHAMBERS];

			bool lightsOn;
			long lastRefresh;
			
		public:
			Aircraft();
			Location initControls();
			void powerUpEngines();
			void powerDownEngines();

			void retractLandingGear();
			void deployLandingGear();
			
			bool isLandingGearDeployed() const;
			bool areControlsInitialized() const;
			bool areEnginesPowered() const;

			void setEngineThrottle(int) const;
			void setEngineThrottleFull() const;
			
			void setFlaps(const int) const;
			void setRudder(const int) const;
			void setElevators(const int) const;
			void setLeftAileron(const int) const;
			void setRightAileron(const int) const;
			void deploySpoilers() const;
			void retractSpoilers() const;
			
			bool isLocationValid() const;
			Location getLocation() const;
			float getBearing() const;
			float getAltitude() const;
			float getPitch() const;
			float getRoll() const;
			float getYaw() const;
			float getVelocity() const;

			void dropPackage(const int) const;
			void openDoor(const int);
			void closeDoor(const int);
			
			int getFuel() const;
			bool isLowOnFuel() const;

			void refresh();
	};
#endif
