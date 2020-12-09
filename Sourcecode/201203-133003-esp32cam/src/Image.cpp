#include "Image.h"

Image::Image(){
    
}

Image::~Image(){

}

void Image::getImage(uint8_t * buffer){
    memcpy(buffer,&vBuffer[0],vBuffer.size());
}

int Image::getSize(){
    return vBuffer.size();
}

void Image::writeToImageBuff(uint8_t * buffer, int size){
    vBuffer.insert(vBuffer.end(),buffer,buffer+size);
}
