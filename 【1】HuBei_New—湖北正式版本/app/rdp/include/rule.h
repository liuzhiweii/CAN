/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� Report.h
* �ļ���ʶ�� 
* ����ժҪ��reportģ���ڲ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lzy
* ������ڣ� 2012.02.26
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _RDP_RULE_H
#define _RDP_RULE_H


/**************************************************************************
 *                        ����                                            *
 **************************************************************************/
 #define RDP_CLIENT_REQUEST_OPSPEC_ID 38975
 #define RDP_MAX_LLRP_DEPTH  8

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
 /*��Ϣ�Ƿ������*/
#define  RULE_SENG_MESSAGE_TO_LLRP_YES 0
#define  RULE_SENG_MESSAGE_TO_LLRP_NO 1

/*Ψһ��ʶ����*/
#define RDP_IDENTIFY_TYPE_INVENTORY  0
#define RDP_IDENTIFY_TYPE_READOP 1

/*op����*/
#define RDP_LLRP_C1G2_OPRESULTTYPE_READ         349
#define RDP_LLRP_ISO6B_OPRESULTTYPE_READ         645
#define RDP_LLRP_GB_OPRESULTTYPE_READ         765

#define RDP_LLRP_TAGSPEC_TYPE_EPC 338
#define RDP_LLRP_TAGSPEC_TYPE_ISO6B 635
#define RDP_LLRP_TAGSPEC_TYPE_GB 756

/*�����б�ǩ��Э������*/
#define RDP_LLRP_TAG_C1G2        241
#define RDP_LLRP_TAG_ISO6B       626
#define RDP_LLRP_TAG_GB       771


/*�շ���Ϣ*/
#define     RULE_SEND_MSG(socket, msg)     (zmq_send(socket, msg,(T_Msg *)msg->wMsgLen+sizeof(T_Msg),0)<0?1:0)
#define     RULE_RECV_MSG(socket, buf,len) zmq_recv(socket, buf, len, 0)

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
 /*����У������������*/
 typedef struct tagT_ParameterError
{  
    unsigned int bIsLastField;/*���һ�������Ƿ���field ����*/
    unsigned short int wFieldNum;/*field ���*/
    unsigned short int wLastParameterErrorCode;/*���һ����������Ĵ�����*/
    unsigned short int wFieldErrorCode;/*field����Ĵ�����*/
    unsigned short int wParameterDepth;/*�����ݹ����*/
    unsigned short int wParameterType[7];/*ÿ�����������*/
    unsigned char ErrorDetail[100];/*��������*/
}T_ParameterError;

/*AOΨһ��ʶ��Ӧ��*/
typedef struct tagT_RuleAOIdentifyTable
{
    unsigned char ucIdentifyType;/*0,��㣬1��op*/
    unsigned char ucOpSpecId;/*op id*/
    unsigned char ucOpIndex;/*Ψһ��ʶ���ڵ�opIndex*/
    unsigned char ucPad;
}T_RuleAOIdentifyTable;

/*AO��̬����*/
typedef struct tagT_RuleAODenymicList
{
    unsigned int dwAccessOriginalState;    /*AO״̬*/
    T_RuleAOIdentifyTable tRuleAOIdentifyTable;/*AOΨһ��ʶ��Ӧ*/
     LLRP_tSADD_ACCESSSPEC * pAddAccessspec; /*AO��̬�ṹ*/
    struct tagT_RuleAODenymicList *ptNext;/*��һ�����*/
}T_RuleAODenymicList;

/*�¼���enable�󷵻����⣻enable�ȷ��أ�����start���⣬
 ��ҪRULE��LLRP�߳�ͬ�����������˽ṹ����ROУ����ƽṹ����RULEģ��*/
typedef struct tagT_RuleROCtrlAndParam
{
   
    unsigned char       ucLinkIndex; /*�ж���·��ʶ*/
    unsigned char       ucLinkType; 
    unsigned char       ucPad[2];
    unsigned int   dwROSpecID;
    unsigned int   dwUseing; //����ṹ�Ƿ��õ�	
    LLRP_tEROSpecState  eCurrentState;
}T_RuleROCtrlAndParam;

/**************************************************************************
 *                             ������                                      *
 **************************************************************************/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
extern T_RdpOpFilterHashEntry *g_ptRdpOpFilterHashEntry;
extern unsigned int g_bReportInitFinish;
extern T_USPRdpAdvancedConfig g_tRdpAdvancedConfig;
/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
unsigned int usp_rdp_rule_Entry();
unsigned int rule_checkAddAccessspec(T_Msg *ptMsg , LLRP_tSADD_ACCESSSPEC *ptLLRP_tSADD_ACCESSSPEC);
unsigned int rule_checkDeleteAccesssSpec(T_Msg *ptMsg, LLRP_tSDELETE_ACCESSSPEC *ptLLRP_tSDELETE_ACCESSSPEC);
unsigned int rule_checkDeleteROSpec(T_Msg *ptMsg, LLRP_tSDELETE_ROSPEC *ptLLRP_tSDELETE_ROSPEC);
unsigned int rule_checkAddROSpec(T_Msg *ptMsg, LLRP_tSADD_ROSPEC *ptLLRPAddRospec);
unsigned int rule_checkDisableAccesssSpec(T_Msg *ptMsg, LLRP_tSDISABLE_ACCESSSPEC *ptLLRP_tSDISABLE_ACCESSSPEC);
unsigned int rule_checkDisableROSpec(T_Msg *ptMsg, LLRP_tSDISABLE_ROSPEC *ptLLRP_tSDISABLE_ROSPEC);
unsigned int rule_checkEnableAccesssSpec(T_Msg *ptMsg, LLRP_tSENABLE_ACCESSSPEC *ptLLRP_tSENABLE_ACCESSSPEC);
unsigned int rule_checkEnableROSpec(T_Msg *ptMsg, LLRP_tSENABLE_ROSPEC *ptLLRP_tSENABLE_ROSPEC);
unsigned int rule_checkStopROSpec(T_Msg *ptMsg, LLRP_tSSTOP_ROSPEC *ptLLRP_tSSTOP_ROSPEC);
unsigned int rule_checkStartROSpec(T_Msg *ptMsg, LLRP_tSSTART_ROSPEC *ptLLRP_tSSTART_ROSPEC);
unsigned int rule_SendMsgToClient(T_Msg *ptReceiveMsg, unsigned int dwMessageID , LLRP_tSMessage *ptLLRP_tSMessage);

#endif  /* _RDP_REPORT_H */

