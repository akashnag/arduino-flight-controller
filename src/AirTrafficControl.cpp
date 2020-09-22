// ---------------------------------------------------------------------------------------------
//  Copyright (c) Akash Nag. All rights reserved.
//  Licensed under the MIT License. See LICENSE.md in the project root for license information.
// ---------------------------------------------------------------------------------------------

#include "AirTrafficControl.h"

AirTrafficControl::AirTrafficControl()
{
	
}

void AirTrafficControl::establishCommunication()
{
	comms.begin();
}

void AirTrafficControl::inform(const String message) const
{
	comms.sendData(message);
}

void AirTrafficControl::clearedToSend() const
{
	comms.sendData("CTS");
}

void AirTrafficControl::informCTS(const String message) const
{
	comms.sendData(message);
	comms.sendData("EOM");
}

int AirTrafficControl::receiveCommand(int &param) const
{
	String comm = comms.receive();
	comm.trim();
	if(comm.equalsIgnoreCase("ERROR")) return -1;
	
	int pos = comm.indexOf(' ');
	
	if(pos == -1) {
		param = 0;
		return comm.toInt();
	} else {
		param = comm.substring(pos+1).toInt();
		return comm.substring(0,pos).toInt();
	}
}

bool AirTrafficControl::withinRange() const
{
	int p, tries=0;
	do {
		informCTS("HELLO");
		tries++;		
	} while(tries < MAX_ATC_COMM_RETRIES && receiveCommand(p) == ATCC_NOCOMM);
	
	return(tries<MAX_ATC_COMM_RETRIES);
}
