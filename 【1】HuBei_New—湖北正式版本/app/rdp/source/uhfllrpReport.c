/***************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�ufhllrpReport.c
* �ļ���ʶ��
* ����ժҪ��LLRPЭ��report����ģ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2012.05.17 
**********************************************************************/


/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include"rdppub.h" 
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
unsigned char g_ucLocalReportDataBuffer[LLRP_MAX_REPORT_BUFFER];
T_RdpLlrpAcculateReportData g_tRdpLlrpReaderAcculateReport[MAX_LINK_NUM];

extern T_RdpLlrpReaderDispatch g_tRdpLlrpReaderDispatch;
extern T_RdpConfig g_tRdpConfig;

extern void *pair_socket_report;
/***********************************************************
 *                     ���ر���                            *
***********************************************************/


/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/


/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/


/**************************************************************************
* �������ƣ�llrp_getReportuSec
* �������������ر����ʱ��
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��  
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static unsigned long long llrp_getReportuSec()
{
    struct timeval tCurrentTime;
    unsigned long long ldwReportuSec = 0;
    if (gettimeofday(&tCurrentTime, NULL) != 0)
    {
        RdpLlrpDbgPrint("(LLRP)gettimeofday is error\r\n");
    }
    ldwReportuSec = (unsigned long long)tCurrentTime.tv_sec;
    ldwReportuSec = ldwReportuSec * 1000000 + tCurrentTime.tv_usec;
    return ldwReportuSec;
}
/**************************************************************************
* �������ƣ�llrp_getTagReportContentSelector
* �������������ر��������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��   T_USPRdpLlrpTagReportContentSelector * ���������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
T_USPRdpLlrpTagReportContentSelector * llrp_getTagReportContentSelector()
{
    if (g_tRdpLlrpReaderDispatch.ptReaderRoSpecData \
            ->tRdpLlrpROCtrlAndParam[g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex] \
            .tRdpLlrpROSpec.tRdpLlrpROReport.bIsDefaultCfgFlag != TRUE )
    {
        return &g_tRdpLlrpReaderDispatch.ptReaderRoSpecData \
               ->tRdpLlrpROCtrlAndParam[g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex] \
               .tRdpLlrpROSpec.tRdpLlrpROReport.tRdpLlrpTagReportContentSelector;
    }
    else
    {
        return &g_tRdpConfig.tROReportSpec.tRdpLlrpTagReportContentSelector;
    }

}

/**************************************************************************
* �������ƣ�llrp_constructLocalROReport
* �������������챾�ر��沢���뵽������
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int wProtoType,
                             unsigned short int wAntennaIndex,
                             unsigned short int wRssi,
                             unsigned short int wPC,
                             unsigned short int wCRC,
                             unsigned short int wChannelIndex,
                             unsigned short int wTagLen,
                             unsigned char     *pbTagID
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_constructLocalROReport(unsigned short int wProtoType,
                                 unsigned short int wAntennaIndex,
                                 unsigned short int wRssi,
                                 unsigned short int wPC,
                                 unsigned short int wCRC,
                                 unsigned short int wChannelIndex,
                                 unsigned short int wTagLen,
                                 unsigned char *pbTagID)
{
    unsigned int dwTestIndex = 0;
    T_RdpLlrpSingleReportCtrl *ptTempSingleReportCtr = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    T_RdpLlrpSingleReportCtrl *ptMallocSingleReportCtr = NULL;
    T_RdpLlrpTagReport *ptTempTagReport = NULL;
    T_RdpLlrpROTagReport *ptTempRoTagReport = NULL;
    T_USPUHFEpcInventoryRes *ptTempEpcInventoryRes = NULL;
    T_USPUHFISO6BInventoryRes *ptTempISO6BInventoryRes = NULL;
    T_USPRdpLlrpTagReportContentSelector *ptTempTagReportContentSelector = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpProtocolInvSet *ptProtocolInvSet = NULL;
    if ((wProtoType == LLRP_AirProtocols_EPCGlobalClass1Gen2)||(wProtoType == LLRP_AirProtocols_GB))
    {
        /*ISO6B ���ȵ�λ����*/
        ptTempSingleReportCtr->tUID.wTagDataLen = wTagLen * sizeof(unsigned short int);
    }
    else if (wProtoType == LLRP_AirProtocols_ISO18000_6B)
    {
        /*ISO6B ���ȵ�λ���ֽ�*/
        ptTempSingleReportCtr->tUID.wTagDataLen = wTagLen;
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)Rcp return an protocol type unsuported\r\n");
        /*д���쳣*/
        RDP_EXCEPT(RDP_LLRP_PROTO_ERROR, 0, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    memcpy(ptTempSingleReportCtr->tUID.aucTagData, pbTagID, ptTempSingleReportCtr->tUID.wTagDataLen);
    ptTempSingleReportCtr->ucReportType = LLRP_ROSPEC_REPORT_DATA;
    /*����������е�Ro*/
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    /*��ȡRO����*/
    ptTempSingleReportCtr->ucROType
    = ((LLRP_ROSpecStartTriggerType_Periodic == ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStartTrigger.dwROSpecStartTriggerType) ? \
       RO_START_TRIGGER_PERIOD : RO_START_TRIGGER_SINGLE);
    RdpLlrpDbgPrint("(LLRP)eROSpecStartTriggerType:%d,ucROType:%d\r\n",
                    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStartTrigger.dwROSpecStartTriggerType,
                    ptTempSingleReportCtr->ucROType);
    /*����ro�ڲ�����*/
    ptTempRoTagReport = &ptTempSingleReportCtr->tRdpLlrpTagReport.tROTagReport;
    /*��ȡ��ǰRo�ı�������ѡ�����*/
    ptTempTagReportContentSelector = llrp_getTagReportContentSelector();
    /*Reoport��ֵ*/
    ptTempRoTagReport->dwROSpecID = ptRunRoCtrlAndPara->tRdpLlrpROSpec.dwROSpecID;
    ptTempRoTagReport->wSpecIndex = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableSpecIndex,
                                    ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex,
                                    LLRP_DEFAULT_REPORT_WORD16);
    /*�����������*/
    ptTempRoTagReport->dwAccessSpecID = LLRP_DEFAULT_REPORT_WORD32;

    /*�ֱ��� ��һ�κ����һ�ε�ʱ��*/
    ptTempRoTagReport->lwFirstSeenTimestampUTC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableFirstSeenTimestamp,
            llrp_getReportuSec(),
            LLRP_DEFAULT_REPORT_WORD32);
    ptTempRoTagReport->lwLastSeenTimestampUTC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableLastSeenTimestamp,
            llrp_getReportuSec(),
            LLRP_DEFAULT_REPORT_WORD32);

    ptTempRoTagReport->wTagCount = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableTagSeenCount,
                                   1, LLRP_DEFAULT_REPORT_WORD16);
    ptProtocolInvSet = llrp_getRunAP();
    ptTempRoTagReport->wInventoryParameterSpecID = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableInventoryParameterSpecID,
            ptProtocolInvSet->wInvParamSpecID, LLRP_DEFAULT_REPORT_WORD16);
    ptTempRoTagReport->wPeakRSSI = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePeakRSSI,
                                   wRssi , LLRP_DEFAULT_REPORT_WORD16);

    ptTempRoTagReport->wAntennaID = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableAntennaID,
                                    wAntennaIndex, LLRP_DEFAULT_REPORT_WORD16);
    ptTempRoTagReport->wChannelIndex = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableChannelIndex,
                                       wChannelIndex, LLRP_DEFAULT_REPORT_WORD16);
    if (wProtoType == LLRP_AirProtocols_EPCGlobalClass1Gen2)
    {
        /*ISO6B ���ȵ�λ����*/
        ptTempRoTagReport->eProtocolID = LLRP_AirProtocols_EPCGlobalClass1Gen2;
        ptTempRoTagReport->wCRC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableCRC, wCRC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempRoTagReport->wPC_Bits = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePC, wPC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempRoTagReport->tID.tEpcID.wTagDataLen = wTagLen;
        memcpy(ptTempRoTagReport->tID.tEpcID.awTagData, pbTagID, wTagLen*sizeof(unsigned short int));
        RdpLlrpDbgPrint("(LLRP)EPC:");
        for (dwTestIndex = 0; dwTestIndex < ptTempRoTagReport->tID.tEpcID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x  ", ptTempRoTagReport->tID.tEpcID.awTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        /**/
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpROTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpEpcID, awTagData)
                                         + ptTempRoTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int);
    }
    else if (wProtoType == LLRP_AirProtocols_ISO18000_6B)
    {
        ptTempRoTagReport->eProtocolID = LLRP_AirProtocols_ISO18000_6B;
        ptTempRoTagReport->tID.tISOID.wTagDataLen = wTagLen;
        memcpy(ptTempRoTagReport->tID.tISOID.aucTagData, pbTagID, wTagLen);
        RdpLlrpDbgPrint("(LLRP)ISO6B TID :");
        for (dwTestIndex = 0; dwTestIndex < ptTempRoTagReport->tID.tISOID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptTempRoTagReport->tID.tISOID.aucTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpROTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpISOID, aucTagData)
                                         + ptTempRoTagReport->tID.tISOID.wTagDataLen;
    }
    else if (wProtoType == LLRP_AirProtocols_GB)
    {
        /*GB ���ȵ�λ����*/
        ptTempRoTagReport->eProtocolID = LLRP_AirProtocols_GB;
        ptTempRoTagReport->wPC_Bits = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePC, wPC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempRoTagReport->tID.tGBID.wTagDataLen = wTagLen;
        memcpy(ptTempRoTagReport->tID.tGBID.awTagData, pbTagID, wTagLen*sizeof(unsigned short int));
        RdpLlrpDbgPrint("(LLRP)EPC:");
        for (dwTestIndex = 0; dwTestIndex < ptTempRoTagReport->tID.tGBID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x  ", ptTempRoTagReport->tID.tGBID.awTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        /**/
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpROTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpGBID, awTagData)
                                         + ptTempRoTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int);
    }
    /*������ۼ�������*/
