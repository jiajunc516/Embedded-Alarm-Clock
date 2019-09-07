/*
 * Project2.c
 *
 * Created: 2018/2/2 10:41:07
 * Author : Xiao Yang, Jiajun Chang
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
int lcd_row;
int lcd_column;


int is_pressed(int r, int c)
{
	DDRC = 0x00;
	SET_BIT(DDRC, r);
	PORTC = 0x00;
	SET_BIT(PORTC,c+4);
	wait_avr(1);
	
	if (GET_BIT(PINC,c+4)) return 0;
	return 1;
}


int encode_key(void)
{
	for (int r = 0; r < 4; ++r)
		for(int c = 0; c < 4; ++c)
			if(is_pressed(r,c)!=0)
				return  r*4+c+1;
	return 0;
}

char* get_key(int k)
{
	switch (k)
	{
		case 1:
			return "1";
		case 2:
			return "2";
		case 3:
			return "3";
		case 4:
			return "A";
		case 5:
			return "4";
		case 6:
			return "5";
		case 7:
			return "6";
		case 8:
			return "B";
		case 9:
			return "7";
		case 10:
			return "8";
		case 11:
			return "9";
		case 12:
			return "C";
		case 13:
			return "*";
		case 14:
			return "0";
		case 15:
			return "#";
		case 16:
			return "D";
		default:
			return 0;
	}
}


struct date_time{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int hour2;
	char* ampm;	
	};

void change_time(struct date_time* dt)
{
	dt->second++;
	//minute switch
	if(dt->second == 60)
	{
		dt->second = 0;
		dt->minute++;
	}
	//hour switch
	if(dt->minute >= 60)
	{
		dt->minute = 0;
		dt->hour++;
	}
	if(strcmp(dt->ampm,"") && dt->hour > 12)
	{
		dt->hour = 1;
	}
	if(dt->hour == 24)
	{
		dt->hour = 0;
		dt->day++;
	}
	//12-hour clock AM PM switch
	if (dt->second == 0 && dt->minute==0 && dt->hour == 12)
	{

		if(!strcmp(dt->ampm, "AM"))
		{
			dt->ampm = "PM";
		}
		if(!strcmp(dt->ampm, "PM"))
		{	
			dt->ampm = "AM";
			dt->day++ ;
		}
	}
	
	//regular month switch
	if(dt->month == 1 || dt->month == 3 || dt->month == 5 || dt->month == 7 || dt->month == 8 || dt->month == 10 || dt->month == 12)
	{
		if(dt->day > 31)
		{
			dt->day = 1;
			dt->month++;
		}
	}
	else
	{
		if(dt->day > 30)
		{
			dt->day = 1;
			dt->month++;
		}
	}
	
	//February switch
	if(dt->month == 2)
	{
		if((dt->year % 4 == 0 && dt->year % 100 != 0) || (dt->year % 100 == 0 && dt->year % 400 == 0))
		{
			if(dt->day > 29)
			{
				dt->month++;
				dt->day = 1;
			}
		}
		else
		{
			if(dt->day == 29)
			{
				dt->month++;
				dt->day = 1;
			}
		}
	}
	//year switch
	if(dt->month == 13)
	{
		dt->month = 1;
		dt->year++;
		
	}
	
}


void set_dt(struct date_time* dt, int r, int c, int k)
{
	if(r == 0){
		if(c==0)
			dt->month = k*10;
		if(c == 1)
			dt->month += k;
		// c == 2 is '/'
		if(c == 3)
			dt->day = k*10;
		if(c == 4)
			dt->day += k;
		// c == 5 is '/'
		if (c == 6)
			dt->year = k*1000;
		if (c == 7)
			dt->year += k*100;
		if (c == 8)
			dt->year += k*10;
		if (c == 9)
			dt->year += k;
	}
	else{
		if(c==0)
			dt->hour = k*10;
		if(c == 1)
			dt->hour += k;
		// c == 2 is ':'
		if(c == 3)
			dt-> minute= k*10;
		if(c == 4)
			dt->minute += k;
			// c == 5 is ':'
		if (c == 6)
			dt->second = k*10;
		if (c == 7)
			dt->second += k;					
	}
}

void set_time(struct date_time* dt, char* key)
{
	set_format(dt,key);
	if(!strcmp(key, "1"))
	{
		put_lcd('1');
		set_dt(dt, lcd_row,lcd_column, 1 );
		lcd_column++;
	}
	if(!strcmp(key, "2"))
	{
		put_lcd('2');
		set_dt(dt, lcd_row,lcd_column, 2 );
		lcd_column++;
	}
	if(!strcmp(key, "3"))
	{
		put_lcd('3');
		set_dt(dt, lcd_row,lcd_column, 3 );
		lcd_column++;
	}
	if(!strcmp(key, "4"))
	{
		put_lcd('4');
		set_dt(dt, lcd_row,lcd_column, 4 );
		lcd_column++;
	}
	if(!strcmp(key, "5"))
	{
		put_lcd('5');
		set_dt(dt, lcd_row,lcd_column, 5 );
		lcd_column++;
	}
	if(!strcmp(key, "6"))
	{
		put_lcd('6');
		set_dt(dt, lcd_row,lcd_column, 6 );
		lcd_column++;
	}
	if(!strcmp(key, "7"))
	{
		put_lcd('7');
		set_dt(dt, lcd_row,lcd_column, 7 );
		lcd_column++;
	}
	if(!strcmp(key, "8"))
	{
		put_lcd('8');
		set_dt(dt, lcd_row,lcd_column, 8 );
		lcd_column++;
	}
	if(!strcmp(key, "9"))
	{
		put_lcd('9');
		set_dt(dt, lcd_row,lcd_column, 9);
		lcd_column++;
	}
	if(!strcmp(key, "0"))
	{
		put_lcd('0');
		set_dt(dt, lcd_row,lcd_column, 0 );
		lcd_column++;
	}
}

void set_format(struct date_time* dt, char* key)
{
	if(lcd_row == 1 && lcd_column == 8)
	{
		if(!strcmp(key, "A"))
			dt->ampm = "AM";
		if(!strcmp(key, "B"))
			dt->ampm = "PM";
		pos_lcd(1,9);	
		puts_lcd2(dt->ampm);
		pos_lcd(1,8);
		return;
	}
	
	if(lcd_row == 0 && lcd_column == 10)
	{
		lcd_row = 1;
		lcd_column = 0;
		return;
	}
	
	if (lcd_column == 2 || lcd_column == 5)
	{
		if(lcd_row == 0)
			put_lcd('/');
		if(lcd_row == 1)
			put_lcd(':');
		lcd_column++;
		return;
	}
}

bool check_valid(struct date_time* dt)
{
	return dt->month <= 12 && dt->day <= 31 && dt->hour <= 12
			&& dt->minute <= 60 && dt->second <=60;
}

void show_time(struct date_time* dt)
{
	pos_lcd(0,0);
	char buffer[20];
	sprintf(buffer, "%02d/%02d/%04d", dt->month, dt->day, dt->year);
	puts_lcd2(buffer);
	
	pos_lcd(1,0);
	char buffer2[20];
	sprintf(buffer2,"%02d:%02d:%02d %s", dt->hour, dt->minute,dt->second, dt->ampm);
	puts_lcd2(buffer2);	
}

void show_alarmtime(int h, int m, const char* ap)
{
	pos_lcd(1,0);
	char buffer[20];
	sprintf(buffer, "%02d:%02d %s", h, m, ap);
	puts_lcd2(buffer);
}
void set_alarmtime(int* h, int* m, int k)
{
	if(lcd_column == 0)
		*h = k*10;
	if(lcd_column == 1)
		*h += k;
	// lcd_column == 2 is ':'
	if(lcd_column == 3)
		*m = k*10;
	if(lcd_column == 4)
		*m += k;

}

void get_alarmtime(int* h, int* m, char* ap, char* key)
{
	if (lcd_column == 2)
	{
		put_lcd(':');
		lcd_column++;
	}
	else if (lcd_column == 5)
	{
		if(!strcmp(key, "A"))
		{
			*ap = "AM";
			lcd_column++;
		}
		if(!strcmp(key, "B"))
		{
			*ap = "PM";
			lcd_column++;
		}
	}
	else
	{
		if(!strcmp(key, "1"))
		{
			put_lcd('1');
			set_alarmtime(h, m, 1 );
			lcd_column++;
		}
		if(!strcmp(key, "2"))
		{
			put_lcd('2');
			set_alarmtime(h, m, 2);
			lcd_column++;
		}
		if(!strcmp(key, "3"))
		{
			put_lcd('3');
			set_alarmtime(h, m, 3);
			lcd_column++;
		}
		if(!strcmp(key, "4"))
		{
			put_lcd('4');
			set_alarmtime(h, m, 4);
			lcd_column++;
		}
		if(!strcmp(key, "5"))
		{
			put_lcd('5');
			set_alarmtime(h, m, 5);
			lcd_column++;
		}
		if(!strcmp(key, "6"))
		{
			put_lcd('6');
			set_alarmtime(h, m, 6);
			lcd_column++;
		}
		if(!strcmp(key, "7"))
		{
			put_lcd('7');
			set_alarmtime(h, m, 7);
			lcd_column++;
		}
		if(!strcmp(key, "8"))
		{
			put_lcd('8');
			set_alarmtime(h, m, 8);
			lcd_column++;
		}
		if(!strcmp(key, "9"))
		{
			put_lcd('9');
			set_alarmtime(h, m, 9);
			lcd_column++;
		}
		if(!strcmp(key, "0"))
		{
			put_lcd('0');
			set_alarmtime(h, m, 0 );
			lcd_column++;
		}
	}
}

bool check_alarmvalid(int h, int m)
{
	return h <= 12 && m <= 59;
}

bool check_alarm(struct date_time* dt, int h, int m, char* ap)
{
	return (dt->hour == h) && (dt->minute == m) && (dt->ampm == ap) && (dt->second >= 1) && (dt->second <= 30);
}

void time_transfer(struct date_time* dt) //12 - 24.
{
		
		pos_lcd(0,0);
		char buffer[20];
		sprintf(buffer, "%02d/%02d/%04d", dt->month, dt->day, dt->year);
		puts_lcd2(buffer);
		
		pos_lcd(1,0);
		char buffer2[20];	
		if(!strcmp(dt->ampm, "PM"))
		{	
			dt->ampm = "";	
			if(dt->hour != 12)
				dt->hour+= 12;
			sprintf(buffer2,"%02d:%02d:%02d", dt->hour, dt->minute,dt->second,dt->ampm);
		}
		else
		{			
			dt->ampm = "";
			if (dt->hour == 12)
				dt->hour = 0;					
					
			sprintf(buffer2,"%02d:%02d:%02d", dt->hour, dt->minute,dt->second,dt->ampm);
		}
		puts_lcd2(buffer2);
}

void wait_avr2(unsigned short msec)
{
	TCCR0 = 2;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 8) * 0.0001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void play_note(float freq, float duration)
{
	int k = freq* duration;
	float TH = 1.0 /(2 * freq);
	float TL = TH;
	int i;
	for(i = 0; i < k; ++i)
	{
		SET_BIT(PORTA, 3);
		wait_avr2(round(TH*10000));
		
		CLR_BIT(PORTA, 3);
		wait_avr2(round(TL*10000));
		
	}
	return;
}

int main(void)
{
	char reset = 0;
	char set_alarm = 0;
	char alrm_set = 0;
    /* Replace with your application code */
	ini_avr();	
	ini_lcd();
	clr_lcd();
	struct date_time dt;
	dt.ampm = "PM"; dt.day = 28; dt.month = 2; dt.year = 1700;
	dt.hour = 11; dt.minute = 49; dt.second = 50;

	int alarm_h, alarm_min = 0;
	char* alarm_ampm;
	

    while (1) 
    {
		
		const char* key = get_key(encode_key());
		wait_avr(300);
		
		if(reset)
		{
			set_time(&dt,key);
			put_lcd(' ');
			wait_avr(100);
			pos_lcd(lcd_row,lcd_column);
			put_lcd('*');
			pos_lcd(lcd_row,lcd_column);
		}
		else if(!reset)
		{
			show_time(&dt);
			wait_avr(650);
			change_time(&dt);
			if (alrm_set && check_alarm(&dt, alarm_h, alarm_min, alarm_ampm))
			{
				SET_BIT(DDRA, 1);
				SET_BIT(PORTA, 1);
				SET_BIT(DDRA, 3);
				play_note(261, 0.3);
				CLR_BIT(PORTA, 3);				
				wait_avr(100);
				
				show_time(&dt);

				SET_BIT(DDRA, 3);
				play_note(261, 0.3);
				CLR_BIT(PORTA, 3);
				wait_avr(100);
				CLR_BIT(PORTA,1);
				
				SET_BIT(DDRA, 3);
				play_note(261, 0.3);
				CLR_BIT(PORTA, 3);
				wait_avr(100);
				
				change_time(&dt);
				
				
			}
		}
		
		if (!strcmp(key, "D"))
		{
		
			clr_lcd();
			if(reset){
				puts_lcd2("Setting finished");
			}
			else
				puts_lcd2("Setting mode");
			if (!check_valid(&dt))
			{
				dt.day = 23; dt.month = 8; dt.hour = 0;
				dt.minute = 0; dt.hour = 0; dt.second = 0;
			}
		
			
			wait_avr(500);
			clr_lcd();
			show_time(&dt);
		
			reset = ~reset;		
	
			lcd_column = 0;
			lcd_row = 0;
		}
		
		
		if (!strcmp(key, "C"))
		{
			set_alarm = 1;
			int count = 0;
			
			alarm_h = dt.hour;
			alarm_min = dt.minute;
			alarm_ampm = dt.ampm;
			lcd_column = 0;
			
			while (set_alarm)
			{
				if (count == 0)
				{
					clr_lcd();
					puts_lcd2("Setting Alarm");
					wait_avr(500);
				}
				else if (lcd_column <= 6)
				{
					clr_lcd();
					show_alarmtime(alarm_h, alarm_min, alarm_ampm);
					put_lcd(' ');
					wait_avr(100);
					pos_lcd(1,lcd_column);
					put_lcd('*');
					pos_lcd(1,lcd_column);
					wait_avr(300);
					const char* set_key1 = get_key(encode_key());
					get_alarmtime(&alarm_h, &alarm_min, &alarm_ampm, set_key1);
					if (lcd_column == 6)
					{
						clr_lcd();
						show_alarmtime(alarm_h, alarm_min, alarm_ampm);
						lcd_column = 8;
					}
				}
				else if (!check_alarmvalid(alarm_h, alarm_min))
				{
					const char* set_key2 = get_key(encode_key());
					if (!strcmp(set_key2, "C"))
					{
						clr_lcd();
						puts_lcd2("Set Failed");
						wait_avr(500);
						set_alarm = 0;
					}
				}
				else
				{
					const char* set_key2 = get_key(encode_key());
					if (!strcmp(set_key2, "C"))
					{
						clr_lcd();
						puts_lcd2("Alarm Set");
						wait_avr(500);
						set_alarm = 0;
						alrm_set = 1;
					}
				}
				count++;
			}
		}
		if (!strcmp(key, "#"))
		{
			alrm_set = 0;
		}
		
	}
	return 1;
}

