#include <minix/syslib.h>
#include <minix/drivers.h>

#include "test4.h"

static int hook_id2 =2;
static int hook_id = 1; //para as interrup�oes
unsigned char scan_code = 0;


int test_scan() {

	unsigned long irq_set = keyboard_subscribe();
	printf("Kb scan started:\n");
	while (scan_code != 0x81) {
		read_keyboard(irq_set);
		if (scan_code & 0x80) {
			printf("Breakcode: 0x%X\n", scan_code);
		} else {
			printf("Makecode: 0x%X\n", scan_code);
		}
	}
	keyboard_unsubscribe();
	return 0;

}
int scan_kb(unsigned int *xi, unsigned int *yi, unsigned int *xf, unsigned int *yf, unsigned long color) {


	unsigned long irq_set = keyboard_subscribe();
	printf("Kb scan started:\n");
	while (scan_code != 0x81) {
		read_keyboard(irq_set);
		if (scan_code & 0x80) {
			if (scan_code == 0x91)
			{
				vg_move_square_up(xi,yi,xf,yf, color, 2);
			}
			else if (scan_code == 0x9F)
			{
				vg_move_square_down(xi,yi,xf,yf, color, 2);
			}
			else if (scan_code == 0x9E)
			{
				vg_move_square_left(xi,yi,xf,yf, color, 2);
			}
			else if (scan_code == 0xA0)
			{
				vg_move_square_right(xi,yi,xf,yf, color, 2);
			}

		} else {
			printf("Makecode: 0x%X\n", scan_code);
		}
	}
	keyboard_unsubscribe();
	return 0;

}



int keyboard_unsubscribe() {
	if (sys_irqdisable(&hook_id) == OK && sys_irqrmpolicy(&hook_id) == OK)
			return 0;
		else
			return 1;
}




unsigned long keyboard_subscribe() {
printf("teste subs");
	int hook = hook_id;

		if (sys_irqsetpolicy(KB_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) == OK && sys_irqenable(&hook_id) == OK)
		{
			printf("hook: %d", hook);
			return (1 << hook);

		}
		else
			return -1;



}




int read_keyboard(unsigned long irq_set) {

	int ipc_status;
		unsigned long tmpscancode;
		message msg;
		while (!(driver_receive(ANY, &msg, &ipc_status) == 0 && is_ipc_notify(
				ipc_status) && msg.NOTIFY_ARG & irq_set)) {
		}
		while (sys_inb(DATA_REG, &tmpscancode) != OK) {
		}
		scan_code = tmpscancode;
		return 0;
}




int to_keyboard(unsigned long port, unsigned char byte) {
	unsigned long st; // estado

	while (1) {
		sys_inb(STAT_REG, &st);
		if ((st & IBF) == 0) {
			sys_outb(port, byte);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return -1;
}

/*
#include "test4.h"

static int hook_id;
unsigned char scancode;
char leds = 0;

unsigned long kb_read(unsigned long irq_set) {
	int ipc_status;
	unsigned long tmpscancode;
	message msg;
	while (!(driver_receive(ANY, &msg, &ipc_status) == 0 && is_ipc_notify(
			ipc_status) && msg.NOTIFY_ARG & irq_set)) {
	}
	while (sys_inb(OUT_BUF, &tmpscancode) != OK) {
	}
	scancode = tmpscancode;
	return 0;
}

int kb_subscribe_int(void) {
	hook_id = 1;
	if (sys_irqsetpolicy(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == OK
			&& sys_irqenable(&hook_id) == OK) {
		return 2;
	}
	return 0;
}

int kb_unsubscribe_int(void) {
	if (sys_irqrmpolicy(&hook_id) != OK) {
		return 1;
	}
	return 0;
}

int test_scan(void) {
	unsigned long irq_set = kb_subscribe_int();
	printf("Kb scan started:\n");
	while (scancode != 0x81) {
		kb_read(irq_set);
		if (scancode & 0x80) {
			printf("Breakcode: 0x%X\n", scancode);
		} else {
			printf("Makecode: 0x%X\n", scancode);
		}
	}
	kb_unsubscribe_int();
	return 0;
}

int kb_toggle_led(char n) {
	unsigned long ret;
	char cicle_repeat = 0xFF;
	leds ^= 1 << n;
	while (cicle_repeat && 1) {
		tickdelay(micros_to_ticks(DELAY_US));
		if (sys_outb(IN_BUF, S_LEDS_C) != OK) {
			cicle_repeat = 1;
			continue;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		if (sys_inb(OUT_BUF, &ret) != OK || ret == KB_ERROR || ret == KB_RESEND) {
			cicle_repeat = 1;
			continue;
		} else {
			cicle_repeat = 2;
		}
		while (cicle_repeat && 2) {
			tickdelay(micros_to_ticks(DELAY_US));
			if (sys_outb(IN_BUF, leds) != OK) {
				cicle_repeat = 2;
				continue;
			}
			tickdelay(micros_to_ticks(DELAY_US));
			if (sys_inb(OUT_BUF, &ret) != OK || ret == KB_RESEND) {
				cicle_repeat = 2;
				continue;
			} else if (ret == KB_ERROR) {
				cicle_repeat = 3;
			} else {
				cicle_repeat = 0;
			}
		}

	}

}

int test_leds(unsigned short n, unsigned short *leds) {
	int i;
	printf("testing leds... \n");
	for (i = 0; i < n; i++) {
		kb_toggle_led(*leds);
		sleep(1);
		++leds;
	}
	printf("done \n");
}

*/
