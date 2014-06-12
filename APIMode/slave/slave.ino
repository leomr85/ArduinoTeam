/*  ArduinoTeam Experiment
    
    This source code is implemented for a Slave Node in a WSN.
    The Slave Node is responsible for capture the sensor
    values and transmit it back to the Master Node.
    
    Developers Team:
      Carlos Barros Montez
      Leonardo Martins Rodrigues
      Aujor Tadeu Cavalca
      Daniel Floriani da Silva
      Gabriel Goulart
      Murilo Rodegheri
   
    Institution:
      Federal University of Santa Catarina (UFSC)
        Department of Automation and Systems Engineering
        PO Box 476, Florianópolis, SC
        88040-900 Brazil
   
    Date of creation: 11/Jun/2014

    This source code is for specific purpose and the authors do not
    guarantee it to work in other cases. As an open source code, its
    use is free. Please keep the main authors list in the header.

    Note:
      XBee needs to be End Device (API mode).
*/



/* ######### Includes ########## */



/* #### Functions Prototypes ### */
void setup();
void loop();
void txRequest();
void workingExample();
void waitFor(long time);



/* ######## Definitions ######## */



/* ##### Global Variables ###### */



/* ##### Arduino Functions ##### */
void setup(){
  Serial.begin(9600);
}

void loop(){
  waitFor(2000);  
  txRequest();
}



/* ###### Other Functions ###### */

/* 
 * Tx Request: (16-bit Address). A TX Request message will
 * cause the module to send RF Data as an RF Packet. See
 * page 62 of Xbee Datasheet.
 * No parameters.
 * 
 */
void txRequest(){
  // Creating a struct to handle the frame.
  typedef struct txRequest{
    byte startByte;
    byte lengthMSB;
    byte lengthLSB;
    byte frameType;
    byte frameID;
    byte destAddrMSB;
    byte destAddrLSB;
    byte options;
    byte payload;
    byte checksum;
  }txReq;
  
  txReq frame;

  /* ############### Build the frame. ############### */
  // Start byte (byte 1).
  frame.startByte = 0x7E;
  
  // Lenght (bytes 2 and 3).
    // Note: MSB first, LSB last.
  frame.lengthMSB = 0x00;
  frame.lengthLSB = 0x06;
  
  // Frame type (byte 4).
  frame.frameType = 0x01;
  
  // Frame ID (byte 5).
    // Note: 0x0 = Disable Response frame.
  frame.frameID = (byte)0x00;
  
  // Destination Address (bytes 6 and 7).
    // Note: MSB first, LSB last.
    // Note: For Broadcast use 0xFFFF.
  frame.destAddrMSB = 0xFF;
  frame.destAddrLSB = 0xFF;
 
  // Options (Byte 8).
    // Note: 0x01 = Disable ACK.
    // Note: 0x04 = Send packet with broadcast PAN ID.
  frame.options = 0x01;

  // RF Data (Bytes 9 -> n).
    // Note: Up to 100 bytes per packet.
  frame.payload = 0x48;
  
  // Checksum.
    // Note: To calculate, do not include frame delimiter and length.
  long sum = 0x01 + 0xFF + 0xFF + 0x01 + 0x48;
  frame.checksum = 0xFF - (sum & 0xFF);
  /* ################################################ */
  
  int bufferLength = sizeof(frame);
  byte buffer[] = {frame.startByte, frame.lengthMSB, frame.lengthLSB, frame.frameType, frame.frameID, frame.destAddrMSB, frame.destAddrLSB, frame.options, frame.payload, frame.checksum};
  Serial.write(buffer, bufferLength);
}

/* 
 * This funcion waits for 'time' miliseconds (it does not do any-
 * thing in practice, just wait).
 * Parameters:
 *  long time = the time (in ms) that the function should wait.
 */
void waitFor(long time){
  // millis function returns the number of milliseconds since the
  // program started. 
  
  // Create two variables: initial and end time.
  unsigned long ini_time = millis();
  unsigned long end_time = ini_time;
  
  while(end_time - ini_time <= time){
    end_time = millis();
  }
}
