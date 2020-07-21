/***************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�Report.c
* �ļ���ʶ��
* ����ժҪ��reportģ��ʵ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�lzy 
* ������ڣ�2012.03.01
*
* �޸ļ�¼1��  
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
**********************************************************************/


/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include "rdpapppub.h"
#include "report.h"


/***********************************************************
 *                     ��������                            *
***********************************************************/

/***********************************************************
*                 �ļ��ڲ�ʹ�õĺ�                        *
***********************************************************/

/***********************************************************
*               �ļ��ڲ�ʹ�õ���������                    *
***********************************************************/

/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/
unsigned char g_aucReportRevDataBuf[READER_MAX_RCV_MSG_LEN];
unsigned char g_aucReportSendDataBuf[READER_MAX_SEND_MSG_LEN];
T_RdpOpFilterHashEntry *g_ptRdpOpFilterHashEntry = NULL;
T_RdpTagReportFilterHashEntry *g_ptRdpTagReportFilterHashEntry = NULL;
unsigned int g_bReportInitFinish = FALSE;
T_USPRdpAdvancedConfig g_tRdpAdvancedConfig;

extern  void *ctx;
/***********************************************************
 *                     ���ر���                            *
***********************************************************/



/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/

/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/
static unsigned int report_dealReportFilter(T_Msg *ptMsg);
static unsigned int report_sendLinkReport(T_Msg *ptMsg);
static unsigned int report_dealReportFilter_SendLinkReport(T_RdpLlrpSingleReportCtrl **ptAcculateReportHead);
static unsigned int report_dealRoReportFilter_SendLinkReport(T_RdpLlrpSingleReportCtrl **ptAcculateReportHead);
static unsigned int report_sendLinkReport_SendReport(T_Msg *ptMsg);
static unsigned int reportWorkProc(T_Msg *ptMsg);
static unsigned int report_dealReportFilter_DeleteFilterNode(T_RdpDeleteFilterNodes *ptDeleteFilterNode);
static void report_init();

