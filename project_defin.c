#include <lpc21xx.h>
#include "lcd.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "rtc.h"
#include "project_defines.h"
#include "project_main.h"
#include "delay.h"

#define MAX_MED 10

/* medicine storage */
int med_hour[MAX_MED];
int med_min[MAX_MED];
int med_count = 0;
int cnt = 0;
int var=0;
extern int flag;


/* external flags */
extern int rtc_display_flag;
extern int menu_flag;
extern int medicine_edit_flag;
extern int rtc_edit_flag;
extern int alarm_flag;


/* MENU DISPLAY */

void menu_card()
{
    char key;
    do{
         INPUT:CmdLCD(0x01);
         StrLCD("1:RTC EDIT");
         CmdLCD(0xC0);
         StrLCD("2:MED_E ");
	   StrLCD("3.EXIT");
         key = KeyScan();
	   while(!ColScan());
         delay_ms(200);
	   if(((key=='1')||(key=='2')||(key=='3'))==0)
	   {
	         CmdLCD(0x01);
		   StrLCD("INVALID INPUT");
		   delay_ms(200);
		   goto INPUT;
	   }
         if(key=='1')
         {
               menu_flag = 0;
               rtc_edit_flag = 1;
		   rtc_menu_card();
         }

         else if(key=='2')
         {
               menu_flag = 0;
               medicine_edit_flag = 1;
		   medicine_menu_card();
         }
	   else if(key=='3')	
	   {
		   menu_flag=0;
		   CmdLCD(0X01);
	   }
      }while(key!='3');
}


              /* RTC EDIT MENU */

