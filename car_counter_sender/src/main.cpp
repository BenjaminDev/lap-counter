// /*
//  * SimpleSender.cpp
//  *
//  *  Demonstrates sending IR codes in standard format with address and command
//  *  An extended example for sending can be found as SendDemo.
//  *
//  *  Copyright (C) 2020-2022  Armin Joachimsmeyer
//  *  armin.joachimsmeyer@gmail.com
//  *
//  *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
//  *
//  *  MIT License
//  */
// #include <Arduino.h>

// #define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
// //#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
// //#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

// /*
//  * This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
//  */
// // #include "PinDefinitionsAndMore.h"
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #define IR_SEND_PIN 2
// void setup() {
//     pinMode(LED_BUILTIN, OUTPUT);

//     Serial.begin(9600);

//     // Just to know which program is running on my Arduino
//     Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
//     Serial.print(F("Send IR signals at pin "));
//     Serial.println(IR_SEND_PIN);

//     /*
//      * The IR library setup. That's all!
//      */
// //    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin
//     IrSender.begin(DISABLE_LED_FEEDBACK); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
// }

// /*
//  * Set up the data to be sent.
//  * For most protocols, the data is build up with a constant 8 (or 16 byte) address
//  * and a variable 8 bit command.
//  * There are exceptions like Sony and Denon, which have 5 bit address.
//  */
// uint8_t sCommand = 0x34;
// uint8_t sRepeats = 0;

// void loop() {
//     /*
//      * Print current send values
//      */
//     Serial.println();
//     Serial.println("Send now: address=0x00, command=0x");
//     Serial.print(sCommand, HEX);
//     Serial.print(F(", repeats="));
//     Serial.print(sRepeats);
//     Serial.println();

//     Serial.println(F("Send standard NEC with 8 bit address"));
//     Serial.flush();

//     // Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
//     IrSender.sendNEC(0x00, sCommand, sRepeats);

//     /*
//      * Increment send values
//      */
//     sCommand += 0x11;
//     sRepeats++;
//     // clip repeats at 4
//     if (sRepeats > 4) {
//         sRepeats = 4;
//     }

//     delay(1000);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
// }

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed); // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
// uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
//                         600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
//                         600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
//                         650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
//                         650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
//                         650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
//                         650, 1650, 650, 1650, 650, 1650, 600};
// // Example Samsung A/C state captured from IRrecvDumpV2.ino
// uint8_t samsungState[kSamsungAcStateLength] = {
//     0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
//     0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

void setup()
{
  irsend.begin();
  Serial.begin(9600);
}

void loop()
{
  //   Serial.println("NEC");
  irsend.sendNEC(0xFFA25DUL);
  delay(1);
}