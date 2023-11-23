/*
 ===========================================================================
 Name        : buffer.cpp
 Author      : Brandon Van Pelt
 Created	 : 9/3/2021
 Description : Ring buffer for CAN Bus messages
 ===========================================================================
 */

#include "cbBufferFD.h"

#define CAN_BUFFER_FD_C_

 // Copies provided structure into the buffer
void cbBufferFD::push_cb(long unsigned int id, uint8_t length, const uint8_t* data)
{
	// Copy message
	rxBuffer[bufferInPtr].id = id;
	rxBuffer[bufferInPtr].length = length;
	memcpy((void*)rxBuffer[bufferInPtr].data, (const void*)data, 8);

	// Increment bufferInPtr
	bufferInPtr = ((bufferInPtr + 1) & BUFFER_SIZE);

	// Overflow case
	if (bufferInPtr == bufferOutPtr)
	{
		// Disable interrupts
		cli();
		bufferOutPtr = ((bufferOutPtr + 1) & BUFFER_SIZE);
		sei();
	}
}

// Assign next structure in buffer to provided structure
bool cbBufferFD::pop_cb(struct CAN_Frame_FD* bufOut)
{
	if (stack_size_cb())
	{
		// Copy message
		bufOut->id = rxBuffer[bufferOutPtr].id;
		bufOut->length = rxBuffer[bufferOutPtr].length;
		memcpy((void*)bufOut->data, (const void*)rxBuffer[bufferOutPtr].data, 8);
		bufferOutPtr = ((bufferOutPtr + 1) & BUFFER_SIZE);
		return true;
	}
	return false;
}

// Calculates current structures in buffer by subtracting points then anding with max buffer size value
uint8_t cbBufferFD::stack_size_cb(void)
{
	// Disable interrupts
	cli();
	uint8_t size = (bufferInPtr - bufferOutPtr) & BUFFER_SIZE;
	sei();
	return size;
}

// Check the next structure int he buffer without incrementing
void cbBufferFD::peek_cb(struct CAN_Frame_FD* peek)
{
	peek->id = rxBuffer[bufferOutPtr].id;
	memcpy((void*)peek->data, (const void*)rxBuffer[bufferOutPtr].data, 8);
}

// Reset both points back to zero
void cbBufferFD::clear_buffer_cb()
{
	bufferOutPtr = 0;

	// Disable interrupts
	cli();
	bufferInPtr = 0;
	sei();
}