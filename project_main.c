#include <lpc21xx.h>

#include "delay.h"
#include "rtc_defines.h"
#include "rtc.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"

#include "project_defines.h"
#include "project_main.h"
#include "interrupt.h"


/* RTC variables */

int hour,min,sec;
int date,month,year;
int day;

/* flags */

int alarm_flag = 0;
int rtc_display_flag = 1;
int medicine_edit_flag = 0;
int rtc_edit_flag = 0;
int menu_flag = 0;

int main()
{

    /* buzzer pin */

    IODIR0 |= 1<<BUZZER;


    /* initialize modules */

    RTC_Init();
    InitLCD();
    InitKPM();
    Init_Interrupts();

    while(1)
    {

        /* RTC DISPLAY MODE */

        if(rtc_display_flag==1)
        {

            do
            {

                GetRTCTimeInfo(&hour,&min,&sec);
                DisplayRTCTime(hour,min,sec);

                GetRTCDateInfo(&date,&month,&year);
                DisplayRTCDate(date,month,year);

                GetRTCDay(&day);
                DisplayRTCDay(day);

                check_medicine_time(&hour,&min,&sec);

                delay_ms(200);

            }

            while((rtc_display_flag == 1) && (menu_flag == 0));

        }

        /* MEDICINE MENU */
	  if(menu_flag==1)
	  {
		menu_card();
	  }
	  if(medicine_edit_flag==1)
	  {
            medicine_menu_card();

	  }

	  /* RTC MENU */
	  else if(rtc_edit_flag==1)
	  {
		rtc_menu_card();

	  }

    }

}

