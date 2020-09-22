// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Location.h"
#include <math.h>

Location::Location(const Location &loc)
{
	latitude = loc.latitude;
	longitude = loc.longitude;	
}

Location::Location(const float lat, const float lon)
{
	latitude=lat;
	longitude=lon;
}

float Location::getLatitude() const
{
	return latitude;
}

float Location::getLongitude() const
{
	return longitude;
}

float Location::distanceTo(const Location &loc) const
{
	float lat1 = latitude, lat2 = loc.latitude;
	float lon1 = longitude, lon2 = loc.longitude;

	float phi_1 = degToRadians(lat1);
	float phi_2 = degToRadians(lat2);

	float delta_phi = degToRadians(lat2 - lat1);
	float delta_lambda = degToRadians(lon2 - lon1);

	float a = pow(sin(delta_phi/2.0),2) + (cos(phi_1) * cos(phi_2) * pow(sin(delta_lambda/2.0),2));
	float c = 2 * atan2(sqrt(a), sqrt(1-a));
	
	return(c * EARTH_MEAN_RADIUS_METRES);
}

String Location::toString() const
{
	String x = String("[");
	x = x + latitude + ", " + longitude + "]";
	return x;
}

// -----------------------------------------------------------------

Airport::Airport(const Location *loc, const float b, const float a)
{
	location = loc;
	bearing = b;
	altitude = a;
}
