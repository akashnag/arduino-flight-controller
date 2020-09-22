// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Battery_h
	#define Battery_h

	#include <Arduino.h>
	#include "Global.h"

	class Battery
	{
		private:
			float getBatteryVoltage() const;
			float getCellVoltage(int) const;
		
		public:
			Battery();
			void init() const;
			int getPercentage() const;
			bool isLow() const;
	};
#endif
