#include "config.h"
#include "myiic.h"
 
//static u32 Read_DLVR()
//{ 
//	u32 buf_in = 0 ;
//	u8 i ; 
//	SPI_CS = 1;
//	delay_us(5);	
//	SPI_CS = 0;
//	delay_us(5);
//	SPI_SCK = 0;//output_low(SPI_SCK);
//	delay_us(5);
//	for(i=0; i<32;i++)
//	{
//		SPI_SCK = 1;//output_HIGH(SPI_SCK);
//		delay_us(5);
//		if(SPI_MISO) buf_in |= (0x80>>i);
//		SPI_SCK = 0;//output_low(SPI_SCK); 
//		delay_us(5);
//	} 
//	SPI_CS = 1;
//	return buf_in; 
//}
//static void Write_DLVR(u32 buf_out)
//{
//	u8 i = 32;
//	u8 temp ;
//	SPI_CS = 0;
//	delay_us(5);
//	SPI_SCK = 0;//output_LOW(SPI_SCK);
//	delay_us(5);
//	do
//	{    
//		temp = ( buf_out & 0x80 );
//		if(temp >0 ) SPI_MISO = 1;//output_high(SPI_MOSI);
//		else         SPI_MISO = 0;//output_low(SPI_MOSI);
//		delay_us(5);
//		SPI_SCK = 1;//output_HIGH(SPI_SCK);
//		delay_us(5);
//		SPI_SCK = 0;//output_LOW(SPI_SCK);
//		buf_out <<=1;
//		delay_us(5);
//	} while( --i != 0 );
//	SPI_MISO = 1;//output_high(SPI_MOSI);

//	SPI_CS = 1;
//}


////����IIC��ʼ�ź�
//static void IIC_Start(void)
//{ 
//	IIC_SDA = 1;	  	  
//	IIC_SCL = 1;
//	delay_us(5);
// 	IIC_SDA = 0;	//START:when CLK is high,DATA change form high to low 
//	delay_us(5);
//	IIC_SCL = 0;	//ǯסI2C���ߣ�׼�����ͻ�������� 
//}
//	  
////����IICֹͣ�ź�
//static void IIC_Stop(void)
//{ 
//	IIC_SCL = 0;
//	IIC_SDA = 0;	//STOP:when CLK is high DATA change form low to high
// 	delay_us(5);
//	IIC_SCL = 1; 
//	IIC_SDA = 1;	//����I2C���߽����ź�
//	delay_us(5);							   	
//}

////�ȴ�Ӧ���źŵ���
////����ֵ��1������Ӧ��ʧ��
////        0������Ӧ��ɹ�
//static u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime = 0;  
//	IIC_SDA = 1;
//	delay_us(5);	   
//	IIC_SCL = 1;
//	delay_us(5);	 
//	while(IIC_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime > 250)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	}
//	IIC_SCL = 0;	//ʱ�����0 	   
//	return 0;  
//}
// 
////����ACKӦ��
//static void IIC_Ack(void)
//{
//	IIC_SDA = 0;
//	IIC_SCL = 0;  
//	delay_us(5);
//	IIC_SCL = 1;
//	delay_us(5);
//	IIC_SCL = 0;
//	IIC_SDA = 1;
//}

////������ACKӦ��		    
//static void IIC_NAck(void)
//{
//	IIC_SDA = 1;
//	delay_us(5);
//	IIC_SCL = 0; 
//	delay_us(5);
//	IIC_SCL = 1;
//	delay_us(5);
//	IIC_SCL = 0;
//}
//					 				     
////IIC����һ���ֽ�
////���شӻ�����Ӧ��
////1����Ӧ��
////0����Ӧ��			  
//static void IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
////	SDA_OUT(); 	    
//    IIC_SCL = 0;		//����ʱ�ӿ�ʼ���ݴ���
//    for(t = 0; t < 8; t++)
//    {              
//        IIC_SDA = (txd & 0x80) >> 7;
//        txd <<= 1; 	  
//		delay_us(5);   //��TEA5767��������ʱ���Ǳ����
//		IIC_SCL = 1;
//		delay_us(5); 
//		IIC_SCL = 0;	
//		delay_us(5);
//    }	 
//}
//	    
////��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
//static u8 IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i, receive = 0;

