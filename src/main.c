/********************************************************************************************* */
//    Eduboard2 ESP32-S3 Template with BSP
//    Author: Martin Burger
//    Juventus Technikerschule
//    Version: 1.0.0
//    
//    This is the ideal starting point for a new Project. BSP for most of the Eduboard2
//    Hardware is included under components/eduboard2.
//    Hardware support can be activated/deactivated in components/eduboard2/eduboard2_config.h
/********************************************************************************************* */
#include "eduboard2.h"
#include "memon.h"

#include "notes_to_freq.h"

#define TAG "KEYBOARD"

void templateTask(void* param) {

    buzzer_start(NOTE_C3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_D3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_E3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_F3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_G3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_A3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_H3, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_C4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_D4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_E4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_F4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_G4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_A4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_H4, 100);
    vTaskDelay(100/portTICK_PERIOD_MS);
    buzzer_start(NOTE_C5, 300);
    vTaskDelay(100/portTICK_PERIOD_MS);

    for(;;) {
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void app_main()
{
    //Initialize Eduboard2 BSP
    eduboard2_init();

    //Create templateTask
    xTaskCreatePinnedToCore(templateTask, "templateTask", 2*2048, NULL, 10, NULL, 0);

    return;
}