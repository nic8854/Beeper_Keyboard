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

#include "noteToFrequency.h"

#define TAG "KEYBOARD"
#define MELODY_SPEED 75 //ms per step

static octave_t octave = Octave_4;
static uint8_t volume = 100;
static bool decay = false;

EventGroupHandle_t keyboardHitEventGroup;
#define KEYBOARD_C          (1 << 0)  // bit 0
#define KEYBOARD_CS         (1 << 1)  // bit 1
#define KEYBOARD_D          (1 << 2)  // bit 2
#define KEYBOARD_DS         (1 << 3)  // bit 3
#define KEYBOARD_E          (1 << 4)  // bit 4
#define KEYBOARD_F          (1 << 5)  // bit 5
#define KEYBOARD_FS         (1 << 6)  // bit 6
#define KEYBOARD_G          (1 << 7)  // bit 7
#define KEYBOARD_GS         (1 << 8)  // bit 8
#define KEYBOARD_A          (1 << 9)  // bit 9
#define KEYBOARD_AS         (1 << 10) // bit 10
#define KEYBOARD_H          (1 << 11) // bit 11
#define KEYBOARD_ALL        (KEYBOARD_C | KEYBOARD_CS | KEYBOARD_D | KEYBOARD_DS | KEYBOARD_E | KEYBOARD_F | KEYBOARD_FS | KEYBOARD_G | KEYBOARD_GS | KEYBOARD_A | KEYBOARD_AS | KEYBOARD_H)

static EventBits_t keyFromTouchPoint(int x, int y) {
    const int screenWidth = (int)lcdGetWidth();
    const int screenHeight = (int)lcdGetHeight();
    const int top = 16;
    const int left = 0;
    const int right = (screenWidth > 0 ? screenWidth : CONFIG_WIDTH) - 1;
    const int bottom = (screenHeight > 0 ? screenHeight : CONFIG_HEIGHT) - 1;

    if (x < left || x > right || y < top || y > bottom) {
        return 0;
    }

    const int keyboardWidth = right - left + 1;
    const int whiteKeyWidth = keyboardWidth / 7;
    const int whiteKeyHeight = bottom - top + 1;
    const int blackKeyWidth = (whiteKeyWidth * 2) / 3;
    const int blackKeyHeight = (whiteKeyHeight * 3) / 5;

    const int blackKeyAfterWhite[] = {0, 1, 3, 4, 5};
    const EventBits_t blackKeyBits[] = {KEYBOARD_CS, KEYBOARD_DS, KEYBOARD_FS, KEYBOARD_GS, KEYBOARD_AS};
    for (int i = 0; i < (int)(sizeof(blackKeyAfterWhite) / sizeof(blackKeyAfterWhite[0])); i++) {
        const int idx = blackKeyAfterWhite[i];
        const int boundaryX = left + ((idx + 1) * whiteKeyWidth);
        int x1 = boundaryX - (blackKeyWidth / 2);
        int x2 = x1 + blackKeyWidth - 1;
        if (x1 < left) x1 = left;
        if (x2 > right) x2 = right;

        if (x >= x1 && x <= x2 && y >= top && y <= (top + blackKeyHeight)) {
            return blackKeyBits[i];
        }
    }

    const EventBits_t whiteKeyBits[] = {KEYBOARD_C, KEYBOARD_D, KEYBOARD_E, KEYBOARD_F, KEYBOARD_G, KEYBOARD_A, KEYBOARD_H};
    int whiteIdx = (x - left) / whiteKeyWidth;
    if (whiteIdx < 0) whiteIdx = 0;
    if (whiteIdx > 6) whiteIdx = 6;
    return whiteKeyBits[whiteIdx];
}

static void playStartupMelody() {
    buzzer_start(noteToFrequency(Note_C, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_D, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_E, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_F, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_G, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_A, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_H, Octave_3), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_C, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_D, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_E, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_F, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_G, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_A, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_H, Octave_4), MELODY_SPEED);
    vTaskDelay(MELODY_SPEED/portTICK_PERIOD_MS);
    buzzer_start(noteToFrequency(Note_C, Octave_5), MELODY_SPEED * 3);
}

static void drawKeyboardOctave(void) {
    const int screenWidth = (int)lcdGetWidth();
    const int screenHeight = (int)lcdGetHeight();
    const int top = 16;
    const int left = 0;
    const int right = (screenWidth > 0 ? screenWidth : CONFIG_WIDTH) - 1;
    const int bottom = (screenHeight > 0 ? screenHeight : CONFIG_HEIGHT) - 1;

    const int keyboardWidth = right - left + 1;
    const int whiteKeyWidth = keyboardWidth / 7;
    const int whiteKeyHeight = bottom - top + 1;
    const int blackKeyWidth = (whiteKeyWidth * 2) / 3;
    const int blackKeyHeight = (whiteKeyHeight * 3) / 5;

    // Draw white keys
    for (int i = 0; i < 7; i++) {
        int x1 = left + (i * whiteKeyWidth);
        int x2 = x1 + whiteKeyWidth - 1;
        if (x2 > right) x2 = right;
        lcdDrawFillRect(x1, top, x2, bottom, WHITE);
        lcdDrawRect(x1, top, x2, bottom, GRAY);
    }

    // Draw black keys
    const int blackKeyAfterWhite[] = {0, 1, 3, 4, 5};
    for (int i = 0; i < (int)(sizeof(blackKeyAfterWhite) / sizeof(blackKeyAfterWhite[0])); i++) {
        const int idx = blackKeyAfterWhite[i];
        const int boundaryX = left + ((idx + 1) * whiteKeyWidth);
        int x1 = boundaryX - (blackKeyWidth / 2);
        int x2 = x1 + blackKeyWidth - 1;
        if (x1 < left) x1 = left;
        if (x2 > right) x2 = right;
        lcdDrawFillRect(x1, top, x2, top + blackKeyHeight, BLACK);
        lcdDrawRect(x1, top, x2, top + blackKeyHeight, GRAY);
    }
    lcdUpdateVScreen();
}

