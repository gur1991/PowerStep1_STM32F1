#include "m6e_apply.h"
#include "uart_config.h"
#include "config.h"

static TMR_Reader r, *rp=NULL;
static TMR_ReadPlan plan;
static uint8_t *gantennaList=NULL;
static uint8_t gantennaCount=0x0;
static uint8_t gbuffer[20];

static TMR_TagReadData trd;
char epcStr[128];
char dataStr[128];


static Chemical_reagent_Info_type RfidReadInfo[10];
static uint8_t index_get=0;


void Clear_Storage_Info_Rfid(void)
{
	memset(RfidReadInfo, 0, sizeof(RfidReadInfo));
	index_get=0;
}	


void Destory_M6e_Config(void)
{
#if USE_AUTOMATIC_INJECTION_BOARD	
	TMR_destroy(rp);
#endif	
}	




uint8_t Init_M6e_Config(TMR_Region region, uint32_t Rpowerdbm,uint32_t Wpowerdbm)
{
 
  TMR_Status ret=0;
#if USE_AUTOMATIC_INJECTION_BOARD	
	uint32_t powerdbm; 
  TMR_TRD_MetadataFlag metadata = TMR_TRD_METADATA_FLAG_ALL;

	Load_RFID_Uart_Config();	
  
  rp = &r;
	printf("start. \r\n");
  ret = TMR_create(rp, "tmr:///com1");
  printf("create ret:%d \r\n",ret);
	
	gbuffer[0] = 1;
	gbuffer[1] = 2;
  gantennaList = gbuffer;
  gantennaCount = 2;

  ret = TMR_connect(rp);
	printf("connect ret:%d \r\n",ret);
	
	powerdbm=2500;
	ret = TMR_paramSet(rp, TMR_PARAM_RADIO_READPOWER, &powerdbm);
	ret = TMR_paramGet(rp, TMR_PARAM_RADIO_READPOWER, &powerdbm);
	printf("******* ReadPowerdbm:%d \r\n",powerdbm);
	
	powerdbm=3000;
	ret = TMR_paramSet(rp, TMR_PARAM_RADIO_WRITEPOWER, &powerdbm);
	ret = TMR_paramGet(rp, TMR_PARAM_RADIO_WRITEPOWER, &powerdbm);
	printf("******* WritePowerdbm:%d \r\n",powerdbm);
	
	region=TMR_REGION_PRC;
  ret = TMR_paramSet(rp, TMR_PARAM_REGION_ID, &region);
	ret = TMR_paramGet(rp, TMR_PARAM_REGION_ID, &region);
	printf("******* region:%d \r\n",region);

  
	ret = isAntDetectEnabled(rp, gantennaList);
	printf("******* gantennaList:%d \r\n",ret);
	
	printf("read type:%d \r\n",rp->readerType);

  if (rp->readerType == TMR_READER_TYPE_SERIAL)
  {
			ret = TMR_paramSet(rp, TMR_PARAM_METADATAFLAG, &metadata);
	}
  ret = TMR_RP_init_simple(&plan, gantennaCount, gantennaList, TMR_TAG_PROTOCOL_GEN2, 1000);


	Exit_RFID_Uart_Config();	
	
#endif	
	return ret;		

}



uint8_t M6e_Read_Info(void)
{
  TMR_Status ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
	int i=0;
	Load_RFID_Uart_Config();	
	
	ret = TMR_paramSet(rp, TMR_PARAM_READ_PLAN, &plan);
  printf("****read plan:ret:0x%x\r\n",ret);
	
	
	printf("**** start TMR_read.\r\n");
	ret = TMR_read(rp, 500, NULL);
  printf("**** read:ret:0x%X\r\n",ret);
	
	ret=TMR_hasMoreTags(rp);
	printf("**** TMR_hasMoreTags:ret:0x%X\r\n",ret);

	Clear_Storage_Info_Rfid();
	while (TMR_SUCCESS == TMR_hasMoreTags(rp))
  {
		printf("start next tag. \r\n");
    ret = TMR_getNextTag(rp, &trd);
		printf("getNextTag:ret:0x%x\r\n",ret);
		TMR_bytesToHex(trd.tag.epc, trd.tag.epcByteCount, epcStr);
    printf("EPC:%s %d", epcStr,trd.tag.epcByteCount);
		
		TMR_bytesToHex(trd.data.list, trd.data.len, dataStr);
	  printf("  data(%d): %s\n", trd.data.len, dataStr);
	
		
		RfidReadInfo[i].used=1;
		RfidReadInfo[i].epcStringCount=trd.tag.epcByteCount>sizeof(RfidReadInfo[i].epcString)?sizeof(RfidReadInfo[i].epcString):trd.tag.epcByteCount;
		memcpy(RfidReadInfo[i].epcString, trd.tag.epc, RfidReadInfo[i].epcStringCount);

		if(trd.data.len){
			  RfidReadInfo[i].userBlankCount=trd.data.len>sizeof(RfidReadInfo[i].userBlankData)?sizeof(RfidReadInfo[i].userBlankData):trd.data.len;
				memcpy(RfidReadInfo[i].userBlankData, trd.data.list, RfidReadInfo[i].userBlankCount);
		}
		i++;	
	}
	index_get=i;
  Exit_RFID_Uart_Config();	
	
#endif	
	return ret;
}


