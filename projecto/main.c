#ifdef EMUL /* In emulation mode*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#else	/* Running on Minix*/

#include <minix/drivers.h>

#endif

#include "vbe.h"
#include "video_gr.h"

#define WAIT_TIME_S 5
/* Return to text mode */

int vg_draw_square(unsigned long xi, unsigned long yi, unsigned long xf, unsigned long yf, unsigned long color)
{
	vg_draw_line(xi,yi, xf, yi, color);
	vg_draw_line(xi,yi, xi, yf, color);
	vg_draw_line(xi,yf, xf, yf, color);
	vg_draw_line(xf,yf, xf, yi, color);

	int i;
	int j;
	int xi2 = xi, yi2 = yi;

	for (i = 0; i < (yf-yi); i++)
	{
		for (j = 0; j < (xf-xi); j++)
		{
			vg_set_pixel(xi2,yi2, color);
			xi2++;
		}
		yi2++;
		xi2 = xi;
	}
}

int vg_move_square_down_right(unsigned long xi, unsigned long yi, unsigned long xf, unsigned long yf, unsigned long color, unsigned int num_movimentos)
{
	int i;
	for (i = 0; i < num_movimentos; i++)
	{
		vg_draw_square(xi, yi, xf, yf, color);

		sleep(1);
		vg_draw_square(xi, yi, xf, yf, vg_get_pixel(0,0));

		xi+=10;
		xf+=10;
		yi+=10;
		yf+=10;
	}
}

int vg_move_square_up(unsigned long xi, unsigned long *yi, unsigned long xf, unsigned long *yf, unsigned long color, unsigned int num_movimentos)
{
	int i;
	for (i = 0; i < num_movimentos; i++)
	{
		if ((xi-10) >= 0 && (xf-10) >= 0 && (*yi-10) >= 0 && (*yf-10) >= 0)
		{
			vg_draw_square(xi, (*yi), xf, (*yf), color);

			sleep(1);
			vg_draw_square(xi, (*yi), xf, (*yf), vg_get_pixel(0,0));

			(*yi)-=10;
			(*yf)-=10;
		}
		else
			break;
	}
}

int main()
{
	//permite ao programa usar a memoria
	sef_startup();
	/* Change to video mode */
	vg_init(0x105);
	vg_fill(0x16);

	//vg_draw_square(0,0,400,100, 0x14);

	scan_kb();

	//test_scan();
	sleep(5);
	/*  for WAIT_TIME_S (5) seconds */
	vg_exit();

	return 0;
}

/*
 * W - 0x11 0x91
A - 0x1E 0x9E
S - 0X1F 0X9F
D - 0X20 0XA0
 */

