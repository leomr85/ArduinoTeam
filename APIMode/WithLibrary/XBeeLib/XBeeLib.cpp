/*
	XBeeLib - Library for XBee Modules.
	Created by Leonardo M. Rodrigues, July 17, 2014.
	Released into the puclic domain.
*/

#include "Arduino.h"
#include "XBeeLib.h"



/* ############################# */
/* ##### Class constructor ##### */
/* ############################# */
XBeeLib::XBeeLib(int coordinatorEnable)
{
	setCoordinatorEnabled(coordinatorEnable);
}



/* ############################# */
/* ##### Functions/Methods ##### */
/* ############################# */


// helloWorld - Function for tests only.
void XBeeLib::helloWorld()
{
	Serial.println("Hello World.");
}


// waitFor - Function to wait for 'time' milliseconds.
void XBeeLib::waitFor(long time)
{
	unsigned long ini_time = millis();
	unsigned long end_time = ini_time;
	
	while(end_time - ini_time <= time){
		end_time = millis();
	}
}


// txRequest - Function to send RF data as a RF Packet.
void XBeeLib::txRequest(String* message)
{
	int i = 0;
	int j = 0;
	int bufferLength = 9 + (*message).length();
	
	byte buffer[bufferLength];
	byte payload[sizeof(byte) * 100]; // Packet Payload (up to 100 bytes).
	byte checksum;
	
	// Serial.println(sizeof(*message) * (*message).length());
	
	// Start byte (byte 1).
	buffer[0] = 0x7E;
	
	// Lenght (bytes 2 and 3).
		// Note: MSB first, LSB last.
	buffer[1] = 0x00;
	buffer[2] = 0x05 + (byte)(*message).length();
	
	// Frame type (byte 4).
	buffer[3] = 0x01;
	
	// Frame ID (byte 5).
		// Note: 0x0 = Disable Response frame.
	buffer[4] = 0x00;
	
	// Destination Address (bytes 6 and 7).
		// Note: MSB first, LSB last.
		// Note: For Broadcast use 0xFFFF.
	buffer[5] = 0xFF;
	buffer[6] = 0xFF;
	
	// Options (Byte 8).
		// Note: 0x01 = Disable ACK.
		// Note: 0x04 = Send packet with broadcast PAN ID.
	buffer[7] = 0x01;
	
	// RF Data (Bytes 9 -> n).
		// Note: Up to 100 bytes per packet.
		// Get the 'message' content and convert to byte.
    (*message).getBytes(payload,sizeof(payload));
	
	// Checksum.
		// Note: To calculate, do not include frame delimiter and length.
		// Summing frame values, EXCEPT the values into message string.
		// It will be done below.
	checksum = 0x01 + 0xFF + 0xFF + 0x01;
	
	j = 8;
	for(i=0; payload[i] != '\0'; i++){
		buffer[j] = payload[i];
		checksum = checksum + buffer[j];
		j++;
	}
	
	checksum = 0xFF - (checksum & 0xFF);
	buffer[j++] = checksum;
	buffer[j++] = '\0';
	
	// For tests.
	// Serial.print("Verify: ");
	// for(j=0; j < 17; j++){
	// 	Serial.print(buffer[j],HEX);
	// 	Serial.print(" ");
	// }
	// Serial.println();
	
	Serial.print("\n>>> ");
	Serial.write(buffer, bufferLength);
	Serial.println();
}


// rxPacket - Function to read the frame received.
void XBeeLib::rxPacket()
{
	int index;
	
	int size;
	int length;
	int payloadPosition;
	
	if(getCoordinatorEnabled() == 1){
		size = 8;
		length = 22;
		payloadPosition = 14;
	}
	else{
		size = 1;
		length = 9;
		payloadPosition = 8;
	}
	
	clearReceived();
	char frame[size];
	index = 0;
	
	if(Serial.available() > length){
		Serial.println();
		for(int i = 0; i <= length; i++){
			if((i >= payloadPosition) && (i < length)){
				frame[index] = Serial.read();
				addReceived((String)((char)frame[index]));
				Serial.print(frame[index], HEX);
				Serial.print(" ");
				index++;			
			}
			else{
			    Serial.print(Serial.read(), HEX);
			    Serial.print(" ");
			}
		}
		
	    Serial.print("\n<<< ");
		Serial.println(getReceived());
		
		if(getCoordinatorEnabled() == 0){
		    if(getReceived() == "r"){
		      //setConfigure();
		    }
		    if(getReceived() == "s"){
		      //setSleepMode();
		    }
		    if(getReceived() == "t"){
				setPayload(getStatus());
				txRequest(getPayloadAddress());
		    }
		}
	}
}


// getStatus - Function to sense Temperature and Voltage.
String XBeeLib::getStatus()
{
	String msgToSend = "";
  
	// Read the temperature sensor value.
	int sensor_t;
	sensor_t = 25; // For tests only.

	// Read the battery voltage value.
	long sensor_b;
	getBattVoltage(&sensor_b); // For tests only.

	if(sensor_t < 0){
		msgToSend = ((String)sensor_t +":"+ (String)sensor_b);
	}
	else{
		msgToSend = ("+" + (String)sensor_t +":"+ (String)sensor_b);
	}

	return (msgToSend);
}

// getBattVoltage - Function to verify the battery voltage.
void XBeeLib::getBattVoltage(long *battVoltage){
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  // Wait for Vref to settle.
  waitFor(5);
  // Convert
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  *battVoltage = ADCL;
  *battVoltage |= ADCH<<8;
  // Back-calculate AVcc in mV.
  *battVoltage = 1126400L / *battVoltage;
}


// setPayload - Function to set a new payload.
void XBeeLib::setPayload(String message)
{
	_payload = message;
}

// getPayload - Function to get the payload value.
String XBeeLib::getPayload()
{
	return (_payload);
}

// getPayload - Function to get the payload value.
String* XBeeLib::getPayloadAddress()
{
	return &(_payload);
}


// setReceived - Function to set a value to received message.
void XBeeLib::setReceived(String message)
{
	_received = message;
}

// addReceived - Function to add an item to received message.
void XBeeLib::addReceived(String item)
{
	_received += item;
}

// clearReceived - Function to clear the '_received' content.
void XBeeLib::clearReceived()
{
	_received = "";
}

// getReceived - Function to get the value of the received message.
String XBeeLib::getReceived()
{
	return (_received);
}


// getCoordinatorEnabled - Function to get the device type.
int XBeeLib::getCoordinatorEnabled(){
	return (_coordinatorEnabled);
}

// setCoordinatorEnabled - Function to set the device type.
void XBeeLib::setCoordinatorEnabled(int type){
	if(type == 1){
		_coordinatorEnabled = type;
	}
	else{
		// if (type != 1), then set as a device.
		_coordinatorEnabled = 0;
	}
}