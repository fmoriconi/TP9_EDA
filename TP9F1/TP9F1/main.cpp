#include<cstdio>
#include<windows.h>
#include <chrono>
#include <iostream>
#include <string>

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "Hitachi.h"

int main(void)
{
	Hitachi hitachi;
	if (hitachi.lcdInitOk()) {

		char ch = NULL;

		//const unsigned char * str = (const unsigned char *) "ahre"; ///Si no casteo, no compila. 

		//hitachi << (unsigned char *) "ALAN SE LA COME Y ARI SE LA DA"; ///Eso está mal, porque yo aca debería poder poner el "ahre" directamente, y no lo hace.



		//while ( (ch = getchar() ) != '}') ///Esto es un teclado con el cual podemos poner cualquier cosa.
		//{
		//	if(ch != '\n')
		//		hitachi << ch;

		//	if (ch == '{')
		//		hitachi.lcdClear();
		//}

		hitachi.lcdClear();

		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();

		hitachi << (const unsigned char *) "Mid.";

		hitachi.lcdMoveCursorRight();
		hitachi.lcdMoveCursorRight();

		hitachi << (const unsigned char *) "Der.";

		hitachi.lcdSetCursorPosition({ 0,0 });

		hitachi << (const unsigned char *) "Izq.";

		hitachi.lcdSetCursorPosition({ 1,0 });

		hitachi << (const unsigned char *) "Izq.";

		hitachi.lcdSetCursorPosition({ 0,0 });

		hitachi << (const unsigned char *) "Clean 2 EOL:";

		hitachi.lcdClearToEOL();

		//hitachi.sendByte(RS::DATA_REGISTER, 0x41);
		//hitachi.lcdMoveCursorRight();
		//hitachi.sendByte(RS::DATA_REGISTER, 0x6C);
		//hitachi.lcdMoveCursorRight();
		//hitachi.sendByte(RS::DATA_REGISTER, 0x61);
		//hitachi.lcdMoveCursorRight();
		//hitachi.sendByte(RS::DATA_REGISTER, 0x6E);
		//hitachi.lcdMoveCursorLeft();
		//hitachi.lcdMoveCursorLeft();
		//hitachi.lcdMoveCursorLeft();
		//hitachi.lcdMoveCursorLeft();
		//hitachi.lcdMoveCursorLeft();
		//hitachi.lcdMoveCursorLeft();
		//hitachi.sendByte(RS::DATA_REGISTER, 0x20);

		//hitachi.sendByte(RS::DATA_REGISTER, 0x73);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x65);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x20);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x6C);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x61);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x20);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x63);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x6F);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x6D);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x65);
		//hitachi.sendByte(RS::DATA_REGISTER, 0x20);

		std::cout << "nice";
	}
	else
		std::cout << "fuck";
	getchar();
	return 0;
}
