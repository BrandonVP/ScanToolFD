/*
 ===========================================================================
 Name        : buffer.h
 Author      : Brandon Van Pelt
 Created	 : 9/3/2021
 Description : Ring buffer for CAN Bus messages
 ===========================================================================
 */

#ifndef CAN_BUFFER_H_
#define CAN_BUFFER_H_

#include "common.h"

#define BUFFER_SIZE 0x7F // Array sizes: F, 1F, 3F, 7F, (Must increase both Ptrs to 16 bit to go higher) FF, 1FF, 3FF, 7FF, FFF

struct CAN_Frame
{
	uint32_t id;
    uint16_t length;
	uint8_t data[8] = { 0 };
};

class cbBuffer
{
private:
	uint8_t bufferOutPtr = 0;
	uint8_t bufferInPtr = 0;
	struct CAN_Frame rxBuffer[BUFFER_SIZE + 1];
public:
	void push_cb(long unsigned int, uint8_t length, const uint8_t*);
	bool pop_cb(struct CAN_Frame*);
	void peek_cb(struct CAN_Frame*);
	void clear_buffer_cb(void);
	uint8_t stack_size_cb(void);
};

#endif // BUFFER_H_