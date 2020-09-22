// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef MathUtility_h
	#define MathUtility_h

	#include <Arduino.h>
	
	// ------------------------ Functions --------------------------------
	float degToRadians(const float);
	float radToDegrees(const float);
	float easeOut(const float, const float, const float, const float);
	bool isInRange(const float, const float, const float);
	int unipolarToBipolarBearing(const int);
	int bipolarToUnipolarBearing(const int);
#endif
