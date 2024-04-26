/*
 ===========================================================================
 Name        : SDCard.h
 Author      : Brandon Van Pelt
 Created	 :
 Description : SDCard class manages the SD card reader hardware
 ===========================================================================
 */
#pragma once
#include "common.h"

#include "CANBusSend.h"


#define ROW_DIM 100

class SDCard
{
protected:
	//bool canDir = false;
	//typedef char MyArray[20][13];
	//char SDfilename[50];
public:
	//void setSDFilename(char*);
	//void writeFileS(char*, uint8_t);
	//void SDCard::saveSDCapture(char* filename, char* incoming, uint8_t size);
	//bool startSD();
	//void createDRIVE(char*);
	//void readFile(char* filename, uint8_t* arrayIn);
	//void writeFile(char*, int, int);
	//void writeFile(char*, String);
	//void writeFileln(char*);
	void writeSendMsg(CANBusTXScheduler, int);
	void readSendMsg(CANBusTXScheduler&, int);
	//void deleteFile(char*);
	//void readLogFile(char*);
	//uint32_t fileLength(char*);
	//void tempCopy(char* filename);
	//void split(char*, uint32_t);
	//uint32_t fileSize(char*);
	//uint8_t printDirectory(File dir, MyArray&);
	//void writeMACs(savedMACs);
	//void readMAC(savedMACs& msgStruct);
	//bool readLogFileLCD(char* filename, uint32_t&, bool);
};
