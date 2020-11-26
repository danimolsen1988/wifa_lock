#include <Arduino.h>
#include <WiFi.h>
#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>
#include <vector>

#define ssid "TP-LINK_2.4GHz"
#define pass "****"

void loopy();

WiFiServer server(2555);

TFT_eSPI tft = TFT_eSPI(); //invoke lib
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  Serial.println("Setting up TFT-display");
  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_RED);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  Serial.println("TFT-display ready!");

  Serial.println("WiFi connecting");
  // setup wifi
  while(WiFi.begin(ssid)!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin(); 
  delay(500); 
  Serial.print("Server on port 2555 & IP: "); Serial.println(WiFi.localIP());  
}


std::vector<uint8_t> imgVector;
bool sizeRead = false;
uint32_t fileSize = 0;
uint32_t totalSize = 0;
uint8_t * img;
void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client){
    Serial.println("Client connected");
    while(client.connected()){
      if((fileSize = client.available())){
          Serial.println("LOOP");      
          img = new uint8_t[fileSize];
          client.readBytes(img,fileSize);
          Serial.println(fileSize);
          imgVector.insert(imgVector.end(), img, img + fileSize);    
          totalSize += fileSize;
      }
    }
    //CLIENT SHOULD ALWAYS disconnect after sending
    Serial.print("Bytes recieved: "); Serial.println(totalSize); 
    Serial.print("IMG recieved - file size: "); Serial.println(imgVector.size());  
    uint16_t w = 0, h = 0;    
    TJpgDec.getJpgSize(&w, &h, &imgVector[0], imgVector.size());
    Serial.print("Width:"); Serial.print(w); Serial.print(", Height:"); Serial.println(h);

    // Draw the image, top left at 0,0
    TJpgDec.drawJpg(0, 0, &imgVector[0], imgVector.size());
    fileSize = 0; 
    totalSize = 0;
    std::vector<uint8_t>().swap(imgVector); //clear
    Serial.println("");
    Serial.println("");
  }
  delay(20);
}

void loopy(){
    // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client){
    Serial.println("Client connected");
    int count = 0;
    uint8_t msgSize[4];
    while(client.connected()){
      if(client.available()){      
        if(!sizeRead){
          Serial.println("Read image size");
          client.readBytes(msgSize,4);
          //Convert to uint32_t
          fileSize = msgSize[0] + (msgSize[1] << 8) + (msgSize[2] << 16) + (msgSize[3] << 24);
          Serial.print("Image size: ");Serial.println(fileSize);
          //uint8_t arr[fileSize+1];
          img = new uint8_t[fileSize+1];
          //img = arr;
          //img.clear();
          //img.resize(fileSize);
          //std::vector<uint8_t>().swap(img); //clear
          //img.reserve(fileSize+1);          
          sizeRead = true;
        }else{
          //Serial.print("Reading byte");Serial.println(count);
          //first lump is read, read img bytes
          uint8_t data = client.read();
          //img.push_back(data);
          img[count] = data;
          count++;
        }
      }
    }
    Serial.print("COUNT: "); Serial.println(count);
    //CLIENT SHOULD ALWAYS disconnect after sending
    //Serial.print("IMG recieved - file size: "); Serial.println(img.size());
    //Serial.println((char*)&img[0]);
    Serial.print("IMG recieved - file size: "); Serial.println((sizeof(img)));    
    Serial.println("");
    Serial.println("");    

    //uint8_t tempArr[img.size()];
    //std::copy(img.begin(),img.end(),tempArr);

    uint16_t w = 0, h = 0;
    //TJpgDec.getJpgSize(&w, &h, tempArr, fileSize);
    TJpgDec.getJpgSize(&w, &h, img, fileSize);

    Serial.print("Width:"); Serial.print(w); Serial.print(", Height:"); Serial.println(h);

    // Draw the image, top left at 0,0
    //TJpgDec.drawJpg(0, 0, tempArr, fileSize);
    TJpgDec.drawJpg(0, 0, img, fileSize);
    //img.clear();
    sizeRead =false;   
    fileSize = 0; 
  }
  delay(20);
}