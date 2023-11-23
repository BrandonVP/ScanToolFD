/*
 ===========================================================================
 Name        : buffer.h
 Author      : Brandon Van Pelt
 Created	 : 9/3/2021
 Description : Ring buffer for CAN Bus messages
 ===========================================================================
 */
#pragma once
#ifndef CAN_BUFFER_FD_H_
#define CAN_BUFFER_C_H_

#include "common.h"

#define BUFFER_SIZE 0x7F // Array sizes: F, 1F, 3F, 7F, (Must increase both Ptrs to 16 bit to go higher) FF, 1FF, 3FF, 7FF, FFF

struct CAN_Frame_FD
{
	uint32_t id;
    uint16_t length;
	uint8_t data[64] = { 0 };
};

class cbBufferFD
{
private:
	uint8_t bufferOutPtr = 0;
	uint8_t bufferInPtr = 0;
	struct CAN_Frame_FD rxBuffer[BUFFER_SIZE + 1];
public:
	void push_cb(long unsigned int, uint8_t length, const uint8_t*);
	bool pop_cb(struct CAN_Frame_FD*);
	void peek_cb(struct CAN_Frame_FD*);
	void clear_buffer_cb(void);
	uint8_t stack_size_cb(void);
};

#endif // CAN_BUFFER_C_H_