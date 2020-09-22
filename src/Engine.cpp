// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "Engine.h"

Engine::Engine()
{
	
}

void Engine::stopEngine()
{
	setThrottle(0);
	engine.detach();
}

void Engine::startEngine(const int pin, const bool shouldDelay)
{
	engine.attach(pin);
	engine.write(30);
	enginePower = 30;
	if(shouldDelay) delay(3000);
}

void Engine::setThrottle(const int percent)
{
	int power = (percent==0 ? 30 : map(percent, 1, 100, 40, 130));
	
	engine.write(power);
	enginePower = power;
}

void Engine::adjustThrottle(const int percent)
{
	int currentPercent = (enginePower==30 ? 0 : map(enginePower, 40, 130, 1, 100));
	int newPercent = currentPercent + percent;

	if(newPercent < 0) newPercent = 0;
	if(newPercent > 100) newPercent = 100;
	setThrottle(newPercent);
}

void Engine::fullThrottle()
{
	setThrottle(100);
}