#if 1
    RdpLlrpDbgPrint("(LLRP)eProtocolID: %d\r\n", ptTempRoTagReport->eProtocolID );
    RdpLlrpDbgPrint("(LLRP)ROSpecID : %d\r\n", ptTempRoTagReport->dwROSpecID);
    RdpLlrpDbgPrint("(LLRP)dwAccessSpecID : %d\r\n", ptTempRoTagReport->dwAccessSpecID);
    RdpLlrpDbgPrint("(LLRP)SpecIndex : %d\r\n", ptTempRoTagReport->wSpecIndex);
    RdpLlrpDbgPrint("(LLRP)TagCount : %d\r\n", ptTempRoTagReport->wTagCount);
    RdpLlrpDbgPrint("(LLRP)scPeakRSSI : %d\r\n", ptTempRoTagReport->wPeakRSSI);
    RdpLlrpDbgPrint("(LLRP)ROSpecID : %d\r\n", ptTempRoTagReport->dwROSpecID);
    RdpLlrpDbgPrint("(LLRP)AntennaID : %d\r\n", ptTempRoTagReport->wAntennaID);
    RdpLlrpDbgPrint("(LLRP)ChannelIndex : %d\r\n", ptTempRoTagReport->wChannelIndex);
    RdpLlrpDbgPrint("(LLRP)CRC : %d\r\n", ptTempRoTagReport->wCRC);
    RdpLlrpDbgPrint("(LLRP)PC_Bits: %d\r\n", ptTempRoTagReport->wPC_Bits);
    RdpLlrpDbgPrint("(LLRP)InventoryParameterSpecID :%d\r\n", ptTempRoTagReport->wInventoryParameterSpecID);
