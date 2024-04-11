/*
 ===========================================================================
 Name        : CANBusSend.h
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus send functions
 ===========================================================================
 */

#ifndef _CANBUS_SEND_h
#define _CANBUS_SEND_h

#include "common.h"
#include "cbBuffer.h"
#include "cbBufferFD.h"

#ifdef _CANBUS_SEND_c

uint8_t SEND_createSendBtns();

#else

uint8_t SEND_createSendBtns();

#endif // _CANBUS_SEND_c
#endif // _CANBUS_SEND_h

