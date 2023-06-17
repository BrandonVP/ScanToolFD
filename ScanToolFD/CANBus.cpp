/*
 ===========================================================================
 Name        : CANBus.cpp
 Author      : Brandon Van Pelt
 Created	 :
 Description : CANBus manages the CAN bus hardware
 ===========================================================================
 */

#include "CANBus.h"
#include <FlexCAN_T4.h>
#include "PIDS.h"
#include "config.h"

/*
 //
void CAN1_Callback(const CAN_message_t& msg)
{

}

//
void CAN2_Callback(const CAN_message_t& msg)
{

}

//
void CAN3_FD_Callback(const CANFD_message_t& msg)
{

}

 // Initialize can1 and set the baud rates here
void CANBus::initialize_CAN1()
{
	

}

// Initialize can2 and set the baud rates here
void CANBus::initialize_CAN2()
{



}

// Initialize can3 FD and set the baud rates here
void CANBus::initialize_CAN3_FD()
{



}

// Set can1 Filter and Mask
void CANBus::setFilterMaskCAN1(uint32_t filter, uint32_t mask)
{


}

// Set can2 Filter and Mask
void CANBus::setFilterMaskCAN2(uint32_t filter, uint32_t mask)
{


}

// Set can3 FD Filter and Mask
void CANBus::setFilterMaskCAN3_FD(uint32_t filter, uint32_t mask)
{


}

// Set can1 Filter and Mask
void CANBus::setBaudCAN1(uint32_t baud)
{


}

// Set can2 Filter and Mask
void CANBus::setBaudCAN2(uint32_t baud)
{


}

// Set can3 FD Filter and Mask
void CANBus::setBaudCAN3_FD(uint32_t baud)
{


}

// Set can1 Filter and Mask
uint32_t CANBus::getBaudCAN1()
{


}

// Set can2 Filter and Mask
uint32_t CANBus::getBaudCAN2()
{


}

// Set can3 FD Filter and Mask
uint32_t CANBus::getBaudCAN3_FD()
{


}

// Set can1 Filter and Mask
void CANBus::EnableCAN1()
{


}

// Set can2 Filter and Mask
void CANBus::EnableCAN2()
{


}

// Set can3 FD Filter and Mask
void CANBus::EnableCAN3_FD()
{


}

// Set can1 Filter and Mask
void CANBus::DisableCAN1()
{


}

// Set can2 Filter and Mask
void CANBus::DisableCAN2()
{


}

// Set can3 FD Filter and Mask
void CANBus::DisableCAN3_FD()
{


}
*/




//
void CANBus::sendCANOut(uint8_t channel, CAN_message_t CANBus, bool serialOut)
{
	if (channel == 0)
	{
		can1.write(CANBus);
	}
	if (channel == 1)
	{
		can2.write(CANBus);
	}
	/*
	if (channel == 2)
	{
		// TODO: define some stuff / organize
		Serial3.write(0xFE);
		Serial3.write(0x0A);
		Serial3.write((CANBus.id >> 0) & 0xFF);
		Serial3.write((CANBus.id >> 8) & 0xFF);
		Serial3.write(CANBus.length);
		for (uint8_t i = 0; i < 8; i++)
		{
			Serial3.write(CANBus.data.bytes[i]);
		}
		Serial3.write(0xFD);
	}
	if (serialOut)
	{
		char buffer[MSG_STRING_LENGTH];
		sprintf(buffer, "%8d    %9f    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, (float)millis(), CANBus.id, CANBus.length, CANBus.data.bytes[0], CANBus.data.bytes[1], CANBus.data.bytes[2], CANBus.data.bytes[3], CANBus.data.bytes[4], CANBus.data.bytes[5], CANBus.data.bytes[6], CANBus.data.bytes[7]);
		SERIAL_CAPTURE(buffer);
	}
	*/
}

