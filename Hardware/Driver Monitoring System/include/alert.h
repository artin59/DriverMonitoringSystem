#ifndef ALERT_H
#define ALERT_H
#include <Arduino.h>

//Initializes by storing the pin arrays
void alert_init(uint8_t *front, uint8_t *back);

//Called by can_handler when payload == 0x01 (eyes closed)
void alert_trigger(void);

//Called by can_handler when payload == 0x00 (eyes open)
void alert_clear(void);

#endif