////	SDA_IN();			//SDA����Ϊ����
//// 	IIC_SDA = 1;
//	delay_us(5);
//    for(i = 0; i < 8; i++)
//	{
//        IIC_SCL = 0; 
//        delay_us(5);
//		IIC_SCL = 1;
//		delay_us(5); 
//        receive <<= 1;
//        if(IIC_SDA)receive++;   
//		
//    }
//						 
//    if(!ack)
//        IIC_NAck();//����nACK
//    else
//        IIC_Ack(); //����ACK
//		   
//    return receive;
//}

#define DLVR_ADDR	0x28
/*
bit Start_Sensor_All(void)
{
	IIC_Start();
	i2c_pic_write((DLVR_ADDR<<1)|0x01);
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
}
static bit Start_PRESSURE_ONLY(void)
{
	IIC_Start();
	IIC_Send_Byte((DLVR_ADDR<<1)&0xfe);
	IIC_Wait_Ack();
	IIC_Stop(); 
	return 0;
}
*/
static u32 Read_DLVR_Sensor(void)
{
	u32 temp_buf = 0;
//  Start_Sensor_All(); 
//	Start_PRESSURE_ONLY(); 
	IIC_Start();
	IIC_Send_Byte((DLVR_ADDR<<1)|0x01);
	IIC_Wait_Ack();
	temp_buf = IIC_Read_Byte(1);
	temp_buf =temp_buf << 8;
	temp_buf |= IIC_Read_Byte(1);
	temp_buf =temp_buf << 8;
	temp_buf |= IIC_Read_Byte(1);
	temp_buf =temp_buf << 8;
	temp_buf |= IIC_Read_Byte(0);
	IIC_Stop(); 
	return temp_buf;
}

//static void Write_Pressure_Sensor(u32 temp_buf)
//{
//	u8 temp = 0;
//	Start_Sensor_All();
//	Start_PRESSURE_ONLY(); 
//	IIC_Start();
//	i2c_pic_write((DLVR_ADDR<<1)&0xfe);
//	if(IIC_Wait_Ack()) return;
//	temp = (u8)(temp_buf >> 24);
//	i2c_pic_write(temp);
//	if(IIC_Wait_Ack()) return;
//	temp = (u8)(temp_buf >> 16);
//	i2c_pic_write(temp);
//	if(IIC_Wait_Ack()) return;
//	temp = (u8)(temp_buf >> 8);
//	i2c_pic_write(temp);
//	if(IIC_Wait_Ack()) return;
//	temp_buf =(u8)temp_buf;
//	i2c_pic_write(temp);
//	if(IIC_Wait_Ack()) return;
//	IIC_Stop();  
//} 
//void pressure_sensor_initial(void)
//{
//	Pressure_sensor.status = 0;
//	Pressure_sensor.pressure = 0;
//	Pressure_sensor.temperature = 0;
//	Pressure_sensor.pressure_ad = 0;
//	Pressure_sensor.temperature_ad = 0;
//}

#define			NORMAL		0 
s16 Get_DLVR_Pressure(void)
{
	u32 buf_temp; 
	u16 data_buf[3];
	s16 sitemp;
	buf_temp = Read_DLVR_Sensor();
	
	data_buf[0]= (buf_temp >>5) & 0x7ff;     		//temperature ad
	data_buf[1] = (buf_temp >> 16) & 0x3fff;	 	//pressure ad
	data_buf[2] = (u8)(buf_temp >> 30) & 0x03; 		//status;n
	Pressure.sensor_status  = data_buf[2];
	if(Pressure.sensor_status == NORMAL)
	{
		sitemp = ((s32)data_buf[1] -8192)* 200 / 16384;              //Get the pressure value
// 		test[1] = (s32)data_buf[0] * 200 / 2047 - 50;				 //Get the pressure sensor's temperature
		return sitemp;
	}
// 	test[0] = buf_temp  ;
	return 10000;
	
}
 

