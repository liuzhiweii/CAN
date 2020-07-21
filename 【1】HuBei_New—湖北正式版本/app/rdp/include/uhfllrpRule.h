#ifndef  UHF_LLRP_RULE_H
#define  UHF_LLRP_RULE_H
#if 1
#include "zmq.h"
#include "zmq_utils.h"
#include "parson.h"
#include "utimer.h"
#include "uhf.conf"
#include "usp_uhf_cfg.h"
#include "usp_uhf_epc.h"
#include "usp_uhf_iso6b.h"
#include "usp_uhf_gb.h"   // support for GB
#include "usp_uhf_err.h"
#include "usp_uhf.h"
#include "usp_ltk.h"
#include "usp_uhfllrpconfig.h"
#endif

#include "rdpprotocolpub.h"

 
 /*
 void llrp_addRo(T_Msg *ptMsg, LLRP_tSADD_ROSPEC *ptLLRP_tSADDROSpec);
 void llrp_deleteRo(T_Msg *ptMsg, LLRP_tSDELETE_ROSPEC  *ptLLRP_tSDELETE_ROSPEC);
 void llrp_enableRo(T_Msg *ptMsg, LLRP_tSENABLE_ROSPEC  *ptLLRP_tSENABLE_ROSPEC);
 void llrp_disableRo(T_Msg *ptMsg, LLRP_tSDISABLE_ROSPEC *ptLLRP_tSDISABLE_ROSPEC);
 void llrp_startRo(T_Msg *ptMsg, LLRP_tSSTART_ROSPEC *ptLLRP_tSSTART_ROSPEC);
 void llrp_stopRo(T_Msg *ptMsg, LLRP_tSSTOP_ROSPEC *ptLLRP_tSSTOP_ROSPEC);
 void llrp_addAo(T_Msg *ptMsg, LLRP_tSADD_ACCESSSPEC   *ptLLRP_tSADD_ACCESSSPEC);
 void llrp_deleteAo(T_Msg *ptMsg, LLRP_tSDELETE_ACCESSSPEC   *ptLLRP_tSDELETE_ACCESSSPEC);
 void llrp_disableAo(T_Msg *ptMsg, LLRP_tSDISABLE_ACCESSSPEC *ptLLRP_tSDISABLE_ACCESSSPEC);
 void llrp_enableAo(T_Msg *ptMsg, LLRP_tSENABLE_ACCESSSPEC *ptLLRP_tSENABLE_ACCESSSPEC);
 void llrp_getRo(T_Msg *ptRcvMsg, LLRP_tSGET_ROSPECS *ptLLRP_tSGET_ROSPECS);
 void llrp_getAo(T_Msg *ptRcvMsg, LLRP_tSGET_ACCESSSPECS *ptLLRP_tSGET_ACCESSSPECS);
 void llrp_getReport(T_Msg *ptRcvMsg,LLRP_tSGET_REPORT *ptLLRP_tSGET_REPORT);
 void llrp_externPeriodtimerForRoStartTrigger(T_Msg *ptRcvMsg);
 */
/**************************Rdp��ȫ�ֱ���********************************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/
/*Llrp�̶߳����ȫ�ֱ����ṹ*/
typedef struct tagT_RdpLlrpCtrl
{
    unsigned short int   wRDPLlrpState;
    unsigned int  bIsConfigChg;
    unsigned short int   wPad;
}T_RdpLlrpCtrl;

/**************************�Ķ������ȵ�������ݽṹ**************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/

typedef struct tagT_RdpLlrpROEntry
{
    unsigned int  dwROIndex;
    unsigned int   dwPriority;
    struct tagT_RdpLlrpROEntry *ptNext;
}T_RdpLlrpROEntry;




typedef struct tagT_RdpLlrpROActiveEntry 
{
    unsigned int   dwROIndex;
}T_RdpLlrpROActiveEntry;


typedef struct tagT_RdpLlrpAccessEntry
{
    unsigned int  dwAccessIndex;
    struct tagT_RdpLlrpAccessEntry *ptNext;
}T_RdpLlrpAccessEntry;

/******************    Ro��StartTrigger �������*******************************************************/

/*�յ�enable��ϢʱstartTrigger�Ĵ���period ��immdiate*/
typedef void ( *usp_rdp_llrp_EnableStartTrigger)(void*);
/*�ⲿ�����¼���trigger����*/
typedef void ( *usp_rdp_llrp_ExternalStartTrigger)(void *);
/*������Ro����ʱ���ظ���ʼ*/
typedef void ( *usp_rdp_llrp_PeriodStartTrigger)(void *);
/*���ݲ�ͬ��startTrigger����ͬ������*/
typedef void ( *usp_rdp_llrp_InitStartTrigger)(void  *);

typedef struct  tagT_RdpLlrpRoPeriodOfStartTrigger
{

    unsigned long long lwUTCTimestampMicroSec; 
    unsigned int dwOffset;
    unsigned int dwPeriod;
}T_RdpLlrpRoPeriodOfStartTrigger;


typedef struct tagT_RdpLlrpRoGpiOfStartTrigger
{
     unsigned int dwGPIPortNum;
     unsigned int  ucGPIEvent;
     unsigned int dwTimeout;
    
}T_RdpLlrpRoGpiOfStartTrigger;

typedef union  tagT_RdpLlrpRoParamOfStartTrigger
{
     T_RdpLlrpRoGpiOfStartTrigger        tRdpLlrpRoGpiOfStartTrigger;
     T_RdpLlrpRoPeriodOfStartTrigger    tRdpLlrpRoPeriodOfStartTrigger;
     
}T_RdpLlrpRoParamOfStartTrigger;

typedef struct tagT_RdpLlrpROStartTrigger
{
    unsigned int dwROSpecStartTriggerType;
    T_RdpLlrpRoParamOfStartTrigger         tRdpLlrpRoParamOfStartTrigger; 
    usp_rdp_llrp_InitStartTrigger            pfusp_rdp_llrp_InitStartTrigger;
    usp_rdp_llrp_EnableStartTrigger        pfusp_rdp_llrp_EnableStartTrigger;
    usp_rdp_llrp_ExternalStartTrigger      pfusp_rdp_llrp_ExternalStartTrigger;
    usp_rdp_llrp_PeriodStartTrigger         pfusp_rdp_llrp_PeriodStartTrigger;
}T_RdpLlrpROStartTrigger;

/******************    Ro��StopTrigger �������*****************************************************/

/*�ⲿ�����¼���trigger����*/
typedef void ( * usp_rdp_llrp_externalStoptTrigger)(void *);
/*ÿ��֡������AP��������ж�*/
typedef void ( * usp_rdp_llrp_endofAPDealforStopTrigger)(void*);
/*�ж�stopTrigger�Ƿ���*/
typedef unsigned int  ( * usp_rdp_llrp_judgeHappenforStopTrigger)(void *);
/*stopTrigger�ĳ�ʼ��*/
typedef void ( *usp_rdp_llrp_initStopTrigger)(void *);

