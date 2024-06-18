/*************************************************************************************
 *  Created By: Edgar Nyandoro
 *  Created On: 19 June, 2024
 ***********************************************************************************/

#include <SIM900.h>

SIM900 sim900(7, 8); //Rx, Tx

void handleSMS(String number, String message) {
  Serial.println("Receive SMS from number: " + number + "\nMessage: " + message);
  // add other operations to handle receiving SMS

}

void setup() {
  Serial.begin(9600);

  sim900.begin(9600);

  sim900.setSMSCallback(handleSMS);
}

void loop() {
  // set SIM900 to always listen to the incoming
  sim900.listen();
}