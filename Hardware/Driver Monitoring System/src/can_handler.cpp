#include "can_handler.h"
#include "alert.h"

static MCP2515 s_mcp;

static volatile uint8_t intFlag = 0;
static uint8_t sIntPin = 0;

static void canISR(void){
    intFlag = 1;
}

uint8_t canInit(uint8_t csPin, uint8_t intPin){
    
    sIntPin = intPin;
    s_mcp = MCP2515(csPin);
    s_mcp.reset();

    if (s_mcp.setBitrate(CAN_BITRATE, CAN_CLOCK) != MCP2515::ERROR_OK) {
        Serial.println("CAN setBitrate failed");
        return 0; 
    }

    s_mcp.setFilterMask(MCP2515::MASK0, false, 0x7FFU);
    s_mcp.setFilter(MCP2515::RXF0,      false, DRIVER_ALERT_ID);

     if (s_mcp.setNormalMode() != MCP2515::ERROR_OK) {
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

    if (frame->can_dlc < 1U) {
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
    
    uint8_t irq = s_mcp.getInterrupts();

    if (irq & MCP2515::CANINTF_RX0IF) {
    
        if (s_mcp.readMessage(MCP2515::RXB0, &frame) == MCP2515::ERROR_OK) {
            dispatch_frame(&frame);
        }
    }

    if (irq & MCP2515::CANINTF_RX1IF) {
        if (s_mcp.readMessage(MCP2515::RXB1, &frame) == MCP2515::ERROR_OK) {
            dispatch_frame(&frame);
        }
    }

}