/**************************************************************************
* �������ƣ�report_dealReportFilter_SendLinkReport
* �����������ϱ������з��ͱ�����Ϣ����
* ���ʵı��ϱ��������ݿ�hash��
* �޸ĵı��ϱ��������ݿ�hash��
* ���������T_RdpLlrpSingleReportCtrl **ptAcculateReportHead ��������ͷָ��
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_dealReportFilter_SendLinkReport(T_RdpLlrpSingleReportCtrl **ptAcculateReportHead)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_RdpLlrpSingleReportCtrl *ptTempReportCtrl = NULL;
    RdpReportDbgPrint("report_dealReportFilter_SendLinkReport\r\n");
    /*������·���洴���������˹��˽����߸��²����������ݿ�*/
    for (ptTempReportCtrl = *ptAcculateReportHead;NULL != ptTempReportCtrl;)
    {
        RdpReportDbgPrint("ptTempReportCtrl=0x%x,RO type=%d\r\n",
                          ptTempReportCtrl, ptTempReportCtrl->ucROType);
        /*���α���ֱ�Ӽ���*/
        if (RO_START_TRIGGER_SINGLE == ptTempReportCtrl->ucROType)
        {
            ptTempReportCtrl = ptTempReportCtrl->ptNext;
            continue;
        }

        /*����RO�����ϱ����˴���*/
        if (ROSPEC_REPORT_DATA == ptTempReportCtrl->ucReportType)
        {
            dwRet = report_refreshRoReportFilterNode(g_ptRdpTagReportFilterHashEntry, ptTempReportCtrl);
        }
        else if (ACCESSSPEC_REPORT_DATA == ptTempReportCtrl->ucReportType)
        {
            if (0 != ptTempReportCtrl->tUID.wTagDataLen)
            {
                /*��ȡΨһ��ʶ*/
                dwRet = report_refreshAoReportFilterNode(g_ptRdpTagReportFilterHashEntry, ptTempReportCtrl);
            }
            else
            {
                /*ɾ����·���*/
                RdpReportDbgPrint("tag report filter can not find only identifier and delete the AO report\r\n");
                dwRet = RDP_FAILED;
            }
        }
        if (RDP_SUCCESS != dwRet)
        {
            /*ɾ����·���*/
            report_deleteReportNode(&ptTempReportCtrl, ptAcculateReportHead);
        }
        else
        {
            ptTempReportCtrl = ptTempReportCtrl->ptNext;
        }

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�report_dealReportFilter_DeleteFilterNode
* �����������ϱ�������ɾ�����˽����Ϣ����
* ���ʵı��ϱ��������ݿ�hash��
* �޸ĵı��ϱ��������ݿ�hash��
* ���������T_RdpDeleteFilterNodes *ptDeleteFilterNode ��Ϣ�ṹ��
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_dealReportFilter_DeleteFilterNode(T_RdpDeleteFilterNodes *ptDeleteFilterNode)
{
    unsigned int dwRet = RDP_SUCCESS;
    if (RDP_DELETE_FILTER_TYPE_AO == ptDeleteFilterNode->ucDeleteType)
    {
        /*ɾ���ϱ����˽��*/
        dwRet = report_freeAoReportFilterNode(g_ptRdpTagReportFilterHashEntry, ptDeleteFilterNode->dwRoOrAoId);
    }
    else if (RDP_DELETE_FILTER_TYPE_RO == ptDeleteFilterNode->ucDeleteType)
    {
        /*ɾ���ϱ����˽��*/
        dwRet = report_freeRoReportFilterNode(g_ptRdpTagReportFilterHashEntry, ptDeleteFilterNode->dwRoOrAoId);
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�report_dealReportFilter
* ��������������ģ����ϱ������йص���Ϣ����
* ���ʵı��ϱ��������ݿ�hash��
* �޸ĵı��ϱ��������ݿ�hash��
* ���������T_Msg *ptMsg ���յ�����Ϣ
* ���������T_Msg *ptMsg ��Ϣ��
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_dealReportFilter(T_Msg *ptMsg)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wMsgID;
    T_RdpLlrpSingleReportCtrl *ptReportLinkCtrl = NULL;
    T_RdpDeleteFilterNodes *ptRdpDeleteFilterNodes = NULL;
    T_RdpLlrpSendReportContent *ptSendReportContent = NULL;
    if (FALSE == g_tRdpAdvancedConfig.tTagFilterCfg.ucTurnOnRFFilter)
    {
        return dwRet;
    }
    wMsgID = ptMsg->wMsgID;
    switch (wMsgID)
    {
    case EV_USP_RDP_SEND_LINK_REPORT:
        {
            RdpReportDbgPrint("(REPORT ReportFilter) receive msg EV_USP_RDP_SEND_LINK_REPORT\r\n");
            ptSendReportContent = (T_RdpLlrpSendReportContent *)(ptMsg + 1);
            ptReportLinkCtrl = ptSendReportContent->ptReportAddr;
            dwRet = report_dealReportFilter_SendLinkReport(&ptReportLinkCtrl);
            ptSendReportContent->ptReportAddr = ptReportLinkCtrl;
            break;
        }
    case EV_USP_RDP_DELETE_FILTER_NODE:
        {
            RdpReportDbgPrint("(REPORT ReportFilter) receive msg EV_USP_RDP_DELETE_FILTER_NODE\r\n");
            ptRdpDeleteFilterNodes = (T_RdpDeleteFilterNodes *)(ptMsg + 1);
            dwRet = report_dealReportFilter_DeleteFilterNode(ptRdpDeleteFilterNodes);
            break;
        }
    case EV_RDP_REPORT_TAGTRPORT_FILTER_TIMER:
        {
            RdpReportDbgPrint("(REPORT ReportFilter) receive msg EV_RDP_REPORT_TAGTRPORT_FILTER_TIMER\r\n");
            report_dealReportFilter_PollingClear(g_ptRdpTagReportFilterHashEntry);
            break;
        }

    default:
        {
            break;
        }
    }
    return dwRet;
}


/**************************************************************************
* �������ƣ�report_sendLinkReport_SendReport
* �������������ͱ����з��ͱ�����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_sendLinkReport_SendReport(T_Msg *ptMsg)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwSendLen = 0;
    T_RdpLlrpSendReportContent *ptSendReportContent = NULL;
    T_RdpLlrpSingleReportCtrl *ptTempReportCtrl = NULL;
    T_RdpLlrpSingleReportCtrl *ptAcculateReportHead = NULL;
    LLRP_tSRO_ACCESS_REPORT *ptRoAndAccessReport = NULL;
    LLRP_tSTagReportData * ptTagReportData = NULL;
    ptSendReportContent = (T_RdpLlrpSendReportContent *)(ptMsg + 1);
    ptAcculateReportHead = ptSendReportContent->ptReportAddr;
    RdpReportDbgPrint("SendReport ptAcculateReportHead=0x%x\r\n", ptAcculateReportHead);
    if (NULL == ptAcculateReportHead)
    {
        RdpReportDbgPrint("no report to send to client\r\n");
        return dwRet;
    }
    do
    {
        /*������·���湹������*/
        ptRoAndAccessReport = LLRP_RO_ACCESS_REPORT_construct();
        for (ptTempReportCtrl = ptAcculateReportHead;NULL != ptTempReportCtrl;)
        {
            dwSendLen += ptTempReportCtrl->wMsgLen;
            if (dwSendLen > 1900)
            {
                /*���ȳ���Ӧ�÷���*/
                if (ptTempReportCtrl->wMsgLen > 1900)
                {
                    /*�������ȳ���Ӧ��ɾ��*/
                    RdpWarningPrint("report is too big delete it \r\n");
                    report_deleteReportNode(&ptTempReportCtrl, &ptAcculateReportHead);
                }
                RdpReportDbgPrint("dwSendLen=%d\r\n", dwSendLen);
                dwSendLen = 0;
                break;
            }
            /*��������*/

            ptTagReportData = report_constructSingleTagReport(ptTempReportCtrl);
            LLRP_RO_ACCESS_REPORT_addTagReportData(ptRoAndAccessReport, ptTagReportData);
            /*ɾ����·���*/
            report_deleteReportNode(&ptTempReportCtrl, &ptAcculateReportHead);
        }
        /*���ͱ���*/
        dwRet = report_sendReportMsgAndFree(ptMsg, ptSendReportContent->dwMessgeID, \
                                            (LLRP_tSMessage *)ptRoAndAccessReport);
    }
    while (NULL != ptTempReportCtrl) ;
    return dwRet;
}
/**************************************************************************
* �������ƣ�report_sendLinkReport_SendEvent
* �������������ͱ����з����¼�
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_sendLinkReport_SendEvent(T_Msg *ptMsg)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_RdpLlrpSendReportContent *ptSendReportContent = NULL;
    LLRP_tSREADER_EVENT_NOTIFICATION *ptReadEventNotification = NULL;
    ptSendReportContent = (T_RdpLlrpSendReportContent *)(ptMsg + 1);
    ptReadEventNotification = (LLRP_tSREADER_EVENT_NOTIFICATION *)ptSendReportContent->ptReportAddr;
    RdpReportDbgPrint("SendEvent addr=0x%x\r\n", ptReadEventNotification);
    /*���ͱ���*/
    dwRet = report_sendReportMsgAndFree(ptMsg, ptSendReportContent->dwMessgeID, \
                                        (LLRP_tSMessage *)ptReadEventNotification);

    return dwRet;
}