#endif
    /*���ؽڵ㵽RO�ı���������*/
    RdpLlrpDbgPrint("(LLRP)insert RoReport into activeRO report link\r\n");
    llrp_insertReportToRunRoReportLink(ptTempSingleReportCtr);
}

/**************************************************************************
* �������ƣ�llrp_constructLocalAOReportHead
* ��������������AO����ĳ�ȥop����Ĳ���
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int wProtoType,
                             unsigned short int wAntennaIndex,
                             unsigned short int wRssi,
                             unsigned short int wPC,
                             unsigned short int wCRC,
                             unsigned short int wChannelIndex,
                             unsigned short int wTagLen,
                             unsigned char     *pbTagID
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_constructLocalAOReportHead(unsigned short int wProtoType,
                                     unsigned short int wAntennaIndex,
                                     unsigned short int wRssi,
                                     unsigned short int wPC,
                                     unsigned short int wCRC,
                                     unsigned short int wChannelIndex,
                                     unsigned short int wTagLen,
                                     unsigned char *pbTagID)
{
    unsigned int dwTestIndex = 0;
    T_RdpLlrpSingleReportCtrl *ptTempSingleReportCtr = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    T_RdpLlrpTagReport *ptTempTagReport = NULL;
    T_RdpLlrpAccessTagReport *ptTempAccessTagReport = NULL;
    T_USPRdpLlrpTagReportContentSelector *ptTempTagReportContentSelector = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptAccessCtrlAndParam = NULL;
    T_RdpLlrpProtocolInvSet *ptProtocolInvSet = NULL;
    RdpLlrpDbgPrint("(LLRP)local AoReport head: \r\n");
    if ((wProtoType == LLRP_AirProtocols_EPCGlobalClass1Gen2)
            || (wProtoType == LLRP_AirProtocols_ISO18000_6B)
            || (wProtoType == LLRP_AirProtocols_GB))
    {
        /*ISO6B ���ȵ�λ����*/
        ptTempSingleReportCtr->tUID.wTagDataLen = wTagLen;
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)Rcp return an protocol type unsuported\r\n");
        /*д���쳣*/
        RDP_EXCEPT(RDP_LLRP_PROTO_ERROR, wProtoType, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    memcpy(ptTempSingleReportCtr->tUID.aucTagData, pbTagID, ptTempSingleReportCtr->tUID.wTagDataLen);
    ptTempSingleReportCtr->ucReportType = LLRP_ACCESSSPEC_REPORT_DATA;
    /*����������е�Ro*/
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    /*��ȡRO��������*/
    ptTempSingleReportCtr->ucROType = ((LLRP_ROSpecStartTriggerType_Periodic == ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStartTrigger.dwROSpecStartTriggerType) ? RO_START_TRIGGER_PERIOD : RO_START_TRIGGER_SINGLE);
    RdpLlrpDbgPrint("(LLRP)eROSpecStartTriggerType :%d,ucROType:%d\r\n",
                    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStartTrigger.dwROSpecStartTriggerType,
                    ptTempSingleReportCtr->ucROType);
    /*����ro�ڲ�����*/
    ptTempAccessTagReport = &ptTempSingleReportCtr->tRdpLlrpTagReport.tAccessTagReport;
    /*��ȡ��ǰRo�ı�������ѡ�����*/
    ptTempTagReportContentSelector = llrp_getTagReportContentSelector();
    /*Reoport��ֵ*/
    ptTempAccessTagReport->dwROSpecID = ptRunRoCtrlAndPara->tRdpLlrpROSpec.dwROSpecID;
    ptTempAccessTagReport->wSpecIndex = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableSpecIndex,
                                        ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex,
                                        LLRP_DEFAULT_REPORT_WORD16);
    /*�����������*/
    ptAccessCtrlAndParam = llrp_getRunAccess();
    if (ptAccessCtrlAndParam == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    ptTempAccessTagReport->dwAccessSpecID = ptAccessCtrlAndParam->tRdpLlrpAccess.dwAccessSpecID;

    /*�ֱ��� ��һ�κ����һ�ε�ʱ��*/
    ptTempAccessTagReport->lwFirstSeenTimestampUTC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableFirstSeenTimestamp,
            llrp_getReportuSec(),
            LLRP_DEFAULT_REPORT_WORD32);
    ptTempAccessTagReport->lwLastSeenTimestampUTC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableLastSeenTimestamp,
            llrp_getReportuSec(),
            LLRP_DEFAULT_REPORT_WORD32);

    ptTempAccessTagReport->wTagCount = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableTagSeenCount,
                                       1, LLRP_DEFAULT_REPORT_WORD16);
    ptProtocolInvSet = llrp_getRunAP();
    ptTempAccessTagReport->wInventoryParameterSpecID = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableInventoryParameterSpecID,
            ptProtocolInvSet->wInvParamSpecID, LLRP_DEFAULT_REPORT_WORD16);
    ptTempAccessTagReport->wPeakRSSI = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePeakRSSI,
                                       wRssi , LLRP_DEFAULT_REPORT_WORD16);
    ptTempAccessTagReport->wAntennaID = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableAntennaID,
                                        wAntennaIndex, LLRP_DEFAULT_REPORT_WORD16);
    ptTempAccessTagReport->wChannelIndex = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableChannelIndex,
                                           wChannelIndex, LLRP_DEFAULT_REPORT_WORD16);
    if (wProtoType == LLRP_AirProtocols_EPCGlobalClass1Gen2)
    {
        /*ISO6B ���ȵ�λ����*/
        ptTempAccessTagReport->eProtocolID = LLRP_AirProtocols_EPCGlobalClass1Gen2;
        ptTempAccessTagReport->wCRC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableCRC, wCRC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempAccessTagReport->wPC_Bits = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePC, wPC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempAccessTagReport->tID.tEpcID.wTagDataLen = wTagLen / sizeof(unsigned short int);
        memcpy(ptTempAccessTagReport->tID.tEpcID.awTagData, pbTagID, wTagLen);
        RdpLlrpDbgPrint("(LLRP)EPC:");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReport->tID.tEpcID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x  ", ptTempAccessTagReport->tID.tEpcID.awTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        /**/
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpAccessTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpEpcID, awTagData)
                                         + ptTempAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int);
    }
    else if (wProtoType == LLRP_AirProtocols_ISO18000_6B)
    {
        ptTempAccessTagReport->eProtocolID = LLRP_AirProtocols_ISO18000_6B;
        ptTempAccessTagReport->tID.tISOID.wTagDataLen = wTagLen;
        memcpy(ptTempAccessTagReport->tID.tISOID.aucTagData, pbTagID, wTagLen);
        RdpLlrpDbgPrint("(LLRP)ISO6B TID :");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReport->tID.tISOID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptTempAccessTagReport->tID.tISOID.aucTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpAccessTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpISOID, aucTagData)
                                         + ptTempAccessTagReport->tID.tISOID.wTagDataLen;
    }
     if (wProtoType == LLRP_AirProtocols_GB)
    {
        /*ISO6B ���ȵ�λ����*/
        ptTempAccessTagReport->eProtocolID = LLRP_AirProtocols_GB;
        ptTempAccessTagReport->wCRC = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnableCRC, wCRC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempAccessTagReport->wPC_Bits = LLRP_SET_DATA_REPORT(ptTempTagReportContentSelector->bEnablePC, wPC, LLRP_DEFAULT_REPORT_WORD16);
        ptTempAccessTagReport->tID.tGBID.wTagDataLen = wTagLen / sizeof(unsigned short int);
        memcpy(ptTempAccessTagReport->tID.tGBID.awTagData, pbTagID, wTagLen);
        RdpLlrpDbgPrint("(LLRP)EPC:");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReport->tID.tGBID.wTagDataLen; dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x  ", ptTempAccessTagReport->tID.tGBID.awTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        /**/
        ptTempSingleReportCtr->wMsgLen = struct_fixed_len(T_RdpLlrpAccessTagReport, tID)
                                         + struct_fixed_len(T_RdpLlrpGBID, awTagData)
                                         + ptTempAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int);
    }
    /*������ۼ�������*/
