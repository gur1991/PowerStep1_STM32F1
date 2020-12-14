#ifndef PROCESS_SHARK_RFID_TYPE_H_
#define PROCESS_SHARK_RFID_TYPE_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

#ifndef u8
typedef unsigned char u8;
#endif

typedef enum Chemical_Produce_Name
{
	BUFFER_A=0xB1,
	BUFFER_B=0xB2,
	BUFFER_C=0xB3,
	DILUENT=0xD1,

	COLUNM=0xC1,
	FILITER=0xF1,
}Chemical_Produce_Name_Type;


typedef enum Chemical_Produce_Method
{
	M_Hba1c = 0xA1,
}Chemical_Produce_Method_Type;



typedef struct
{
	u8 year;
	u8 month;
	u8 day;
}Chemical_Produce_Period_Type;

//8
typedef struct
{
	u8 sn[9];
	u8 volume[2];
	u8 method;
	//u8 nor;
}Chemical_Produce_Regent_Type;

typedef struct
{
	u8 sn[9];
	u8 total[2];
	u8 method;
	//u8 nor;
}Chemical_Produce_Colunm_Type;

typedef struct
{
	u8 sn[9];
	u8 total[2];
	u8 nor;
}Chemical_Produce_Filiter_Type;

typedef union 
{
	Chemical_Produce_Regent_Type regent;
	Chemical_Produce_Colunm_Type colunm;
	Chemical_Produce_Filiter_Type filiter;
}Chemical_Produce_Info_Type;


#define SEPAX_CHECK 176

//16 BYTE
typedef struct Sepax_EPC
{
	//u8 check;//1
	u8 type;//1
	Chemical_Produce_Info_Type info;//12
	Chemical_Produce_Period_Type date;//3
}Sepax_EPC_Type_T;





#ifdef  __cplusplus
}
#endif
#endif /* PROCESS_SHARK_SERVICE_EXECUTE_THREAD_SHARK_WORK_SHARK_TYPE_H_ */
