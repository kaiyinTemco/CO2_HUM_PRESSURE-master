#include "stmflash.h"
#include "delay.h"
#include "usart.h"
//u16 iapbuf[1024]; 

iapfun jump2app; 
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0    //set Main Stack value
    BX r14
}
//V1.1�޸�˵��
//������STMFLASH_Write������ַƫ�Ƶ�һ��bug.
//////////////////////////////////////////////////////////////////////////////////

//����STM32��FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;//д���������.
	FLASH->KEYR = FLASH_KEY2;
}

//flash����
void STMFLASH_Lock(void)
{
  FLASH->CR |= 1 << 7;//����
}

//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res = FLASH->SR; 
	if(res & (1 << 0))
		return 1;		    //æ
	else if(res & (1 << 2))
		return 2;			//��̴���
	else if(res & (1 << 4))
		return 3;			//д��������
	return 0;				//�������
}

//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res = STMFLASH_GetStatus();
		if(res != 1)
			break;//��æ,����ȴ���,ֱ���˳�.
		delay_us(1);
		time--;
	 }while(time);
	
	 if(time == 0)
		 res = 0xff;//TIMEOUT
	 return res;
}

//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res = 0;
	res = STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms    
	if(res == 0)
	{ 
		FLASH->CR |= 1 << 1;			//ҳ����
		FLASH->AR = paddr;				//����ҳ��ַ 
		FLASH->CR |= 1<<6;				//��ʼ����		  
		res = STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res != 1)//��æ
		{
			FLASH->CR &= ~(1 << 1);//���ҳ������־.
		}
	}
	return res;
}

//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res = STMFLASH_WaitDone(0XFF);	 
	if(res == 0)//OK
	{
		FLASH->CR |= 1 << 0;//���ʹ��
		*(vu16*)faddr = dat;//д������
		res = STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res != 1)//�����ɹ�
		{
			FLASH->CR &= ~(1 << 0);//���PGλ.
		}
	} 
	return res;
}
 
//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

u8 STMFLASH_BYTE(u32 faddr)
{
	if(faddr%2 == 0)
		return  *(vu16*)faddr & 0xff; 
	else 
		return (*(vu16*)(faddr-1)>>8) & 0xff;	
}

#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i = 0; i < NumToWrite; i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr, pBuffer[i]);
	    WriteAddr += 2;									//��ַ����2.
	}  
}
 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 

//void STMFLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)	
//{
//	u32 secpos;		//������ַ
//	u16 secoff;		//������ƫ�Ƶ�ַ(16λ�ּ���)
//	u16 secremain;	//������ʣ���ַ(16λ�ּ���)	   
// 	u16 i;    
//	u32 offaddr;	//ȥ��0X08000000��ĵ�ַ
//	if(WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
//		return;		//�Ƿ���ַ
//	
//	STMFLASH_Unlock();							//����
//	offaddr = WriteAddr - STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
//	secpos = offaddr / STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
//	secoff = (offaddr%STM_SECTOR_SIZE) / 2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
//	secremain = STM_SECTOR_SIZE / 2 - secoff;	//����ʣ��ռ��С   
//	if(NumToWrite <= secremain)
//		secremain = NumToWrite;					//�����ڸ�������Χ
//	
//	while(1) 
//	{	
//		STMFLASH_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);	//������������������
//		for(i = 0; i < secremain; i++)			//У������
//		{
//			if(STMFLASH_BUF[secoff + i] != 0XFFFF)
//				break;							//��Ҫ����  	  
//		}
//		
//		if(i < secremain)						//��Ҫ����
//		{
//			STMFLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);	//�����������
//			for(i = 0; i < secremain; i++)		//����
//			{
//				STMFLASH_BUF[i + secoff] = pBuffer[i];	  
//			}
//			STMFLASH_Write_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);	//д����������  
//		}
//		else
//		{
//			STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);	//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
//		}
//		
//		if(NumToWrite == secremain)
//		{
//			break;	//д�������
//		}
//		else		//д��δ����
//		{
//			secpos++;								//������ַ��1
//			secoff = 0;								//ƫ��λ��Ϊ0 	 
//		   	pBuffer += secremain;  					//ָ��ƫ��
//			WriteAddr += secremain*2;				//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
//		   	NumToWrite -= secremain;				//�ֽ�(16λ)���ݼ�
//			if(NumToWrite >(STM_SECTOR_SIZE / 2))
//				secremain = STM_SECTOR_SIZE / 2;	//��һ����������д����
//			else
//				secremain = NumToWrite;				//��һ����������д����
//		}	 
//	};
//	
//	STMFLASH_Lock();	//����
//}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);	//��ȡ2���ֽ�.
		ReadAddr += 2;									//ƫ��2���ֽ�.	
	}
}


void STMFLASH_MUL_Read(u32 ReadAddr, u8 *pBuffer, u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{ 
		pBuffer[i] = STMFLASH_BYTE(ReadAddr);	//��ȡ1���ֽ�. 
		ReadAddr ++;			//ƫ��2���ֽ�.	
	}
}
//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr, u16 WriteData)   	
{
//	STMFLASH_Write(WriteAddr, &WriteData, 1);	//д��һ���� 
}



void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];//�����2K�ֽ�
	if(appxaddr < STM32_FLASH_BASE || (appxaddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
		return;		//�Ƿ���ַ
// 	STMFLASH_Unlock();							//���� 
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//ƫ��2���ֽ�
		STMFLASH_BUF[i++]=temp;	    
		if(i==1024)
		{
			i=0;
// 			STMFLASH_Write(fwaddr,iapbuf,1024);	
 			STMFLASH_Write_NoCheck(fwaddr, STMFLASH_BUF,1024);
			fwaddr+=2048;//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)
	{
// 		STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.
 		STMFLASH_Write_NoCheck(fwaddr, STMFLASH_BUF,i);
	}
// 	STMFLASH_Lock();	//����
}
//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
//	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
//	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
//	}
}		 

