// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Navigation_h
	#define Navigation_h

	#include <Arduino.h>
	#include "Location.h"

	class Location;

	float getDesiredBearing(const Location&, const Location&);				// returns bearing in degrees (0 deg = North, 90 deg = East, 180 deg = South, 270 deg = West)
	float getDesiredBearingBiPolar(const Location&, const Location&);		// returns bearing in degrees (0 deg = North, +90 deg = East, +180 deg = South, -90 deg = West)
	float bearingToGeometricAngle(const float);
	float geometricAngleToBearing(const float);
	Location getLocationBeforeLocation(const Location&, const float, const float);
#endif