typedef struct tagT_RdpLlrpRoGpiOfStopTrigger
{
     unsigned int dwGPIPortNum;
     unsigned int  ucGPIEvent;
     unsigned int dwTimeout;
    
}T_RdpLlrpRoGpiOfStopTrigger;


typedef struct  tagT_RdpLlrpRoDurationOfStopTrigger
{
    unsigned int dwDuration;   
    
}T_RdpLlrpRoDurationOfStopTrigger;



typedef union  tagT_RdpLlrpRoParamOfStopTrigger
{
     
     T_RdpLlrpRoGpiOfStopTrigger            tRdpLlrpRoGpiOfStopTrigger ;
     T_RdpLlrpRoDurationOfStopTrigger    tRdpLlrpRoDurationOfStopTrigger;
     
}T_RdpLlrpRoParamOfStopTrigger;


typedef struct tagT_RdpLlrpROStopTrigger   
{
    unsigned int dwROStopTriggerState;/*����Ϊ1 δ����Ϊ0*/    
    T_RdpLlrpRoParamOfStopTrigger                tRdpLlrpRoParamOfStopTrigger; 
    usp_rdp_llrp_initStopTrigger                          pfusp_rdp_llrp_initStopTrigger;
    usp_rdp_llrp_externalStoptTrigger                  pfusp_rdp_llrp_externalStoptTrigger;
    usp_rdp_llrp_endofAPDealforStopTrigger         pfusp_rdp_llrp_endofAPDealforStopTrigger;
    usp_rdp_llrp_judgeHappenforStopTrigger           pfusp_rdp_llrp_judgeHappenforStopTrigger;
    
}T_RdpLlrpROStopTrigger;

/**************************RO��Ao�ı���������ݽṹ***************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/


/*Access�����Ŀ��ƽṹ*/
typedef struct  T_RdpLlrpOpResultCtrl
{
    unsigned short int wOpSpecID; /*OpId*/ 
    unsigned short int wOpType; /*Op������*/
    unsigned short int wResult; 
    unsigned short int wOpResultAddr;
    unsigned short int wOpResultlen;
    unsigned short int wOpOriginalLen;
    unsigned short int wOpOriginalAddr;
    unsigned short int wPad;
}T_RdpLlrpOpResultCtrl;

typedef LLRP_tEAirProtocols EAirProtocols;



typedef struct tagT_RdpLlrpISOID
{
    unsigned short int wTagDataLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
    unsigned char      aucTagData[2];      /* �䳤 EPC ���ݵĵ�һ�����ݶ� */
    
}T_RdpLlrpISOID;


typedef struct  tagT_RdpLlrpEpcID
{
    unsigned short int wTagDataLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
    unsigned short int awTagData[1];      /* �䳤 EPC ���ݵĵ�һ�����ݶ� */
    
}T_RdpLlrpEpcID;

typedef struct  tagT_RdpLlrpGBID
{
    unsigned short int wTagDataLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
    unsigned short int awTagData[1];      /* �䳤 EPC ���ݵĵ�һ�����ݶ� */
}T_RdpLlrpGBID;

typedef union  tagT_RdpLlrpID
{
    T_RdpLlrpEpcID    tEpcID;
    T_RdpLlrpISOID    tISOID;
    T_RdpLlrpGBID    tGBID;
}T_RdpLlrpID;


typedef struct  tagT_RdpLlrpROTagReport
{
    unsigned int                  dwROSpecID;
    unsigned short int                  wSpecIndex;
    unsigned short int                  wInventoryParameterSpecID;
    unsigned short int                  wAntennaID;
    unsigned short int                  wPeakRSSI;
    unsigned short int                  wChannelIndex;
    unsigned long long                  lwFirstSeenTimestampUTC;
    unsigned long long                  lwLastSeenTimestampUTC;
    unsigned short int                  wTagCount;
    unsigned short int                  wPC_Bits;
    unsigned short int                  wCRC;
    unsigned int                  dwAccessSpecID; 
    EAirProtocols            eProtocolID;
    T_RdpLlrpID              tID;
    
}T_RdpLlrpROTagReport;


 #define USP_RDP_LLRP_UID_SIZE        (unsigned int)62


/*Ψһ�Ա�ʶ*/
typedef struct tagT_RdpLlrpUID
{
    unsigned short int wTagDataLen;    /*�ֽ�Ϊ��λ*/
    unsigned char      aucTagData[USP_RDP_LLRP_UID_SIZE];      
    
}T_RdpLlrpUID;

typedef struct  tagT_RdpLlrpAccessTagReport
{
    unsigned int                  dwROSpecID;
    unsigned short int                  wSpecIndex;
    unsigned short int                  wInventoryParameterSpecID;
    unsigned short int                  wAntennaID;
    unsigned short int                  wPeakRSSI;
    unsigned short int                  wChannelIndex;
    unsigned long long                  lwFirstSeenTimestampUTC;
    unsigned long long                  lwLastSeenTimestampUTC;
    unsigned short int                  wTagCount;
    unsigned short int                  wPC_Bits;
    unsigned short int                  wCRC;
    unsigned int                  dwAccessSpecID; 
    EAirProtocols            eProtocolID;
    unsigned int                    dwOpNum;
    T_RdpLlrpOpResultCtrl    tListofOpResultCtrl[MAX_NUM_OPSPECS_PER_ACCESSSPEC];
    T_RdpLlrpID        tID;
    unsigned char aucData[0];    
}T_RdpLlrpAccessTagReport;

typedef union  tagT_RdpLlrpTagReport
{
    T_RdpLlrpROTagReport         tROTagReport;
    T_RdpLlrpAccessTagReport   tAccessTagReport;
    
 }T_RdpLlrpTagReport;

typedef struct tagT_RdpLlrpSignalReportCtrl
{
    unsigned char                                  ucReportType; /*0 ROReport 1 AccessReport */
    unsigned char                                  ucROType; /*0 ���� 1 ���� */
    unsigned short int                                  wMsgLen;
    /*ÿ�����涼��Ψһ�Ա�ʶ��ֵ������*/
    T_RdpLlrpUID                      tUID;
    struct tagT_RdpLlrpSignalReportCtrl   *ptlast;
    struct tagT_RdpLlrpSignalReportCtrl   *ptNext;   
    T_RdpLlrpTagReport   tRdpLlrpTagReport;
}T_RdpLlrpSingleReportCtrl;

/* ���ڱ���RO���ϱ�����ΪNULL�Ļ��۱��������*/

typedef struct   tagT_RdpLlrpAcculateReportData
{
    /*��·��ʶ�Ϳ���*/
    unsigned char ucLinkIndex;
    unsigned char ucLinkType;
    unsigned char ucPad[2];
    unsigned int dwReportSize;  
    unsigned int dwReportNum;  	
    T_RdpLlrpSingleReportCtrl *ptAcculateReportHead;    
    T_RdpLlrpSingleReportCtrl *ptActiveRoReportTail;
}T_RdpLlrpAcculateReportData;


