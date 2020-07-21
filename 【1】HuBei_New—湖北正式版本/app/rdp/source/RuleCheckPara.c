/***************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�RuleCheckLlrpPara.c
* �ļ���ʶ��
* ����ժҪ��llrp��Ϣ����У�麯��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�lzy 
* ������ڣ�2012.02.26
*
* �޸ļ�¼1��  
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
*��ע:
*����Enable RO/AO,Start RO��Ϣ��Ӧ�ȷ������ȴ�LLRPģ�鷵����Ӧ���ٷ�������
*����ʹ��̨���յ���Ӧ�����յ��¼���������ϢӦ���յ�LLRP��Ӧ�������̨����
*��Ӧ��Ϣ������һ���������ڽ���LLRPģ��ȫ�ֱ�������У��
**********************************************************************/


/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
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
extern T_RuleROCtrlAndParam g_tRuleROCtrlAndParam[MAX_NUM_RO_SPEC];
/***********************************************************
 *                     ���ر���                            *
***********************************************************/



/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/

/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/
static unsigned int rule_checkAOSpecDepth(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSMessage *ptLLRPMessage, T_ParameterError *ptParameterError);
static unsigned int rule_checkAOSpec_AOSpecID(T_Msg *ptMsg, unsigned int dwMessageId, unsigned int dwAOSpecID, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_AntennaID(T_Msg *ptMsg, unsigned int dwMessageId, unsigned int dwAntennaID, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_CurrentState(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAccessSpecState eCurrentState, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_ProtocolID(T_Msg *ptMsg,
        unsigned int dwMessageId,
        LLRP_tEAirProtocols eProtocolID,
        LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_StopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAccessSpecStopTrigger * pAccessSpecStopTrigger, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_ReportSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAccessReportSpec * pAccessReportSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_AccessCommand(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSAccessCommand * pAccessCommand, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_AccessCommand_AirProtocolTagSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * pAirProtocolTagSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkEpcAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId);
static unsigned int rule_checkISO6BAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId);
static unsigned int rule_checkAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId);
static unsigned int rule_checkAOSpec_AccessCommand_OpSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkAOSpec_AccessCommand_C1G2CustomerSetting(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSC1G2CustomerSetting * pC1G2CustomerSetting, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpecDepth(T_Msg *ptMsg,
                                    unsigned int dwMessageId,
                                    LLRP_tSMessage *ptLLRPMessage,
                                    T_ParameterError *ptParameterError);
static unsigned int rule_checkROSpec_ROStartTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROSpecStartTrigger * pROSpecStartTrigger, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_ROStopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROSpecStopTrigger * pROSpecStopTrigger, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_listSpecParameter(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listSpecParameter, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAISpec * pAISpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaIDs(T_Msg *ptMsg, unsigned int dwMessageId, llrp_u16v_t AntennaIDs, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AISpecStopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAISpecStopTrigger * pAISpecStopTrigger, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_InventoryParameterSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSInventoryParameterSpec * pInventoryParameterSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSInventoryParameterSpec * ptlistInventoryParameterSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AntennaID(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAntennaConfiguration * pAntennaConfiguration, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_RFTransmitter(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSRFTransmitter * pRFTransmitter, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSAntennaConfiguration * ptlistAntennaConfiguration, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Epc(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listAirProtocolInventoryCommandSettings, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Iso6B(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listAirProtocolInventoryCommandSettings, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_ROReportSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROReportSpec * pROReportSpec, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_ROSpecID(T_Msg *ptMsg, unsigned int dwMessageId, unsigned int dwROSpecID, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_Priority(T_Msg *ptMsg, unsigned int dwMessageId, unsigned char ucPriority, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkROSpec_CurrentState(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEROSpecState eCurrentState, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkGBMB(unsigned char ucGBMB);
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_GB(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listAirProtocolInventoryCommandSettings, LLRP_tSMessage *ptLLRPMessage);
static unsigned int rule_checkGBAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId);
static unsigned int rule_checkGBOpMB(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSMessage *ptLLRPMessage,unsigned char ucGBMB,unsigned int dwTypeNum);
/**************************************************************************
* �������ƣ�rule_checkAOSpecDepth
* ��������������ao llrp�ṹ���������ʧ����Ϣ���͸���̨
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSMessage *ptLLRPMessage llrp����
*                             T_ParameterError *ptParameterError ��������ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpecDepth(T_Msg *ptMsg,
                                    unsigned int dwMessageId,
                                    LLRP_tSMessage *ptLLRPMessage,
                                    T_ParameterError *ptParameterError)
{
    unsigned short int wLoop = 0;
    unsigned int dwRet = RDP_FAILED;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    LLRP_tSParameterError tSParameterError[RDP_MAX_LLRP_DEPTH];
    LLRP_tSADD_ACCESSSPEC_RESPONSE *ptAddRospecResonse = NULL;

    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    memset((void*)&tSParameterError, 0, sizeof(tSParameterError));

    ptAddRospecResonse = (LLRP_tSADD_ACCESSSPEC_RESPONSE *)ptLLRPMessage;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_ParameterError;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSLLRPStatus.pParameterError = &tSParameterError[0];
    ptAddRospecResonse->pLLRPStatus = &tSLLRPStatus;

    /*��ʼ��*/
    for (wLoop = 0;wLoop < RDP_MAX_LLRP_DEPTH;wLoop++)
    {
        tSParameterError[wLoop].hdr.elementHdr.pType = &LLRP_tdParameterError;
        tSParameterError[wLoop].eErrorCode = LLRP_StatusCode_P_ParameterError;
    }
    tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ptParameterError->ErrorDetail);
    tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ptParameterError->ErrorDetail;
    tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_ParameterError;
    /*�����������*/
    for (wLoop = 0;wLoop < ptParameterError->wParameterDepth - 1;wLoop++)
    {
        tSParameterError[wLoop].eErrorCode = LLRP_StatusCode_P_ParameterError;
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].pParameterError = &tSParameterError[wLoop + 1];
    }

    /*���һ�㴦��*/
    if (TRUE == ptParameterError->bIsLastField)
    {
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].eErrorCode = ptParameterError->wLastParameterErrorCode;
        tSParameterError[wLoop].pFieldError = &tSFieldError;
        tSFieldError.FieldNum = ptParameterError->wFieldNum;
        tSFieldError.eErrorCode = ptParameterError->wFieldErrorCode;
    }
    else
    {
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].eErrorCode = ptParameterError->wLastParameterErrorCode;
        tSParameterError[wLoop].pParameterError = NULL;
    }
    rule_SendMsgToClient(ptMsg, dwMessageId, ptLLRPMessage);
    dwRet = (unsigned int)ptParameterError->wFieldErrorCode;
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpecDepth
* ����������У��AO��AO id
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             unsigned int dwAOSpecID AO id
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AOSpecID(T_Msg *ptMsg,
                                        unsigned int dwMessageId,
                                        unsigned int dwAOSpecID,
                                        LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwIndex = 0;
    unsigned int dwTestData = 0;
    T_ParameterError tParameterError;
    T_RdpLlrpAccessEntry *ptAccessAddCursor = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRdpLlrpAccessCtrlAndParam = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (0 == dwAOSpecID)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild AOSpecID can not = 0");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild AOSpecID can not = 0\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    /*�޸�access�������� �Ƿ���access */
    for (ptAccessAddCursor = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue);
            NULL != ptAccessAddCursor;ptAccessAddCursor = ptAccessAddCursor->ptNext)
    {
        ptRdpLlrpAccessCtrlAndParam = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessAddCursor->dwAccessIndex]);
        if (ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccess.dwAccessSpecID == dwAOSpecID
                && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex == ptMsg->ucLinkIndex
                && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType == ptMsg->ucLinkType
                && TRUE == ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucUseing
                && LLRP_ACCESSDISPATCH_STATE_DELETE != ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.dwDispatchStatus)
        {
            RdpRuleDbgPrint(" AccessID =%d\r\n", dwAOSpecID);
            RdpRuleDbgPrint("LinkIndex  =%d\r\n", ptMsg->ucLinkIndex);
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wParameterDepth = 1;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invaild AccessSpecID");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invaild AccessSpecID\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    for (dwIndex = 0;dwIndex < MAX_NUM_ACCESSSPECS;dwIndex++)
    {
        if (TRUE == g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[dwIndex].tRdpLlrpAccessDispatchCtrl.ucUseing)
        {
            dwTestData++;
        }
    }
    /*����������*/
    if (MAX_NUM_ACCESSSPECS == dwTestData)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "the AccessSpecNum is OutOfRange");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("the AccessSpecNum is OutOfRange\r\n");
        return LLRP_StatusCode_A_OutOfRange;

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_AntennaID
* ����������У��AO�����ߺ�
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             unsigned int dwAntennaID AO �����ߺ�
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AntennaID(T_Msg *ptMsg,
        unsigned int dwMessageId,
        unsigned int dwAntennaID,
        LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*����������*/
    if (dwAntennaID > MAX_NUM_ANT_SUPPORT)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "antennaid is out of range");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("antennaid is out of range\r\n");
        return LLRP_StatusCode_A_OutOfRange;

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_CurrentState
* ����������У��AO��AO ״̬
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAccessSpecState eCurrentState AO ״̬
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_CurrentState(T_Msg *ptMsg,
        unsigned int dwMessageId,
        LLRP_tEAccessSpecState eCurrentState,
        LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*״̬����ȷ*/
    if ((LLRP_AccessSpecState_Disabled != eCurrentState)
            && (LLRP_AccessSpecState_Active != eCurrentState))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 3;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "AO current state is not disable and active");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("AO current state is not disable and active\r\n");
        return LLRP_StatusCode_A_Invalid;

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_ProtocolID
* ����������У��AO��Э���
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э���
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_ProtocolID(T_Msg *ptMsg,
        unsigned int dwMessageId,
        LLRP_tEAirProtocols eProtocolID,
        LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));

    if ((LLRP_AirProtocols_Unspecified == eProtocolID)
            || ((LLRP_AirProtocols_EPCGlobalClass1Gen2 != eProtocolID)
                && (LLRP_AirProtocols_ISO18000_6B != eProtocolID)
                && (LLRP_AirProtocols_GB != eProtocolID)))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 2;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "protocal is not epc or iso6B");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("protocal is not epc or iso6B\r\n");
        return LLRP_StatusCode_A_Invalid;

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_StopTrigger
* ����������У��AO��stop trigger
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSAccessSpecStopTrigger * pAccessSpecStopTrigger, stop trigger
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_StopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAccessSpecStopTrigger * pAccessSpecStopTrigger, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (NULL == pAccessSpecStopTrigger)
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        strcpy((char *)tParameterError.ErrorDetail, "missing parameter AccessSpecStopTrigger");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("missing parameter AccessSpecStopTrigger\r\n");
        return LLRP_StatusCode_P_MissingParameter;
    }
    if ((LLRP_AccessSpecStopTriggerType_Operation_Count == pAccessSpecStopTrigger->eAccessSpecStopTrigger)
            && (0 == pAccessSpecStopTrigger->OperationCountValue))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 208;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
        strcpy((char *)tParameterError.ErrorDetail, "Invalid OperationCountValue");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("Invalid OperationCountValue\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_ReportSpec
* ����������У��AO���ϱ� trigger
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSAccessReportSpec * pAccessReportSpec �ϱ� trigger
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_ReportSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAccessReportSpec * pAccessReportSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (NULL != pAccessReportSpec)
    {
        if ((LLRP_AccessReportTriggerType_Whenever_ROReport_Is_Generated != pAccessReportSpec->eAccessReportTrigger)
                && (LLRP_AccessReportTriggerType_End_Of_AccessSpec != pAccessReportSpec->eAccessReportTrigger))
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 239;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "AccessReportTrigger is out of range");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("AccessReportTrigger is out of range\r\n");
            return LLRP_StatusCode_A_Invalid;
        }

    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_AccessCommand
* ����������У��AO�з�������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSAccessCommand * pAccessCommand��������
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AccessCommand(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSAccessCommand * pAccessCommand, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*����Ϊ��*/
    if (NULL == pAccessCommand)
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        strcpy((char *)tParameterError.ErrorDetail, "missing parameter AccessCommand");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("missing parameter AccessCommand\r\n");
        return LLRP_StatusCode_P_MissingParameter;

    }
    /*У��տ�Э��ģ��*/
    dwRet = rule_checkAOSpec_AccessCommand_AirProtocolTagSpec(ptMsg, dwMessageId,
            eProtocolID, pAccessCommand->pAirProtocolTagSpec, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��op����*/
    dwRet = rule_checkAOSpec_AccessCommand_OpSpec(ptMsg, dwMessageId,
            eProtocolID, pAccessCommand->listAccessCommandOpSpec, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��Imjָ��*/
    dwRet = rule_checkAOSpec_AccessCommand_C1G2CustomerSetting(ptMsg, dwMessageId,
            pAccessCommand->pC1G2CustomerSetting, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_AccessCommand_AirProtocolTagSpec
* ����������У��AccessCommand�пտ�Э��ģ��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSParameter * pAirProtocolTagSpec �տ�Э��ģ��
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AccessCommand_AirProtocolTagSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * pAirProtocolTagSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if ((NULL == pAirProtocolTagSpec)
            || ((RDP_LLRP_TAGSPEC_TYPE_EPC == pAirProtocolTagSpec->elementHdr.pType->TypeNum)
                && (LLRP_AirProtocols_EPCGlobalClass1Gen2 != eProtocolID)))
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 209;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
        strcpy((char *)tParameterError.ErrorDetail, "AirProtocolTagSpec type is epc but protocal is not epc");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("AirProtocolTagSpec type is epc but protocal is not epc\r\n");
        return LLRP_StatusCode_P_UnexpectedParameter;

    }
    if ((NULL == pAirProtocolTagSpec )
            || ((RDP_LLRP_TAGSPEC_TYPE_ISO6B == pAirProtocolTagSpec->elementHdr.pType->TypeNum)
                && (LLRP_AirProtocols_ISO18000_6B != eProtocolID)))
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 209;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
        strcpy((char *)tParameterError.ErrorDetail, "AirProtocolTagSpec type is iso6b but protocal is not iso6b");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("AirProtocolTagSpec type is iso6b but protocal is not iso6b\r\n");
        return LLRP_StatusCode_P_UnexpectedParameter;
    }
    if ((NULL == pAirProtocolTagSpec )
            || ((RDP_LLRP_TAGSPEC_TYPE_GB == pAirProtocolTagSpec->elementHdr.pType->TypeNum)
                && (LLRP_AirProtocols_GB != eProtocolID)))
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 209;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
        strcpy((char *)tParameterError.ErrorDetail, "AirProtocolTagSpec type is GB but protocal is not GB");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("AirProtocolTagSpec type is GB but protocal is not GB\r\n");
        return LLRP_StatusCode_P_UnexpectedParameter;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkEpcAccessOp
* ����������У��AccessCommand epc Э��op����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * ptAccessCommandOpSpec op ��Ϣ
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* ���������unsigned short int *pwOpSpecId   OpSpecId��Ϣ
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkEpcAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    LLRP_tSC1G2ReadCustomUID *ptLLRPC1G2ReadCustomUID = NULL;
    LLRP_tSC1G2Read *ptLLRPC1G2Read = NULL;
    LLRP_tSC1G2BlockWrite *ptLLRPC1G2BlockWrite = NULL;
    LLRP_tSC1G2Write *ptLLRPC1G2Write = NULL;
    LLRP_tSC1G2WriteVerify *ptLLRPC1G2WriteVerify = NULL;
    LLRP_tSC1G2Lock *ptLLRPC1G2Lock = NULL;
    LLRP_tSC1G2Kill *ptLLRPC1G2Kill = NULL;
    LLRP_tSC1G2BlockReadlock *ptLLRPC1G2BlockReadLock = NULL;
    LLRP_tSC1G2BlockPermalock *ptLLRPC1G2BlockPermalock = NULL;
    LLRP_tSC1G2BlockPermalockQuery *ptLLRPC1G2BlockPermalockQuery = NULL;
    LLRP_tSImpinjSetQTConfig *ptLLRPImpinjSetQTConfig = NULL;
    LLRP_tSImpinjGetQTConfig *ptLLRPImpinjGetQTConfig = NULL;
    LLRP_tSClientRequestOpSpec *pLLRPClientRequestOpSpec = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    switch (ptAccessCommandOpSpec->elementHdr.pType->TypeNum)
    {
    case LLRP_C1G2ReadCustomUID:
        {
            ptLLRPC1G2ReadCustomUID = (LLRP_tSC1G2ReadCustomUID *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2ReadCustomUID->OpSpecID;
            if ((ptLLRPC1G2ReadCustomUID->WordCount > 31)
                    || (ptLLRPC1G2ReadCustomUID->WordCount <= 0))
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 3;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 703;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_C1G2ReadCustomUID read WordCount is not in [1 31]");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_C1G2ReadCustomUID read WordCount is not in [1 31]\r\n");
                return LLRP_StatusCode_A_OutOfRange;
            }
            break;
        } 
    case LLRP_C1GOperationType_Read:
        {
            ptLLRPC1G2Read = (LLRP_tSC1G2Read *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2Read->OpSpecID;
            break;
        }
    case LLRP_C1GOperationType_Write:
        {
            ptLLRPC1G2Write = (LLRP_tSC1G2Write *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2Write->OpSpecID;
            break;
        }
    case LLRP_C1GOperationType_BLOCKWrite:
        {
            ptLLRPC1G2BlockWrite = (LLRP_tSC1G2BlockWrite *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2BlockWrite->OpSpecID;
            break;
        }
    case LLRP_C1GOperationType_Lock:
        {
            ptLLRPC1G2Lock = (LLRP_tSC1G2Lock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2Lock->OpSpecID;
            break;
        }
    case LLRP_C1GOperationType_Kill:
        {
            ptLLRPC1G2Kill = (LLRP_tSC1G2Kill*)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2Kill->OpSpecID;
            break;
        }
    case LLRP_C1G2_ALIENREADLOCK:
        {
            ptLLRPC1G2BlockReadLock = (LLRP_tSC1G2BlockReadlock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2BlockReadLock->OpSpecID;
            break;
        }
    case LLRP_C1G2_BLOCKLOCK:
        {
            ptLLRPC1G2BlockPermalock = (LLRP_tSC1G2BlockPermalock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2BlockPermalock->OpSpecID;
            break;
        }
    case LLRP_C1G2_BLOCKLOCKQUERY:
        {
            ptLLRPC1G2BlockPermalockQuery = (LLRP_tSC1G2BlockPermalockQuery *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2BlockPermalockQuery->OpSpecID;
            break;
        }
    case LLRP_C1G2WriteVerify:
        {
            ptLLRPC1G2WriteVerify = (LLRP_tSC1G2WriteVerify *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPC1G2WriteVerify->OpSpecID;
            break;
        }
    case LLRP_ImpinjSetQTConfig:
        {
            ptLLRPImpinjSetQTConfig = (LLRP_tSImpinjSetQTConfig *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPImpinjSetQTConfig->OpSpecID;
            if ((1 < ptLLRPImpinjSetQTConfig->Persistence)
                    || (1 < ptLLRPImpinjSetQTConfig->AccessRange)
                    || (1 < ptLLRPImpinjSetQTConfig->DataProfile))
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 210;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_ImpinjSetQTConfig parameter error >1");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_ImpinjSetQTConfig parameter error >1\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
            break;
        }
    case LLRP_ImpinjGetQTConfig:
        {
            ptLLRPImpinjGetQTConfig = (LLRP_tSImpinjGetQTConfig *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPImpinjGetQTConfig->OpSpecID;
            break;
        }
    case LLRP_ClientRequestOpSpec:
        {
            pLLRPClientRequestOpSpec = (LLRP_tSClientRequestOpSpec *)ptAccessCommandOpSpec;
            *pwOpSpecId = pLLRPClientRequestOpSpec->OpSpecID;
            if (0 == pLLRPClientRequestOpSpec->OpSpecID)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 210;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_ClientRequestOpSpec OpSpecId is 0");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_ClientRequestOpSpec OpSpecId is 0\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
            break;
        }
    default:
        {
            RDP_EXCEPT(RDP_INVALID_STATE,
                       ptAccessCommandOpSpec->elementHdr.pType->TypeNum, ptMsg->wMsgID, 0, RDP_CHECK_ID);
            RdpRuleDbgPrint("(RuleCheckPara) rule_checkEpcAccessOp invalid paratype is %d\r\n",
                            ptAccessCommandOpSpec->elementHdr.pType->TypeNum);
            break;
        }
    }
    RdpRuleDbgPrint("rule_checkEpcAccessOp  wOpSpecId=%d\r\n", *pwOpSpecId);
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkISO6BAccessOp
* ����������У��AccessCommand ISO6B Э��op����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * ptAccessCommandOpSpec op ��Ϣ
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* ���������unsigned short int *pwOpSpecId   OpSpecId��Ϣ
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkISO6BAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    LLRP_tSISO6BRead *ptLLRPISO6BRead = NULL;
    LLRP_tSISO6BWrite *ptLLRPISO6BWrite = NULL;
    LLRP_tSISO6BLock *ptLLRPISO6BLock = NULL;
    LLRP_tSISO6BReadCustomUID *ptLLRPISO6BReadCustomUID = NULL;
    LLRP_tSISO6BQueryLock *ptLLRPISO6BQueryLock = NULL;
    LLRP_tSClientRequestOpSpec *pLLRPClientRequestOpSpec = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    switch (ptAccessCommandOpSpec->elementHdr.pType->TypeNum)
    {
    case LLRP_ISO6BReadCustomUID:
        {
            ptLLRPISO6BReadCustomUID = (LLRP_tSISO6BReadCustomUID *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPISO6BReadCustomUID->OpSpecID;
            if ((ptLLRPISO6BReadCustomUID->ByteCount > 62)
                    || (ptLLRPISO6BReadCustomUID->ByteCount <= 0))
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 2;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 704;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_ISO6BReadCustomUID read ByteCount is not in [1 62]");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_ISO6BReadCustomUID read ByteCount is not in [1 62]\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
            break;
        }
    case LLRP_ISO6BOperationType_Read:
        {
            ptLLRPISO6BRead = (LLRP_tSISO6BRead *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPISO6BRead->OpSpecID;
            break;
        }

    case LLRP_ISO6BOperationType_Write:
        {
            ptLLRPISO6BWrite = (LLRP_tSISO6BWrite *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPISO6BWrite->OpSpecID;
            break;
        }
    case LLRP_ISO6BOperationType_Lock:
        {
            ptLLRPISO6BLock = (LLRP_tSISO6BLock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPISO6BLock->OpSpecID;
            break;
        }
    case LLRP_ISO6BOperationType_QueryLock:
        {
            ptLLRPISO6BQueryLock = (LLRP_tSISO6BQueryLock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPISO6BQueryLock->OpSpecID;
            break;
        }
    case LLRP_ClientRequestOpSpec:
        {
            pLLRPClientRequestOpSpec = (LLRP_tSClientRequestOpSpec *)ptAccessCommandOpSpec;
            *pwOpSpecId = pLLRPClientRequestOpSpec->OpSpecID;
            if (0 == pLLRPClientRequestOpSpec->OpSpecID)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 210;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_ClientRequestOpSpec OpSpecId is 0");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_ClientRequestOpSpec OpSpecId is 0\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
            break;
        }
    default:
        {
            RDP_EXCEPT(RDP_INVALID_STATE,
                       ptAccessCommandOpSpec->elementHdr.pType->TypeNum, ptMsg->wMsgID, 0, RDP_CHECK_ID);
            RdpRuleDbgPrint("(RuleCheckPara) rule_checkEpcAccessOp invalid paratype is %d\r\n",
                            ptAccessCommandOpSpec->elementHdr.pType->TypeNum);
            break;
        }
    }
    RdpRuleDbgPrint("rule_checkISO6BAccessOp  wOpSpecId=%d\r\n", *pwOpSpecId);
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkGBAccessOp
* ����������У��AccessCommand ���� Э��op����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * ptAccessCommandOpSpec op ��Ϣ
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* ���������unsigned short int *pwOpSpecId   OpSpecId��Ϣ
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkGBAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    LLRP_tSGBReadCustomUID *ptLLRPGBReadCustomUID = NULL;
    LLRP_tSGBRead *ptLLRPGBRead = NULL;
    LLRP_tSGBWrite *ptLLRPGBWrite = NULL;
    LLRP_tSGBErase *ptLLRPGBErase=NULL;
    LLRP_tSGBLock *ptLLRPGBLock = NULL;
    LLRP_tSGBKill *ptLLRPGBKill = NULL;
    LLRP_tSClientRequestOpSpec *pLLRPClientRequestOpSpec = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    switch (ptAccessCommandOpSpec->elementHdr.pType->TypeNum)
    {
    case LLRP_GBReadCustomUID:
        {
            ptLLRPGBReadCustomUID = (LLRP_tSGBReadCustomUID *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBReadCustomUID->OpSpecID;
            if ((ptLLRPGBReadCustomUID->WordCount > 31)
                    || (ptLLRPGBReadCustomUID->WordCount <= 0))
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 3;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 770;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_GBReadCustomUID read WordCount is not in [1 31]");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_GBReadCustomUID read WordCount is not in [1 31]\r\n");
                return LLRP_StatusCode_A_OutOfRange;
            }
		/*У��MB*/
	     dwRet=rule_checkGBOpMB(ptMsg, dwMessageId, ptLLRPMessage, ptLLRPGBReadCustomUID->MB, LLRP_GBReadCustomUID);
            break;
        }
    case LLRP_GBOperationType_Read:
        {
            ptLLRPGBRead = (LLRP_tSGBRead *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBRead->OpSpecID;
	     /*У��MB*/
	     dwRet=rule_checkGBOpMB(ptMsg, dwMessageId, ptLLRPMessage, ptLLRPGBRead->MB, LLRP_GBOperationType_Read);
            break;
        }       
    case LLRP_GBOperationType_Write:
        {
            ptLLRPGBWrite = (LLRP_tSGBWrite *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBWrite->OpSpecID;
	     /*У��MB*/
	     dwRet=rule_checkGBOpMB(ptMsg, dwMessageId, ptLLRPMessage, ptLLRPGBWrite->MB, LLRP_GBOperationType_Write);
            break;
        }
    case LLRP_GBOperationType_Erase:
        {
            ptLLRPGBErase = (LLRP_tSGBErase*)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBErase->OpSpecID;
	    /*У��MB*/
	     dwRet=rule_checkGBOpMB(ptMsg, dwMessageId, ptLLRPMessage, ptLLRPGBErase->MB, LLRP_GBOperationType_Erase);
            break;
        }
    case LLRP_C1GOperationType_Lock:
        {
            ptLLRPGBLock = (LLRP_tSGBLock *)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBLock->OpSpecID;
	     /*У��MB*/
	     dwRet=rule_checkGBOpMB(ptMsg, dwMessageId, ptLLRPMessage, ptLLRPGBLock->MB, LLRP_GBOperationType_Lock);
            break;
        }
    case LLRP_C1GOperationType_Kill:
        {
            ptLLRPGBKill = (LLRP_tSGBKill*)ptAccessCommandOpSpec;
            *pwOpSpecId = ptLLRPGBKill->OpSpecID;
            break;
        }
    case LLRP_ClientRequestOpSpec:
        {
            pLLRPClientRequestOpSpec = (LLRP_tSClientRequestOpSpec *)ptAccessCommandOpSpec;
            *pwOpSpecId = pLLRPClientRequestOpSpec->OpSpecID;
            if (0 == pLLRPClientRequestOpSpec->OpSpecID)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = 210;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_ClientRequestOpSpec OpSpecId is 0");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_ClientRequestOpSpec OpSpecId is 0\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
            break;
        }
    default:
        {
            RDP_EXCEPT(RDP_INVALID_STATE,
                       ptAccessCommandOpSpec->elementHdr.pType->TypeNum, ptMsg->wMsgID, 0, RDP_CHECK_ID);
            RdpRuleDbgPrint("(RuleCheckPara) rule_checkGBAccessOp invalid paratype is %d\r\n",
                            ptAccessCommandOpSpec->elementHdr.pType->TypeNum);
            break;
        }
    }
    RdpRuleDbgPrint("rule_checkGBAccessOp  wOpSpecId=%d\r\n", *pwOpSpecId);
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAccessOp
* ����������У��AccessCommand��op����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSParameter * pLLRPParameter op ��Ϣ
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* ���������unsigned short int *pwOpSpecId   OpSpecId��Ϣ
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAccessOp(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage, unsigned short int *pwOpSpecId)
{
    unsigned int dwRet = RDP_SUCCESS;

    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == eProtocolID)
    {
        /*У��6c op*/
        dwRet = rule_checkEpcAccessOp(ptMsg, dwMessageId, ptAccessCommandOpSpec, ptLLRPMessage, pwOpSpecId);
    }
    else if (LLRP_AirProtocols_ISO18000_6B == eProtocolID)
    {
        /*У��6B op*/
        dwRet = rule_checkISO6BAccessOp(ptMsg, dwMessageId, ptAccessCommandOpSpec, ptLLRPMessage, pwOpSpecId);
    }
    else if (LLRP_AirProtocols_GB == eProtocolID)
    {
        /*У��GB op*/
        dwRet = rule_checkGBAccessOp(ptMsg, dwMessageId, ptAccessCommandOpSpec, ptLLRPMessage, pwOpSpecId);
    }
    else
    {
        /*�쳣*/
        RDP_EXCEPT(RDP_INVALID_STATE, eProtocolID, ptMsg->wMsgID, 0, RDP_CHECK_ID);
        RdpRuleDbgPrint("(RuleCheckPara) rule_checkAccessOp invalid protocal type is %d\r\n", eProtocolID);
        dwRet = RDP_INVALID_STATE;
    }

    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_AccessCommand_OpSpec
* ����������У��AccessCommand��op��������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSParameter * ptAccessCommandOpSpec op����
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AccessCommand_OpSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSParameter * ptAccessCommandOpSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwLoop = 0;
    unsigned short int wLoop = 0;
    unsigned short int wOpIndex = 0;
    unsigned short int wUIDNum = 0;
    unsigned int dwImjOpIndex = 0;
    unsigned int dwUIDOpIndex = 0;
    unsigned int dwFirstRWOpIndex = 0;
    unsigned int bIfOpNotReadWrite = FALSE;
    unsigned int bIfImjExist = FALSE;
    unsigned int bIfGetRWOp = FALSE;
    T_ParameterError tParameterError;
    unsigned short int wOpSpecId[MAX_NUM_OPSPECS_PER_ACCESSSPEC] = {0};
    LLRP_tSParameter *pLLRPParameter = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));

    for (dwLoop = 0, pLLRPParameter = ptAccessCommandOpSpec;NULL != pLLRPParameter;
            pLLRPParameter = pLLRPParameter->pNextSubParameter , dwLoop++)

    {
        /*�������Ͳ�֧�ֲ�������*/
        if (LLRP_C1G2ReadCustomUID != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_ISO6BReadCustomUID != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_ISO6BOperationType_Read != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_ISO6BOperationType_Write != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_C1GOperationType_Read != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_C1GOperationType_Write != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_C1GOperationType_BLOCKWrite != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_ImpinjSetQTConfig != pLLRPParameter->elementHdr.pType->TypeNum &&
                LLRP_ImpinjGetQTConfig != pLLRPParameter->elementHdr.pType->TypeNum&&
                LLRP_GBOperationType_Read != pLLRPParameter->elementHdr.pType->TypeNum&&
                LLRP_GBOperationType_Write!= pLLRPParameter->elementHdr.pType->TypeNum&&
                LLRP_GBReadCustomUID!= pLLRPParameter->elementHdr.pType->TypeNum)
        {
            bIfOpNotReadWrite = TRUE;
        }
        if ((LLRP_ImpinjSetQTConfig == pLLRPParameter->elementHdr.pType->TypeNum)
                || (LLRP_ImpinjGetQTConfig == pLLRPParameter->elementHdr.pType->TypeNum))
        {
            bIfImjExist = TRUE;
            dwImjOpIndex = dwLoop;
        }
        else if (LLRP_C1G2ReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum)
        {
            dwUIDOpIndex = dwLoop;
            wUIDNum++;
        }
        else if (LLRP_ISO6BReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum)
        {
            wUIDNum++;
        }
	else if (LLRP_GBReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum)
        {
            wUIDNum++;
        }
        else if ((LLRP_ISO6BOperationType_Read == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_ISO6BOperationType_Write == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_C1GOperationType_Read == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_C1GOperationType_Write == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_C1GOperationType_BLOCKWrite == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_GBOperationType_Read == pLLRPParameter->elementHdr.pType->TypeNum)
                 || (LLRP_GBOperationType_Write == pLLRPParameter->elementHdr.pType->TypeNum))
        {
            if (FALSE == bIfGetRWOp)
            {
                dwFirstRWOpIndex = dwLoop;
                bIfGetRWOp = TRUE;
            }
        }
        if (((pLLRPParameter->elementHdr.pType->TypeNum > 636 && pLLRPParameter->elementHdr.pType->TypeNum < 681)
                || (LLRP_ISO6BReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum))
                && (LLRP_AirProtocols_ISO18000_6B != eProtocolID ))
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
            strcpy((char *)tParameterError.ErrorDetail, "op type is belonging to iso6b but the protocal is not 6B");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("op type is belonging to iso6b but the protocal is not 6B\r\n");
            return LLRP_StatusCode_P_UnexpectedParameter;
        }
        if (((pLLRPParameter->elementHdr.pType->TypeNum > 340 && pLLRPParameter->elementHdr.pType->TypeNum < 345 )
                || (LLRP_ImpinjSetQTConfig == pLLRPParameter->elementHdr.pType->TypeNum)
                || (LLRP_ImpinjGetQTConfig == pLLRPParameter->elementHdr.pType->TypeNum)
                || (LLRP_C1G2ReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum))
                && (LLRP_AirProtocols_EPCGlobalClass1Gen2 != eProtocolID))
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
            strcpy((char *)tParameterError.ErrorDetail, "op type is  belonging to epc but the protocal is not epc");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("op type is  belonging to epc but the protocal is not epc\r\n");
            return LLRP_StatusCode_P_UnexpectedParameter;
        }
	 if (((pLLRPParameter->elementHdr.pType->TypeNum >=758 && pLLRPParameter->elementHdr.pType->TypeNum <=762)
                || (LLRP_GBReadCustomUID == pLLRPParameter->elementHdr.pType->TypeNum))
                && (LLRP_AirProtocols_GB != eProtocolID ))
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
            strcpy((char *)tParameterError.ErrorDetail, "op type is  belonging to GB but the protocal is not GB");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("op type is  belonging to GB but the protocal is not GB\r\n");
            return LLRP_StatusCode_P_UnexpectedParameter;
        }
        /*�����������˺��ֹ��̨�·�ClientRequestOp*/
        if ((LLRP_ClientRequestOpSpec == pLLRPParameter->elementHdr.pType->TypeNum)
                && (TRUE == g_tRdpAdvancedConfig.tTagFilterCfg.ucTurnOnOPFilter))
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnexpectedParameter;
            strcpy((char *)tParameterError.ErrorDetail, "op type is LLRP_ClientRequestOpSpec but OpFilter is open");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("op type is LLRP_ClientRequestOpSpec but OpFilter is open\r\n");
            return LLRP_StatusCode_P_UnexpectedParameter;
        }
        if (dwLoop < MAX_NUM_OPSPECS_PER_ACCESSSPEC)
        {
            /*У��op�Ӳ�����Ϣ����ȡop id*/
            dwRet = rule_checkAccessOp(ptMsg, dwMessageId, eProtocolID, pLLRPParameter, ptLLRPMessage, &(wOpSpecId[dwLoop]));
            if (RDP_SUCCESS != dwRet)
            {
                return dwRet;
            }
        }
    }
    /*У��op��*/
    if ((dwLoop > MAX_NUM_OPSPECS_PER_ACCESSSPEC) || (0 == dwLoop))
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 209;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
        strcpy((char *)tParameterError.ErrorDetail, "OpNum is outofRange");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("OpNum is outofRange dwLoop=%d\r\n", dwLoop);
        return LLRP_StatusCode_P_OverflowParameter;
    }
    /*У��UID��*/
    if (wUIDNum > 1)
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
        tParameterError.wParameterType[0] = 207;
        tParameterError.wParameterType[1] = 209;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
        strcpy((char *)tParameterError.ErrorDetail, "ReadCustomUID NUM is more than 1");
        rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("ReadCustomUID NUM is more than 1 wUIDNum=%d\r\n", wUIDNum);
        return LLRP_StatusCode_P_OverflowParameter;
    }
    /*У������������*/
    if (TRUE == g_tRdpAdvancedConfig.tTagFilterCfg.ucTurnOnOPFilter)
    {
        /*if ((dwUIDOpIndex <= dwImjOpIndex) && (TRUE == bIfImjExist))
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
            strcpy((char *)tParameterError.ErrorDetail, "open opFilter but exist Img opindex bigger than UID opIndex");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("open opFilter but exist Img opindex bigger than UID opIndex dwUIDOpIndex=%d,dwImjOpIndex=5d\r\n",
                            dwUIDOpIndex, dwImjOpIndex);
            return LLRP_StatusCode_A_Invalid;
        }*/
        if (dwUIDOpIndex > dwFirstRWOpIndex)
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
            strcpy((char *)tParameterError.ErrorDetail, "open opFilter but exist UID opindex bigger than first Read or write opIndex ");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("open opFilter but exist UID opindex bigger than first Read or write opIndex dwUIDOpIndex=%d,dwFirstRWOpIndexs=%d\r\n",
                            dwUIDOpIndex, dwFirstRWOpIndex);
            return LLRP_StatusCode_A_Invalid;
        }
        /*����op��֧��*/
        if (TRUE == bIfOpNotReadWrite)
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 1;
            tParameterError.wParameterDepth = 2;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
            strcpy((char *)tParameterError.ErrorDetail, "open opFilter but op is not read or write");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("open opFilter but op is not read or write\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
        /*У��OpSpecId,���ܴ�����ͬ��opSpecID*/
        for (wOpIndex = 0;wOpIndex < dwLoop;wOpIndex++)
        {
            for (wLoop = wOpIndex + 1;wLoop < dwLoop;wLoop++)
            {
                if (wOpSpecId[wOpIndex] == wOpSpecId[wLoop])
                {
                    tParameterError.bIsLastField = FALSE;
                    tParameterError.wFieldNum = 1;
                    tParameterError.wParameterDepth = 2;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
                    tParameterError.wParameterType[0] = 207;
                    tParameterError.wParameterType[1] = 209;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
                    strcpy((char *)tParameterError.ErrorDetail, "open opFilter but OpSpecId is the same");
                    rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("open opFilter but OpSpecId is the same\r\n");
                    return LLRP_StatusCode_A_Invalid;
                }
            }
            /*OpSpecId���ܺ͹����ClientRequestOp�·���ID��ͬ*/
            if (RDP_CLIENT_REQUEST_OPSPEC_ID == wOpSpecId[wOpIndex])
            {
                tParameterError.bIsLastField = FALSE;
                tParameterError.wFieldNum = 1;
                tParameterError.wParameterDepth = 2;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_P_ParameterError;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_ParameterError;
                strcpy((char *)tParameterError.ErrorDetail, "open opFilter but OpSpecId is 38975");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("open opFilter but OpSpecId is 38975\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
        }
    }


    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkAOSpec_AccessCommand_OpSpec
* ����������У��AccessCommand��C1G2CustomerSetting
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSC1G2CustomerSetting * pC1G2CustomerSetting Imj����
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkAOSpec_AccessCommand_C1G2CustomerSetting(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSC1G2CustomerSetting * pC1G2CustomerSetting, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));

    if (NULL != pC1G2CustomerSetting)
    {
        RdpRuleDbgPrint("pC1G2CustomerSetting->CircumstanceSetting=%d\r\n", pC1G2CustomerSetting->CircumstanceSetting);
        if ((1 != pC1G2CustomerSetting->CircumstanceSetting)
                && (0 != pC1G2CustomerSetting->CircumstanceSetting))
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wParameterDepth = 3;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterType[0] = 207;
            tParameterError.wParameterType[1] = 209;
            tParameterError.wParameterType[2] = 5201;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "open opFilter but op is not read or write");
            rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("open opFilter but op is not read or write\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkAddAccessspec
* ����������У��ADD AO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSADD_ACCESSSPEC *ptLLRP_tSADD_ACCESSSPEC ��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkAddAccessspec(T_Msg *ptMsg , LLRP_tSADD_ACCESSSPEC *ptLLRP_tSADD_ACCESSSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwMessageID = 0;
    LLRP_tSADD_ACCESSSPEC_RESPONSE tSADD_ACCESSSPEC_RESPONSE;
    LLRP_tSMessage *ptLLRPMessage = NULL;
    LLRP_tSAccessSpec *ptLLRPAccessSpec = NULL;
    LLRP_tSLLRPStatus tSLLRPStatus;


    memset((void*)&tSADD_ACCESSSPEC_RESPONSE, 0, sizeof(tSADD_ACCESSSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;

    if (ptLLRP_tSADD_ACCESSSPEC != NULL)
    {
        tSADD_ACCESSSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdADD_ACCESSSPEC_RESPONSE;
        tSADD_ACCESSSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
        ptLLRPAccessSpec = ptLLRP_tSADD_ACCESSSPEC->pAccessSpec;
        dwMessageID = ptLLRP_tSADD_ACCESSSPEC->hdr.MessageID;
        ptLLRPMessage = (LLRP_tSMessage *) & tSADD_ACCESSSPEC_RESPONSE;
        RdpRuleDbgPrint("dwMessageID=%d\r\n", dwMessageID);
    }
    else
    {
        return RDP_FAILED;
    }
    /*У��AO id */
    dwRet = rule_checkAOSpec_AOSpecID(ptMsg, dwMessageID,
                                      ptLLRPAccessSpec->AccessSpecID, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У������id*/
    dwRet = rule_checkAOSpec_AntennaID(ptMsg, dwMessageID,
                                       ptLLRPAccessSpec->AntennaID, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��Э���*/
    dwRet = rule_checkAOSpec_ProtocolID(ptMsg, dwMessageID,
                                        ptLLRPAccessSpec->eProtocolID, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��AO״̬*/
    dwRet = rule_checkAOSpec_CurrentState(ptMsg, dwMessageID, ptLLRPAccessSpec->eCurrentState, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��stop trigger*/
    dwRet = rule_checkAOSpec_StopTrigger(ptMsg, dwMessageID, ptLLRPAccessSpec->pAccessSpecStopTrigger,
                                         ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У����ʲ�������*/
    dwRet = rule_checkAOSpec_AccessCommand(ptMsg, dwMessageID, ptLLRPAccessSpec->eProtocolID,
                                           ptLLRPAccessSpec->pAccessCommand, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У�鱨��trigger*/
    dwRet = rule_checkAOSpec_ReportSpec(ptMsg, dwMessageID,
                                        ptLLRPAccessSpec->pAccessReportSpec, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkDeleteAccesssSpec
* ����������У��delete AO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSDELETE_ACCESSSPEC *ptLLRP_tSDELETE_ACCESSSPEC ��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkDeleteAccesssSpec(T_Msg *ptMsg, LLRP_tSDELETE_ACCESSSPEC *ptLLRP_tSDELETE_ACCESSSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_RdpLlrpAccessEntry *ptAccessAddCursor = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRdpLlrpAccessCtrlAndParam = NULL;
    unsigned char ErrorDetail[100] = {0};
    // unsigned short int wMessageID =0;
    LLRP_tSDELETE_ACCESSSPEC_RESPONSE tSDELETE_ACCESSSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSDELETE_ACCESSSPEC_RESPONSE, 0, sizeof(tSDELETE_ACCESSSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSDELETE_ACCESSSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdDELETE_ACCESSSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSDELETE_ACCESSSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSDELETE_ACCESSSPEC->hdr.MessageID;
    tSDELETE_ACCESSSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    RdpRuleDbgPrint("delete AccessID =%d\r\n", ptLLRP_tSDELETE_ACCESSSPEC->AccessSpecID);
    RdpRuleDbgPrint("LinkIndex  =%d\r\n", ptMsg->ucLinkIndex);
    if (0 != ptLLRP_tSDELETE_ACCESSSPEC->AccessSpecID)
    {
        for (ptAccessAddCursor = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue);NULL != ptAccessAddCursor;ptAccessAddCursor = ptAccessAddCursor->ptNext)
        {
            ptRdpLlrpAccessCtrlAndParam = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessAddCursor->dwAccessIndex]);
            if (ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccess.dwAccessSpecID == ptLLRP_tSDELETE_ACCESSSPEC->AccessSpecID
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex == ptMsg->ucLinkIndex
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType == ptMsg->ucLinkType
                    && TRUE == ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucUseing)
            {
                return dwRet;
            }
        }
        /*û���ҵ���������*/
        if (NULL == ptAccessAddCursor)
        {
            /*�������� ����*/

            strcpy((char *)ErrorDetail, "invaild AccessSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSDELETE_ACCESSSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSDELETE_ACCESSSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild AccessSpecID2\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    else
    {
        return RDP_SUCCESS;
    }
    return RDP_SUCCESS;
}

/**************************************************************************
* �������ƣ�rule_checkDeleteROSpec
* ����������У��delete RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSDELETE_ROSPEC *ptLLRP_tSDELETE_ROSPEC ��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkDeleteROSpec(T_Msg *ptMsg, LLRP_tSDELETE_ROSPEC *ptLLRP_tSDELETE_ROSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wLoop = 0;
    unsigned char ErrorDetail[100] = {0};
    LLRP_tSDELETE_ROSPEC_RESPONSE tSDELETE_ROSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSDELETE_ROSPEC_RESPONSE, 0, sizeof(tSDELETE_ROSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSDELETE_ROSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdDELETE_ROSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;

    tSDELETE_ROSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSDELETE_ROSPEC->hdr.MessageID;
    tSDELETE_ROSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    RdpRuleDbgPrint("delete MessageID=%d\r\n", ptLLRP_tSDELETE_ROSPEC->hdr.MessageID);
    if (0 != ptLLRP_tSDELETE_ROSPEC->ROSpecID)
    {
        RdpRuleDbgPrint("delete ROSpecID=%d\r\n", ptLLRP_tSDELETE_ROSPEC->ROSpecID);
        for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
        {
            if ((ptLLRP_tSDELETE_ROSPEC->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                    && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                    && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                    && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
            {
                dwRet = RDP_SUCCESS;
                return dwRet;
            }

        }
        /*û���ҵ���������*/
        if (MAX_NUM_RO_SPEC == wLoop)
        {
            /*�������� ����*/
            strcpy((char *)ErrorDetail, "invaild ROSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSDELETE_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSDELETE_ROSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild delete ROSpecID\r\n");
            return LLRP_StatusCode_A_Invalid;

        }
    }
    else
    {

        return dwRet;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpecDepth
* ��������������ro llrp�ṹ���������ʧ����Ϣ���͸���̨
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSMessage *ptLLRPMessage llrp����
*                             T_ParameterError *ptParameterError ��������ṹ��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpecDepth(T_Msg *ptMsg,
                                    unsigned int dwMessageId,
                                    LLRP_tSMessage *ptLLRPMessage,
                                    T_ParameterError *ptParameterError)
{
    unsigned short int wLoop = 0;
    unsigned int dwRet = RDP_FAILED;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    LLRP_tSParameterError tSParameterError[RDP_MAX_LLRP_DEPTH];
    LLRP_tSADD_ROSPEC_RESPONSE *ptADD_ROSPEC_RESPONSE = NULL;

    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    memset((void*)&tSParameterError, 0, sizeof(tSParameterError));

    ptADD_ROSPEC_RESPONSE = (LLRP_tSADD_ROSPEC_RESPONSE *)ptLLRPMessage;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_ParameterError;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSLLRPStatus.pParameterError = &tSParameterError[0];
    ptADD_ROSPEC_RESPONSE->pLLRPStatus = &tSLLRPStatus;
    /*��ʼ��*/
    for (wLoop = 0;wLoop < RDP_MAX_LLRP_DEPTH;wLoop++)
    {
        tSParameterError[wLoop].hdr.elementHdr.pType = &LLRP_tdParameterError;
        tSParameterError[wLoop].eErrorCode = LLRP_StatusCode_P_ParameterError;
    }
    /*��������*/
    tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ptParameterError->ErrorDetail);
    tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ptParameterError->ErrorDetail;

    /*���һ�����֮ǰ�Ĳ�����*/
    for (wLoop = 0;wLoop < ptParameterError->wParameterDepth - 1;wLoop++)
    {
        tSParameterError[wLoop].eErrorCode = LLRP_StatusCode_P_ParameterError;
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].pParameterError = &tSParameterError[wLoop + 1];
    }

    /*���һ�����*/
    if (TRUE == ptParameterError->bIsLastField)
    {
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].eErrorCode = ptParameterError->wLastParameterErrorCode;
        tSParameterError[wLoop].pFieldError = &tSFieldError;
        tSFieldError.FieldNum = ptParameterError->wFieldNum;
        tSFieldError.eErrorCode = ptParameterError->wFieldErrorCode;
    }
    else
    {
        tSParameterError[wLoop].ParameterType = ptParameterError->wParameterType[wLoop];
        tSParameterError[wLoop].eErrorCode = ptParameterError->wLastParameterErrorCode;
        tSParameterError[wLoop].pParameterError = NULL;
    }
    rule_SendMsgToClient(ptMsg, dwMessageId, ptLLRPMessage);
    dwRet = (unsigned int)ptParameterError->wFieldErrorCode;
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_ROStartTrigger
* ����������У��ro start trigger
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSROSpecStartTrigger * pROSpecStartTrigger ro start trigger
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_ROStartTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROSpecStartTrigger * pROSpecStartTrigger, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*У��trigger type*/
    if (pROSpecStartTrigger->eROSpecStartTriggerType > LLRP_ROSpecStartTriggerType_GPI)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 179;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild ROStartTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild ROStartTrigger\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    /*У��trigger �Ƿ�����*/
    if ((LLRP_ROSpecStartTriggerType_GPI == pROSpecStartTrigger->eROSpecStartTriggerType
            && NULL == pROSpecStartTrigger->pGPITriggerValue)
            || (LLRP_ROSpecStartTriggerType_Periodic == pROSpecStartTrigger->eROSpecStartTriggerType
                && NULL == pROSpecStartTrigger->pPeriodicTriggerValue))
    {
        if (LLRP_ROSpecStartTriggerType_GPI == pROSpecStartTrigger->eROSpecStartTriggerType)
        {
            strcpy((char *)tParameterError.ErrorDetail, "the GPITriggerValue not exist in ROStartTrigger");
        }
        else
        {
            strcpy((char *)tParameterError.ErrorDetail, "the PeriodicTriggerValue not exist in ROStartTrigger");
        }

        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 2;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 179;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        if (LLRP_ROSpecStartTriggerType_GPI == pROSpecStartTrigger->eROSpecStartTriggerType)
        {
            RdpRuleDbgPrint("the GPITriggerValue not exist in ROStartTrigger\r\n");
        }
        else
        {
            RdpRuleDbgPrint("the PeriodicTriggerValue not exist in ROStartTrigger\r\n");
        }
        return LLRP_StatusCode_P_MissingParameter;
    }
    /*У��gpi trigger*/
    if (NULL != pROSpecStartTrigger->pGPITriggerValue
            && (pROSpecStartTrigger->pGPITriggerValue->GPIPortNum > MAX_GPI_NUM
                || 0 == pROSpecStartTrigger->pGPITriggerValue->GPIPortNum ) )
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 179;
        tParameterError.wParameterType[3] = 181;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild GPINum in ROStartTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild GPINum in ROStartTrigger\r\n");
        return LLRP_StatusCode_A_OutOfRange;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_ROStopTrigger
* ����������У��ro stop trigger
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLLRP_tSROSpecStopTrigger * pROSpecStopTrigger ro stop trigger
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_ROStopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROSpecStopTrigger * pROSpecStopTrigger, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*У��trigger type*/
    if (pROSpecStopTrigger->eROSpecStopTriggerType > LLRP_ROSpecStopTriggerType_GPI_With_Timeout)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 182;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild ROStopTrigger type");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);

        RdpRuleDbgPrint("invaild ROStopTrigger type\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    /*У��trigger �Ƿ�����*/
    if ((LLRP_ROSpecStopTriggerType_Duration == pROSpecStopTrigger->eROSpecStopTriggerType
            && 0 == pROSpecStopTrigger->DurationTriggerValue)
            || (LLRP_ROSpecStopTriggerType_GPI_With_Timeout == pROSpecStopTrigger->eROSpecStopTriggerType
                && NULL == pROSpecStopTrigger->pGPITriggerValue) )
    {
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 182;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        strcpy((char *)tParameterError.ErrorDetail, "the GPITriggerValue not exist in ROStartTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);

        RdpRuleDbgPrint("the GPITriggerValue not exist in ROStartTrigger\r\n");
        return LLRP_StatusCode_A_OutOfRange;
    }
    /*У��gpi trigger */
    if (NULL != pROSpecStopTrigger->pGPITriggerValue
            && (pROSpecStopTrigger->pGPITriggerValue->GPIPortNum > MAX_GPI_NUM
                || 0 == pROSpecStopTrigger->pGPITriggerValue->GPIPortNum))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 178;
        tParameterError.wParameterType[2] = 182;
        tParameterError.wParameterType[3] = 181;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild GPINum in ROStopTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);

        RdpRuleDbgPrint("invaild GPINum in ROStopTrigger\r\n");
        return LLRP_StatusCode_A_OutOfRange;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_listSpecParameter
* ����������У��listSpec
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * listSpecParameter listSpec����
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_listSpecParameter(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listSpecParameter, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    T_ParameterError tParameterError;
    LLRP_tSParameter *ptLLRP_tSParameter = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*У��RFSurvey*/
    for ( ptLLRP_tSParameter = listSpecParameter, dwTestData = 0;
            NULL != ptLLRP_tSParameter;ptLLRP_tSParameter = ptLLRP_tSParameter->pNextSubParameter, dwTestData++)
    {
        if (RFSURVEY_PARAMETER_TYPENUM == ptLLRP_tSParameter->elementHdr.pType->TypeNum )
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
            tParameterError.wParameterDepth = 1;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnsupportedParameter;
            strcpy((char *)tParameterError.ErrorDetail, "the RFSurvey is UnsupportedParameter");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("the RFSurvey is UnsupportedParameter\r\n");
            return LLRP_StatusCode_P_UnsupportedParameter;
        }
    }
    /*AI�����������*/
    if (dwTestData > MAX_NUM_SPECS_PERROSPEC)
    {
        RdpRuleDbgPrint("the AISpecNum is OutOfRange\r\n");
        tParameterError.bIsLastField = FALSE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 1;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
        strcpy((char *)tParameterError.ErrorDetail, "the AISpecNum is OutOfRange ");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        return LLRP_StatusCode_P_UnsupportedParameter;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec
* ����������У��AI
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSAISpec * pAISpec AI����
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAISpec * pAISpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    /*�ж�AI�еĲ���AntennaID*/
    dwRet = rule_checkROSpec_AISpec_AntennaIDs(ptMsg, dwMessageId, pAISpec->AntennaIDs, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }

    /*�ж�AI�в���StopTrigger�����ĺϷ���*/
    dwRet = rule_checkROSpec_AISpec_AISpecStopTrigger(ptMsg, dwMessageId, pAISpec->pAISpecStopTrigger, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*�ж�AI�в���listInventoryParameterSpec�����ĺϷ���*/
    dwRet = rule_checkROSpec_AISpec_InventoryParameterSpec(ptMsg, dwMessageId, pAISpec->listInventoryParameterSpec, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    return dwRet;

}

/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaIDs
* ����������У��AI ������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             llrp_u16v_t AntennaIDs���߲���
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaIDs(T_Msg *ptMsg, unsigned int dwMessageId, llrp_u16v_t AntennaIDs, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    unsigned int dwIndex = 0;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*У������*/
    RdpRuleDbgPrint("g_tRdpConfig.trAntennaExtConfiguration[0].eForandRevConfigType=%d\r\n",
                    g_tRdpConfig.trAntennaExtConfiguration[0].eForandRevConfigType);
    for (dwTestData = 0;dwTestData < AntennaIDs.nValue;dwTestData++)
    {
        if (AntennaIDs.pValue[dwTestData] > MAX_NUM_ANT_SUPPORT
                || (0 == AntennaIDs.pValue[dwTestData] && AntennaIDs.nValue > 1))
        {
            RdpRuleDbgPrint("the AntennaIDS has Error\r\n");
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
            tParameterError.wParameterDepth = 2;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has Error");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            return LLRP_StatusCode_P_FieldError;
        }
        if (1 == AntennaIDs.pValue[dwTestData]
                && LLRP_ForandRevConfigType_All_in_one != g_tRdpConfig.trAntennaExtConfiguration[0].eForandRevConfigType)
        {
            for (dwIndex = 0;dwIndex < AntennaIDs.nValue;dwIndex++)
            {
                if (3 == AntennaIDs.pValue[dwIndex])
                {
                    RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error3\r\n");
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 2;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    return LLRP_StatusCode_P_FieldError;
                }
            }

            if ( LLRP_ForandRevConfigType_Receive == g_tRdpConfig.trAntennaExtConfiguration[0].eForandRevConfigType)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 2;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error1\r\n");
                return LLRP_StatusCode_P_FieldError;
            }
        }

        if (2 == AntennaIDs.pValue[dwTestData]
                && LLRP_ForandRevConfigType_All_in_one != g_tRdpConfig.trAntennaExtConfiguration[1].eForandRevConfigType)
        {
            for (dwIndex = 0;dwIndex < AntennaIDs.nValue;dwIndex++)
            {
                if (4 == AntennaIDs.pValue[dwIndex])
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 2;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error2\r\n");
                    return LLRP_StatusCode_P_FieldError;
                }
            }
            if ( LLRP_ForandRevConfigType_Receive == g_tRdpConfig.trAntennaExtConfiguration[1].eForandRevConfigType)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 2;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error4\r\n");
                return LLRP_StatusCode_P_FieldError;
            }
        }


        if (3 == AntennaIDs.pValue[dwTestData]
                && LLRP_ForandRevConfigType_All_in_one != g_tRdpConfig.trAntennaExtConfiguration[2].eForandRevConfigType)
        {
            for (dwIndex = 0;dwIndex < AntennaIDs.nValue;dwIndex++)
            {
                if (1 == AntennaIDs.pValue[dwIndex])
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 2;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error5\r\n");
                    return LLRP_StatusCode_P_FieldError;
                }
            }
            if ( LLRP_ForandRevConfigType_Receive == g_tRdpConfig.trAntennaExtConfiguration[2].eForandRevConfigType)
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 2;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error6\r\n");
                return LLRP_StatusCode_P_FieldError;
            }
        }


        if (4 == AntennaIDs.pValue[dwTestData]
                && LLRP_ForandRevConfigType_All_in_one != g_tRdpConfig.trAntennaExtConfiguration[3].eForandRevConfigType)
        {
            for (dwIndex = 0;dwIndex < AntennaIDs.nValue;dwIndex++)
            {
                if (2 == AntennaIDs.pValue[dwIndex] )
                {
                    RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Erro7r\r\n");
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 2;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    return LLRP_StatusCode_P_FieldError;
                }
            }
            if ( LLRP_ForandRevConfigType_Receive == g_tRdpConfig.trAntennaExtConfiguration[3].eForandRevConfigType)
            {
                RdpRuleDbgPrint("the AntennaIDS has ForandRevConfig Error8\r\n");
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 2;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "the AntennaIDS has ForandRevConfig Error");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                return LLRP_StatusCode_P_FieldError;
            }
        }


    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AISpecStopTrigger
* ����������У��AI stop trigger
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSAISpecStopTrigger * pAISpecStopTrigger AI stop trigger
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AISpecStopTrigger(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAISpecStopTrigger * pAISpecStopTrigger, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*У��type*/
    if (pAISpecStopTrigger->eAISpecStopTriggerType > LLRP_AISpecStopTriggerType_Tag_Observation)
    {
        RdpRuleDbgPrint("invalid AISpecStopTrigger type\r\n");
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 184;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        strcpy((char *)tParameterError.ErrorDetail, "invalid AISpecStopTrigger type");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        return LLRP_StatusCode_A_Invalid;
    }
    /*У�����������*/
    if ((LLRP_AISpecStopTriggerType_GPI_With_Timeout == pAISpecStopTrigger->eAISpecStopTriggerType && NULL == pAISpecStopTrigger->pGPITriggerValue)
            || (LLRP_AISpecStopTriggerType_Tag_Observation == pAISpecStopTrigger->eAISpecStopTriggerType && NULL == pAISpecStopTrigger->pTagObservationTrigger) )
    {
        RdpRuleDbgPrint("MissingParameter in AIStopTrigger\r\n");
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 3;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 184;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_MissingParameter;
        strcpy((char *)tParameterError.ErrorDetail, "MissingParameter in AIStopTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        return LLRP_StatusCode_A_OutOfRange;
    }
    /*У��GPI*/
    if (LLRP_AISpecStopTriggerType_GPI_With_Timeout == pAISpecStopTrigger->eAISpecStopTriggerType
            && (pAISpecStopTrigger->pGPITriggerValue->GPIPortNum > MAX_GPI_NUM || 0 == pAISpecStopTrigger->pGPITriggerValue->GPIPortNum))
    {
        RdpRuleDbgPrint("invaild GPINum in AIStopTrigger\r\n");
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 184;
        tParameterError.wParameterType[3] = 181;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild GPINum in AIStopTrigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        return LLRP_StatusCode_A_OutOfRange;
    }
    if (LLRP_AISpecStopTriggerType_Tag_Observation == pAISpecStopTrigger->eAISpecStopTriggerType
            && ( LLRP_TagObservationTriggerType_N_Attempts_To_See_All_Tags_In_FOV_Or_Timeout < pAISpecStopTrigger->pTagObservationTrigger->eTriggerType))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 184;
        tParameterError.wParameterType[3] = 185;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild TagObersavationTriger type in AI stop trigger");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild TagObersavationTriger type in AI stop trigger\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    /*У���ǩ�۲�*/
    if (LLRP_AISpecStopTriggerType_Tag_Observation == pAISpecStopTrigger->eAISpecStopTriggerType
            && ( (LLRP_TagObservationTriggerType_N_Attempts_To_See_All_Tags_In_FOV_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType && 0 == pAISpecStopTrigger->pTagObservationTrigger->NumberOfAttempts )
                 || (LLRP_TagObservationTriggerType_Upon_Seeing_N_Tags_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType && 0 == pAISpecStopTrigger->pTagObservationTrigger->NumberOfTags )
                 || (LLRP_TagObservationTriggerType_Upon_Seeing_No_More_New_Tags_For_Tms_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType && 0 == pAISpecStopTrigger->pTagObservationTrigger->T )))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 184;
        tParameterError.wParameterType[3] = 185;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild Parameter in TagObersavationTriger of AIStopTriger");
        if (LLRP_TagObservationTriggerType_Upon_Seeing_N_Tags_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType )
        {
            tParameterError.wFieldNum = 1;
        }
        if ( LLRP_TagObservationTriggerType_N_Attempts_To_See_All_Tags_In_FOV_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType)
        {
            tParameterError.wFieldNum = 2;
        }
        if (LLRP_TagObservationTriggerType_Upon_Seeing_No_More_New_Tags_For_Tms_Or_Timeout == pAISpecStopTrigger->pTagObservationTrigger->eTriggerType)
        {
            tParameterError.wFieldNum = 3;
        }
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild Parameter in TagObersavationTriger of AIStopTrigger\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_InventoryParameterSpec
* ����������У��AI ������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSInventoryParameterSpec * pInventoryParameterSpec ������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_InventoryParameterSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSInventoryParameterSpec * pInventoryParameterSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    unsigned int dwIndex = 0;
    unsigned int dwAIndex = 0;
    unsigned short int wInventoryParameterSpecID[MAX_NUM_INVENTORY_PARA_SPECS_PERAISPEC] = {0};
    unsigned short int wProtocolID[MAX_NUM_INVENTORY_PARA_SPECS_PERAISPEC] = {0};
    LLRP_tSParameter *ptLLRP_tSParameter = NULL;
    LLRP_tSInventoryParameterSpec *ptLLRPInventoryParameterSpec = NULL;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    for (ptLLRP_tSParameter = (LLRP_tSParameter *)pInventoryParameterSpec, dwTestData = 0;
            NULL != ptLLRP_tSParameter;
            ptLLRP_tSParameter = ptLLRP_tSParameter->pNextSubParameter, dwTestData++)
        if (dwTestData > MAX_NUM_INVENTORY_PARA_SPECS_PERAISPEC)
        {
            tParameterError.bIsLastField = FALSE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 2;
            tParameterError.wParameterType[0]
            = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
            strcpy((char *)tParameterError.ErrorDetail, "the InventrorySpecNum is OutOfRange ");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("the InventrorySpecNum is OutOfRange\r\n");
            return LLRP_StatusCode_P_OverflowParameter;
        }
    /*���InventoryParameter�Ĵ���*/
    for (dwIndex = 0, ptLLRPInventoryParameterSpec = pInventoryParameterSpec;
            NULL != ptLLRPInventoryParameterSpec;
            ptLLRPInventoryParameterSpec = (LLRP_tSInventoryParameterSpec *)ptLLRPInventoryParameterSpec->hdr.pNextSubParameter, dwIndex++)
    {
        wInventoryParameterSpecID[dwIndex] = ptLLRPInventoryParameterSpec->InventoryParameterSpecID;
        wProtocolID[dwIndex] = ptLLRPInventoryParameterSpec->eProtocolID;
        if (0 == wInventoryParameterSpecID[dwIndex])
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 3;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid InventoryParameterSpecID");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid InventoryParameterSpecID\r\n");
            return LLRP_StatusCode_P_OverflowParameter;
        }
        if (1 != wProtocolID[dwIndex] && 100 != wProtocolID[dwIndex]&& 200 != wProtocolID[dwIndex])
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 3;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid eProtocolID");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid eProtocolID\r\n");
            return LLRP_StatusCode_P_OverflowParameter;
        }
    }
    /*InventroyID�ظ��ж�*/
    for (dwIndex = 0;dwIndex < dwTestData ;dwIndex++)
    {
        for (dwAIndex = dwIndex + 1;dwAIndex < dwTestData;dwAIndex++)
        {
            if (wInventoryParameterSpecID[dwIndex] == wInventoryParameterSpecID[dwAIndex])
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
                tParameterError.wParameterDepth = 3;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wParameterType[2] = 186;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "invalid InventoryParameterSpecID");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("invalid InventoryParameterSpecID\r\n");
                return LLRP_StatusCode_P_OverflowParameter;
            }
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration
* ����������У����������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSInventoryParameterSpec * pInventoryParameterSpec ������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSInventoryParameterSpec * ptlistInventoryParameterSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    LLRP_tSAntennaConfiguration * pAntennaConfiguration = NULL;

    /*�ж�AI�еĲ���AntennaID*/
    for (pAntennaConfiguration = ptlistInventoryParameterSpec->listAntennaConfiguration;NULL != pAntennaConfiguration;pAntennaConfiguration = (LLRP_tSAntennaConfiguration *)pAntennaConfiguration->hdr.pNextSubParameter)
    {

        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_AntennaID(ptMsg, dwMessageId, pAntennaConfiguration, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }
        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_RFTransmitter(ptMsg, dwMessageId, pAntennaConfiguration->pRFTransmitter, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }
        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd(ptMsg, dwMessageId, ptlistInventoryParameterSpec->eProtocolID, pAntennaConfiguration, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_AntennaID
* ����������У���������������ߺ�
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSAntennaConfiguration * pAntennaConfiguration��������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AntennaID(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSAntennaConfiguration * pAntennaConfiguration, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    unsigned int dwIndex = 0;
    unsigned int dwAIndex = 0;
    unsigned short int wAntennaID[MAX_NUM_ANT_SUPPORT] = {0};
    LLRP_tSAntennaConfiguration *ptLLRP_tSAntennaConfiguration = NULL;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    /*�ж�����ID�ĺϷ��� ����ID��0ʱ��AntennaConfiguration�������ܴ���1 ͬһ���������õĲ����ж�� ����ID�������ֵ*/
    for (dwTestData = 0, ptLLRP_tSAntennaConfiguration = pAntennaConfiguration;
            NULL != ptLLRP_tSAntennaConfiguration;
            ptLLRP_tSAntennaConfiguration = (LLRP_tSAntennaConfiguration *)ptLLRP_tSAntennaConfiguration->hdr.pNextSubParameter, dwTestData++)
    {
        wAntennaID[dwTestData] = ptLLRP_tSAntennaConfiguration->AntennaID;
        if (0 == wAntennaID[dwTestData])
        {
            dwAIndex = 0xff;
        }
        if (wAntennaID[dwTestData] > MAX_NUM_ANT_SUPPORT)
        {
            RdpRuleDbgPrint("invalid AntennaID in Antennaconfig\r\n");
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 4;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wParameterType[3] = 222;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid AntennaID in Antennaconfig");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            return LLRP_StatusCode_A_Invalid;
        }
    }
    if (0xff == dwAIndex && dwTestData > 1 )
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterDepth = 4;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 186;
        tParameterError.wParameterType[3] = 222;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invalid AntennaID in Antennaconfig");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invalid AntennaID in Antennaconfig\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    for (dwIndex = 0;dwIndex < dwTestData - 1 && dwTestData != 0;dwIndex++)
    {
        for (dwAIndex = dwIndex;dwAIndex < dwTestData - 1 && dwTestData != 0;dwAIndex++)
        {
            if (wAntennaID[dwAIndex] == wAntennaID[dwAIndex + 1])
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
                tParameterError.wParameterDepth = 4;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wParameterType[2] = 186;
                tParameterError.wParameterType[3] = 222;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "invalid AntennaID in Antennaconfig");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("invalid AntennaID in Antennaconfig\r\n");
                return LLRP_StatusCode_A_Invalid;
            }
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_RFTransmitter
* ����������У�����������з��͹���
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSRFTransmitter * pRFTransmitter ���书��
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_RFTransmitter(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSRFTransmitter * pRFTransmitter, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (NULL != pRFTransmitter && pRFTransmitter->HopTableID > 1)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 5;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 186;
        tParameterError.wParameterType[3] = 222;
        tParameterError.wParameterType[4] = 224;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invalid HopTableID in RFTransmitter of Antennaconfig");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invalid HopTableID in RFTransmitter of Antennaconfig\r\n");
        return LLRP_StatusCode_A_OutOfRange;
    }
    if (pRFTransmitter != NULL
            && pRFTransmitter->TransmitPower > (OAM_CFG_ANT_POWER_MAX - OAM_CFG_ANT_POWER_MIN))
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 2;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterDepth = 5;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wParameterType[1] = 183;
        tParameterError.wParameterType[2] = 186;
        tParameterError.wParameterType[3] = 222;
        tParameterError.wParameterType[4] = 224;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invalid TransmitPower in RFTransmitter of Antennaconfig");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invalid TransmitPower in Antennaconfig\r\n");
        return LLRP_StatusCode_A_OutOfRange;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd
* ����������У�����������пտ�Э�����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEAirProtocols eProtocolID Э��
*                             LLRP_tSAntennaConfiguration * ptlistAntennaConfiguration��������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEAirProtocols eProtocolID, LLRP_tSAntennaConfiguration * ptlistAntennaConfiguration, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    LLRP_tSParameter * ptlistAirProtocolInventoryCommandSettings = NULL;
    ptlistAirProtocolInventoryCommandSettings = ptlistAntennaConfiguration->listAirProtocolInventoryCommandSettings;
    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == eProtocolID)
    {
        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Epc(ptMsg, dwMessageId, ptlistAirProtocolInventoryCommandSettings, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }
    }
    else if(LLRP_AirProtocols_ISO18000_6B == eProtocolID)
    {
        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Iso6B(ptMsg, dwMessageId, ptlistAirProtocolInventoryCommandSettings, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }

    }
   else
    {
        dwRet = rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_GB(ptMsg, dwMessageId, ptlistAirProtocolInventoryCommandSettings, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }

    }

    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Epc
* ����������У�����������пտ�epcЭ�����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * listAirProtocolInventoryCommandSettings �տ�Э������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Epc(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listAirProtocolInventoryCommandSettings, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    T_ParameterError tParameterError;
    LLRP_tSC1G2InventoryCommand *ptLLRP_tSC1G2InventoryCommand = NULL;
    LLRP_tSC1G2Filter *ptLLRP_tSC1G2Filter = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (listAirProtocolInventoryCommandSettings != NULL)
    {
        ptLLRP_tSC1G2InventoryCommand = (LLRP_tSC1G2InventoryCommand *)listAirProtocolInventoryCommandSettings;
        ptLLRP_tSC1G2Filter = (LLRP_tSC1G2Filter *)ptLLRP_tSC1G2InventoryCommand->listC1G2Filter;
        if (ptLLRP_tSC1G2InventoryCommand->listC1G2Filter != NULL)
        {
            ptLLRP_tSC1G2Filter = (LLRP_tSC1G2Filter *)ptLLRP_tSC1G2InventoryCommand->listC1G2Filter;
            dwTestData = 0;
            do
            {
                dwTestData++;
                if (LLRP_C1G2TruncateAction_Do_Not_Truncate != ptLLRP_tSC1G2Filter->eT)
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 330;
                    tParameterError.wParameterType[5] = 331;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "invalid T  in C1G2Filter of Antennaconfig");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("invalid T  in C1G2Filter of Antennaconfig\r\n");

                    return LLRP_StatusCode_A_Invalid;
                }
                if (( ptLLRP_tSC1G2Filter->pC1G2TagInventoryMask->Pointer > 95) || (ptLLRP_tSC1G2Filter->pC1G2TagInventoryMask->TagMask.nBit > 96)
                        || (ptLLRP_tSC1G2Filter->pC1G2TagInventoryMask->TagMask.nBit > 96))
                {
                    tParameterError.bIsLastField = TRUE;
                    //tParameterError.wFieldNum =2;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 7;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 330;
                    tParameterError.wParameterType[5] = 331;
                    tParameterError.wParameterType[6] = 332;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "invalid point or TagMask in C1G2TagInventoryMask of Antennaconfig");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("invalid point or TagMask  in C1G2TagInventoryMask of Antennaconfig\r\n");

                    if (95 < ptLLRP_tSC1G2Filter->pC1G2TagInventoryMask->Pointer)
                    {
                        tParameterError.wFieldNum = 1;
                    }
                    else
                    {
                        tParameterError.wFieldNum = 2;
                    }
                    return LLRP_StatusCode_A_OutOfRange;
                }
                if (NULL != ptLLRP_tSC1G2Filter->pC1G2TagInventoryStateAwareFilterAction)
                {
                    tParameterError.bIsLastField = FALSE;
                    tParameterError.wFieldNum = 2;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 330;
                    tParameterError.wParameterType[5] = 331;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnsupportedParameter;
                    strcpy((char *)tParameterError.ErrorDetail, "C1G2TagInventoryStateAwareFilterAction  is not surported");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("C1G2TagInventoryStateAwareFilterAction  is not surported\r\n");
                    return LLRP_StatusCode_A_Invalid;
                }
                ptLLRP_tSC1G2Filter = (LLRP_tSC1G2Filter *)ptLLRP_tSC1G2Filter->hdr.pNextSubParameter;
            }
            while (ptLLRP_tSC1G2Filter != NULL);

            if (dwTestData > MAX_NUM_SELECT_FILTER_PERQUERY)
            {
                tParameterError.bIsLastField = FALSE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 5;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wParameterType[2] = 186;
                tParameterError.wParameterType[3] = 222;
                tParameterError.wParameterType[4] = 330;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
                strcpy((char *)tParameterError.ErrorDetail, "C1G2FilterNum is outofRange");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("C1G2FilterNum is outofRange \r\n");
                return LLRP_StatusCode_P_OverflowParameter;
            }
        }
        if (NULL != ptLLRP_tSC1G2InventoryCommand->pC1G2RFControl
                && ptLLRP_tSC1G2InventoryCommand->pC1G2RFControl->ModeIndex > EPC_RFMODE_TABLE_LEN
                && ptLLRP_tSC1G2InventoryCommand->pC1G2RFControl->ModeIndex != 10000)
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 1;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
            tParameterError.wParameterDepth = 6;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wParameterType[3] = 222;
            tParameterError.wParameterType[4] = 330;
            tParameterError.wParameterType[5] = 335;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid ModeIndex in C1G2RFControl");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid ModeIndex in C1G2RFControl\r\n");
            return LLRP_StatusCode_A_OutOfRange;
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Iso6B
* ����������У�����������пտ�ISO6BЭ�����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * listAirProtocolInventoryCommandSettings �տ�Э������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_Iso6B(T_Msg *ptMsg,
        unsigned int dwMessageId,
        LLRP_tSParameter * listAirProtocolInventoryCommandSettings,
        LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    LLRP_tSISO6BInventoryCommand *ptLLRP_tSISO6BInventoryCommand = NULL;
    LLRP_tSISO6BFilter *ptLLRP_tSISO6BFilter = NULL;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (listAirProtocolInventoryCommandSettings != NULL
            && 632 == listAirProtocolInventoryCommandSettings->elementHdr.pType->TypeNum)
    {
        /*6B������Ϣ*/
        ptLLRP_tSISO6BInventoryCommand = (LLRP_tSISO6BInventoryCommand *)listAirProtocolInventoryCommandSettings;
        if (ptLLRP_tSISO6BInventoryCommand->listISO6BFilter != NULL)
        {
            ptLLRP_tSISO6BFilter = (LLRP_tSISO6BFilter *)ptLLRP_tSISO6BInventoryCommand->listISO6BFilter;
            do
            {
                if (ptLLRP_tSISO6BFilter->Address > 208)
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 1;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 632;
                    tParameterError.wParameterType[5] = 634;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "invalid Address in ISO6BFilter");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("invalid Address in ISO6BFilter\r\n");

                    return LLRP_StatusCode_A_OutOfRange;
                }
                if (8 != ptLLRP_tSISO6BFilter->WordData.nValue)
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 3;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 632;
                    tParameterError.wParameterType[5] = 634;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "invalid WordData  in ISO6BFilter");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("invalid WordData  in ISO6BFilter\r\n");

                    return LLRP_StatusCode_A_Invalid;
                }
                if (ptLLRP_tSISO6BFilter->CommandType > 7)
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 632;
                    tParameterError.wParameterType[5] = 634;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                    strcpy((char *)tParameterError.ErrorDetail, "invalid CommandType in ISO6BFilter");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("invalid CommandType in ISO6BFilter\r\n");
                    return LLRP_StatusCode_A_Invalid;
                }
                ptLLRP_tSISO6BFilter = (LLRP_tSISO6BFilter *)ptLLRP_tSISO6BFilter->hdr.pNextSubParameter;
            }
            while (ptLLRP_tSISO6BFilter != NULL);
        }

        if (ptLLRP_tSISO6BInventoryCommand->pISO6BRFControl != NULL
                && ptLLRP_tSISO6BInventoryCommand->pISO6BRFControl->ModeIndex > ISO6B_RFMODE_TABLE_LEN)
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 6;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wParameterType[3] = 222;
            tParameterError.wParameterType[4] = 632;
            tParameterError.wParameterType[5] = 633;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid ModeIndex  in ISO6BRFControl");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid ModeIndex in ISO6BRFControl\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_GB
* ����������У�����������пտڹ���Э�����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSParameter * listAirProtocolInventoryCommandSettings �տ�Э������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_AISpec_AntennaConfiguration_AirProInvCmd_GB(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSParameter * listAirProtocolInventoryCommandSettings, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    T_ParameterError tParameterError;
    LLRP_tSGBInventoryCommand *ptLLRP_tSGBInventoryCommand = NULL;
    LLRP_tSGBFilter *ptLLRP_tSGBFilter = NULL;
    LLRP_tSGBTagInventoryMask * pGBTagInventoryMask=NULL;  
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (listAirProtocolInventoryCommandSettings != NULL)
    {
        ptLLRP_tSGBInventoryCommand = (LLRP_tSGBInventoryCommand *)listAirProtocolInventoryCommandSettings;
        ptLLRP_tSGBFilter = (LLRP_tSGBFilter *)ptLLRP_tSGBInventoryCommand->listGBFilter;
        if (ptLLRP_tSGBInventoryCommand->listGBFilter != NULL)
        {
            ptLLRP_tSGBFilter = (LLRP_tSGBFilter *)ptLLRP_tSGBInventoryCommand->listGBFilter;
            dwTestData = 0;
            do
            {
                dwTestData++;
                pGBTagInventoryMask =ptLLRP_tSGBFilter->pGBTagInventoryMask;           
                if ((NULL != pGBTagInventoryMask)&&(TRUE!=rule_checkGBMB(pGBTagInventoryMask->MB)))
                {
                    tParameterError.bIsLastField = TRUE;
                    tParameterError.wFieldNum = 0;
                    tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                    tParameterError.wParameterDepth = 6;
                    tParameterError.wParameterType[0] = 177;
                    tParameterError.wParameterType[1] = 183;
                    tParameterError.wParameterType[2] = 186;
                    tParameterError.wParameterType[3] = 222;
                    tParameterError.wParameterType[4] = 750;
                    tParameterError.wParameterType[5] = 752;
                    tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_UnsupportedParameter;
                    strcpy((char *)tParameterError.ErrorDetail, "MB in GBTagInventoryMask is error");
                    rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                    RdpRuleDbgPrint("MB in GBTagInventoryMask is error\r\n");
                    return LLRP_StatusCode_A_Invalid;
                }
                ptLLRP_tSGBFilter = (LLRP_tSGBFilter *)ptLLRP_tSGBFilter->hdr.pNextSubParameter;
            }
            while (ptLLRP_tSGBFilter != NULL);

            if (dwTestData > MAX_NUM_SORT_FILTER_PERQUERY)
            {
                tParameterError.bIsLastField = FALSE;
                tParameterError.wFieldNum = 0;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterDepth = 5;
                tParameterError.wParameterType[0] = 177;
                tParameterError.wParameterType[1] = 183;
                tParameterError.wParameterType[2] = 186;
                tParameterError.wParameterType[3] = 222;
                tParameterError.wParameterType[4] = 750;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_OverflowParameter;
                strcpy((char *)tParameterError.ErrorDetail, "GBFilterNum is outofRange");
                rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("GBFilterNum is outofRange \r\n");
                return LLRP_StatusCode_P_OverflowParameter;
            }
        }
        if (NULL != ptLLRP_tSGBInventoryCommand->pGBRFControl
                && ptLLRP_tSGBInventoryCommand->pGBRFControl->ModeIndex > GB_RFMODE_TABLE_LEN
                && ptLLRP_tSGBInventoryCommand->pGBRFControl->ModeIndex != 10000)
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 1;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
            tParameterError.wParameterDepth = 6;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 183;
            tParameterError.wParameterType[2] = 186;
            tParameterError.wParameterType[3] = 222;
            tParameterError.wParameterType[4] = 750;
            tParameterError.wParameterType[5] = 754;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid ModeIndex in GBRFControl");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid ModeIndex in GBRFControl\r\n");
            return LLRP_StatusCode_A_OutOfRange;
        }
	 /*CIN,CCN����ôУ��*/
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_ROReportSpec
* ����������У��RO ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSROReportSpec * pROReportSpec RO ��������
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_ROReportSpec(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSROReportSpec * pROReportSpec, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (NULL != pROReportSpec)
    {
        if (pROReportSpec->eROReportTrigger > LLRP_ROReportTriggerType_No_ROReport)
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterDepth = 2;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wParameterType[1] = 237;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invalid ROReport trigger type");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invalid ROReport trigger type\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_ROSpecID
* ����������У��RO id
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             unsigned int dwROSpecID RO id
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_ROSpecID(T_Msg *ptMsg, unsigned int dwMessageId, unsigned int dwROSpecID, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned int dwTestData = 0;
    unsigned short int wLoop = 0;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (0 == dwROSpecID)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild ROSpecID can not = 0");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild ROSpecID can not = 0\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    /*�޸�addro����Ϣ������֤*/
    for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
    {
        if ((dwROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
        {
            tParameterError.bIsLastField = TRUE;
            tParameterError.wFieldNum = 0;
            tParameterError.wParameterDepth = 1;
            tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
            tParameterError.wParameterType[0] = 177;
            tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
            strcpy((char *)tParameterError.ErrorDetail, "invaild ROSpecID");
            rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
            RdpRuleDbgPrint("invaild add ROSpecID\r\n");
            return LLRP_StatusCode_A_Invalid;
        }

    }

    for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
    {
        if (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
        {
            dwTestData++;
        }
    }
    if (MAX_NUM_RO_SPEC == dwTestData)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 0;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "the ROSpecNum is OutOfRange");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("the ROSpecNum is OutOfRange\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_Priority
* ����������У��RO ���ȼ�
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             unsigned char Priority ���ȼ�
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_Priority(T_Msg *ptMsg, unsigned int dwMessageId, unsigned char ucPriority, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (ucPriority > 7)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 1;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild Priority");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild Priority\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkROSpec_CurrentState
* ����������У��RO ״̬
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tEROSpecState eCurrentState RO״̬
*                             LLRP_tSMessage *ptLLRPMessage llrp ��Ϣ��Ӧ
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkROSpec_CurrentState(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tEROSpecState eCurrentState, LLRP_tSMessage *ptLLRPMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_ParameterError tParameterError;
    memset((void*)&tParameterError, 0, sizeof(tParameterError));
    if (eCurrentState != LLRP_ROSpecState_Disabled)
    {
        tParameterError.bIsLastField = TRUE;
        tParameterError.wFieldNum = 2;
        tParameterError.wParameterDepth = 1;
        tParameterError.wFieldErrorCode = LLRP_StatusCode_A_Invalid;
        tParameterError.wParameterType[0] = 177;
        tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
        strcpy((char *)tParameterError.ErrorDetail, "invaild  ROSpecState");
        rule_checkROSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
        RdpRuleDbgPrint("invaild  ROSpecState\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkAddROSpec
* ����������У��add RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LRP_tSADD_ROSPEC *ptLLRPAddRospec ��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkAddROSpec(T_Msg *ptMsg, LLRP_tSADD_ROSPEC *ptLLRPAddRospec)
{

    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wMessageID = 0;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSADD_ROSPEC_RESPONSE tLLRPAddRospecResponse;
    LLRP_tSMessage *ptLLRPMessage = NULL;
    LLRP_tSROSpec *ptLLRPROSpec = NULL;
    LLRP_tSAISpec *ptLLRPAISpec = NULL;
    LLRP_tSInventoryParameterSpec *ptLLRPInventoryParameterSpec = NULL;

    memset((void*)&tLLRPAddRospecResponse, 0, sizeof(tLLRPAddRospecResponse));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    if (NULL != ptLLRPAddRospec)
    {
        tLLRPAddRospecResponse.hdr.elementHdr.pType = &LLRP_tdADD_ROSPEC_RESPONSE;
        ptLLRPROSpec = ptLLRPAddRospec->pROSpec;
        wMessageID = ptLLRPAddRospec->hdr.MessageID;
        ptLLRPMessage = (LLRP_tSMessage *) & tLLRPAddRospecResponse;
    }
    else
    {
        return RDP_FAILED;
    }
    /*RO id*/
    dwRet = rule_checkROSpec_ROSpecID(ptMsg, wMessageID, ptLLRPROSpec->ROSpecID, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*prority*/
    dwRet = rule_checkROSpec_Priority(ptMsg, wMessageID, ptLLRPROSpec->Priority, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }

    /*��⵱ǰ״̬*/
    dwRet = rule_checkROSpec_CurrentState(ptMsg, wMessageID, ptLLRPROSpec->eCurrentState,
                                          ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*/*У��starttrigger*/
    dwRet = rule_checkROSpec_ROStartTrigger(ptMsg, wMessageID,
                                            ptLLRPROSpec->pROBoundarySpec->pROSpecStartTrigger, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��stoptrigger*/
    dwRet = rule_checkROSpec_ROStopTrigger(ptMsg, wMessageID,
                                           ptLLRPROSpec->pROBoundarySpec->pROSpecStopTrigger, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    /*У��listspec*/
    dwRet = rule_checkROSpec_listSpecParameter(ptMsg, wMessageID,
            ptLLRPROSpec->listSpecParameter, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }


    for (ptLLRPAISpec = (LLRP_tSAISpec *)ptLLRPROSpec->listSpecParameter;
            NULL != ptLLRPAISpec; ptLLRPAISpec = (LLRP_tSAISpec *)ptLLRPAISpec->hdr.pNextSubParameter)
    {
        /*У��AI*/
        dwRet = rule_checkROSpec_AISpec(ptMsg, wMessageID,
                                        (LLRP_tSAISpec *)ptLLRPROSpec->listSpecParameter, ptLLRPMessage);
        if (RDP_SUCCESS != dwRet)
        {
            return dwRet;
        }

        /*�ж���������*/
        for (ptLLRPInventoryParameterSpec = ptLLRPAISpec->listInventoryParameterSpec;
                NULL != ptLLRPInventoryParameterSpec;ptLLRPInventoryParameterSpec = \
                        (LLRP_tSInventoryParameterSpec *)ptLLRPInventoryParameterSpec->hdr.pNextSubParameter)
        {

            /*��������*/
            dwRet = rule_checkROSpec_AISpec_AntennaConfiguration(ptMsg, wMessageID,
                    ptLLRPInventoryParameterSpec, ptLLRPMessage);
            if (RDP_SUCCESS != dwRet)
            {
                return dwRet;
            }
        }

    }
    /*У��ro �ϱ�trigger*/
    dwRet = rule_checkROSpec_ROReportSpec(ptMsg, wMessageID,
                                          ptLLRPROSpec->pROReportSpec, ptLLRPMessage);
    if (RDP_SUCCESS != dwRet)
    {
        return dwRet;
    }
    return RDP_SUCCESS;
}


/**************************************************************************
* �������ƣ�rule_checkDisableAccesssSpec
* ����������У��Disable AO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSDISABLE_ACCESSSPEC *ptLLRP_tSDISABLE_ACCESSSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkDisableAccesssSpec(T_Msg *ptMsg, LLRP_tSDISABLE_ACCESSSPEC *ptLLRP_tSDISABLE_ACCESSSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_RdpLlrpAccessEntry *ptAccessAddCursor = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRdpLlrpAccessCtrlAndParam = NULL;
    unsigned char ErrorDetail[100] = {0};
    //unsigned short int wMessageID =0;
    LLRP_tSDISABLE_ACCESSSPEC_RESPONSE tSDISABLE_ACCESSSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSDISABLE_ACCESSSPEC_RESPONSE, 0, sizeof(tSDISABLE_ACCESSSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSDISABLE_ACCESSSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdDISABLE_ACCESSSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSDISABLE_ACCESSSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSDISABLE_ACCESSSPEC->hdr.MessageID;
    tSDISABLE_ACCESSSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    RdpRuleDbgPrint("disable AccessID =%d\r\n", ptLLRP_tSDISABLE_ACCESSSPEC->AccessSpecID);
    RdpRuleDbgPrint("LinkIndex  =%d\r\n", ptMsg->ucLinkIndex);
    if (0 != ptLLRP_tSDISABLE_ACCESSSPEC->AccessSpecID)
    {
        for (ptAccessAddCursor = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue);NULL != ptAccessAddCursor;ptAccessAddCursor = ptAccessAddCursor->ptNext)
        {
            ptRdpLlrpAccessCtrlAndParam = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessAddCursor->dwAccessIndex]);
            if (ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccess.dwAccessSpecID == ptLLRP_tSDISABLE_ACCESSSPEC->AccessSpecID
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex == ptMsg->ucLinkIndex
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType == ptMsg->ucLinkType
                    && TRUE == ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucUseing)
            {
                return dwRet;
            }
        }
        /*û���ҵ���������*/
        if (NULL == ptAccessAddCursor )
        {
            /*�������� ����*/
            strcpy((char *)ErrorDetail, "invaild AccessSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSDISABLE_ACCESSSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSDISABLE_ACCESSSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild AccessSpecID3\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    else
    {
        return dwRet;
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkDisableROSpec
* ����������У��Disable RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSDISABLE_ROSPEC *ptLLRP_tSDISABLE_ROSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkDisableROSpec(T_Msg *ptMsg, LLRP_tSDISABLE_ROSPEC *ptLLRP_tSDISABLE_ROSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wLoop = 0;
    unsigned char ErrorDetail[100] = {0};
    LLRP_tSDISABLE_ROSPEC_RESPONSE tSDISABLE_ROSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSDISABLE_ROSPEC_RESPONSE, 0, sizeof(tSDISABLE_ROSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSDISABLE_ROSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdDISABLE_ROSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSDISABLE_ROSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSDISABLE_ROSPEC->hdr.MessageID;
    tSDISABLE_ROSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    if (0 != ptLLRP_tSDISABLE_ROSPEC->ROSpecID)
    {
        for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
        {
            if ((ptLLRP_tSDISABLE_ROSPEC->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                    && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                    && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                    && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
            {
                dwRet = RDP_SUCCESS;
                return dwRet;
            }

        }
        /*û���ҵ���������*/
        if (MAX_NUM_RO_SPEC == wLoop)
        {
            /*�������� ����*/
            strcpy((char *)ErrorDetail, "invaild ROSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSDISABLE_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSDISABLE_ROSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild Disable ROSpecID\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    else
    {
        dwRet = RDP_SUCCESS;
        return dwRet;
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkEnableAccesssSpec
* ����������У��Enale AO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSENABLE_ACCESSSPEC *ptLLRP_tSENABLE_ACCESSSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkEnableAccesssSpec(T_Msg *ptMsg, LLRP_tSENABLE_ACCESSSPEC *ptLLRP_tSENABLE_ACCESSSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_RdpLlrpAccessEntry *ptAccessAddCursor = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRdpLlrpAccessCtrlAndParam = NULL;
    unsigned char ErrorDetail[100] = {0};
    // unsigned short int wMessageID =0;
    LLRP_tSENABLE_ACCESSSPEC_RESPONSE tSENABLE_ACCESSSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSENABLE_ACCESSSPEC_RESPONSE, 0, sizeof(tSENABLE_ACCESSSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSENABLE_ACCESSSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdENABLE_ACCESSSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;
    tSENABLE_ACCESSSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSENABLE_ACCESSSPEC->hdr.MessageID;
    tSENABLE_ACCESSSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    if (0 != ptLLRP_tSENABLE_ACCESSSPEC->AccessSpecID)
    {
        RdpRuleDbgPrint("ptLLRP_tSENABLE_ACCESSSPEC->AccessSpecID =%d\r\n", ptLLRP_tSENABLE_ACCESSSPEC->AccessSpecID);
        for (ptAccessAddCursor = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue);NULL != ptAccessAddCursor;ptAccessAddCursor = ptAccessAddCursor->ptNext)
        {
            ptRdpLlrpAccessCtrlAndParam = &(g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessAddCursor->dwAccessIndex]);
            if (ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccess.dwAccessSpecID == ptLLRP_tSENABLE_ACCESSSPEC->AccessSpecID
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex == ptMsg->ucLinkIndex
                    && ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType == ptMsg->ucLinkType
                    && TRUE == ptRdpLlrpAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucUseing)
            {
                tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_Success;
                RdpRuleDbgPrint("enable Access has finded The Right Access\r\n");
                rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ACCESSSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ACCESSSPEC_RESPONSE);
                return dwRet;
            }
        }
        /*û���ҵ���������*/
        if (NULL == ptAccessAddCursor)
        {
            /*�������� ����*/

            strcpy((char *)ErrorDetail, "invaild AccessSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            RdpRuleDbgPrint("enable Access has NOT finded The Right Access\r\n");
            rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ACCESSSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ACCESSSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild AccessSpecID4\r\n");
            return LLRP_StatusCode_A_Invalid;
        }
    }
    else
    {
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_Success;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ACCESSSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ACCESSSPEC_RESPONSE);
        return dwRet;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�rule_checkEnableROSpec
* ����������У��Enale RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             LLRP_tSENABLE_ROSPEC *ptLLRP_tSENABLE_ROSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkEnableROSpec(T_Msg *ptMsg, LLRP_tSENABLE_ROSPEC *ptLLRP_tSENABLE_ROSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wLoop = 0;
    unsigned char ErrorDetail[100] = {0};
    LLRP_tSENABLE_ROSPEC_RESPONSE tSENABLE_ROSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSENABLE_ROSPEC_RESPONSE, 0, sizeof(tSENABLE_ROSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSENABLE_ROSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdENABLE_ROSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;

    tSENABLE_ROSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSENABLE_ROSPEC->hdr.MessageID;
    tSENABLE_ROSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;
    RdpRuleDbgPrint("ptLLRP_tSENABLE_ROSPEC->ROSpecID=%d\r\n", ptLLRP_tSENABLE_ROSPEC->ROSpecID);
    if (0 != ptLLRP_tSENABLE_ROSPEC->ROSpecID)
    {
        for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
        {
            if ((ptLLRP_tSENABLE_ROSPEC->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                    && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                    && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                    && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
            {

                tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_Success;
                rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ROSPEC_RESPONSE);
                return dwRet;
            }

        }
        /*û���ҵ���������*/
        if (MAX_NUM_RO_SPEC == wLoop)
        {
            /*�������� ����*/
            strcpy((char *)ErrorDetail, "invaild ROSpecID");
            tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
            tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
            tSLLRPStatus.pFieldError = &tSFieldError;
            tSFieldError.FieldNum = 0;
            tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ROSPEC_RESPONSE);
            RdpRuleDbgPrint("invaild Enable ROSpecID\r\n");
            return LLRP_StatusCode_A_Invalid;

        }
    }
    else
    {
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_Success;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSENABLE_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSENABLE_ROSPEC_RESPONSE);
        return dwRet;
    }
    return dwRet;
}

/**************************************************************************
* �������ƣ�rule_checkStopROSpec
* ����������У��STOP RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                            LLRP_tSSTOP_ROSPEC *ptLLRP_tSSTOP_ROSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkStopROSpec(T_Msg *ptMsg, LLRP_tSSTOP_ROSPEC *ptLLRP_tSSTOP_ROSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wLoop = 0;
    unsigned char ErrorDetail[100] = {0};
    LLRP_tSSTOP_ROSPEC_RESPONSE tSSTOP_ROSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSSTOP_ROSPEC_RESPONSE, 0, sizeof(tSSTOP_ROSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSSTOP_ROSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdSTOP_ROSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;

    tSSTOP_ROSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSSTOP_ROSPEC->hdr.MessageID;
    tSSTOP_ROSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;

    /*RO id��0*/
    if (0 == ptLLRP_tSSTOP_ROSPEC->ROSpecID)
    {
        /*�������� ����*/
        strcpy((char *)ErrorDetail, "ROSpecID is 0 invalid");
        tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
        tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
        tSLLRPStatus.pFieldError = &tSFieldError;
        tSFieldError.FieldNum = 0;
        tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSSTOP_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSSTOP_ROSPEC_RESPONSE);
        RdpRuleDbgPrint("ROSpecID is 0 invalid\n");
        return LLRP_StatusCode_A_Invalid;
    }
    for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
    {
        if ((ptLLRP_tSSTOP_ROSPEC->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
        {
            return dwRet;
        }

    }
    /*û���ҵ���������*/
    if (MAX_NUM_RO_SPEC == wLoop)
    {
        /*�������� ����*/
        strcpy((char *)ErrorDetail, "invaild ROSpecID");
        tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
        tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
        tSLLRPStatus.pFieldError = &tSFieldError;
        tSFieldError.FieldNum = 0;
        tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSSTOP_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSSTOP_ROSPEC_RESPONSE);
        RdpRuleDbgPrint("invaild Stop ROSpecID\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return RDP_SUCCESS;
}

/**************************************************************************
* �������ƣ�rule_checkStartROSpec
* ����������У��START RO ��Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                            LLRP_tSSTART_ROSPEC *ptLLRP_tSSTART_ROSPEC��Ϣ�屾��
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
unsigned int rule_checkStartROSpec(T_Msg *ptMsg, LLRP_tSSTART_ROSPEC *ptLLRP_tSSTART_ROSPEC)
{
    unsigned int dwRet = RDP_SUCCESS;
    unsigned short int wLoop = 0;
    unsigned char ErrorDetail[100] = {0};
    LLRP_tSSTART_ROSPEC_RESPONSE tSSTART_ROSPEC_RESPONSE;
    LLRP_tSLLRPStatus tSLLRPStatus;
    LLRP_tSFieldError tSFieldError;
    memset((void*)&tSSTART_ROSPEC_RESPONSE, 0, sizeof(tSSTART_ROSPEC_RESPONSE));
    memset((void*)&tSLLRPStatus, 0, sizeof(tSLLRPStatus));
    memset((void*)&tSFieldError, 0, sizeof(tSFieldError));
    tSSTART_ROSPEC_RESPONSE.hdr.elementHdr.pType = &LLRP_tdSTART_ROSPEC_RESPONSE;
    tSLLRPStatus.hdr.elementHdr.pType = &LLRP_tdLLRPStatus;
    tSFieldError.hdr.elementHdr.pType = &LLRP_tdFieldError;

    tSSTART_ROSPEC_RESPONSE.hdr.MessageID = ptLLRP_tSSTART_ROSPEC->hdr.MessageID;
    tSSTART_ROSPEC_RESPONSE.pLLRPStatus = &tSLLRPStatus;

    /*RO id is 0*/
    if (0 == ptLLRP_tSSTART_ROSPEC->ROSpecID)
    {
        /*�������� ����*/
        strcpy((char *)ErrorDetail, "invaild ROSpecID=0");
        tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
        tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
        tSLLRPStatus.pFieldError = &tSFieldError;
        tSFieldError.FieldNum = 0;
        tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSSTART_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSSTART_ROSPEC_RESPONSE);
        RdpRuleDbgPrint("StartRO invaild ROSpecID=0\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    for (wLoop = 0;wLoop < MAX_NUM_RO_SPEC;wLoop++)
    {
        if ((ptLLRP_tSSTART_ROSPEC->ROSpecID == g_tRuleROCtrlAndParam[wLoop].dwROSpecID)
                && (TRUE == g_tRuleROCtrlAndParam[wLoop].dwUseing)
                && ((LLRP_ROSpecState_Inactive == g_tRuleROCtrlAndParam[wLoop].eCurrentState)
                    || (LLRP_ROSpecState_Active == g_tRuleROCtrlAndParam[wLoop].eCurrentState))
                && (ptMsg->ucLinkIndex == g_tRuleROCtrlAndParam[wLoop].ucLinkIndex)
                && (ptMsg->ucLinkType == g_tRuleROCtrlAndParam[wLoop].ucLinkType))
        {

            tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_Success;
            rule_SendMsgToClient(ptMsg, ptLLRP_tSSTART_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSSTART_ROSPEC_RESPONSE);
            return dwRet;
        }

    }
    /*û���ҵ���������*/
    if (MAX_NUM_RO_SPEC == wLoop)
    {
        /*�������� ����*/
        strcpy((char *)ErrorDetail, "invaild ROSpecID");
        tSLLRPStatus.ErrorDescription.nValue = (unsigned short int)strlen((char *)ErrorDetail);
        tSLLRPStatus.ErrorDescription.pValue = (unsigned char*)ErrorDetail;
        tSLLRPStatus.eStatusCode = LLRP_StatusCode_M_FieldError;
        tSLLRPStatus.pFieldError = &tSFieldError;
        tSFieldError.FieldNum = 0;
        tSFieldError.eErrorCode = LLRP_StatusCode_A_Invalid;
        rule_SendMsgToClient(ptMsg, ptLLRP_tSSTART_ROSPEC->hdr.MessageID, (LLRP_tSMessage *)&tSSTART_ROSPEC_RESPONSE);
        RdpRuleDbgPrint("invaild Start ROSpecID\r\n");
        return LLRP_StatusCode_A_Invalid;
    }
    return RDP_SUCCESS;
}
/**************************************************************************
* �������ƣ�rule_checkGBMB
* ����������У�����洢������
* ���ʵı���
* �޸ĵı���
* ���������unsigned char ucGBMB ����洢��
* �����������
* �� �� ֵ��TRUE: ������ȷ������Ϊ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/02/26    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkGBMB(unsigned char ucGBMB)
{
     if((0x00==ucGBMB)||(0x10==ucGBMB)||(0x20==ucGBMB)||((0x30<=ucGBMB)&&(0x3F>=ucGBMB)))
     	{
	   return TRUE;
     	}
     else
	{
	   return FALSE;
     	} 	
}
/**************************************************************************
* �������ƣ�rule_checkGBOpMB
* ����������У�������� �����д洢������
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptMsg ���յ�����Ϣ
*                             unsigned int dwMessageID llrp��ϢMessageID
*                             LLRP_tSMessage *ptLLRPMessage llrp��Ϣ��Ӧ�ṹ��
                               unsigned char ucGBMB ����洢��
                               unsigned int dwTypeNum ��������
* �����������
* �� �� ֵ��TRUE: ������ȷ������Ϊ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 14/02/19    V1.0       lzy        ����
**************************************************************************/
static unsigned int rule_checkGBOpMB(T_Msg *ptMsg, unsigned int dwMessageId, LLRP_tSMessage *ptLLRPMessage,unsigned char ucGBMB,unsigned int dwTypeNum)
{
     T_ParameterError tParameterError;
	memset((void*)&tParameterError, 0, sizeof(tParameterError));
	     if (FALSE==rule_checkGBMB(ucGBMB))
            {
                tParameterError.bIsLastField = TRUE;
                tParameterError.wFieldNum = 1;
                tParameterError.wParameterDepth = 3;
                tParameterError.wFieldErrorCode = LLRP_StatusCode_A_OutOfRange;
                tParameterError.wParameterType[0] = 207;
                tParameterError.wParameterType[1] = 209;
                tParameterError.wParameterType[2] = dwTypeNum;
                tParameterError.wLastParameterErrorCode = LLRP_StatusCode_P_FieldError;
                strcpy((char *)tParameterError.ErrorDetail, "LLRP_GBOperation MB error");
                rule_checkAOSpecDepth(ptMsg, dwMessageId, ptLLRPMessage, &tParameterError);
                RdpRuleDbgPrint("LLRP_GBOperation MB error\r\n");
                return LLRP_StatusCode_A_OutOfRange;

        }
	 return RDP_SUCCESS;
}

