#include "esp_camera.h"
#include <Arduino.h>
#include <vector>
#include <WiFi.h>


#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#define ssid "TP-LINK_2.4GHz"
#define pass "****"
#define host "192.168.0.102"
#define port 2555
#define wifi_reconnects 10

#include "camera_pins.h"

void captureImage();
bool connectWiFi();
esp_err_t initCam();
void goToSleep();
void toggleFlash(int on);

std::vector<uint8_t> capture();
camera_fb_t * cam = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");
  //setup flash
  pinMode(4, OUTPUT);
  toggleFlash(0);

  //init cam
  if(initCam() != ESP_OK){
    //sleep
    goToSleep();
  }

  //connect wifi
  if(!connectWiFi()){
    //sleep
    goToSleep();
  }

  //capture img and transmit
  captureImage();

  //goto sleep
  goToSleep();

}

// deep sleep
void goToSleep(){
  //this is where i wanna sleep
  Serial.println("Setting up Sleep");
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1); //1 = High, 0 = Low   

  Serial.println("Entering sleep");
  esp_deep_sleep_start();
}

// toggle flash
void toggleFlash(int on){
  if(!on){
    digitalWrite(4, LOW);
  }else{
    digitalWrite(4, HIGH);
  }  
}

// setup cam
esp_err_t initCam(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;


  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

   // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return err;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  
  //set frame size and quality
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_quality(s,20);
  
  //use effect!!!
  //s->set_special_effect(s,2); //0-6 dont know what effect each number is

  Serial.println("Camera Ready!");
  return ESP_OK;
}

// capture image and send
void captureImage(){
  //flash on
  toggleFlash(1);
  delay(200);//allow light to illuminate
  Serial.println("Connecting to ");Serial.println(host);
  WiFiClient client;
  if(!client.connect(host,port)){
    Serial.println("Connection failed!");
    return;
  }

  cam = esp_camera_fb_get();
  if (!cam) {
    Serial.println("Camera capture failed");
    throw;
  }
  //flash off
  toggleFlash(0);
  if(cam->format == PIXFORMAT_JPEG){    
    Serial.println("Image captured");
    client.write_P((const char*)cam->buf,cam->len);
  } else {
    Serial.println("Non-JPEG data not implmented");
    return;
  }
  Serial.print("Bytes sent:");Serial.println(cam->len);
  Serial.println("Image sent");
  client.flush();
  client.stop();
  Serial.println("Connection closed");
  delay(5000);//make sure image is sent
  esp_camera_fb_return(cam);  
}

// setup wifi
bool connectWiFi(){  
  Serial.println("WiFi connecting");  
  int failed = 0;
  while(WiFi.begin(ssid)!=WL_CONNECTED){
    delay(500);
    failed++;
    if(failed >= wifi_reconnects){
      Serial.println("WiFi - connection error");
      return false;
    }
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(200);
  return true;
}



void loop() {
  // put your main code here, to run repeatedly: 
  //not used with deepsleep
}




