// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Location_h
	#define Location_h

	#include <Arduino.h>
	#include "Global.h"
	#include "MathUtility.h"	
	
	#define EARTH_MEAN_RADIUS_METRES 6371000
	
	class Location
	{
		private:
			float latitude;
			float longitude;
			
		public:
			Location(const Location&);
			Location(const float, const float);
			float distanceTo(const Location&) const;				// returns distance in metres
			float getLatitude() const;
			float getLongitude() const;
			String toString() const;
	};

	struct Airport
	{
		public:
			Location *location;
			float bearing;
			float altitude;

			Airport(const Location*, const float, const float);
	};
#endif