typedef struct  tagT_RdpLlrpSendReportContent
{
   unsigned int dwMessgeID;
   T_RdpLlrpSingleReportCtrl *ptReportAddr;
}T_RdpLlrpSendReportContent;

typedef struct  tagT_RdpAddAo
{
   /*Ao����ķ�ʽ*/
   unsigned int dwAOReportSndTrigger;
   LLRP_tSMessage *ptAddAoMsgAddr;
}T_RdpAddAo;


/*���������õĸ��ֲ������ؽ�����ڲ��ṹ*/

typedef struct   tagT_RdpLlrpEpcWriteOpResult
{
    LLRP_tEC1G2WriteResultType eResult;
    unsigned short int                               wOpSpecID;
    unsigned short int                               wNumWordsWritten;
}T_RdpLlrpEpcWriteOpResult;


typedef struct  tagT_RdpLlrpEpcReadOpResult
{
    LLRP_tEC1G2ReadResultType eResult;
    unsigned short int                              wOpSpecID;
    unsigned short int                              wDataLength;
    unsigned char                                    aucData[0];
}T_RdpLlrpEpcReadOpResult;

typedef struct   tagT_RdpLlrpEpcKillOpResult
{
    LLRP_tEC1G2KillResultType eResult;
    unsigned short int                            wOpSpecID;  
}T_RdpLlrpEpcKillOpResult;

typedef struct    tagT_RdpLlrpEpcLockOpResult
{
    LLRP_tEC1G2LockResultType eResult;
    unsigned short int                              wOpSpecID;
}T_RdpLlrpEpcLockOpResult;

typedef struct   tagT_RdpLlrpEpcBlcokLockOpResult
{
    unsigned short int     wOpSpecID;
    unsigned short int     wReadLock;
    unsigned int     dwResult; 
    unsigned short int     wLockStateLen;
    unsigned char          ucLockState[1];
    unsigned char          ucPad[1];  
}T_RdpLlrpEpcBlcokLockOpResult;

typedef struct  tagT_RdpLlrpEpcAlienReadLockOpResult
{
    unsigned short int     wOpSpecID;
    unsigned short int     wPad;
    unsigned int     dwResult; 

}T_RdpLlrpEpcAlienReadLockOpResult;


typedef struct  TagT_RdpLlrpEpcWriteVerifyResult
{
    unsigned short int  wOpSpecID;
    unsigned short int  wPad;
    unsigned int  dwResult;  
}T_RdpLlrpEpcWriteVerifyResult;
typedef struct   tagT_RdpLlrpISO6BReadOpResult
{
    unsigned char       ucResult;
    unsigned char       ucSuccessLength;
    unsigned short int   wOpSpecID;
    unsigned short int   wDataLength;
    unsigned char       aucData[2];
}T_RdpLlrpISO6BReadOpResult;

typedef struct   tagT_RdpLlrpISO6BWriteOpResult
{
    unsigned char       ucResult;
    unsigned char       ucPad[3];
    unsigned short int wOpSpecID;
    unsigned short int wNumWordsWritten;
}T_RdpLlrpISO6BWriteOpResult;

typedef struct  tagT_RdpLlrpISO6BLockOpResult
{
    unsigned char       ucResult;
    unsigned short int wOpSpecID;
    unsigned short int wDataLength;
    unsigned char       aucData[3];
}T_RdpLlrpISO6BLockOpResult;
  
typedef struct  tagT_RdpLlrpISO6BLockQueryOpResult
{
    unsigned short int  wOpSpecID;
    unsigned short int  wDataLength;
    unsigned char        aucData[0];
}T_RdpLlrpISO6BLockQueryOpResult;


typedef struct tagT_RdpLlrpEpcImpinjSetQTOpResult
{
    unsigned short int wOpSpecID;
    unsigned short int wResult;	
}T_RdpLlrpEpcImpinjSetQTOpResult;

typedef struct  tagT_RdpLlrpEpcImpinjGetQTOpResult
{
    unsigned short int wOpSpecID;
    unsigned short int wResult;	
    unsigned short int wDataProfile;	
    unsigned short int wAccessRange;		 
	
}T_RdpLlrpEpcImpinjGetQTOpResult;

typedef struct   tagT_RdpLlrpGBWriteOpResult
{
    unsigned int                               dwResult;
    unsigned short int                               wOpSpecID;
    unsigned short int                               wNumWordsWritten;
}T_RdpLlrpGBWriteOpResult;

typedef struct   tagT_RdpLlrpGBEraseOpResult
{
    unsigned int                               dwResult;
    unsigned short int                               wOpSpecID;
    unsigned short int                               wNumWordsErase;
}T_RdpLlrpGBEraseOpResult;


typedef struct  tagT_RdpLlrpGBReadOpResult
{
    unsigned int                               dwResult;
    unsigned short int                              wOpSpecID;
    unsigned short int                              wDataLength;
    unsigned char                                    aucData[0];
}T_RdpLlrpGBReadOpResult;

typedef struct   tagT_RdpLlrpGBKillOpResult
{
    unsigned int                               dwResult;
    unsigned short int                            wOpSpecID;  
}T_RdpLlrpGBKillOpResult;

typedef struct    tagT_RdpLlrpGBLockOpResult
{
    unsigned int                               dwResult;
    unsigned short int                              wOpSpecID;
}T_RdpLlrpGBLockOpResult;

/******************    Ro��report Trigger ����ر��������������********************/

 /*�ⲿ�����¼���trigger����*/
 
typedef void ( * usp_rdp_llrp_InitAIForReportTrigger) (void *);
typedef void ( * usp_rdp_llrp_InitROForReportTrigger)(void *); 

/*ÿ��֡������AP��������ж�*/
typedef unsigned int ( * usp_rdp_llrp_JudgeReportNumForReportTrigger)(void *);
/*�ж�stopTrigger�Ƿ���*/
typedef unsigned int  ( * usp_rdp_llrp_EndAIForReportTrigger)(void *);
/*stopTrigger�ĳ�ʼ��*/
typedef unsigned int ( *usp_rdp_llrp_EndROForReportTrigger)(void *);


typedef struct tagT_rdpLlrpROReport
{
    unsigned int                                                           bIsDefaultCfgFlag;
    unsigned short int                                                            wThresholdOfReportNum;  
    unsigned short int                                                            wPad;  
    T_USPRdpLlrpTagReportContentSelector               tRdpLlrpTagReportContentSelector ;   
    usp_rdp_llrp_InitAIForReportTrigger                   pfusp_rdp_llrp_InitAIForReportTrigger;
    usp_rdp_llrp_InitROForReportTrigger                  pfusp_rdp_llrp_InitROForReportTrigger;
    usp_rdp_llrp_JudgeReportNumForReportTrigger   pfusp_rdp_llrp_JudgeReportNumForReportTrigger;
    usp_rdp_llrp_EndAIForReportTrigger                   pfusp_rdp_llrp_EndAIForReportTrigger;
    usp_rdp_llrp_EndROForReportTrigger                  pfusp_rdp_llrp_EndROForReportTrigger;
    /*��ǰRo�����ı���*/
    T_RdpLlrpAcculateReportData                        tActiveRoReportLink;
}T_RdpLlrpROReport;      



