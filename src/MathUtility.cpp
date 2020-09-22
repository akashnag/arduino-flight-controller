// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "MathUtility.h"

float degToRadians(const float deg)
{
	return((deg * PI) / 180.0);
}

float radToDegrees(const float rad)
{
	return((180.0 * rad) / PI);
}

float easeOut(const float stepCount, const float maxSteps, const float startValue, const float endValue)
{
	float delta = endValue - startValue;
	float t = stepCount, b = startValue, c = delta, d = maxSteps;
	return c * sin(t/d * (PI/2)) + b;
}

bool isInRange(const float value, const float rangeCap1, const float rangeCap2)
{
	if(rangeCap1 <= rangeCap2 && value >= rangeCap1 && value <= rangeCap2)
		return true;
	else if(rangeCap1 >= rangeCap2 && value <= rangeCap1 && value >= rangeCap2)
		return true;
	else
		return false;
}

int unipolarToBipolarBearing(const int bearing)
{
	int b = (bearing >= 360 || bearing < 0) ? bearing % 360 : bearing;	
	return((bearing > 180 && bearing < 360) ? b-360 : b);	
}

int bipolarToUnipolarBearing(const int bearing)
{
	return(bearing < 0 ? bearing+360 : bearing);
}
