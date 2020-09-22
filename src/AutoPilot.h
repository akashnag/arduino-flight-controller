// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef AutoPilot_h
	#define AutoPilot_h

	#include "Navigation.h"
	#include "Aircraft.h"
	#include "Global.h"
	#include "MathUtility.h"
	
	class AutoPilot
	{
		private:
			Aircraft *aircraft;

			Location *targetLocation;
			float targetAltitude;
			bool isTargetLocationValid;
			bool isTargetAltitudeValid;

			bool setAilerons(const int) const;				// degree must be bipolar: -180 to +180
			bool setElevators(const int) const;				// degree must be bipolar: -180 to +180
			bool setBearing(const float, const int) const;

			bool isStalled();
			void recoverFromStall();
			bool isBeingSweptBack();
			void preventBeingSweptBack();
			
		public:
			AutoPilot(const Aircraft*);

			void testFlightControls() const;
			void takeOff() const;
			void land(const Airport&) const;
			void landImmediately(const float) const;

			void setCruiseMode() const;

			void setCourse(const Location&);
			bool keepOnCourse(const int) const;
			void setAltitude(const float);
			bool maintainAltitude(const int) const;

			void stabilizeRoll() const;
			void stabilizePitch() const;

			void turn(const float) const;							// degree must be bipolar
			
			float getETA() const;									// return ETA in seconds
	};
#endif
