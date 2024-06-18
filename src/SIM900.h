#include <SoftwareSerial.h>

class SIM900 {
  //------------------------------------------------------------------------------------
  private:
    const SoftwareSerial simSerial;
    void (*callCallback)(const String&);
    void (*smsCallback)(const String&, const String&);
    String phoneNumber;
    String messageText;
    int resetPin;
    bool debug = false;

  public:
    SIM900(int rxPin, int txPin, int resetPin = -1) : simSerial(rxPin, txPin), resetPin(resetPin) {}

    //------------------------------------------------------------------------------------
    void begin(unsigned long baudrate = 9600) {
      simSerial.begin(baudrate);
      pinMode(resetPin, OUTPUT);
      reset();
      if (isModuleReady()) {
        tryATcommand("ATE1"); // Echo ON
        tryATcommand("AT+CMGF=1;"); // Set SMS mode to text
        tryATcommand("AT+CNMI=1,2,0,0,0"); // Set new message indication mode
        tryATcommand("AT+CLIP=1"); // Enable caller ID
        tryATcommand("AT+DDET=1"); // Enable DTMF detection
        Serial.println("Module is ready.");
      } else {
        Serial.println("Failed to initialize the module after multiple attempts.");
      }
    }

    //------------------------------------------------------------------------------------
    void listen() {
      int availableBytes = simSerial.available();
      if (availableBytes <= 0) return;
      String incomingResponse = simSerial.readString();
      if (debug) {
        Serial.println(incomingResponse);
      }
      if (incomingResponse.indexOf("+CMT:") != -1) {
        // We have received a new message
        int idx1 = incomingResponse.indexOf("+CMT: \"") + 7;
        int idx2 = incomingResponse.indexOf("\"", idx1);
        phoneNumber = incomingResponse.substring(idx1, idx2);
        
        idx1 = incomingResponse.lastIndexOf("\"") + 1;
        messageText = incomingResponse.substring(idx1);
        messageText.trim();
        if (smsCallback) {
          smsCallback(phoneNumber, messageText);
        }
      } else if (incomingResponse.indexOf("RING") != -1) {
        // We have received a new call
        int idx1 = incomingResponse.indexOf("\"") + 1;
        int idx2 = incomingResponse.indexOf("\"", idx1);
        phoneNumber = incomingResponse.substring(idx1, idx2);
        if (callCallback) {
          callCallback(phoneNumber);
        }
      }
    }

    //------------------------------------------------------------------------------------
    bool isModuleReady() const {
      int tries = 0;
      while (tries < 20) {
        if (tryATcommand("AT")) {
          return true;
        }
        Serial.print(".");
        tries++;
      }
      return false;
    }

    //------------------------------------------------------------------------------------
    bool getResponse(const String& expectedAnswer, unsigned int timeout = 1000) const {
      unsigned long startTime = millis();
      while (millis() - startTime < timeout) {
        while(simSerial.available()){
          String response = simSerial.readString();
          if (response.indexOf(expectedAnswer) != -1) {
            Serial.println(response);
            return true;
          }
        }
      }
      return false;
    }

    //------------------------------------------------------------------------------------
    bool tryATcommand(const String& cmd, const String& expectedAnswer = "OK", unsigned int timeout=1000) const {
      simSerial.println(cmd);
      return getResponse(expectedAnswer, timeout);
    }

    //------------------------------------------------------------------------------------
    String getPhoneNumber() {
      return phoneNumber;
    }

    //------------------------------------------------------------------------------------
    String getMessageText() {
      return messageText;
    }

    //------------------------------------------------------------------------------------
    void sendSMS(const String& phoneNumber, const String& messageText) {
      if (tryATcommand("AT+CMGF=1;")) {
        if (tryATcommand("AT+CMGS=\"" + phoneNumber + "\"\r", ">")) {
          simSerial.print(messageText);
          simSerial.write(0x1A); // Send CTRL+Z character to end message
          if (getResponse("+CMGS:", 10000)) {
            Serial.println("Message sent successfully");
            return;
          }
        }
      }
      Serial.println("Error: Failed to send message");
    }

    //------------------------------------------------------------------------------------
    void makeCall(const String& phoneNumber) {
      simSerial.print("ATD");
      simSerial.print(phoneNumber);
      simSerial.println(";");
    }

    //------------------------------------------------------------------------------------
    void setDebug(bool flag) {
      debug = flag;
    }

    //------------------------------------------------------------------------------------
    void setCallCallback(void (*callback)(String)) {
      callCallback = callback;
    }

    //------------------------------------------------------------------------------------
    void setSMSCallback(void (*callback)(String, String)) {
      smsCallback = callback;
    }

    //------------------------------------------------------------------------------------
    void reset() {
      if (resetPin != 0) {
        digitalWrite(resetPin, LOW);
        delay(100);
        digitalWrite(resetPin, HIGH);
      }
    }
};
