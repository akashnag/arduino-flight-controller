// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Engine_h
	#define Engine_h

	#include <Arduino.h>
	#include <Servo.h>

	class Engine
	{
		private:
			Servo engine;			
			int enginePower;

		public:
			Engine();
			void startEngine(const int, const bool);
			void stopEngine();

			void setThrottle(const int);
			void adjustThrottle(const int);
			void fullThrottle();
	};
#endif
