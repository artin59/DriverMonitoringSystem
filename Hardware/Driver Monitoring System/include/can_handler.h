#include "mcp2515.h"
#include <Arduino.h>


#define CAN_BITRATE CAN_500KBPS
#define CAN_CLOCK MCP_8MHZ

#define DRIVER_ALERT_ID 0X067
#define EYES_CLOSED 0x01
#define EYES_OPEN 0X00

//sets bitrate and hardware interrupt
//Returns 1 on success, 0 on failure 
uint8_t canInit(uint8_t cs_pin, uint8_t int_pin);

//Returns 1 if an unserviced interrupt is waiting, 0 otherwise
uint8_t canIntPending(void);

//Reads all CAN frames from the MCP2515 and process it
void canProcess(void);