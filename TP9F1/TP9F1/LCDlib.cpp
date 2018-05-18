#include "LCDlib.h"

FT_HANDLE * lcdInit(int iDevice)
{
	Timer timer;
	FT_HANDLE * handle;
	FT_STATUS status = FT_OpenEx(MY_LCD_DESCRIPTION, FT_OPEN_BY_DESCRIPTION, handle);
	if (status == FT_OK)
	{
		status = FT_SetBitMode(handle, 0xff, 1);
		if (status == FT_OK)
		{
			/*FT_Write(handle,(LCD_LOW_NIBBLE ^ )*/
		}
	}

}
