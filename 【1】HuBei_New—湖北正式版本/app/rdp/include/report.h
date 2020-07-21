/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� Report.h
* �ļ���ʶ�� 
* ����ժҪ��reportģ���ڲ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lzy
* ������ڣ� 2012.03.07
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _RDP_REPORT_H
#define _RDP_REPORT_H


/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
#define INVENTORY_PERIOD                                    (unsigned int)0
#define INVENTORY_SINGLE                                    (unsigned int)1
#define INDENTIFIER_INVENTORY                               (unsigned int)0
#define INDENTIDIER_READ                                    (unsigned int)1


 /*ɾ��RO��Ӧ�Ĺ��˽��*/
#define RDP_DELETE_FILTER_TYPE_RO  1

 /*ɾ��AO��Ӧ�Ĺ��˽��*/
#define RDP_DELETE_FILTER_TYPE_AO  2

/*����RSSI�����������*/
#define RSSI_SORT_NUM          5

#define REPORT_HAS_SEND_REPORT 1

#define REPORT_HAS_NOT_SEND_REPORT 0

/*RSSI��16λ���8λ�з���ֵת�����賣��*/
#define REPORT_RSSI_CONSTANT_VALUE 85

/*��ǩ������ѯʱ��*/
#define TAGFILTER_OPPOLLTIME 20
#define TAGFILTER_RFPOLLTIME 20
#define TAGFILTER_RFOPERATETIME 5
/*�շ���Ϣ����*/
#define     REPORT_SEND_MSG(socket, msg)     zmq_send(socket, msg,(T_Msg *)msg->wMsgLen,0)
#define     REPORT_RECV_MSG(socket, buf,len) zmq_recv(socket, buf, len, 0)

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/

/*�ϱ������ڲ��ṹ�� */
typedef struct tagT_RdpReportFilterHashEntry
{
    struct tagT_RdpReportFilterHashEntry *ptNext;/*ָ����һ�����*/
    struct tagT_RdpReportFilterHashEntry *ptLast;/*ָ��ǰһ�����*/
    unsigned short int wNumOfOpSpect;/*op����*/
    unsigned short int wAllOpResult;/*AO������op�Ĳ������ָʾ*/
    unsigned int dwOpResult[8];/*ÿ��op�Ĳ������*/
    unsigned long long dwCreatTime;/*��㴴��ʱ��*/
    unsigned int dwAccessId;/*ao id*/
    unsigned int dwROSpecId;/*ro id*/
    T_RdpLlrpUID tRdpLlrpUID;/*Ψһ��ʶ*/
}T_RdpTagReportFilterHashEntry;

/*Rssi ��ز����ݽṹ */
/*Rssi����Ķ���*/
typedef struct tagT_RdpRssiQueueNode
{
    unsigned short int wRssiAccmulation;/*Rssi�ۼ�ֵ*/
    unsigned short int wPad;
    unsigned int dwRssiBuildTime;/*����ʱ��*/
    struct tagT_RdpRssiQueueNode *ptNext;/*ָ����һ�����*/
    T_RdpLlrpUID tRssiTagId;/*Ψһ��ʶ*/
}T_RdpRssiQueueNode;

/*Rssi������еĿ��ƽṹ*/
typedef struct tagT_RdpRssiQueueCtrl
{
    unsigned short int wRssiNodeNum;                           /*Rssi�����*/
    unsigned short int wPad;
    T_RdpRssiQueueNode *ptHeadOfRssiQueue;/*���ͷָ��*/
    T_RdpRssiQueueNode *ptTailOfRssiQueue;  /*���βָ��*/
}T_RdpRssiQueueCtrl ;

/*rssi�����ڲ��ṹ*/
typedef struct tagT_RdpRssiSort
{
    unsigned short int wRssiAccmulation;           /*Rssi�ۼ�ֵ*/
    T_RdpLlrpUID tRssiTagId;                /*Ψһ��ʶ*/
}T_RdpRssiSort;

/**************************************************************************
 *                             ������                                      *
 **************************************************************************/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
 unsigned int usp_rdp_report_Entry();
unsigned int report_refreshRoReportFilterNode(T_RdpTagReportFilterHashEntry *ptTagReportFilterHashHandle, 
                                                                                 T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl);
unsigned int report_refreshAoReportFilterNode(T_RdpTagReportFilterHashEntry *ptTagReportFilterHashHandle,
                                                                                T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl);
unsigned int report_freeAoReportFilterNode(T_RdpTagReportFilterHashEntry *ptTagReportFilterHandle,
                                                                         unsigned int dwAccessID);
unsigned int report_freeRoReportFilterNode(T_RdpTagReportFilterHashEntry *ptTagReportFilterHandle,
                                                                          unsigned int dwROSpecID);
unsigned int report_dealReportFilter_PollingClear(T_RdpTagReportFilterHashEntry *ptTagReportFilterHandle);
void report_dealReportFilterMemory();
unsigned int report_deleteReportNode(T_RdpLlrpSingleReportCtrl **ptOneReportCursor, T_RdpLlrpSingleReportCtrl **ptSendReportHead);
void report_insertReportNodeInHead(T_RdpLlrpSingleReportCtrl **ptSendReportHead, T_RdpOpFilterHashEntry *ptCurrOpFilterNode);
unsigned int report_insertReportNodeInTail(T_RdpLlrpSingleReportCtrl **ptSendReportHead, 
                                                                         T_RdpLlrpSingleReportCtrl *ptSingleReportCtrl);
LLRP_tSTagReportData *report_constructSingleTagReport(T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl);
unsigned int report_sendReportMsgAndFree(T_Msg *ptReceiveMsg, unsigned int dwMessageID , LLRP_tSMessage *ptMessage);


#endif  /* _RDP_REPORT_H */

