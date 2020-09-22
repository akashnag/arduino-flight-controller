// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef AirTrafficControl_h
	#define AirTrafficControl_h

	#include <Arduino.h>
	#include "WirelessTransceiver.h"
	#include "ATCCommand.h"

	class AirTrafficControl
	{
		private:
			WirelessTransceiver comms;
			
		public:
			AirTrafficControl();
			void establishCommunication();
			void inform(const String) const;
			int receiveCommand(int&) const;
			void clearedToSend() const;
			void informCTS(const String) const;
			bool withinRange() const;
	};
#endif
