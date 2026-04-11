#include "can_handler.h"
#include "alert.h"
#include <SPI.h>

// MCP2515 object constructed with CS on pin 10 before canInit() is called
MCP2515 mcp2515(10);

//Volatile flag set by the ISR to signal a pending CAN frame to the main loop
static volatile uint8_t intFlag = 0;
//Stores the interrupt pin number passed in at init time
static uint8_t sIntPin = 0;

//ISR: runs immediately on the falling edge of INT pin, sets flag and returns
static void canISR(void){
    intFlag = 1;
}

//Configure the MCP2515 bitrate, enter normal mode, and attach the hardware interrupt
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

    //Configure the INT pin as input and fire canISR on every falling edge
    pinMode(sIntPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(sIntPin), canISR, FALLING);

    return 1;
}

//Return the current state of the interrupt flag
uint8_t canIntPending(void){
    return intFlag;
}

//Validate the frame ID and payload, then call the appropriate alert function
static void dispatch_frame(const struct can_frame *frame){
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


//Clear the interrupt flag, read pending frames from the MCP2515, and dispatch each one
void canProcess(void){

    if (!intFlag) {
        return;
    }
    
    //Clear before reading so a frame arriving mid-SPI sets the flag again rather than being lost
    intFlag = 0;

    struct can_frame frame;

    // Read the CANINTF register to determine which receive buffers hold new frames
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