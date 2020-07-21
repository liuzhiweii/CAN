/***************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�Rule.c
* �ļ���ʶ��
* ����ժҪ��Ruleģ��ʵ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�lzy 
* ������ڣ�2012.02.29
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

#include "rdppub.h"
#include "rdpapppub.h"
#include "rule.h"


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
extern T_RdpConfig g_tRdpConfig;
T_RuleAODenymicList *s_ptRuleAODenymicList = NULL;
static unsigned char s_aucRuleRevDataBuf[READER_MAX_RCV_MSG_LEN];
static unsigned char s_aucRuleSendDataBuf[READER_MAX_SEND_MSG_LEN];
T_RuleROCtrlAndParam g_tRuleROCtrlAndParam[MAX_NUM_RO_SPEC];
void *pair_socket_rule_llrp=NULL;
void *pair_socket_rule_report=NULL;


extern void *push_socket_comm;
extern void *pull_socket_comm;
extern void *ctx;

/***********************************************************
 *                     ���ر���                            *
***********************************************************/



/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/

/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/
static unsigned int ruleworkProc(T_Msg *ptMsg, LLRP_tSMessage *ptMessage);
static unsigned int rule_decodeMessage(T_Msg *ptMsg, LLRP_tSMessage **ptMessage);
static unsigned int rule_checkParameter(T_Msg *ptMsg, LLRP_tSMessage *ptMessage);
static unsigned int rule_transferAndSendMessage(T_Msg *ptMsg, LLRP_tSMessage *ptMessage);
static unsigned int rule_sendInnerMessageToLlrp(T_Msg *ptMsg, LLRP_tSMessage *ptMessage);
static unsigned int rule_sendAOInnerMessageToLlrp(T_Msg *ptMsg, unsigned int dwAOReportTrigger,LLRP_tSMessage *ptMessage);
static unsigned int rule_SendSuccessReponseMsgToClient(T_Msg *ptReceiveMsg, unsigned int dwMessageID);
static unsigned int rule_sendDeleteFilterNodeMsgToReport(T_RdpDeleteFilterNodes *ptDeleteFilterNodes);
static void rule_init();
/**************************************************************************
* �������ƣ�void GetRDPLLTime()
* ������������õ�ǰʱ��
* ���ʵı���
* �޸ĵı���
* �����������
* ���������struct timeval *ptCurrentTime ��ȡ��ʱ��
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/16    V1.0       lzy        ����
**************************************************************************/
void GetRDPLLTime(struct timeval *ptCurrentTime)
{

    if (gettimeofday(ptCurrentTime, NULL) != 0)
    {
        RdpRuleDbgPrint("gettimeofday is error\r\n");
    }

}
/**************************************************************************
* �������ƣ�rule_SendSuccessReponseMsgToClient
* ����������ruleģ�鷢����Ϣ����̨
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptReceiveMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSMessage *ptLLRPMessage LLRP��Ϣ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_SendSuccessReponseMsgToClient(T_Msg *ptReceiveMsg, unsigned int dwMessageID)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_Msg *ptSendMsg = NULL;
    LLRP_tSErrorDetails pError;
    T_RdpDeleteFilterNodes tDeleteFilterNodes;
    LLRP_tSADD_ROSPEC_RESPONSE *ptLLRP_tSADD_ROSPEC_RESPONSE = NULL;
    LLRP_tSADD_ACCESSSPEC_RESPONSE *ptLLRP_tSADD_ACCESSSPEC_RESPONSE = NULL;
    LLRP_tSDISABLE_ROSPEC_RESPONSE *ptLLRP_tSDISABLE_ROSPEC_RESPONSE = NULL;
    LLRP_tSDELETE_ROSPEC_RESPONSE *ptLLRP_tSDELETE_ROSPEC_RESPONSE = NULL;
    LLRP_tSDISABLE_ACCESSSPEC_RESPONSE *ptLLRP_tSDISABLE_ACCESSSPEC_RESPONSE = NULL;
    LLRP_tSDELETE_ACCESSSPEC_RESPONSE *ptLLRP_tSDELETE_ACCESSSPEC_RESPONSE = NULL;
    LLRP_tSSTOP_ROSPEC_RESPONSE *ptLLRP_tSSTOP_ROSPEC_RESPONSE = NULL;
    LLRP_tSERROR_MESSAGE *ptSERROR_MESSAG = NULL;
    LLRP_tSMessage *ptTempMessage = NULL;
    T_RdpLlrpRuleRsp *ptRdpLlrpRuleRsp = NULL;
    ptRdpLlrpRuleRsp = (T_RdpLlrpRuleRsp *)(ptReceiveMsg + 1);
    ptTempMessage = (LLRP_tSMessage *)ptRdpLlrpRuleRsp->ptRuleRspAddr;

    /*���ݲ�ͬ����Ϣ���� ���ؽṹ*/
    switch (ptReceiveMsg->wMsgID)
    {
    case EV_ADD_ROSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_ADD_ROSPEC_RESPONSE\r\n");
            ptLLRP_tSADD_ROSPEC_RESPONSE = LLRP_ADD_ROSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSADD_ROSPEC_RESPONSE;
            break;
        }
    case EV_ADD_ACCESSSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_ADD_ACCESSSPEC_RESPONSE\r\n");
            ptLLRP_tSADD_ACCESSSPEC_RESPONSE = LLRP_ADD_ACCESSSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSADD_ACCESSSPEC_RESPONSE;
            break;
        }
    case EV_DISABLE_ROSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_DISABLE_ROSPEC_RESPONSE\r\n");
            ptLLRP_tSDISABLE_ROSPEC_RESPONSE = LLRP_DISABLE_ROSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSDISABLE_ROSPEC_RESPONSE;
            /*��Report ģ�鷢��ɾ�����˽����Ϣ*/
            tDeleteFilterNodes.ucDeleteType = RDP_DELETE_FILTER_TYPE_RO;
            tDeleteFilterNodes.dwRoOrAoId = ptRdpLlrpRuleRsp->dwRuleID;
            rule_sendDeleteFilterNodeMsgToReport(&tDeleteFilterNodes);
            break;
        }
    case EV_DELETE_ROSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_DELETE_ROSPEC_RESPONSE\r\n");
            ptLLRP_tSDELETE_ROSPEC_RESPONSE = LLRP_DELETE_ROSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSDELETE_ROSPEC_RESPONSE;
            /*��Report ģ�鷢��ɾ�����˽����Ϣ*/
            tDeleteFilterNodes.ucDeleteType = RDP_DELETE_FILTER_TYPE_RO;
            tDeleteFilterNodes.dwRoOrAoId = ptRdpLlrpRuleRsp->dwRuleID;
            rule_sendDeleteFilterNodeMsgToReport(&tDeleteFilterNodes);
            break;
        }
    case EV_DISABLE_ACCESSSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_DISABLE_ACCESSSPEC_RESPONSE\r\n");
            ptLLRP_tSDISABLE_ACCESSSPEC_RESPONSE = LLRP_DISABLE_ACCESSSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSDISABLE_ACCESSSPEC_RESPONSE;
            /*��Report ģ�鷢��ɾ�����˽����Ϣ*/
            tDeleteFilterNodes.ucDeleteType = RDP_DELETE_FILTER_TYPE_AO;
            tDeleteFilterNodes.dwRoOrAoId = ptRdpLlrpRuleRsp->dwRuleID;
            rule_sendDeleteFilterNodeMsgToReport(&tDeleteFilterNodes);
            break;
        }

    case EV_DELETE_ACCESSSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_DELETE_ACCESSSPEC_RESPONSE\r\n");
            ptLLRP_tSDELETE_ACCESSSPEC_RESPONSE = LLRP_DELETE_ACCESSSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSDELETE_ACCESSSPEC_RESPONSE;
            /*��Report ģ�鷢��ɾ�����˽����Ϣ*/
            tDeleteFilterNodes.ucDeleteType = RDP_DELETE_FILTER_TYPE_AO;
            tDeleteFilterNodes.dwRoOrAoId = ptRdpLlrpRuleRsp->dwRuleID;
            rule_sendDeleteFilterNodeMsgToReport(&tDeleteFilterNodes);
            break;
        }
    case EV_STOP_ROSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)EV_STOP_ROSPEC_RESPONSE\r\n");
            ptLLRP_tSSTOP_ROSPEC_RESPONSE = LLRP_STOP_ROSPEC_RESPONSE_construct();
            ptSERROR_MESSAG = (LLRP_tSERROR_MESSAGE *)ptLLRP_tSSTOP_ROSPEC_RESPONSE;
            break;
        }
    case EV_ENABLE_ROSPEC_RESPONSE:
    case EV_START_ROSPEC_RESPONSE:
    case EV_ENABLE_ACCESSSPEC_RESPONSE:
        {
            RdpRuleDbgPrint("(RULE)RESPONSE NEED DESTRUCT\r\n");
            break;
        }
    default:
        {
            RdpRuleDbgPrint("(RULE)the Msg ID is not supported =%d\r\n", ptReceiveMsg->wMsgID);
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptReceiveMsg->wMsgID, 0, 0, RDP_RULE_ID);
            break;
        }

    }

    if (NULL != ptTempMessage)
    {
        usp_ltk_freeMessage((LLRP_tSMessage *)ptTempMessage);
    }

    /*����ṹ*/
    if (ptSERROR_MESSAG != NULL)
    {
        LLRP_tSLLRPStatus *ptSLLRPStatus = LLRP_LLRPStatus_construct();
        ptSLLRPStatus->eStatusCode = LLRP_StatusCode_M_Success;
        LLRP_ERROR_MESSAGE_setLLRPStatus(ptSERROR_MESSAG, ptSLLRPStatus);
        ptSERROR_MESSAG->pLLRPStatus = ptSLLRPStatus;
        ptSERROR_MESSAG->hdr.MessageID = dwMessageID;
        ptSendMsg = (T_Msg *)s_aucRuleSendDataBuf;
        ptSendMsg->ucLinkIndex = ptReceiveMsg->ucLinkIndex;
        ptSendMsg->ucLinkType = ptReceiveMsg->ucLinkType;
        ptSendMsg->mtype = ptReceiveMsg->mtype;
        ptSendMsg->wMsgSequence = ptReceiveMsg->wMsgSequence;
        ptSendMsg->wSenderTno = USP_RDP_RULE_TNO;
        ptSendMsg->wReceiverTno = ptReceiveMsg->wReceiverTno;
        ptSendMsg->wMsgID = EV_USP_COMM_SendData;
        ptSendMsg->wMsgLen = READER_MAX_SEND_MSG_LEN - sizeof(T_Msg);
        RdpRuleDbgPrint("(RULE)ptSendMsg->wReceiverTno=%d\r\n" , ptSendMsg->wReceiverTno);
        /*���ⷢ��*/
        dwRet = usp_ltk_encodeMessage((unsigned char *)(ptSendMsg + 1),
                                      &ptSendMsg->wMsgLen,
                                      (LLRP_tSMessage *)ptSERROR_MESSAG, &pError);

        if (dwRet != RDP_SUCCESS)
        {
            RdpRuleDbgPrint("(RULE)usp_ltk_encodeMessage Error ptSendMsg->dwRet=%d\r\n" , dwRet);
            RDP_EXCEPT(RDP_ENCODE_MESSAGE_ERROR, dwRet, 0, 0, RDP_RULE_ID);
            usp_ltk_freeMessage((LLRP_tSMessage *)ptSERROR_MESSAG);
            return dwRet;
        }

        if (RDP_SUCCESS > RDP_SEND_MSG(push_socket_comm, ptSendMsg))
        {
            RdpRuleDbgPrint("(RULE)MsgSend Error\r\n");
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, 0, 0, 0, RDP_RULE_ID);
            usp_ltk_freeMessage((LLRP_tSMessage *)ptSERROR_MESSAG);
            return RDP_SEND_MESSGAE_ERROR;
        }

        RdpRuleDbgPrint("(RULE)MsgSend Success dwMessageID=%d\r\n", dwMessageID);
        usp_ltk_freeMessage((LLRP_tSMessage *)ptSERROR_MESSAG);

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_SendMsgToClient
* ����������ruleģ�鷢����Ϣ����̨
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptReceiveMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSMessage *ptLLRPMessage LLRP��Ϣ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_SendMsgToClient(T_Msg *ptReceiveMsg,
                            unsigned int dwMessageID ,
                            LLRP_tSMessage *ptLLRPMessage)
{

    unsigned int dwRet = RDP_SUCCESS;
    T_Msg *ptSendMsg = NULL;
    LLRP_tSErrorDetails pError;

    ptSendMsg = (T_Msg *)s_aucRuleSendDataBuf;
    ptLLRPMessage->MessageID = dwMessageID;
    ptSendMsg->ucLinkIndex = ptReceiveMsg->ucLinkIndex;
    ptSendMsg->ucLinkType = ptReceiveMsg->ucLinkType;
    ptSendMsg->mtype = ptReceiveMsg->mtype;
    ptSendMsg->wMsgSequence = ptReceiveMsg->wMsgSequence;
    ptSendMsg->wSenderTno = USP_RDP_RULE_TNO;
    ptSendMsg->wReceiverTno = ptReceiveMsg->wSenderTno;
    ptSendMsg->wSrcTno = USP_RDP_RULE_TNO;
    ptSendMsg->wDstTno = ptReceiveMsg->wSrcTno;
    ptSendMsg->wMsgID = EV_USP_COMM_SendData;
    ptSendMsg->wMsgLen = READER_MAX_SEND_MSG_LEN - sizeof(T_Msg);
    /*���ⷢ��*/
    RdpRuleDbgPrint("(RULE)ptSendMsg->wReceiverTno=%d\r\n" , ptSendMsg->wReceiverTno);
    dwRet = usp_ltk_encodeMessage((unsigned char *)(ptSendMsg + 1), &ptSendMsg->wMsgLen, ptLLRPMessage, &pError);

    if (dwRet != RDP_SUCCESS)
    {
        RdpRuleDbgPrint("(RULE)usp_ltk_encodeMessage Error ptSendMsg->dwRet=%d\r\n" , dwRet);
        return dwRet;
    }

    if (RDP_SUCCESS > RDP_SEND_MSG(push_socket_comm, ptSendMsg))
    {
        RdpRuleDbgPrint("(RULE)MsgSend Error\r\n");
        return dwRet;

    }

    RdpRuleDbgPrint("(RULE)rule_SendMsgToClient MsgSend Success\r\n");
    return dwRet;

}

