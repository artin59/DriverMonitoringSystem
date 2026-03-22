#include "motorDriver.h"


void initialize(uint8_t *front, uint8_t *back){
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
void forwards(uint8_t *front, uint8_t *back){
    for (int i = 0; i < PINS; i++){
        if (i % 2 == 0){
            digitalWrite(front[i], LOW);
            digitalWrite(back[i], LOW);
        }
        else{
            digitalWrite(front[i], HIGH);
            digitalWrite(back[i], HIGH);
        }
    }
}

//If pin 1 is HIGH, and 2 is LOW:  motor -> backwards
void backwards(uint8_t *front, uint8_t *back){
    for (int i = 0; i < PINS; i++){
        if (i % 2 == 0){
            digitalWrite(front[i], HIGH);
            digitalWrite(back[i], HIGH);
        }
        else{
            digitalWrite(front[i], LOW);
            digitalWrite(back[i], LOW);
        }
    }
}