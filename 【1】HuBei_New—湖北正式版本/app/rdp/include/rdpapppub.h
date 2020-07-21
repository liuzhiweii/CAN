/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� UhfRdp.h
* �ļ���ʶ�� 
* ����ժҪ��RDP �ڲ�����ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lzy
* ������ڣ� 2012.03.08
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _UHF_RDP_H
#define _UHF_RDP_H
#include "uhfllrpRule.h"

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
 /*ɾ��RO��Ӧ�Ĺ��˽��*/
#define RDP_DELETE_FILTER_TYPE_RO  1

 /*ɾ��AO��Ӧ�Ĺ��˽��*/
#define RDP_DELETE_FILTER_TYPE_AO  2

#define MAX_GPI_NUM 7
#define MAX_GPO_NUM 7


/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
 
 /*ɾ�����˽����Ϣ�ṹ��*/
 typedef struct tagT_RdpDeleteFilterNodes
{
    unsigned char ucDeleteType;         /*ɾ������0:RO,1:AO*/
    unsigned char ucPad[3];
    unsigned int dwRoOrAoId;     /*RO����AO��Ӧ��ID*/
}T_RdpDeleteFilterNodes;

typedef struct tagT_WriteOpFilter
{
    unsigned short int wAddress;
    unsigned short int wLength;

}
T_RemainWriteOp;

typedef struct tagT_ReadOpFilter
{

    unsigned short int wAddress;
    unsigned short int wLength;

}
T_RemainReadOp;

typedef union tagT_RemainOp
{
    T_RemainReadOp tRemainReadOp;
    T_RemainWriteOp tRemainWriteOp;
}T_RemainOp;

/*ÿ��op�Ľ����Ϣ*/
typedef struct tagT_RdpOpResult
{
    unsigned char ucOpResult;/*OP�������*/
    T_RemainOp tRemainOp;/*opʣ���д����*/
}T_RdpOpResult;

/*����������op�����Ϣ*/
typedef struct tagT_RdpOpFilterControl
{
    unsigned char ucOpNum;/*op����*/
    unsigned char ucPad;
    unsigned short int wPad;
    T_RdpOpResult tOpResult[MAX_NUM_OPSPECS_PER_ACCESSSPEC];/*ÿ��op�Ĳ������*/
}T_RdpOpFilterCtrl;


/*�������˽��ṹ*/
typedef struct tagT_RdpOpFilterHashEntry
{
     struct tagT_RdpOpFilterHashEntry *ptNext;/*ָ����һ�����*/
     struct tagT_RdpOpFilterHashEntry *ptLast;/*ָ��ǰһ�����*/
    T_RdpOpFilterCtrl tOpFilterCtrl;/*op������Ϣ*/
    unsigned long long dwCreatTime;/*�������˽�㴴��ʱ��*/
    unsigned long long dwOperateTime;/*�������˽�����ʱ��*/
    T_RdpLlrpSingleReportCtrl              *ptSignalReportCtrl;/*��㱨��*/
    unsigned char                                           ucLinkIndex;/*��·����*/
    unsigned char                                           ucLinkType;/*��·����*/
    unsigned short int                        wHasSend;
    unsigned int                         dwAccessId;/*AO id*/
    unsigned int                         dwMessageID;/*Messge id*/
    unsigned short int                         wSendTno;/*���̨���͵�ģ���*/
    T_RdpLlrpUID                        tRdpLlrpUID;/*Ψһ��ʶ*/
}T_RdpOpFilterHashEntry;


/**************************************************************************
 *                             ������                                      *
 **************************************************************************/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
extern unsigned int g_bRdpPowerOnCfgFinish;
extern T_RdpConfig g_tRdpConfig;
extern T_RdpLlrpReaderDispatch g_tRdpLlrpReaderDispatch;
/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
unsigned int rdp_calcHashKey(T_RdpLlrpUID *ptRdpLlrpUID, unsigned short int HashTableLength);

void report_constructAccessReport(LLRP_tSTagReportData *ptTagReportData,T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl);
void llrp_buildAnNodeFoDynamicAccessQueue(T_RdpLlrpDynamicAccess *ptQueueHead,LLRP_tSADD_ACCESSSPEC *ptAddAccessAddr);
void  llrp_deleteAnNodeForDynamicAccessQueue(T_RdpLlrpDynamicAccess *ptQueueHead,unsigned int dwAccessID);

#endif  /* _RDP_REPORT_H */

