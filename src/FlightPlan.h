// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef FlightPlan_h
	#define FlightPlan_h

	#include <Arduino.h>
	#include "Waypoint.h"

	struct FlightPlanNode
	{
		Waypoint *waypoint;
		FlightPlanNode *next;
		FlightPlanNode *prev;

		FlightPlanNode(const Waypoint&);
	};

	class FlightPlan
	{
		private:
			int count;
			FlightPlanNode *front;
			FlightPlanNode *rear;

		public:
			FlightPlan();
			void appendWaypoint(const Waypoint&);
			void insertWaypoint(const int, const Waypoint&);
			void removeFirstWaypoint();
			void removeWaypoint(const int);
			int getWaypointCount() const;
			Waypoint* getWaypoint(const int) const;
			void clearAll();
	};
#endif
