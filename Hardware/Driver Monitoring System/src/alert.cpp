#include "alert.h"
#include "motorDriver.h"


//Module-private pointers to the motor pin arrays passed in from main
uint8_t static *sFront;
uint8_t static *sBack;

//Store the front and back pin array pointers for use by other alert functions
void alertInit(uint8_t *front, uint8_t *back){
    sFront = front;
    sBack = back;
}

//Set the drowsy flag and print a warning when closed eyes are detected
void alertTrigger(void){
    Serial.println("[ALERT] Driver has fallen ASLEEP!!");
    sDriverAsleep = 1;
}

//Clear the drowsy flag, print confirmation, and resume driving forwards
void alertClear(void){
    Serial.println("Driver AWAKE!");
    sDriverAsleep = 0;
    forwards(sFront, sBack);
}

//Call stopGo every loop iteration while the driver is marked as asleep
void alertRun(void){
    if (sDriverAsleep){
        stopGo(sFront, sBack);
    }
}