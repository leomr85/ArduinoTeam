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
void txRequest(String* message);
void waitFor(long time);



/* ######## Definitions ######## */
#define SIZE 5
#define LEN 9
#define PAYLOADPOS 8



/* ##### Global Variables ###### */
String message = "";
char messageReceived[SIZE];



/* ##### Arduino Functions ##### */
void setup(){
  Serial.begin(9600);
  
  // Print on Serial Monitor who I am!
  Serial.println("\n###############################");
  Serial.println("###### Starting the Slave #####");
  Serial.println("###############################");
}

void loop(){
  // The Slave only waits for requisitions.
  waitForRequisition();
}



/* ###### Other Functions ###### */


/* 
 * On a reconfigure request, call this function to configure
 * some parameter at the node.
 * No parameters.
 *
 */
void setConfigure(){
  
}


/* 
 * On a sleep request, call this function to put the node in
 * sleep mode (XBee and Arduino, if possible).
 * No parameters.
 *
 */
void setSleepMode(){
  
}


/* 
 * On a Status request, call this function to get the batte-
 * ry voltage and temperature values.
 * No parameters.
 *
 */
String getStatus(){
  String msgToSend = "";
  
  // Read the temperature sensor value.
  int sensor_t;
  sensor_t = 25; // For tests only.
  
  // Read the battery voltage value.
  long sensor_b;
  sensor_b = 5010; // For tests only.
  
  if(sensor_t < 0){
    msgToSend = ((String)sensor_t +":"+ (String)sensor_b);
  }
  else{
    msgToSend = ("+" + (String)sensor_t +":"+ (String)sensor_b);
  }
  
  return (msgToSend);
}


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
 * This funcion waits for a requisition and choose the appro-
 * priate option according to the payload.
 * No parameters.
 *
 */
void waitForRequisition(){
  int index = 0;
  
  // Reading and interpreting the received message.
  if(Serial.available() > LEN){
    for(int i = 0; i <= LEN; i++){
      // The position 14 is the beggining of the payload.
      if(i == PAYLOADPOS){
        messageReceived[index] = Serial.read();
        Serial.print(messageReceived[index], HEX);
        Serial.print(' ');
      }
      else{
        Serial.print(Serial.read(), HEX);
        Serial.print(" ");
      }
    }
    Serial.println();
    
    // Print the message in the payload.
    Serial.print("Received: ");
    Serial.println(messageReceived[0]); // For tests only.
    
    if(messageReceived[index] == 'r'){
      setConfigure();
    }
    if(messageReceived[index] == 's'){
      setSleepMode();
    }
    if(messageReceived[index] == 't'){
      //Serial.println(getStatus());
      setMessageToSend(getStatus());
      Serial.print("Sended: ");
      txRequest(&message);
    }
    
    Serial.println();
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
