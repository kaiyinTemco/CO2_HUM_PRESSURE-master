#include "modbus.h"
#include "ud_str.h"
#include "controls.h"
//#include "inputs.h"
static uint16_t analog_buffer[MAX_AI_CHANNEL] ;

// ����input_type[point]������Ӧ��Ӳ��
void Set_Input_Type(uint8_t point)
{
	
}


// ����input��ԭʼADCֵ��10bit�� �������10λ����Ҫ����Ӧת��
uint16_t get_input_raw(uint8_t point)
{
//	return (modbus.input[point]+1)/4;
	return 0;//AD_Value[point]/4 ;
//	return 512 ;
}

// ����input��ԭֵ
// if digital output, 0 - off, 1000 - on
// if analog ouput, 0 - 10v ��Ӧ 0-1023
#ifdef T38AI8AO6DO
void set_output_raw(uint8_t point,uint16_t value)
{
//	output_raw[point] = value;
	inputs[value].value = value ;
}
#endif

// ��adcֵת����5v��range��ʾ
// �����input����ģ�鲻��Ҫ�޸�
unsigned int conver_by_unit_5v(uint8_t sample)
{	
//	sample =  ( sample * 5000L ) >> 10;	
	return ( sample * 5000L ) >> 10;		
}


// ��adcֵת����10v��range��ʾ
// �����input����ģ�鲻��Ҫ�޸�
unsigned int conver_by_unit_10v(uint8_t sample)
{
	return  ( sample * 10000L ) >> 10;

}

// ��adcֵת����customer table��range��ʾ
// �����input����ģ�鲻��Ҫ�޸�
unsigned int conver_by_unit_custable(uint8_t point,uint8_t sample)
{	
	if(input_type[point] == INPUT_V0_5)
	{
			return  ( sample * 50L ) >> 10;		
	}
	else if(input_type[point] == INPUT_I0_20ma)
	{
		return ( 20000L * sample ) >> 10; 
	}
	else if(input_type[point] == INPUT_0_10V)
	{
		return (sample * 10000) >> 10;
	}	
	else if(input_type[point] == INPUT_THERM)
	{
		return get_input_value_by_range( inputs[point].range, inputs[point].value );
	}
	return 0;	
}


// �������input��Ŀ
uint8_t get_max_input(void)
{	
	return MAX_INS;
}
// �������output��Ŀ
#ifdef T38AI8AO6DO
uint8_t get_max_output(void)
{	
	return MAX_OUTS;
}
#endif
// if��high speed ���ܣ�����high_spd_counter
uint32_t get_high_spd_counter(uint8_t point)
{
	//return (high_spd_counter[point] + high_spd_counter_tempbuf[point]) * 1000;
	return inputs[point].value ;

}

 unsigned int Filter(uint8_t channel,uint16_t input)
{
	// -------------FILTERING------------------
	s16  siDelta = 0;
	u16  uiResult = 0;
	u8  i = 0;
  	u16  uiTemp = 0;
	i = channel;
	uiTemp = input;  
	siDelta = uiTemp - analog_buffer[i];    //compare new reading and old reading

	// If the difference in new reading and old reading is greater than 5 degrees, implement rough filtering.
  if (( siDelta >= 100 ) || ( siDelta <= -100)) // deg f
	  	analog_buffer[i] = analog_buffer[i] + (siDelta >> 1);//1 
 			
	// Otherwise, implement fine filtering.
	else
	{			
	  analog_buffer[i] = (u32)analog_buffer[i]*inputs[i].filter;
	  analog_buffer[i] += (u32)uiTemp;
	  analog_buffer[i] = (u16)(analog_buffer[i]/(inputs[i].filter));			 	 
	}
	uiResult = analog_buffer[i]; 
 

	return uiResult;	
}
