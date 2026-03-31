#include "alert.h"
#include "motorDriver.h"

uint8_t static *sFront;
uint8_t static *sBack;

void alertInit(uint8_t *front, uint8_t *back){
    sFront = front;
    sBack = back;
}

void alertTrigger(void){
    Serial.println("[ALERT] Driver has fallen ASLEEP!!");
    sDriverAsleep = 1;
}

void alertClear(void){
    Serial.println("Driver AWAKE!");
    sDriverAsleep = 0;
    forwards(sFront, sBack);
}

void alertRun(void){
    if (sDriverAsleep){
        stopGo(sFront, sBack);
    }
}