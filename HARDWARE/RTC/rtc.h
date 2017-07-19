#ifndef __RTC_H
#define __RTC_H	    

#include "stm32f10x.h"

//ʱ��ṹ��
#pragma pack(push)
#pragma pack(1)
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8 w_month;
	vu8 w_date;
	vu8 week;		 
}_calendar_obj;	
#pragma pack(pop)
extern _calendar_obj calendar;					//�����ṹ��
//extern _calendar_obj calendar_ghost;					//�����ṹ��
extern u8 calendar_ghost_enable;
extern u8 Rtc_Sec_It;

u8 RTC_Init(void);      						//ƽ��,�����ж�
u8 Time_Adjust(void);	//����ʱ��	


#endif
