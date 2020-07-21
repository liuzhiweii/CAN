/*********************************************************************
* ��Ȩ���� (C)2005, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ� uhf.c
* �ļ���ʶ��
* ����ժҪ�� �ײ�Э�����ģ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�zdx  ft
* ������ڣ� 2006-09-26
*
* �޸ļ�¼1�� ����ΪLLRP 20100113 zdx
* �޸ļ�¼2�� ���µ�RUP  20111201
* �޸ļ�¼3��
**********************************************************************/

/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "uhfFpgaRegDef.h"
#include "uhf.h"
#include "uhf_epc.h"
#include "uhf_iso6b.h"
#include "uhf_trace.h"
#include "uhf_rf.h"
#include "EPC.h"
#include "ISO6B.h"
#include "GB.h"
#include "common.h"
#include <zmq.h>
#include "parson.h"


#if (!USP_IS_DISTRIBUTED)
#include "Board.h"
#endif

#include "FpgaCtl.h"
#include "uhf_dbg.h"
//#include "uhf_time.h"

//volatile WORD16* g_pvUSPDrvFpgaVirtStartAddr0;

/***********************************************************
*                 �ļ��ڲ�ʹ�õĺ�                        *
***********************************************************/
#define UHF_REQUEST_MAX_COUNT     (15)
#define UHF_RF_AMP_ON   		  (1)  			/* 1). �򿪹��ŵ�Դ */
#define UHF_RF_AMP_OFF  		  (0)  			/*     �رչ��ŵ�Դ */
#define UHF_AMP_NO_NEED_SLEEP     (0)
#define UHF_AMP_NEED_SLEEP        (1)



/*******************  ZMQ variables begin *********************/

static void *Req_to_CMM = NULL;
static void *rdp_rep_socket = NULL;
static void *Sub_to_CMM  = NULL;
static void *context    = NULL;

/*******************  ZMQ variables end *********************/




/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/
/* ��Ƶ�������� */

/* ��Ƶ���� */
T_UhfCtrl     g_tUhfCtrl;

extern T_GBCtl    g_tGBCtrl;
extern T_ISO6BCtl g_tISO6BCtrl;
extern T_EpcCtl   g_tEpcCtrl;

static BYTE   g_aucDesc[USP_OSS_ST_DESC_SIZE];    /*��������������*/
static BYTE   g_aucTraceBuffer[TRACEBUFFERLEN];
static T_UhfRfConfigPara g_tRfConfigPara;
WORD32 g_dwUSPUhfVirTagWorkFlag = TRUE;

/***********************************************************
 *                     ���ر���                            *
***********************************************************/
/* Rcp���̵�ǰ״̬ */
static WORD32 g_dwRcpState = UHF_STATE_INIT;

/* ������Ϣ���� */
static WORD32 g_dwRcvMsgBuf[UHF_MAX_MSG_SIZE * 2];

/* ������Ϣ���� */
static WORD32 g_dwSendMsgBuf[UHF_MAX_MSG_SIZE / 4];

/* ������Ϣ���� */
static WORD32 g_dwRfRcvMsgBuf[UHF_MAX_MSG_SIZE / 4];

/* ������Ϣ���� */
static WORD32 g_dwRfSendMsgBuf[UHF_MAX_MSG_SIZE / 4];


static T_MsgFunDef g_tMsgProcTable[] = USP_UHF_MSG_PROC_TABLE;
static WORD32 uhf_InitStateprocess(T_Msg *ptRcvMsg);
static WORD32 uhf_WorkStateProcess(T_Msg *ptRcvMsg);
static WORD32 uhf_SendMsg(WORD16 wSendModule, T_Msg *ptSndMsg);
extern WORD32 uhfRf_ConvChannelToFreq(WORD16 wChannelNo, WORD16 wHopTableID);
extern void   uhfCliProc(T_Msg *ptRecvMsg);
extern WORD32 uhf_epc_getModeParas(WORD16 wModeIndex, WORD16 wAntIndex);
extern WORD32 InitUhfSIGALRM(void);
static void uhf_protocol_para_init();



/**********************************************************************
* �������ƣ�static void  uhf_SetDefautCfg()
* ������������ʼ��uhfģ���������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/10     RUP     �Զ���          ����
************************************************************************/
static void  uhf_SetDefautCfg()
{
    WORD16 wLoop = 0;

    g_tUhfCtrl.dwWorkFreq = 0;
    g_tUhfCtrl.wAntIndex = 1;
    g_tUhfCtrl.wWorkChannel = 0;

    for (wLoop = 1; wLoop <= READER_ANTENNA_COUNT; wLoop++)
    {
        g_tUhfCtrl.atRfAntCfg[wLoop].ucAntStause = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].ucModeIndex = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEPCReadRssiCfg.ucTurnOnFilter = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEPCReadRssiCfg.wRSSIThreshHold = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tISO6BReadRssiCfg.ucTurnOnFilter = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tISO6BReadRssiCfg.wRSSIThreshHold = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEPCWriteRssiCfg.ucTurnOnFilter = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEPCWriteRssiCfg.wRSSIThreshHold = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tISO6BWriteRssiCfg.ucTurnOnFilter = 0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tISO6BWriteRssiCfg.wRSSIThreshHold = 0;
        /* Fix Me lxc */
        /* RSSI Needed? */
        g_tUhfCtrl.atRfAntCfg[wLoop].tEpcUserDefPara.eDataEncodeType = DATA_ENCODE_TYPE_MILLER2;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEpcUserDefPara.eModulateType = Modulate_TYPE_PR;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEpcUserDefPara.wForDataRate = 80;
        g_tUhfCtrl.atRfAntCfg[wLoop].tEpcUserDefPara.wRevDataRate = 160;
        g_tUhfCtrl.atRfAntCfg[wLoop].tGBUserDefPara.eDataEncodeType = DATA_ENCODE_TYPE_FM0;
        g_tUhfCtrl.atRfAntCfg[wLoop].tGBUserDefPara.eModulateType = Modulate_TYPE_DSB;
        g_tUhfCtrl.atRfAntCfg[wLoop].tGBUserDefPara.wForDataRate = 80;
        g_tUhfCtrl.atRfAntCfg[wLoop].tGBUserDefPara.wRevDataRate = 320;

    }
}

/**********************************************************************
* �������ƣ�void uhf_protocol_para_init()
* ������������ʼ��Э����صĲ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static void uhf_protocol_para_init()
{
    uhf_SetDefautCfg();

    uhf_Epc_InitCtrlStruct();

    uhf_ISO6B_InitCtrlStruct();

    uhf_GB_InitCtrlStruct();

    /*��0 �ŵ���Ƶģʽ���ʼ��EPC Э�����*/
    uhf_epc_getModeParas(0, 1);

    /*��0 �ŵ���Ƶģʽ���ʼ��GB Э�����*/
    uhf_GB_getModeParas(0, 1);

    /*��0 �ŵ���Ƶģʽ���ʼ��EPC Э�����*/
    uhf_ISO6B_Cal_Mode_Para(0);

    /* ��FPGA����Э��������� */
    uhf_ProtoHwInit();
}

/**********************************************************************
* �������ƣ�uhf_initRfCfgParas
* ������������ʼ��������صĲ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

static void uhf_initRfCfgParas()
{
    WORD32 dwIndex = 0;
    T_UhfRfConfigPara     *ptRfConfigPara    = &g_tRfConfigPara;
    T_UhfRfAntConfigPara  *ptAntRfConfigPara = NULL;


    ptRfConfigPara->wRcvDatt = 0x3f;
    ptRfConfigPara->wLKCLSwitch = 1;
    ptRfConfigPara->wLowNoEn =  0; /*Ĭ�Ϲر�*/

    for (dwIndex = 0; dwIndex < READER_ANTENNA_COUNT; dwIndex++)
    {
        DEBUG_MSG("set ant %d cfgs\r\n", dwIndex);

        ptAntRfConfigPara = &ptRfConfigPara->tAntConfigPara[dwIndex];

        /* �����׷����� */
        ptAntRfConfigPara->wAntMode = 0;
        ptAntRfConfigPara->wAgcDatt = 0x3f;
        /*����LBT���� Ŀǰû����չ�ӿ�*/
        ptAntRfConfigPara->wAntLBT = 0;

        ptAntRfConfigPara->aucPower[0] = 0;
        ptAntRfConfigPara->aucPower[1] = 0xff;
        ptAntRfConfigPara->aucPower[2] = 0xff;
        ptAntRfConfigPara->aucPower[3] = 0xff;

        ptAntRfConfigPara->wRevFilterSwitch = 0;

        ptAntRfConfigPara->tFreqCtrl.wFreqTableID = 0;
        ptAntRfConfigPara->tFreqCtrl.wFreqWorkType = 0;
        ptAntRfConfigPara->tFreqCtrl.wFixFreqChannel = 0;


    }
}
/**********************************************************************
* �������ƣ�void UHF_Init(void)
* ����������Rcp ���̵ĳ�ʼ������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 usp_uhf_init()
{
    DEBUG_MSG("UHF:init begin\r\n");

    /* ��ʼ��FPGA������FPGA */
    uhf_mac_Init();

    /* ��ʼ��ģʽ���Э��ʱ��� */
    uhf_protocol_para_init();

    /*��ʼ���տ��źŷ���״̬ */
    uhf_TxSet(STOP_FPGA_RF_TX);

    /* ��װ SIG �źŴ����� */
    InitUhfSIGALRM();

    uhf_FPGAIntEnCtrl(FPGA_INT_ENABLE);

    uhf_initRfCfgParas();