// Send out CAN Bus message
void CANBus::sendFrame(uint32_t id, byte* frame, uint8_t frameLength = 8, bool serialOut = false)
{
	// Outgoing message ID
	CANOut.id = id;

	// Do not use extended frames
	CANOut.flags.extended = false;

	// Message length
	CANOut.len = frameLength;

	// Assign object to message array
	memcpy((void*)CANOut.buf, (const void*)frame, frameLength);

	can1.write(CANOut);

	if (serialOut)
	{
		char buffer[MSG_STRING_LENGTH];
		can1.read(incCAN1);
		//sprintf(buffer, "%8d    %9f    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, (float)millis(), CANOut.id, CANOut.length, CANOut.data.bytes[0], CANOut.data.bytes[1], CANOut.data.bytes[2], CANOut.data.bytes[3], CANOut.data.bytes[4], CANOut.data.bytes[5], CANOut.data.bytes[6], CANOut.data.bytes[7]);
		SERIAL_CAPTURE(buffer);
	}
}

// Find supported vehicle PIDS
uint8_t CANBus::getPIDList(uint8_t state, uint8_t range, uint8_t bank)
{
	
	// Message used to requrest range of PIDS
	//byte frame[8] = { 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//char buffer[MSG_STRING_LENGTH];
	//frame[2] = range;
	uint8_t endState = state;
	/*
	switch (state)
	{
	case 1:
		sendFrame(PID_LIST_TX, frame);
		endState = 2;
		return endState;
		break;
	case 2:
		if (can1.get_rx_buff(inccan1))// && inccan1.id == ECU_RX)
		{
			if (inccan1.id == ECU_RX)
			{
				// Log PID message
				sprintf(buffer, "%8d    %9f    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, (float)millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);

				sdCard.writeFile(fullDir, buffer);
				// Write the values from the bank if the Corresponding bit matches
				uint16_t pos = 0;
				for (uint8_t i = 3; i < 7; i++)
				{
					for (int8_t j = 7; j >= 0; j--)
					{
						if (CHECK_BIT(inccan1.data.bytes[i], j))
						{
							sdCard.writeFile(PIDDir, "0x");
							sdCard.writeFile(PIDDir, PID_bank[bank][pos], HEX);
							sdCard.writeFileln(PIDDir);
						}
						pos++;
					}
				}
				(CHECK_BIT(inccan1.data.byte[6], 0)) ? endState = 3 : endState = 4;
			}
			else
			{
				SerialUSB.println(inccan1.id, HEX);
			}

		}
		break;
	}
	*/
	return endState;
}

// Get VIN, last argument gives option to save in SD Card
uint8_t CANBus::requestVIN(uint16_t state, bool saveSD)
{
	/*
	uint8_t ReadVIN1st[8] = { 0x02, 0x09, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 };
	uint8_t ReadVIN_2nd[8] = { 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	switch (state)
	{
	case 0:
		sendFrame(0x7E0, ReadVIN1st);
		return ++state;
		break;
	case 1:
		if (can1.read(incCAN1))
		{
			if (incCAN1.buf[0] == 0x10)
			{
				VIN[0] = incCAN1.buf[5];
				VIN[1] = incCAN1.buf[6];
				VIN[2] = incCAN1.buf[7];

				// Send second frame to continue VIN request

				sendFrame(0x7E0, ReadVIN_2nd);
				return ++state;
			}
		}
		return state;
		break;
	case 2:
		if (can1.read(incCAN1))
		{
			if (incCAN1.buf[0] == 0x21)
			{
				VIN[3] = incCAN1.buf[1];
				VIN[4] = incCAN1.buf[2];
				VIN[5] = incCAN1.buf[3];
				VIN[6] = incCAN1.buf[4];
				VIN[7] = incCAN1.buf[5];
				VIN[8] = incCAN1.buf[6];
				VIN[9] = incCAN1.buf[7];
				return ++state;
			}
		}
		return state;
		break;
	case 3:
		if (can1.read(incCAN1))
		{
			if (incCAN1.buf[0] == 0x22)
			{
				VIN[10] = incCAN1.buf[1];
				VIN[11] = incCAN1.buf[2];
				VIN[12] = incCAN1.buf[3];
				VIN[13] = incCAN1.buf[4];
				VIN[14] = incCAN1.buf[5];
				VIN[15] = incCAN1.buf[6];
				VIN[16] = incCAN1.buf[7];
				vehicleVIN = String(VIN);
				hasVIN = true;
				return ++state;
			}
		}
		return state;
		break;
	case 4:
		if (saveSD)
		{
			char VINLOG[7] = { 'L', 'O', 'G', '.', 't', 'x', 't' };
			char PID[7] = { 'P', 'I', 'D', '.', 't', 'x', 't' };

			// Create directory paths
			uint8_t j = 0;
			for (uint8_t i = 9; i < 17; i++)
			{
				fullDir[j] = VIN[i];
				PIDDir[j] = VIN[i];
				j++;
			}
			fullDir[8] = '/';
			PIDDir[8] = '/';
			SerialUSB.println(fullDir);
			SerialUSB.println(PIDDir);
			sdCard.createDRIVE(fullDir);

			j = 0;
			for (uint8_t i = 9; i < 16; i++)
			{
				PIDDir[i] = PID[j];
				j++;
			}
			SerialUSB.println(PIDDir);
			j = 0;
			for (uint8_t i = 9; i < 16; i++)
			{
				fullDir[i] = VINLOG[j];
				j++;
			}
			SerialUSB.println(fullDir);

			// Write VIN to log
			sdCard.writeFile(fullDir, "VIN: ");
			sdCard.writeFile(fullDir, VIN);
			sdCard.writeFileln(fullDir);
			sdCard.deleteFile(PIDDir);
			return ++state;
		}
		return state;
		break;
	}
	*/
	return state;
}

