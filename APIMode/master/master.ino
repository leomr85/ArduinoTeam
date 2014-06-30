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
#define LEN 18



/* ##### Global Variables ###### */
char messageReceived[20];



/* ##### Arduino Functions ##### */
void setup(){
  Serial.begin(9600);
}

void loop(){
  int index = 0;
  
  // Reading and interpreting the received message.
  if(Serial.available() > LEN){
    for(int i = 0; i <= LEN; i++){
      // The position 14 is the beggining of the payload.
      // LEN - 1 is to discard the checksum byte.
      if((i >= 14) && (i <= LEN-1)){
        messageReceived[index] = Serial.read();
        Serial.print(messageReceived[index], HEX);
        Serial.print(' ');
        index++;
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
    Serial.println(messageReceived);
  }
  
    
  
}



/* ###### Other Functions ###### */