#if (!USP_IS_DISTRIBUTED)
    usp_uhf_rf_init();/*��������Ժ�Ҫ�ƶ�����Ƶ����ȥ��*/
#endif
    DEBUG_MSG("UHF: Init end \r\n");
    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�void usp_uhf_reset(void)
* ����������Rcp ���̵ĳ�ʼ������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 usp_uhf_reset()
{
    DEBUG_MSG("UHF:reset begin\r\n");


    uhf_mac_Init();
    /* ��FPGA����Э��������� */
    uhf_ProtoHwInit();
    /*��ʼ���տ��źŷ���״̬ */
    uhf_TxSet(STOP_FPGA_RF_TX);

    uhf_FPGAIntEnCtrl(FPGA_INT_ENABLE);

#if (!USP_IS_DISTRIBUTED)
    uhfRf_Reset();/*��������Ժ�Ҫ�ƶ�����Ƶ����ȥ��*/
#endif
    DEBUG_MSG("UHF: Init end \r\n");
    return USP_SUCCESS;
}
#if 0

/**********************************************************************
* �������ƣ�static void ChangePriority(void)
* �����������޸Ľ��̵����ȼ�
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/22    V0.0.1       zdx         ����
************************************************************************/
static void uhf_SndPowerOnSuccess()
{
    T_Msg *ptSndMsg = (T_Msg *)g_dwSendMsgBuf;
    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgID = EV_USP_SCP_PowerOnSuccess;
    ptSndMsg->wSenderTno = USP_UHF_TNO;
    ptSndMsg->wReceiverTno = USP_OSS_SCP_TNO;
    ptSndMsg->wMsgLen = sizeof(T_USPOssScpPowerOnAck);
    ((T_USPOssScpPowerOnAck *)(ptSndMsg + 1))->ucRound = 1;
    uhf_SendMsg(USP_OSS_SCP_TNO, ptSndMsg);
}

/**********************************************************************
* �������ƣ�void   uhf_MsgProc(T_Msg *ptRcvMsg)
* ����������
* ���������wAction  1 ���� 0 ɱ��
* �����������
* �� �� ֵ��    ��
* ����˵����
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/22            V0.0.1         zdx         ����
***********************************************************************/
void  uhf_MsgProc(T_Msg *ptRcvMsg)
{
    if (NULL == ptRcvMsg)
    {
        DEBUG_MSG("UHF:ptRcvMsg is null\r\n");
        //LAPInsert(FILE_UHF, USP_UHF_RCVED_PARA_ERROR, 0, 0);
        return;
    }

    #if  USP_VIR_TAG_USE
    if(ptRcvMsg->wMsgID == EV_USP_VIR_TAG_StartWork)
    {
        usp_vir_fpga_initReg();
        g_dwUSPUhfVirTagWorkFlag = TRUE;
        return;
    }
    #endif

    switch (g_dwRcpState)
    {
            /*     ��ʼ״̬    */
        case UHF_STATE_INIT:
        {
            uhf_InitStateprocess(ptRcvMsg);
            break;
        }

        /* ����״̬   */
        case UHF_STATE_WORK:
        {
            uhf_WorkStateProcess(ptRcvMsg);
            break;
        }

        /* Ĭ�ϴ��� */
        default:
        {
            UHFAlertPrint("\r\nRCP: invalid STATE = %d !!!\r\n\r\n\r\n", g_dwRcpState);
            break;
        } /* default: */
    }
}
#endif

JSON_Object *parse_string_to_object(const char *strings,JSON_Value **string_value)
{
    JSON_Object *tmp_object=NULL;
    JSON_Value *tmp_value=NULL;
 
    /* ����Ĭ������ */
    tmp_value= json_parse_string(strings);
    *string_value = NULL;
	if(NULL==tmp_value)
	{
		printf("json_parse_string null pointer error \r\n");
		return NULL;
	}
   if(json_value_get_type(tmp_value) == JSONObject)
   {
      tmp_object = json_value_get_object(tmp_value);
      *string_value = tmp_value;
   }
   else
   {
       json_value_free(tmp_value);
       *string_value=NULL;
       return NULL;
   }
    return tmp_object;
}


void send_cfg_msg(const char *type,const char *action,const char *content)
{
    int rc=0;     
	char send_buf[1024]={0};
    JSON_Value *tmp_value=NULL;
    JSON_Object *tmp_object=NULL;
 
    printf("send_msg type=%s,action=%s,content=%s\r\n",type,action,content);
    
    tmp_value=json_value_init_object();
    tmp_object=tmp_value->value.object;
    json_object_set_string(tmp_object,"type",type);
    json_object_set_string(tmp_object,"action",action);
    if(NULL!=content)
    {
       json_object_set_string(tmp_object,"content",content);
    }
    memset(send_buf,0,sizeof(send_buf));
    rc=json_serialize_to_buffer(tmp_value,send_buf,sizeof(send_buf));
    /*�ͷ�*/
    json_value_free(tmp_value);
    printf("json_serialize_to_buffer send_buf = %s \r\n", send_buf);
    zmq_send(Req_to_CMM,send_buf,(strlen(send_buf)+1),0);
}

