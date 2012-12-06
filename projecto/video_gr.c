#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>

#include "vbe.h"


/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned long mode) {

	struct reg86u registos;

	struct mem_range alcance;
	vbe_mode_info_t vbe_info;

	//definir o modo de inicializaçao da placa grafica
	registos.u.b.intno = 0x10;
	registos.u.w.ax=0x4F02;
	registos.u.w.bx = 1 << 14 | mode;

	sys_int86(&registos); //alterar as defs da placa grafica

	vbe_get_mode_info(mode, &vbe_info);

	//definir o range do mapeamento da memoria
	alcance.mr_base = vbe_info.PhysBasePtr;
	alcance.mr_limit = alcance.mr_base + vbe_info.XResolution * vbe_info.YResolution * (vbe_info.BitsPerPixel/8);

	//pedir permissoes ao minix
	sys_privctl(SELF, SYS_PRIV_ADD_MEM, &alcance);
	video_mem = vm_map_phys(SELF, (void *)alcance.mr_base, vbe_info.XResolution * vbe_info.YResolution * (vbe_info.BitsPerPixel/8));

	v_res = vbe_info.YResolution;
	h_res = vbe_info.XResolution;
	bits_per_pixel = vbe_info.BitsPerPixel;


}

int vg_fill(unsigned long color) {

	char *memoria;
	memoria = video_mem;
	int i;
	unsigned fim = h_res * v_res;
	for (i=0;i< fim;i++){
		*memoria=color;
		memoria+=bits_per_pixel/8;
	}

	return 0;}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	char* memoria;
	memoria = video_mem;
	*(memoria + y*h_res+x) = color;
	return 0;

}

long vg_get_pixel(unsigned long x, unsigned long y) {

	long cor;
	cor = *(video_mem + y*h_res +x);

	return cor;

}

int vg_draw_line(unsigned long xi, unsigned long yi,
		unsigned long xf, unsigned long yf, unsigned long color) {
	int x, y, i;

	if (xi == xf)
	{
		if (yi > yf)
		{
			int temp = yi;
			yi = yf;
			yf= temp;
		}

		y = yi;

		for (i = 0; i <= (yf-yi); i++)
		{
			vg_set_pixel(xi, y, color);

			y++;
		}
	}

	else if ((yi == yf))
	{
		if (xi > xf)
		{
			int temp = xi;
			xi =xf;
			xf= temp;
		}

		x = xi;

		for (i = 0; i <= (xf-xi); i++)
		{
			vg_set_pixel(x, yi, color);

			x++;
		}
	}

	else{


		for(x=xi; x<=xf; x++)
		{
			y = (((yf - yi) * (x-xi))/(xf-xi)) + yi;
			vg_set_pixel(x, y, color);
		}

}

return 0;
}



int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;

}
