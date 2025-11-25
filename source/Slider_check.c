#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	
	unsigned char i2c_3dslider, i2c_volslider, volmax,volmin;
	Result res_3d, res_vol, mcu_init, res_volmax, res_volmin;

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

		res_volmin = MCUHWC_ReadRegister(0x58, &volmin, 1);//get minimum volume from mcu reg
		res_volmax = MCUHWC_ReadRegister(0x59, &volmax, 1);//get maxiomum volume from mcu reg

		if (R_FAILED(res_vol)) {//if it somehow fails to read data from MCU, very bad if it happens
			printf("Failed to read volume slider data!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_vol);
			sleep(15);
			break;
		}

		if (R_FAILED(res_3d)) {
			printf("Failed to read 3D slider data!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_3d);
			sleep(15);
			break;
		}

		if (R_FAILED(res_volmin)) {
			printf("Failed to read minimum volume!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_volmin);
			sleep(15);
			break;
		}

		if (R_FAILED(res_volmax)) {
			printf("Failed to read maximum volume!\n\n(How did this happen?)\n\nCode: 0x%08lX", res_volmin);
			sleep(15);
			break;
		}

		printf("Welcome to Slider_check! Hold START to quit.\n\n" "Max volume: %3u %02X\e[K\r\n\n" "Min volume: %3u %02X\e[K\r\n\n"
		"Volume slider position: %3u %02X \e[K\r\n\n" "3d slider position: %3u %02X \e[K\r\e[9A",
		volmax, volmax, volmin, volmin, i2c_volslider, i2c_volslider, i2c_3dslider, i2c_3dslider);
		
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