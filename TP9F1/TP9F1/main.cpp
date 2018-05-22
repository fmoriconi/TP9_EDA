#include<cstdio>
#include<windows.h>
#include <chrono>
#include <iostream>
#include <string>

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "Hitachi.h"

//EJEMPLO PARA USAR LA LIBRERIA HITACHI MADE BY GRUPO 3

#define MY_LCD_DESCRIPTION "EDA LCD 3 B"

int main(void)
{
	Hitachi hitachi(MY_LCD_DESCRIPTION);
	if (hitachi.lcdInitOk()) {


	//Presentacion
		hitachi << (const unsigned char *)"    LOADING..    " ;
		for (int i = 0; i < 13; i++) {
			hitachi << (char)0xFF;
			hitachi.wait(50);
		}
		hitachi.sendByte(RS::DATA_REGISTER, 0xFF);
		hitachi.wait(700);
		hitachi.lcdClear();
		hitachi << (const unsigned char*)"     READY!     ";
		hitachi.wait(400);
		hitachi.lcdClear();

	//Mini tecladito
		char ch = NULL;
		while ( (ch = getchar() ) != '}') //Esto es un teclado con el cual podemos probar el display char a char. Con { se limpia la pantalla y con } se cierra el programa.
		{
			if(ch != '\n')
				hitachi << ch;

			if (ch == '{')
				hitachi.lcdClear();
		}
	}

	//Error
	else {
		std::cout << "Could not load LCD Hitachi: " << MY_LCD_DESCRIPTION << std::endl;
		std::cout << "Check if the LCD description is right, or if the LCD is plugged in." << std::endl;
		getchar();
	}

	return 0;
}