typedef struct tagT_RdpLlrpDynamicRo
{
  struct tagT_RdpLlrpDynamicRo *ptNext;
  LLRP_tSADD_ROSPEC *ptAddRo;
}T_RdpLlrpDynamicRo;

typedef struct tagT_RdpLlrpDynamicAccess
{
  struct tagT_RdpLlrpDynamicAccess*ptNext;
  LLRP_tSADD_ACCESSSPEC *ptAddAccess;
}T_RdpLlrpDynamicAccess;
/*ȫ�ֱ�����Ӧ��������*/

/*�Ķ���������·����ı���ȴ�GetReport��Ϣ�ϱ�����*/





/**************************AI��stopTrigger�Ĳ�������************************************************/

 /*�ⲿ�����¼���trigger����*/
 
typedef void ( * usp_rdp_llrp_initAIForAIStopTrigger) (void   *);
 /*AI�����������ۣ�����n����ǩÿ�����ʱ ��ȡ*/
typedef void ( * usp_rdp_llrp_AccmulateForAIStopTrigger)(void *);  

/*AI��stopTrigger �ⲿ������������*/
typedef void ( * usp_rdp_llrp_ExternalTriggerForAIStopTrigger)(void *); 

/*ÿ��֡������AP��������ж�*/
typedef unsigned int ( * usp_rdp_llrp_judgeHappenForAIStopTrigger)(void *);

/*ÿ��AP�������ж�AI��������*/
typedef unsigned int  ( * usp_rdp_llrp_endAPForAIStopTrigger)(void *);
typedef unsigned int  ( * usp_rdp_llrp_AccmulateExecuteTimesForAIStopTrigger)(void *);

 
typedef struct   tagT_RdpLlrpAIParamForStopTrigger
{   
    
    unsigned short int  wAIExecuteTimes;
    unsigned short int  wNumberOfAttempts;
    unsigned short int  wNumberOfTags;
    unsigned short int  wAITagNum;  
    unsigned int  bAIStopTriggerStatus;
    unsigned long long  lwAIStartTime;
    unsigned long long  lwLastTagSeenTime;
    unsigned int  dwDuration;
    unsigned int  dwTimeOut;
    unsigned int  dwTms;
   
}T_RdpLlrpAIParamForStopTrigger;

typedef struct tagT_RdpLlrpAIGpiOfStopTrigger
{
     unsigned int dwGPIPortNum;
     unsigned int  ucGPIEvent;
     unsigned int dwTimeout;
    
}T_RdpLlrpAIGpiOfStopTrigger;
typedef struct   tagT_RdpLlrpAIStopTrigger
{              
    T_RdpLlrpAIParamForStopTrigger                     tRdpLlrpAIParamForStopTrigger;
    T_RdpLlrpAIGpiOfStopTrigger                          tRdpLlrpAIGpiOfStopTrigger;
    usp_rdp_llrp_initAIForAIStopTrigger                       pfusp_rdp_llrp_initAIForAIStopTrigger;
    usp_rdp_llrp_AccmulateForAIStopTrigger                pfusp_rdp_llrp_AccmulateForAIStopTrigger;
    usp_rdp_llrp_AccmulateExecuteTimesForAIStopTrigger  pfusp_rdp_llrp_AccmulateExecuteTimesForAIStopTrigger;
    usp_rdp_llrp_ExternalTriggerForAIStopTrigger         pfusp_rdp_llrp_ExternalTriggerForAIStopTrigger;
    usp_rdp_llrp_judgeHappenForAIStopTrigger            pfusp_rdp_llrp_judgeHappenForAIStopTrigger;
    usp_rdp_llrp_endAPForAIStopTrigger                      pfusp_rdp_llrp_endAPForAIStopTrigger;
    
}T_RdpLlrpAIStopTrigger;

/**************************���ߵ����ü̳п��ƽṹ************************************************/

typedef struct   tagT_RdpLlrpC12FilterCtrl
{
    unsigned int bIsDefaultCfgFlag;
    unsigned int bInventoryStateUnawareFilterActionCtrl[MaxEPCSelectNum];
}T_RdpLlrpC12FilterCtrl;

typedef struct   tagT_RdpLlrpISOInvCtrl
{
    unsigned int bIsDefaultCfgFlag;
    unsigned int bISOFilterControl;
    unsigned int bISORFControl;
    unsigned int bISOSingulationControl;
}T_RdpLlrpISOInvCtrl;

typedef struct   tagT_RdpLlrpC1G2InvCtrl

{
    unsigned int                         bIsDefaultCfgFlag;
    unsigned int                         bC1G2Filterctrl;
    unsigned int                         bC1G2RFCtrl;
    unsigned int                         bC1G2SingulationCtrl;
    T_RdpLlrpC12FilterCtrl    tRdpLlrpC12FilterCtrl;
}T_RdpLlrpC1G2InvCtrl;

typedef struct   tagT_RdpLlrpGBFilterCtrl
{
    unsigned int bIsDefaultCfgFlag;
    unsigned int bGBTagInventoryStateAwareFilterAction[MaxGBSortNum];
}T_RdpLlrpGBFilterCtrl;

typedef struct   tagT_RdpLlrpGBInvCtrl

{
    unsigned int                         bIsDefaultCfgFlag;
    unsigned int                         bGBFilterctrl;
    unsigned int                         bGBRFCtrl;
    unsigned int                         bGBSingulationCtrl;
    T_RdpLlrpGBFilterCtrl    tRdpLlrpGBFilterCtrl;
}T_RdpLlrpGBInvCtrl;

typedef struct   tagT_RdpLlrpAirProtocolInvCtrl
{
    T_RdpLlrpC1G2InvCtrl                  tRdpLlrpC1G2InvCtrl;
    T_RdpLlrpISOInvCtrl                     tRdpLlrpISOInvCtrl;
    T_RdpLlrpGBInvCtrl                  tRdpLlrpGBInvCtrl;
}T_RdpLlrpAirProtocolInvCtrl;


typedef struct   tagT_RdpLlrpAntConfigCtrl
{
    /*1 �̳� 0 Я��*/
    unsigned int                             bIsDefaultCfgFlag;
    unsigned int                             bRFT_RFReceiverCtrl;
    unsigned int                             bRFTransmitterCtrl;
    T_RdpLlrpAirProtocolInvCtrl     tRdpLlrpAirProtocolInvCtrl;
}T_RdpLlrpAntConfigCtrl;


/**************************AI�����ߺ�Э��Ĳ�������************************************************/
typedef struct  tagT_RdpLlrpProtocolInvSet
{
    unsigned short int                                                     wInvParamSpecID;
    unsigned short int                                                     wPad;
    EAirProtocols                                               eProtocolID;  
    T_USPRdpLlrpAntennaConfiguration                  tUSPRdpLlrpAntennaConfiguration;
    /*���������Ƿ�̳еĹ�ϵ*/
    T_RdpLlrpAntConfigCtrl                                  tRdpLlrpAntConfigCtrl;
}T_RdpLlrpProtocolInvSet;

