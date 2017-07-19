#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

void SPI1_Init(void);							//��ʼ��SPI1��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);				//SPI1���߶�дһ���ֽ�

void SPI2_Init(void);							//��ʼ��SPI1��
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI1�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);				//SPI1���߶�дһ���ֽ�
		 
#endif