//
uint8_t CANBus::PIDStream(uint8_t& value, bool saveToSD)
{
	// Frames to request VIN
	//uint8_t PIDRequest[8] = { 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	// Waits to recieve all messages
	//bool isWait = true;
	//uint8_t PID = 0;
	uint8_t result = 0;

	/*
	// Send first frame requesting VIN
	if (can1.read(incCAN1))
	{
		PID = incCAN1.bufs[2];
		switch (PID)
		{
		case PID_ENGINE_RPM:
			if (inccan1.data.bytes[2] == PID_ENGINE_RPM) {
				uint16_t rpm;
				rpm = ((256 * inccan1.data.bytes[3]) + inccan1.data.bytes[4]) / 4; //formula 256*A+B/4
				//Serial.print(F("Engine RPM: "));
				//Serial.println(rpm, DEC);
				if (saveToSD)
				{
					sdCard.writeFile(fullDir, "Engine RPM: ");
					sdCard.writeFile(fullDir, rpm, DEC);
					sdCard.writeFileln(fullDir);
				}
				value = rpm;
				result = PID;
			}
			break;
		case PID_FUEL_LEVEL:
			if (inccan1.data.bytes[2] == PID_FUEL_LEVEL) {
				uint16_t level;
				level = ((100 * inccan1.data.bytes[1])) / 255; //formula 100*A/255
				//Serial.print(F("Fuel Level (%): "));
				//Serial.println(lev, DEC);
				if (saveToSD)
				{
					sdCard.writeFile(fullDir, "Fuel Level (%): ");
					sdCard.writeFile(fullDir, level, DEC);
					sdCard.writeFileln(fullDir);
				}
				value = level;
				result = PID;
			}
			break;
		case PID_THROTTLE_POSITION:
			if (inccan1.data.bytes[2] == PID_THROTTLE_POSITION) {
				uint16_t position;
				position = ((100 * inccan1.data.bytes[3])) / 255; //formula 100*A/255
				//Serial.print(F("Throttle (%): "));
				//Serial.println(pos, DEC);
				if (saveToSD)
				{
					sdCard.writeFile(fullDir, "Throttle (%): ");
					sdCard.writeFile(fullDir, position, DEC);
					sdCard.writeFileln(fullDir);
				}
				value = position;
				result = PID;
			}
			break;
		case PID_VEHICLE_SPEED:
			if (inccan1.data.bytes[2] == PID_VEHICLE_SPEED) {
				uint16_t vehicleSpeed;
				vehicleSpeed = ((inccan1.data.bytes[3])) / 1.609344; //formula 100*A/255
				//SerialUSB.print(F("MPH: "));
				//SerialUSB.println(vehicleSpeed, DEC);
				if (saveToSD)
				{
					sdCard.writeFile(fullDir, "MPH: ");
					sdCard.writeFile(fullDir, vehicleSpeed, DEC);
					sdCard.writeFileln(fullDir);
				}
				value = vehicleSpeed;
				result = PID;
			}
			break;
		case PID_MASS_AIR_FLOW:
			if (inccan1.data.bytes[2] == PID_MASS_AIR_FLOW) {
				uint16_t airFlow;
				airFlow = ((256 * inccan1.data.bytes[3]) + inccan1.data.bytes[4]) / 100; //formula 100*A/255
				//SerialUSB.print(F("MAF: (gram/s) "));
				//SerialUSB.println(flow, DEC);
				if (saveToSD)
				{
					sdCard.writeFile(fullDir, "MAF: ");
					sdCard.writeFile(fullDir, airFlow, DEC);
					sdCard.writeFileln(fullDir);
				}
				value = airFlow;
				result = PID;
			}
			break;
		case PID_ENGINE_LOAD:
			if (inccan1.data.bytes[2] == PID_ENGINE_LOAD)
			{
				uint16_t engineLoad = (inccan1.data.bytes[3]) / 2.55; //formula A/2.55
				value = engineLoad;
				result = PID;
			}
			break;
		case PID_COOLANT_TEMP:
			if (inccan1.data.bytes[2] == PID_COOLANT_TEMP)
			{
				uint16_t coolantTemp = (((inccan1.data.bytes[3]) - 40) * 1.8) + 32; //formula A - 40 for C, C * 1.8 + 32 = F
				value = coolantTemp;
				result = PID;
			}
			break;
		default: result = 1;
		}

		if (saveToSD)
		{
			sdCard.writeFile(fullDir, "ID: 0x");
			sdCard.writeFile(fullDir, inccan1.id, HEX);
			sdCard.writeFile(fullDir, " Len: ");
			sdCard.writeFile(fullDir, inccan1.length, HEX);
			sdCard.writeFile(fullDir, " Data: ");

			for (int count = 0; count < inccan1.length; count++)
			{
				sdCard.writeFile(fullDir, inccan1.data.bytes[count], HEX);
				sdCard.writeFile(fullDir, " ");
			}
			sdCard.writeFileln(fullDir);
		}
	}
	*/
	return result;
}

