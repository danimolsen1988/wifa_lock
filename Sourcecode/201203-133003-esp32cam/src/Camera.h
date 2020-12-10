#ifndef CAMERA_H
#define CAMERA_H

#include "Image.h"
#include <Arduino.h>
#include "esp_camera.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "img_converters.h"

class Camera {
    public:
        Camera();
        virtual ~Camera();
        esp_err_t captureImage();
        bool getHasHuman();
        Image getImage();
        esp_err_t initCam();
    private:
        Image image;
        bool hasHuman;
        camera_fb_t * cam = NULL;        
        mtmn_config_t mtmn_config = {0};
};

#endif /* CAMERA_H */