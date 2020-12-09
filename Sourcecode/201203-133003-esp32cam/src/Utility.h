#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <Arduino.h>
#include "driver/rtc_io.h"

#define RED_PIN 15
#define GREEN_PIN 2
#define BLUE_PIN 14
#define FLASH_PIN 4

class Utility
{
public:
    Utility();
    virtual ~Utility();
    static void initGPIO()
    {
        pinMode(FLASH_PIN, OUTPUT);
        digitalWrite(FLASH_PIN, LOW);

        pinMode(RED_PIN, OUTPUT);
        digitalWrite(RED_PIN, LOW);

        pinMode(BLUE_PIN, OUTPUT);
        digitalWrite(BLUE_PIN, LOW);

        pinMode(GREEN_PIN, OUTPUT);
        digitalWrite(GREEN_PIN, LOW);
    };    
    static void sleep()
    {
        //this is where i wanna sleep
        Serial.println("Setting up Sleep");

        rtc_gpio_set_direction(GPIO_NUM_15, RTC_GPIO_MODE_INPUT_OUTPUT);
        rtc_gpio_pulldown_en(GPIO_NUM_15);
        rtc_gpio_pullup_dis(GPIO_NUM_15); // set the pin as pull-up

        rtc_gpio_set_direction(GPIO_NUM_14, RTC_GPIO_MODE_INPUT_OUTPUT);
        rtc_gpio_pulldown_en(GPIO_NUM_14);
        rtc_gpio_pullup_dis(GPIO_NUM_14); // set the pin as pull-up

        rtc_gpio_set_direction(GPIO_NUM_2, RTC_GPIO_MODE_INPUT_OUTPUT);
        rtc_gpio_pulldown_en(GPIO_NUM_2);
        rtc_gpio_pullup_dis(GPIO_NUM_2);

        rtc_gpio_set_direction(GPIO_NUM_4, RTC_GPIO_MODE_INPUT_OUTPUT);
        rtc_gpio_pulldown_en(GPIO_NUM_4);
        rtc_gpio_pullup_dis(GPIO_NUM_4); // set the pin as pull-up                 // set the pin as pull-up
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,
                            ESP_PD_OPTION_ON); // keep the RTC IO domain powered

        esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1); //1 = High, 0 = Low

        Serial.println("Entering sleep");
        esp_deep_sleep_start();
    };
    static void toggleFlash(int on)
    {
        if (!on)
        {
            digitalWrite(FLASH_PIN, LOW);
        }
        else
        {
            digitalWrite(FLASH_PIN, HIGH);
        }
    };
    static void setRGB(int red, int green, int blue)
    {
        digitalWrite(RED_PIN, red ? HIGH : LOW);
        digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
        digitalWrite(GREEN_PIN, green ? HIGH : LOW);
    };
};

#endif /* UTILITY_H */