void rtc_menu_card()
{
    int h,m,s,d,mo,y,day;

    CmdLCD(0x01);
    StrLCD("SET THE TIME:");
    LABEL1:CmdLCD(0XC0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("HOUR(0-23):");
    h = ReadNum();
    if(!((h>=0) && (h<=23)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invalid hour inp");
	   delay_ms(500);
	   goto LABEL1;
    }
    LABEL2: CmdLCD(0xC0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("MINUTE(0-59)");
    m = ReadNum();
    if(!((m>=0) && (m<=59)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invalid min inp");
	   delay_ms(500);
	   goto LABEL2;
    }
    LABEL3:CmdLCD(0xC0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("SECONDS(0-59)");
    s = ReadNum();
    if(!((s>=0) && (s<=59)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invalid SEC inp");
	   delay_ms(500);
	   goto LABEL3;
    }
    CmdLCD(0x01);
    StrLCD("SET DD-MM-YEAR");
    LABEL4:CmdLCD(0xc0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("Date(1-31):");
    d = ReadNum();
    if(!((d>=1) && (d<=31)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invalid date inp");
	   delay_ms(500);
	   goto LABEL4;
    }
    LABEL5:CmdLCD(0xc0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("MONTH(1-12):");
    mo = ReadNum();
    if(!((mo>=1) && (mo<=12)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invaled month inp");
	   delay_ms(500);
	   goto LABEL5;
    }
    if(mo==2)
    {
    	   if(!((d>=1)&&((d<=28)||(d<=29))))
	   {
		 CmdLCD(0XC0);
		 StrLCD("2-Monthrang(0-29)");
		 delay_ms(500);
		 goto LABEL4;
	   }
    }
    LABEL6:CmdLCD(0xc0);
    StrLCD("                ");
    CmdLCD(0XC0);
    StrLCD("year:");
    y = ReadNum();
    if(!((y>=1111) && (y<=2030)))
    {
	   CmdLCD(0XC0);
	   StrLCD("Invalid YEAR inp");
	   delay_ms(500);
	   goto LABEL6;
    }   
    LABEL7:CmdLCD(0x01);
    StrLCD("SET DAY(0-6):");
    day = ReadNum();
    if(!((day>=0)&&(day<=6)))
    {
	    CmdLCD(0XC0);
	    StrLCD("Invalid day inp");
	    delay_ms(500);
	    goto LABEL7;
    }
    SetRTCTimeInfo(h,m,s);
    SetRTCDateInfo(d,mo,y);
    SetRTCDay(day);
    CmdLCD(0X01);
    rtc_edit_flag = 0;
    rtc_display_flag = 1;
}

                 /* MEDICINE MENU */

void medicine_menu_card()
{
    char key;
    int h,m,i,med_no;
    do
    {
         input2:CmdLCD(0x01);
	   StrLCD("1.NUMt ");
         StrLCD("2:SET ");
         CmdLCD(0xC0);
         StrLCD("3:EDT ");
         StrLCD("4:ADD ");    
    	   StrLCD("5.EX");
         key = ReadNum();
	   if(((key==1)||(key==2)||(key==3)||(key==4)||(key==5))==0)
	   {
	 	CmdLCD(0x01);
	 	StrLCD("INVALID INPUT");
		delay_ms(200);
		goto input2;
	   }
         /* SET NUMBER OF MEDICINES */
         if(key==1)
         {
            CmdLCD(0x01);
            TABLET: StrLCD("TAB COUNT:");
            cnt = ReadNum();
        	if(cnt > MAX_MED)
        	{    
			StrLCD("MAXCNT REACH");
			goto TABLET;
		}
            med_count = cnt;
            CmdLCD(0x01);
            StrLCD("COUNT SAVED");
            delay_ms(1500); 
         }
         /* SET MEDICINE TIMES */
         else if(key==2)
        {
           if(med_count == 0)
           {
               CmdLCD(0x01);
               StrLCD("SET COUNT 1ST");
               delay_ms(1500);
               return;
           }
           for(i=0;i<med_count;i++)
           {
               CmdLCD(0x01);
               StrLCD("MED Timing:");
               CharLCD('1'+i);
		   MEDH1:CmdLCD(0XC0);
		   StrLCD("                ");
		   CmdLCD(0XC0);
   		   StrLCD("HOUR(0-23):");
    		   h = ReadNum();
		   if(!((h>=0) && (h<=23)))
		   {
			CmdLCD(0XC0);
			StrLCD("Invalid hour inp");
			goto MEDH1;
		   }
   		   MEDM1: CmdLCD(0xC0);
   		   StrLCD("                ");
		   CmdLCD(0XC0);
    		   StrLCD("MINUTE(0-59)");
    		   m = ReadNum();
		   if(!((m>=0) && (m<=59)))
		   {
		       CmdLCD(0XC0);
			 StrLCD("Invalid min inp");
			 goto MEDM1;
		   }
               med_hour[i] = h;
               med_min[i] = m;
          }
        CmdLCD(0x01);
        StrLCD("TIME SAVED");
        delay_ms(1500);
	  DisplaymedicineTime();
	  delay_ms(100);
     }
     /* EDIT PARTICULAR MEDICINE */
     else if(key==3)
     {
        if(med_count == 0)
        {
            CmdLCD(0x01);
            StrLCD("NO MED DATA");
            delay_ms(1500);
            return;
        }
        EDITM:CmdLCD(0x01);
        StrLCD("MED NUMBER:");
        med_no = ReadNum();
        if(med_no > med_count || med_no == 0)
        {
            CmdLCD(0x01);
            StrLCD("INVALID");
            delay_ms(1500);
            goto EDITM;
        }

        med_no = med_no - 1;
	  MEDH2:CmdLCD(0XC0);
	  StrLCD("                ");
	  CmdLCD(0XC0);
   	  StrLCD("HOUR(0-23):");
    	  h = ReadNum();
	  if(!((h>=0) && (h<=23)))
	  {
		CmdLCD(0XC0);
		StrLCD("Invalid hour inp");
		goto MEDH2;
	  }
   	  MEDM2: CmdLCD(0xC0);
   	  StrLCD("                ");
	  CmdLCD(0XC0);
    	  StrLCD("MINUTE(0-59)");
    	  m = ReadNum();
	  if(!((m>=0) && (m<=59)))
	  {
		CmdLCD(0XC0);
		StrLCD("Invalid min inp");
		goto MEDM2;
	  }
        med_hour[med_no] = h;
        med_min[med_no] = m;
        CmdLCD(0x01);
        StrLCD("UPDATED");
        delay_ms(1500);
	  DisplaymedicineTime();
	  delay_ms(100);
    }
    /* ADD NEW MEDICINE */
    else if(key==4)
    {
        if(med_count >= MAX_MED)
        {
            CmdLCD(0x01);
            StrLCD("MEMORY FULL");
            delay_ms(1500);
            return;
        }

        CmdLCD(0x01);
        StrLCD("ADD MED ");
        CharLCD('1'+med_count);
	  MEDH3:CmdLCD(0XC0);
	  StrLCD("                ");
	  CmdLCD(0XC0);
   	  StrLCD("HOUR(0-23):");
    	  h = ReadNum();
	  if(!((h>=0) && (h<=23)))
	  {
		CmdLCD(0XC0);
		StrLCD("Invalid hour inp");
		goto MEDH3;
	  }
   	  MEDM3: CmdLCD(0xC0);
   	  StrLCD("                ");
	  CmdLCD(0XC0);
    	  StrLCD("MINUTE(0-59)");
     	  m = ReadNum();
	  if(!((m>=0) && (m<=59)))
	  {
	     CmdLCD(0XC0);
	     StrLCD("Invalid min inp");
	     goto MEDM3;
	  }
        med_hour[med_count] = h;
        med_min[med_count] = m;
        med_count++;
        CmdLCD(0x01);
        StrLCD("MED ADDED");
        delay_ms(1500);
	  DisplaymedicineTime();
	  delay_ms(100);
    }
  }while(key!=5);
  CmdLCD(0X01);
  medicine_edit_flag = 0;
  rtc_display_flag = 1;
}

             /* CHECK MEDICINE TIME */
void check_medicine_time(int *hour,int *min ,int *sec)
{
    int i;
    int start_min;

    for(i=0;i<med_count;i++)
    {
        if((*hour==med_hour[i]) && (*min==med_min[i]) && (*sec ==0) && (rtc_display_flag==1))
        {
            rtc_display_flag = 0;

            CmdLCD(0x01);
            StrLCD("TAKE MEDICINE");

            alarm_flag = 1;
            buzzer_on();

            start_min = *min;

            while(1)
            {
                GetRTCTimeInfo(hour,min,sec);

                if(alarm_flag == 0)
                {
					CmdLCD(0X01);
                    break;
                }

                if(*min != start_min)
                {
                    buzzer_off();

                    CmdLCD(0x01);
                    StrLCD("MED SKIPPED");
                    delay_ms(1500);

                    alarm_flag = 0;
                    rtc_display_flag = 1;
					CmdLCD(0X01);
                   break;
                }
            }
        }
    }
}


/* BUZZER CONTROL */

void buzzer_on()
{
    IOSET0=1<<BUZZER;
}

void buzzer_off()
{
    IOCLR0=1<<BUZZER;
}

void DisplaymedicineTime()
{
  
  int i;
  CmdLCD(0x01);
  StrLCD("MEDICINE TIMING");
  for(i=0;i<med_count;i++)
  {
      CmdLCD(0XC0);
	CharLCD(med_hour[i]/10+48);
	CharLCD(med_hour[i]%10+48);
	CharLCD(':');
	CharLCD(med_min[i]/10+48);
	CharLCD(med_min[i]%10+48);
	delay_ms(1500);
  }
}

