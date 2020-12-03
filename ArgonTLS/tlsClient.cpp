/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Keld/Documents/engineer/IoT/tlsClient-argon/src/tlsClient.ino"
/*
 * Project tlsClient
 * Description:
 * Author:
 * Date:
 */

#include "TlsClientHandler.h"
#include <vector>
#include "debug.h"
#include "HardwareInterface.h"


void setup();
void loop();
#line 14 "c:/Users/Keld/Documents/engineer/IoT/tlsClient-argon/src/tlsClient.ino"
TlsClientHandler client;
TCPServer server = TCPServer(2555);
TCPClient tcpClient;
HardwareInterface hw;
uint8_t * img;
uint32_t totalSize = 0;
uint32_t buffersize = 0;
std::vector<uint8_t> imgVector;

bool camLoop();

void setup() {
    delay(2000);
#if DEBUG_AZURE == 1
    Serial.begin(115200);
    Serial.println(Time.timeStr());
    Serial.println(WiFi.localIP().toString());
#endif
    hw.setup();
    client.setup();
    server.begin();
    hw.openLock(0);
}

void loop() {
    // connect HTTPS server.
    
    if(camLoop()){
        if(client.Detect(&imgVector[0],imgVector.size())) {
            Serial.println("client detected!");
            hw.openLock(true);
            hw.rgbControl(2);
            delay(5000);
            hw.rgbControl(1);
            hw.openLock(false);
        }  else {
            hw.faceNotRecogged();
        }
    }
    delay(1000);


}

bool camLoop() {
    buffersize = 0; 
    totalSize = 0;
    std::vector<uint8_t>().swap(imgVector);
    tcpClient = server.available();
    if(tcpClient) {
#if DEBUG_AZURE == 1
       Serial.println("client connected!");
#endif
       while(tcpClient.connected()) {
           if((buffersize = tcpClient.available())) {
                img = new uint8_t[buffersize];
                tcpClient.readBytes((char*)img,buffersize);
                imgVector.insert(imgVector.end(), img, img + buffersize);    
                totalSize += buffersize;
                delete(img); 
           }
       }

        return true;
   }
   return false;
}