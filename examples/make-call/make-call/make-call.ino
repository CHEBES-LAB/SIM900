/*************************************************************************************
 *  Created By: Edgar Nyandoro
 *  Created On: 19 June, 2024
 ***********************************************************************************/
 
#include <SIM900.h>

SIM900 sim900(7, 8); //Rx, Tx

//Enter the phone number of the person whom you want to make a call
String phoneNumber= "+ZZxxxxxxxxxx";

void handleSMS(String number, String message) {
  Serial.println("number: " + number + "\nMessage: " + message);
}

void handleCall(String number) {
  Serial.println("New call from " + number);
}

void setup() {
  Serial.begin(9600);
  
 sim900.begin(9600);
  
 sim900.setSMSCallback(handleSMS);
 sim900.setCallCallback(handleCall);
  
 sim900.makeCall(phoneNumber);
}

void loop() {
 sim900.listen();
}