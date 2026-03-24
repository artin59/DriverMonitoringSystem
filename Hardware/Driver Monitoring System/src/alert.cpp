#include "alert.h"
#include "motorDriver.h"

uint8_t static *s_front;
uint8_t static *s_back;

void alert_init(uint8_t *front, uint8_t *back){
    s_front = front;
    s_back = back;
}

void alert_trigger(void){
    Serial.println("[ALERT] Driver has fallen ASLEEP!!");
    stopGo(s_front, s_back);
}

void alert_clear(void){
    Serial.println("Driver AWAKE!");
    forwards(s_front, s_back);
}