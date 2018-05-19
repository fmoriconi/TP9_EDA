#pragma once

#include "ftd2xx.h"

struct cursorPosition
{
	int row; // Va de 0 a 1
	int column; //Va de 0 a 15
};

class basicLCD
{
public:

	basicLCD();
	virtual ~basicLCD();

	//Getters

	virtual bool lcdInitOk() = 0;
	virtual FT_STATUS lcdGetError() = 0;

	//Screenclear
	virtual bool lcdClear() = 0;
	virtual bool lcdClearToEOL() = 0;

	//Operadores
	virtual basicLCD& operator<<(const unsigned char c) = 0;
	virtual basicLCD& operator<<(const unsigned char * c) = 0;

	//Movimiento del cursor
	virtual bool lcdMoveCursorUp() = 0;
	virtual bool lcdMoveCursorDown() = 0;
	virtual bool lcdMoveCursorRight() = 0;
	virtual bool lcdMoveCursorLeft() = 0;

	virtual bool lcdSetCursorPosition(const cursorPosition pos) = 0;

	virtual cursorPosition lcdGetCursorPosition() = 0;

protected:

	int cadd; //Variable correspondiente a la posición del cursor pues no la podemos leer, pero si llevar la cuenta de su posición.

	virtual void lcdUpdateCursor() = 0; //Actualización del cursor.

};

