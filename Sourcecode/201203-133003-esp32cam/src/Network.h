#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include "Image.h"
#include <WiFi.h>

enum class ResponseStatus: char { TIME_OUT, ACCESS = 'a', DENIED = 'b' };

class Network {   
    
    public:
        Network();
        ~Network();
        bool verifyFace(Image image);
    private:
        bool transmitImage(Image image);
        ResponseStatus getResult();
        bool wifiConnect();        
};

#endif /* NETWORK_H */