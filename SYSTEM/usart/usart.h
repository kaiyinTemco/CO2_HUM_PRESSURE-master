#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f10x.h"

//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	


	  	


void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);

#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


#endif