void guiTask(void* param) {
    EventBits_t lastKey = 0;

    drawKeyboardOctave();

    for(;;) {
        EventBits_t currentKey = 0;

        if (ft6236_is_touched()) {
            touchevent_t te = ft6236_get_touch_event(true);
            if (te.touches > 0) {
                const int x = te.points[0].x;
                const int y = te.points[0].y;
                currentKey = keyFromTouchPoint(x, y);
            }
        }

        if (currentKey != lastKey) {
            xEventGroupClearBits(keyboardHitEventGroup, KEYBOARD_ALL);
            if (currentKey != 0) {
                xEventGroupSetBits(keyboardHitEventGroup, currentKey);
                ESP_LOGI(TAG, "Key press bit: 0x%04lx", (unsigned long)currentKey);
            }
            lastKey = currentKey;
        }

        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void soundTask(void* param) {
    EventBits_t eventBits = 0;
    uint16_t noteToPlay = 0;
    uint16_t lastPlayedNote = 0;
    uint8_t decayVolume = volume;

    buzzer_set_volume(volume);
    playStartupMelody();

    for(;;) {
        if(xEventGroupWaitBits(keyboardHitEventGroup, KEYBOARD_ALL, pdFALSE, pdFALSE, 10/portTICK_PERIOD_MS)) {
            eventBits = xEventGroupGetBits(keyboardHitEventGroup);
            if(eventBits & KEYBOARD_C) noteToPlay = noteToFrequency(Note_C, octave);
            else if(eventBits & KEYBOARD_CS) noteToPlay = noteToFrequency(Note_Cs, octave);
            else if(eventBits & KEYBOARD_D) noteToPlay = noteToFrequency(Note_D, octave);
            else if(eventBits & KEYBOARD_DS) noteToPlay = noteToFrequency(Note_Ds, octave);
            else if(eventBits & KEYBOARD_E) noteToPlay = noteToFrequency(Note_E, octave);
            else if(eventBits & KEYBOARD_F) noteToPlay = noteToFrequency(Note_F, octave);
            else if(eventBits & KEYBOARD_FS) noteToPlay = noteToFrequency(Note_Fs, octave);
            else if(eventBits & KEYBOARD_G) noteToPlay = noteToFrequency(Note_G, octave);
            else if(eventBits & KEYBOARD_GS) noteToPlay = noteToFrequency(Note_Gs, octave);
            else if(eventBits & KEYBOARD_A) noteToPlay = noteToFrequency(Note_A, octave);
            else if(eventBits & KEYBOARD_AS) noteToPlay = noteToFrequency(Note_As, octave);
            else if(eventBits & KEYBOARD_H) noteToPlay = noteToFrequency(Note_H, octave);
            
        } else {
            noteToPlay = 0;
        }
        if(noteToPlay != lastPlayedNote) {
            if(noteToPlay != 0) {
                decayVolume = volume;
                buzzer_start(noteToPlay, 10000);
                ESP_LOGI(TAG, "Playing note: %d Hz", noteToPlay);
            } else {
                buzzer_stop();
                ESP_LOGI(TAG, "Stopping sound");
            }
            lastPlayedNote = noteToPlay;
        }
        if(noteToPlay != 0 && decayVolume > 5 && decay) {
            buzzer_set_volume(decayVolume);
            decayVolume--;
            ESP_LOGI(TAG, "Decaying volume: %d", decayVolume);
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void inputTask(void* param) {
    int32_t rotationChange = 0;
    for(;;) {
        if(button_get_state(SW0, true) == SHORT_PRESSED) {
            if(octave > Octave_0) {
                octave --;
            }

        }

        if(button_get_state(SW1, true) == SHORT_PRESSED) {
            if(octave < Octave_8) {
                octave ++;
            }
        }

        if(button_get_state(SW2, true) == SHORT_PRESSED) {
            decay = !decay;
        }

        for(int i = LED0; i <= LED7; i++) {
            if(i < octave) {
                led_set(i, true);
            } else {
                led_set(i, false);
            }
        }
        
        rotationChange = rotary_encoder_get_rotation(true);
            if(rotationChange != 0) {
                if(volume + rotationChange > 100) {
                    volume = 100;
                } else if(volume + rotationChange < 1) {
                    volume = 1;
                } else {
                    volume += rotationChange;
                }
                buzzer_set_volume(volume);
                ESP_LOGI(TAG, "Volume changed to: %d", volume);
        }

        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void app_main()
{
    //Initialize Eduboard2 BSP
    eduboard2_init();
    keyboardHitEventGroup = xEventGroupCreate();

    //Create templateTask
    xTaskCreatePinnedToCore(guiTask, "guiTask", 2*2048, NULL, 10, NULL, 0);
    xTaskCreatePinnedToCore(soundTask, "soundTask", 2*2048, NULL, 10, NULL, 1);
    xTaskCreatePinnedToCore(inputTask, "inputTask", 2*2048, NULL, 10, NULL, 1);

    return;
}