#if 1
    RdpLlrpDbgPrint("(LLRP)eProtocolID:%d  ", ptTempAccessTagReport->eProtocolID);
    RdpLlrpDbgPrint("(LLRP)wMsgLen:%d    ", ptTempSingleReportCtr->wMsgLen);
    RdpLlrpDbgPrint("(LLRP)ROSpecID : %d   ", ptTempAccessTagReport->dwROSpecID);
    RdpLlrpDbgPrint("(LLRP)SpecIndex : %d   ", ptTempAccessTagReport->wSpecIndex);
    RdpLlrpDbgPrint("(LLRP)TagCount : %d  ", ptTempAccessTagReport->wTagCount);
    RdpLlrpDbgPrint("(LLRP)scPeakRSSI : %d  ", ptTempAccessTagReport->wPeakRSSI);
    RdpLlrpDbgPrint("(LLRP)ROSpecID : %d   ", ptTempAccessTagReport->dwROSpecID);
    RdpLlrpDbgPrint("(LLRP)AntennaID : %d  ", ptTempAccessTagReport->wAntennaID);
    RdpLlrpDbgPrint("(LLRP)ChannelIndex : %d  ", ptTempAccessTagReport->wChannelIndex);
    RdpLlrpDbgPrint("(LLRP)CRC : %d  ", ptTempAccessTagReport->wCRC);
    RdpLlrpDbgPrint("(LLRP)PC_Bits: %d  ", ptTempAccessTagReport->wPC_Bits);
    RdpLlrpDbgPrint("(LLRP)InventoryParameterSpecID :%d\r\n", ptTempAccessTagReport->wInventoryParameterSpecID);