//
/*
int CANBus::PIDStreamGauge(uint16_t sendID, uint8_t PID)
{
	
	// Frames to request VIN
	uint8_t PIDRequest[8] = { 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	PIDRequest[2] = PID;

	bool isWait = true;
	unsigned long timer = millis();

	// Send first frame requesting VIN
	for (int i = 0; i < 5; i++)
	{
		sendFrame(sendID, PIDRequest);

		while (millis() - timer < 100)
		{
			if (can1.read(incCAN1)) 
			{
				switch (PID)
				{
				case PID_ENGINE_RPM:
					if (inccan1.data.bytes[2] == PID_ENGINE_RPM) {
						uint16_t rpm;
						rpm = ((256 * inccan1.data.bytes[3]) + inccan1.data.bytes[4]) / 4; //formula 256*A+B/4
						return rpm;
					}
				case PID_FUEL_LEVEL:
					if (inccan1.data.bytes[2] == PID_FUEL_LEVEL) {
						uint16_t level = ((100 * inccan1.data.bytes[1])) / 255; //formula 100*A/255
						return level;
					}
					break;
				case PID_THROTTLE_POSITION:
					if (inccan1.data.bytes[2] == PID_THROTTLE_POSITION) {
						uint16_t pos = ((100 * inccan1.data.bytes[3])) / 255; //formula 100*A/255
						return pos;
					}
				case PID_VEHICLE_SPEED:
					if (inccan1.data.bytes[2] == PID_VEHICLE_SPEED) {
						uint16_t speed = ((inccan1.data.bytes[3])) / 1.609344; //formula 100*A/255
						return speed;
					}
					break;
				case PID_MASS_AIR_FLOW:
					if (inccan1.data.bytes[2] == PID_MASS_AIR_FLOW) {
						uint16_t airFlow = ((256 * inccan1.data.bytes[3]) + inccan1.data.bytes[4]) / 100; //formula 100*A/255
						return airFlow;
					}
					break;
				case PID_ENGINE_LOAD:
					if (inccan1.data.bytes[2] == PID_ENGINE_LOAD) {
						uint16_t engineLoad = (inccan1.data.bytes[3]) / 2.55; //formula A/2.55
						return engineLoad;
					}
					break;
				case PID_COOLANT_TEMP:
					if (inccan1.data.bytes[2] == PID_COOLANT_TEMP) {
						uint16_t coolantTemp = (((inccan1.data.bytes[3]) - 40) * 1.8) + 32; //formula A - 40 for C, C * 1.8 + 32 = F
						return coolantTemp;
					}
					break;
				}
			}
		}
	}
	
	return -1;
}*/

