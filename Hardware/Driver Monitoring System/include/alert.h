#ifndef ALERT_H
#define ALERT_H
#include <Arduino.h>

//Initializes by storing the pin arrays
void alertInit(uint8_t *front, uint8_t *back);

//Called by can_handler when payload == 0x01 (eyes closed)
void alertTrigger(void);

//Called by can_handler when payload == 0x00 (eyes open)
void alertClear(void);

#endif