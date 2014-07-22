/*  ArduinoTeam Experiment
    
    This source code is implemented for a Master Node in a WSN.
    The Master Node (or Cluster Head) is responsible for retrieve
    information from Slaves Nodes and execute data fusion.
    
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
      XBee needs to be Coordinator (API mode).
*/



/* ######### Includes ########## */



/* #### Functions Prototypes ### */



/* ######## Definitions ######## */
#define SIZE 10
#define LEN 22



/* ##### Global Variables ###### */
String message;
char messageReceived[SIZE];



/* ##### Arduino Functions ##### */
void setup(){
  Serial.begin(9600);
  
  // Print on Serial Monitor who I am!
  Serial.println("\n###############################");
  Serial.println("##### Starting the Master #####");
  Serial.println("###############################");
}

void loop(){  
  // Send a broadcast message to other devices.
  // Use: "r" - reconfigure, "s" - sleep, "t" - status.
  Serial.print("\nSended: ");
  setMessageToSend("t");
  txRequest(&message);
  
  // Wait to receive some response.
  waitForResponse();
  
  // Wait to send another requisition.
  waitFor(3000);
}



/* ###### Other Functions ###### */


/* 
 * Tx Request: (16-bit Address). A TX Request message will
 * cause the module to send RF Data as an RF Packet. See
 * page 62 of Xbee Datasheet.
 * Parameters:
 *   String* message = pointer to the string.
 */
void txRequest(String* message){
  
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
    byte payload[(sizeof(byte) * 20)];
    byte checksum;
  }txReq;
  
  txReq frame;

  /* ############### Build the frame. ############### */
  // Start byte (byte 1).
  frame.startByte = 0x7E;
  
  // Lenght (bytes 2 and 3).
    // Note: MSB first, LSB last.
  frame.lengthMSB = 0x00;
  frame.lengthLSB = 0x05 + (byte)(*message).length();
  
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
    // Get the 'message' content and convert to byte.
  (*message).getBytes(frame.payload,sizeof(frame.payload));
  
  // Checksum.
    // Note: To calculate, do not include frame delimiter and length.
    // Summing frame values, EXCEPT the values into message string.
    // It will be done below.
  long sum = 0x01 + 0xFF + 0xFF + 0x01;
  /* ################################################ */
  
  int i = 0, j = 0;
  // Build the frame 'packet' to send.
    // Note: There are 9 fields + the length of 'message' string.
  int bufferLength = 9 + (*message).length();
  byte buffer[bufferLength];
  
  // Fill 'buffer' fields.
  buffer[0] = frame.startByte;
  buffer[1] = frame.lengthMSB;
  buffer[2] = frame.lengthLSB;
  buffer[3] = frame.frameType;
  buffer[4] = frame.frameID;
  buffer[5] = frame.destAddrMSB;
  buffer[6] = frame.destAddrLSB;
  buffer[7] = frame.options;
  j = 8;
  // Fill 'buffer' with respect to 'message' payload.
  for(i=0;frame.payload[i] != '\0'; i++){
    buffer[j] = frame.payload[i];
    // Sum the value into 'sum'.
    sum = sum + buffer[j];
    j++;
  }
  
  // Do final checksum math.
  frame.checksum = 0xFF - (sum & 0xFF);
  buffer[j++] = frame.checksum;
  buffer[j++] = '\0'; // Indicates the buffer end.
  
  // Send the message via Serial.
  Serial.write(buffer, bufferLength);
  Serial.println(); // Just break the line.
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


/* 
 * This funcion sets the message that will be sent to other
 * device.
 * Parameters:
 *  String option = a string to send. It can be only a sim-
 *  ple character or a complete string. E.g.: "t", "+9:5010".
 */
void setMessageToSend(String option){
  // Set the global variable 'message'.
  message = option;
}


/* 
 * This funcion gets the message that will be received from
 * other device.
 * No parameters.
 * 
 */
void waitForResponse(){
  // Variable to control position in a Char Array.
  int index = 0;
  
  // Variable to store the response;
  String received = "";
  
  // Reading and interpreting the received message.
  if(Serial.available() > LEN){
    for(int i = 0; i <= LEN; i++){
      // The position 14 is the beggining of the payload.
      // LEN - 1 is to discard the checksum byte.
      if((i >= 13) && (i < LEN)){
        messageReceived[index] = Serial.read();
        Serial.print(messageReceived[index], HEX);
        Serial.print(" ");
        index++;
        received += (String)messageReceived[index];
      }
      else{
        // Print the frame content.
        Serial.print(Serial.read(), HEX);
        Serial.print(" ");
      }
    }
    Serial.println();
    
    // Print the message in the payload.
    Serial.print("Received: ");
    Serial.println(received);
  }
}
