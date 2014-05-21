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
   
    Date of creation: 21/May/2014

    This source code is for specific purpose and the authors do not
    guarantee it to work in other cases. As an open source code, its
    use is free, but please keep the main authors list in the header.
      
*/


/* ##### Functions Prototypes ##### */
void setup();
void loop();
void initializeSlave();
void processMessage(String *newMessage);



/* ######## Definitions ######## */
#define SIZE 15



/* ##### Global Variables ###### */
int byteIn;
String message;


/* ##### Arduino Functions ##### */
void setup(){
  initializeSlave();
}

void loop(){
  
  // Verify if there is 'something' in Serial.
  if (Serial.available() > 0){
    message = Serial.readStringUntil('\n');
  }
  
  // If a message arrive, then process it.
  if(message != ""){
    processMessage(&message);
  }
}



/* ###### Other Functions ###### */

/* 
 * This funcion initialize the things that are necessary to the Slave.
 * No parameters.
 */
void initializeSlave(){
  // Initialize the serial port at 9600 baud.
  Serial.begin(9600);
  
  // Set pins to transmit/receive data.
}

/* 
 * This funcion process the message received by the Slave.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
void processMessage(String *newMessage){
  // Simple example to what to do with the Received String.
  Serial.print("Received: ");
  Serial.println(*newMessage);
  *newMessage = "";
}
