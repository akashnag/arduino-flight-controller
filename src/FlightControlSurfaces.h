// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef FlightControlSurfaces_h
	#define FlightControlSurfaces_h

	#include <Arduino.h>
	#include <Servo.h>
	#include "Global.h"

	class FlightControlSurfaces
	{
		private:
			Servo flaps;
			Servo elevators;
			Servo rudder;
			Servo leftAileron;
			Servo rightAileron;
			Servo spoilers;
			
			bool spoilersDeployed;
			
			int flapMode;
			int leftAileronPosition;
			int rightAileronPosition;
			int elevatorPosition;
			int rudderPosition;

		public:
			FlightControlSurfaces();
			void init();
			
			void retractSpoilers();
			void deploySpoilers();

			void setFlaps(int);
			void setLeftAileron(int);
			void setRightAileron(int);
			void setElevators(int);
			void setRudder(int);
			
			int getLeftAileronPosition();
			int getRightAileronPosition();
			int getElevatorPosition();
			int getRudderPosition();
	};
#endif