typedef struct   tagT_RdpLlrpAntInv
{
    unsigned char                                     ucAntIndex;  
    unsigned char                                     ucProtocolNum;
    T_RdpLlrpProtocolInvSet           tRdpLlrpProtocolInvSet[MAX_NUM_INVENTORY_PARA_SPECS_PERAISPEC];
}T_RdpLlrpAntInv;

typedef struct   tagT_RdpLlrpAntsInvSet
{
    unsigned char                           ucAntNum;  /* ��������*/
    unsigned char                           ucPad[3];
    T_RdpLlrpAntInv     tAntInv[MAX_NUM_ANT_SUPPORT];
}T_RdpLlrpAntsInvSet;


/**************************AI���ڲ���������************************************************/

  typedef struct   tagT_RdpLlrpAI
{
    T_RdpLlrpAIStopTrigger    tRdpLlrpAIStopTrigger;
    T_RdpLlrpAntsInvSet       tRdpLlrpAntsInvSet;
}T_RdpLlrpAI;


typedef struct  tagT_RdpLlrpAISet
    
{
    unsigned short int  wAINum; //RO��specs������
    T_RdpLlrpAI  tRdpLlrpAI[MAX_NUM_SPECS_PERROSPEC];
}T_RdpLlrpAISet;



/**************************RO�ڲ���������******************************************************/

/*������ô����ֱ���ñ�׼�Ļ�������һ����ö�ٵ���ô����*/
typedef struct tagT_RdpLlrpROBoundary
{
    T_RdpLlrpROStartTrigger          tRdpLlrpROStartTrigger;
    T_RdpLlrpROStopTrigger           tRdpLlrpROStopTrigger;
}
T_RdpLlrpROBoundary;
 
typedef struct   tagT_RdpLlrpRO
{
    unsigned int dwROSpecID;
    unsigned char ucPriority;
    LLRP_tEROSpecState          eCurrentState;
    T_RdpLlrpROBoundary         tRdpLlrpROBoundary;
    T_RdpLlrpROReport             tRdpLlrpROReport;   
    T_RdpLlrpAISet                   tRdpLlrpAISet;   
     
}T_RdpLlrpRO;

/**************************RO�ڲ����ƽṹ*******************************************************/
typedef struct tagT_RdpLlrpRODispatchCtrl
{
   
    unsigned char       ucLinkIndex; /*�ж���·��ʶ*/
    unsigned char       ucLinkType; 
    unsigned int   wReceiverTno;
    unsigned int   wSenderTno;  
    unsigned char       ucPad[2];
    unsigned int dwUseing; //����ṹ�Ƿ��õ�	
    unsigned int dwDispatchStatus; /*����Ro�ĵ���״̬���������Ready��RO�ı�׼״̬*/
    LLRP_tEROSpecState  eCurrentState;
    unsigned int dwRevMessageID; /*��Enbleʱ��Ҫ��������MessageID ��Ȼ���������Ҫ������*/
    unsigned int dwDeleteRoMsgID; 
    unsigned int dwDisableRoMsgID; 
    unsigned int dwStopRoMsgID; 
    unsigned int dwDeleteRoID;     /*����ActiveRo��˵ Ҫ��¼��deleteRo��ID�ţ��Ա��������ɾ������ӦRo�Ľṹ*/
    unsigned int dwDisableRoID;     /*����ActiveRo��˵ Ҫ��¼��deleteRo��ID�ţ��Ա��������ɾ������ӦRo�Ľṹ*/
    unsigned long long lwStartuSec;
	
}T_RdpLlrpRODispatchCtrl;


typedef struct  tagT_RdpLlrpActiveROCtrlIndex
{
    /*�����õĵ�ǰ�������±���ǰ��Э���±�*/
    unsigned char ucWorkAIIndex;
    unsigned char ucWorkAntIndex;
    unsigned char ucWorkProtocolIndex;
    unsigned char ucPad[1];
}T_RdpLlrpActiveROCtrlIndex;
/***********************************Access stopTrigger���ڲ��ṹ***********************************************/

typedef void  ( * usp_rdp_llrp_initAOStopTrigger)(void *);
typedef unsigned int ( * usp_rdp_llrp_IncreaseConditionAndJudeEndForAOStopTrigger)(void *);


typedef struct    tagT_RdpLlrpAccessParamStopTrigger
{
    
    unsigned short int wOperationCountValue; /*ƥ��ɹ��Ĵ���*/
    unsigned short int wCurrentOperationValue;    
}T_RdpLlrpAccessParamStopTrigger;

typedef struct    tagT_RdpLlrpAccessStopTrigger
{  
    T_RdpLlrpAccessParamStopTrigger                                       tRdpLlrpAccessParamStopTrigger;
    usp_rdp_llrp_initAOStopTrigger                                            pfusp_rdp_llrp_initAOStopTrigger;
    usp_rdp_llrp_IncreaseConditionAndJudeEndForAOStopTrigger pfusp_rdp_llrp_IncreaseConditionAndJudeEndForAOStopTrigger;
}T_RdpLlrpAccessStopTrigger;

/***********************************Access Report �ڲ����Ⱥ����ݽṹ***********************************************/
typedef void ( * usp_rdp_llrp_sendOneAOReportForAOReportTrigger)(T_RdpLlrpSingleReportCtrl *);
/*���в���2��Ĭ�ϵ�����*/
typedef void  ( * usp_rdp_llrp_ConstructAOReportForAOReportTrigger)(T_RdpLlrpSingleReportCtrl *);



 /*���湹�������ǲ���Ҳ���Է�װ��Ro��Ao�ڲ����������װ AO��Ӧ����һ�ݱ�������ݵ�����
    ���챨��ĺ���ָ��*/
typedef struct tagT_RdpLlrpAccessReport
{
    unsigned int                                                                 bIsDefaultCfgFlag;
    /*ƥ��ɹ��Ϳ���һ�ݴ�Ro��AO��*/
    T_USPRdpLlrpTagReportContentSelector                   tUSPRdpLlrpTagReportContentSelector; 
    usp_rdp_llrp_sendOneAOReportForAOReportTrigger    pfusp_rdp_llrp_sendOneAOReportForAOReportTrigger;  
    usp_rdp_llrp_ConstructAOReportForAOReportTrigger pfusp_rdp_llrp_ConstructAOReportForAOReportTrigger; 
}T_RdpLlrpAccessReport;
 


/***************************Access command ���ڲ��ṹ**********************************************************/
#define MAX_PATTERN_Buffer  512

typedef unsigned char T_TagPatternBuffer[512];

#define MAX_OP_Buffer 300

typedef unsigned char T_OpBuffer[MAX_OP_Buffer];

