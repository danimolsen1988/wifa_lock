/*
 * Project mainModule
 * Description:
 * Author:
 * Date:
 */

#include "TlsClientHandler.h"
#include <vector>
#include "debug.h"
#include "HardwareInterface.h"

SYSTEM_MODE(MANUAL);

TlsClientHandler client;
TCPServer server = TCPServer(2555);
TCPClient tcpClient;
HardwareInterface hw;
uint8_t * img;
uint32_t totalSize = 0;
uint32_t buffersize = 0;
std::vector<uint8_t> imgVector;
SystemSleepConfiguration sleepcfg;
String clientIP;

bool camLoop();

void setup() {
    WiFi.on();
    WiFi.connect();
    while(WiFi.connecting());
    delay(2000);    // was here--------------------------------
    sleepcfg.mode(SystemSleepMode::ULTRA_LOW_POWER)
    .duration(600000UL)
    .network(NETWORK_INTERFACE_WIFI_STA, SystemSleepNetworkFlag::NONE);
    
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

void loop()
{
  // connect HTTPS server.
  //Serial.println(WiFi.localIP().toString());
  if (camLoop())
  {
    bool facialRecog = client.Detect(&imgVector[0], imgVector.size());    
    //SEND SOME CODE BACK.....
    if(!tcpClient.connect(clientIP, 2555))
    {
      //ERROR
      //Serial.println("CAN NOT CONNECT OT CAM AGAIN");
    }
    else
    {
      tcpClient.write(facialRecog ? 'a' : 'b');
    }
    if (facialRecog)
    {
      hw.openLock(true);
      hw.rgbControl(2);
      delay(5000);
      hw.rgbControl(0);
      hw.openLock(false);
      delay(2000);
    }
    else
    {
      hw.faceNotRecogged();
      delay(2000);
    }
  }
#if DEBUG_AZURE == 0
  System.sleep(sleepcfg);
#endif
}

bool camLoop() {
    buffersize = 0; 
    totalSize = 0;
    std::vector<uint8_t>().swap(imgVector);
    tcpClient = server.available();
    if(tcpClient) {
      clientIP = tcpClient.remoteIP().toString();
#if DEBUG_AZURE == 1
       Serial.println("client connected!");
       Serial.printf("client ip:%s\n",clientIP.c_str());
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