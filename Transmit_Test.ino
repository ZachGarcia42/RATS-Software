/*
   RadioLib SX127x Transmit Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the libraries
#include <RadioLib.h>
// #include "RS-FEC.h"

// SX1276 has the following connections:
// NSS pin:   1
// DIO0 pin:  20
// RESET pin: 15
// DIO1 pin:  6
SX1276 radio = new Module(1, 20, 15, 6);

//FEC Setup
// String test = "{\"long\":0.03,\"lat\":0.07,\"glong\":0.0,\"glat\":0.0,\"elev\":6534.9}";
// {"long":0.03,"lat":0.07,"glong":0.0,"glat":0.0,"elev":6534.9}
// const int msglen = 100; const uint8_t ECC_LENGTH = 5; //Max message length, and number of guardian bytes, Max corrected bytes ECC_LENGTH/2
// char message_frame[msglen]; //container for shorter messages
// char repaired[msglen];
// char encoded[msglen + ECC_LENGTH];
//66110NBA03
//223298730

// RS::ReedSolomon<msglen, ECC_LENGTH> rs;

void setup() {

  SPI.setRX(16);
  SPI.setTX(19);
  SPI.setCS(1);
  SPI.setSCK(18);

  delay(3000);
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  // SX1276::begin(434.0, 125.0, 9, 7, 0x12, 10, 8, 0);
  int state = radio.begin(915.0, 125.0, 9, 7, 0x12, 20, 8, 0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  

}

void loop() {
  // Serial.println(F("FEC Encoding packet ... "));
  Serial.println(F("Enter message to send:"));
  Serial.print(">");
  while (Serial.available() == 0) {}     //wait for data available

  String teststr = Serial.readString();  //read until timeout

  char msg[teststr.length()];
  teststr.toCharArray(msg, teststr.length());
  // String teststr = "abcdefghijklmnopqrst";

  Serial.print("Serial Input: ");
  Serial.println(teststr);
  // int str_len = teststr.length()+1;
  // char message[str_len];
  // teststr.toCharArray(message, str_len);
  // Serial.print("Char Array: ");
  // Serial.println(message);

  // char message[] = "{\"long\":0.03,\"lat\":0.07,\"glong\":0.0,\"glat\":0.0,\"elev\":6534.9}";
  
  // memset(message_frame, '-', sizeof(message_frame)); //Clear the array
  // for(uint i = 0; i < sizeof(message_frame); i++) {    Serial.print(message_frame[i]);    }    Serial.println("");
  // for(uint i = 0; i <= str_len; i++) {
  //   message_frame[i] = message[i];
  // }
  // for(uint i = 0; i < sizeof(message_frame); i++) {    Serial.print(message_frame[i]);    }    Serial.println("");


  // rs.Encode(message_frame, encoded);
  // Serial.print("Original: "); Serial.println(message_frame);
  // Serial.print("Encoded:  ");        for(uint i = 0; i < sizeof(encoded); i++) {    Serial.print(encoded[i]);    }    Serial.println("");

  // String msg = encoded;
  // msg = msg.substring(0, msglen+ECC_LENGTH);


  // Serial.print("Encoded:  ");        for(uint i = 0; i < sizeof(encoded); i++) {    Serial.print(encoded[i]);    }    Serial.println("");

  
  // msg.toCharArray(encoded, msglen+ECC_LENGTH+2);


  // for(uint i = 20; i < 40; i+=2) {        encoded[i] = 'A';    } //Let's steal some byte from 20 to 25.


  // Serial.print("Transmitting: ");    for(uint i = 0; i < sizeof(message_frame); i++) {    Serial.print(message_frame[i]);    }    Serial.println("");

  // rs.Decode(encoded, repaired);
  // String result;    memcmp(message_frame, repaired, msglen) == 0 ? result="SUCCESS" : result="FAILURE"; //Compare the arrays
  // Serial.print("Result: ");   Serial.println(result);
  // Serial.print("Repaired: "); Serial.println(repaired);


  Serial.print(F("[SX1278] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 256 characters long
  // NOTE: transmit() is a blocking method!
  //       See example SX127x_Transmit_Interrupt for details
  //       on non-blocking transmission method.
  // Serial.println(sizeof(message_frame));
  // int state = radio.transmit(message_frame);
  int state = radio.transmit(msg);

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

    // print measured data rate
    Serial.print(F("[SX1278] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(100);
}
