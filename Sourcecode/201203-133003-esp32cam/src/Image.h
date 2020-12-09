#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <Arduino.h>

class Image {
    public:
        Image();
        virtual ~Image();
        void getImage(uint8_t * buffer);
        void writeToImageBuff(uint8_t * buffer, int size);
        int getSize();
    private:
       std::vector<uint8_t> vBuffer; //actual image
};

#endif /* IMAGE_H */