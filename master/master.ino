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
    use is free, but please keep the main authors list in the header.
      
*/


/* ######### Includes ########## */



/* #### Functions Prototypes ### */
void setup();
void loop();
void initializeMaster();
void processReceivedData();
void waitFor(long time);



/* ######## Definitions ######## */



/* ##### Global Variables ###### */
String message;



/* ##### Arduino Functions ##### */
void setup(){
  initializeMaster();
}

void loop(){
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
 * This funcion initialize the things that are necessary to the Master.
 * No parameters.
 */
void initializeMaster(){
  // Initialize the serial port at 9600 baud.
  Serial.begin(9600);
  
  // Set pins to transmit/receive data.
}

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
