1. ���io_control_xxx.revx.lib�� ud_str.h  controls.h �����ļ�
ud_str.h  -- ������input��output�Ľṹ
control.h -- ���Լ��Ĵ�������Ҫ��ӵĺ���

extern void Set_Input_Type(uint8_t point);  
extern unsigned int get_input_raw(uint8_t point);
extern void set_output_raw(uint8_t point,unsigned int value);
extern uint8_t get_max_output(void);
extern uint8_t get_max_input(void);
extern void map_extern_output(uint8_t point);
extern unsigned int conver_by_unit_5v(uint8 sample);
extern unsigned int conver_by_unit_10v(uint8 sample);
extern unsigned int conver_by_unit_custable(uint8_t point,uint8 sample);
extern unsigned long get_high_spd_counter(uint8_t point);

������ӵĺ������ԣ� �ο�io_example.c

