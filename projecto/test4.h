#ifndef TEST4_H
#define TEST4_H

#include <minix/sysutil.h>


#define KB_IRQ 0x01
#define TIMER0_IRQ 0x00
#define DELAY_US 20000
#define STAT_REG 0x64
#define DATA_REG 0x60
#define LEDS_SWITCH 0xED
#define BIT(n) (0x01<<n)

//slides da teorica

#define OBF		BIT(0)
#define IBF BIT(1)
#define SYS BIT(2)
#define A2 BIT(3)
#define INH BIT(4)
#define AUX		BIT(5)
#define TO_ERR		BIT(6)
#define PAR_ERR		BIT(7)




int read_keyboard();

int to_keyboard(unsigned long port, unsigned char byte);

unsigned long keyboard_subscribe( );

int test_scan();

int test_leds(unsigned short n, unsigned short *leds);


int kbc_unsubscribe_int();

int timer_subscribe_int();

int timer_unsubscribe_int();

#endif

/*
//#ifndef __TEST4_H
//#define __TEST4_H
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#define OUT_BUF 0x60
#define IN_BUF 0x60
#define KB_IRQ 1
#define S_LEDS_C 0xED
#define KB_RESEND 0xFE
#define KB_ERROR 0xFC
#define TIMER0_IRQ 0
#define TIMER_HOOKBIT 2
#define KB_HOOKBIT 1
#define DELAY_US    20000

int test_scan(void);
int test_leds(unsigned short n, unsigned short *leds);
unsigned long kb_read(unsigned long irq_set);
int kb_toggle_led(char n);
*/
