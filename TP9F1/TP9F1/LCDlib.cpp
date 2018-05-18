#include "LCDlib.h"
#include <iostream>

FT_HANDLE * lcdInit(int iDevice)
{
	Timer timer;
	FT_HANDLE * handle=nullptr;
	DWORD byteswritten;
	FT_STATUS status = FT_OpenEx((PVOID)MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, handle);
	if (status == FT_OK)
	{
		status = FT_SetBitMode(handle, 0xff, 1);
		if (status == FT_OK)
		{
			status=FT_Write(handle, (LPVOID)((0xff) ^ LCD_D0 ^ LCD_D1 ^LCD_D2 ), 1, &byteswritten);
			timer.start();
			do
			{
				timer.stop();

			} while (timer.getTime() < 4);
			if (status == FT_OK)
			{
				status=FT_Write(handle, (LPVOID)((0xff) ^ LCD_D0 ^ LCD_D1 ^LCD_D2), 1, &byteswritten);
				timer.start();
				do
				{
					timer.stop();

				} while (timer.getTime() < 0.1);
				
				status=FT_Write(handle, (LPVOID)((0xff) ^ LCD_D0 ^ LCD_D1 ^LCD_D2), 1, &byteswritten);
				if (status == FT_OK)
				{
					status=FT_Write(handle, (LPVOID)((0xff) ^ LCD_D0 ^ LCD_D1 ^LCD_D2 ^ LCD_D4), 1, &byteswritten);
					if (status == FT_OK)
					{
						status = FT_Write(handle, (LPVOID)((0xff) ^ LCD_D0 ^ LCD_D1 ^LCD_D2^ LCD_D4), 2, &byteswritten);
						if (status == FT_OK)
						{
							status = FT_Write(handle, (LPVOID)(LCD_D3), 2, &byteswritten);
							if (status == FT_OK)
							{
								status = FT_Write(handle, (LPVOID)(LCD_FUNCTION_CLEAR_DISPLAY), 2, &byteswritten);
								if (status == FT_OK)
								{
									status = FT_Write(handle, (LPVOID)(LCD_D2 | LCD_D1), 2, &byteswritten);
									std::cout << "funciono la concha de tu madre" << std::endl;
								}
							}
						}
					}
				}
			}
		}
	}

	return handle;
}
