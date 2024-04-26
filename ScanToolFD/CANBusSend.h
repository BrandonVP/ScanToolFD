/*
 ===========================================================================
 Name        : CANBusSend.h
 Author      : Brandon Van Pelt
 Created	 : 11/15/2020 8:54:18 AM
 Description : CAN Bus send functions
 ===========================================================================
 */
#pragma once

#include "common.h"
//#include "SDCard.h"

#define MAX_STORED_MESSAGES 20

struct MessageRX
{
	char name[9] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
	uint8_t channel = 0;
	uint16_t interval = 0;
	uint32_t id = 0;
	uint32_t timer = 0;
	uint8_t data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bool isOn = false;
	bool isDel = true;
};

struct CANBusTXScheduler
{
	uint8_t nodeLength = 0;
	MessageRX node[MAX_STORED_MESSAGES];
};

#ifndef _CANBUS_SEND_h
#define _CANBUS_SEND_h

#include "cbBuffer.h"
#include "cbBufferFD.h"


#ifdef _CANBUS_SEND_c

//SDCard abc;
CANBusTXScheduler CANBusSendList;

uint8_t SEND_createSendBtns();
//void loadCANBusTXMsg(SDCard obj);
//void saveCANBusTXMsg(SDCard obj);

#else

//extern CANBusTXScheduler CANBusSendList;

uint8_t SEND_createSendBtns();
//void loadCANBusTXMsg(SDCard obj);
//void saveCANBusTXMsg(SDCard obj);

#endif // _CANBUS_SEND_c
#endif // _CANBUS_SEND_h