/**************************************************************************
* �������ƣ�report_sendLinkReport
* �������������ͱ���ģ����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int report_sendLinkReport(T_Msg *ptMsg)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wMsgID = 0;
    wMsgID = ptMsg->wMsgID;
    switch (wMsgID)
    {
    case EV_USP_RDP_SEND_LINK_REPORT:
        {
            RdpReportDbgPrint("(REPORT send report) receive msg EV_USP_RDP_SEND_LINK_REPORT\r\n");
            report_sendLinkReport_SendReport(ptMsg);
            break;
        }
    case EV_READER_EVENT_NOTIFICATION:
        {
            RdpReportDbgPrint("(REPORT send report) receive msg EV_READER_EVENT_NOTIFICATION\r\n");
            report_sendLinkReport_SendEvent(ptMsg);
            break;
        }
    default:
        {
            break;
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�report_init
* ��������������ģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/05/04    V1.0       lzy        ����
**************************************************************************/
static void report_init()
{
    /* ��ʼ��ȫ�ֱ��� */
    g_ptRdpOpFilterHashEntry = NULL;
    g_ptRdpTagReportFilterHashEntry = NULL;
    memset(g_aucReportRevDataBuf, 0, sizeof(g_aucReportRevDataBuf));
    memset(g_aucReportSendDataBuf, 0, sizeof(g_aucReportSendDataBuf));
    memset(&g_tRdpAdvancedConfig, 0, sizeof(g_tRdpAdvancedConfig));

    memcpy(&g_tRdpAdvancedConfig, &(g_tRdpConfig.tUSPRdpAdvancedConfig), sizeof(T_USPRdpAdvancedConfig));
    /*����������˺��ϱ��������ݿ��ڴ�*/
    report_dealReportFilterMemory();
    g_bReportInitFinish = TRUE;
    return ;
}

