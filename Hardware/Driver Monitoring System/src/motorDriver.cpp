#include "motorDriver.h"

void motorInit(uint8_t *front, uint8_t *back){
    //Initialize wheel pins
    pinMode(front[0], OUTPUT);
    pinMode(front[1], OUTPUT);
    pinMode(front[2], OUTPUT);
    pinMode(front[3], OUTPUT);
}

void stop(uint8_t *front, uint8_t *back){
    for (int i = 0; i < PINS; i++){
        digitalWrite(front[i], HIGH);
        digitalWrite(back[i], HIGH);
    }
}

//If pin 1 is LOW, and 2 is HIGH:  motor -> forwards
void forwards(uint8_t *front, uint8_t *back) {
    digitalWrite(front[0], HIGH);
    digitalWrite(front[1], LOW);

    digitalWrite(front[2], HIGH);
    digitalWrite(front[3], LOW);
}

//If pin 1 is HIGH, and 2 is LOW:  motor -> backwards
void backwards(uint8_t *front, uint8_t *back) {
    for (int i = 0; i < PINS; i += 2) {
        digitalWrite(front[i],   HIGH);
        digitalWrite(front[i+1], LOW);
        digitalWrite(back[i],    HIGH);
        digitalWrite(back[i+1],  LOW);
    }
}

void stopGo(uint8_t *front, uint8_t *back) {
    static uint8_t phase = 0;       // 0 = stopped, 1 = forwards
    static uint32_t lastTime = 0;

    uint32_t now = millis();

    if (phase == 0 && (now - lastTime >= 300)) {
        forwards(front, back);
        lastTime = now;
        phase = 1;
    } else if (phase == 1 && (now - lastTime >= 300)) {
        stop(front, back);
        lastTime = now;
        phase = 0;
    }
}
