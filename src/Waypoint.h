// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef Waypoint_h
	#define Waypoint_h

	#include "Location.h"

	class Waypoint
	{
		private:
			Location *location;
			bool isDelivery;
			int packageID;

		public:
			Waypoint(const Waypoint&);
			Waypoint(const Location&, const bool, const int);
			Location getLocation() const;
			bool isDeliveryPoint() const;
			int getPackageID() const;
			String toString() const;
	};
#endif
