#include "delay.h"

#ifdef SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif

static u8  fac_us = 0;	//us��ʱ������			   
static u16 fac_ms = 0;	//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
   
//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
#ifdef SYSTEM_SUPPORT_OS
	u32 reload;
#endif
 	SysTick->CTRL &= ~(1 << 2);				//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us = SYSCLK;// / 8;					//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	    
#ifdef SYSTEM_SUPPORT_OS
	reload = SYSCLK / 8;					//ÿ���ӵļ������� ��λΪK	   
	reload *= 1000000 / configTICK_RATE_HZ;	//����configTICK_RATE_HZ�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms = 1000 / configTICK_RATE_HZ;		//����ucos������ʱ�����ٵ�λ	   
// 	SysTick->CTRL |= 1 << 1;				//����SYSTICK�ж�
	SysTick->LOAD = reload; 				//ÿ1/configTICK_RATE_HZ���ж�һ��	
	SysTick->CTRL |= 1 << 0;				//����SYSTICK    
#else
	fac_ms = (u16)fac_us * 1000;			//��os��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    

#ifdef SYSTEM_SUPPORT_OS
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = nus * fac_us; 					//��Ҫ�Ľ�����	  		 
	tcnt = 0;

// 	vTaskSuspendAll();						//��ֹos���ȣ���ֹ���us��ʱ
	taskENTER_CRITICAL();
	told = SysTick->VAL;        			//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)
				tcnt += told - tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;	    

			told = tnow;
			if(tcnt >= ticks)				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
				break;
		}  
	};
// 	xTaskResumeAll();						//����os����
	taskEXIT_CRITICAL(); 									    
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)	//���os�Ѿ�������
	{		  
		if(nms >= fac_ms)					//��ʱ��ʱ�����ucos������ʱ������ 
		{
   			vTaskDelay(nms / fac_ms);		//ucos��ʱ
		}
		nms %= fac_ms;						//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ 
}

#else	//����osʱ
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus * fac_us;	//ʱ�����	  		 
	SysTick->VAL = 0x00;			//��ռ�����
	SysTick->CTRL = 0x01;			//��ʼ���� 	 
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));	//�ȴ�ʱ�䵽�� 
	  
	SysTick->CTRL = 0x00;			//�رռ�����
	SysTick->VAL = 0X00;			//��ռ�����	 
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * fac_ms;	//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;				//��ռ�����
	SysTick->CTRL = 0x01;				//��ʼ����  
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));	//�ȴ�ʱ�䵽��   

	SysTick->CTRL = 0x00;				//�رռ�����
	SysTick->VAL = 0X00;				//��ռ�����	  	    
} 
#endif
