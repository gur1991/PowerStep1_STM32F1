#ifndef __CHECK_RS232_H
#define __CHECK_RS232_H
#include "uart_command_control_protocol.h"
typedef struct
{
	uint8_t H;
	uint8_t L;
}check_bit_type_t;

check_bit_type_t caculate_tansfer_check_bit(Powerstep1_contorl_motor_command_t p);
#endif
