/*************************************************************************************
 *  Created By: Edgar Nyandoro
 *  Created On: 19 June, 2024
 ***********************************************************************************/

#include <SIM900.h>

SIM900 sim900(7, 8); //Rx, Tx

//Enter the phone number of the person whom you want to send sms
String phoneNumber = "+ZZxxxxxxxxxx";
String message = "Hello from SIM900";

void setup() {
  Serial.begin(9600);
  
  sim900.begin(9600);
  
  sim900.sendSMS(phoneNumber, message);
}

void loop() {}