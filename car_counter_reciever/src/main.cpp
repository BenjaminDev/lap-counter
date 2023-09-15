/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino (or later) example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#define DEBUG
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

bool IRrecv::decodeBEN(decode_results *results, uint16_t offset,
                       const uint16_t nbits, const bool strict) {

    // 600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650}
    bool foound;
    // Serial.println("Start of message");
    uint16_t num_bits = 16;
    const int message[] = {4427,  2285,  241,  922, 202, 323, 241, 898, 227, 324, 240, 324, 255, 311, 253, 870};
                          //  4467,  2256,  256,  871, 268  295  269  856  270  295  268  296  268  297  268 857
                          //  4427  2285  241  922 202 323 241 898 227 324 240 324 255 311 253 870
    uint16_t matches  = 0;
    for (uint16_t i = 0; i< num_bits; i++){
      Serial.print(results->rawbuf[offset+i]);
      Serial.print(" ");
      // matchSpace
      // Serial.println(results->rawbuf[offset+i]);
      // Serial.println(message[i]);
      if (abs(results->rawbuf[offset+i] - message[i])< 50 ) {
        matches++;
      }
    }
    if (matches > 14){
      return true;
    }
    Serial.println("");
    Serial.println(matches);
    Serial.println("");
    // Serial.println(nbits);
    // Serial.println(foound);
    return false;
}

#ifndef STASSID
#define STASSID ""
#define STAPSK ""
// #define STASSID "P"
// #define STAPSK "P"

#endif
#define DEBUG_WIFI_GENERIC
const uint16_t kRecvPin = 14; // Pin 14 is D5
const char* host = "192.168.1.34"; // Matt's house
// const char *host = "192.168.1.12"; // Benj's house
const uint16_t port = 65000;
const int32_t delay_between_triggers = 3000; // 10 Second delay between triggers.
const char *car_name = "Sprout";
// const bool DEBUG = false;

// Use turn on the save buffer feature for more complete capture coverage.
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 15;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
// decode_results results;  // Somewhere to store the results


decode_results results;

void setup()
{
  
  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  Serial.print("\nConnecting to ");
  Serial.println(STASSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.print("connected, address=");
  Serial.println(WiFi.localIP());

  irrecv.enableIRIn(); // Start the receiver
  // irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  // irrecv.enableIRIn();  // Start the receiver
  while (!Serial)      // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("JF is now running and waiting for IR messages. ");
  Serial.println(kRecvPin);
}

void loop()
{
  // 600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650}
  
  if (irrecv.decode(&results))
  {
    // Serial.println("Found");
    // Serial.println(results.value);
    // Serial.println(results.decode_type);
    // Serial.println(results.overflow);
    // Serial.println(results.bits);
    // if (results.value == 16753245UL){
      Serial.println("We got one");
      Serial.println(results.decode_type);
      WiFiClient client;
      for (int i = 0; i < 5; i++){
        if(client.connect(host, port)){
          break;
        }
      }
      // Logging a lap
      client.println(car_name);
      delay(delay_between_triggers);
      Serial.println("ready");
    // }
    // serialPrintUint64(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
// 101001 10100100 00010000 10010100