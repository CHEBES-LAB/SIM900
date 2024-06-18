/*************************************************************************************
 *  Created By: Edgar Nyandoro
 *  Created On: 19 June, 2024
 ***********************************************************************************/

#include <SIM900.h>

SIM900 sim900(7, 8); //Rx, Tx

void handleCall(String number) {
  Serial.println("Receiving call from " + number);
  // add other operations to handle receiving call
}

void setup() {
  Serial.begin(9600);

  sim900.begin(9600);

  sim900.setCallCallback(handleCall);
}

void loop() {
  // set SIM900 to always listen to the incoming
  sim900.listen();
}