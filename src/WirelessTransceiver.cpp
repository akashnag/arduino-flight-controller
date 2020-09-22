// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "WirelessTransceiver.h"
#include <string.h>

WirelessTransceiver::WirelessTransceiver()
{
	payload.senderID = AIRCRAFT_ADDRESS;
}

void WirelessTransceiver::begin()
{
	radio = new RF24(WT_NRF24L01_PIN1, WT_NRF24L01_PIN2);
	radio->begin();
	radio->setPALevel(RF24_PA_LOW);		// or RF24_PA_MAX
	
	radio->openWritingPipe(AIRCRAFT_ADDRESS);
	radio->openReadingPipe(1, ATC_ADDRESS);

	role = RECEIVER;
	radio->startListening();
}

Payload* WirelessTransceiver::sendData(const String statusText) const
{
	payload.timestamp = micros();
	payload.message = statusText;
	payload.parameter = 0;

	role = SENDER;
	radio->stopListening();
	if (!radio->write(&payload, sizeof(Payload))) return false;

	role = RECEIVER;
	radio->startListening();

	unsigned long started_waiting_at = micros();
	boolean timeout = false;

	while (!radio->available())
	{
		if(micros() - started_waiting_at > WT_TIMEOUT_PERIOD) 
		{
			timeout = true;
			break;
		}
	}

	if(timeout) return NULL;
    
    Payload *reply = new Payload();
    radio->read(reply, sizeof(Payload));
	return reply;
}

Payload* WirelessTransceiver::receive() const
{
	Payload *dataReceived = new Payload();

	if(radio->available())
	{
		while(radio->available())
		{
			radio->read(dataReceived, sizeof(Payload));
		}

		payload.timestamp = micros();
		payload.message = "ACK";
		payload.paramter = 0;		
		role = SENDER;
		radio->stopListening();
		radio->write(&payload, sizeof(Payload));
		role = RECEIVER;
		radio->startListening();

		return dataReceived;
	} else {
		return NULL;
	}
}
