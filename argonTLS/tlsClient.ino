/*
 * Project tlsClient
 * Description:
 * Author:
 * Date:
 */

#include "TlsClientHandler.h"
#include <vector>

TlsClientHandler client;
TCPServer server = TCPServer(2555);
TCPClient tcpClient;
uint8_t * img;
uint32_t totalSize = 0;
uint32_t buffersize = 0;
std::vector<uint8_t> imgVector;

bool camLoop();

void setup() {
    delay(2000);
    Serial.begin(9600);
    Serial.println(Time.timeStr());
    Serial.println(WiFi.localIP().toString());
    client.setup();
    server.begin();
}

void loop() {
    // connect HTTPS server.

    if(camLoop()){
        Serial.println(imgVector.size());   // DEBUG
        client.Detect(&imgVector[0],imgVector.size());
    }


}

bool camLoop() {
    buffersize = 0;
    totalSize = 0;
    std::vector<uint8_t>().swap(imgVector);
    tcpClient = server.available();
    if(tcpClient) {
       Serial.println("client connected!");
       while(tcpClient.connected()) {
           if((buffersize = tcpClient.available())) {
                img = new uint8_t[buffersize];
                tcpClient.readBytes((char*)img,buffersize);
                imgVector.insert(imgVector.end(), img, img + buffersize);
                totalSize += buffersize;
           }
       }
        free(img);
        return true;
   }
   return false;
}
