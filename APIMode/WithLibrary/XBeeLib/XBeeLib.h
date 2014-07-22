/*
	XBeeLib - Library for XBee Modules.
	Created by Leonardo M. Rodrigues, July 17, 2014.
	Released into the puclic domain.
*/

#ifndef XBeeLib_h
#define XBeeLib_h

#include "Arduino.h"

class XBeeLib
{
	public:
		XBeeLib(int coordinatorEnable);
		void helloWorld();
		void waitFor(long time);
		void txRequest(String* message);
		void rxPacket();
		String getStatus();
		void getBattVoltage(long *battVoltage);
		
		void setPayload(String message);
		String getPayload();
		String* getPayloadAddress();
		
		void setReceived(String message);
		void addReceived(String item);
		void clearReceived();
		String getReceived();
		int getCoordinatorEnabled();
		void setCoordinatorEnabled(int type);
	
	private:
		// Coordinator Enable (0 = Device | 1 = Coordinator).
		int _coordinatorEnabled;
		
		// Frame payload.
		String _payload;
		
		// Payload received in the frame.
		String _received;
		
};

#endif