// Used to receive CAN Bus messages in the user requested configuration
bool CANBus::LCDOutCAN(uint8_t * msg, uint8_t& len, uint32_t& id, uint8_t config)
{
	/*
	if (config == can1 && can1.get_rx_buff(inccan1))
	{
		static uint32_t testTimer1 = 0;
		//static uint32_t testTimer2 = 0;

		//uint32_t v = millis() - testTimer2;

		if (millis() - testTimer1 > 2000)
		{
			SerialUSB.println(can1.available());
			//SerialUSB.println(millis() - testTimer2);
			testTimer1 = millis();
		}
		//testTimer2 = millis();

		id = inccan1.id;
		len = inccan1.length;
		memcpy((void*)msg, (const void*)inccan1.data.bytes, inccan1.length);
		return true;
	}
	else if (config == can2 && can2.get_rx_buff(inccan2))
	{
		id = inccan2.id;
		len = inccan2.length;
		memcpy((void*)msg, (const void*)inccan2.data.bytes, inccan2.length);
		return true;
	}
	else if (config == BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			id = inccan1.id;
			len = inccan1.length;
			memcpy((void*)msg, (const void*)inccan1.data.bytes, inccan1.length);
			return true;
		}
		if (can2.get_rx_buff(inccan2))
		{
			id = inccan2.id;
			len = inccan2.length;
			memcpy((void*)msg, (const void*)inccan2.data.bytes, inccan2.length);
			return true;
		}
	}
	else if (config == BRIDGE_can2_RX)
	{
		if (can1.get_rx_buff(inccan1))
		{
			can2.sendFrame(inccan1);
		}
		if (can2.get_rx_buff(inccan2))
		{
			id = inccan2.id;
			len = inccan2.length;
			memcpy((void*)msg, (const void*)inccan2.data.bytes, inccan2.length);
			can1.sendFrame(inccan2);
			return true;
		}
	}
	else if (config == BRIDGE_BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			can2.sendFrame(inccan1);
			id = inccan1.id;
			len = inccan1.length;
			memcpy((void*)msg, (const void*)inccan1.data.bytes, inccan1.length);
			return true;
		}
		if (can2.get_rx_buff(inccan2))
		{
			can1.sendFrame(inccan2);
			id = inccan2.id;
			len = inccan2.length;
			memcpy((void*)msg, (const void*)inccan2.data.bytes, inccan2.length);
			return true;
		}
	}
	else if (config == WIFI)
	{
		if (Serial3.available() > 0)
		{
			uint8_t recByte = Serial3.read();
			switch (state)
			{
			case START_BYTE:
				if (recByte == STARTING_BYTE)
				{
					state = PACKET_LENGTH;
					return false;
				}
				break;
			case PACKET_LENGTH:
				state = CAN_BUS_ID1;
				if (recByte == PACKET_SIZE)
				{
					packetIndex = 0;
					return false;
				}
				else
				{
					// Bad packet
					state = START_BYTE;
				}
				break;
			case CAN_BUS_ID1:
				incWIFI.id = recByte;
				state = CAN_BUS_ID2;
				break;
			case CAN_BUS_ID2:
				incWIFI.id += (recByte << 8);
				state = CAN_BUS_LENGTH;
				break;
			case CAN_BUS_LENGTH:
				incWIFI.length = recByte;
				state = CAN_BUS_DATA;
				break;
			case CAN_BUS_DATA:
				incWIFI.data.bytes[packetIndex] = recByte;
				packetIndex++;
				if (packetIndex == PACKET_SIZE - 2)
				{
					state = END_BYTE;
				}
				break;
			case END_BYTE:
				if (recByte == ENDING_BYTE)
				{
					// Successful packet
					state = START_BYTE;
					id = incWIFI.id;
					len = incWIFI.length;
					memcpy((void*)msg, (const void*)incWIFI.data.bytes, incWIFI.length);


					// Filter and Mask the message
					uint32_t filterValue = CANWiFiFilter & CANWiFiMask;
					if (filterValue == (id & CANWiFiMask))
					{
						return true;
					}
					else
					{
						return false;
					}

				}
				else
				{
					// packet failed restart
					state = START_BYTE;
				}
				break;
			}
		}
	}
	*/
	return false;
}