unsigned int cfg_proc(char * recv_msg)//cfgpara_json_to_local
{
	int 			rc       =0;
	const char 		*type    = NULL;
	const char 		*content = NULL;
	int 			result   = 0;
	JSON_Object 	*cur_object = NULL;
	JSON_Value  	*cur_value  = NULL;
	JSON_Object 	*cfg_object = NULL;
	JSON_Value 		*cfg_value  = NULL;
	JSON_Object 	*antenna_obj = NULL;
	unsigned char   tmpBuf[256];
	int wIndex;
	T_USPUHFReaderCfg  ptReaderCfg;
	cur_object=parse_string_to_object((char *)recv_msg,&cur_value);
	if(NULL==cur_object) 
	{
	   return;
	}
	type=json_object_get_string(cur_object,"type");

	//request cfg ack
	result=(int)json_object_get_number(cur_object,"result");
	content=json_object_get_string(cur_object,"content");
			
	cfg_object=parse_string_to_object(content,&cfg_value);
	if(NULL==cur_object) 
	{
	   DEBUG_MSG("deal_llrp_cfg_msg null pointer error 11\r\n");
	   json_value_free(cur_value);
	   return;
	}
	

	ptReaderCfg.wEPCModuleDepth		= (unsigned short int)json_object_get_number(cfg_object,"wEPCModuleDepth");
	ptReaderCfg.wISO6BModuleDepth	= (unsigned short int)json_object_get_number(cfg_object,"wISO6BModuleDepth");
	ptReaderCfg.wGBModuleDepth		= (unsigned short int)json_object_get_number(cfg_object,"wGBModuleDepth");
	for(wIndex=0;wIndex<4;wIndex++)
	{
		sprintf(tmpBuf,"Antenna%d",wIndex+1);
		antenna_obj=json_object_dotget_object(cfg_object,tmpBuf);
		if(NULL==antenna_obj)
		{
			DEBUG_MSG("antenna_obj null pointer error\r\n");
			return;
		}

	    /*RSSI �������ýӿ� */
		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.ucFilterSwitch =  
	    (unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6C.OpenReadFilter");
	    ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6C.RssiReadThreshold");
		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.ucFilterThreshold = 100;

	 	ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.ucFilterSwitch = 
	    (unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilterGB.OpenReadFilter");
		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilterGB.RssiReadThreshold");
		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.ucFilterThreshold = 100;

		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.ucFilterSwitch = 
	    (unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6B.OpenReadFilter");
		ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6B.RssiReadThreshold");	
	    ptReaderCfg.tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.ucFilterThreshold = 100;

		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.ucFilterSwitch = 
		(unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6C.OpenWriteFilter");
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6C.RssiWriteThreshold");	
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.ucFilterThreshold = 100;

		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.ucFilterSwitch = 
		(unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilterGB.OpenWriteFilter");		
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilterGB.RssiWriteThreshold");	
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.ucFilterThreshold = 100;

		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.ucFilterSwitch = 
		(unsigned char)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6B.OpenWriteFilter");		
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.wMaxReadRSSI = 
		(unsigned short int)json_object_dotget_number(antenna_obj,"ExtCfg.RssiFilter6B.RssiWriteThreshold");		
		ptReaderCfg.tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.ucFilterThreshold = 100;

		ptReaderCfg.tAntEpcUserDefPara[wIndex].eDataEncodeType  = 0;

		ptReaderCfg.tAntEpcUserDefPara[wIndex].eModulateType = 0;

 		ptReaderCfg.tAntEpcUserDefPara[wIndex].wForDataRate = 0;

 		ptReaderCfg.tAntEpcUserDefPara[wIndex].wPIEValue   = 0;

		ptReaderCfg.tAntEpcUserDefPara[wIndex].wRevDataRate = 0;
	}

	usp_uhf_set_cfg(&ptReaderCfg);

	json_value_free(cur_value);
	return;

	
}
#if 0
int cfg_proc(WORD32 * recvbuf)
{
	T_USPUHFReaderCfg *ptRFConfigRequestAck = NULL;
	ptRFConfigRequestAck = (T_USPUHFReaderCfg *)(recvbuf + 1);
	usp_uhf_set_cfg(ptRFConfigRequestAck);	
}

#endif

void zmq_close_socket(void)
{
	zmq_close(Req_to_CMM);
	zmq_close(rdp_rep_socket);
	zmq_close(Sub_to_CMM);
	zmq_ctx_term(context);
}

unsigned int zmq_init_socket(void)
{
	int ret;
	context = zmq_ctx_new ();
	if(NULL == context)
		return ERR;
	rdp_rep_socket =  zmq_socket(context, ZMQ_REP);
	DEBUG_MSG("create_socket\n");
	if(NULL == rdp_rep_socket)
	{
	    return ERR;
	}

	create_vir_fpga_socket(context);
	
	ret = zmq_bind(rdp_rep_socket, "ipc://rdp_rcp.ipc");
	if(ret < 0)
	{
		DEBUG_MSG("zmq_connect rdp_rep_socket err\n");
	    return ERR;
	}
	
	DEBUG_MSG("connect rdp_rep_socket ok   ret =  %d \n",ret);

	
	Req_to_CMM =  zmq_socket(context, ZMQ_REQ);;
	if(NULL == Req_to_CMM)
	{
	    return ERR;
	}

	ret = zmq_connect (Req_to_CMM, "tcp://127.0.0.1:5504");
	if(ret < 0)
	{
		DEBUG_MSG("zmq_connect Req_to_CMM err\n");
	    return ERR;
	}

	DEBUG_MSG("connect Req_to_CMM ok ret = %d \n",ret);



	Sub_to_CMM = zmq_socket (context, ZMQ_SUB);
	if(NULL == Sub_to_CMM)
	{
	    return ERR;
	}
	ret = zmq_connect (Sub_to_CMM, "tcp://127.0.0.1:5505");
	if(ret < 0)
	{
		DEBUG_MSG("zmq_connect Sub_to_CMM err\n");
	    return ERR;
	}

	DEBUG_MSG("connect Sub_to_CMM ok,ret = %d\n",ret);
	return SUCCESS;

}
void zmq_recv_message(void)
{
	zmq_pollitem_t items[3];

	int ret,recv_len;

	items[0].socket = Req_to_CMM;    
	items[0].events = ZMQ_POLLIN;
	items[1].socket = rdp_rep_socket;    
	items[1].events = ZMQ_POLLIN;
	items[2].socket = Sub_to_CMM;    
	items[2].events = ZMQ_POLLIN;

    memset(g_dwRcvMsgBuf,0,sizeof(g_dwRcvMsgBuf));
	
	 /* ѭ��������Ϣ������ */
    while ( 1 )
    {
        ret = zmq_poll(items, 3, -1);
		DEBUG_MSG("zmq_poll return , return %d \n", ret);
		
		if(items[0].revents & ZMQ_POLLIN)
			{                     
				recv_len = zmq_recv(Req_to_CMM, g_dwRcvMsgBuf, sizeof(g_dwRcvMsgBuf), 0);            
				if(recv_len < 0) 
					continue;            
				DEBUG_MSG("recv from Req_to_CMM socket %d bytes \n", recv_len);
				cfg_proc((char *)g_dwRcvMsgBuf);
				DEBUG_MSG("handle_message done\n");
				continue;
			}
		else if(items[1].revents & ZMQ_POLLIN)
			{                     
				recv_len = zmq_recv(rdp_rep_socket, g_dwRcvMsgBuf, sizeof(g_dwRcvMsgBuf), 0);            
				if(recv_len < 0) 
					continue;            
				DEBUG_MSG("recv from rep socket %d bytes \n", recv_len);
				uhf_WorkStateProcess((T_Msg *)g_dwRcvMsgBuf);
				DEBUG_MSG("handle_message done\n");
				continue;
			}
			else if(items[2].revents & ZMQ_POLLIN)
			{                     
				recv_len = zmq_recv(Sub_to_CMM, g_dwRcvMsgBuf, sizeof(g_dwRcvMsgBuf), 0);            
				if(recv_len < 0) 
					continue;            
				DEBUG_MSG("recv from Sub_to_CMM socket %d bytes \n", recv_len);
				cfg_proc((char *)g_dwRcvMsgBuf);
				DEBUG_MSG("handle_message done\n");
				continue;
			}
		else
			{
				DEBUG_MSG("poll error\n");
			}
   
	}
	
}



/**********************************************************************
* �������ƣ�void usp_uhf_entry(void)
* ����������Rcp ���̵���ں���
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
int  main(void)
{
    WORD32 dwRetVal = USP_SUCCESS;
    T_Msg *ptRcvMsg = NULL;
	T_Msg *ptSendMsg = NULL;
	int ret;
	ptSendMsg = (T_Msg *)g_dwSendMsgBuf;

	init_logm("rcp");

	ret = zmq_init_socket();
	if(SUCCESS != ret)
	{
		DEBUG_MSG("VMM create socket err!!\n");
	}	
	
   #if 1 
         usp_vir_fpga_initReg();
         g_dwUSPUhfVirTagWorkFlag = TRUE;
   #endif
	dwRetVal = usp_uhf_init();
	
	if (USP_SUCCESS == dwRetVal)
	{
		send_cfg_msg("uhf", "request", NULL);

	}
	
	zmq_recv_message();
	zmq_close_socket();
	
} /* void usp_uhf_entry(void) */

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_inventory
* ���������������Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/
WORD32 usp_uhf_epc_msg_inventory(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcInventory *ptEpcInventory = NULL;
    T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp = NULL;

    ptEpcInventory = (T_USPUHFEpcInventory *)(ptRcvMsg + 1);
    ptEpcTagInventoryResp = (T_USPUHFEpcInventoryRes *)(ptSndMsg + 1);
    usp_uhf_epc_inventory(ptEpcInventory, ptEpcTagInventoryResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcInventoryRes)
                        + ptEpcTagInventoryResp->tTagId.wTagWordLen * 2;
    UHFAlertPrint("UHF:epc inventory process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_msg_query
* ������������������Ϣ��װ����������㵽��ǩ��
                                 ��Ҫ�����ķ���quer����ֱ���յ�������֡
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_msg_query(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp = NULL;

    ptEpcTagInventoryResp = (T_USPUHFEpcInventoryRes *)(ptSndMsg + 1);
    usp_uhf_epc_query(ptEpcTagInventoryResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcInventoryRes)
                        + ptEpcTagInventoryResp->tTagId.wTagWordLen * 2;
    UHFAlertPrint("UHF:epc query process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_read
* ��������������Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/
WORD32 usp_uhf_epc_msg_read(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcRead *ptEpcRead = NULL;
    T_USPUHFEpcReadRes *ptEPCReadResp = NULL;

    ptEpcRead = (T_USPUHFEpcRead *)(ptRcvMsg + 1);
    ptEPCReadResp = (T_USPUHFEpcReadRes *)(ptSndMsg + 1);
    usp_uhf_epc_read(ptEpcRead, ptEPCReadResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcReadRes)
                        + ptEPCReadResp->wResultLength * 2;
    UHFAlertPrint("UHF:epc read process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_write
* ����������д��ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/
WORD32 usp_uhf_epc_msg_write(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcWrite *ptEpcWrite = NULL;
    T_USPUHFEpcWriteRes *ptEPCWriteResp = NULL;

    ptEpcWrite = (T_USPUHFEpcWrite *)(ptRcvMsg + 1);
    ptEPCWriteResp = (T_USPUHFEpcWriteRes *)(ptSndMsg + 1);
    usp_uhf_epc_write(ptEpcWrite, ptEPCWriteResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcWriteRes);
    UHFAlertPrint("UHF:epc write process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_lock
* ����������������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_epc_msg_lock(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcLock *ptEpcLock = NULL;
    T_USPUHFEpcLockRes *ptEPCLockResp = NULL;

    ptEpcLock = (T_USPUHFEpcLock *)(ptRcvMsg + 1);
    ptEPCLockResp = (T_USPUHFEpcLockRes *)(ptSndMsg + 1);
    usp_uhf_epc_lock(ptEpcLock, ptEPCLockResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcLockRes);
    UHFAlertPrint("UHF:epc lock process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_kill
* ����������ɱ����ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_epc_msg_kill(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcKill *ptEpcKill = NULL;
    T_USPUHFEpcKillRes *ptEPCKillResp = NULL;

    ptEpcKill = (T_USPUHFEpcKill *)(ptRcvMsg + 1);
    ptEPCKillResp = (T_USPUHFEpcKillRes *)(ptSndMsg + 1);
    usp_uhf_epc_kill(ptEpcKill, ptEPCKillResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcKillRes);
    UHFAlertPrint("UHF:epc lock process time ");
    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_blocklock
* ����������������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_epc_msg_blocklock(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcBlockLock      *ptEpcBlockLock = NULL;
    T_USPUHFEpcBlockLockRes   *ptEPCBlockLockResp = NULL;

    ptEpcBlockLock = (T_USPUHFEpcBlockLock *)(ptRcvMsg + 1);
    ptEPCBlockLockResp = (T_USPUHFEpcBlockLockRes *)(ptSndMsg + 1);
    usp_uhf_epc_blocklock(ptEpcBlockLock, ptEPCBlockLockResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcBlockLockRes);
    UHFAlertPrint("UHF:epc blocklock process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_readlock
* ������������������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_epc_msg_readlock(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcReadLock *ptEpcReadLock = NULL;
    T_USPUHFEpcReadLockRes *ptEPCReadLockResp = NULL;

    ptEpcReadLock = (T_USPUHFEpcReadLock *)(ptRcvMsg + 1);
    ptEPCReadLockResp = (T_USPUHFEpcReadLockRes *)(ptSndMsg + 1);
    usp_uhf_epc_readlock(ptEpcReadLock, ptEPCReadLockResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcReadLockRes);
    UHFAlertPrint("UHF:epc readlock process time ");
    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_ipjqt
* ����������impinj ״̬ת������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_epc_msg_ipjqt(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFEpcIPJQT   *ptEpcIPJQT = NULL;
    T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp = NULL;

    ptEpcIPJQT = (T_USPUHFEpcIPJQT *)(ptRcvMsg + 1);
    ptEpcIPJQTResp = (T_USPUHFEpcIPJQTRes *)(ptSndMsg + 1);

    usp_uhf_epc_ipjqt(ptEpcIPJQT, ptEpcIPJQTResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcIPJQTRes);
    UHFAlertPrint("UHF:epc ipjqt process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_msg_cfg
* ����������work״̬�½��յ���������Ϣ
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_msg_cfg(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFReaderCfg *ptRFConfigRequestAck = NULL;
    ptRFConfigRequestAck = (T_USPUHFReaderCfg *)(ptRcvMsg + 1);
    usp_uhf_set_cfg(ptRFConfigRequestAck);
    ptSndMsg->wMsgLen = 0;
    UHFAlertPrint("UHF:epc ipjqt process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_inventory
* ����������iso6b�����Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_inventory(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFISO6BInventory *ptISO6BInventory = NULL;
    T_USPUHFISO6BInventoryRes *ISO6BInventoryRes = NULL;

    ptISO6BInventory = (T_USPUHFISO6BInventory *)(ptRcvMsg + 1);
    ISO6BInventoryRes = (T_USPUHFISO6BInventoryRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_inventory(ptISO6BInventory, ISO6BInventoryRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BInventoryRes);
    ptSndMsg->mtype = 1;
    UHFAlertPrint("UHF:iso6b inventory process time ");
    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_gettag
* ����������iso6b��������ȡ��ǩ��Ϣ��װ������㵽
                                һ��6b��ǩ����Ҫ������ֱ��û�б�ǩΪֹ
                                �����ͱ�֤�˺�6c������һ��
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_gettag(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{ 
    T_USPUHFISO6BInventoryRes *ISO6BInventoryRes = NULL;

    ISO6BInventoryRes = (T_USPUHFISO6BInventoryRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_gettag(ISO6BInventoryRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BInventoryRes);
    ptSndMsg->mtype = 1;
    UHFAlertPrint("UHF:iso6b_gettag process time ");
    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_read
* ����������iso6b����ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_read(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFISO6BRead *ptISO6BRead = NULL;
    T_USPUHFISO6BReadRes *ISO6BReadRes = NULL;

    ptISO6BRead = (T_USPUHFISO6BRead *)(ptRcvMsg + 1);
    ISO6BReadRes = (T_USPUHFISO6BReadRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_read(ptISO6BRead, ISO6BReadRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BReadRes) + ISO6BReadRes->wResultLength ;
    ptSndMsg->mtype = 1;
    UHFAlertPrint("UHF:iso6b_read process time ");
    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_write
* ����������iso6bд��ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_write(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFISO6BWrite *ptISO6BWrite = NULL;
    T_USPUHFISO6BWriteRes *ISO6BWriteRes = NULL;

    ptISO6BWrite = (T_USPUHFISO6BWrite *)(ptRcvMsg + 1);
    ISO6BWriteRes = (T_USPUHFISO6BWriteRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_write(ptISO6BWrite, ISO6BWriteRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BWriteRes);
    ptSndMsg->mtype = 1;
    UHFAlertPrint("UHF:iso6b_write process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_lock
* ����������iso6b������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_lock(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFISO6BLock *ptISO6BLock = NULL;
    T_USPUHFISO6BLockRes *ISO6BLockRes = NULL;

    ptISO6BLock = (T_USPUHFISO6BLock *)(ptRcvMsg + 1);
    ISO6BLockRes = (T_USPUHFISO6BLockRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_lock(ptISO6BLock, ISO6BLockRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BLockRes) + ISO6BLockRes->wResultLength;
    ptSndMsg->mtype = 1;
    ptSndMsg->wSenderTno = USP_UHF_TNO;
    UHFAlertPrint("UHF:iso6b_lock process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_iso6b_msg_lockquery
* ����������iso6b������ǩ��ѯ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_iso6b_msg_lockquery(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFISO6BLockQuery *ptISO6BLockQuery = NULL;
    T_USPUHFISO6BLockQueryRes *ISO6BLockQueryRes = NULL;

    ptISO6BLockQuery = (T_USPUHFISO6BLockQuery *)(ptRcvMsg + 1);
    ISO6BLockQueryRes = (T_USPUHFISO6BLockQueryRes *)(ptSndMsg + 1);
    usp_uhf_iso6b_lockquery(ptISO6BLockQuery, ISO6BLockQueryRes);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BLockQueryRes) + ISO6BLockQueryRes->wResultLength;
    ptSndMsg->mtype = 1;

    UHFAlertPrint("UHF:iso6b_lockquery process time ");
    return USP_SUCCESS;
}

/////////////////
///GB msg
/////////////////
/**********************************************************************
* �������ƣ�usp_uhf_gb_msg_inventory
* ���������������Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2           ƽ̨��    lxc        ����
************************************************************************/

////For GB Test lxc
WORD32 usp_uhf_gb_msg_inventory(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
#ifdef GB_DEBUG
    UHFAlertPrint("Debug GB Inventory\n");
    T_USPUHFGBInventory tInventoryBuf[4];
    T_USPUHFGBInventory *ptGBInventory = &tInventoryBuf[0];
    T_USPUHFGBInventoryRes *ptGBTagInventoryResp = NULL;

    T_USPUHFReaderCfg tReaderCfgBuf[2];
    T_USPUHFReaderCfg *ptReaderCfg = &tReaderCfgBuf[0];

    WORD32 dwRet = 0;

    memset(tInventoryBuf, 0, sizeof(T_USPUHFGBInventory)*4);
    memset(tReaderCfgBuf, 0, sizeof(T_USPUHFReaderCfg)*2);
 
    ptGBInventory->ucAntennaID = 1;
    ptGBInventory->ucProtoType = 0x2; /* GB */
    ptGBInventory->tGBInventoryPara.wCCN = 3; /* ����ײ��ֵ */
    ptGBInventory->tGBInventoryPara.wCIN = 4; /* ���д��� */
    ptGBInventory->tGBInventoryPara.wSession = 0;
    ptGBInventory->tGBInventoryPara.wModeindex = 0; /* ģʽ���������ʣ����Ʒ�ʽ�����뷽ʽ */
    ptGBInventory->tGBInventoryPara.ucGBQureySel = 0; /* ƥ������ */
    ptGBInventory->tGBInventoryPara.ucGBQureyAction = 1; /* ��sort�����rule�������Ӧ */
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.wMaskNum = 1;
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].wMemBank = 1;
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].wMaskLength = 0;
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].wMaskBitPointer = 0;
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].ucSortTarget = 0; /* �ỰS0 */
    ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].ucSortAction = 0; /* rule */
    //ptGBInventory->tGBInventoryPara.tGBInventoryMaskList.tGBInventoryMask[0].ucMaskData[GB_MAX_MASK_BYTE_LEN] = {0};

    ptGBInventory->tInventoryRfPara.wTransmitPower = 10; /* ��Ƶ�������� */
    ptGBInventory->tInventoryRfPara.wReceiverSensitivity = 0; /* ò��û�� */
    ptGBInventory->tInventoryRfPara.tFreqCfg.wFreqTableID = 0; /* 800M ��Ƶ�� */
    ptGBInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType = 0; /* ��Ƶ */
    ptGBInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel = 10; /* China800Frequency */
    ptGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode = 0;
    ptGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer = 1;
    ptGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum = 20;
    ptGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel[20] = 0;

    ptReaderCfg->wGBModuleDepth = 0;
    ptReaderCfg->ucTurnOnLKCL = 1;
    ptReaderCfg->wFreqType = 1;
    ptReaderCfg->ucRecvDATT = 63;
    ptReaderCfg->tAntRfCommonCfg[0].aucAntPower[0] = 10;
    ptReaderCfg->tAntRfCommonCfg[0].aucAntPower[1] = 10;
    ptReaderCfg->tAntRfCommonCfg[0].aucAntPower[2] = 10;
    ptReaderCfg->tAntRfCommonCfg[0].aucAntPower[3] = 10;
    ptReaderCfg->tAntRfCommonCfg[0].ucForandRevConfig = 0;
    ptReaderCfg->tAntRfCommonCfg[0].wCanChkStatus = 1;
    ptReaderCfg->tAntRfCommonCfg[0].wRevFilterSwitch = 0;
    ptReaderCfg->tAntRfCommonCfg[0].wLbtDattValue = 0x3f;
    
    /* ����GBCTL���� */
    dwRet = usp_uhf_set_cfg(ptReaderCfg);
    if(dwRet != USP_SUCCESS)
    {
        UHFAlertPrint("usp_uhf_set_cfg ERROR!!\r\n",dwRet);
    }
    usp_uhf_gb_inventory(ptGBInventory, ptGBTagInventoryResp);
    

#else
    T_USPUHFGBInventory *ptGBInventory = NULL;
    T_USPUHFGBInventoryRes *ptGBTagInventoryResp = NULL;
    ptGBInventory = (T_USPUHFGBInventory *)(ptRcvMsg + 1);
    ptGBTagInventoryResp = (T_USPUHFGBInventoryRes *)(ptSndMsg + 1);
    usp_uhf_gb_inventory(ptGBInventory, ptGBTagInventoryResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBInventoryRes)
                        + ptGBTagInventoryResp->tTagId.wTagWordLen * 2;
    UHFAlertPrint("UHF:GB inventory process time ");
#endif
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_gb_msg_query
* ������������������Ϣ��װ����������㵽��ǩ��
                                 ��Ҫ�����ķ���quer����ֱ���յ�������֡
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_gb_msg_query(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBInventoryRes *ptGBTagInventoryResp = NULL;

    ptGBTagInventoryResp = (T_USPUHFGBInventoryRes *)(ptSndMsg + 1);
    usp_uhf_gb_query(ptGBTagInventoryResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBInventoryRes)
                        + ptGBTagInventoryResp->tTagId.wTagWordLen * 2;
    UHFAlertPrint("UHF:GB query process time ");
    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�usp_uhf_gb_msg_read
* ��������������Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2           ƽ̨��    lxc        ����
************************************************************************/

WORD32 usp_uhf_gb_msg_read(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBRead *ptGBRead = NULL;
    T_USPUHFGBReadRes *ptGBReadResp = NULL;

    ptGBRead = (T_USPUHFGBRead *)(ptRcvMsg + 1);
    ptGBReadResp = (T_USPUHFGBReadRes *)(ptSndMsg + 1);
    usp_uhf_gb_read(ptGBRead, ptGBReadResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBReadRes)
                        + ptGBReadResp->wResultLength * 2;
    UHFAlertPrint("UHF:GB read process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_gb_msg_write
* ����������д��ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2           ƽ̨��    lxc        ����
************************************************************************/

WORD32 usp_uhf_gb_msg_write(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBWrite *ptGBWrite = NULL;
    T_USPUHFGBWriteRes *ptGBWriteResp = NULL;

    ptGBWrite = (T_USPUHFGBWrite *)(ptRcvMsg + 1);
    ptGBWriteResp = (T_USPUHFGBWriteRes *)(ptSndMsg + 1);
    usp_uhf_gb_write(ptGBWrite, ptGBWriteResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBWriteRes);
    UHFAlertPrint("UHF:GB write process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_msg_lock
* ����������������ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_gb_msg_lock(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBLock *ptGBLock = NULL;
    T_USPUHFGBLockRes *ptGBLockResp = NULL;

    ptGBLock = (T_USPUHFGBLock *)(ptRcvMsg + 1);
    ptGBLockResp = (T_USPUHFGBLockRes *)(ptSndMsg + 1);
    usp_uhf_gb_lock(ptGBLock, ptGBLockResp); 
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBLockRes);
    UHFAlertPrint("UHF:GB lock process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_gb_msg_erase
* ��������:   ������ǩ��װ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2          ƽ̨��    lxc        ����
************************************************************************/
WORD32 usp_uhf_gb_msg_erase(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBErase *ptGBErase = NULL;
    T_USPUHFGBEraseRes *ptGBEraseResp = NULL;

    ptGBErase = (T_USPUHFGBErase *)(ptRcvMsg + 1);
    ptGBEraseResp = (T_USPUHFGBEraseRes *)(ptSndMsg + 1);
    usp_uhf_gb_erase(ptGBErase, ptGBEraseResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBEraseRes);
    UHFAlertPrint("UHF:GB erase process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�WORD32 usp_uhf_gb_msg_kill(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
* ����������ɱ����ǩ��Ϣ��װ����
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2           ƽ̨��    lxc        ����
************************************************************************/

WORD32 usp_uhf_gb_msg_kill(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_USPUHFGBKill *ptGBKill = NULL;
    T_USPUHFGBKillRes *ptGBKillResp = NULL;

    ptGBKill = (T_USPUHFGBKill *)(ptRcvMsg + 1);
    ptGBKillResp = (T_USPUHFGBKillRes *)(ptSndMsg + 1);
    usp_uhf_gb_kill(ptGBKill, ptGBKillResp);
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBKillRes);
    UHFAlertPrint("UHF:GB lock process time ");
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_msg_request_cfg
* �������������ض���ģ���������ù���������Ϣ
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_msg_request_cfg(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    ptSndMsg->wMsgID = EV_USP_OAM_CMP_RCPConfigRequest;
    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgLen = 0;
    ptSndMsg->wSenderTno = USP_UHF_TNO;
    ptSndMsg->wReceiverTno = USP_OAM_CMP_TNO;
    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ�usp_uhf_msg_stop_proc
* ������������RDP�����쳣��ֹͣfpga�Ĵ���ǿ�ƽӿ�
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_msg_stop_proc(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    uhf_StopMacProc();
    uhf_StopTx();
    ptSndMsg->wMsgLen = 0;
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�usp_uhf_msg_spec_proc
* ����������Ϊrdp�ṩ�Ŀ���Ϣ����ӿ�
* ���������ptRcvMsg ���յ�����Ϣ   ptsndmsg ������Ϣ
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/7           ƽ̨��    zdx        ����
************************************************************************/

WORD32 usp_uhf_msg_spec_proc(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    ptSndMsg->wMsgLen = 0;
    return USP_SUCCESS;
}
#if 0
/**********************************************************************
* �������ƣ�int uhf_InitStateprocess(T_Msg *ptRcvMsg)
* ����������Init״̬����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������
* ���������int

* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static WORD32 uhf_InitStateprocess(T_Msg *ptRcvMsg)
{
    WORD32 dwRet = USP_SUCCESS;
    T_Msg *ptSndMsg = NULL;
    T_USPUHFReaderCfg *ptRFConfigRequestAck = NULL;
    ptSndMsg = (T_Msg *)g_dwSendMsgBuf;

    if (EV_USP_SCP_PowerOn == ptRcvMsg->wMsgID)
    {
        /* �յ��ϵ���Ϣ */
        DEBUG_MSG("UHF: receive a EV_PowerOn message \r\n");
        dwRet = usp_uhf_init();

        if (USP_SUCCESS == dwRet)
        {
            usp_uhf_msg_request_cfg(NULL, ptSndMsg);

            /* ����RF�������� */
            DEBUG_MSG("UHF: request RF configure data\r\n");
            uhf_SendMsg(USP_OAM_CMP_TNO, ptSndMsg);
        }
        else
        {
            /*UHF ��ʼ��ʧ��*/
            ptSndMsg->wMsgID  = EV_USP_SCP_PowerOnFailure;
            ptSndMsg->mtype = 1;
            ptSndMsg->wMsgLen = 0;
            ptSndMsg->wSenderTno = USP_UHF_TNO;
            ptSndMsg->wReceiverTno = USP_OSS_SCP_TNO;
            DEBUG_MSG("UHF: Send power on failed to scp\r\n");
            uhf_SendMsg(USP_OSS_SCP_TNO, ptSndMsg);
        }
    }
    else if (EV_USP_OAM_CMP_RCPConfigRequestAck == ptRcvMsg->wMsgID)
    {
        DEBUG_MSG("UHF:Receive EV_USP_OAM_CMP_RCPConfigRequestAck \r\n");
        ptRFConfigRequestAck = (T_USPUHFReaderCfg *)(ptRcvMsg + 1);
        usp_uhf_set_cfg(ptRFConfigRequestAck);
        uhf_SndPowerOnSuccess();
        g_dwRcpState = UHF_STATE_WORK;

    }
    else
    {
        DEBUG_MSG("UHF: init state receive invalid message = 0x%08x !\r\n", ptRcvMsg->wMsgID);
        //LAPInsert(FILE_UHF, USP_UHF_RCV_MSG_ERROR, ptRcvMsg->wMsgID, 0);
    }


    return USP_SUCCESS;
}
#endif

/**********************************************************************
* �������ƣ�int RCP_IdleState_process(T_Msg *ptRcvMsg)
* ����������Idle״̬����Ϣ����
* ���������T_Msg *ptRcvMsg ���յ�����Ϣ
* ���������
* �� �� ֵ��int

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
* 2010/12/1    V04.00.00       zdx          �޸�Ϊ��һѭ������
************************************************************************/
static WORD32 uhf_WorkStateProcess(T_Msg *ptRcvMsg)
{
    WORD32 dwRetVal = USP_SUCCESS;
    T_Msg *ptSndMsg = NULL;
    struct timeval Rcp_StartTime;
    struct timeval Rcp_EndTime;
    T_MsgFunDef *ptProtoclProcFun = NULL;
    WORD16 wMsgIDIndex = 0;

    GetCurrUsTime(&Rcp_StartTime);
    ptSndMsg = (T_Msg *)g_dwSendMsgBuf;

    wMsgIDIndex = GetUhfMsgIndex(ptRcvMsg->wMsgID);
    
    //uhf_SetTestpin();
    if (wMsgIDIndex < GetUhfMsgIndex(EV_USP_UHF_MAX_MSG_ID))
    {
        ptProtoclProcFun = &g_tMsgProcTable[wMsgIDIndex];
        ptProtoclProcFun->ptProcFun(ptRcvMsg, ptSndMsg);
        ptSndMsg->wMsgID = ptProtoclProcFun->dwSndMsgId;
        ptSndMsg->mtype = 1;
        ptSndMsg->wSenderTno = USP_UHF_TNO;

        if (ptProtoclProcFun->wReceiverTno == 0)
        {
            /*����������ȥ*/
            ptSndMsg->wReceiverTno = ptRcvMsg->wSenderTno;
        }
        else
        {
            ptSndMsg->wReceiverTno = ptProtoclProcFun->wReceiverTno;
        }

        GetCurrUsTime(&Rcp_EndTime);
        UHFAlertPrint(" %dms\r\n\r\n\r\n", CalMsTimeInterval(&Rcp_StartTime, &Rcp_EndTime));
    }
		/*wzp 20150421*/
#if 0
    else
    {
        if (EV_USP_CLI_Cmd_Request == ptRcvMsg->wMsgID)
        {
            DEBUG_MSG("receive a cli message.\r\n");
            uhfCliProc(ptRcvMsg);
            ptSndMsg->wSenderTno = USP_UHF_TNO;
            ptSndMsg->wReceiverTno = 0xffff;

        }
        else if (EV_USP_OAM_CMP_CfgParaChg == ptRcvMsg->wMsgID)
        {
            usp_uhf_msg_request_cfg(NULL, ptSndMsg);
        }

#if !USP_IS_DISTRIBUTED
        else if (EV_USP_OAM_DMP_AppRequest == ptRcvMsg->wMsgID)
        {
            uhfRf_CreateDmpResMsg(ptRcvMsg, ptSndMsg);
            ptSndMsg->wSenderTno = USP_UHF_TNO;
            ptSndMsg->wReceiverTno = USP_OAM_DMP_TNO;

        }

#endif
        else if (EV_USP_OAM_CMP_RCPConfigRequestAck ==
                 ptRcvMsg->wMsgID)
        {
            T_USPUHFReaderCfg *ptReaderCfg =
                (T_USPUHFReaderCfg *)(ptRcvMsg + 1);
            usp_uhf_set_cfg(ptReaderCfg);
            ptSndMsg->wSenderTno = USP_UHF_TNO;
            ptSndMsg->wReceiverTno = 0xffff;
        }
        else
        {
            DEBUG_MSG("Rcv invalid message %x\r\n", ptRcvMsg->wMsgID);
            //LAPInsert(FILE_UHF, USP_UHF_RCV_MSG_ERROR, ptRcvMsg->wMsgID, 0);
            ptSndMsg->wSenderTno = USP_UHF_TNO;
            ptSndMsg->wReceiverTno = 0xffff;
        }
    }
#endif
    /*���е���Ϣ����Ҫ�����յ������кŷ���*/
    ptSndMsg->wMsgSequence = ptRcvMsg->wMsgSequence;
    ptSndMsg->mtype = 1;
    
    //uhf_SetTestpin();
	
    //uhf_SendMsg(ptSndMsg->wReceiverTno, ptSndMsg);
    zmq_send(rdp_rep_socket, ptSndMsg, sizeof(T_Msg)+ptSndMsg->wMsgLen, ZMQ_DONTWAIT);
	
    //uhf_SetTestpin();
    
    return (dwRetVal);

} /* int RCP_IdleState_process(T_Msg *ptMsg) */


/**********************************************************************
* �������ƣ�uhf_SetRssiCfg
* ��������������һ�����ߵ�RSSI��ֵ
* ���������WORD32 wIndex ���õ�����
                            T_USPUHFReaderCfg *ptReaderCfg ���յ�������
* ���������
            �ɹ���0
            ʧ�ܣ�������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/1           RUP           zdx            ����
************************************************************************/

void  uhf_SetRssiCfg(WORD32 wIndex, T_USPUHFReaderCfg *ptReaderCfg)
{
    T_AntCfgs *ptCurAntCfg = &g_tUhfCtrl.atRfAntCfg[wIndex + 1];

    /*RSSI �������ýӿ� */
    ptCurAntCfg->tEPCReadRssiCfg.ucTurnOnFilter
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.ucFilterSwitch;
    ptCurAntCfg->tEPCReadRssiCfg.wRSSIThreshHold
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6C.ucFilterThreshold / 100;
    DEBUG_MSG("UHF:epc read fileter %d %d\r\n",
                ptCurAntCfg->tEPCReadRssiCfg.ucTurnOnFilter,
                ptCurAntCfg->tEPCReadRssiCfg.wRSSIThreshHold);

    ptCurAntCfg->tGBReadRssiCfg.ucTurnOnFilter
        = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.ucFilterSwitch;
    ptCurAntCfg->tGBReadRssiCfg.wRSSIThreshHold
        = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilterGB.ucFilterThreshold / 100;
    DEBUG_MSG("UHF:GB read fileter %d %d\r\n",
            ptCurAntCfg->tGBReadRssiCfg.ucTurnOnFilter,
            ptCurAntCfg->tGBReadRssiCfg.wRSSIThreshHold);

    ptCurAntCfg->tISO6BReadRssiCfg.ucTurnOnFilter
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.ucFilterSwitch;
    ptCurAntCfg->tISO6BReadRssiCfg.wRSSIThreshHold
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tReadRSSIFilter.tRSSIFilter6B.ucFilterThreshold / 100;

    DEBUG_MSG("UHF:iso read fileter %d %d\r\n",
                ptCurAntCfg->tISO6BReadRssiCfg.ucTurnOnFilter,
                ptCurAntCfg->tISO6BReadRssiCfg.wRSSIThreshHold);

    ptCurAntCfg->tEPCWriteRssiCfg.ucTurnOnFilter
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.ucFilterSwitch;
    ptCurAntCfg->tEPCWriteRssiCfg.wRSSIThreshHold
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6C.ucFilterThreshold / 100;
    DEBUG_MSG("UHF:epc write fileter %d %d\r\n",
                ptCurAntCfg->tEPCWriteRssiCfg.ucTurnOnFilter,
                ptCurAntCfg->tEPCWriteRssiCfg.wRSSIThreshHold);

    ptCurAntCfg->tGBWriteRssiCfg.ucTurnOnFilter
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.ucFilterSwitch;
    ptCurAntCfg->tGBWriteRssiCfg.wRSSIThreshHold
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilterGB.ucFilterThreshold / 100;
    DEBUG_MSG("UHF:GB write fileter %d %d\r\n",
                ptCurAntCfg->tGBWriteRssiCfg.ucTurnOnFilter,
                ptCurAntCfg->tGBWriteRssiCfg.wRSSIThreshHold);

    ptCurAntCfg->tISO6BWriteRssiCfg.ucTurnOnFilter
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.ucFilterSwitch;
    ptCurAntCfg->tISO6BWriteRssiCfg.wRSSIThreshHold
    = ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.wMaxReadRSSI
      * ptReaderCfg->tAntRfCommonCfg[wIndex].tWriteRSSIFilter.tRSSIFilter6B.ucFilterThreshold / 100;


    DEBUG_MSG("UHF:iso write fileter %d %d\r\n",
                ptCurAntCfg->tISO6BWriteRssiCfg.ucTurnOnFilter,
                ptCurAntCfg->tISO6BWriteRssiCfg.wRSSIThreshHold);

}

/**********************************************************************
* �������ƣ�uhf_SetEpcUserDefCfg
* ��������������һ�����ߵ��û��Զ���epc����
* ���������WORD16 wIndex, ������
                       T_USPUHFReaderCfg *ptReaderCfg   ���յ�������
* �����������
* �� �� ֵ��    ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/1           RUP           zdx            ����
************************************************************************/

void uhf_SetEpcUserDefCfg(WORD16 wIndex, T_USPUHFReaderCfg *ptReaderCfg)
{
    T_AntCfgs *ptCurAntCfg = &g_tUhfCtrl.atRfAntCfg[wIndex + 1];

    ptCurAntCfg->tEpcUserDefPara.eDataEncodeType
    = ptReaderCfg->tAntEpcUserDefPara[wIndex].eDataEncodeType;

    ptCurAntCfg->tEpcUserDefPara.eModulateType
    = ptReaderCfg->tAntEpcUserDefPara[wIndex].eModulateType;

    ptCurAntCfg->tEpcUserDefPara.wForDataRate
    = ptReaderCfg->tAntEpcUserDefPara[wIndex].wForDataRate;

    ptCurAntCfg->tEpcUserDefPara.wPIEValue
    = ptReaderCfg->tAntEpcUserDefPara[wIndex].wPIEValue;

    ptCurAntCfg->tEpcUserDefPara.wRevDataRate
    = ptReaderCfg->tAntEpcUserDefPara[wIndex].wRevDataRate;

    DEBUG_MSG("UHF:encodetype %d modutype %d forward %d reverse %d\r\n",
                ptCurAntCfg->tEpcUserDefPara.eDataEncodeType,
                ptCurAntCfg->tEpcUserDefPara.eModulateType,
                ptCurAntCfg->tEpcUserDefPara.wForDataRate,
                ptCurAntCfg->tEpcUserDefPara.wRevDataRate);
}

void uhf_SetGBUserDefCfg(WORD16 wIndex, T_USPUHFReaderCfg *ptReaderCfg)
{
    T_AntCfgs *ptCurAntCfg = &g_tUhfCtrl.atRfAntCfg[wIndex + 1];

    //ptCurAntCfg->tGBUserDefPara.eDataEncodeType
    //= ptReaderCfg->tAntGBUserDefPara[wIndex].eDataEncodeType;

    //ptCurAntCfg->tGBUserDefPara.eModulateType
    //= ptReaderCfg->tAntGBUserDefPara[wIndex].eModulateType;

    //ptCurAntCfg->tGBUserDefPara.wForDataRate
    //= ptReaderCfg->tAntGBUserDefPara[wIndex].wForDataRate;

    //ptCurAntCfg->tGBUserDefPara.wPIEValue
    //= ptReaderCfg->tAntGBUserDefPara[wIndex].wPIEValue;

    //ptCurAntCfg->tGBUserDefPara.wRevDataRate
    //= ptReaderCfg->tAntGBUserDefPara[wIndex].wRevDataRate;

    DEBUG_MSG("UHF:encodetype %d modutype %d forward %d reverse %d\r\n",
                ptCurAntCfg->tGBUserDefPara.eDataEncodeType,
                ptCurAntCfg->tGBUserDefPara.eModulateType,
                ptCurAntCfg->tGBUserDefPara.wForDataRate,
                ptCurAntCfg->tGBUserDefPara.wRevDataRate);
}


/**********************************************************************
* �������ƣ�uhf_SetCommonCfg
* ������������¼ͨ�õ�һЩ���ã��Ķ�����һ������
* ���������T_USPUHFReaderCfg *ptReaderCfg   ���յ�������
* �����������
* �� �� ֵ��    ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/1           RUP           zdx            ����
************************************************************************/
void uhf_SetCommonCfg(T_USPUHFReaderCfg *ptReaderCfg)
{
    BYTE aucEPCModuleDepth[] = {80, 90, 100};
    BYTE aucISO6BModuleDepth[] = {18, 100};
    BYTE ucGBModuleDepth[] = {100};

    /* ������� */
    if (ptReaderCfg->wEPCModuleDepth < 2)
    {
        g_tEpcCtrl.wRTModDepth = aucEPCModuleDepth[ptReaderCfg->wEPCModuleDepth];
    }
    else
    {
        g_tEpcCtrl.wRTModDepth = aucEPCModuleDepth[2];
    }


    g_tISO6BCtrl.wModDepth =
        (ptReaderCfg->wISO6BModuleDepth > 1) ? aucISO6BModuleDepth[0] : \
        aucISO6BModuleDepth[ptReaderCfg->wISO6BModuleDepth];

    g_tGBCtrl.wRTModDepth = ucGBModuleDepth[ptReaderCfg->wGBModuleDepth];

    DEBUG_MSG("UHF: EPC Depth %d 6Bdepth %d GB Depth %d\r\n", g_tEpcCtrl.wRTModDepth,
                g_tISO6BCtrl.wModDepth, g_tGBCtrl.wRTModDepth);

}
/**********************************************************************
* �������ƣ�usp_uhf_set_cfg
* ����������������������ȫ�ֱ�������
* ���������T_USPUHFReaderCfg *ptReaderCfg ���յ�������
* ���������
            �ɹ���0
            ʧ�ܣ�������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/1           RUP           zdx            ����
************************************************************************/

WORD32 usp_uhf_set_cfg(T_USPUHFReaderCfg *ptReaderCfg)
{
    WORD32 dwIndex = 0;
    /* ���õ������ */
    uhf_SetCommonCfg(ptReaderCfg);

    for (dwIndex = 0; dwIndex < READER_ANTENNA_COUNT; dwIndex++)
    {
        DEBUG_MSG("UHF:set ant %d cfgs\r\n", dwIndex);

        uhf_SetRssiCfg(dwIndex, ptReaderCfg);
        uhf_SetEpcUserDefCfg(dwIndex, ptReaderCfg);
        //uhf_SetGBUserDefCfg(dwIndex, ptReaderCfg); /* ��ʱ��Ҫ */
    }

#if !USP_IS_DISTRIBUTED
    uhfRf_SetCfgs(ptReaderCfg);
#endif
    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ�void uhf_SendTraceMsg(WORD16 wMsgID,WORD16 ParaLen,BYTE *pucPara)
* ������������������õĺ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/12/17   V0.0.1       zdx         ����
************************************************************************/
WORD32 uhf_SendTraceMsg(WORD16 wSendOrRecv, WORD16 wMsgID, WORD16 ParaLen, WORD16 *pwPara)
{
    T_Msg *ptTraceMsg = NULL;
    WORD16 *pwTraceLenPtr = NULL;
    WORD16 wFpgaDir = USP_OSS_ST_SND;
    ptTraceMsg = (T_Msg *)g_aucTraceBuffer;
    ptTraceMsg->mtype = 1;
    ptTraceMsg->ucLinkType = 0;
    ptTraceMsg->ucLinkIndex = 0;

    DEBUG_MSG("UHF Trace ID is %d\r\n", wMsgID);

    if (ParaLen > (TRACEBUFFERLEN - sizeof(T_Msg)*2 - 2))
    {
        DEBUG_MSG("Trace size is too long\r\n");
        //LAPInsert(FILE_UHF, USP_UHF_RCVED_PARA_ERROR, ParaLen, 0);
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if (NULL != pwPara)
    {
        /*������fpga����δ֪֡ʱ����и�ʽת�����������������*/
        if (EV_FPGA_UNKNOW_RESP == wMsgID)
        {
            pwTraceLenPtr = (WORD16 *)(ptTraceMsg + 1);
            *pwTraceLenPtr = ParaLen;
            memcpy((void *)(pwTraceLenPtr + 1), (void *)pwPara, ParaLen);
            ParaLen += 2; /*����ռ�õ�һ��WORD16�ĳ���*/
        }
        else
        {
            memcpy((void *)(ptTraceMsg + 1), (void *)pwPara, ParaLen);
        }

        DEBUG_MSG("set information \r\n");
    }
    else
    {
        if (ParaLen != 0)
        {
            DEBUG_MSG("Trace Len should be 0\r\n");
            return USP_UHF_RCVED_PARA_ERROR;
        }
    }

    if (wSendOrRecv == EV_RCP_TO_FPGA)
    {
        ptTraceMsg->wSenderTno = USP_UHF_TNO;
        ptTraceMsg->wReceiverTno = USP_FPGA_TNO;
        wFpgaDir = USP_OSS_ST_RCV;
    }
    else
    {
        ptTraceMsg->wSenderTno = USP_FPGA_TNO;
        ptTraceMsg->wReceiverTno = USP_UHF_TNO;
        wFpgaDir = USP_OSS_ST_SND;
    }

    ptTraceMsg->wMsgID = wMsgID;
    ptTraceMsg->wMsgLen = ParaLen;

    DEBUG_MSG("send trace message Direction %d\r\n", wFpgaDir);
    usp_oss_trace(wFpgaDir, g_aucDesc, ptTraceMsg);

    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�uhf_getInventQ
* ��������������Ԥ����ǩ��ת����Э��Ҫ���qֵ
* ���������WORD16 wTagPopulation Ԥ����ǩ��
* ���������
* �� �� ֵ��    WORD16 ת�����Qֵ
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/12/17   V0.0.1       zdx         ����
************************************************************************/

WORD16 uhf_getInventQ(WORD16 wTagPopulation)
{
    WORD16 wInventaryQ = 0;


    DEBUG_MSG("inventory tags %d \r\n", wTagPopulation);

    if (wTagPopulation < 2)
    {
        wInventaryQ = 0;
    }
    else if (wTagPopulation == 2)
    {
        wInventaryQ = 1;
    }
    else if (wTagPopulation <= 4)
    {
        wInventaryQ = 2;
    }
    else if (wTagPopulation <= 8)
    {
        wInventaryQ = 3;
    }
    else if (wTagPopulation <= 16)
    {
        wInventaryQ = 4;
    }
    else if (wTagPopulation <= 32)
    {
        wInventaryQ = 5;
    }
    else if (wTagPopulation <= 64)
    {
        wInventaryQ = 6;
    }
    else if (wTagPopulation <= 128)
    {
        wInventaryQ = 7;
    }
    else if (wTagPopulation <= 256)
    {
        wInventaryQ = 8;
    }
    else if (wTagPopulation <= 512)
    {
        wInventaryQ = 9;
    }
    else if (wTagPopulation <= 1024)
    {
        wInventaryQ = 10;
    }
    else if (wTagPopulation <= 2048)
    {
        wInventaryQ = 11;
    }
    else if (wTagPopulation <= 4096)
    {
        wInventaryQ = 12;
    }
    else if (wTagPopulation <= 8192)
    {
        wInventaryQ = 13;
    }
    else if (wTagPopulation <= 16384)
    {
        wInventaryQ = 14;
    }
    else if (wTagPopulation <= 32768)
    {
        wInventaryQ = 15;
    }
    else
    {
        wInventaryQ = 15;
    }

    DEBUG_MSG("The final Q is %d \r\n", wInventaryQ);

    return wInventaryQ;

}

/**********************************************************************
* �������ƣ�uhf_SendRfCfgs
* ��������������Ƶ�巢��������Ϣ
* ���������T_UhfInventRfConfigPara *ptInventRfConfigPara   ��Ƶ����
                                 T_UhfInventRfConfigRes *ptInventRfCfgRes   ���÷���
* ���������
* �� �� ֵ��    EV_USP_UHF_CFG_RCV_TIME_OUT  ��ʱû�з�������
                                 USP_SUCCESS   �ɹ����Ͳ����շ��ؽ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/12/17   RUP       zdx         ����
************************************************************************/

WORD32  uhf_SendRfCfgs(T_UhfInventRfConfigPara *ptInventRfConfigPara,
                       T_UhfInventRfConfigRes *ptInventRfCfgRes)
{
    WORD32 dwRet = USP_SUCCESS;

#if USP_IS_DISTRIBUTED
    T_Msg *ptSndMsg = (T_Msg *)g_dwRfSendMsgBuf;
    T_Msg *ptRcvMsg = (T_Msg *)g_dwRfRcvMsgBuf;
    static WORD16 wSerialNu = 0;

    T_UhfInventRfConfigPara *ptInventRfCfg
    = (T_UhfInventRfConfigPara *)(ptSndMsg + 1);
    T_UhfInventRfConfigRes *ptInventRfCfgResTmp
    = (T_UhfInventRfConfigRes *)(ptRcvMsg + 1);
    T_Msg           tExpectMsg;
    T_Msg           *atExcept[] = {&tExpectMsg, NULL};

    memcpy(ptInventRfCfg, ptInventRfConfigPara,
           sizeof(T_UhfInventRfConfigPara));

    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgID = EV_USP_UHF_INVENT_RF_CFG;
    ptSndMsg->wSenderTno = USP_UHF_TNO;
    ptSndMsg->wMsgLen = sizeof(T_UhfInventRfConfigPara);
    ptSndMsg->wReceiverTno = USP_UHF_RF_TNO;
    ptSndMsg->wMsgSequence = USP_OSS_SCP_MSGSEQ(wSerialNu);
    dwRet = usp_oss_scp_sendTaskMsg(USP_UHF_RF_TNO, ptSndMsg, USP_OSS_IPC_NOWAIT);

    if (USP_SUCCESS != dwRet)
    {
        DEBUG_MSG("uhf send to rf invent cfg error\r\n");
        return dwRet;
    }

    DEBUG_MSG("uhf send to rf invent serial %d\r\n", wSerialNu);

    memset(&tExpectMsg, 0xff, sizeof(T_Msg));
    tExpectMsg.wMsgID = EV_USP_UHF_INVENT_RF_CFG_RES;
    tExpectMsg.wMsgSequence = wSerialNu;
    dwRet = usp_oss_scp_recvTaskMsgWithTypes(USP_UHF_TNO, ptRcvMsg,
            UHF_MAX_MSG_SIZE, UHF_CFG_RF_CFG_WAIT_TIME,
            atExcept);

    if (USP_SUCCESS != dwRet)
    {
        DEBUG_MSG("rcv rf response error %d\r\n", dwRet);
    }
    else
    {
        DEBUG_MSG("uhf ant %d channel %d\r\n",
                    ptInventRfCfgResTmp->wAntIndex,
                    ptInventRfCfgResTmp->wChannelNo);
        memcpy(ptInventRfCfgRes, ptInventRfCfgResTmp,
               sizeof(T_UhfInventRfConfigRes));
        dwRet = ptInventRfCfgRes->wResult;
    }

#else
    dwRet = uhfRf_SetInventCfgs(ptInventRfConfigPara, ptInventRfCfgRes);

    if (dwRet != USP_SUCCESS)
    {
        DEBUG_MSG("uhf :set rf error\r\n");
    }

#endif
    return dwRet;
}


/**********************************************************************
* �������ƣ�uhf_SendRfCfgs
* ��������������Ƶ�巢�����ʱ��Ƶ������Ϣ
* ���������WORD16 wAntIndex,  ���ߺ�
                                 wProtoType               Э������ EPC ����ISO ����GB
                                 WORD16 wForDataRate,  ǰ������
                                 WORD16 wRcvDataRate,  ��������
                                 WORD16 wModDepth,     �������
                                 WORD16 wModType,    ���Ʒ�ʽ
                                 T_USPUHFInventRfPara *ptRfPara   ��Ƶ����
* ���������
* �� �� ֵ��    ���� ����
                                 USP_SUCCESS   �ɹ����Ͳ����շ��ؽ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/12/17   RUP       zdx         ����
* 2012/3/21    RUP       zdx   ����Э����Ʋ���
************************************************************************/
WORD32 uhf_SetInventRfPara(WORD16 wAntIndex, WORD16 wProtoType, WORD16 wForDataRate,
                           WORD16 wRcvDataRate, WORD16 wModDepth,
                           WORD16 wModType, T_USPUHFInventRfPara *ptRfPara)
{
    WORD32 dwRet = USP_SUCCESS;
    T_UhfInventRfConfigPara *ptRfCfgPara = NULL;
    T_UhfInventRfConfigRes *ptRfCfgRes = NULL;
    T_UhfInventRfConfigPara tRfconfigpara;
    T_UhfInventRfConfigRes tInventRfCfgRes;
    struct timeval Rf_Cfg_StartTime;
    struct timeval Rf_Cfg_EndTime;

    /*��¼����rssi���˲����ͷ��ظ�RDP��Ϣ�л��õ�*/
    g_tUhfCtrl.wAntIndex = wAntIndex;

    ptRfCfgPara = &tRfconfigpara;
    ptRfCfgRes = &tInventRfCfgRes;

    ptRfCfgPara->wAntIndex = wAntIndex;
    ptRfCfgPara->wForDataRate = wForDataRate;
    ptRfCfgPara->wRevDataRate = wRcvDataRate;
    ptRfCfgPara->wModulateDepth = wModDepth;
    ptRfCfgPara->wModulateType = wModType;
    ptRfCfgPara->wProtocolType = wProtoType;
    /*���ְ�Ҳ��Ҫ���õ�������*/
    //SetRbModulateType(wModType);

    if (NULL == ptRfPara)
    {
        /*��Ƶ������Ϣ����*/
        ptRfCfgPara->wRfCfgExist = RF_CFG_NOT_EXIST;

    }
    else
    {
        ptRfCfgPara->wRfCfgExist = RF_CFG_EXIST;
        memcpy(&ptRfCfgPara->tRfCfg, ptRfPara, sizeof(T_USPUHFInventRfPara));
    }

    GetCurrUsTime(&Rf_Cfg_StartTime);
    dwRet = uhf_SendRfCfgs(ptRfCfgPara, ptRfCfgRes);

    if (dwRet != USP_SUCCESS)
    {
        DEBUG_MSG("UHF:set rf error\r\n");
        return dwRet;
    }

    GetCurrUsTime(&Rf_Cfg_EndTime);
    DEBUG_MSG("UHF:uhf cfg rf time %dus\r\n",
                CalMsTimeInterval(&Rf_Cfg_StartTime, &Rf_Cfg_EndTime));

    DEBUG_MSG("UHF:rf ant index %d\r\n", ptRfCfgRes->wAntIndex);
    g_tUhfCtrl.wWorkChannel = ptRfCfgRes->wChannelNo;

#if USP_IS_DISTRIBUTED
    /*���ֻ���ڷֲ�ʽ��ϵͳ����Ҫ
          ��Ƶ����Ժ���Ҫ��ʱ1.5ms*/
    uhf_TxSet(UHF_RF_START_TX);
    //usp_drv_timer_usleep(1500);
    USP_OSS_VOS_SLEEP_USEC(460);
#endif

    GetCurrUsTime(&Rf_Cfg_EndTime);
    DEBUG_MSG("UHF:uhf cfg rf wait time %dms\r\n",
                CalMsTimeInterval(&Rf_Cfg_StartTime, &Rf_Cfg_EndTime));


    return USP_SUCCESS;
}

#endif
/**************************************************** file end ********************************************************************/
