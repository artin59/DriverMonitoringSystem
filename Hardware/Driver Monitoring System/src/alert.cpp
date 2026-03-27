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
    stopGo(sFront, sBack);
}

void alertClear(void){
    Serial.println("Driver AWAKE!");
    forwards(sFront, sBack);
}