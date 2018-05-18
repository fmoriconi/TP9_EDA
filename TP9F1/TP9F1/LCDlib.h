#pragma once

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
#define LCD_D4	(1<<PORT_P4)
#define LCD_D5	(1<<PORT_P5)
#define LCD_D6	(1<<PORT_P6)
#define LCD_D7	(1<<PORT_P7)

//Mascaras utiles

#define LCD_HIGH_NIBBLE (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define LCD_LOW_NIBBLE (~(LCD_HIGH_NIBBLE))

//Funciones de LCD

#define LCD_FUNCTION_ENABLE_ON (LCD_ENABLE)
#define LCD_FUNCTION_ENABLE_OFF	(LCD_ENABLE ^ LCD_ENABLE)
#define LCD_FUNCTION_RS_INSTRUCTION_REGISTER (LCD_RS ^ LCD_RS)
#define LCD_FUNCTION_RS_DATA_REGISTER (LCD_RS)
#define LCD_FUNCTION_CLEAR_DISPLAY (0x01)
#define LCD_FUNCTION_RETURN_HOME (0x02)