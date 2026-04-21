#include "type.h"
#include "delay.h"
#include <lpc21xx.h>
#include "project_defines.h"
#include "interrupt.h"
#include "lcd.h"
#include "project_main.h"

/* flags from main */
extern int rtc_display_flag;
extern int menu_flag;
extern int alarm_flag;

#define EINT0_INPUT_PIN 0X0000000C
#define EINT0_VIC_CHN0 14


#define EINT1_INPUT_PIN 0X000000C0
#define EINT1_VIC_CHN0 15

void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void Init_Interrupts(void)
{
	//cfg po.1 & p0.3 pin as EINTO EINT1 INPUT pin
	//clr bit (2 &3 )   AND   (6&7)w/0 affecting other bit
	PINSEL0&=(u32)~(3<<2)|(u32)~(3<<6);
	//update the bit 2& 3 for EINT0 PIN functions
	PINSEL0|=EINT0_INPUT_PIN|EINT1_INPUT_PIN;
	//cfg VIC peripherial |block
	//allow EINT0 ,EINT1 AS IRQ type
	//VICINTSELECT=0//DEFAULT
	//enable the einto EINT1 through channel
	VICIntEnable=1<<EINT0_VIC_CHN0|1<<EINT1_VIC_CHN0;
	//cfg  Enable as v.irq with highest priority
	VICVectCntl0=(1<<5)|EINT0_VIC_CHN0;
	//load the eint0__isr address into sfr
	VICVectAddr0=(u32)eint0_isr;
	//cfg  Enable as v.irq with highest priority
	VICVectCntl1=(1<<5)|EINT1_VIC_CHN0;
	//load the eint1__isr address into sfr
	VICVectAddr1=(u32)eint1_isr;


	//cfg EINT0 ,EINT1 VIA EXTERNAL INTERRUPT SFRS
	//ENABLE THE EINT0 EINT1
	//EXTINT0 =0 ---DEFAULT
	//CFG THE EINT0 ,EINT1 as edge triggerd interupt
	EXTMODE=(1<<1)|(1<<0);
	//CFG THE EINT0 ,EINT1 as falling edge trigger
	//EXTPOLAR=0--->DEFAULT
	//CFG THE STATUS OF LED PIN FOR EINT0,EINT1 AS gpio_out
	//IODIR1|=1<<EINT0_STATUS_LED|1<<EINT1_STATUS_LED;
	EXTPOLAR = 0;
	
	
}
void eint0_isr(void) __irq
{
	//make rtc display_flag	to '0'
	rtc_display_flag = 0;
	//enable the  menu_flag
      menu_flag = 1;
	//CLEAR EINT0 STATUS IN EXTINT PERIPHERIAL
	EXTINT=1<<0;
	//CLEAR EINT0 STATUS IN VIC PERIPHERAL
	VICVectAddr=0;
}

void eint1_isr(void) __irq
{
	//check alarm flag is enable or not if enable clear the buzzer	
	 if(alarm_flag)
       {
	     //stop buzzer
           buzzer_off();
           alarm_flag = 0;
           //rtc_display_flag = 1;
       }
	 //make rtc_display_flag=1;
	 rtc_display_flag = 1;

	 //CLEAR EINT1 STATUS IN EXTINT PERIPHERIAL
	 EXTINT=1<<1;
	 //CLEAR EINT1 STATUS IN VIC PERIPHERAL
	 VICVectAddr=0;
}


