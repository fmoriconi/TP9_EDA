#include "Hitachi.h"



Hitachi::Hitachi()
{
	UCHAR buffer;
	Timer timer, maxconnectiontime;

	while (!FT_OK && maxconnectiontime.getTime() < LCD_MAX_CONNECTION_TIME) {
		if (FT_OpenEx((PVOID)MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, &(this->handle)) == FT_OK) {		//OPENEX
			if (FT_SetBitMode((handle), LCD_BYTE, 1) == FT_OK) {											//SETBITMODE
				buffer = LCD_FUNCTION_FUNCTION_SET_8_BIT;
				if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {							//WRITE 8BIT
					do { timer.stop(); } while (timer.getTime() < 4);										//WAIT 4MS
					if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {						//WRITE 8BIT
						do { timer.stop(); } while (timer.getTime() < 1);									//WAIT 100microS (ya fue, 1ms)
						if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {					//WRITE 8BIT
							buffer = LCD_FUNCTION_FUNCTION_SET_4_BIT;
							if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {				//WRITE 4BIT
								if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {			//WRITE 4BIT
									buffer = LCD_FUNCTION_DISPLAY_CONTROL_OFF;
									if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {		//DISP OFF
										buffer = LCD_FUNCTION_CLEAR_DISPLAY;
										if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {	//CLEAR DISP
											buffer = LCD_FUNCTION_ENTRY_MODE_SET;
											if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {	//ENTRY MODE SET
												this->initerror = false;
												this->status = FT_OK;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		maxconnectiontime.stop();
	}
	if (this->status != FT_OK) {
		std::cout << "Could not open Hitachi HD44780: " << MY_LCD_DESCRIPTION << std::endl;
	}
}

Hitachi::~Hitachi()
{
}

void Hitachi::sendNybble(RS registerselect, UCHAR data)
{
	UCHAR buffer;

	buffer = data << 4;
	buffer = buffer & LCD_HIGH_NIBBLE;										//Limpio el low nibble y apago enable y register select
	if (registerselect == RS::DATA_REGISTER) {								//Si me piden data register prendo register select
		buffer = buffer | LCD_FUNCTION_RS_DATA_REGISTER;
	}
	this->status = FT_Write(this->handle, &buffer, 2, &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero
	buffer = buffer | LCD_FUNCTION_ENABLE_ON;								//Prendo enable
	this->status = FT_Write(this->handle, &buffer, 2, &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero
	buffer = buffer ^ LCD_FUNCTION_ENABLE_ON;								//Apago enable
	this->status = FT_Write(this->handle, &buffer, 2, &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero

}

void Hitachi::sendByte(RS registerselect, UCHAR data)
{
	UCHAR buffer = data;
	this->sendNybble(registerselect, buffer);								//Mando la parte superior de data como nybble
	buffer = data >> 4;														//Mando low nybble de data a su high nybble
	buffer = buffer & LCD_HIGH_NIBBLE;										//Limpio el low nybble de data
	this->sendNybble(registerselect, buffer);								//Mando la parte superior de data como nybble

}

bool Hitachi::lcdClear() {


	bool success = false;

	sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY);

	if (this->status == FT_OK)
		success = true;

	return success;

}