#endif
    /*���ؽڵ㵽RO�ı���������*/

}


/**************************************************************************
* �������ƣ�llrp_constructLocalAOReportHeadForClientReuqest
* ����������
* ���ʵı���
* �޸ĵı���
* �������������clientreuqst���ػ�ʱAO�����ͷ��
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_constructLocalAOReportHeadForClientReuqest()
{
    unsigned int dwTestIndex = 0;
    unsigned short int wProtoType;
    unsigned short int wAntennaIndex;
    unsigned short int wRssi;
    unsigned short int wChannelIndex;
    unsigned short int wTagLen;
    unsigned char aucTagID[USP_RDP_LLRP_UID_SIZE];
    T_RdpLlrpSingleReportCtrl *ptTempSingleReportCtr = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    T_RdpLlrpAccessTagReport *ptTempAccessTagReport = NULL;
    if (ptTempSingleReportCtr->ucReportType == LLRP_ROSPEC_REPORT_DATA)
    {
        /*д���쳣�������ʹ���*/
        RDP_EXCEPT(RDP_LLRP_PROTO_ERROR, 0, 0, 0, RDP_LLRP_REPORT_ID);
        RdpLlrpDbgPrint("(LLRP)this is not access report an fatal error happen\r\n");
    }
    if (ptTempAccessTagReport->eProtocolID == LLRP_AirProtocols_EPCGlobalClass1Gen2)
    {
        wProtoType = LLRP_AirProtocols_EPCGlobalClass1Gen2;
        wTagLen = ptTempAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int);
        memcpy(aucTagID, ptTempAccessTagReport->tID.tEpcID.awTagData, wTagLen);
    }
    else if(ptTempAccessTagReport->eProtocolID == LLRP_AirProtocols_ISO18000_6B)
    {
        wProtoType = LLRP_AirProtocols_ISO18000_6B;
        wTagLen = ptTempAccessTagReport->tID.tISOID.wTagDataLen;
        memcpy(aucTagID, ptTempAccessTagReport->tID.tISOID.aucTagData, wTagLen);
    }
    else if (ptTempAccessTagReport->eProtocolID == LLRP_AirProtocols_GB)
    {
        wProtoType = LLRP_AirProtocols_GB;
        wTagLen = ptTempAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int);
        memcpy(aucTagID, ptTempAccessTagReport->tID.tGBID.awTagData, wTagLen);
    }

    wAntennaIndex = ptTempAccessTagReport->dwAccessSpecID;
    wChannelIndex = ptTempAccessTagReport->wChannelIndex;
    wRssi = ptTempAccessTagReport->wPeakRSSI;
    llrp_constructLocalAOReportHead(wProtoType,
                                    wAntennaIndex,
                                    wRssi,
                                    LLRP_DEFAULT_REPORT_WORD16,
                                    LLRP_DEFAULT_REPORT_WORD16,
                                    wChannelIndex,
                                    wTagLen,
                                    aucTagID);
}
/**************************************************************************
* �������ƣ�llrp_insertReportToRunRoReportLink
* �������������뱨�浽Ro��Report������
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl *ptLocalSingleReportCtrl
                             ���ر���ı���ڵ�
* ���������
* �� �� ֵ��  0 �ɹ�����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_insertReportToRunRoReportLink(T_RdpLlrpSingleReportCtrl *ptLocalSingleReportCtrl)
{
    T_RdpLlrpSingleReportCtrl *ptReportTail = NULL;
    T_RdpLlrpSingleReportCtrl *ptMallocSingleReportCtr = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAcculateReportData *ptActiveROReportLink = NULL;
    T_RdpLlrpOpResultCtrl *ptRdpLlrpOpResultCtrl = NULL;
    T_RdpLlrpEpcReadOpResult *ptRdpLlrpEpcReadOpResult = NULL;
    unsigned int dwTestIndex = 0;
    ptMallocSingleReportCtr = (T_RdpLlrpSingleReportCtrl *)malloc( struct_fixed_len(T_RdpLlrpSingleReportCtrl, tRdpLlrpTagReport) + ptLocalSingleReportCtrl->wMsgLen);
    /*ֱ�ӿ����ڴ�����*/
    memcpy(ptMallocSingleReportCtr,
           ptLocalSingleReportCtrl,
           struct_fixed_len(T_RdpLlrpSingleReportCtrl, tRdpLlrpTagReport) + ptLocalSingleReportCtrl->wMsgLen);
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_REPORT_ID);
        return ;
    }
    ptActiveROReportLink = &ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink;
    /*���ӱ������*/
    ptActiveROReportLink->dwReportSize = + (struct_fixed_len(T_RdpLlrpSingleReportCtrl, tRdpLlrpTagReport) + ptLocalSingleReportCtrl->wMsgLen);
    /*���ӱ�������*/
    ptActiveROReportLink->dwReportNum++;
    /*��������������,Ҫ�ϱ��澯��ȫ����д��һ������*/
    ptReportTail = ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail;
    /*û�нڵ�����*/
    if (ptReportTail == NULL)
    {

        ptMallocSingleReportCtr->ptlast = NULL;
        ptMallocSingleReportCtr->ptNext = NULL;
        /*���µ�ǰRO�����βָ��*/
        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail = ptMallocSingleReportCtr;
        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptAcculateReportHead = ptMallocSingleReportCtr;
        RdpLlrpDbgPrint("(LLRP)RoReportlink is empty this report is first, RoreportLink head addr:0x%x\r\n", ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptAcculateReportHead );

    }
    else
    {
        /*���µ�ǰRO����ָ���±��浽β�ڵ� */
        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail->ptNext = ptMallocSingleReportCtr;
        ptMallocSingleReportCtr->ptlast = ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail;
        ptMallocSingleReportCtr->ptNext = NULL;
        /*���µ�ǰRO�����βָ��*/
        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail = ptMallocSingleReportCtr;
        RdpLlrpDbgPrint("(LLRP)insert this report into RoReportLink, RoreportLink head addr:0x%x,ReportLink tail addr:0x%x\r\n",
                        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptAcculateReportHead,
                        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink.ptActiveRoReportTail);

    }
}

