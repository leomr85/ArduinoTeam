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
   
    Date of creation: 21/May/2014

    This source code is for specific purpose and the authors do not
    guarantee it to work in other cases. As an open source code, its
    use is free. Please keep the main authors list in the header.
      
*/


/* ######### Includes ########## */



/* #### Functions Prototypes ### */
void setup();
void loop();
void initializeMaster();
void processReceivedData();
void waitFor(long time);
void cleanString(String *string);
int identifySlaves();



/* ######## Definitions ######## */



/* ##### Global Variables ###### */
String message;
int slavesCounter;



/* ##### Arduino Functions ##### */
void setup(){
  initializeMaster();
}

void loop(){
  // Discover Slaves in the neighborhood.
//  slavesCounter = identifySlaves();
//  Serial.println(slavesCounter, DEC);
  
  // Send a message to the Slave.
  Serial.print("req_reconf ");
  
  // Wait for an Ack. In this case, an "Ok ".
  if (Serial.available() > 0){
    message = Serial.readStringUntil(' ');
  }
  
  // If the Ack was received, do something!
  if(message != ""){
    // In this case, the master only prints the message.
    Serial.println(message);
  }
  else{
    Serial.println();
  }
  
  // Wait a little bit to send a new message.
  waitFor(3000);
}



/* ###### Other Functions ###### */

/* 
 * This funcion initialize the things that are necessary to the Mas-
 * ter.
 * No parameters.
 */
void initializeMaster(){
  // Initialize the serial port at 9600 baud.
  Serial.begin(9600);
  
  // Set pins to transmit/receive data.
  slavesCounter = 0;
}

/* 
 * This function identifies how many slaves the master can communi-
 * cate.
 * No parameters.
 * Return: the number of slaves with success communication.
 */
int identifySlaves(){
  String incomingMessage;
  int numberOfSlaves = 0;
  long time = 2000;
  unsigned long ini_time = millis();
  unsigned long end_time = ini_time;
  
  // Send a message to the Slaves.
  Serial.print("req_reconf ");
  
  // Wait for Slaves' Acks.
  while(end_time - ini_time <= time){
    if (Serial.available() > 0){
      incomingMessage = Serial.readStringUntil(' ');
      if(incomingMessage == "Ok"){
        numberOfSlaves++;
        cleanString(&incomingMessage);
      }
    }
    end_time = millis();
  }
  return numberOfSlaves;  
}

/* 
 * This funcion process the received data (Fusion/Aggregation).
 * No parameters.
 * 
 */
void processReceivedData(){
  // Do something with the data: Fusion/Aggregation
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
 * This funcion clean the content of a string.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
void cleanString(String *string){
  *string = "";
}
