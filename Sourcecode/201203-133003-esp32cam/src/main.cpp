#include "esp_camera.h"
#include "driver/rtc_io.h"
#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "Camera.h"
#include "Utility.h"
#include "Network.h"


void setup() {
  Serial.begin(115200);
  Serial.println("");
  
  Utility::initGPIO();
Camera cam;
if(cam.initCam()!= ESP_OK){
    //stop
    Serial.println("Cam Init failed");
    Utility::sleep();
  } 

  //CAPTURE IMG
  if(cam.captureImage()!= ESP_OK) {
    //stop
    Serial.println("Capture img failed");
    Utility::sleep();
  } 
  //CHECK FOR HUMAN
  if(!cam.getHasHuman()){
    //stop
    Serial.println("no human Face");
    Utility::sleep();
  }

  //TRANSMIT
  Image img = cam.getImage();
  Serial.println("");

  //CONNECT WIFI
  Network net;
  Utility::setRGB(0,0,1);
  bool verified = net.verifyFace(img);
  if(verified){
    Utility::setRGB(0,1,0);
  }else{
    Utility::setRGB(1,0,0);
    Serial.println("verify face failed");
  }
  delay(2000);  
  Utility::setRGB(0,0,0);
  //SLEEP
  Utility::sleep();  
  
}


void loop() {  
  //never gets here.
}
