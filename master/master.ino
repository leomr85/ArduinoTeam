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

/* ##### Functions Prototypes ##### */
void setup();
void loop();
void initializeMaster();



/* ##### Arduino Functions ##### */
void setup(){
  initializeMaster();
}

void loop(){
  
}



/* ##### Other Functions ##### */
void initializeMaster(){
  // Initialize the serial port at 9600 baud.
  Serial.begin(9600);
  
  // Set pins to transmit/receive data.
}
