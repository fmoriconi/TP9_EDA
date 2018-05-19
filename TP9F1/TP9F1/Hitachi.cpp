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

FT_STATUS Hitachi::sendNybble(RS registerselect, UCHAR data)
{
	return FT_STATUS();
}

FT_STATUS Hitachi::sendByte(RS, UCHAR data)
{
	return FT_STATUS();
}

bool Hitachi::lcdClear() {


	bool success = false;

	sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY);

	if (this->status == FT_OK)
		success = true;

	return success;

}