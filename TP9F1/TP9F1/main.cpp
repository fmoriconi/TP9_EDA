#include<cstdio>
#include<windows.h>
#include <chrono>
#include <iostream>

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include "LCDlib.h"
#include "Hitachi.h"

int main(void)
{
	Hitachi hitachi;
	if (hitachi.lcdInitOk()) {
		hitachi.sendByte(RS::DATA_REGISTER, 0x41);
		std::cout << "nice";
	}
	else
		std::cout << "fuck";
	getchar();
	return 0;
}
