#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(/* args */)
{
}

HardwareInterface::~HardwareInterface()
{
}

void HardwareInterface::setup() {
  myServo.attach(servopin);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// Consider adding an argument that specifies which motor to run
void HardwareInterface::openLock(int command) {
  if (command == 0)
  {
    //close
    Serial.println("Closing the lock");
    myServo.write(90);
  }
  else if (command == 1)
  {
    //open
    Serial.println("Opening the lock");
    myServo.write(0);
    //Implement nonblocking wait
  }
  else {
    //close
    Serial.println("Closing the lock");
    myServo.write(90);
  }
}

// 0  = All off
// 1 = Red on 
// 2 = Green on 
// 3 = blue pin on
void HardwareInterface::rgbControl(int color) {
      // Max brightness of LEDs
  int brightness = 128; 

  // All off
  if (color == 0){
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }
  // Red
  if (color == 1){
    analogWrite(redPin, brightness);

  }
  // Green
  else if (color == 2){
    //Set green part of RGB to light
    analogWrite(greenPin, brightness);
  }
  // BLue
  else if (color == 3){
    analogWrite(bluePin, brightness);

  }
}

void HardwareInterface::faceNotRecogged(){
  // Blink red LED a few times
  int blinkytimes = 2;
  for (int i = 0; i < blinkytimes; i++){
    rgbControl(1);
    myServo.write(90);
    delay(500);
    rgbControl(0);
    myServo.write(90);
    delay(500);
  }
}