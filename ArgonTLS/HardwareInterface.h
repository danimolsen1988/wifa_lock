#include "Particle.h"

class HardwareInterface
{
private:
    Servo myservo;
    int servopin = D2;
    int redPin = D3;
    int greenPin = D4;
    int bluePin = D5;

public:
    HardwareInterface(/* args */);
    ~HardwareInterface();
    void setup();
    void openLock(int command);
    void rgbControl(int color);
    void faceNotRecogged();
};

