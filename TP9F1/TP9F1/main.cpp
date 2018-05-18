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
	bool isok = hitachi.lcdInitOk();
	std::cout << isok;
	getchar();
	return 0;
}
