#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>
#include <Arduino.h>

#define PINS 4

/*
 * If pin 1 is LOW, and 2 is HIGH:  motor -> forwards
 * If pin 1 is HIGH, and 2 is LOW:  motor -> backwards
 * If pin 1 is HIGH, and 2 is HIGH: motor -> stops
 * If pin 1 is LOW, and 2 is LOW:   motor -> stops
 */

//Array structure: [right 1, right 2, left 1, left 2]

//Initialize the pins by setting them to output
void initialize(uint8_t *front, uint8_t *back);

//Stops all motors 
void stop(uint8_t *front, uint8_t *back);

//All motors forwards
void forwards(uint8_t *front, uint8_t *back);

//All motors backwards
void backwards(uint8_t *front, uint8_t *back);

//Stop and move repeatedly twice
void stopGo(uint8_t *front, uint8_t *back);


#endif