uint8_t M6e_Magic_Read_Rfid_Info(uint8_t* length)
{
	
	uint8_t ret=0;	

#if USE_AUTOMATIC_INJECTION_BOARD		
	ret=M6e_Read_Info()==0?0:1;
	if(!ret){
		*length=index_get;
	}else{
		*length=0;
	}	
#endif	
	
	return ret;
}	

Chemical_reagent_Info_type M6e_Magic_Get_One_Rfid_Info(uint8_t index)
{
	return RfidReadInfo[index];
}


uint8_t Get_EPC_String(uint8_t*length, uint8_t* epc)
{
	uint8_t ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
	TMR_TagReadData trd;
	memset(epcStr, 0, sizeof(epcStr));
  Load_RFID_Uart_Config();	
	
	ret = TMR_paramSet(rp, TMR_PARAM_READ_PLAN, &plan);
	ret = TMR_read(rp, 1000, NULL);
	if(!TMR_hasMoreTags(rp))
	 {
		    ret = TMR_getNextTag(rp, &trd);
		    TMR_bytesToHex(trd.tag.epc, trd.tag.epcByteCount, epcStr);
		    *length=2*trd.tag.epcByteCount;
		    memcpy(epc, epcStr, *length);
	}

	Exit_RFID_Uart_Config();	
#endif
	
	return ret;
}


uint8_t M6e_Magic_Write_Rfid_EPC(uint8_t* epcData,uint8_t epcByteCount)
{
	  TMR_Status ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
    TMR_TagData epc;
    TMR_TagOp tagop;
		Load_RFID_Uart_Config();	
    epc.epcByteCount = epcByteCount;
    memcpy(epc.epc, epcData, epc.epcByteCount * sizeof(uint8_t));
    ret = TMR_TagOp_init_GEN2_WriteTag(&tagop, &epc);
    ret = TMR_executeTagOp(rp, &tagop, NULL, NULL);
    Exit_RFID_Uart_Config();	
#endif
	
	  return ret;
}

uint8_t M6e_Magic_Write_Rfid_Blank(uint8_t wordCount,uint16_t* writeData)
{
		TMR_Status ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD		
    TMR_TagFilter filter, *pfilter = &filter;

    TMR_TagOp writeop, readop;
    TMR_TagOp* tagopArray[8];
    TMR_TagOp_List tagopList;
    TMR_TagOp listop;
    TMR_uint8List response;
    uint8_t responseData[16];
    TMR_uint16List writeArgs;
	  
		
		Load_RFID_Uart_Config();	
	
    pfilter = NULL;
    {
      writeArgs.list = writeData;
      writeArgs.len = writeArgs.max = wordCount;

      TMR_TagOp_init_GEN2_WriteData(&writeop, TMR_GEN2_BANK_USER, 0, &writeArgs);
      TMR_TagOp_init_GEN2_ReadData(&readop, TMR_GEN2_BANK_USER, 0, writeArgs.len);

      tagopList.list = tagopArray;
      tagopList.len = 0;

      tagopArray[tagopList.len++] = &writeop;
      tagopArray[tagopList.len++] = &readop;


      listop.type = TMR_TAGOP_LIST;
      listop.u.list = tagopList;

      response.list = responseData;
      response.max = sizeof(responseData) / sizeof(responseData[0]);
      response.len = 0;

      ret = TMR_executeTagOp(rp, &listop, pfilter, &response);
      //TMR_bytesToHex(response.list, response.len, dataStr);
    }
    tagopList.list = NULL;
    Exit_RFID_Uart_Config();	
#endif
		
		return ret;
}


