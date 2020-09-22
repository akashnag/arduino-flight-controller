// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#ifndef WirelessTransceiver_h
	#define WirelessTransceiver_h

	#include <Arduino.h>
	#include "Global.h"
	#include <SPI.h>
	#include <RF24.h>

	#define AIRCRAFT_ADDRESS "ARCFT";
	#define ATC_ADDRESS "ARTFC";

	struct Payload
	{
		char senderID[6];
		unsigned long timestamp;
		char message[256];
		int parameter;
	};

	typedef enum { SENDER=0, RECEIVER=1 } NodeRole;

	class WirelessTransceiver
	{
		private:
			RF24 *radio;
			Payload payload;
			NodeRole role;
	
		public:
			WirelessTransceiver();
			void begin();
			Payload* receive() const;
			Payload* sendData(const String) const;
	};
#endif
