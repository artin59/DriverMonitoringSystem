#include <Arduino.h>
#include "motorDriver.h"


#define FR1 12
#define FR2 11
#define FL1 10
#define FL2 9

#define BR1 8
#define BR2 7
#define BL1 6
#define BL2 5

uint8_t front[4] = {FR1, FR2, FL1, FL2};
uint8_t back[4] = {BR1, BR2, BL1, BL2};
void setup() {

  initialize(front, back);
  Serial.begin(9600);

}

void loop() {
}
