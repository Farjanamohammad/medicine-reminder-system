
/* Menu */
void menu_card(void);

/* RTC editing */
void rtc_menu_card(void);

/* Medicine editing */
void medicine_menu_card(void);

/* Medicine time edit */
void medicine_time(int *hour,int *min);

/* RTC edit */
void rtc_time(int *hour,int *min,int *sec);

/* Check medicine time */
void check_medicine_time(int *hour,int *min,int *sec);

/* Buzzer */
void buzzer_on(void);
void buzzer_off(void);

/* Interrupt init */
void Init_Interrupts(void);

/*display medicine time*/
void DisplaymedicineTime(void);
