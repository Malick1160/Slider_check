#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	
	unsigned char i2c_3dslider, i2c_volslider;
	Result res_3d, res_vol, mcu_init;

	gfxInitDefault(); //init services 
	mcu_init = mcuHwcInit();

	consoleInit(GFX_TOP, NULL);

	while (aptMainLoop())
	{
		
		if (R_FAILED(mcu_init)) {//mcuHcInit can fail due to old luma or *hax setup and in that case we immediately stop the program to prevent unintended effects
			printf("mcuHwcInit Failed! (outdated setup?)");
			sleep(20);
			break;
		}

		res_vol = MCUHWC_ReadRegister(0x27, &i2c_volslider, 1);//get raw volume slider position from mcu reg
		res_3d = MCUHWC_ReadRegister(0x8, &i2c_3dslider, 1);//get raw 3d slider position from mcu reg


		if (R_FAILED(res_vol)) {//if it somehow fails to read volume slider from MCU, very bad if it happens
			printf("Failed to read volume slider data!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_vol);
			sleep(20);
			break;
		}

		if (R_FAILED(res_3d)) {//if it somehow fails to read 3d slider from MCU, very bad if it happens
			printf("Failed to read 3D slider data!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_3d);
			sleep(20);
			break;
		}

		printf("Welcome to Slider_check! Hold START to quit.\n\ni2c Volume(Raw Value): %3u %02X \e[K\r\n\n" "i2c 3d(Raw Value): %3u %02X \e[K\r\e[5A", i2c_volslider, i2c_volslider, i2c_3dslider, i2c_3dslider);
		
		hidScanInput();//quit by holding start

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();

	}

	// Exit services
	mcuHwcExit();
	gfxExit();
	return 0;
}