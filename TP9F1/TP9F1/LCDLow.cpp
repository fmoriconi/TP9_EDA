#include "LCDLow.h"
bool initDisplay(FT_HANDLE * handle_, const char* lcd_description) {

	Timer maxconnectiontime;
	FT_STATUS status = FT_OTHER_ERROR;

	while (status != FT_OK && maxconnectiontime.getTime() < LCD_MAX_CONNECTION_TIME) {

		if (FT_OpenEx((PVOID)lcd_description, FT_OPEN_BY_DESCRIPTION, (handle_)) == FT_OK) {

			if (FT_SetBitMode(*handle_, LCD_BYTE, 1) == FT_OK) {

				sendNybbleLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_FUNCTION_SET_8_BIT >> 4, handle_);
				waitLow(10);
				sendNybbleLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_FUNCTION_SET_8_BIT >> 4, handle_);
				waitLow(10);
				sendNybbleLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_FUNCTION_SET_8_BIT >> 4, handle_);
				sendNybbleLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_FUNCTION_SET_4_BIT >> 4, handle_);

				sendByteLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_FUNCTION_SET_4_BIT | LCD_D3, handle_);
				sendByteLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_DISPLAY_CONTROL_OFF, handle_);
				sendByteLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY, handle_);
				sendByteLow(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_ENTRY_MODE_SET, handle_);

				status = FT_OK;
			}
		}
		maxconnectiontime.stop();
	}
	return (status == FT_OK);
}

FT_STATUS sendNybbleLow(RS registerselect, UCHAR data, FT_HANDLE* handle) {

	UCHAR buffer;
	DWORD bw;
	FT_STATUS status;
	Timer auxtimer;

	buffer = data << 4;
	buffer = buffer & LCD_HIGH_NIBBLE;																											//Limpio el low nibble y apago enable y register select

	if (registerselect == RS::DATA_REGISTER) {																									//Si me piden data register prendo register select
		buffer = buffer | LCD_FUNCTION_RS_DATA_REGISTER;
	}

	status = FT_Write(*handle, &buffer, sizeof(buffer), &bw);																					//Escribo a LCD
	waitLow(LCD_WAIT_TIME);																														//Espero
	buffer = buffer | LCD_FUNCTION_ENABLE_ON;																									//Prendo enable
	status = FT_Write(*handle, &buffer, sizeof(buffer), &bw);																					//Escribo a LCD
	waitLow(LCD_WAIT_TIME); 																														//Espero
	buffer = buffer & (~LCD_FUNCTION_ENABLE_ON);																								//Apago enable
	status = FT_Write(*handle, &buffer, sizeof(buffer), &bw);																					//Escribo a LCD
	waitLow(LCD_WAIT_TIME);
	//Espero
	return status;
}

FT_STATUS sendByteLow(RS registerselect, UCHAR data, FT_HANDLE* handle) {
	UCHAR buffer = data;


	FT_STATUS status;
	buffer = data >> 4;														//Mando high nybble de data a su low nybble
	buffer = buffer & LCD_LOW_NIBBLE;										//Limpio el high nybble del buffer
	status = sendNybbleLow(registerselect, buffer, handle);								//Mando la parte superior de data como nybble
	buffer = data & LCD_LOW_NIBBLE;											//Limpio el high nybble de data
	status = sendNybbleLow(registerselect, buffer, handle);								//Mando la parte inferior de data como nybble
	return status;
}

void waitLow(unsigned ms) {

	Timer auxtimer;
	auxtimer.start();

	do {
		auxtimer.stop();
	} while (auxtimer.getTime() < LCD_WAIT_TIME);
}