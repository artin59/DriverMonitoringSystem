#include "motorDriver.h"

void motorInit(uint8_t *front, uint8_t *back){
    //Initialize wheel pins
    pinMode(front[0], OUTPUT);
    pinMode(front[1], OUTPUT);
    pinMode(front[2], OUTPUT);
    pinMode(front[3], OUTPUT);

    pinMode(back[0], OUTPUT);
    pinMode(back[1], OUTPUT);
    pinMode(back[2], OUTPUT);
    pinMode(back[3], OUTPUT);
}

//Stop all motors by setting both pins of every pair HIGH
void stop(uint8_t *front, uint8_t *back){
    for (int i = 0; i < PINS; i++){
        digitalWrite(front[i], HIGH);
        digitalWrite(back[i], HIGH);
    }
}

//If pin 1 is LOW, and 2 is HIGH:  motor -> forwards
//Drives all motors forwards
void forwards(uint8_t *front, uint8_t *back) {
    for (int i = 0; i < PINS; i += 2) {
        digitalWrite(front[i],   LOW);
        digitalWrite(front[i+1], HIGH);
        digitalWrite(back[i],    LOW);
        digitalWrite(back[i+1],  HIGH);
    }
}

//If pin 1 is HIGH, and 2 is LOW:  motor -> backwards
//Drives all motors backwards
void backwards(uint8_t *front, uint8_t *back) {
    for (int i = 0; i < PINS; i += 2) {
        digitalWrite(front[i],   HIGH);
        digitalWrite(front[i+1], LOW);
        digitalWrite(back[i],    HIGH);
        digitalWrite(back[i+1],  LOW);
    }
}

//Non-blocking state machine that alternates between forwards and stopped every 300ms
void stopGo(uint8_t *front, uint8_t *back) {
    static uint8_t phase = 0;       //Tracks current state: 0 = stopped, 1 = forwards
    static uint32_t lastTime = 0;

    uint32_t now = millis();

    //If in stopped state and 300 ms has passed drive all motors forwards and change state to forwards
    if (phase == 0 && (now - lastTime >= 300)) {
        forwards(front, back);
        lastTime = now;
        phase = 1;
    } 
    //If in forwards state and 300 ms has passed stop all motors and change state to stopped
    else if (phase == 1 && (now - lastTime >= 300)) {
        stop(front, back);
        lastTime = now;
        phase = 0;
    }
}
