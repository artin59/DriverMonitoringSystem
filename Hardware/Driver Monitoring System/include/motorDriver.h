#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>

/*
 * If pin 1 is LOW, and 2 is HIGH:  motor -> forwards
 * If pin 1 is HIGH, and 2 is LOW:  motor -> backwards
 * If pin 1 is HIGH, and 2 is HIGH: motor -> stops
 * If pin 1 is LOW, and 2 is LOW:   motor -> stops
 */

typedef struct{
    uint8_t left1;
    uint8_t left2;
    uint8_t right1;
    uint8_t right2;
} MotorPins;


//Initialize the pins by setting them to output
void initialize(MotorPins *right, MotorPins *left);

//Stops all motors 
void stop(MotorPins *right, MotorPins *left);

//All motors forwards
void forwards(MotorPins *right, MotorPins *left);

//All motors backwards
void backwarsd(MotorPins *right, MotorPins *left);


#endif