/*
 * Controller.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: hongsan
 */

#include "controller.h"


uart::Controller uart::CONTROLLER;

void uart::Controller::loop()
{
    if (txQueue_.notEmpty())
    {
        if (txReady_()) write_(txQueue_.pop());
    }

}

void uart::Controller::init()
{
    initUART_();
    txQueue_.init(txBuf_,TX_BUF_SIZE);
    rxState_ = HEADER;
}

bool uart::Controller::sendMessage(uint16_t type, uint8_t length, const uint8_t* data)
{
    uint8_t checkSum = 0u;
    if (txQueue_.available() < length + 11) return false;
    txQueue_.push(HEADER_INDICATOR);
    checkSum += (uint8_t) HEADER_INDICATOR;
    txQueue_.push(length);
    checkSum += length;
    txQueue_.push((type >> 8) & 0xFF);
    checkSum += (uint8_t) ((type >> 8) & 0xFF);
    txQueue_.push(type & 0xFF);
    checkSum += (uint8_t) (type & 0xFF);

    for (int i =0;i < length;i++)
    {
        txQueue_.push(data[i]);
        checkSum += data[i];
    }
    txQueue_.push(checkSum);
    txQueue_.push(FOOTER_INDICATOR);


    return true;
}

#include <string.h>
void uart::Controller::printMessage(char* text)
{
    int l = strlen(text) + 1;
    this->sendMessage(1, l, (uint8_t*)text);
}

#include <stdio.h>
#include <stdarg.h>

void uart::Controller::printfMessage(char* format,...)
{
    va_list args;
    va_start (args, format);
    char buf[256];

    auto l = vsprintf(buf, format ,args) + 1;
    va_end(args);
    this->sendMessage(1, l, (uint8_t*)buf);
}

void uart::Controller::processCommand_()
{

}
