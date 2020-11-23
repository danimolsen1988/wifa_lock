/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Keld/Documents/engineer/IoT/tlsClient/src/tlsClient.ino"
/*
 * Project tlsClient
 * Description:
 * Author:
 * Date:
 */

#include "TlsClientHandler.h"

void setup();
void loop();
#line 10 "c:/Users/Keld/Documents/engineer/IoT/tlsClient/src/tlsClient.ino"
TlsClientHandler client;

void setup() {
    Serial.begin(9600);
    Serial.print(Time.timeStr());
    client.setup();
}

void loop() {
    // connect HTTPS server.
    client.Detect();
    delay(30000);

}