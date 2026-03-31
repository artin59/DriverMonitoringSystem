#include <Arduino.h>
#include "motorDriver.h"
#include "alert.h"
#include "can_handler.h"

#define FRONT_RIGHT_1 9
#define FRONT_RIGHT_2 8
#define FRONT_LEFT_1 7
#define FRONT_LEFT_2 6

#define BACK_RIGHT_1 5
#define BACK_RIGHT_2 4
#define BACK_LEFT_1 3
#define BACK_LEFT_2 3

#define CAN_CS 10
#define CAN_INT 2

//SCK: D13
//SI: D11
//SO: D12


static uint8_t front[4] = {FRONT_RIGHT_1, FRONT_RIGHT_2, FRONT_LEFT_1, FRONT_LEFT_2};
static uint8_t back[4] = {BACK_RIGHT_1, BACK_RIGHT_2, BACK_LEFT_1, BACK_LEFT_2};

void setup() {

  delay(5000);

  Serial.begin(9600);
  motorInit(front, back);
  alertInit(front, back);
  forwards(front, back);


  while (canInit(CAN_CS, CAN_INT) == 0){
     Serial.println("CAN init failed");
     delay(1000);
  }

  Serial.println("System ready!");


}

void loop() {

  if (canIntPending()){
     canProcess();
  }
  alertRun();
}
