// 
// 
// 

#include "serialTransfer.h"


// Read in CAN Bus messages

// Send CAN Bus messages

// WiFi MAC

// WiFi reset

// Set dongle mac to connect

// Read back connected dongle mac

// Set Filter Mask

// Set baud rate

// Read saved macs from sd card

// save mac to sd card

/*

void serialConnect(uint8_t index)
{
	// TODO: Set all other node to off
	for (uint8_t i = 0; i < 10; i++)
	{
		dongle.node[i].isOn = false;
	}
	dongle.node[index].isOn = true;

	Serial3.write(0xCA);
	Serial3.write(0xCC);
	Serial3.write(dongle.node[index].data[0]);
	Serial3.write(dongle.node[index].data[1]);
	Serial3.write(dongle.node[index].data[2]);
	Serial3.write(dongle.node[index].data[3]);
	Serial3.write(dongle.node[index].data[4]);
	Serial3.write(dongle.node[index].data[5]);
	delay(100);
	// WiFi Reset
	Serial3.write(0xBA);
	Serial3.write(0xBF);
	if (Serial3.read() == 0xFF);
	delay(100);
	//REQUEST_EXTERNAL_RESET;
}

*/