#define MAX_TAG_PATTERN_NUM  2
typedef struct   tagT_RdpLlrpOPCtrl
{
    unsigned short int wOpType; 
    unsigned short int wOpID;
    unsigned short int wOpLen;
    unsigned short int wPad;
    unsigned int dwAccessPassword;
}T_RdpLlrpOPCtrl;    

 typedef struct   tagT_RdpLlrpAirProtocolTagCtrl
{
    unsigned int                  dwTagPatternNum;
    T_TagPatternBuffer   tTagPattern[MAX_TAG_PATTERN_NUM];  
}T_RdpLlrpAirProtocolTagCtrl;


 typedef struct  tagT_RdpLlrpListOPCtrlSet
{
    unsigned short int             wOPNum;
    unsigned short int             wOpIndex;
    T_RdpLlrpOPCtrl   tRdpLlrpOPCtr[MAX_NUM_OPSPECS_PER_ACCESSSPEC]; /*�洢�������*/
    T_OpBuffer         tSingleOp[MAX_NUM_OPSPECS_PER_ACCESSSPEC];
}T_RdpLlrpListOPCtrlSet;
 

typedef struct  tagT_RdpLlrpAccessCmdCtrl
{
    unsigned int                               bCircumstanceSetting;
    unsigned int                               bClientRequest;
    T_RdpLlrpAirProtocolTagCtrl       tAirProtocolTag;
    T_RdpLlrpListOPCtrlSet             tRdpLlrpListOPCtrlSet;
    T_RdpLlrpUID                          tUID;
}T_RdpLlrpAccessCmdCtrl;

typedef struct tagT_RdpLlrpClientRequestOpSet
{
    LLRP_tEAirProtocols eProtocolID; 
    T_RdpLlrpListOPCtrlSet tListOp;
    T_RdpLlrpUID tMatchUID;
}T_RdpLlrpClientRequestOpSet;


/**************************AO�ڲ��ṹ��������************************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/
typedef struct  tagT_RdpLlrpActiveAccessCtrlIndex
{
    /*�����õĵ�ǰ�������±���ǰ��Э���±�*/
    unsigned char ucWorkOpIndex;
    unsigned char ucPad[3];
}T_RdpLlrpActiveAccessCtrlIndex;

typedef struct   tagT_RdpLlrpAccess
{
   
    unsigned int                                    dwAccessSpecID;
    unsigned short int                                    wAntennaID;
    unsigned short int                                    wPad;
    unsigned int                                    dwROSpecID; /*0���еĶ�����*/
    LLRP_tEAirProtocols                     eProtocolID; /*EPC 1 18000-B 100*/
    /*access�ĵ�ǰ״̬ ������ʱ���յ�delete�����ת��״̬��
    ÿ��ƥ�䷵��ʱ���ȼ��״̬��������Ӧ����*/
    LLRP_tEAccessSpecState             eCurrentState;
    T_RdpLlrpAccessStopTrigger        tRdpLlrpAccessStopTrigger;
    T_RdpLlrpAccessReport               tRdpLlrpAccessReport;
    T_RdpLlrpAccessCmdCtrl             tRdpLlrpAccessCmdCtrl;
    /*�ռ�Ӧ�ò��������� ���Ծ�̬����*/
}T_RdpLlrpAccess;

typedef struct   tagT_RdpLlrpAccessDispatchCtrl
{
    unsigned int    ucUseing; //����ṹ�Ƿ��õ�
    unsigned char        ucLinkIndex; /*�ж���·��ʶ*/
    unsigned char        ucLinkType;
    unsigned int   wReceiverTno;
    unsigned int   wSenderTno;    
    unsigned int  dwMessageID; /*��¼���һ��AOָ��*/
    unsigned int  dwDeleteAccessMsgID; /*��¼���һ��AOָ��*/
    unsigned int  dwDisableAccessMsgID; /*��¼���һ��AOָ��*/
    unsigned int dwDeleteAccessID;     /*����ActiveAccess��˵ Ҫ��¼��deleteAccess��ID�ţ��Ա��������ɾ������ӦAccess�Ľṹ*/
    unsigned int dwDisableAccessID;     /*����ActiveAccess��˵ Ҫ��¼��deleteAccess��ID�ţ��Ա��������ɾ������ӦAccess�Ľṹ*/
    unsigned int  dwReportTrigger;
    unsigned int  dwDispatchStatus;
}T_RdpLlrpAccessDispatchCtrl;

typedef struct  tagT_RdpLlrpAccessCtrlAndParam
{
    T_RdpLlrpAccessDispatchCtrl   tRdpLlrpAccessDispatchCtrl;
    T_RdpLlrpActiveAccessCtrlIndex     tRdpLlrpActiveAccessCtrlIndex; 
    T_RdpLlrpUID                       tMatchUID;/*ƥ���EPC����ISO����*/
    T_RdpLlrpAccess                  tRdpLlrpAccess;
}T_RdpLlrpAccessCtrlAndParam;

