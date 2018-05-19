#include "Hitachi.h"



Hitachi::Hitachi()
{
	//UCHAR buffer;
	//Timer timer, maxconnectiontime;

	//while (!FT_OK && maxconnectiontime.getTime() < LCD_MAX_CONNECTION_TIME) {
	//	if (FT_OpenEx((PVOID)MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, &(this->handle)) == FT_OK) {		//OPENEX
	//		if (FT_SetBitMode((handle), LCD_BYTE, 1) == FT_OK) {											//SETBITMODE
	//			buffer = LCD_FUNCTION_FUNCTION_SET_8_BIT;

	//			if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {							//WRITE 8BIT
	//				do { timer.stop(); } while (timer.getTime() < 4);										//WAIT 4MS

	//				if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {						//WRITE 8BIT
	//					do { timer.stop(); } while (timer.getTime() < 1);									//WAIT 100microS (ya fue, 1ms)

	//					if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {					//WRITE 8BIT
	//						buffer = LCD_FUNCTION_FUNCTION_SET_4_BIT;

	//						if (FT_Write(this->handle, &buffer, 1, &bytesWritten) == FT_OK) {				//WRITE 4BIT

	//							if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {			//WRITE 4BIT
	//								buffer = LCD_FUNCTION_DISPLAY_CONTROL_OFF;

	//								if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {		//DISP OFF
	//									buffer = LCD_FUNCTION_CLEAR_DISPLAY;

	//									if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {	//CLEAR DISP
	//										buffer = LCD_FUNCTION_ENTRY_MODE_SET;

	//										if (FT_Write(this->handle, &buffer, 2, &bytesWritten) == FT_OK) {	//ENTRY MODE SET
	//											this->initerror = false;
	//											this->status = FT_OK;
	//										}
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	maxconnectiontime.stop();
	//}
	//if (this->status != FT_OK) {
	//	std::cout << "Could not open Hitachi HD44780: " << MY_LCD_DESCRIPTION << std::endl;
	//}

	bool found = false;
	status = !FT_OK;
	bool error_log = true; // No usable LCD at start
	std::chrono::seconds MaxTime(LCD_MAX_CONNECTION_TIME);/*The display has a settling time after the physical connection so the attempt to connect
												  will be done for a few seconds*/

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> current = start;

	while (status != FT_OK && ((current - start) < MaxTime))//loop till succesful connection o max connecting time is exceeded
	{
		status = FT_OpenEx((void *)MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, &(this->handle));

		if (status == FT_OK)
		{
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(handle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
			{
				error_log = false;
				sendByte(RS::INSTRUCTION_REGISTER, 0x02);
				sendByte(RS::INSTRUCTION_REGISTER, 0x28);
				sendByte(RS::INSTRUCTION_REGISTER, 0x0E);
				sendByte(RS::INSTRUCTION_REGISTER, 0x06);
				sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY);
				this->initerror = false;

			}
			else
				printf("Couldn't configure LCD\n");
		}
		current = std::chrono::system_clock::now();
	}

	if (status != FT_OK)
		printf("Couldn't open LCD\n");
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
	this->status = FT_Write(this->handle, &buffer, sizeof(buffer), &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero
	buffer = buffer | LCD_FUNCTION_ENABLE_ON;								//Prendo enable
	this->status = FT_Write(this->handle, &buffer, sizeof(buffer), &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero
	buffer = buffer ^ LCD_FUNCTION_ENABLE_ON;								//Apago enable
	this->status = FT_Write(this->handle, &buffer, sizeof(buffer), &bytesWritten);		//Escribo a LCD
	this->wait(LCD_WAIT_TIME);												//Espero

}

void Hitachi::sendByte(RS registerselect, UCHAR data)
{
	UCHAR buffer = data;
	buffer = data >> 4;														//Mando low nybble de data a su high nybble
	buffer = buffer & LCD_LOW_NIBBLE;										//Limpio el low nybble de data
	this->sendNybble(registerselect, buffer);								//Mando la parte superior de data como nybble
	buffer = data & LCD_HIGH_NIBBLE;
	this->sendNybble(registerselect, buffer);								//Mando la parte superior de data como nybble


}

bool Hitachi::lcdClear() {


	bool success = false;

	sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_CLEAR_DISPLAY); //Limpio el display

	if (this->status == FT_OK) {
		
		sendByte(RS::INSTRUCTION_REGISTER, LCD_FUNCTION_RETURN_HOME); //Pongo el adress counter en 0.
		
		if (this->status == FT_OK) {
			success = true;
			this->lastCadd = this->cadd = 1;
		}
	}

	return success;

}

bool Hitachi::lcdClearToEOL() {

	bool success = false;

	while ((cadd % 16) != 0) {
		this->lastCadd = this->cadd++; //Hago esto porque sé que el cursor se movió por si mismo y entonces registro dicho cambio en ambas variables.
		sendByte(RS::DATA_REGISTER, LCD_SPACE_CHAR); ///Ni idea cual char de la cartilla es el espacio, será lo que viene antes que el signo de exclamación?
		success = true;
	}

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
		this->cadd = pos.column + (15 * pos.row); //Me paro en la columna adecuada y luego, de ser necesario, bajo una fila.
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

	UCHAR direction = LCD_FUNCTION_CURSOR_SHIFT; //Por default, shiftea hacia la izquierda.


	if (this->cadd > this->lastCadd) { //cadd está a la derecha de lastCadd.

		direction = direction | LCD_D3; //A partir de ahora shiftea hacia la derecha.

		for (int i = 0; i < (this->cadd - this->lastCadd); i++)
			sendByte(RS::INSTRUCTION_REGISTER, direction);
	}
	else if (this->lastCadd > this->cadd) { //cadd está más a la izquierda que lastCadd.

		for (int i = 0; i < (cadd - lastCadd); i++)
			sendByte(RS::INSTRUCTION_REGISTER, direction); //Envío que se mueva a la izquierda;

	}


	this->lastCadd = this->cadd; //Actualizo el valor de lastCadd.

}

//basicLCD& Hitachi::operator<<(const unsigned char * c) {
//
//	std::string str = "";
//
//	for (int i = 0; *(c + i) != '/0'; i++)
//		str += *(c); //Agrego cada caracter al string.
//
//	while (str.size() > 32) {
//		str.erase(0, 1);
//	} //Si el string tiene más de 32 caracteres, borro el primero hasta que queden solo 32.
//
//	for (unsigned int i = 0; i < str.size(); i++) {
//		sendByte(RS::DATA_REGISTER, str[i]);
//		this->lastCadd = this->cadd++; //Se que el cursor se movió, por lo que lo registro en cadd y lastCadd.
//	} ///¿Qué pasa si ya tenía algo escrito y se llena el display? ¿Se sobreescribe? Si es así, ¿Como?
//
//}
//
//basicLCD& Hitachi::operator<<(const unsigned char c) {
//
//	sendByte(RS::DATA_REGISTER, c);
//	this->lastCadd = this->cadd++; ///Nuevamente, falta trabajar los casos límites que, por lo menos a mi, me resultan un poco ambiguos en la consigna, e incluso aunque elija una interpretación ni idea de como solucionarlo.
//
//	return;
//
//}
///Esto no estoy entendiendo bien que corno retornean.