/**************************************************************************
* �������ƣ�llrp_freeReportToROReportLink
* �����������ͷ�Ro��������Ŀռ�
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpAcculateReportData *ptActiveRoReportLink
                             ��ǰ��Ro����ṹ
* ���������
* �� �� ֵ��  0 �ɹ�����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_freeReportToROReportLink(T_RdpLlrpAcculateReportData *ptActiveRoReportLink)
{
    T_RdpLlrpSingleReportCtrl *ptReportCursor = NULL;
    RdpLlrpDbgPrint("(LLRP)active Ro delete or disable  free Report\r\n");
    while (ptActiveRoReportLink->ptAcculateReportHead != NULL)
    {

        ptReportCursor = ptActiveRoReportLink->ptAcculateReportHead;
        ptActiveRoReportLink->ptAcculateReportHead = ptReportCursor->ptNext;
        free(ptReportCursor);
    }
    ptActiveRoReportLink->dwReportNum = 0;
    ptActiveRoReportLink->dwReportSize = 0;
    ptActiveRoReportLink->ptActiveRoReportTail = NULL;
    
}

/**************************************************************************
* �������ƣ�llrp_sendReport
* �������������ͱ����Reportģ��
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl *ptSendReport,
                             �����ͷ�ڵ��ַ
                             T_Msg *ptSndMsg
                             ��������msgID�������Ϣ
* ���������
* �� �� ֵ��  0 �ɹ�����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_sendReport(T_RdpLlrpSingleReportCtrl *ptSendReport, T_Msg *ptSndMsg, unsigned int dwMessgeID)
{
    /*ֱ�Ӱѵ�ַ���͸�Reportģ��*/
    T_RdpLlrpSendReportContent *ptSendReportContent = NULL;
    ptSendReportContent = (T_RdpLlrpSendReportContent *)(ptSndMsg + 1);
    ptSndMsg->wMsgID = EV_USP_RDP_SEND_LINK_REPORT;
    ptSendReportContent->dwMessgeID = dwMessgeID;
    ptSendReportContent->ptReportAddr = ptSendReport;
    if (NULL != ptSendReportContent->ptReportAddr)
    {
        RdpLlrpDbgPrint("(LLRP)llrp send report to Report module,the report head addr =0x%x,dwMessgeID=%d\r\n",
                        ptSendReportContent->ptReportAddr, dwMessgeID);
        if (0 > RDP_SEND_MSG(pair_socket_report, ptSndMsg))
        {
            /*����ʧ��д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, 0, 0, 0, RDP_LLRP_REPORT_ID);
        }
    }
}
/**************************************************************************
* �������ƣ�llrp_getRunRoMsgID
* ������������ȡ��������Ro����Ϣ��
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��  0 �ɹ�����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_getRunRoMsgID()
{ 
    T_RdpLlrpROCtrlAndParam *ptTempRdpLlrpROCtrlAndPara = NULL;
    ptTempRdpLlrpROCtrlAndPara =  llrp_getRunRO();
    if(ptTempRdpLlrpROCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_RO_STATE_ERROR, 0, 0, 0, RDP_LLRP_REPORT_ID);
    }
    else
    {
        return ptTempRdpLlrpROCtrlAndPara->tRdpLlrpRODispatchCtrl.dwRevMessageID;
    }
    return 0;
}
