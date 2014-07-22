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
#include <XBeeLib.h>



/* #### Functions Prototypes ### */



/* ######## Definitions ######## */



/* ##### Global Variables ###### */
XBeeLib xb(0); // 0 = End Device.



/* ##### Arduino Functions ##### */
void setup(){
  Serial.begin(9600);
}

void loop(){
  
  // Wait for requisitions.
  xb.rxPacket();
  
}
