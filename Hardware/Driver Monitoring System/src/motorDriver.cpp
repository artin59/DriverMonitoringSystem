#include "motorDriver.h"

void motorInit(uint8_t *front, uint8_t *back){
    //Initialize wheel pins
    for (int i = 0; i < PINS; i++){
        pinMode(front[i], OUTPUT);
        pinMode(back[i], OUTPUT);
    }
}

void stop(uint8_t *front, uint8_t *back){
    for (int i = 0; i < PINS; i++){
        digitalWrite(front[i], HIGH);
        digitalWrite(back[i], HIGH);
    }
}

//If pin 1 is LOW, and 2 is HIGH:  motor -> forwards
void forwards(uint8_t *front, uint8_t *back) {
    for (int i = 0; i < PINS; i += 2) {  
        digitalWrite(front[i],   LOW);
        digitalWrite(front[i+1], HIGH);
        digitalWrite(back[i],    LOW);
        digitalWrite(back[i+1],  HIGH);
    }
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

void stopGo(uint8_t *front, uint8_t *back){
    stop(front, back);
    delay(500);
    forwards(front, back);
    delay(500);
    stop(front, back);
    delay(500);
    forwards(front, back);
}

