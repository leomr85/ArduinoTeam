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


/* ######### Includes ########## */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>



/* #### Functions Prototypes ### */
void setup();
void loop();
void initializeSlave();
void waitFor(long time);
void cleanString(String *string);
void getTemperature(float *temperature);
void getBattVoltage(long *battVoltage);
int processMessage(String *newMessage);



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
    if(processMessage(&message) != 0){
      Serial.println("Error! Message not recognized.");
    }
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
int processMessage(String *newMessage){
  
  // Verify if the message is a Request to Sleep.
  if(*newMessage == "req_sleepm"){
    // Notify the Master the reception of a sleep message.
    Serial.print("Received: ");
    Serial.println(*newMessage);
    Serial.println("  zzZ ... zzZ ... zzZ ");
    Serial.flush();
    cleanString(newMessage);    
    
    // The slave should sleep here.
    // Select the watchdog timer mode.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // Reset status flag.
    MCUSR &= ~(1 << WDRF);
    // Enable configuration changes.
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // Set the prescalar = 9 = 0b1001 (~10s).
    WDTCSR = (1 << WDP0) | (0 << WDP1) | (0 << WDP2) | (1 << WDP3);
    // Enable interrupt mode.
    WDTCSR |= (1 << WDIE);
    
    sleep_enable();
    sleep_mode();    
    sleep_disable();

    return 0;
  }
  
  // Verify if the message is a Request Status.
  if(*newMessage == "req_status"){
    Serial.print("Received: ");
    Serial.println(*newMessage);
    cleanString(newMessage);
    
    // The slave needs to get the sensor values here.
    float sensor_t; // Temperature.
    long sensor_b; // Batery Voltage.
    getTemperature(&sensor_t);
    getBattVoltage(&sensor_b);
    Serial.print("  Temperature (*C): ");
    Serial.println(sensor_t);
    Serial.print("  Battery Voltage (mV): ");
    Serial.println(sensor_b, DEC);
    
    // The slave should send its status to the Master here.
    Serial.print("  Sending data to the Master...");
    // Send code here...
    Serial.println(" OK!");
    
    return 0;
  }
  
  // Verify if the message is a Request to configure.
  if(*newMessage == "req_reconf"){
    Serial.print("Received: ");
    Serial.println(*newMessage);
    cleanString(newMessage);
    
    // The slave should reconfigure here.
    Serial.print("  Reconfiguring...");
    // Reconfiguring code here...
    Serial.println(" Done!");

    // Notify the Master about the reception of a reconfigure message.
    Serial.print("  Sending ack to the Master...");
    // Send code here...
    Serial.println(" OK!");
    
    return 0;
  }
  
  // If the message do not fit in earlier cases, indicates an error.
  cleanString(newMessage);
  return 1;
}

/* 
 * This funcion clean the content of a string.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
void cleanString(String *string){
  *string = "";
}

/* 
 * This funcion gets the temperature sensor state.
 * Parameters:
 *  float *temperature = pointer for the temperature variable.
 */
void getTemperature(float *temperature){
  // Read the temperature sensor value.
  *temperature = 25.0; // Test.
}

/* 
 * This funcion gets the Battery Voltage sensor state.
 * Parameters:
 *  float *battVoltage = pointer for the battery Voltage variable.
 */
void getBattVoltage(long *battVoltage){
  // Read the battery voltage sensor value.
  
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  // Wait for Vref to settle.
  waitFor(2);
  // Convert
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  *battVoltage = ADCL;
  *battVoltage |= ADCH<<8;
  // Back-calculate AVcc in mV.
  *battVoltage = 1126400L / *battVoltage;
}

/* 
 * This funcion waits for 'time' miliseconds (it does not do anything in practice, just wait).
 * Parameters:
 *  long time = the time (in ms) that the function should wait.
 */
void waitFor(long time){
  // millis function returns the number of milliseconds since the program started. 
  
  // Create two variables: initial and end time.
  unsigned long ini_time = millis();
  unsigned long end_time = ini_time;
  
  while(end_time - ini_time <= time){
    end_time = millis();
  }
}

/* 
 * This is a Interrupt Service Routine. This 'function' handles the
 * watchdog timer.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
ISR(WDT_vect){
  // Show that we handle the interruption from watchdog.
  Serial.println("  Waked up again!");
  // Disable the watchdog timer.
  wdt_disable();
}
