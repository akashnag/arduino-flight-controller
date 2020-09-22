// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Waypoint.h"

Waypoint::Waypoint(const Waypoint &wp)
{
	const Location loc = *wp.location;
	bool d = wp.isDelivery;
	int p = wp.packageID;

	location = new Location(loc);
	isDelivery = d;
	packageID = p;

	if(!isDelivery)
		packageID = -1;
	else if(packageID < 0)
		isDelivery = false;
}

Waypoint::Waypoint(const Location &loc, const bool d, const int p)
{
	location = new Location(loc);
	isDelivery = d;
	packageID = p;

	if(!isDelivery)
		packageID = -1;
	else if(packageID < 0)
		isDelivery = false;
}

Location Waypoint::getLocation() const
{
	return *location;
}

bool Waypoint::isDeliveryPoint() const
{
	return isDelivery;
}

int Waypoint::getPackageID() const
{
	return packageID;
}

String Waypoint::toString() const
{
	String x = String(isDelivery ? "Y" : "N");
	x = x + "\t" + (isDelivery ? String(packageID) : "N/A") + "\t" + location->toString();
	return x;
}
