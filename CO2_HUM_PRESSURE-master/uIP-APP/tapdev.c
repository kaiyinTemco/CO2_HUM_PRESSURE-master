/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: tapdev.c,v 1.8 2006/06/07 08:39:58 adam Exp $
 */	    
#include "tapdev.h"
#include "uip.h"
#include "enc28j60.h"
#include <stdio.h>
#include "udp_scan.h"
#include "modbus.h"
#include "tcpip.h"

//#define DHCP_ENABLE

//���ڹ̶�IP��ַ���ش򿪺��IP���ã�������û�������
#define UIP_DRIPADDR0   192
#define UIP_DRIPADDR1   168
#define UIP_DRIPADDR2   0
#define UIP_DRIPADDR3   111

//MAC��ַ,����Ψһ
const u8 mymac[6]={0x04, 0x02, 0x35, 0xaF, 0x00, 0x01};	//MAC��ַ
																				  
//��������Ӳ����������MAC��ַ 
//����ֵ��0��������1��ʧ�ܣ�
u8 tapdev_init(void)
{   	 
	u8 i, res = 0;
#ifndef	DHCP_ENABLE
	uip_ipaddr_t ipaddr;
#endif
	
	res = ENC28J60_Init((u8*)modbus.mac_addr);	//��ʼ��ENC28J60					  
	//��IP��ַ��MAC��ַд�뻺����

 	for (i = 0; i < 6; i++)
	{
		uip_ethaddr.addr[i] = modbus.mac_addr[i];
	}

    //ָʾ��״̬:0x476 is PHLCON LEDA(��)=links status, LEDB(��)=receive/transmit
 	//PHLCON��PHY ģ��LED ���ƼĴ���	    
	ENC28J60_PHY_Write(PHLCON, 0x0476);
	uip_init();							//uIP��ʼ��	
	if(modbus.ghost_ip_mode == 0)	
	{
		uip_ipaddr(ipaddr, modbus.ip_addr[0], modbus.ip_addr[1], modbus.ip_addr[2], modbus.ip_addr[3]);	//���ñ�������IP��ַ
		uip_sethostaddr(ipaddr);					    
		uip_ipaddr(ipaddr, modbus.gate_addr[0], modbus.gate_addr[1], modbus.gate_addr[2], modbus.gate_addr[3]); 	//��������IP��ַ(��ʵ������·������IP��ַ)
		uip_setdraddr(ipaddr);						 
		uip_ipaddr(ipaddr, modbus.mask_addr[0], modbus.mask_addr[1], modbus.mask_addr[2], modbus.mask_addr[3]);	//������������
		uip_setnetmask(ipaddr);
		
		uip_ipaddr(uip_hostaddr_submask, modbus.ip_addr[0], modbus.ip_addr[1], modbus.ip_addr[2],255);

	delay_ms(1) ;
	uip_listen(HTONS(modbus.listen_port));       // 10000, modbustcp
//	tcp_app_init();
	udp_scan_init();
	bip_Init();
}
else
{
//	printf("dhcpc\r\n");
	dhcpc_init(modbus.mac_addr, 6);
	dhcpc_request();	

}
	
	return res;	
}

//��ȡһ������  
uint16_t tapdev_read(void)
{	
	return  ENC28J60_Packet_Receive(MAX_FRAMELEN, uip_buf);
}

//����һ������  
void tapdev_send(void)
{
	ENC28J60_Packet_Send(uip_len, uip_buf);
}
