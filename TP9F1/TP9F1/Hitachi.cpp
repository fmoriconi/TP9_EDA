#include "Hitachi.h"

#define LCD_CLEAR_TIME 350

Hitachi::Hitachi(const char* lcd_description)
{
	std::string desc = lcd_description;
	if (initDisplay(&(this->handle), lcd_description)) {
		this->initerror = false;
	}
}

Hitachi::~Hitachi()
{
}

void Hitachi::sendNybble(RS registerselect, UCHAR data) //Send nibble envia la parte BAJA de data.
{
	this->status = sendNybbleLow(registerselect, data, &handle);
}

void Hitachi::sendByte(RS registerselect, UCHAR data)
{
	this->status = sendByteLow(registerselect, data, &handle);
}

bool Hitachi::lcdClear() {

	bool success = false;

	sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY); //Limpio el display

	if (this->status == FT_OK) {

		sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_RETURN_HOME); //Pongo el adress counter en 0.

		if (this->status == FT_OK) {
			success = true;
			this->cadd = 1;
		}
	}

	return success;

}

bool Hitachi::lcdClearToEOL() {

	bool success = false;

	int temp = this->cadd;

	while ((cadd % 17) != 0) {
		this->cadd++; //Hago esto porque sé que el cursor se movió por si mismo y entonces registro dicho cambio
		sendByte(RS::DATA_REGISTER, LCD_SPACE_CHAR); ///Ni idea cual char de la cartilla es el espacio, será lo que viene antes que el signo de exclamación?
		success = true;
	}

	this->cadd = temp;

	lcdUpdateCursor();

	return success;

}

bool Hitachi::lcdMoveCursorUp() {

	bool success = false;

	if (this->cadd > 16) {
		cadd -= 16;
		cursorPosition cp = { 0, cadd }; //Indico la nueva posición en una estructura cursorposition para pasarla como parámetro.
		lcdSetCursorPosition(cp);
		success = true;
	}

	return success;
}

bool Hitachi::lcdMoveCursorDown() {

	bool success = false;

	if (this->cadd <= 16) {
		cadd += 16;
		cursorPosition cp = { 1 , cadd }; //Indico la nueva posición en una estructura cursorposition para pasarla como parámetro.
		lcdSetCursorPosition(cp);
		success = true;
	}

	return success;

}

bool Hitachi::lcdMoveCursorRight() {

	bool success = false;

	if (cadd < 32) {
		this->cadd++;
		success = true;
		lcdUpdateCursor();
	}
	return success;
}

bool Hitachi::lcdMoveCursorLeft() {

	bool success = false;

	if (cadd > 1) {
		this->cadd--;
		success = true;
		lcdUpdateCursor();
	}

	return success;

}

bool Hitachi::lcdSetCursorPosition(const cursorPosition pos) {

	bool success = false;

	if ((pos.column < 16) && (pos.row < 2)) {
		this->cadd = 1 + pos.column + (16 * pos.row); //Me paro en la columna adecuada y luego, de ser necesario, bajo una fila.
		success = true;
		lcdUpdateCursor();
	}

	return success;
}

cursorPosition Hitachi::lcdGetCursorPosition() {

	cursorPosition cp = { 0,0 };

	if (cadd > 15) //Solo amerita cambiar la fila si cadd es mayor a 15, de lo contrario está bien dejarlo en 0.
		cp.row = 1;

	cp.column = (cadd % 16); //Me devuelve un valor de 0 a 15.

	return cp;

}

void Hitachi::lcdUpdateCursor() {

	if (this->cadd <= 16)
		sendByte(RS::INSTRUCTION_REGISTER, (LCD_FUNCTION_SET_DDRAM_ADDRESS + (this->cadd - 1))); //Recordemos que cadd es el cursor address + 1.
	else if(this->cadd > 16)
		sendByte(RS::INSTRUCTION_REGISTER, (LCD_FUNCTION_SET_DDRAM_ADDRESS | LCD_DISPLAY_LINE2) + (this->cadd - 17));

}

Hitachi& Hitachi::operator<<(const unsigned char * c) {

	std::string str = "";
	bool maxSize = false; //Bool para indicar si el string es mayor a 31 caracteres.

	for (int i = 0; *(c + i) != '\0'; i++)
		str += *(c+i); //Agrego cada caracter al string.

	while (str.size() > 32) {
		str.erase(0, 1);
		maxSize = true;
	} //Si el string tiene más de 32 caracteres, borro el primero hasta que queden solo 32.

	if (maxSize)
		lcdClear();

	for (unsigned int i = 0; i < str.size(); i++) {

		if (this->cadd == 17) { //Cuando paso el 16, debo bajar.
			sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_SET_DDRAM_ADDRESS | LCD_DISPLAY_LINE2);
		}

		sendByte(RS::DATA_REGISTER, str[i]);
		this->cadd++; //Se que el cursor se movió, por lo que lo registro en cadd


		if (this->cadd > 31) {
			sendByte(RS::DATA_REGISTER, 0xE7);
			Timer waitForNewLine;
			do {
				waitForNewLine.stop();
			} while (waitForNewLine.getTime() < LCD_CLEAR_TIME);
			this->lcdClear();
			this->cadd = 1;
			this->lcdUpdateCursor();
		}
	}

	return *this;
}

Hitachi& Hitachi::operator<<(const unsigned char c) {

	if (this->cadd == 17) { //Cuando paso al 16, debo bajar.
		sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_SET_DDRAM_ADDRESS | LCD_DISPLAY_LINE2);
	}

	sendByte(RS::DATA_REGISTER, c);
	this->cadd++; ///Nuevamente, falta trabajar los casos límites que, por lo menos a mi, me resultan un poco ambiguos en la consigna, e incluso aunque elija una interpretación ni idea de como solucionarlo.
	if (this->cadd > 31) {
		sendByte(RS::DATA_REGISTER, 0x7E);
		Timer waitForNewLine;
		do {
			waitForNewLine.stop();
		} while (waitForNewLine.getTime() < LCD_CLEAR_TIME);
		this->lcdClear();
		this->cadd = 1;
		this->lcdUpdateCursor();
	}

	return *this;
}
///Esto no estoy entendiendo bien que corno retornean.
