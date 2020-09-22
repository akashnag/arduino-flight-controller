// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "FlightPlan.h"

FlightPlanNode::FlightPlanNode(const Waypoint &wp)
{
	waypoint = new Waypoint(wp);
	prev = NULL;
	next = NULL;
}

// -------------------------------------------------------

FlightPlan::FlightPlan()
{
	count = 0;
	front = NULL;
	rear = NULL;
}

void FlightPlan::appendWaypoint(const Waypoint &wp)
{
	FlightPlanNode *temp = new FlightPlanNode(wp);
	if(count==0)
	{
		front = temp;
		rear = temp;
	} else {
		rear->next = temp;
		temp->prev = rear;
		rear = temp;
	}
	count++;
}

void FlightPlan::removeFirstWaypoint()
{
	if(count==0) return;
	if(count==1)
	{
		delete front;
		front = NULL;
		rear = NULL;
	} else {
		FlightPlanNode *temp = front;
		front = front->next;
		front->prev = NULL;
		delete temp;
	}
	count--;
}

void FlightPlan::removeWaypoint(const int index)
{
	if(count==0) return;
	if(index==0)
		removeFirstWaypoint();
	else {
		FlightPlanNode *temp = front;			// this is the node that will be deleted
		FlightPlanNode *tempPrior = NULL;
		FlightPlanNode *x = NULL;
		
		for(int i=0; i<index-1; i++) 
		{
			tempPrior = temp;
			temp = temp->next;
		}

		x = temp->next;
		tempPrior->next = x;
		x->prev = tempPrior;
		delete temp;
		count--;
	}
}

void FlightPlan::insertWaypoint(const int index, const Waypoint &wp)
{
	if(count==0 || index==count-1)
		appendWaypoint(wp);
	else {
		FlightPlanNode *newNode = new FlightPlanNode(wp);
		FlightPlanNode *temp = front;
		FlightPlanNode *tempPrior = NULL;
		
		for(int i=0; i<index-1; i++) 
		{
			tempPrior = temp;
			temp = temp->next;
		}

		tempPrior->next = newNode;
		newNode->prev = tempPrior;
		newNode->next = temp;
		temp->prev = newNode;
		count++;
	}
}

Waypoint* FlightPlan::getWaypoint(const int index) const
{
	if(index < 0 || index >= count) return NULL;
	
	FlightPlanNode *temp = front;
	int i=0;
	while(i < index) temp = temp->next;
	return temp->waypoint;
}

void FlightPlan::clearAll()
{
	while(count > 0) removeFirstWaypoint();
}

int FlightPlan::getWaypointCount() const
{
	return count;
}
