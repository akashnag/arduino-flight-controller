// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Battery.h"

Battery::Battery()
{

}

void Battery::init() const
{
	analogReference(DEFAULT);
}

float Battery::getCellVoltage(const int cell) const
{
	int v = analogRead(CELL_PINS[cell]);
	int inpVolt = map(v, 0, 1023, 0, 500);
	return(inpVolt / 100.0);
}

float Battery::getBatteryVoltage() const
{
	float sum = 0;
	for(int i=0; i<BATTERY_CELL_COUNT; i++) sum += getCellVoltage(i);
	return sum;
}

int Battery::getPercentage() const
{
	float voltage = getBatteryVoltage();
	int p = ((voltage * 100) / (BATTERY_CELL_COUNT * BATTERY_CELL_FULL_THRESHOLD));
	return(p < 0 ? 0 : (p > 100 ? 100 : p));
}

bool Battery::isLow() const
{
	for(int i=0; i<BATTERY_CELL_COUNT; i++)
	{
		float cellVoltage = getCellVoltage(i);
		if(cellVoltage < BATTERY_CELL_LOW_THRESHOLD) return true;
	}
	return false;
}