/**************************************************************************
* �������ƣ�reportWorkProc
* ��������������ģ�鹦�ܴ���
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/02    V1.0       lzy        ����
**************************************************************************/
static unsigned int reportWorkProc(T_Msg *ptMsg)
{
    unsigned int dwRet = RDP_SUCCESS;

    /*�����ϱ�����*/
    report_dealReportFilter(ptMsg);
	
    /*���ͱ���*/
    report_sendLinkReport(ptMsg);
    return dwRet;
}
void *pair_socket_report_llrp=NULL;
void *pair_socket_report_rule=NULL;

void create_zmq_sockets_report(void)
{
    int value;
    int ret;
	
    /*inproc llrp*/
	pair_socket_report_llrp = zmq_socket(ctx, ZMQ_PAIR);
    if(NULL == pair_socket_report_llrp)
    {
        printf("create pair_socket_report_llrp fail!\n");
        exit(-1);
    }
    ret = zmq_connect(pair_socket_report_llrp, "inproc://report.inproc");
	if(ret<0)
    {
        printf("zmq_connect pair_socket_report_llrp fail!\n");
        exit(-1);
    }

	/*inproc rule */
	pair_socket_report_rule = zmq_socket(ctx, ZMQ_PAIR);
    if(NULL == pair_socket_report_rule)
    {
        printf("create pair_socket_report_rule fail!\n");
        exit(-1);
    }
    ret = zmq_connect(pair_socket_report_rule, "inproc://rule_report.inproc");
	if(ret<0)
    {
        printf("zmq_connect pair_socket_report_rule fail!\n");
        exit(-1);
    }
    return;
}

/**************************************************************************
* �������ƣ�usp_rdp_report_Entry
* ����������rdp���洦���߳���ں���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/05    V1.0       lzy        ����
**************************************************************************/
unsigned int rdp_report_main()
{
    zmq_pollitem_t	items[2];
	int item_num;
	int rc;
    T_Msg *ptMsg = NULL;
    unsigned int dwRet = RDP_SUCCESS;

    RdpReportDbgPrint("(usp_rdp_report_Entry) running...... \r\n");

	create_zmq_sockets_report();

    /*ģ���ʼ��*/
    report_init();
	
    /* �ȴ�������� */
    while (FALSE == g_bRdpPowerOnCfgFinish)
    {
        usleep(10000);
    }
    RdpReportDbgPrint("(Report)come into rdp_report_main\r\n");

	memset(items,0,sizeof(items));
		 items[0].socket = pair_socket_report_llrp;
		 items[0].events = ZMQ_POLLIN;
		 items[1].socket = pair_socket_report_rule;
		 items[1].events = ZMQ_POLLIN;

    /*  ������Ϣ���� */
    for (;;)
    {
       rc = zmq_poll(items, 2, -1);
			 if(rc<0)
			 {
				printf("(Report)zmq_poll return , return %d %d,%s\n", rc,errno,zmq_strerror(errno));
				continue;
			 }
			 if(items[0].revents & ZMQ_POLLIN)
			 {
			     dwRet=REPORT_RECV_MSG(pair_socket_report_llrp,g_aucReportRevDataBuf,READER_MAX_RCV_MSG_LEN);
			 }
			 else if(items[1].revents & ZMQ_POLLIN)
			 {
			     dwRet=REPORT_RECV_MSG(pair_socket_report_rule,g_aucReportRevDataBuf,READER_MAX_RCV_MSG_LEN);
			 }

        

        if (dwRet<=0)
        {
            /*��Ϣ���н��ճ���*/
            RdpErrorPrint("(Report)REPORT:MsgRcv Error\r\n");
            RDP_EXCEPT(RDP_RECV_MESSGAE_ERROR, dwRet, 0, 0, RDP_REPORT_ID);
            continue;
        }
		ptMsg = (T_Msg *)g_aucReportRevDataBuf;
        RdpReportDbgPrint("(Report)ptMsg->ucLinkType=%d\r\n", ptMsg->ucLinkType);
        RdpReportDbgPrint("(Report)ptMsg->ucLinkIndex=%d\r\n", ptMsg->ucLinkIndex);
        RdpReportDbgPrint("(Report)ptMsg->wMsgLen=%d\r\n", ptMsg->wMsgLen);
        RdpReportDbgPrint("(Report)ptMsg->wMsgID=%d\r\n", ptMsg->wMsgID);

        dwRet = reportWorkProc(ptMsg);
        if (RDP_SUCCESS != dwRet)
        {
            RdpErrorPrint("(Report) reportWorkProc error dwRet=0x%x\r\n", dwRet);
            continue;
        }
    }

    return RDP_SUCCESS;
}
