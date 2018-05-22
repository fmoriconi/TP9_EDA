#pragma once

#include "ftd2xx.h"
#include "Timer.h"

//Puertos fisicos
#define PORT_P0 0
#define PORT_P1 1
#define PORT_P2 2
#define PORT_P3 3
#define PORT_P4 4
#define PORT_P5 5
#define PORT_P6 6
#define PORT_P7 7

//Puertos logicos
#define LCD_ENABLE	(1<<PORT_P0)
#define LCD_RS	(1<<PORT_P1)
#define LCD_D0 (1<<PORT_P0)
#define LCD_D1	(1<<PORT_P1)
#define LCD_D2	(1<<PORT_P2)
#define LCD_D3	(1<<PORT_P3)
#define LCD_D4	(1<<PORT_P4)
#define LCD_D5	(1<<PORT_P5)
#define LCD_D6	(1<<PORT_P6)
#define LCD_D7	(1<<PORT_P7)

//Mascaras utiles

#define LCD_BYTE (0xFF)
#define LCD_HIGH_NIBBLE (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define LCD_LOW_NIBBLE (~(LCD_HIGH_NIBBLE))

//Funciones de LCD

#define LCD_FUNCTION_ENABLE_ON (LCD_ENABLE)
#define LCD_FUNCTION_ENABLE_OFF	(LCD_ENABLE ^ LCD_ENABLE)
#define LCD_FUNCTION_RS_INSTRUCTION_REGISTER (LCD_RS ^ LCD_RS)
#define LCD_FUNCTION_RS_DATA_REGISTER (LCD_RS)
#define LCD_FUNCTION_CLEAR_DISPLAY (0x01)
#define LCD_FUNCTION_RETURN_HOME (0x02)
#define LCD_FUNCTION_ENTRY_MODE_SET (LCD_D3|LCD_D2)
#define LCD_FUNCTION_DISPLAY_CONTROL_OFF (LCD_D3)
#define LCD_FUNCTION_FUNCTION_SET_4_BIT	(LCD_D5)
#define LCD_FUNCTION_FUNCTION_SET_8_BIT (LCD_D5|LCD_D4)
#define LCD_FUNCTION_CURSOR_SHIFT (LCD_D4) //LCD_D2 debe setearse según se desee mover para la izquierda o para la derecha.

#define LCD_SPACE_CHAR 0x20 // Por ahora pongo este pero ni idea si está bien. Creo que si igual.
#define LCD_DISPLAY_LINE2 0x40 //Posición de memoria de la segunda linea del display.

#define LCD_FUNCTION_SET_DDRAM_ADDRESS LCD_D7 //Con ese bit encendido indico la instrucción de seteo de la posición del cursor. El resto de los bits indican dicha dirección.

#define LCD_MAX_CONNECTION_TIME 5000
#define LCD_WAIT_TIME 10

enum class RS { INSTRUCTION_REGISTER, DATA_REGISTER };

bool initDisplay(FT_HANDLE* handle, const char* lcd_description);
FT_STATUS sendNybbleLow(RS registerselect, UCHAR data, FT_HANDLE* handle);
FT_STATUS sendByteLow(RS registerselect, UCHAR data, FT_HANDLE* handle);
void waitLow(unsigned);