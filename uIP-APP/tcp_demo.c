#include "stm32f10x.h"
#include "usart.h"	 		   
#include "uip.h"	    
#include "enc28j60.h"
#include "httpd.h"
#include "tcp_demo.h"
#include "tcp_modbus.h"
//TCPӦ�ýӿں���(UIP_APPCALL)
//���TCP����(����server��client)��HTTP����
void tcp_appcall(void)
{	
//	switch(uip_conn->lport)		//���ؼ����˿�80��1200
//	{
//		case HTONS(80):
//			httpd_appcall(); 
//			break;
//		case HTONS(modbus.listen_port):
//		    tcp_server_appcall(); 
//			break;
//		default:						  
//		    break;
//	}
//	if(uip_conn->lport ==  HTONS(80)) httpd_appcall(); 
//	else 
		if(uip_conn->lport ==  HTONS(modbus.listen_port))
	tcp_server_appcall(); 
//	switch(uip_conn->rport)		//Զ������1400�˿�
//	{
//	    case HTONS(1400):
//			tcp_client_demo_appcall();
//			break;
//		default: 
//			break;
//	}   
}

//��ӡ��־��
void uip_log(char *m)
{
//	printf("uIP log:%s\r\n", m);
}
