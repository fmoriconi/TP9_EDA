#pragma once

#include "basicLCD.h"
#include "LCDLow.h"
#include <iostream>
#include <string>

class Hitachi{
public:

	//Constructor y destructor
	Hitachi(const char* lcd_description);
	~Hitachi();

	//Escritura
	void sendNybble(RS registerselect, UCHAR data);
	void sendByte(RS, UCHAR data);

	//--------Métodos públicos heredados--------//

	//Getters
	void wait(float ms) { auxtimer.start(); do { auxtimer.stop(); } while (auxtimer.getTime() < ms); }
	bool lcdInitOk() { return !initerror; }
	FT_STATUS lcdGetError() { return this->status; }

	//Screenclear
	bool lcdClear();
	bool lcdClearToEOL();

	//Operadores
	Hitachi& operator<<(const unsigned char c);
	Hitachi& operator<<(const unsigned char * c);

	//Movimiento del cursor
	bool lcdMoveCursorUp();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorLeft();
	bool lcdSetCursorPosition(const cursorPosition pos);

	cursorPosition lcdGetCursorPosition();

private:

	FT_HANDLE handle;
	FT_STATUS status;
	DWORD bytesWritten;
	Timer auxtimer;

	bool initerror = true;

	int cadd = 1; //Cursor Address.

	void lcdUpdateCursor();

};