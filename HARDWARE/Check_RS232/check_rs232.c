#include "check_rs232.h"

check_bit_type_t caculate_tansfer_check_bit(Powerstep1_contorl_motor_command_t p)
{
	check_bit_type_t check;
	uint8_t buf[100];
	memset(buf,0,sizeof(buf));
	memcpy(buf,&(p.CommandPowerStep1),sizeof(p.CommandPowerStep1));
	int len=sizeof(p.CommandPowerStep1);
	int sum=0,i=0;

	for(i=0;i<len;i++)
	{
		sum+=buf[i];
	}
	check.H=sum/128;
	check.L=sum%128;

	return check;
}
