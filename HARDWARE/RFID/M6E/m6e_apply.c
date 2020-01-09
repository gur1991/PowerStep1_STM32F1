#include "m6e_apply.h"
#include "uart_config.h"
#include "config.h"

#define M6E_APPLY_DEBUG 1

static TMR_Reader r, *rp=NULL;
static TMR_ReadPlan plan;
static TMR_TagReadData trd;

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




uint32_t Init_M6e_Config(TMR_Region region, uint32_t Rpowerdbm,uint32_t Wpowerdbm)
{
 
  TMR_Status ret=0;
#if USE_AUTOMATIC_INJECTION_BOARD
	
	static uint8_t *gantennaList=NULL;
	static uint8_t gantennaCount=0x0;
	static uint8_t gbuffer[20];	
	uint32_t powerdbm; 
  TMR_TRD_MetadataFlag metadata = TMR_TRD_METADATA_FLAG_ALL;

	Load_RFID_Uart_Config();	
  
  rp = &r;
#if M6E_APPLY_DEBUG	
	printf("start. \r\n");
#endif	
  ret = TMR_create(rp, "tmr:///com1");
#if M6E_APPLY_DEBUG	
  printf("create ret:%d \r\n",ret);
#endif	
	gbuffer[0] = 1;
	gbuffer[1] = 2;
  gantennaList = gbuffer;
  gantennaCount = 2;

  ret = TMR_connect(rp);
#if M6E_APPLY_DEBUG
	printf("connect ret:%d \r\n",ret);
#endif	
	//powerdbm=2500;
	powerdbm=Rpowerdbm;
	ret = TMR_paramSet(rp, TMR_PARAM_RADIO_READPOWER, &powerdbm);
	ret = TMR_paramGet(rp, TMR_PARAM_RADIO_READPOWER, &powerdbm);
#if M6E_APPLY_DEBUG	
	printf("******* ReadPowerdbm:%d \r\n",powerdbm);
#endif	
	//powerdbm=3000;
	powerdbm=Wpowerdbm;
	ret = TMR_paramSet(rp, TMR_PARAM_RADIO_WRITEPOWER, &powerdbm);
	ret = TMR_paramGet(rp, TMR_PARAM_RADIO_WRITEPOWER, &powerdbm);
#if M6E_APPLY_DEBUG	
	printf("******* WritePowerdbm:%d \r\n",powerdbm);
#endif	
	//region=TMR_REGION_PRC;
  ret = TMR_paramSet(rp, TMR_PARAM_REGION_ID, &region);
	ret = TMR_paramGet(rp, TMR_PARAM_REGION_ID, &region);
#if M6E_APPLY_DEBUG	
	printf("******* region:%d \r\n",region);
#endif
  
	ret = isAntDetectEnabled(rp, gantennaList);
#if M6E_APPLY_DEBUG	
	printf("******* gantennaList:%d \r\n",ret);
	
	printf("read type:%d \r\n",rp->readerType);
#endif
  if (rp->readerType == TMR_READER_TYPE_SERIAL)
  {
			ret = TMR_paramSet(rp, TMR_PARAM_METADATAFLAG, &metadata);
	}
  ret = TMR_RP_init_simple(&plan, gantennaCount, gantennaList, TMR_TAG_PROTOCOL_GEN2, 1000);


	Exit_RFID_Uart_Config();	
	
#endif	
	return ret;		

}



uint32_t M6e_Read_Info(void)
{
  TMR_Status ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD
	char dataStr[128];
	char epcStr[128];	
	int i=0;
	Load_RFID_Uart_Config();	
	
	ret = TMR_paramSet(rp, TMR_PARAM_READ_PLAN, &plan);
#if M6E_APPLY_DEBUG 
	printf("****read plan:ret:0x%x\r\n",ret);
	
	
	printf("**** start TMR_read.\r\n");
#endif	
	ret = TMR_read(rp, 500, NULL);
#if M6E_APPLY_DEBUG  
	printf("**** read:ret:0x%X\r\n",ret);
#endif	
	ret=TMR_hasMoreTags(rp);
#if M6E_APPLY_DEBUG	
	printf("**** TMR_hasMoreTags:ret:0x%X\r\n",ret);
#endif
	Clear_Storage_Info_Rfid();
	while (TMR_SUCCESS == TMR_hasMoreTags(rp))
  {
#if M6E_APPLY_DEBUG		
		printf("start next tag. \r\n");
#endif    
		ret = TMR_getNextTag(rp, &trd);
#if M6E_APPLY_DEBUG		
		printf("getNextTag:ret:0x%x\r\n",ret);
#endif		
		TMR_bytesToHex(trd.tag.epc, trd.tag.epcByteCount, epcStr);
#if M6E_APPLY_DEBUG    
		printf("EPC:%s %d", epcStr,trd.tag.epcByteCount);
#endif		
		TMR_bytesToHex(trd.data.list, trd.data.len, dataStr);
#if M6E_APPLY_DEBUG	  
		printf("  data(%d): %s\n", trd.data.len, dataStr);
#endif	
		
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


uint32_t M6e_Magic_Read_Rfid_Info(uint8_t* length)
{
	
	uint32_t ret=0;	

#if USE_AUTOMATIC_INJECTION_BOARD		
	//ret=M6e_Read_Info()==0?0:1;
	ret=M6e_Read_Info();
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


uint32_t Get_EPC_String(uint8_t*length, uint8_t* epc)
{
	uint32_t ret=0;
	
#if USE_AUTOMATIC_INJECTION_BOARD
	char epcStr[128];	
	
	memset(epcStr, 0, sizeof(epcStr));
  Load_RFID_Uart_Config();	
	
	ret = TMR_paramSet(rp, TMR_PARAM_READ_PLAN, &plan);
	ret = TMR_read(rp, 500, NULL);
	if(!TMR_hasMoreTags(rp))
	 {
		    ret = TMR_getNextTag(rp, &trd);
		    TMR_bytesToHex(trd.tag.epc, trd.tag.epcByteCount, epcStr);
		    *length=2*trd.tag.epcByteCount;
				
				if(*length>48)*length=48;
		    memcpy(epc, epcStr, *length);
					
	}

	Exit_RFID_Uart_Config();	
#endif
	
	return ret;
}


uint32_t M6e_Magic_Write_Rfid_EPC(uint8_t* epcData,uint8_t epcByteCount)
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

uint32_t M6e_Magic_Write_Rfid_Blank(uint8_t wordCount,uint16_t* writeData)
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


