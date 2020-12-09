#include "Camera.h"
#include "Utility.h"



//#define FLASH_PIN 4
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"

Camera::Camera() {
}

Camera::~Camera() {

}

esp_err_t Camera::captureImage() {
    //size_t out_len, out_width, out_height;
    uint8_t * out_buf;
    bool s;
    hasHuman = false;

    Utility::toggleFlash(1);   
    //toggleFlash(1);
    
    delay(200);//allow light to illuminate
    cam = esp_camera_fb_get();

    if (!cam) {
        Serial.println("Camera capture failed");
        return ESP_FAIL;
    }

    Utility::toggleFlash(0);


    if(cam->format == PIXFORMAT_JPEG){    
    Serial.println("Image captured");
    //client.write_P((const char*)cam->buf,cam->len);

    //GET IMAGE HERE....
    image = Image();
    image.writeToImageBuff(cam->buf,cam->len);

    } else {
        esp_camera_fb_return(cam);
        Serial.println("Non-JPEG data not implmented");
        return ESP_FAIL;
    }

    dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, cam->width, cam->height, 3);
    if (!image_matrix) {
        esp_camera_fb_return(cam);
        Serial.println("dl_matrix3du_alloc failed");
        return ESP_FAIL;
    }

    out_buf = image_matrix->item;
    //out_len = cam->width * cam->height * 3;
    //out_width = cam->width;
    //out_height = cam->height;

    s = fmt2rgb888(cam->buf, cam->len, cam->format, out_buf);
    s = fmt2rgb888(cam->buf, cam->len, cam->format, out_buf);
    esp_camera_fb_return(cam);

    if(!s){
        dl_matrix3du_free(image_matrix);
        Serial.println("to rgb888 failed");
        return ESP_FAIL;
    }

    box_array_t *net_boxes = face_detect(image_matrix, &mtmn_config);
    
    
        
    if (net_boxes){
        hasHuman = true;
        free(net_boxes->score);
        free(net_boxes->box);
        free(net_boxes->landmark);
        free(net_boxes);
    }
    dl_matrix3du_free(image_matrix);
    return ESP_OK;
}


Image Camera::getImage() {
    return image;
}

bool Camera::getHasHuman() {
    return hasHuman;
}

esp_err_t Camera::initCam() {
    //pinMode(FLASH_PIN, OUTPUT);
    //toggleFlash(0);

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
    if (psramFound())
    {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    }
    else
    {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return err;
    }

    sensor_t *s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV3660_PID)
    {
        s->set_vflip(s, 1);       // flip it back
        s->set_brightness(s, 1);  // up the brightness just a bit
        s->set_saturation(s, -2); // lower the saturation
    }

    //set frame size and quality
    //s->set_framesize(s, FRAMESIZE_CIF);
    s->set_framesize(s, FRAMESIZE_QVGA);
    s->set_quality(s, 20);

    //use effect!!!
    //s->set_special_effect(s,2); //0-6 dont know what effect each number is

    mtmn_config.type = FAST;
    mtmn_config.min_face = 60;
    mtmn_config.pyramid = 0.707;
    mtmn_config.pyramid_times = 4;
    mtmn_config.p_threshold.score = 0.15; //0.6
    mtmn_config.p_threshold.nms = 0.7;
    mtmn_config.p_threshold.candidate_number = 20;
    mtmn_config.r_threshold.score = 0.15; //0.6
    mtmn_config.r_threshold.nms = 0.7;
    mtmn_config.r_threshold.candidate_number = 10;
    mtmn_config.o_threshold.score = 0.15; //0.1
    mtmn_config.o_threshold.nms = 0.7;
    mtmn_config.o_threshold.candidate_number = 2;

    Serial.println("Camera Ready!");
    return ESP_OK;
}

        