// Displays CAN traffic on Serial out
bool CANBus::SerialOutCAN(uint8_t config)
{
	/*
	char buffer[MSG_STRING_LENGTH];

	// Display can1
	if (config == can1 && can1.get_rx_buff(inccan1))
	{
		sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
		SERIAL_CAPTURE(buffer);
	}
	// Display can2
	else if ((config == can2) && (can2.get_rx_buff(inccan2)))
	{
		sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
		SERIAL_CAPTURE(buffer);
	}
	// Display can1 & can2
	else if (config == BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
			SERIAL_CAPTURE(buffer);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SERIAL_CAPTURE(buffer);
		}
	}
	// Forward traffic between can1-can2 and display can2
	else if (config == BRIDGE_can2_RX)
	{
		if (can1.get_rx_buff(inccan1))
		{
			can2.sendFrame(inccan1);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SERIAL_CAPTURE(buffer);
			can1.sendFrame(inccan2);
		}
	}
	// Forward traffic between can1-can2 and display can1 & can2
	else if (config == BRIDGE_BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
			SERIAL_CAPTURE(buffer);
			can2.sendFrame(inccan1);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SERIAL_CAPTURE(buffer);
			can1.sendFrame(inccan2);
		}
	}
	else if (config == WIFI)
	{
		if (Serial3.available() > 0)
		{
			uint8_t recByte = Serial3.read();
			switch (state)
			{
			case START_BYTE:
				if (recByte == STARTING_BYTE)
				{
					state = PACKET_LENGTH;
					return false;
				}
				break;
			case PACKET_LENGTH:
				state = CAN_BUS_ID1;
				if (recByte == PACKET_SIZE)
				{
					packetIndex = 0;
					return false;
				}
				else
				{
					// Bad packet
					state = START_BYTE;
				}
				break;
			case CAN_BUS_ID1:
				incWIFI.id = recByte;
				state = CAN_BUS_ID2;
				break;
			case CAN_BUS_ID2:
				incWIFI.id += (recByte << 8);
				state = CAN_BUS_LENGTH;
				break;
			case CAN_BUS_LENGTH:
				incWIFI.length = recByte;
				state = CAN_BUS_DATA;
				break;
			case CAN_BUS_DATA:
				incWIFI.data.bytes[packetIndex] = recByte;
				packetIndex++;
				if (packetIndex == PACKET_SIZE - 2)
				{
					state = END_BYTE;
				}
				break;
			case END_BYTE:
				if (recByte == ENDING_BYTE)
				{
					// Successful packet
					state = START_BYTE;
					char buffer[MSG_STRING_LENGTH];
					sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\r\n", ++messageNum, millis(), incWIFI.id, incWIFI.length, incWIFI.data.bytes[0], incWIFI.data.bytes[1], incWIFI.data.bytes[2], incWIFI.data.bytes[3], incWIFI.data.bytes[4], incWIFI.data.bytes[5], incWIFI.data.bytes[6], incWIFI.data.bytes[7]);
					SERIAL_CAPTURE(buffer);
					return true;
				}
				else
				{
					// packet failed restart
					state = START_BYTE;
				}
				break;
			}
		}
	}
	*/
	return true;
}

// Wirte large blocks to SD card
void CANBus::SDCardBuffer(char* message, bool endCapture)
{
	/*
	static char* buf = (char*)malloc((SD_CAPTURE_BLOCK_SIZE) * sizeof(char));
	static char* a1 = buf;
	static uint8_t count = 0;

	if (!endCapture)
	{
		memcpy(a1, message, MSG_STRING_LENGTH);
		a1 += MSG_STRING_LENGTH;
		count++;
	}

	if ((count == SD_CAPTURE_NUM_MSG) || endCapture)
	{
		sdCard.writeFileS(buf, count);
		a1 = buf;
		count = 0;

#ifdef DEBUG_SD_CAPTURE
		static uint8_t printBufSize = 0;
		printBufSize++;
		if (printBufSize == 12)
		{
			SerialUSB.println(can1.available());
			printBufSize = 0;
		}
#endif
	}
	*/
}

