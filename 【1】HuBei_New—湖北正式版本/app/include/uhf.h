/************************************************************************
* ��Ȩ���� (C)2011, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� uhf.h
* �ļ���ʶ�� 
* ����ժҪ������ƵЭ�����õ��ĺ궨��
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� zdx
* ������ڣ� 2011.12.1
* 
* �޸ļ�¼1�� 
*    �޸����ڣ�
*    �� �� �ţ�   
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/

#ifndef _UHF_H
#define _UHF_H


/************************************************/

#define UHF_CFG_TIMEOUT_TIME     (1000)               /*ƽ̨��ʱ������10ms����*/
#define UHF_AUTO_RF_CLOSE_TIME   (3000)
#define UHF_CFG_RF_CFG_WAIT_TIME (3000)
/* RFIDЭ������ */
#define UHF_PROTO_TYPE_EPC      (0x0)   /* GEN2 */
#define UHF_PROTO_TYPE_ISO6B    (0x1)   /* 18000-6 Type B */
#define UHF_PROTO_TYPE_GB        (0x2)


#define INVENTORY_NOT_FINISH      (0)   /* ��ǩ��㻹δ���� */
#define INVENTORY_HAS_FINISH      (1)   /* ��ǩ����Ѿ����� */

/* ���ʱ����״̬���� */
#define MAC_INVENTORY_PROCESS_FINISH                 (0)
#define MAC_INVENTORY_PROCESS_SUCCESS              (1)


/* QUERY����M�������� */
#define EPC_QUERY_M_1   (0)
#define EPC_QUERY_M_2   (1)
#define EPC_QUERY_M_4   (2)
#define EPC_QUERY_M_8   (3)
// For GB
#define GB_QUERY_M_1   (0)
#define GB_QUERY_M_2   (1)
#define GB_QUERY_M_4   (2)
#define GB_QUERY_M_8   (3)


/* QUERY����TRext�������� */
#define EPC_QUERY_TREXT_NO_PILOT_TONE    (0)
#define EPC_QUERY_TREXT_USE_PILOT_TONE   (1)
// For GB --------FIX ME !!
#define GB_QUERY_TREXT_NO_PILOT_TONE    (0)
#define GB_QUERY_TREXT_USE_PILOT_TONE   (1)


/* QUERY����DR�������� */
#define EPC_QUERY_DR_8          (0)
#define EPC_QUERY_DR_64_DIV_3   (1)
#define EPC_QUERY_DR_GB         (3)
//for GB  -------FIX ME !!
#define GB_QUERY_DR_8          (0)
#define GB_QUERY_DR_64_DIV_3   (1)
#define GB_QUERY_DR_GB         (3)

/* Rcp����״̬ */
#define UHF_STATE_INIT                  (0)   /* ���̳�ʼ״̬     */
#define UHF_STATE_WORK                  (1)   /* ����״̬         */
#define UHF_STATE_WAITE_CONFIG          (2)   /* �ڹ���״̬�½���RF������ */
#define UHF_STATE_RF_CONFIG             (3)

/* RCP��Ϣ�����������bytes */
#define UHF_MAX_MSG_SIZE  (2048)
#define TRACEBUFFERLEN  1024

#if (UHF_BOARD_TYPE == BOARD_TYPE_RBCB1)
#define FPGA_FREQUENCE         (30720000)
#define WRITE_RCV_DELAY_H      (0x0009)  
#define WRITE_RCV_DELAY_L      (0x6000)
#define EPC_BLOCK_START_DELAY  (0X8BD51)
#define UHF_BEFORE_SIG_SND_TIME (460)
#elif (UHF_BOARD_TYPE == BOARD_TYPE_RFCB)
#define FPGA_FREQUENCE          (26000000)
#define WRITE_RCV_DELAY_H       (0x0007)  
#define WRITE_RCV_DELAY_L       (0x6590)
#define EPC_BLOCK_START_DELAY  (0x76590)
#define UHF_BEFORE_SIG_SND_TIME (460)
#elif (UHF_BOARD_TYPE == BOARD_TYPE_PURM)
#define FPGA_FREQUENCE         (26000000)
#define WRITE_RCV_DELAY_H      (0x0007)  
#define WRITE_RCV_DELAY_L      (0x6590)
#define EPC_BLOCK_START_DELAY  (0x76590)
#define UHF_BEFORE_SIG_SND_TIME (460)
#else
#define FPGA_FREQUENCE         (26000000)
#define WRITE_RCV_DELAY_H      (0x0007)  
#define WRITE_RCV_DELAY_L      (0x6590)
#define EPC_BLOCK_START_DELAY  (0x76590)
#define UHF_BEFORE_SIG_SND_TIME (460)
#endif


#define GetUhfMsgIndex(wMsgId)    ((wMsgId)-EV_USP_UHF_WORDK_MSG_START -1)

#define ReadLowLevelFrame(pwReadLen) uhf_readFpgaFrame(pwReadLen)

#define TraceToMacMsg(EV_Command)  if (usp_cli_isTraceByTno(USP_FPGA_TNO))\
{\
 uhf_SendTraceMsg(EV_RCP_TO_FPGA, EV_Command, 0, NULL);\
}


typedef WORD32 usp_uhf_Protocol_fun(T_Msg *,T_Msg *);

typedef struct tagTMsgFunDef
{
  WORD32 dwRcvMsgId;
  WORD32 dwSndMsgId;
  usp_uhf_Protocol_fun *ptProcFun;
  WORD16 wReceiverTno;
  WORD16 wPad;
}T_MsgFunDef;

typedef struct
{
    BYTE ucTurnOnFilter;
    BYTE ucPad;
    WORD16 wRSSIThreshHold;
} T_RssiCfg;

typedef struct
{
    BYTE   ucModeIndex;         /*�Ķ���Ĭ��Э���Ż����ñ�ÿ������һ��ֵ*/
    BYTE   ucAntStause;         /*�����Ƿ���λ*/
    WORD16 wPad;
    T_USPUHFEpcUserDefPara   tEpcUserDefPara;  
    T_USPUHFGBUserDefPara   tGBUserDefPara;    
    T_USPUHFRFModeCfg         tRfModeCfg;
    T_RssiCfg                tEPCReadRssiCfg;
    T_RssiCfg                tEPCWriteRssiCfg;
    T_RssiCfg                tISO6BReadRssiCfg;
    T_RssiCfg                tISO6BWriteRssiCfg;
    T_RssiCfg                tGBReadRssiCfg;
    T_RssiCfg                tGBWriteRssiCfg;
} T_AntCfgs;

typedef struct
{
   WORD16 wAntIndex;   /* ��ǰ�������� ����Ƶ����*/
   WORD16 wWorkChannel;
   /* ��ǰ����Ƶ��, ��λ��kHz,  */
   WORD32 dwWorkFreq;         
   T_AntCfgs atRfAntCfg[READER_ANTENNA_COUNT + 1];
} T_UhfCtrl;



WORD32 uhf_SendTraceMsg(WORD16 wSendOrRecv, WORD16 wMsgID, WORD16 ParaLen, WORD16 *pwPara);
WORD16 uhf_getInventQ(WORD16 wTagPopulation);


#endif

