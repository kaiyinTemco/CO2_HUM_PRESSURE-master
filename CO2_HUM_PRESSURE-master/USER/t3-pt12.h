#ifndef _T3_PT12_H
#define _T3_PT12_H


#include <math.h>
#include "define.h"
#include "modbus.h"



#ifdef T3PT12

#define	RTD_R0	100


#define	RPS		4
//#define	RP1		91
//#define	RP2		120
//#define	RP3		130
//#define	RP4		150

#define	RP1		150
#define	RP2		130
#define	RP3		120
#define	RP4		91


typedef union 
{
	u8 qauter_temp_C[4];
	u16 half_temp_C[2];
	float temp_C;
				
} _TEMP_VALUE_;

extern unsigned char   int_float ;
extern float Rs[RPS]	;	
extern _TEMP_VALUE_ _temp_value[12],_temp_offset_value[12];
extern _TEMP_VALUE_ _offset_rc[12] ;
//extern _TEMP_VALUE_ _rtd_ABC[30];
extern signed int temperature[12];
extern _TEMP_VALUE_   linear_K , linear_B;

s32 PT12_FITER(u8 channel,u32 input, u8 filter);
float data_convert(float input_data , u8 resol_bit ) ;
void min2method(float *K, float *B, u8 PointNum, float *Xbuf, float *Ybuf) ;
float get_rtd_temperature(long rtd_ad , unsigned char channel) ;
void update_temperature	(void);
#endif

#endif