// Displays CAN traffic on Serial out
bool CANBus::SDOutCAN(uint8_t config)
{
	/*
	char buffer[67];
	// Display can1
	if (config == can1 && can1.get_rx_buff(inccan1))
	{
		sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
		SD_CAPTURE(buffer, false);
	}
	// Display can2
	else if (config == can2 && can2.get_rx_buff(inccan2))
	{
		sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
		SD_CAPTURE(buffer, false);
	}
	// Display can1 & can2
	else if (config == BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
			SD_CAPTURE(buffer, false);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SD_CAPTURE(buffer, false);
		}
	}
	// Forward traffic between can1-can2 and display can1
	else if (config == BRIDGE_can2_RX)
	{
		if (can1.get_rx_buff(inccan1))
		{
			can2.sendFrame(inccan1);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SD_CAPTURE(buffer, false);
			can1.sendFrame(inccan2);
		}
	}
	// Forward traffic between can1-can2 and display can1 & can2
	else if (config == BRIDGE_BOTH)
	{
		if (can1.get_rx_buff(inccan1))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan1.id, inccan1.length, inccan1.data.bytes[0], inccan1.data.bytes[1], inccan1.data.bytes[2], inccan1.data.bytes[3], inccan1.data.bytes[4], inccan1.data.bytes[5], inccan1.data.bytes[6], inccan1.data.bytes[7]);
			SD_CAPTURE(buffer, false);
			can2.sendFrame(inccan1);
		}
		if (can2.get_rx_buff(inccan2))
		{
			sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), inccan2.id, inccan2.length, inccan2.data.bytes[0], inccan2.data.bytes[1], inccan2.data.bytes[2], inccan2.data.bytes[3], inccan2.data.bytes[4], inccan2.data.bytes[5], inccan2.data.bytes[6], inccan2.data.bytes[7]);
			SD_CAPTURE(buffer, false);
			can1.sendFrame(inccan2);
		}
	}
	else if (config == WIFI)
	{
		if (Serial3.available() > 0)
		{
			uint8_t recByte = Serial3.read();
			switch (state)
			{
			case START_BYTE:
				if (recByte == STARTING_BYTE)
				{
					state = PACKET_LENGTH;
					return false;
				}
				break;
			case PACKET_LENGTH:
				state = CAN_BUS_ID1;
				if (recByte == PACKET_SIZE)
				{
					packetIndex = 0;
					return false;
				}
				else
				{
					// Bad packet
					state = START_BYTE;
				}
				break;
			case CAN_BUS_ID1:
				incWIFI.id = recByte;
				state = CAN_BUS_ID2;
				break;
			case CAN_BUS_ID2:
				incWIFI.id += (recByte << 8);
				state = CAN_BUS_LENGTH;
				break;
			case CAN_BUS_LENGTH:
				incWIFI.length = recByte;
				state = CAN_BUS_DATA;
				break;
			case CAN_BUS_DATA:
				incWIFI.data.bytes[packetIndex] = recByte;
				packetIndex++;
				if (packetIndex == PACKET_SIZE - 2)
				{
					state = END_BYTE;
				}
				break;
			case END_BYTE:
				if (recByte == ENDING_BYTE)
				{
					// Successful packet
					state = START_BYTE;
					sprintf(buffer, "%8d    %9d    %04X   %d   %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X\n", ++messageNum, millis(), incWIFI.id, incWIFI.length, incWIFI.data.bytes[0], incWIFI.data.bytes[1], incWIFI.data.bytes[2], incWIFI.data.bytes[3], incWIFI.data.bytes[4], incWIFI.data.bytes[5], incWIFI.data.bytes[6], incWIFI.data.bytes[7]);
					SD_CAPTURE(buffer, false);
					return true;
				}
				else
				{
					// packet failed restart
					state = START_BYTE;
				}
				break;
			}
		}
	}
	*/
	return true;
}