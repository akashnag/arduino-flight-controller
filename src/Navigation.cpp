// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Navigation.h"

float getDesiredBearing(const Location &from, const Location &to)
{
	// ASSUMPTION: the earth is flat

	// RECTIFY this function to take into account the directionality: i.e. which is FROM and which is TO, bcoz
	// if reversed, the bearing will change by 180 degrees

	float deltaLat = to.getLatitude() - from.getLatitude();
	float deltaLon = to.getLongitude() - from.getLongitude();

	float theta = atan2(deltaLat, deltaLon);
	float deg = radToDegrees(theta);

	return geometricAngleToBearing(deg);
}

float geometricAngleToBearing(const float angle)
{
	if(angle <= 90)
		return 90-angle;
	else if(angle >= 90 && angle <= 270)
		return 360 - (angle-90);
	else			// i.e. angle > 270
		return 90 + (360-angle);
}

float bearingToGeometricAngle(const float bearing)
{
	if(bearing <= 90)
		return 90-bearing;
	else if(bearing > 90 && bearing <= 180)
		return(360 - (bearing-90));
	else if(bearing > 180 && bearing <= 270)
		return(270 - (bearing - 180));
	else // > 270
		return(180 - (bearing - 270));
}

float getDesiredBearingBiPolar(const Location &from, const Location &to)
{
	float c = getDesiredBearing(from,to);
	if(c <= 180)
		return c;
	else
		return c - 360;
}

Location getLocationBeforeLocation(const Location &loc, const float bearing, const float distance)
{
	// ASSUME that the earth is flat

	/*
	 * Known location:	(Ax, Ay)
	 * Desired location: (Lx, Ly)
	 * Known bearing (geometric): M
	 * Distance between known and desired location: D
	 * 
	 * Eqn(1):	y = Ay - M.(Ax-x)
	 * Eqn(2):	D = root((Lx-Ax)^2 + (Ly-Ay)^2)
	 * 
	 * Put y=Ly and x=Lx in Eqn(1) to get Ly in terms of Lx only 
	 * Put that relative value of Ly in Eqn(2) to get the concrete value of Lx
	 * Put the concrete value of Lx in Eqn(1) to get the concrete value of Ly
	 * Return (Lx,Ly)
	 */

	const float Ay = loc.getLatitude();
	const float Ax = loc.getLongitude();
	const float D = distance;
	const float M = bearingToGeometricAngle(bearing);

	const float P = Ay - (M * Ax);
	const float Q = pow(Ax,2) + (2 * pow(Ay,2)) - pow(D,2);
	const float R = 2 * (Ax + Ay - P);
	const float S = (Q - (2 * Ay * P) + pow(P,2));

	// Reduced Eqn for Lx:   (2 * Lx^2) - (R * Lx) + S = 0

	const float A = 2;
	const float B = -R;
	const float C = S;

	const float discriminant = sqrt(pow(B,2) - (4*A*C));
	const float denominator = (2 * A);

	const float Lx_root1 = (-B + discriminant)/denominator;
	const float Lx_root2 = (-B - discriminant)/denominator;
	
	// Putting these two roots in Eqn(1) we get:
	const float Ly_root1 = Ay - (M*(Ax - Lx_root1));
	const float Ly_root2 = Ay - (M*(Ax - Lx_root2));
	
	// ADD CODE: Decide which is the correct location: (Lx_root1, Ly_root1) OR (Lx_root2, Ly_root2) and return it
	
	return Location(0,0);
}
