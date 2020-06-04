#include "check_rs232.h"


//CommandPowerStep1 type 这两个部分才是校验内容
check_bit_type_t caculate_tansfer_check_bit(Powerstep1_contorl_motor_command_t p)
{
	static check_bit_type_t check;
	memset(&check,0,sizeof(check));
	Powerstep1_contorl_motor_command_t t;
	memset(&t,0,sizeof(t));
	uint8_t buf[100];
	memset(buf,0,sizeof(buf));
	
	t.type=p.type;
	t.CommandPowerStep1=p.CommandPowerStep1;
	memcpy(buf,&t,sizeof(t));
	
	
	uint32_t len=sizeof(t);
	uint32_t sum=0,i=0;

	for(i=0;i<len;i++)
	{
		sum+=buf[i];
	}
	check.H=sum/128;
	check.L=sum%128;

	return check;
}
