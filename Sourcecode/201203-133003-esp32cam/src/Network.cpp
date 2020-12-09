#include "Network.h"

#define ssid "nest-1"
#define pass "olsen#net"
#define host "192.168.86.34" //server
#define port 2555
#define wifi_reconnects 50
#define result_wait_time_ms 10000



Network::Network() {

}

Network::~Network(){

}

bool Network::verifyFace(Image image) {
    //connect wifi
    if(!wifiConnect()){
        return false;
    }
    //transmit image
    if(!transmitImage(image)){
        return false;
    }
    //get result
    if(getResult() == ResponseStatus::ACCESS){
        return true;
    }
    return false;
}

bool Network::transmitImage(Image image) {
    WiFiClient client;
    if(!client.connect(host,port)){
        Serial.println("Connection failed!");
        return false;
    }

  
    uint8_t buffer[image.getSize()];
    image.getImage(buffer);
    client.write_P((const char*)buffer,image.getSize());
    Serial.print("Bytes sent:");Serial.println(image.getSize());
    Serial.println("Image sent");
    client.flush();
    client.stop();
    Serial.println("Connection closed");
    //delay(5000);//make sure image is sent - may need to wait longer or shorter. 
    return true;
}

bool Network::wifiConnect() {
    Serial.println("WiFi connecting");  
    int failed = 0;

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
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

ResponseStatus Network::getResult(){
    WiFiServer server(2555);
    server.begin();    
    WiFiClient client;
    int buffer = 0;
    int waited = 0;
    while (waited < result_wait_time_ms){
        if ((client = server.available())){
            Serial.println("Client connected");
            if ((buffer = client.available())){
                char byte = client.read();
                server.stop();
                if (byte == 'a'){
                    return ResponseStatus::ACCESS;
                }
                else{
                    return ResponseStatus::DENIED;
                }
            }
        }
        delay(20);
        waited += 20;
    }
    return ResponseStatus::TIME_OUT;
}