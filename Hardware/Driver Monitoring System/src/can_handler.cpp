#include "can_handler.h"
#include "alert.h"
#include <SPI.h>

MCP2515 mcp2515(10);

static volatile uint8_t intFlag = 0;
static uint8_t sIntPin = 0;

static void canISR(void){
    intFlag = 1;
}

uint8_t canInit(uint8_t csPin, uint8_t intPin){

    sIntPin = intPin;
    mcp2515.reset();

    if (mcp2515.setBitrate(CAN_BITRATE, CAN_CLOCK) != MCP2515::ERROR_OK) {
        Serial.println("CAN setBitrate failed");
        return 0;
    }

    if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
        Serial.println("CAN Set normal mode failed");
        return 0;
    }

    pinMode(sIntPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sIntPin), canISR, FALLING);

    return 1;
}

uint8_t canIntPending(void){
    return intFlag;
}

static void dispatch_frame(const struct can_frame *frame)
{
    if (frame->can_id != DRIVER_ALERT_ID) {
        return;
    }

    if (frame->can_dlc < 1) {
        return;
    }

    switch (frame->data[0]) {

        case EYES_CLOSED:
            alertTrigger();
            break;

        case EYES_OPEN:    
            alertClear();
            break;

        default:
            break;
    }
}

void canProcess(void){

    if (!intFlag) {
        return;
    }

    intFlag = 0;

    struct can_frame frame;
    
    uint8_t irq = mcp2515.getInterrupts();

    if (irq & MCP2515::CANINTF_RX0IF) {
    
        if (mcp2515.readMessage(MCP2515::RXB0, &frame) == MCP2515::ERROR_OK) {
            dispatch_frame(&frame);
        }
    }

    if (irq & MCP2515::CANINTF_RX1IF) {
        if (mcp2515.readMessage(MCP2515::RXB1, &frame) == MCP2515::ERROR_OK) {
            dispatch_frame(&frame);
        }
    }

}