/**************************************************************************
* �������ƣ�rule_decodeMessage
* ����������Ruleģ�������Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
                              LLRP_tSMessage *ptMessage Ҫ�����llrp����
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_decodeMessage(T_Msg *ptMsg, LLRP_tSMessage **ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned char *pucDataBuf = NULL;
    LLRP_tSErrorDetails tError;
    LLRP_tSMessage *ptMeMessage = NULL;
	int loop=0;
    if (EV_USP_COMM_RecvData == ptMsg->wMsgID)
    {
        RdpRuleDbgPrint("this is EV_USP_COMM_RecvData Msg\r\n");
        pucDataBuf = (unsigned char *)(ptMsg + 1);
		for(loop=0;loop<ptMsg->wMsgLen;loop++)
		{
			RdpDbgPrintRaw("0x%x ",pucDataBuf[loop]);
		}
		RdpDbgPrintRaw("\r\n");
        ptMeMessage = usp_ltk_decodeMessage(pucDataBuf, ptMsg->wMsgLen, &tError);

        if (LLRP_RC_OK != tError.eResultCode || NULL == ptMeMessage)
        {
            RdpRuleDbgPrint("usp_ltk_decodeMessage Error,pError.eResultCode = %d\r\n", tError.eResultCode);
            RDP_EXCEPT(RDP_DECODE_MESSAGE_ERROR, tError.eResultCode, 0, 0, RDP_RULE_ID);
            return RDP_DECODE_MESSAGE_ERROR;
        }
        ptMsg->wMsgID = ptMeMessage->elementHdr.pType->TypeNum;
    }
    *ptMessage = ptMeMessage;
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_transROToInnerCtrlState
* ����������Ruleģ��ת��RO������Ϣ���ڲ��ṹ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
            LLRP_tSMessage *ptMessage Ҫ�����llrp����
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_transROToInnerCtrlState(T_Msg *ptMsg, LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wMsgID = 0;
    unsigned short int wSenderTno = 0;     /* ���Ͷ������ */
    unsigned short int wLoop = 0;
    LLRP_tSADD_ROSPEC *ptAddRospec = NULL;
    LLRP_tSENABLE_ROSPEC *ptEnableRospec = NULL;
    LLRP_tSDISABLE_ROSPEC *ptDisableRospec = NULL;
    LLRP_tSDELETE_ROSPEC *ptDeleteRospec = NULL;
    LLRP_tSSTOP_ROSPEC *ptStopRospec = NULL;
    LLRP_tSSTART_ROSPEC *ptStartRospec = NULL;
    wSenderTno = ptMsg->wSenderTno;
    wMsgID = ptMsg->wMsgID;
    if (NULL == ptMessage)
    {
        RdpRuleDbgPrint("(rule_transROToInnerCtrlState)wSenderTno is not Client meg.wSenderTno=%d,wMsgID=%d\r\n", wSenderTno, wMsgID);
        return RULE_SENG_MESSAGE_TO_LLRP_YES;
    }

    switch (wMsgID)
    {
    case EV_ADD_ROSPEC:
        {
            ptAddRospec = (LLRP_tSADD_ROSPEC *)ptMessage;
            for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
            {
                if (FALSE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                {
                    break;
                }
            }
            if (MAX_NUM_RO_SPEC == wLoop)
            {
                RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptAddRospec->pROSpec->ROSpecID, 0, RDP_RULE_ID);
                return RDP_FAILED;
            }
            g_tRuleROCtrlAndParam[wLoop].dwROSpecID = ptAddRospec->pROSpec->ROSpecID;
            g_tRuleROCtrlAndParam[wLoop].eCurrentState = ptAddRospec->pROSpec->eCurrentState;
            g_tRuleROCtrlAndParam[wLoop].dwUseing = TRUE;
            g_tRuleROCtrlAndParam[wLoop].ucLinkIndex = ptMsg->ucLinkIndex;
            g_tRuleROCtrlAndParam[wLoop].ucLinkType = ptMsg->ucLinkType;
            break;
        }
    case EV_ENABLE_ROSPEC:
        {
            ptEnableRospec = (LLRP_tSENABLE_ROSPEC *)ptMessage;
            if (0 != ptEnableRospec->ROSpecID)
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptEnableRospec->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                            && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                    {
                        break;
                    }
                }
                if (MAX_NUM_RO_SPEC == wLoop)
                {
                    RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptEnableRospec->ROSpecID, ptMsg->ucLinkIndex, RDP_RULE_ID);
                    return RDP_FAILED;
                }
                g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Inactive;
            }
            else
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                            && (LLRP_ROSpecState_Disabled == g_tRuleROCtrlAndParam[wLoop].eCurrentState))
                    {
                        g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Inactive;
                    }
                }
            }
            break;
        }
    case EV_DISABLE_ROSPEC:
        {
            ptDisableRospec = (LLRP_tSDISABLE_ROSPEC *)ptMessage;
            if (0 != ptDisableRospec->ROSpecID)
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptDisableRospec->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                            && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                    {
                        break;
                    }
                }
                if (MAX_NUM_RO_SPEC == wLoop)
                {
                    RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptDisableRospec->ROSpecID, ptMsg->ucLinkIndex, RDP_RULE_ID);
                    return RDP_FAILED;
                }
                g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Disabled;
            }
            else
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                    {
                        g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Disabled;
                    }
                }
            }
            break;
        }
    case EV_DELETE_ROSEPC:
        {
            ptDeleteRospec = (LLRP_tSDELETE_ROSPEC *)ptMessage;
            if (0 != ptDeleteRospec->ROSpecID)
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptDeleteRospec->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                            && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                    {
                        break;
                    }
                }
                if (MAX_NUM_RO_SPEC == wLoop)
                {
                    RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptDeleteRospec->ROSpecID, ptMsg->ucLinkIndex, RDP_RULE_ID);
                    return RDP_FAILED;
                }
                memset(&g_tRuleROCtrlAndParam[wLoop], 0, sizeof(g_tRuleROCtrlAndParam[wLoop]));
                g_tRuleROCtrlAndParam[wLoop].dwUseing = FALSE;
            }
            else
            {
                for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
                {
                    if ((ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                            && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                            && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                    {
                        memset(&g_tRuleROCtrlAndParam[wLoop], 0, sizeof(g_tRuleROCtrlAndParam[wLoop]));
                        g_tRuleROCtrlAndParam[wLoop].dwUseing = FALSE;
                    }
                }
            }
            break;
        }
    case EV_STOP_ROSPEC:
        {
            ptStopRospec = (LLRP_tSSTOP_ROSPEC *)ptMessage;
            for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
            {
                if ((ptStopRospec->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                        && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                        && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                        && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                {
                    break;
                }
            }
            if (MAX_NUM_RO_SPEC == wLoop)
            {
                RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptStopRospec->ROSpecID, ptMsg->ucLinkIndex, RDP_RULE_ID);
                return RDP_FAILED;
            }
            g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Inactive;
            break;
        }
    case EV_START_ROSPEC:
        {
            ptStartRospec = (LLRP_tSSTART_ROSPEC *)ptMessage;
            for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
            {
                if ((ptStartRospec->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                        && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                        && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType)
                        && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing))
                {
                    break;
                }
            }
            if (MAX_NUM_RO_SPEC == wLoop)
            {
                RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, ptStartRospec->ROSpecID, ptMsg->ucLinkIndex, RDP_RULE_ID);
                return RDP_FAILED;
            }
            g_tRuleROCtrlAndParam[wLoop].eCurrentState = LLRP_ROSpecState_Active;
            break;
        }
    default:
        {
            RdpRuleDbgPrint("(rule_transROToInnerCtrlState)receive not needed id wMsgid= %d\r\n", wMsgID);
            break;
        }
    }
    return dwRet;

}
/**************************************************************************
* �������ƣ�rule_checkParameter
* ����������Ruleģ��LLRP��Ϣ����У��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
                              LLRP_tSMessage *ptMessage Ҫ�����llrp����
* �����������
* �� �� ֵ��0:��Ϣ������������������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkParameter(T_Msg *ptMsg, LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RULE_SENG_MESSAGE_TO_LLRP_YES;
    unsigned short int wMsgID = 0;
    unsigned short int wSenderTno = 0;     /* ���Ͷ������ */
    wSenderTno = ptMsg->wSenderTno;
    wMsgID = ptMsg->wMsgID;
    if (NULL == ptMessage)
    {
        RdpRuleDbgPrint("(Rule rule_checkParameter)wSenderTno is not Client meg.wSenderTno=%d,wMsgID=%d\r\n", wSenderTno, wMsgID);
        return RULE_SENG_MESSAGE_TO_LLRP_YES;
    }

   
   RdpRuleDbgPrint("(Rule check)ptMessage->MessageID=%d\r\n", ptMessage->MessageID);
    switch (wMsgID)
    {
    case EV_ADD_ROSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_ADD_ROSPEC\r\n");
            dwRet = rule_checkAddROSpec(ptMsg, (LLRP_tSADD_ROSPEC *)ptMessage);
            break;
        }
    case EV_ENABLE_ROSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_ENABLE_ROSPEC\r\n");
            dwRet = rule_checkEnableROSpec(ptMsg, (LLRP_tSENABLE_ROSPEC *)ptMessage);
            break;
        }
    case EV_DISABLE_ROSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_DISABLE_ROSPEC\r\n");
            dwRet = rule_checkDisableROSpec(ptMsg, (LLRP_tSDISABLE_ROSPEC *)ptMessage);
            break;
        }
    case EV_DELETE_ROSEPC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_DELETE_ROSEPC\r\n");
            dwRet = rule_checkDeleteROSpec(ptMsg, (LLRP_tSDELETE_ROSPEC *)ptMessage);
            break;
        }
    case EV_STOP_ROSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_STOP_ROSPEC\r\n");
            dwRet = rule_checkStopROSpec(ptMsg, (LLRP_tSSTOP_ROSPEC *)ptMessage);
            break;
        }
    case EV_START_ROSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg receive msg EV_START_ROSPEC\r\n");
            dwRet = rule_checkStartROSpec(ptMsg, (LLRP_tSSTART_ROSPEC *)ptMessage);
            break;
        }
    case EV_ADD_ACCESSSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_ADD_ACCESSSPEC\r\n");
            dwRet = rule_checkAddAccessspec(ptMsg, (LLRP_tSADD_ACCESSSPEC *)ptMessage);
            break;
        }
    case EV_ENABLE_ACCESSSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_ENABLE_ACCESSSPEC\r\n");
            dwRet = rule_checkEnableAccesssSpec(ptMsg, (LLRP_tSENABLE_ACCESSSPEC *)ptMessage);
            break;
        }
    case EV_DISABLE_ACCESSSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_DISABLE_ACCESSSPEC\r\n");
            dwRet = rule_checkDisableAccesssSpec(ptMsg, (LLRP_tSDISABLE_ACCESSSPEC *)ptMessage);
            break;
        }
    case EV_DELETE_ACCESSSPEC:
        {
            RdpRuleDbgPrint("(Rule check)receive msg EV_DELETE_ACCESSSPEC\r\n");
            dwRet = rule_checkDeleteAccesssSpec(ptMsg, (LLRP_tSDELETE_ACCESSSPEC *)ptMessage);
            break;
        }
    case EV_GET_ACCESSSPECS:
    case EV_GET_REPOERT:
    case EV_GET_ROSPEC:
    case EV_UHF_PRIVATE_CMD:
    case EV_UHF_PRIVATE_SETPASSWORD:
    case EV_CLIENT_REQUEST_OP_RESPONSE:
    

        {
            RdpRuleDbgPrint("(Rule check)receive  wMsgid= %d,continue\r\n", wMsgID);
            break;
        }
    default:
        {
            RdpRuleDbgPrint("(Rule check)receive wrong id wMsgid= %d\r\n", wMsgID);
            RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, 0, 0, RDP_RULE_ID);
            break;
        }
    }
    if (RDP_SUCCESS != dwRet)
    {
        dwRet = RULE_SENG_MESSAGE_TO_LLRP_NO;
    }
    else
    {
        dwRet = RULE_SENG_MESSAGE_TO_LLRP_YES;
        /*��ROתΪ�ڲ�״̬*/
        rule_transROToInnerCtrlState(ptMsg, ptMessage);
    }
    return dwRet;

}
/**************************************************************************
* �������ƣ�rule_sendInnerMessageToLlrp
* ����������������Ϣ��llrpģ��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
                              LLRP_tSMessage *ptMessage Ҫ���͵�llrp����
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_sendInnerMessageToLlrp(T_Msg *ptMsg, LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int *pdwTempAddr = NULL;
    T_Msg *ptSendMsg = NULL;
    ptSendMsg = (T_Msg *)s_aucRuleSendDataBuf;
    memcpy(ptSendMsg, ptMsg, sizeof(T_Msg));
    pdwTempAddr = (unsigned int *)(ptSendMsg + 1);
    ptSendMsg->wSrcTno = USP_RDP_RULE_TNO;
    ptSendMsg->wDstTno = USP_RDP_LLRP_TNO;
    ptSendMsg->wSenderTno = ptMsg->wSenderTno;
    ptSendMsg->wReceiverTno = ptMsg->wReceiverTno;
    ptSendMsg->wMsgLen = sizeof(unsigned int);
    *pdwTempAddr = (unsigned int)ptMessage;
    dwRet = RULE_SEND_MSG(pair_socket_rule_llrp, ptSendMsg);
    RdpRuleDbgPrint("(RULE)rule send Inner Message To Llrp dwRet=%d,ptMessage=0x%x\r\n", dwRet, ptMessage);
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_sendAOInnerMessageToLlrp
* ����������������Ϣ��llrpģ��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg, 
                              unsigned int dwAOReportTrigger,
                              Ao��ԭ�е�reportTrigger 
                              LLRP_tSMessage *ptMessage
                              Ao�ĵ�ַ
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_sendAOInnerMessageToLlrp(T_Msg *ptMsg, unsigned int dwAOReportTrigger,LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int *pdwTempAddr = NULL;
    T_RdpAddAo *ptRdpAddAo = NULL;
    T_Msg *ptSendMsg = NULL;
    ptSendMsg = (T_Msg *)s_aucRuleSendDataBuf;
    memcpy(ptSendMsg, ptMsg, sizeof(T_Msg));
    ptRdpAddAo = (T_RdpAddAo *)(ptSendMsg+1);
    ptSendMsg->wSrcTno = USP_RDP_RULE_TNO;
    ptSendMsg->wDstTno = USP_RDP_LLRP_TNO;
    ptSendMsg->wSenderTno = ptMsg->wSenderTno;
    ptSendMsg->wReceiverTno = ptMsg->wReceiverTno;
    ptSendMsg->wMsgLen = sizeof(T_RdpAddAo);
    ptRdpAddAo->dwAOReportSndTrigger =dwAOReportTrigger;
    ptRdpAddAo->ptAddAoMsgAddr = ptMessage;
    dwRet = RULE_SEND_MSG(pair_socket_rule_llrp, ptSendMsg);
    RdpRuleDbgPrint("(RULE)rule send Inner Message To Llrp dwRet=%d,ptMessage=0x%x\r\n", dwRet, ptMessage);
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_sendDeleteFilterNodeMsgToReport
* ��������������ɾ�����˽����Ϣ��reportģ��
* ���ʵı���
* �޸ĵı���
* ���������T_RdpDeleteFilterNodes *ptDeleteFilterNodesɾ�����ṹ��
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_sendDeleteFilterNodeMsgToReport(T_RdpDeleteFilterNodes *ptDeleteFilterNodes)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_Msg *ptSendMsg = NULL;
    T_RdpDeleteFilterNodes *ptTempDeleteFilterNodes = NULL;
    ptSendMsg = (T_Msg *)s_aucRuleSendDataBuf;
    ptTempDeleteFilterNodes = (T_RdpDeleteFilterNodes *)(ptSendMsg + 1);
    ptSendMsg->mtype = 1;
    ptSendMsg->wSenderTno = USP_RDP_RULE_TNO;
    ptSendMsg->wReceiverTno = USP_RDP_REPORT_TNO;
    ptSendMsg->wSrcTno = USP_RDP_RULE_TNO;
    ptSendMsg->wDstTno = USP_RDP_REPORT_TNO;
    ptSendMsg->wMsgID = EV_USP_RDP_DELETE_FILTER_NODE;
    ptSendMsg->wMsgLen = sizeof(T_RdpDeleteFilterNodes);
    memcpy(ptTempDeleteFilterNodes, ptDeleteFilterNodes, sizeof(T_RdpDeleteFilterNodes));
    dwRet = RULE_SEND_MSG(pair_socket_rule_report, ptSendMsg);
    RdpRuleDbgPrint("(Rule)rule_sendDeleteFilterNodeMsgToReport dwRet=%d\r\n", dwRet);
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_transferAndSendMessage
* ����������͸��ģ�鴦��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
                              LLRP_tSMessage *ptMessage Ҫ�����llrp����
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_transferAndSendMessage(T_Msg *ptMsg, LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wMsgID = 0;
    unsigned short int wSenderTno = 0;
    LLRP_tSERROR_MESSAGE *ptRuleRsp = NULL;
    T_RdpLlrpRuleRsp *ptRdpLlrpRuleRsp = NULL;
    LLRP_tSMessage *ptTempMessage = NULL;
    LLRP_tSADD_ACCESSSPEC * ptAddAccessspec = NULL;
    wMsgID = ptMsg->wMsgID;
    wSenderTno = ptMsg->wSenderTno;
    RdpRuleDbgPrint("(Rule transfer)wMsgID=%d,wSenderTno=%d\r\n", wMsgID, wSenderTno);
    if ((USP_RDP_LLRP_TNO != wSenderTno) && (NULL != ptMessage))
    {
        /*��LLRPģ��͸��LLRP��Ϣ*/
        RdpRuleDbgPrint("(Rule transfer)Client Message transfer to LLRP module\r\n");

       
        if( EV_ADD_ACCESSSPEC == ptMessage->elementHdr.pType->TypeNum )
        {
             ptAddAccessspec = (LLRP_tSADD_ACCESSSPEC *)ptMessage;
             RdpRuleDbgPrint("(Rule transfer)test1\r\n");
   
             
             dwRet = rule_sendAOInnerMessageToLlrp(ptMsg,
                                                                             ptAddAccessspec->pAccessSpec->pAccessReportSpec  == NULL? \
                                                                             g_tRdpConfig.tAccessReportSpec.eAccessReportTriggerType: \
                                                                             ptAddAccessspec->pAccessSpec->pAccessReportSpec->eAccessReportTrigger ,ptMessage);               
        }
        else
        {
        dwRet = rule_sendInnerMessageToLlrp(ptMsg, ptMessage);
    }
        
        
    }
    else
    {
        if (NULL != ptMessage)
        {
            RDP_EXCEPT(RDP_MSG_PROC_ERROR, ptMsg->wDstTno, ptMsg->wMsgID, ptMsg->wSenderTno, RDP_RULE_ID);
            RdpRuleDbgPrint("(Rule transfer)inner msg but ptMessage is not NULL ptMsg->wMsgID=%d ,wSenderTno=%d\r\n",
                            ptMsg->wMsgID, wSenderTno);
            usp_ltk_freeMessage(ptMessage);
            return RDP_MSG_PROC_ERROR;
        }

        switch (wMsgID)
        {
        case EV_ADD_ROSPEC_RESPONSE:
        case EV_ENABLE_ROSPEC_RESPONSE:
        case EV_DISABLE_ROSPEC_RESPONSE:
        case EV_DELETE_ROSPEC_RESPONSE:
        case EV_STOP_ROSPEC_RESPONSE:
        case EV_START_ROSPEC_RESPONSE:
        case EV_ADD_ACCESSSPEC_RESPONSE:
        case EV_DISABLE_ACCESSSPEC_RESPONSE:
        case EV_ENABLE_ACCESSSPEC_RESPONSE:
        case EV_DELETE_ACCESSSPEC_RESPONSE:
            {
                ptRdpLlrpRuleRsp = (T_RdpLlrpRuleRsp *)(ptMsg + 1);
                ptRuleRsp = ptRdpLlrpRuleRsp->ptRuleRspAddr;
                RdpRuleDbgPrint("(Rule transfer)Inner msg from module LLRP the addr is 0x%x,Rule ID=%d\r\n",
                                ptRuleRsp, ptRdpLlrpRuleRsp->dwRuleID);
                rule_SendSuccessReponseMsgToClient(ptMsg, ptRuleRsp->hdr.MessageID);
                break;
            }
        case EV_GET_ACCESSSPECS_RESPONSE:
        case EV_GET_ROSPEC_RESPONSE:
        case EV_CLIENT_REQUEST_OP:
        case EV_UHF_PRIVATE_CMD_ACK:
        case EV_UHF_PRIVATE_SETPASSWORD_ACK:
            {
                if (USP_RDP_RULE_TNO == ptMsg->wReceiverTno)
                {
                    RDP_EXCEPT(RDP_MSG_PROC_ERROR, ptMsg->wDstTno, ptMsg->wMsgID, 0, RDP_RULE_ID);
                    RdpRuleDbgPrint("(Rule transfer) rule_transferAndSendMessage wDstTno error ptMsg->wDstTno=%d,ptMsg->wMsgID=%d\r\n",
                                    ptMsg->wDstTno, ptMsg->wMsgID);
                }
                ptTempMessage = (LLRP_tSMessage *)(*(unsigned int *)(ptMsg + 1));
                /*���������������������ʱת������̨*/
                ptMsg->wSenderTno = ptMsg->wReceiverTno;
                ptMsg->wSrcTno = ptMsg->wDstTno;
                dwRet = rule_SendMsgToClient(ptMsg, ptTempMessage->MessageID, (LLRP_tSMessage *)ptTempMessage);
                if (wMsgID != EV_GET_ACCESSSPECS_RESPONSE && wMsgID != EV_GET_ROSPEC_RESPONSE)
                {
                    usp_ltk_freeMessage((LLRP_tSMessage *)ptTempMessage);
                }
                break;
            }
        case EV_RDP_DELETE_OPERATION_COUNT_ACCESS:
            {
                RdpRuleDbgPrint("((Rule transfer) receive an EV_RDP_DELETE_OPERATION_COUNT_ACCESS msg\r\n");
                break;
            }
        default:
            {
                RdpRuleDbgPrint("(Rule transfer) inner msg default wMsgID=%d\r\n", wMsgID);
                RDP_EXCEPT(RDP_INVALID_STATE, wMsgID, 0, 0, RDP_RULE_ID);
                break;
            }
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_init
* ���������������·�ģ���ʼ������
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
static void rule_init()
{
    /* ��ʼ��ȫ�ֱ��� */
    memset(s_aucRuleRevDataBuf, 0, sizeof(s_aucRuleRevDataBuf));
    memset(s_aucRuleSendDataBuf, 0, sizeof(s_aucRuleSendDataBuf));
    memset(g_tRuleROCtrlAndParam, 0, sizeof(g_tRuleROCtrlAndParam));
    return ;
}
/**************************************************************************
* �������ƣ�ruleworkProc
* ����������ruleģ�鹦��ģ�鴦�� 
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg: Ҫ�������Ϣ
                              LLRP_tSMessage *ptMessage Ҫ�����llrp����
* �����������
* �� �� ֵ��0:�ɹ�;����:ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/27    V1.0       lzy        ����
**************************************************************************/
static unsigned int ruleworkProc(T_Msg *ptMsg, LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RULE_SENG_MESSAGE_TO_LLRP_YES;

    /*����У��*/
    dwRet = rule_checkParameter(ptMsg, ptMessage);
    if (RULE_SENG_MESSAGE_TO_LLRP_YES != dwRet)
    {
        /*����Ҫ͸����LLRPģ��ʱ����Ҫ������̬�ڴ�*/
        if (NULL != ptMessage)
        {
            usp_ltk_freeMessage(ptMessage);
        }
        return dwRet;
    }
    /*͸����Ϣ*/
    dwRet = rule_transferAndSendMessage(ptMsg, ptMessage);
    return dwRet;
}


void create_zmq_sockets_rule(void)
{
    
    int value;
    int ret;
	
    /*inproc llrp*/
	pair_socket_rule_llrp = zmq_socket(ctx, ZMQ_PAIR);
    if(NULL == pair_socket_rule_llrp)
    {
        printf("create pair_socket_rule_llrp fail!\n");
        exit(-1);
    }
    ret = zmq_connect(pair_socket_rule_llrp, "inproc://rule.inproc");
	if(ret<0)
    {
        printf("zmq_connect pair_socket_rule_llrp fail!\n");
        exit(-1);
    }

	/*inproc report */
	pair_socket_rule_report = zmq_socket(ctx, ZMQ_PAIR);
    if(NULL == pair_socket_rule_report)
    {
        printf("create pair_socket_rule_report fail!\n");
        exit(-1);
    }
    ret = zmq_bind(pair_socket_rule_report, "inproc://rule_report.inproc");
	if(ret<0)
    {
        printf("zmq_connect pair_socket_rule_report fail!\n");
        exit(-1);
    }
    return;
}

/**************************************************************************
* �������ƣ�usp_rdp_rule_Entry
* ����������rdp�������߳���ں���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/03/1    V1.0       lzy        ����
**************************************************************************/
unsigned int rdp_rule_main()
{
    zmq_pollitem_t	items[2];
	int item_num;
	int rc;
    T_Msg *ptMsg = NULL;
    unsigned int dwRet = RDP_SUCCESS;
    LLRP_tSMessage *ptMessage = NULL;

    RdpRuleDbgPrint("(usp_rdp_rule_Entry) running...... \r\n");
	
	create_zmq_sockets_rule();

	/*ģ���ʼ��*/
    rule_init();

    /* �ȴ�������� */
    while ((FALSE == g_bRdpPowerOnCfgFinish) && (FALSE == g_bReportInitFinish))
    {
        usleep(10000);
    }
    
     memset(items,0,sizeof(items));
		 items[0].socket = pull_socket_comm;
		 items[0].events = ZMQ_POLLIN;
		 items[1].socket = pair_socket_rule_llrp;
		 items[1].events = ZMQ_POLLIN;
    RdpRuleDbgPrint("(RULE)come into rdp_rule_main\r\n");
    /*  ������Ϣ���� */
    for (;;)
    {
		rc = zmq_poll(items, 2, -1);
			 if(rc<0)
			 {
				RdpRuleDbgPrint("(RULE)zmq_poll return , return %d %d,%s\n", rc,errno,zmq_strerror(errno));
				continue;
			 }
			 if(items[0].revents & ZMQ_POLLIN)
			 {
			     dwRet=RULE_RECV_MSG(pull_socket_comm,s_aucRuleRevDataBuf,READER_MAX_RCV_MSG_LEN);
			 }
			 else if(items[1].revents & ZMQ_POLLIN)
			 {
			     dwRet=RULE_RECV_MSG(pair_socket_rule_llrp,s_aucRuleRevDataBuf,READER_MAX_RCV_MSG_LEN);
			 }
        

        if (dwRet<=0)
        {
            /*��Ϣ���н��ճ���*/
            RdpErrorPrint("(RULE)MsgRcv Error\r\n");
            RDP_EXCEPT(RDP_RECV_MESSGAE_ERROR, dwRet, 0, 0, RDP_RULE_ID);
            continue;
        }
        ptMsg = (T_Msg *)s_aucRuleRevDataBuf;
        dwRet = rule_decodeMessage(ptMsg, &ptMessage);
        if (RDP_SUCCESS != dwRet)
        {
            RDP_EXCEPT(RDP_MSG_PROC_ERROR, dwRet, ptMsg->wMsgID, (unsigned int)ptMessage, RDP_RULE_ID);
            RdpErrorPrint("(Rule) rule_decodeMessage error dwRet=0x%x\r\n", dwRet);
            continue;
        }
        RdpRuleDbgPrint("(Rule)ptMsg->wSenderTno=%d\r\n", ptMsg->wSenderTno);
        RdpRuleDbgPrint("(Rule)ptMsg->ucLinkIndex=%d\r\n", ptMsg->ucLinkIndex);
        RdpRuleDbgPrint("(Rule)ptMsg->wMsgLen=%d\r\n", ptMsg->wMsgLen);
        RdpRuleDbgPrint("(Rule)ptMsg->wMsgID=%d\r\n", ptMsg->wMsgID);
        RdpRuleDbgPrint("(Rule)ptMessage=0x%x\r\n", ptMessage);

        dwRet = ruleworkProc(ptMsg, ptMessage);
        if (RDP_SUCCESS != dwRet)
        {
            RdpRuleDbgPrint("(Rule) ruleworkProc Msg not continue dwRet=0x%x\r\n", dwRet);
            continue;
        }
    }

    return RDP_SUCCESS;
}



