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



/* ##### Arduino Functions ##### */
void setup(){
  initializeSlave();
}

void loop(){
  
}



/* ##### Other Functions ##### */
void initializeSlave(){
  // Initialize the serial port at 9600 baud.
  Serial.begin(9600);
  
  // Set pins to transmit/receive data.
}
