#include<cstdio>
#include<windows.h>
#include <chrono>
#include <iostream>
#include <string>

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "Hitachi.h"


#define MY_LCD_DESCRIPTION "EDA LCD 3 B"


int main(void)
{
	Hitachi hitachi(MY_LCD_DESCRIPTION);
	if (hitachi.lcdInitOk()) {

		char ch = NULL;

		while ( (ch = getchar() ) != '}') ///Esto es un teclado con el cual podemos probar el display char a char. Con { se limpia la pantalla y con } se cierra el programa.
		{
			if(ch != '\n')
				hitachi << ch;

			if (ch == '{')
				hitachi.lcdClear();
		}

		hitachi << (unsigned char *) "A ver a ver que pasa si pongo algo suuuuuuuuuuuuuuuuuuuuuper largo";

		/*hitachi.lcdClear();

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
*/
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
	}
	else
		std::cout << "lol";

	return 0;
}
