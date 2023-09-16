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
// #include <IRLibSendBase.h>  
// #define SEND_RCMM
// #include <IRLib_P01_NEC.h>    
// #include <IRLib_P11_RCMM.h>   
// #include<IRLib_P02_Sony.h>
// #include <IRLibCombo.h> 

const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed); // Set the GPIO to be used to sending the message.
// IRsend irsend;
// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[10] = {600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650};
// , 00, 650, 550, 650, 1650, 600, 550, 650, 550,
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
// Constants
const uint16_t kNecTick = 560;
const uint16_t kNecHdrMarkTicks = 16;
const uint16_t kNecHdrMark = kNecHdrMarkTicks * kNecTick;
const uint16_t kNecHdrSpaceTicks = 8;
const uint16_t kNecHdrSpace = kNecHdrSpaceTicks * kNecTick;
const uint16_t kNecBitMarkTicks = 1;
const uint16_t kNecBitMark = kNecBitMarkTicks * kNecTick;
const uint16_t kNecOneSpaceTicks = 3;
const uint16_t kNecOneSpace = kNecOneSpaceTicks * kNecTick;
const uint16_t kNecZeroSpaceTicks = 1;
const uint16_t kNecZeroSpace = kNecZeroSpaceTicks * kNecTick;
const uint16_t kNecRptSpaceTicks = 4;
const uint16_t kNecRptSpace = kNecRptSpaceTicks * kNecTick;
const uint16_t kNecRptLength = 4;
const uint16_t kNecMinCommandLengthTicks = 193;
const uint32_t kNecMinCommandLength = kNecMinCommandLengthTicks * kNecTick;
const uint32_t kNecMinGap = 20000U;
    // kNecMinCommandLength -
    // (kNecHdrMark + kNecHdrSpace + kNECBits * (kNecBitMark + kNecOneSpace) +
    //  kNecBitMark);
    //  
const uint16_t kNecMinGapTicks =
    kNecMinCommandLengthTicks -
    (kNecHdrMarkTicks + kNecHdrSpaceTicks +
     kNECBits * (kNecBitMarkTicks + kNecOneSpaceTicks) + kNecBitMarkTicks);
void setup()
{
  irsend.begin();
  // irsend.enableIROut(36,25);
  Serial.begin(9600);
}

void loop()
{

  // irsend.send(RCMM, 0xFFA25DUL,24);
  // irsend.sendNEC(0xFFA25DUL);
  irsend.sendBose(0xF0F0);
  // IRsend irsend(kIrLed);
//   const uint16_t nbits = 16;
//   const unsigned long data = 0xFFA25DUL;
//   /// @param[in]
// /// @param[in] headerspace Nr. of usecs for the led to be off after the header
// ///   mark. A value of 0 means no header space.
// /// @param[in] 
// /// @param[in] onespace Nr. of usecs for the led to be fully off for a '1' bit.
// /// @param[in] 
// /// @param[in] zerospace Nr. of usecs for the led to be fully off for a '0' bit.
// /// @param[in] footermark Nr. of usecs for the led to be pulsed for the footer
// ///   mark. A value of 0 means no footer mark.
// /// @param[in] gap Nr. of usecs for the led to be off after the footer mark.
// ///   This is effectively the gap between messages.
// ///   A value of 0 means no gap space.
// /// @param[in] mesgtime Min. nr. of usecs a single message needs to be.
// ///   This is effectively the min. total length of a single message.
// /// @param[in] data The data to be transmitted.
// /// @param[in] nbits Nr. of bits of data to be sent.
// /// @param[in] frequency The frequency we want to modulate at. (Hz/kHz)
// /// @param[in] MSBfirst Flag for bit transmission order.
// ///   Defaults to MSB->LSB order.
// /// @param[in] repeat Nr. of extra times the message will be sent.
// ///   e.g. 0 = 1 message sent, 1 = 1 initial + 1 repeat = 2 messages
// /// @param[in] dutycycle Percentage duty cycle of the LED.
// ///   e.g. 25 = 25% = 1/4 on, 3/4 off.
// ///   If you are not sure, try 50 percent.
// /// @note Assumes a frequency < 1000 means kHz otherwise it is in Hz.
// ///   Most common value is 38000 or 38, for 38kHz.
//   irsend.sendGeneric(
//     kNecHdrMark, //  headermark Nr. of usecs for the led to be pulsed for the header mark. A value of 0 means no header mark.
//     kNecHdrSpace, // headerspace Nr. of usecs for the led to be off after the header mark. A value of 0 means no header space.
//     kNecBitMark, // onemark Nr. of usecs for the led to be pulsed for a '1' bit.
//     kNecOneSpace, // onespace Nr. of usecs for the led to be fully off for a '1' bit.
//     kNecBitMark, // zeromark Nr. of usecs for the led to be pulsed for a '0' bit.
//     kNecZeroSpace, //  zerospace Nr. of usecs for the led to be fully off for a '0' bit.
//     kNecBitMark, // footermark Nr. of usecs for the led to be pulsed for the footer mark. A value of 0 means no footer mark.
//     kNecMinGap, // gap Nr. of usecs for the led to be off after the footer mark. This is effectively the gap between messages. A value of 0 means no gap space.
//     kNecMinCommandLength, // mesgtime Min. nr. of usecs a single message needs to be. This is effectively the min. total length of a single message.
//     data, 
//     nbits, // nbits Nr. of bits of data to be sent.
//     38,  // frequency The frequency we want to modulate at. (Hz/kHz)
//     true, // MSBfirst Flag for bit transmission order. Defaults to MSB->LSB order.
//     0,  // Repeats are handled later.
//     33 // dutycycle Percentage duty cycle of the LED. e.g. 25 = 25% = 1/4 on, 3/4 off.  If you are not sure, try 50 percent.
//   ); 
//   // irsend.begin();
//   Serial.println("RCMM");
  // irsend.send
  // irsend.sendRaw(rawData,1,36);
  // delay(10);
  // irsend.
  // 11111111 10100010 01011101
  // 00101001 10100100 00010000 10010100

  // 11110111 10010011 00101111 10100000
  // 11111111 10100010 01011101
  //                   10010110 00
  // 0b11111111 10100010 01011101
}