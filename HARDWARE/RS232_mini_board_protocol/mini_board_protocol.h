#ifndef __MINI_BORAD_PROTOCL_H
#define __MINI_BORAD_PROTOCL_H
#include "sys.h"
#include "uart_command_control_protocol.h"
#include <stdbool.h>
#include "uart5.h"
#include "check_rs232.h"

uint8_t Master_Mini_Board_Set_Degree(mini_set_single_temperature_degree_type_t data);

uint8_t Master_Mini_Board_Get_Degree(mini_get_single_temperature_degree_type_t data,uint32_t* degree,uint8_t* status);


#endif


