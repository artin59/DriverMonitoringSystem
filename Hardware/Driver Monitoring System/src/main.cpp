#include <Arduino.h>
#include "motorDriver.h"
#include "alert.h"
#include "can_handler.h"


#define FRONT_RIGHT_1 12
#define FRONT_RIGHT_2 11
#define FRONT_LEFT_1 10
#define FRONT_LEFT_2 9

#define BACNK_RIGHT_1 8
#define BACK_RIGHT_2 7
#define BACK_LEFT_1 6
#define BACK_LEFT_2 5

#define CAN_CS 4
#define CAN_INT  2

static uint8_t front[4] = {FRONT_RIGHT_1, FRONT_RIGHT_2, FRONT_LEFT_1, FRONT_LEFT_2};
static uint8_t back[4] = {BACNK_RIGHT_1, BACK_RIGHT_2, BACK_LEFT_1, BACK_LEFT_2};

void setup() {

  Serial.begin(9600);
  
  motorInit(front, back);
  alertInit(front, back);

  if (!canInit(CAN_CS, CAN_INT)) {
    Serial.println("CAN init failed");
    while (1);
  }

  Serial.println("System ready!");

  forwards(front, back);

}

void loop() {

  if (canIntPending()){

    canProcess();


  }

}
