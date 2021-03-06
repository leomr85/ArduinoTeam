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
    use is free. Please keep the main authors list in the header.
    
    WD_OR_INT Usage:
      '1': Watchdog timer (no hardware setup required).
      '2': Interruption timer (Connect a 220 Ohm resistor between di-
           gital pin 0 (RX) and the digital pin 2 (INT0)).

    XB_OR_MON Usage:
      '1': XBee communication.
      '2': Serial Monitor communication.
*/


/* ######### Includes ########## */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>



/* #### Functions Prototypes ### */
void setup();
void loop();

void initializeSlave();
void initializeWatchdog();
void initializeInterrupt();

int processMessage(String *newMessage);
void sleepMode();
void getTemperature(float *temperature);
void getBattVoltage(long *battVoltage);
void cleanString(String *string);
void monitorPrint(boolean newLine, String string);
void xBeeModPrint(boolean newLine, String string);
void waitFor(long time);
ISR(WDT_vect);
void pinInterrupt();



/* ######## Definitions ######## */
#define WD_OR_INT 1
#define XB_OR_MON 2



/* ##### Global Variables ###### */
int pin2;
String message;



/* ##### Arduino Functions ##### */
void setup(){
  initializeSlave();
  
  // Verify the value of WD_OR_INT.
  if(WD_OR_INT == 1){
    if(XB_OR_MON == 2){
      monitorPrint(false, "Watchdog-Guided Program Initializing... ");
    }
  }
  if(WD_OR_INT == 2){
    monitorPrint(false, "Interruption-Guided Program Initializing... ");
  }
  
  monitorPrint(true, "Complete!");
}

void loop(){
  
  // Verify if there is 'something' in Serial.
  if (Serial.available() > 0){
    message = Serial.readStringUntil(' ');
  }
  
  // If a message arrive, then process it.
  if(message != ""){
    if(processMessage(&message) != 0){
      monitorPrint(true, "Error! Message not recognized.");
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
  // Initialize variables.
  pin2 = 2; // Digital pin #2.
}

/* 
 * This function enables Watchdog (if it is necessary).
 * No parameters.
 *
 */
void initializeWatchdog(){
  // Select the watchdog timer mode.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // Reset status flag.
  MCUSR &= ~(1 << WDRF);
  // Enable configuration changes.
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Set the prescalar = 9 = 0b1001 (~8s).
  WDTCSR = (1 << WDP0) | (0 << WDP1) | (0 << WDP2) | (1 << WDP3);
  // Enable interrupt mode.
  WDTCSR |= (1 << WDIE);
}

/* 
 * This function enables Interruption at Pin 2 (if it is necessary).
 * No parameters.
 *
 */
void initializeInterrupt(){
  // Set Pin 2 as input.
  pinMode(pin2,INPUT);

  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pinInterrupt, LOW);
  waitFor(5);
  
  // Select the watchdog timer mode.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
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
    // Serial Monitor prints.
    monitorPrint(false, "Received: ");
    monitorPrint(true, *newMessage);
    monitorPrint(true, "  zzZ ... zzZ ... zzZ ");
    
    // XBee Module prints.
    xBeeModPrint(true, "Ok ");
    
    Serial.flush();
    cleanString(newMessage);    
    
    // The slave should sleep here.
    sleepMode();

    return 0;
  }
  
  // Verify if the message is a Request Status.
  if(*newMessage == "req_status"){
    // Serial Monitor prints.
    monitorPrint(false, "Received: ");
    monitorPrint(true, *newMessage);
    Serial.flush();
    cleanString(newMessage);
    
    // The slave needs to get the sensor values here.
    float sensor_t; // Temperature.
    long sensor_b; // Batery Voltage.
    char temporary[10];
    getTemperature(&sensor_t);
    getBattVoltage(&sensor_b);
    
    // The slave should send its status to the Master here.
    // Serial Monitor and XBee Module prints.
    monitorPrint(false, "  Temperature (*C): ");
    dtostrf(sensor_t,6,2,temporary);
    monitorPrint(true, temporary);
    xBeeModPrint(false, temporary);
    xBeeModPrint(false, ":");
    
    monitorPrint(false, "  Battery Voltage (mV): ");
    dtostrf(sensor_b,7,1,temporary);
    monitorPrint(true, temporary);
    xBeeModPrint(true, temporary);
    
    monitorPrint(false, "  Sending data to the Master...");
    monitorPrint(true, " OK!");
    
    return 0;
  }
  
  // Verify if the message is a Request to configure.
  if(*newMessage == "req_reconf"){
    // Serial Monitor prints.
    monitorPrint(false, "Received: ");
    monitorPrint(true, *newMessage);
    Serial.flush();
    cleanString(newMessage);
    
    // The slave should reconfigure here.
    monitorPrint(false, "  Reconfiguring...");
    // Reconfiguring code here...
    monitorPrint(true, " Done!");

    // Notify the Master about the reception of a reconfigure message.
    monitorPrint(false, "  Sending ack to the Master...");
    xBeeModPrint(true, "Ok ");
    monitorPrint(true, " OK!");
    
    return 0;
  }
  
  // If the message do not fit in earlier cases, indicates an error.
  cleanString(newMessage);
  return 1;
}

/* 
 * This is a Sleep Routine. It sets the Watchdog and enable the node
 * to sleep.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
void sleepMode(){
  if(WD_OR_INT == 1){
    initializeWatchdog();
  }
  if(WD_OR_INT == 2){
    initializeInterrupt();
    /* To wake up again the node, just send a character (e.g. "w")
     * and then send a command.
     * Ex: $ req_sleepm > this command puts the node to sleep.
     *     $ w          > this command wakes up the node.
     *     $ req_status > this command retrieves the node status.
    */
  }
  
  sleep_enable();
  sleep_mode();
  sleep_disable();
  
  // Show that we handle the interruption (by the Watchdog or Rx).
  monitorPrint(true, "  Waked up again!");
  // Send a message to the Master: "I am ready to receive commands".
  if(WD_OR_INT == 2)  xBeeModPrint(true, "ready ");
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
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  // Wait for Vref to settle.
  waitFor(5);
  // Convert
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  *battVoltage = ADCL;
  *battVoltage |= ADCH<<8;
  // Back-calculate AVcc in mV.
  *battVoltage = 1126400L / *battVoltage;
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
 * This funcion prints a string into serial Monitor.
 * Parameters:
 *  boolean newLine = indicate if a new line is necessary.
 *  String string = the string to be printed into monitor.
 */
void monitorPrint(boolean newLine, String string){
  if(XB_OR_MON == 2){
    if(newLine == false){
      Serial.print(string);
    }
    else{
      Serial.println(string);
    }
  }
}

/* 
 * This funcion prints a string into XBee Module.
 * Parameters:
 *  boolean newLine = indicate if a new line is necessary.
 *  String string = the string to be printed into monitor.
 */
void xBeeModPrint(boolean newLine, String string){
  if(XB_OR_MON == 1){
    if(newLine == false){
      Serial.print(string);
    }
    else{
      Serial.println(string);
    }
  }
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
 * This is a Interrupt Service Routine. This 'function' handles the
 * watchdog timer.
 * Parameters:
 *  String *newMessage = pointer for the received message.
 */
ISR(WDT_vect){
  // Disable the watchdog timer.
  wdt_disable();
}

/* 
 * This is a Interrupt Service Routine. This 'function' handles the
 * pin interruption by receiving serial data (RX).
 * No parameters.
 * 
 */
void pinInterrupt(){
  // Show that we handle the interruption from pin 2.
  detachInterrupt(0);
}
