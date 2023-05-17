// serialTransfer.h

#ifndef _SERIALTRANSFER_h
#define _SERIALTRANSFER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


// States for serial transfer
#define START_BYTE              (0)
#define PACKET_LENGTH           (1)
#define CAN_BUS_ID1             (2)
#define CAN_BUS_ID2             (3)
#define CAN_BUS_LENGTH          (4)
#define CAN_BUS_DATA            (5)
#define END_BYTE                (6)
// Serial transfer
#define STARTING_BYTE           (0xFE)
#define ENDING_BYTE             (0xFD)
#define PACKET_SIZE             (0x0A)

#endif

