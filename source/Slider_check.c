#include <stdio.h>
#include <string.h>
#include <3ds.h>


int main(int argc, char * argv[])
{

	gfxInitDefault(); //init services 
	mcuHwcInit();
	
	//define and init variables 
	unsigned char pos_volslider, oldvolslider, pos_3dslider, old3dslider, i2c_3dslider, i2c_volslider, old_i2c_volslider, old_i2c_3dslider;

	old3dslider = 0;
	oldvolslider = 0;
	pos_3dslider = 0;
	pos_volslider = 0;
	i2c_3dslider = 0;
	i2c_volslider = 0;
	old_i2c_3dslider = 0;
	old_i2c_volslider = 0;

	consoleInit(GFX_TOP, NULL);

	while (aptMainLoop())
	{
		hidScanInput();//quit by holding start

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		MCUHWC_Get3dSliderLevel(&pos_3dslider);
		MCUHWC_GetSoundSliderLevel(&pos_volslider);
		MCUHWC_ReadRegister(0x27, &i2c_volslider, 1);//get raw volume slider pos from mcu reg
		MCUHWC_ReadRegister(0x8, &i2c_3dslider, 1);//get raw 3d slider pos from mcu reg

		//update printf only if registered values are different from old ones
		if (old3dslider != pos_3dslider || oldvolslider != pos_volslider || old_i2c_3dslider != i2c_3dslider || old_i2c_volslider != i2c_volslider) {

			printf("\n\033[5A\033[2K\rVolume Slider Position: %u %X\n"
				"\033[2K\r3d Slider Position : %u %X \n"
				"\033[2K\ri2c Volume(Raw Value): %u %X\n"
				"\033[2K\ri2c 3d(Raw Value): %u %X\n",
				pos_volslider, pos_volslider, pos_3dslider, pos_3dslider, i2c_volslider, i2c_volslider, i2c_3dslider, i2c_3dslider);		
		}
		
		old3dslider = pos_3dslider;
		oldvolslider = pos_volslider;
		old_i2c_3dslider = i2c_3dslider;
		old_i2c_volslider = i2c_volslider;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();

	}

	// Exit services
	mcuHwcExit();
	gfxExit();
	return 0;
}