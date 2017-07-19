#ifndef _TCP_MODBUS_H
#define _TCP_MODBUS_H
#include "string.h"
#include "stm32f10x.h"
//IO��������	 
#include "bitmap.h"
#include "uip.h"
#include "modbus.h"



void tcp_server_appcall(void) ;
void tcp_server_aborted(void) ;
void tcp_server_timedout(void) ;
void tcp_server_closed(void) ;
void tcp_server_connected(void) ;
void tcp_server_acked(void) ;
void tcp_server_senddata(void); 
	
extern u8 tcp_server_databuf[];   	//�������ݻ���	  
extern u8 tcp_server_sta;				//�����״̬
//[7]:0,������;1,�Ѿ�����;
//[6]:0,������;1,�յ��ͻ�������
//[5]:0,������;1,��������Ҫ����

extern u8 tcp_server_sendbuf[];
extern u16 tcp_server_sendlen;



void tcp_server_appcall(void) ;




#endif
