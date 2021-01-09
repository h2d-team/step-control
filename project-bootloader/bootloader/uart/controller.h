/*
 * Controller.h
 *
 *  Created on: Aug 18, 2017
 *      Author: hongsan
 */

#ifndef UART_CONTROLLER_H_
#define UART_CONTROLLER_H_
#include "queue.h"
#include <stdint.h>

#define HEADER_INDICATOR	0xFE
#define FOOTER_INDICATOR	0xFD
#define MAX_PACKET_LENGTH	250

#define TX_BUF_SIZE 2048

#define UINT32_DECODE_VAL  (uint32_t) (((uint32_t)rxBuffer_[0] <<24) | ((uint32_t)rxBuffer_[1] << 16)| ((uint32_t)rxBuffer_[2] <<8) | (uint32_t)rxBuffer_[3])

namespace uart
{

class Controller
{
public:
    void init();
    void loop();
    bool sendMessage(uint16_t type, uint8_t length, const uint8_t* data);
    void printMessage(char* text);
    void printfMessage(char* format, ...);
    Queue& txQueue(){return txQueue_;}
    void interupt_update_rx();
    bool queueNotEmpty(){return txQueue_.notEmpty();}

private:
    void initUART_();
    bool rxReady_();
    bool txReady_();
    uint8_t read_();
    void write_(uint8_t c);

    /*rx*/
    enum RxState{HEADER = 0, LENGTH, TYPE1, TYPE2, ID1, ID2, DATA, CrC, FOOTER};
    uint8_t rxBuffer_[256];
    uint8_t rxLength_, checkSum_;
    uint8_t rxState_;
    uint8_t rxIndex_;
    uint16_t rxID_;
    uint16_t rxType_;
    void processCommand_();

    /*tx*/
    Queue txQueue_;
    uint8_t txBuf_[TX_BUF_SIZE];
};

extern uart::Controller CONTROLLER;

}

#define UART_COMMAND_BEGIN              void uart::Controller::processCommand_(){switch (rxType_){
#define COMMAND_REG(type, command)      case type: uart_cmd_##command(rxLength_, rxBuffer_);break;
#define UART_COMMAND_END                default:break;}}
#define UART_COMMAND(command)              void uart_cmd_##command(uint8_t length, uint8_t* data)

#endif /* UART_CONTROLLER_H_ */
