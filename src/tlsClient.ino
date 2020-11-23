/*
 * Project tlsClient
 * Description:
 * Author:
 * Date:
 */

#include "TlsClientHandler.h"

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