/**************************RO�ڲ��ṹ��������************************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/
typedef struct  tagT_RdpLlrpROCtrlAndParam
{
    T_RdpLlrpRODispatchCtrl      tRdpLlrpRODispatchCtrl;
    T_RdpLlrpActiveROCtrlIndex tRdpLlrpActiveROCtrlIndex;
    T_RdpLlrpAccessCtrlAndParam *ptMatchAccessCtrlAndPara;
    T_RdpLlrpRO                       tRdpLlrpROSpec;
}T_RdpLlrpROCtrlAndParam;

typedef struct  TagT_RdpLlrpReaderRoData
{
      T_RdpLlrpROEntry          tAddQueuehead;  
      T_RdpLlrpROEntry          tAddQueuetail;  
      T_RdpLlrpROEntry           tDisableRoQueuehead;
      T_RdpLlrpROEntry           tInactiveRoQueuehead;   
      T_RdpLlrpROEntry           tReadyRoQueuehead; 
      T_RdpLlrpROEntry           tActiveRO;   
      T_RdpLlrpROCtrlAndParam   tRdpLlrpROCtrlAndParam[MAX_NUM_RO_SPEC];
}T_RdpLlrpReaderRoData;

typedef struct  TagT_RdpLlrpReaderAccessData
{
      T_RdpLlrpAccessEntry              tAddAccessQueue;   
      T_RdpLlrpAccessEntry              tEnableAccessQueue;
      T_RdpLlrpAccessEntry              tRunAccess;
      T_RdpLlrpAccessCtrlAndParam   tRdpLlrpAccessCtrlAndParam[MAX_NUM_ACCESSSPECS];
}T_RdpLlrpReaderAccessData;

/*Ro���ȵ�������ݽṹ*/
typedef void ( *Add_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
typedef void ( *Enable_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
typedef void ( *Disable_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
typedef void ( *Start_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
typedef void ( *Stop_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
typedef void ( *Delete_RO)(T_Msg *,void *,T_RdpLlrpReaderRoData *);
 typedef void ( *Add_Access)(T_Msg *,void *,T_RdpLlrpReaderAccessData *);
 typedef void ( *Enable_Access)(T_Msg *,void *,T_RdpLlrpReaderAccessData *);
 typedef void ( *Disable_Access)(T_Msg *,void *,T_RdpLlrpReaderAccessData *);
 typedef void ( *Delete_Access)(T_Msg *,void *,T_RdpLlrpReaderAccessData *);
 /*ɨ��Ro���뵽����״̬*/
typedef void ( *ScanReadyROToRun)(T_RdpLlrpReaderRoData *);
 
/*RO�ĵ��ȶ��� */
typedef struct  tagT_RdpLlrpReaderDispatch
{
      Add_RO      pfAdd_RO;   
      Enable_RO  pfEnable_RO;
      Disable_RO pfDisable_RO;   
      Start_RO    pfStart_RO;   
      Stop_RO    pfStop_RO;   
      Delete_RO pfDelete_RO;   
      Add_Access   *pfAdd_Access;
      Enable_Access   *pfEnable_Access;
      Disable_Access   *pfDisable_Access;
      Delete_Access    *pfDelete_Access;
      
      ScanReadyROToRun            pfScanROToRun;
      T_RdpLlrpReaderAccessData *ptReaderAccessSpecData;            
      T_RdpLlrpReaderRoData        *ptReaderRoSpecData;
}T_RdpLlrpReaderDispatch;
     

/**************************RO��Ao�ı���������ݽṹ***************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/

typedef struct  tagT_RdpLlrpC1G2Read
{
    unsigned int     dwRCPCommand;
    unsigned char          ucMB;
    unsigned char           ucPad;
    unsigned short int     wOpSpecID;
    unsigned int     dwAccessPassword;
    unsigned short int     wWordPointer;
    unsigned short int     wWordCount;
   
}T_RdpLlrpC1G2Read;


typedef struct  tagT_RdpLlrpC1G2Write
{ 
    unsigned int     dwRCPCommand;
    unsigned short int    wOpSpecID;
    unsigned char         ucMB;
    unsigned char         ucIsBlockWrite;   /*1 ʹ�ÿ�д 0 ��ͨд*/ 
    unsigned char         wPad[2];
    unsigned int   dwAccessPassword;
    unsigned short int   wWordPointer;
    unsigned short int   wDataLength;
      
    unsigned char        aucData[0]; /*�䳤д������*/
}T_RdpLlrpC1G2Write;

typedef struct  tagT_RdpLlrpC1G2LockPayload
{
    LLRP_tEC1G2LockPrivilege  Privilege;
    LLRP_tEC1G2LockDataField DataField;
}T_RdpLlrpC1G2LockPayload;

typedef struct   tagT_RdpLlrpC1G2LockPayloadList
{
    unsigned short int                            wPayLoadNum;
    unsigned short int                            wPad;
    T_RdpLlrpC1G2LockPayload tRdpLlrpC1G2LockPayLoad[5];
}T_RdpLlrpC1G2LockPayloadList;

typedef struct   tagT_RdpLlrpC1G2Lock
{
     unsigned int                                dwRCPCommand;
    unsigned short int                                 wOpSpecID;
    unsigned short int                                 wPad;
    unsigned int                                 dwAccessPassword;
    T_RdpLlrpC1G2LockPayloadList tRdpLlrpC1G2LockPayLoad;   /*���ͽ��������Ķ�����*/
}T_RdpLlrpC1G2Lock;

typedef struct   tagT_RdpLlrpC1G2Kill
{
    unsigned int     dwRCPCommand;
    unsigned short int wOpSpecID;
    unsigned short int wPad;
    unsigned int wKillPassword;
}T_RdpLlrpC1G2Kill;
 
typedef struct   tagT_RdpLlrpC1G2BlockLock
{
    unsigned int     dwRCPCommand;
    unsigned char        ucMB;
    unsigned char        ucReadLock;
    unsigned short int  wOpSpecID;
    unsigned short int  ucRFU;
    unsigned short int  wBlockPtr;
    unsigned short int  wBlockRange;
    unsigned int  dwAccessPassword;
    unsigned char       ucMask[512];
}T_RdpLlrpC1G2BlockLock;

/*ALEN ר�нӿڶ�����*/
 
typedef struct   tagT_RdpLlrpC1G2ReadLock
{
    unsigned int     dwRCPCommand;
    unsigned short int      wOpSpecID;
    unsigned char          ucMask;
    unsigned char          ucPad;
    unsigned int    dwAccessPassword;
}T_RdpLlrpC1G2ReadLock;

 typedef struct T_RdpLlrpC1G2IPJQT
{
   unsigned short int wOpSpecID;
   unsigned short int wDataProfile;
   unsigned short int wAccessRange;
   unsigned short int wPersistence;
   unsigned short int wReadOrWrite;
   unsigned short int wpad;
   unsigned int dwAccessPassword;
   unsigned int dwRCPCommand;
}T_RdpLlrpC1G2IPJQT;
/* ISO6B Access ����*/

typedef struct tagISO6BTagId
{
    unsigned short int wTagDataLen;    /* �䳤 UID ���ݵĵ�һ�����ݶΣ�������wUID_bits_Len�ֶξ���,Ŀǰ����Ϊ64bit   */
    unsigned char aucTagData[8];
}
T_ISO6BTagId;

typedef struct tagISO6BReadControl
{
    unsigned char ucCommandType;
    unsigned char ucRetryTimes;
}
T_ISO6BReadControl;

typedef struct tagISO6BRead
{
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
    T_ISO6BReadControl t6BReadControl;
    T_ISO6BTagId tTagId;
}
T_ISO6BRead;

typedef struct tagISO6BWriteControl
{
    unsigned char ucCommandType;
    unsigned char ucRetryTimes;
    unsigned char ucVerifyTimes;
}
T_ISO6BWriteControl;

typedef struct tagT_ISO6BWrite
{
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
    unsigned char aucWriteData[256];  /*Ҫд�������*/
    T_ISO6BWriteControl t6BWriteControl;
    T_ISO6BTagId tTagId;
}T_ISO6BWrite;


typedef struct tagT_ISO6BLockControl
{
    unsigned char ucRetryTimes;
    unsigned char ucVerifyTimes;
}T_ISO6BLockControl;

typedef struct tagT_ISO6BLock
{
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
    T_ISO6BLockControl t6BLockControl;
    T_ISO6BTagId tTagId;
}T_ISO6BLock;


typedef struct tagT_ISO6BQueryLockControl
{
    unsigned char ucRetryTimes;
}
T_ISO6BQueryLockControl;

typedef struct tagISO6BQueryLock
{
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
    T_ISO6BQueryLockControl t6BQueryLockControl;
    T_ISO6BTagId tTagId;
}T_ISO6BQueryLock;


typedef struct  tagT_RdpLlrpClientRequestOpSpec
{
    unsigned short int wOpSpecID;
    unsigned short int wPad;
}T_RdpLlrpClientRequestOpSpec;

//���겿��
typedef struct  tagT_RdpLlrpGBRead
{
    unsigned char          ucMB;
    unsigned char           ucPad;
    unsigned short int     wOpSpecID;
    unsigned int     dwReadPassword;
    unsigned short int     wWordPointer;
    unsigned short int     wWordCount;
   
}T_RdpLlrpGBRead;


typedef struct  tagT_RdpLlrpGBWrite
{ 
    unsigned short int    wOpSpecID;
    unsigned char         ucMB;
    unsigned char         aucPad[3];
    unsigned int   dwWritePassword;
    unsigned short int   wWordPointer;
    unsigned short int   wDataLength;
    unsigned char        aucData[0]; /*�䳤д������*/
}T_RdpLlrpGBWrite;

typedef struct  tagT_RdpLlrpGBErase
{ 
    unsigned short int    wOpSpecID;
    unsigned char         ucMB;
    unsigned char         aucPad[3];
    unsigned int   dwWritePassword;
    unsigned short int   wWordPointer;
    unsigned short int   wWordCount;
}T_RdpLlrpGBErase;


typedef union   tagT_RdpLlrpGBLockAction
{
    unsigned char                            ucGBLockMemoryMode;
    unsigned char                            ucGBLockSecureMode;
    unsigned short int                      wPad;
}T_RdpLlrpGBLockAction;


typedef struct   tagT_RdpLlrpGBLock
{
    unsigned short int                                 wOpSpecID;
    unsigned char                                       ucMB;
    unsigned char                                       ucConfig;
    unsigned int                                 dwLockPassword;
    T_RdpLlrpGBLockAction           tRdpLlrpGBLockAction;   /*����������Ϊ*/
}T_RdpLlrpGBLock;

typedef struct   tagT_RdpLlrpGBKill
{
    unsigned short int wOpSpecID;
    unsigned short int wPad;
    unsigned int dwKillPassword;
}T_RdpLlrpGBKill;

/**************************************Ao��ƥ�����***************************/
/*
typedef union  tagT_RdpLlrpAirProtocolTag
{
    T_RdpLlrpC1G2TargetTag  tRdpLlrpC1G2TargetTag;
    T_RdpLlrpISO6BTag          tIRdpLlrpISO6BTag;
}T_RdpLlrpAirProtocolTag;
*/
/*
typedef struct  tagT_RdpLlrpC1G2Tag
{
    unsigned int                           dwC1G2TargetTagNum; 
    T_RdpLlrpC1G2TargetTag   tRdpLlrpC1G2TargetTag[MaxTagPatternNUM];
}T_RdpLlrpC1G2Tag;

*/

/*
typedef struct  tagT_RdpLlrpISO6BTag
{
    unsigned int                          dwISO6BTargetTagNum;
    T_RdpLlrpISO6BTargetTag tRdpLlrpISO6BTargetTag[MaxTagPatternNUM];
}T_RdpLlrpISO6BTag;
*/

typedef struct  T_RdpLlrpMatchParam
{
    unsigned short int wMaskBitCount;
    unsigned short int wDataBitCount;
    unsigned char      aucTagData[2];/*�ֽڶ���д��2��ԭ����1���Դ��������Ӱ��*/
}T_RdpLlrpMatchParam;

typedef struct   tagT_RdpLlrpC1G2TargetTag
{
    unsigned char                           ucMB;
    unsigned char                           ucPad;
    unsigned int                       bMatch;
    unsigned short int                      wPointer; /*��ʼƥ���ָ�� bitָ��*/
    /*ƥ����������*/
    T_RdpLlrpMatchParam   tRdpLlrpMatchParam;
}T_RdpLlrpC1G2TargetTag;

typedef struct   tagT_RdpLlrpISO6BTargetTag
{
    unsigned short int       wBytePtr;
    unsigned short int       wPad;
    unsigned int      bMatch;
    T_RdpLlrpMatchParam  tRdpLlrpMatchParam;
}T_RdpLlrpISO6BTargetTag;

typedef struct   tagT_RdpLlrpGBTargetTag
{
    unsigned char                           ucMB;
    unsigned char                           ucPad;
    unsigned int                       bMatch;
    unsigned short int                      wPointer; /*��ʼƥ���ָ�� bitָ��*/
    /*ƥ����������*/
    T_RdpLlrpMatchParam   tRdpLlrpMatchParam;
}T_RdpLlrpGBTargetTag;

/*typedef struct
{
    unsigned short int antennaId;
    unsigned short int ForDataRate;
    unsigned short int RevDataRate;
    EM_value_Type eM_value;
    EForward_link_Type eForward_link_modulation;
    unsigned short int PIE_Value;
} T_RFModeCfg;*/

/*��������ϢӦ��*/
typedef struct tagT_RdpLlrpRuleRsp 
{
    unsigned int   dwRuleID;
    LLRP_tSERROR_MESSAGE *ptRuleRspAddr;
   
}T_RdpLlrpRuleRsp;

/*************************�����ӿڵ�����******************************************************
*                                                                                                                                                                                *
*                                                                                                                                                                                *
*****************************************************************************************************/
 void llrp_addRo(T_Msg *ptMsg);
 void llrp_deleteRo(T_Msg *ptMsg);
 void llrp_enableRo(T_Msg *ptMsg);
 void llrp_disableRo(T_Msg *ptMsg);
 void llrp_startRo(T_Msg *ptMsg);
 void llrp_stopRo(T_Msg *ptMsg);
 void llrp_addAo(T_Msg *ptMsg);
 void llrp_deleteAo(T_Msg *ptMsg);
 void llrp_disableAo(T_Msg *ptMsg);
 void llrp_enableAo(T_Msg *ptMsg);
 void llrp_getRo(T_Msg *ptRcvMsg);
 void llrp_getAo(T_Msg *ptRcvMsg);
 void llrp_getReport(T_Msg *ptRcvMsg);
 void llrp_externPeriodtimerForRoStartTrigger(T_Msg *ptRcvMsg);
void llrp_reportReaderEventNotification(LLRP_tENotificationEventType eEventType,LLRP_tEROSpecEventType eSubEventType);
unsigned int llrp_sendRuleRspMsg(T_Msg *ptRcvMsg, LLRP_tEStatusCode etLLRP_tEStatusCode, unsigned int dwSndMsgID, unsigned int dwMsgSeqID,unsigned int dwRuleID);
void llrp_dealExternGpiForReader(T_Msg *ptRcvMsg);
void llrp_transferAccessEpcOpPara( T_RdpLlrpListOPCtrlSet *ptListOpSet, LLRP_tSParameter *ptTempParameter);
void llrp_transferAccessISO6BOpPara( T_RdpLlrpListOPCtrlSet *ptListOpSet, LLRP_tSParameter *ptTempParameter);
void llrp_transferAccessGBOpPara( T_RdpLlrpListOPCtrlSet *ptListOpSet, LLRP_tSParameter *ptTempParameter);
void llrp_dealBootTimerPollingTimer(T_Msg *ptRcvMsg);
 unsigned long long  llrp_getPPC8313BootCurrMsTime();
 unsigned long long  llrp_getPPC82xxBootCurrMsTime();
 void llrp_clearAccessListforGetAccessRsp(unsigned char ucLinkIndex);

#endif
