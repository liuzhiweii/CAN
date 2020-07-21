/***************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�ReportSend.c
* �ļ���ʶ��
* ����ժҪ�����淢�ͺ͹�����غ���ʵ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�lzy 
* ������ڣ�2012.03.08
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
extern unsigned char g_aucReportSendDataBuf[READER_MAX_SEND_MSG_LEN];
extern void *push_socket_comm;
/***********************************************************
 *                     ���ر���                            *
***********************************************************/



/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/

/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/
static void report_constructRoReport(LLRP_tSTagReportData *ptTagReportData,
                                     T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl);
static void report_constructEpcOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
                                        LLRP_tSTagReportData *ptTagReportData);
static void report_constructIso6bOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
        LLRP_tSTagReportData *ptTagReportData);
static void report_constructGBOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
                                        LLRP_tSTagReportData *ptTagReportData);


/**************************************************************************
* �������ƣ�report_deleteReportNode
* ����������ɾ����·����
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl **ptSendReportHead, ��·����ͷָ��
*                             T_RdpLlrpSingleReportCtrl **ptOneReportCursor Ҫɾ���ĵ�ǰ����
 
* �����������
* �� �� ֵ    :   TRUEɾ��������·ͷָ��FALSE ɾ���Ĳ�����·ͷָ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
unsigned int report_deleteReportNode(T_RdpLlrpSingleReportCtrl **ptOneReportCursor, T_RdpLlrpSingleReportCtrl **ptSendReportHead)
{
    unsigned int bIsHeadNode = FALSE;
    T_RdpLlrpSingleReportCtrl *ptOneReportFreeNode = NULL;
    ptOneReportFreeNode = *ptOneReportCursor;
    /*��������ժ���ڵ�*/
    RdpReportDbgPrint("(*ptOneReportCursor)=0x%x,(*ptOneReportCursor)->ptlast =0x%x,(*ptOneReportCursor)->ptNext=0x%x\r\n",
                      (*ptOneReportCursor), (*ptOneReportCursor)->ptlast , (*ptOneReportCursor)->ptNext);
    if ((NULL != (*ptOneReportCursor)->ptlast ) && (NULL != (*ptOneReportCursor)->ptNext))
    {
        RdpReportDbgPrint("middle  Report  \r\n");
        (*ptOneReportCursor)->ptlast->ptNext = (*ptOneReportCursor)->ptNext;
        (*ptOneReportCursor)->ptNext->ptlast = (*ptOneReportCursor)->ptlast;

        if (*ptOneReportCursor == *ptSendReportHead)
        {
            bIsHeadNode = TRUE;
        }
    }
    else if ((NULL == (*ptOneReportCursor)->ptlast) && (NULL != (*ptOneReportCursor)->ptNext))
    {
        RdpReportDbgPrint("head Report  \r\n");
        bIsHeadNode = TRUE;
        (*ptOneReportCursor)->ptNext->ptlast = (*ptOneReportCursor)->ptlast;
    }
    else if ((NULL == (*ptOneReportCursor)->ptNext) && (NULL != (*ptOneReportCursor)->ptlast ))
    {
        RdpReportDbgPrint("tail Reort end \r\n");
        (*ptOneReportCursor)->ptlast->ptNext = (*ptOneReportCursor)->ptNext;

        if (*ptOneReportCursor == *ptSendReportHead)
        {
            bIsHeadNode = TRUE;
        }
    }
    else if ((NULL == (*ptOneReportCursor)->ptNext) && (NULL == (*ptOneReportCursor)->ptlast))
    {
        bIsHeadNode = TRUE;
        RdpReportDbgPrint("Only one Report in DeleteReportNode\r\n");
    }
    else
    {
        RdpReportDbgPrint("error happened\r\n");
    }

    *ptOneReportCursor = (*ptOneReportCursor)->ptNext;
    /*�ͷ�����ռ�*/
    free(ptOneReportFreeNode);
    RdpReportDbgPrint("delete this report\r\n");

    if (TRUE == bIsHeadNode)
    {
        *ptSendReportHead = *ptOneReportCursor;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**************************************************************************
* �������ƣ�report_insertReportNodeInHead
* �����������ڱ�������ͷ�������еı�����
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl **ptSendReportHead, ��·����ͷָ��
*                             T_RdpOpFilterHashEntry *ptCurrOpFilterNode ����ĵ�ǰ����
 
* �����������
* �� �� ֵ    :   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
void report_insertReportNodeInHead(T_RdpLlrpSingleReportCtrl **ptSendReportHead, T_RdpOpFilterHashEntry *ptCurrOpFilterNode)
{
    unsigned short int wTempLen = 0;
    T_RdpLlrpSingleReportCtrl *ptTempSignalReportCtrl = NULL;
    T_RdpLlrpSingleReportCtrl *ptTempSignalReportHead = NULL;
    if ((NULL == ptCurrOpFilterNode) || (NULL == ptCurrOpFilterNode->ptSignalReportCtrl))
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_SEND_ID);
        RdpReportDbgPrint("ptCurrOpFilterNode or ptCurrOpFilterNode->ptSignalReportCtrl is null\r\n");
        return ;
    }

    wTempLen = ptCurrOpFilterNode->ptSignalReportCtrl->wMsgLen + struct_fixed_len(T_RdpLlrpSingleReportCtrl, tRdpLlrpTagReport);
    ptTempSignalReportCtrl = (T_RdpLlrpSingleReportCtrl *)malloc(wTempLen);
    if (NULL == ptTempSignalReportCtrl)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_SEND_ID);
        RdpReportDbgPrint("mallco fail\r\n");
    }
    else
    {
        memcpy(ptTempSignalReportCtrl, ptCurrOpFilterNode->ptSignalReportCtrl, wTempLen);
    }
    if (NULL == *ptSendReportHead)
    { /*���п�*/
        *ptSendReportHead = ptTempSignalReportCtrl;
        (*ptSendReportHead)->ptlast = NULL;
        (*ptSendReportHead)->ptNext = NULL;
    }
    else if (NULL == (*ptSendReportHead)->ptlast)
    {
        /*������֮ǰ*/
        ptTempSignalReportHead = *ptSendReportHead;
        ptTempSignalReportCtrl->ptlast = NULL;
        ptTempSignalReportCtrl->ptNext = ptTempSignalReportHead;
        ptTempSignalReportHead->ptlast = ptTempSignalReportCtrl;
        *ptSendReportHead = ptTempSignalReportCtrl;
    }
    else
    {
        /*�쳣*/
        RDP_EXCEPT(RDP_INVALID_STATE, 0, 0, 0, RDP_SEND_ID);
        RdpReportDbgPrint("report_insertReportNodeInHead  abn error\r\n");
    }
    RdpReportDbgPrint("report_insertReportNodeInHead *ptSendReportHead=0x%x\r\n", *ptSendReportHead);
    return ;
}
/**************************************************************************
* �������ƣ�report_insertReportNodeInTail
* �����������ڱ�����β���벻���ڵı�����
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl **ptSendReportHead, ��·����ͷָ��
*                             T_RdpOpFilterHashEntry *ptCurrOpFilterNode ����ĵ�ǰ����
 
* �����������
* �� �� ֵ    :   0�ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
unsigned int report_insertReportNodeInTail(T_RdpLlrpSingleReportCtrl **ptSendReportHead,
                                     T_RdpLlrpSingleReportCtrl *ptSingleReportCtrl)
{
    unsigned int dwMallocLen = 0;
    T_RdpLlrpSingleReportCtrl *ptTempSignalReportCtrl = NULL;

    dwMallocLen = ptSingleReportCtrl->wMsgLen + struct_fixed_len(T_RdpLlrpSingleReportCtrl, tRdpLlrpTagReport);
    /*����ͷ*/
    if (NULL == *ptSendReportHead)
    {
        ptTempSignalReportCtrl = (T_RdpLlrpSingleReportCtrl *)malloc(dwMallocLen);

        if (NULL == ptTempSignalReportCtrl)
        {
            RdpReportDbgPrint("report_insertReportNodeInTail mallc failed\r\n");
            RDP_EXCEPT(RDP_BUF_NO_SPARE_MEM, 0, 0, 0, RDP_SEND_ID);
            return RDP_FAILED;
        }
        memmove(ptTempSignalReportCtrl, ptSingleReportCtrl, dwMallocLen);
        ptTempSignalReportCtrl->ptlast = NULL;
        ptTempSignalReportCtrl->ptNext = NULL;
        *ptSendReportHead = ptTempSignalReportCtrl;
    }
    else
    {
        /*�Ǳ�ͷ*/
        for (ptTempSignalReportCtrl = *ptSendReportHead;
                NULL != ptTempSignalReportCtrl->ptNext;
                ptTempSignalReportCtrl = ptTempSignalReportCtrl->ptNext)
        {}
        ptTempSignalReportCtrl->ptNext = (T_RdpLlrpSingleReportCtrl *)malloc(dwMallocLen);
        if (NULL == ptTempSignalReportCtrl)
        {
            RdpReportDbgPrint("report_insertReportNodeInTail mallc failed 1\r\n");
            RDP_EXCEPT(RDP_BUF_NO_SPARE_MEM, 0, 0, 0, RDP_SEND_ID);
            return RDP_FAILED;
        }
        memmove(ptTempSignalReportCtrl->ptNext, ptSingleReportCtrl, dwMallocLen);
        ptTempSignalReportCtrl->ptNext->ptlast = ptTempSignalReportCtrl;
        ptTempSignalReportCtrl->ptNext->ptNext = NULL;
    }

    return RDP_SUCCESS;
}

/**************************************************************************
* �������ƣ�report_constructRoReport
* ��������������RO����
* ���ʵı���
* �޸ĵı���
* ���������LLRP_tSTagReportData *ptTagReportData ת����llrp�ṹ
                              T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl  �ڲ�����ṹ
 
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/

static void report_constructRoReport(LLRP_tSTagReportData *ptTagReportData,
                                     T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl)
{
    char cRssi = 0;
    unsigned int dwTestIndex = 0;
    T_RdpLlrpROTagReport *ptTempROTagReportData = NULL;
    /*���ڹ���Report����*/
    LLRP_tSEPCData *ptEPCData = NULL;
    LLRP_tSUIDData *ptUIDData = NULL;
    LLRP_tSROSpecID *ptROSpecID = NULL;
    LLRP_tSSpecIndex *ptSpecIndex = NULL;
    LLRP_tSInventoryParameterSpecID *ptInventoryParameterSpecID = NULL;
    LLRP_tSAntennaID *ptAntennaID = NULL;
    LLRP_tSPeakRSSI *ptPeakRSSI = NULL;
    LLRP_tSChannelIndex *ptChannelIndex = NULL;
    LLRP_tSFirstSeenTimestampUTC *ptFirstSeenTimestampUTC = NULL;
    LLRP_tSLastSeenTimestampUTC *ptLastSeenTimestampUTC = NULL;
    LLRP_tSTagSeenCount *ptTagSeenCount = NULL;
    LLRP_tSAccessSpecID *ptAccessSpecID = NULL;
    LLRP_tSC1G2_CRC *ptC1G2_CRC = NULL;
    LLRP_tSC1G2_PC *ptC1G2_PC = NULL;
    LLRP_tSGBData *ptGBData = NULL;
    ptTempROTagReportData = &ptSignalReportCtrl->tRdpLlrpTagReport.tROTagReport;

    /*�Ƚ�EPC�Ƿ����һ������һ��*/
    /*Э�鲻��ͬ*/
    RdpReportDbgPrint("eProtocolID: %d\r\n", ptTempROTagReportData->eProtocolID );
    RdpReportDbgPrint("ROSpecID : %d\r\n", ptTempROTagReportData->dwROSpecID);
    RdpReportDbgPrint("dwAccessSpecID : %d\r\n", ptTempROTagReportData->dwAccessSpecID);
    RdpReportDbgPrint("SpecIndex : %d\r\n", ptTempROTagReportData->wSpecIndex);
    RdpReportDbgPrint("TagCount : %d\r\n", ptTempROTagReportData->wTagCount);
    RdpReportDbgPrint("scPeakRSSI : %d\r\n", ptTempROTagReportData->wPeakRSSI);
    RdpReportDbgPrint("ROSpecID : %d\r\n", ptTempROTagReportData->dwROSpecID);
    RdpReportDbgPrint("AntennaID : %d\r\n", ptTempROTagReportData->wAntennaID);
    RdpReportDbgPrint("ChannelIndex : %d\r\n", ptTempROTagReportData->wChannelIndex);
    RdpReportDbgPrint("CRC : %d\r\n", ptTempROTagReportData->wCRC);
    RdpReportDbgPrint("PC_Bits: %d\r\n", ptTempROTagReportData->wPC_Bits);
    RdpReportDbgPrint("InventoryParameterSpecID :%d\r\n", ptTempROTagReportData->wInventoryParameterSpecID);
    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == ptTempROTagReportData->eProtocolID)
    {
        /*ֱ�����ɱ���*/
        ptEPCData = LLRP_EPCData_construct();
        LLRP_EPCData_setEPC(ptEPCData, LLRP_u1v_construct(ptTempROTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) * 8));
        memcpy(ptEPCData->EPC.pValue, ptTempROTagReportData->tID.tEpcID.awTagData,
               ptTempROTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int));

        RdpDbgPrint("Epc tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempROTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptEPCData->EPC.pValue[dwTestIndex]);

        }
        RdpDbgPrintRaw("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptEPCData);
    }
    else if (LLRP_AirProtocols_ISO18000_6B == ptTempROTagReportData->eProtocolID)
    {
        ptUIDData = LLRP_UIDData_construct();
        LLRP_UIDData_setUID(ptUIDData, LLRP_u8v_construct(ptTempROTagReportData->tID.tISOID.wTagDataLen));
        memcpy(ptUIDData->UID.pValue,
               ptTempROTagReportData->tID.tISOID.aucTagData,
               ptTempROTagReportData->tID.tISOID.wTagDataLen);
        RdpReportDbgPrint("ISO tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempROTagReportData->tID.tISOID.wTagDataLen;dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptUIDData->UID.pValue[dwTestIndex]);

        }
        RdpReportDbgPrint("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptUIDData);
    }
    else if (LLRP_AirProtocols_GB == ptTempROTagReportData->eProtocolID)
    {
        /*ֱ�����ɱ���*/
        ptGBData = LLRP_GBData_construct();
        LLRP_GBData_setGBID(ptGBData, LLRP_u1v_construct(ptTempROTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int) * 8));
        memcpy(ptGBData->GBID.pValue, ptTempROTagReportData->tID.tGBID.awTagData,
               ptTempROTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int));

        RdpReportDbgPrint("GB tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempROTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptGBData->GBID.pValue[dwTestIndex]);

        }
        RdpReportDbgPrint("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptGBData);
    }
    else
    {
        RDP_EXCEPT(RDP_INVALID_STATE, 0, 0, 0, RDP_SEND_ID);
    }

    if (LLRP_DEFAULT_REPORT_WORD32 != ptTempROTagReportData->dwAccessSpecID)
    {
        /*����Access����*/
        ptAccessSpecID = LLRP_AccessSpecID_construct();
        ptAccessSpecID->AccessSpecID = ptTempROTagReportData->dwAccessSpecID;
        LLRP_TagReportData_setAccessSpecID(ptTagReportData, ptAccessSpecID);
    }

    if (LLRP_DEFAULT_REPORT_WORD32 != ptTempROTagReportData->dwROSpecID)
    {
        /*����RO��������*/
        ptROSpecID = LLRP_ROSpecID_construct();
        ptROSpecID->ROSpecID = ptTempROTagReportData->dwROSpecID;
        LLRP_TagReportData_setROSpecID(ptTagReportData, ptROSpecID);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wSpecIndex)
    {
        /*����RO��������*/
        ptSpecIndex = LLRP_SpecIndex_construct();
        ptSpecIndex->SpecIndex = ptTempROTagReportData->wSpecIndex;
        LLRP_TagReportData_setSpecIndex(ptTagReportData, ptSpecIndex);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wAntennaID)
    {
        /*�������ߵĽṹ*/
        ptAntennaID = LLRP_AntennaID_construct();
        ptAntennaID->AntennaID = ptTempROTagReportData->wAntennaID;
        LLRP_TagReportData_setAntennaID(ptTagReportData, ptAntennaID);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wChannelIndex)
    {
        /*�������ߵĽṹ*/
        ptChannelIndex = LLRP_ChannelIndex_construct();
        ptChannelIndex->ChannelIndex = ptTempROTagReportData->wChannelIndex;
        LLRP_TagReportData_setChannelIndex(ptTagReportData, ptChannelIndex);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wInventoryParameterSpecID)
    {
        /*����InventID����*/
        ptInventoryParameterSpecID = LLRP_InventoryParameterSpecID_construct();
        ptInventoryParameterSpecID->InventoryParameterSpecID = ptTempROTagReportData->wInventoryParameterSpecID;
        LLRP_TagReportData_setInventoryParameterSpecID(ptTagReportData,
                ptInventoryParameterSpecID);
    }

    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == ptTempROTagReportData->eProtocolID)
    {
        if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wCRC)
        {
            /*����Airdata����*/
            ptC1G2_CRC = LLRP_C1G2_CRC_construct();
            ptC1G2_CRC->CRC = ptTempROTagReportData->wCRC;
            LLRP_TagReportData_addAirProtocolTagData(ptTagReportData,
                    (LLRP_tSParameter *)ptC1G2_CRC);
        }

        if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wPC_Bits)
        {
            /*����Access����*/
            ptC1G2_PC = LLRP_C1G2_PC_construct();
            ptC1G2_PC->PC_Bits = ptTempROTagReportData->wPC_Bits;
            LLRP_TagReportData_addAirProtocolTagData(ptTagReportData,
                    (LLRP_tSParameter *)ptC1G2_PC);
        }
    }

    if (LLRP_DEFAULT_REPORT_WORD64 != ptTempROTagReportData->lwFirstSeenTimestampUTC)
    {
        ptFirstSeenTimestampUTC = LLRP_FirstSeenTimestampUTC_construct();
        ptFirstSeenTimestampUTC->Microseconds = ptTempROTagReportData->lwFirstSeenTimestampUTC;
        LLRP_TagReportData_setFirstSeenTimestampUTC(ptTagReportData,
                ptFirstSeenTimestampUTC);
    }

    if (LLRP_DEFAULT_REPORT_WORD64 != ptTempROTagReportData->lwLastSeenTimestampUTC)
    {
        ptLastSeenTimestampUTC = LLRP_LastSeenTimestampUTC_construct();
        ptLastSeenTimestampUTC->Microseconds = ptTempROTagReportData->lwLastSeenTimestampUTC;
        LLRP_TagReportData_setLastSeenTimestampUTC(ptTagReportData,
                ptLastSeenTimestampUTC);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wPeakRSSI)
    {
        ptPeakRSSI = LLRP_PeakRSSI_construct();
        /*��Ҫת��rssi*/
        cRssi = (char)10 * log10(ptTempROTagReportData->wPeakRSSI) - 85;
        ptPeakRSSI->PeakRSSI = cRssi;
        LLRP_TagReportData_setPeakRSSI(ptTagReportData, ptPeakRSSI);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempROTagReportData->wTagCount)
    {
        ptTagSeenCount = LLRP_TagSeenCount_construct();
        ptTagSeenCount->TagCount = ptTempROTagReportData->wTagCount;
        LLRP_TagReportData_setTagSeenCount(ptTagReportData, ptTagSeenCount);
    }
}
/**************************************************************************
* �������ƣ�report_constructEpcOpReport
* ��������������AO������EPC op�������
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpAccessTagReport ptAccessTagReport AO����
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
static void report_constructEpcOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
                                        LLRP_tSTagReportData *ptTagReportData)
{
    unsigned int dwOpIndex = 0;
    unsigned short int wLoop = 0;
    unsigned int dwOpResultIndex = 0;
    LLRP_tSC1G2ReadOpSpecResult *ptLLRPC1G2ReadOpSpecResult = NULL;
    LLRP_tSC1G2WriteOpSpecResult *ptLLRPC1G2WriteOpSpecResult = NULL;
    LLRP_tSC1G2BlockWriteOpSpecResult *ptLLRPC1G2BlockWriteOpSpecResult = NULL;
    LLRP_tSC1G2LockOpSpecResult *ptLLRPC1G2LockOpSpecResult = NULL;
    LLRP_tSC1G2KillOpSpecResult *ptLLRPC1G2KillOpSpecResult = NULL;
    LLRP_tSC1G2BlockPermalockOpSpecResult *ptLLRPC1G2BlockPermalockOpSpecResult = NULL;
    LLRP_tSC1G2BlockPermalockQueryOpSpecResult *ptLLRPC1G2BlockPermalockQueryOpSpecResult = NULL;
    LLRP_tSC1G2BlockReadlockOpSpecResult *ptLLRPC1G2BlockReadlockOpSpecResult = NULL;
    LLRP_tSC1G2WriteVerifyResult *ptLLRPC1G2WriteVerifyResult = NULL;
    T_RdpLlrpEpcBlcokLockOpResult *ptC1G2BlockLockOpSpecResult = NULL;
    T_RdpLlrpEpcReadOpResult *ptC1G2ReadOpSpecResult = NULL;
    T_RdpLlrpEpcWriteOpResult *ptC1G2WriteOpSpecResult = NULL;
    T_RdpLlrpEpcLockOpResult *ptC1G2LockOpSpecResult = NULL;
    T_RdpLlrpEpcKillOpResult *ptC1G2KillOpSpecResult = NULL;
    T_RdpLlrpEpcAlienReadLockOpResult *ptC1G2AlienReadLockOpSpecResult = NULL;
    T_RdpLlrpEpcWriteVerifyResult *ptC1G2WriteVerifyResult = NULL;
    T_RdpLlrpEpcImpinjSetQTOpResult *ptC1G2ImpinjSetQTConfigOpSpecResult = NULL;
    T_RdpLlrpEpcImpinjGetQTOpResult *ptC1G2ImpinjGetQTConfigOpSpecResult = NULL;
    LLRP_tSImpinjGetQTConfigOpSpecResult*ptLLRPImpinjGetQTConfigOpSpecResult = NULL;
    LLRP_tSImpinjSetQTConfigOpSpecResult *ptLLRPImpinjSetQTConfigOpSpecResult = NULL;

    /*����ÿ��op*/
    for (dwOpIndex = 0;dwOpIndex < ptAccessTagReport->dwOpNum;dwOpIndex++)
    {
        dwOpResultIndex = ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpResultAddr;
        RdpReportDbgPrint("dwOpIndex =%d,dwOpResultIndex=%d\r\n", dwOpIndex, dwOpResultIndex);
        switch (ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpType)
        {
        case LLRP_C1GOperationType_Read:
        case LLRP_C1G2ReadCustomUID:
            {
                RdpReportDbgPrint("LLRP_C1GOperationType_Read\r\n");
                ptLLRPC1G2ReadOpSpecResult = LLRP_C1G2ReadOpSpecResult_construct();
                ptC1G2ReadOpSpecResult = (T_RdpLlrpEpcReadOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2ReadOpSpecResult->OpSpecID = ptC1G2ReadOpSpecResult->wOpSpecID;
                ptLLRPC1G2ReadOpSpecResult->eResult = ptC1G2ReadOpSpecResult->eResult;
                RdpReportDbgPrint("result = %d,wDataLength=%d\r\n", ptLLRPC1G2ReadOpSpecResult->eResult, ptC1G2ReadOpSpecResult->wDataLength);
                LLRP_C1G2ReadOpSpecResult_setReadData(ptLLRPC1G2ReadOpSpecResult,
                                                      LLRP_u16v_construct(ptC1G2ReadOpSpecResult->wDataLength));

                if (0 != ptC1G2ReadOpSpecResult->wDataLength)
                {
                    memcpy(ptLLRPC1G2ReadOpSpecResult->ReadData.pValue,
                           ptC1G2ReadOpSpecResult->aucData,
                           ptC1G2ReadOpSpecResult->wDataLength * sizeof(unsigned short int));
                }

                for (wLoop = 0; wLoop < ptC1G2ReadOpSpecResult->wDataLength * sizeof(unsigned short int); wLoop++)
                {
                    RdpDbgPrintRaw("ptC1G2ReadOpSpecResult->aucData[%d] = %d\r\n",
                                      wLoop, ptC1G2ReadOpSpecResult->aucData[wLoop]);
                }

                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2ReadOpSpecResult);
                break;
            }
        case LLRP_C1GOperationType_Write:
            {
                RdpReportDbgPrint("LLRP_C1GOperationType_Write\r\n");
                ptLLRPC1G2WriteOpSpecResult = (LLRP_tSC1G2WriteOpSpecResult *)LLRP_C1G2WriteOpSpecResult_construct();
                ptC1G2WriteOpSpecResult = (T_RdpLlrpEpcWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2WriteOpSpecResult->OpSpecID = ptC1G2WriteOpSpecResult->wOpSpecID;
                ptLLRPC1G2WriteOpSpecResult->eResult = ptC1G2WriteOpSpecResult->eResult;
                ptLLRPC1G2WriteOpSpecResult->NumWordsWritten = ptC1G2WriteOpSpecResult->wNumWordsWritten;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2WriteOpSpecResult);
                break;
            }
        case LLRP_C1GOperationType_BLOCKWrite:
            {
                RdpReportDbgPrint("LLRP_C1GOperationType_BLOCKWrite\r\n");
                ptLLRPC1G2BlockWriteOpSpecResult = (LLRP_tSC1G2BlockWriteOpSpecResult *)LLRP_C1G2WriteOpSpecResult_construct();
                ptC1G2WriteOpSpecResult = (T_RdpLlrpEpcWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2BlockWriteOpSpecResult->OpSpecID = ptC1G2WriteOpSpecResult->wOpSpecID;
                ptLLRPC1G2BlockWriteOpSpecResult->eResult = (LLRP_tEC1G2BlockWriteResultType)ptC1G2WriteOpSpecResult->eResult;
                ptLLRPC1G2BlockWriteOpSpecResult->NumWordsWritten = ptC1G2WriteOpSpecResult->wNumWordsWritten;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2BlockWriteOpSpecResult);
                break;
            }
        case LLRP_C1GOperationType_Lock:
            {
                RdpReportDbgPrint("LLRP_C1GOperationType_Lock\r\n");
                ptLLRPC1G2LockOpSpecResult = (LLRP_tSC1G2LockOpSpecResult *)LLRP_C1G2LockOpSpecResult_construct();
                ptC1G2LockOpSpecResult = (T_RdpLlrpEpcLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2LockOpSpecResult->OpSpecID = ptC1G2LockOpSpecResult->wOpSpecID;
                ptLLRPC1G2LockOpSpecResult->eResult = ptC1G2LockOpSpecResult->eResult;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2LockOpSpecResult);
                break;
            }
        case LLRP_C1GOperationType_Kill:
            {
                RdpReportDbgPrint("LLRP_C1GOperationType_Kill\r\n");
                ptLLRPC1G2KillOpSpecResult = (LLRP_tSC1G2KillOpSpecResult *)LLRP_C1G2KillOpSpecResult_construct();
                ptC1G2KillOpSpecResult = (T_RdpLlrpEpcKillOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2KillOpSpecResult->OpSpecID = ptC1G2KillOpSpecResult->wOpSpecID;
                ptLLRPC1G2KillOpSpecResult->eResult = ptC1G2KillOpSpecResult->eResult;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2KillOpSpecResult);
                break;
            }
        case LLRP_C1G2_BLOCKLOCK:
            {
                RdpReportDbgPrint("LLRP_C1G2_BLOCKLOCK\r\n");
                ptLLRPC1G2BlockPermalockOpSpecResult = LLRP_C1G2BlockPermalockOpSpecResult_construct();
                ptC1G2BlockLockOpSpecResult = (T_RdpLlrpEpcBlcokLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2BlockPermalockOpSpecResult->OpSpecID = ptC1G2BlockLockOpSpecResult->wOpSpecID;
                ptLLRPC1G2BlockPermalockOpSpecResult->Result = ptC1G2BlockLockOpSpecResult->dwResult;
                RdpReportDbgPrint("ptLLRPC1G2BlockPermalockOpSpecResult->Result=%d\r\n", ptLLRPC1G2BlockPermalockOpSpecResult->Result);
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2BlockPermalockOpSpecResult);
                break;
            }
        case LLRP_C1G2_BLOCKLOCKQUERY:
            {
                RdpReportDbgPrint("LLRP_C1G2_BLOCKLOCKQUERY\r\n");
                ptLLRPC1G2BlockPermalockQueryOpSpecResult = LLRP_C1G2BlockPermalockQueryOpSpecResult_construct();
                ptC1G2BlockLockOpSpecResult = (T_RdpLlrpEpcBlcokLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2BlockPermalockQueryOpSpecResult->OpSpecID = ptC1G2BlockLockOpSpecResult->wOpSpecID;
                ptLLRPC1G2BlockPermalockQueryOpSpecResult->Result = ptC1G2BlockLockOpSpecResult->dwResult;
                LLRP_C1G2BlockPermalockQueryOpSpecResult_setLockState(ptLLRPC1G2BlockPermalockQueryOpSpecResult,
                        LLRP_u1v_construct(ptC1G2BlockLockOpSpecResult->wLockStateLen));
                memcpy(ptLLRPC1G2BlockPermalockQueryOpSpecResult->LockState.pValue,
                       ptC1G2BlockLockOpSpecResult->ucLockState,
                       ptC1G2BlockLockOpSpecResult->wLockStateLen);
                RdpReportDbgPrint("RDP:ptLLRP_tSC1G2BlockPermalockQueryOpSpecResult->LockState.nbit = %d\r\n",
                                  ptLLRPC1G2BlockPermalockQueryOpSpecResult->LockState.nBit);
                RdpReportDbgPrint("RDP:ptC1G2BlockLockOpSpecResult->wLockStateLen ===%d\r\n",
                                  ptC1G2BlockLockOpSpecResult->wLockStateLen);

                for (wLoop = 0; wLoop < ptLLRPC1G2BlockPermalockQueryOpSpecResult->LockState.nBit; wLoop++)
                {
                    RdpDbgPrintRaw("ptLLRP_tSC1G2BlockPermalockQueryOpSpecResult->LockState.pValue[%d]=%d\r\n",
                                      wLoop, ptLLRPC1G2BlockPermalockQueryOpSpecResult->LockState.pValue[wLoop]);
                }

                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2BlockPermalockQueryOpSpecResult);
                break;
            }
        case LLRP_C1G2_ALIENREADLOCK:
            {
                RdpReportDbgPrint("LLRP_C1G2_ALIENREADLOCK\r\n");
                ptLLRPC1G2BlockReadlockOpSpecResult = LLRP_C1G2BlockReadlockOpSpecResult_construct();
                ptC1G2AlienReadLockOpSpecResult = (T_RdpLlrpEpcAlienReadLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2BlockReadlockOpSpecResult->Result = ptC1G2AlienReadLockOpSpecResult->dwResult;
                ptLLRPC1G2BlockReadlockOpSpecResult->OpSpecID = ptC1G2AlienReadLockOpSpecResult->wOpSpecID;
                RdpReportDbgPrint("LLRP_C1G2_ALIENREADLOCK end\r\n");
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2BlockReadlockOpSpecResult);
                break;
            }
        case LLRP_C1G2WriteVerify:
            {
                RdpReportDbgPrint("LLRP_C1G2WriteVerify\r\n");
                ptLLRPC1G2WriteVerifyResult = (LLRP_tSC1G2WriteVerifyResult *)LLRP_C1G2WriteVerifyResult_construct();
                ptC1G2WriteVerifyResult = (T_RdpLlrpEpcWriteVerifyResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPC1G2WriteVerifyResult->OpSpecID = ptC1G2WriteVerifyResult->wOpSpecID;
                ptLLRPC1G2WriteVerifyResult->Result = ptC1G2WriteVerifyResult->dwResult;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPC1G2WriteVerifyResult);
                break;
            }
        case LLRP_ImpinjGetQTConfig:
            {
                RdpReportDbgPrint("LLRP_ImpinjGetQTConfig\r\n");

                ptLLRPImpinjGetQTConfigOpSpecResult = LLRP_ImpinjGetQTConfigOpSpecResult_construct();
                ptC1G2ImpinjGetQTConfigOpSpecResult = (T_RdpLlrpEpcImpinjGetQTOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPImpinjGetQTConfigOpSpecResult->OpSpecID = ptC1G2ImpinjGetQTConfigOpSpecResult->wOpSpecID;

                LLRP_ImpinjGetQTConfigOpSpecResult_setResult(ptLLRPImpinjGetQTConfigOpSpecResult,
                        (unsigned char)ptC1G2ImpinjGetQTConfigOpSpecResult->wResult);
                LLRP_ImpinjGetQTConfigOpSpecResult_setDataProfile(ptLLRPImpinjGetQTConfigOpSpecResult,
                        (unsigned char)ptC1G2ImpinjGetQTConfigOpSpecResult->wDataProfile);
                LLRP_ImpinjGetQTConfigOpSpecResult_setAccessRange(ptLLRPImpinjGetQTConfigOpSpecResult,
                        (unsigned char)ptC1G2ImpinjGetQTConfigOpSpecResult->wAccessRange);
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPImpinjGetQTConfigOpSpecResult);
                RdpReportDbgPrint("ptLLRP_tSImpinjGetQTConfigOpSpecResult->Result = %d\r\n",
                                  ptLLRPImpinjGetQTConfigOpSpecResult->Result);
                RdpReportDbgPrint("ptLLRP_tSImpinjGetQTConfigOpSpecResult->DataProfile = %d\r\n",
                                  ptLLRPImpinjGetQTConfigOpSpecResult->DataProfile);
                RdpReportDbgPrint("ptLLRP_tSImpinjGetQTConfigOpSpecResult->AccessRange= %d\r\n",
                                  ptLLRPImpinjGetQTConfigOpSpecResult->AccessRange);
                break;
            }
        case LLRP_ImpinjSetQTConfig:
            {
                RdpReportDbgPrint("LLRP_ImpinjSetQTConfig\r\n");
                ptLLRPImpinjSetQTConfigOpSpecResult = LLRP_ImpinjSetQTConfigOpSpecResult_construct();

                ptC1G2ImpinjSetQTConfigOpSpecResult = (T_RdpLlrpEpcImpinjSetQTOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPImpinjSetQTConfigOpSpecResult->OpSpecID = ptC1G2ImpinjSetQTConfigOpSpecResult->wOpSpecID;
                LLRP_ImpinjSetQTConfigOpSpecResult_setResult(ptLLRPImpinjSetQTConfigOpSpecResult,
                        ptC1G2ImpinjSetQTConfigOpSpecResult->wResult);


                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPImpinjSetQTConfigOpSpecResult);
                RdpReportDbgPrint("ptC1G2ImpinjSetQTConfigOpSpecResult->Result=%d",
                                  ptC1G2ImpinjSetQTConfigOpSpecResult->wResult);
                break;
            }
        case LLRP_ClientRequestOpSpec:
            {
                RdpReportDbgPrint("LLRP_ClientRequestOpSpec\r\n");
                break;
            }
        default:
            {
                RdpReportDbgPrint("the operateResult Err\r\n");
                break;
            }
        }
    }
}

/**************************************************************************
* �������ƣ�report_constructIso6bOpReport
* ��������������AO������EPC op�������
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpAccessTagReport ptAccessTagReport AO����
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
static void report_constructIso6bOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
        LLRP_tSTagReportData *ptTagReportData)
{
    unsigned int dwOpIndex = 0;
    unsigned short int wLoop = 0;
    unsigned int dwOpResultIndex = 0;
    T_RdpLlrpISO6BReadOpResult *ptISO6BReadOpSpecResult = NULL;
    T_RdpLlrpISO6BWriteOpResult *ptISO6BWriteOpSpecResult = NULL;
    T_RdpLlrpISO6BLockOpResult *ptISO6BLockOpSpecResult = NULL;
    T_RdpLlrpISO6BLockQueryOpResult *ptISO6BQueryLockOpSpecResult = NULL;

    LLRP_tSISO6BReadOpSpecResult *ptLLRP_tSISO6BReadOpSpecResult = NULL;
    LLRP_tSISO6BWriteOpSpecResult *ptLLRP_tSISO6BWriteOpSpecResult = NULL;
    LLRP_tSISO6BLockOpSpecResult *ptLLRP_tSISO6BLockOpSpecResult = NULL;
    LLRP_tSISO6BQueryLockOpSpecResult *ptLLRP_tSISO6BQueryLockOpSpecResult = NULL;

    /*����ÿ��op*/
    for (dwOpIndex = 0;dwOpIndex < ptAccessTagReport->dwOpNum;dwOpIndex++)
    {
        RdpDbgPrintRaw("dwOpIndex =%d\r\n", dwOpIndex);
        dwOpResultIndex = ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpResultAddr;
        switch (ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpType)
        {
        case LLRP_ISO6BOperationType_Read:
        case LLRP_ISO6BReadCustomUID:
            {
                RdpReportDbgPrint("LLRP_ISO6BOperationType_Read\r\n");
                ptLLRP_tSISO6BReadOpSpecResult = LLRP_ISO6BReadOpSpecResult_construct();
                ptISO6BReadOpSpecResult = (T_RdpLlrpISO6BReadOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRP_tSISO6BReadOpSpecResult->OpSpecID = ptISO6BReadOpSpecResult->wOpSpecID;
                ptLLRP_tSISO6BReadOpSpecResult->Result = ptISO6BReadOpSpecResult->ucResult;
                RdpReportDbgPrint("result = %d\r\n", ptLLRP_tSISO6BReadOpSpecResult->Result);
                LLRP_ISO6BReadOpSpecResult_setReadData(ptLLRP_tSISO6BReadOpSpecResult,
                                                       LLRP_u8v_construct(ptISO6BReadOpSpecResult->wDataLength));
                memcpy(ptLLRP_tSISO6BReadOpSpecResult->ReadData.pValue,
                       ptISO6BReadOpSpecResult->aucData,
                       ptISO6BReadOpSpecResult->wDataLength);

                for (wLoop = 0; wLoop < ptISO6BReadOpSpecResult->wDataLength; wLoop++)
                {
                    RdpDbgPrintRaw("ptLLRP_tSISO6BReadOpSpecResult->aucData[%d] = %d\r\n",
                                      wLoop, ptISO6BReadOpSpecResult->aucData[wLoop]);
                }

                RdpReportDbgPrint("LLRP_ISO6BOperationType_Read end\r\n");
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRP_tSISO6BReadOpSpecResult);
                break;
            }
        case LLRP_ISO6BOperationType_Write:
            {
                RdpReportDbgPrint("LLRP_ISO6BOperationType_Write\r\n");
                ptLLRP_tSISO6BWriteOpSpecResult = LLRP_ISO6BWriteOpSpecResult_construct();
                ptISO6BWriteOpSpecResult = (T_RdpLlrpISO6BWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRP_tSISO6BWriteOpSpecResult->OpSpecID = ptISO6BWriteOpSpecResult->wOpSpecID;
                ptLLRP_tSISO6BWriteOpSpecResult->Result = ptISO6BWriteOpSpecResult->ucResult;
                ptLLRP_tSISO6BWriteOpSpecResult->NumWordsWritten = ptISO6BWriteOpSpecResult->wNumWordsWritten;
                RdpReportDbgPrint("LLRP_ISO6BOperationType_Write end\r\n");
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRP_tSISO6BWriteOpSpecResult);
                break;
            }
        case LLRP_ISO6BOperationType_Lock:
            {
                RdpReportDbgPrint("LLRP_ISO6BOperationType_Lock\r\n");
                ptLLRP_tSISO6BLockOpSpecResult = LLRP_ISO6BLockOpSpecResult_construct();
                ptISO6BLockOpSpecResult = (T_RdpLlrpISO6BLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRP_tSISO6BLockOpSpecResult->OpSpecID = ptISO6BLockOpSpecResult->wOpSpecID;
                ptLLRP_tSISO6BLockOpSpecResult->Result = LLRP_u8v_construct(ptISO6BLockOpSpecResult->wDataLength);
                memcpy(ptLLRP_tSISO6BLockOpSpecResult->Result.pValue,
                       ptISO6BLockOpSpecResult->aucData,
                       ptISO6BLockOpSpecResult->wDataLength);

                for (wLoop = 0; wLoop < ptLLRP_tSISO6BLockOpSpecResult->Result.nValue; wLoop++)
                {
                    RdpDbgPrintRaw("ptLLRP_tSISO6BLockOpSpecResult->Result.pValue[%d]=%d\r\n", wLoop,
                                      ptLLRP_tSISO6BLockOpSpecResult->Result.pValue[wLoop]);
                }

                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRP_tSISO6BLockOpSpecResult);
                break;
            }
        case LLRP_ISO6BOperationType_QueryLock:
            {
                RdpReportDbgPrint("LLRP_ISO6BOperationType_QueryLock\r\n");
                ptLLRP_tSISO6BQueryLockOpSpecResult = LLRP_ISO6BQueryLockOpSpecResult_construct();
                ptISO6BQueryLockOpSpecResult = (T_RdpLlrpISO6BLockQueryOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRP_tSISO6BQueryLockOpSpecResult->OpSpecID = ptISO6BQueryLockOpSpecResult->wOpSpecID;
                LLRP_ISO6BQueryLockOpSpecResult_setResult(ptLLRP_tSISO6BQueryLockOpSpecResult,
                        LLRP_u8v_construct(ptISO6BQueryLockOpSpecResult->wDataLength));
                memcpy(ptLLRP_tSISO6BQueryLockOpSpecResult->Result.pValue,
                       ptISO6BQueryLockOpSpecResult->aucData,
                       ptISO6BQueryLockOpSpecResult->wDataLength);

                for (wLoop = 0; wLoop < ptLLRP_tSISO6BQueryLockOpSpecResult->Result.nValue; wLoop++)
                {
                    RdpDbgPrintRaw("ptLLRP_tSISO6BLockOpSpecResult->Result.pValue[%d]=%d\r\n", wLoop,
                                      ptLLRP_tSISO6BQueryLockOpSpecResult->Result.pValue[wLoop]);
                }

                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRP_tSISO6BQueryLockOpSpecResult);
                break;
            }
        case LLRP_ClientRequestOpSpec:
            {
                RdpReportDbgPrint("LLRP_ClientRequestOpSpec\r\n");
                break;
            }
        default:
            {
                RDP_EXCEPT(RDP_INVALID_STATE, 0, 0, 0, RDP_SEND_ID);
                RdpReportDbgPrint("the operateResult Err\r\n");
                break;
            }
        }
    }
}
/**************************************************************************
* �������ƣ�report_constructGBOpReport
* ��������������AO������GB op�������
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpAccessTagReport ptAccessTagReport AO����
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 02/21/14    V1.0      lzy        ����
**************************************************************************/
static void report_constructGBOpReport(T_RdpLlrpAccessTagReport *ptAccessTagReport,
                                        LLRP_tSTagReportData *ptTagReportData)
{
    unsigned int dwOpIndex = 0;
    unsigned short int wLoop = 0;
    unsigned int dwOpResultIndex = 0;
    LLRP_tSGBReadOpSpecResult *ptLLRPGBReadOpSpecResult = NULL;
    LLRP_tSGBWriteOpSpecResult *ptLLRPGBWriteOpSpecResult = NULL;
    LLRP_tSGBEraseOpSpecResult *ptLLRPGBEraseOpSpecResult = NULL;
    LLRP_tSGBLockOpSpecResult *ptLLRPGBLockOpSpecResult = NULL;
    LLRP_tSGBKillOpSpecResult *ptLLRPGBKillOpSpecResult = NULL;
    T_RdpLlrpGBReadOpResult *ptGBReadOpSpecResult = NULL;
    T_RdpLlrpGBWriteOpResult *ptGBWriteOpSpecResult = NULL;
    T_RdpLlrpGBEraseOpResult *ptGBEraseOpSpecResult = NULL;
    T_RdpLlrpGBLockOpResult *ptGBLockOpSpecResult = NULL;
    T_RdpLlrpGBKillOpResult *ptGBKillOpSpecResult = NULL;
    
    /*����ÿ��op*/
    for (dwOpIndex = 0;dwOpIndex < ptAccessTagReport->dwOpNum;dwOpIndex++)
    {
        dwOpResultIndex = ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpResultAddr;
        RdpReportDbgPrint("dwOpIndex =%d,dwOpResultIndex=%d\r\n", dwOpIndex, dwOpResultIndex);
        switch (ptAccessTagReport->tListofOpResultCtrl[dwOpIndex].wOpType)
        {
        case LLRP_GBOperationType_Read:
        case LLRP_GBReadCustomUID:
            {
                RdpReportDbgPrint("LLRP_GBOperationType_Read\r\n");
                ptLLRPGBReadOpSpecResult = LLRP_GBReadOpSpecResult_construct();
                ptGBReadOpSpecResult = (T_RdpLlrpGBReadOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPGBReadOpSpecResult->OpSpecID = ptGBReadOpSpecResult->wOpSpecID;
                ptLLRPGBReadOpSpecResult->Result = ptGBReadOpSpecResult->dwResult;
                RdpReportDbgPrint("result = %d,wDataLength=%d\r\n", ptLLRPGBReadOpSpecResult->Result, ptGBReadOpSpecResult->wDataLength);
                LLRP_GBReadOpSpecResult_setReadData(ptLLRPGBReadOpSpecResult,
                                                      LLRP_u16v_construct(ptGBReadOpSpecResult->wDataLength));

                if (0 != ptGBReadOpSpecResult->wDataLength)
                {
                    memcpy(ptLLRPGBReadOpSpecResult->ReadData.pValue,
                           ptGBReadOpSpecResult->aucData,
                           ptGBReadOpSpecResult->wDataLength * sizeof(unsigned short int));
                }

                for (wLoop = 0; wLoop < ptGBReadOpSpecResult->wDataLength * sizeof(unsigned short int); wLoop++)
                {
                    RdpDbgPrintRaw("ptGBReadOpSpecResult->aucData[%d] = %d\r\n",
                                      wLoop, ptGBReadOpSpecResult->aucData[wLoop]);
                }

                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPGBReadOpSpecResult);
                break;
            }
        case LLRP_GBOperationType_Write:
            {
                RdpReportDbgPrint("LLRP_GBOperationType_Write\r\n");
                ptLLRPGBWriteOpSpecResult = (LLRP_tSGBWriteOpSpecResult *)LLRP_GBWriteOpSpecResult_construct();
                ptGBWriteOpSpecResult = (T_RdpLlrpGBWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPGBWriteOpSpecResult->OpSpecID = ptGBWriteOpSpecResult->wOpSpecID;
                ptLLRPGBWriteOpSpecResult->Result = ptGBWriteOpSpecResult->dwResult;
                ptLLRPGBWriteOpSpecResult->NumWordsWritten = ptGBWriteOpSpecResult->wNumWordsWritten;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPGBWriteOpSpecResult);
                break;
            }
	  case LLRP_GBOperationType_Erase:
            {
                RdpReportDbgPrint("LLRP_GBOperationType_Erase\r\n");
                ptLLRPGBEraseOpSpecResult = (LLRP_tSGBEraseOpSpecResult *)LLRP_GBEraseOpSpecResult_construct();
                ptGBEraseOpSpecResult = (T_RdpLlrpGBEraseOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPGBEraseOpSpecResult->OpSpecID = ptGBEraseOpSpecResult->wOpSpecID;
                ptLLRPGBEraseOpSpecResult->Result = ptGBEraseOpSpecResult->dwResult;
                ptLLRPGBEraseOpSpecResult->NumWordsErase = ptGBEraseOpSpecResult->wNumWordsErase;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPGBEraseOpSpecResult);
                break;
            }
        case LLRP_GBOperationType_Lock:
            {
                RdpReportDbgPrint("LLRP_GBOperationType_Lock\r\n");
                ptLLRPGBLockOpSpecResult = (LLRP_tSGBLockOpSpecResult *)LLRP_GBLockOpSpecResult_construct();
                ptGBLockOpSpecResult = (T_RdpLlrpGBLockOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPGBLockOpSpecResult->OpSpecID = ptGBLockOpSpecResult->wOpSpecID;
                ptLLRPGBLockOpSpecResult->Result = ptGBLockOpSpecResult->dwResult;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPGBLockOpSpecResult);
                break;
            }
        case LLRP_GBOperationType_Kill:
            {
                RdpReportDbgPrint("LLRP_GBOperationType_Kill\r\n");
                ptLLRPGBKillOpSpecResult = (LLRP_tSGBKillOpSpecResult *)LLRP_GBKillOpSpecResult_construct();
                ptGBKillOpSpecResult = (T_RdpLlrpGBKillOpResult *) & ptAccessTagReport->aucData[dwOpResultIndex];
                ptLLRPGBKillOpSpecResult->OpSpecID = ptGBKillOpSpecResult->wOpSpecID;
                ptLLRPGBKillOpSpecResult->Result = ptGBKillOpSpecResult->dwResult;
                LLRP_TagReportData_addAccessCommandOpSpecResult(ptTagReportData,
                        (LLRP_tSParameter *)ptLLRPGBKillOpSpecResult);
                break;
            }
        case LLRP_ClientRequestOpSpec:
            {
                RdpReportDbgPrint("LLRP_ClientRequestOpSpec\r\n");
                break;
            }
        default:
            {
                RdpReportDbgPrint("the operateResult Err\r\n");
                break;
            }
        }
    }
}
/**************************************************************************
* �������ƣ�report_constructAccessReport
* ��������������AO����
* ���ʵı���
* �޸ĵı���
* ���������LLRP_tSTagReportData *ptTagReportData ת����llrp�ṹ
                              T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl  �ڲ�����ṹ
 
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
void report_constructAccessReport(LLRP_tSTagReportData *ptTagReportData, T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl)
{
    char cRssi = 0;
    unsigned int dwTestIndex = 0;
    T_RdpLlrpAccessTagReport *ptTempAccessTagReportData = NULL;
    LLRP_tSEPCData *ptEPCData = NULL;
    LLRP_tSUIDData *ptUIDData = NULL;
    LLRP_tSGBData *ptGBData = NULL;
    LLRP_tSROSpecID *ptROSpecID = NULL;
    LLRP_tSSpecIndex *ptSpecIndex = NULL;
    LLRP_tSInventoryParameterSpecID *ptInventoryParameterSpecID = NULL;
    LLRP_tSAntennaID *ptAntennaID = NULL;
    LLRP_tSPeakRSSI *ptPeakRSSI = NULL;
    LLRP_tSChannelIndex *ptChannelIndex = NULL;
    LLRP_tSFirstSeenTimestampUTC *ptFirstSeenTimestampUTC = NULL;
    LLRP_tSLastSeenTimestampUTC *ptLastSeenTimestampUTC = NULL;
    LLRP_tSTagSeenCount *ptTagSeenCount = NULL;
    LLRP_tSAccessSpecID *ptAccessSpecID = NULL;
    ptTempAccessTagReportData = &ptSignalReportCtrl->tRdpLlrpTagReport.tAccessTagReport;

    RdpReportDbgPrint("AccessReportData  eProtocolID=%d\r\n", ptTempAccessTagReportData->eProtocolID);
    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == ptTempAccessTagReportData->eProtocolID)
    {
        RdpReportDbgPrint("ptTempAccessTagReportData->tEPCParameter\r\n");
        /*ֱ�����ɱ���*/
        ptEPCData = LLRP_EPCData_construct();
        LLRP_EPCData_setEPC(ptEPCData,
                            LLRP_u1v_construct(ptTempAccessTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) * 8));
        memcpy(ptEPCData->EPC.pValue,
               ptTempAccessTagReportData->tID.tEpcID.awTagData,
               ptTempAccessTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int));
        RdpReportDbgPrint("Epc tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReportData->tID.tEpcID.wTagDataLen * sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptEPCData->EPC.pValue[dwTestIndex]);

        }
        RdpReportDbgPrint("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptEPCData);
    }
    else if (LLRP_AirProtocols_ISO18000_6B == ptTempAccessTagReportData->eProtocolID)
    {
        ptUIDData = LLRP_UIDData_construct();
        LLRP_UIDData_setUID(ptUIDData,
                            LLRP_u8v_construct(ptTempAccessTagReportData->tID.tISOID.wTagDataLen));
        memcpy(ptUIDData->UID.pValue,
               ptTempAccessTagReportData->tID.tISOID.aucTagData,
               ptTempAccessTagReportData->tID.tISOID.wTagDataLen);
        RdpReportDbgPrint("ISO tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReportData->tID.tISOID.wTagDataLen;dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptUIDData->UID.pValue[dwTestIndex]);

        }
        RdpReportDbgPrint("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptUIDData);
    }
    else if (LLRP_AirProtocols_GB == ptTempAccessTagReportData->eProtocolID)
    {
        /*ֱ�����ɱ���*/
        ptGBData = LLRP_GBData_construct();
        LLRP_GBData_setGBID(ptGBData,
                            LLRP_u1v_construct(ptTempAccessTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int) * 8));
        memcpy(ptGBData->GBID.pValue,
               ptTempAccessTagReportData->tID.tGBID.awTagData,
               ptTempAccessTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int));
        RdpReportDbgPrint("GB tag:");
        for (dwTestIndex = 0; dwTestIndex < ptTempAccessTagReportData->tID.tGBID.wTagDataLen * sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("%02x  ", ptGBData->GBID.pValue[dwTestIndex]);

        }
        RdpReportDbgPrint("\r\n");
        LLRP_TagReportData_setEPCParameter(ptTagReportData, (LLRP_tSParameter *)ptGBData);
    }

    if (LLRP_DEFAULT_REPORT_WORD32 != ptTempAccessTagReportData->dwAccessSpecID)
    {
        /*����Access����*/
        ptAccessSpecID = LLRP_AccessSpecID_construct();
        ptAccessSpecID->AccessSpecID = ptTempAccessTagReportData->dwAccessSpecID;
        LLRP_TagReportData_setAccessSpecID(ptTagReportData, ptAccessSpecID);
    }

    if (LLRP_DEFAULT_REPORT_WORD32 != ptTempAccessTagReportData->dwROSpecID)
    {
        /*����RO��������*/
        ptROSpecID = LLRP_ROSpecID_construct();
        ptROSpecID->ROSpecID = ptTempAccessTagReportData->dwROSpecID;
        LLRP_TagReportData_setROSpecID(ptTagReportData, ptROSpecID);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wSpecIndex)
    {
        /*����RO��������*/
        ptSpecIndex = LLRP_SpecIndex_construct();
        ptSpecIndex->SpecIndex = ptTempAccessTagReportData->wSpecIndex;
        LLRP_TagReportData_setSpecIndex(ptTagReportData, ptSpecIndex);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wAntennaID)
    {
        /*�������ߵĽṹ*/
        ptAntennaID = LLRP_AntennaID_construct();
        ptAntennaID->AntennaID = ptTempAccessTagReportData->wAntennaID;
        LLRP_TagReportData_setAntennaID(ptTagReportData, ptAntennaID);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wChannelIndex)
    {
        /*�������ߵĽṹ*/
        ptChannelIndex = LLRP_ChannelIndex_construct();
        ptChannelIndex->ChannelIndex = ptTempAccessTagReportData->wChannelIndex;
        LLRP_TagReportData_setChannelIndex(ptTagReportData, ptChannelIndex);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wInventoryParameterSpecID)
    {
        /*����InventID����*/
        ptInventoryParameterSpecID = LLRP_InventoryParameterSpecID_construct();
        ptInventoryParameterSpecID->InventoryParameterSpecID = ptTempAccessTagReportData->wInventoryParameterSpecID;
        LLRP_TagReportData_setInventoryParameterSpecID(ptTagReportData,
                ptInventoryParameterSpecID);
    }

    if (LLRP_DEFAULT_REPORT_WORD64 != ptTempAccessTagReportData->lwFirstSeenTimestampUTC)
    {
        ptFirstSeenTimestampUTC = LLRP_FirstSeenTimestampUTC_construct();
        ptFirstSeenTimestampUTC->Microseconds = ptTempAccessTagReportData->lwFirstSeenTimestampUTC;
        LLRP_TagReportData_setFirstSeenTimestampUTC(ptTagReportData,
                ptFirstSeenTimestampUTC);
    }

    if (LLRP_DEFAULT_REPORT_WORD64 != ptTempAccessTagReportData->lwLastSeenTimestampUTC)
    {
        ptLastSeenTimestampUTC = LLRP_LastSeenTimestampUTC_construct();
        ptLastSeenTimestampUTC->Microseconds = ptTempAccessTagReportData->lwLastSeenTimestampUTC;
        LLRP_TagReportData_setLastSeenTimestampUTC(ptTagReportData,
                ptLastSeenTimestampUTC);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wPeakRSSI)
    {
        ptPeakRSSI = LLRP_PeakRSSI_construct();
        /*��Ҫת��rssi*/
        cRssi = (char)10 * log10(ptTempAccessTagReportData->wPeakRSSI) -REPORT_RSSI_CONSTANT_VALUE;
        ptPeakRSSI->PeakRSSI = cRssi;
        RdpReportDbgPrint("ptTempAccessTagReportData->wPeakRSSI=%d,cRssi=%d\r\n",
                          ptTempAccessTagReportData->wPeakRSSI, cRssi);
        LLRP_TagReportData_setPeakRSSI(ptTagReportData, ptPeakRSSI);
    }

    if (LLRP_DEFAULT_REPORT_WORD16 != ptTempAccessTagReportData->wTagCount)
    {

        ptTagSeenCount = LLRP_TagSeenCount_construct();
        ptTagSeenCount->TagCount = ptTempAccessTagReportData->wTagCount;
        LLRP_TagReportData_setTagSeenCount(ptTagReportData, ptTagSeenCount);
    }

    /*����op*/
    if (LLRP_AirProtocols_EPCGlobalClass1Gen2 == ptTempAccessTagReportData->eProtocolID)
    {
        RdpReportDbgPrint("report_constructEpcOpReport\r\n");
        report_constructEpcOpReport(ptTempAccessTagReportData, ptTagReportData);
    }
    else if (LLRP_AirProtocols_ISO18000_6B == ptTempAccessTagReportData->eProtocolID)
    {
        RdpReportDbgPrint("report_constructIso6bOpReport\r\n");
        report_constructIso6bOpReport(ptTempAccessTagReportData, ptTagReportData);
    }
    else if (LLRP_AirProtocols_GB == ptTempAccessTagReportData->eProtocolID)
    {
        RdpReportDbgPrint("report_constructGBOpReport\r\n");
        report_constructGBOpReport(ptTempAccessTagReportData, ptTagReportData);
    }
    return ;

}

/**************************************************************************
* �������ƣ�report_constructSingleTagReport
* �������������챨��
* ���ʵı���
* �޸ĵı���
* ���������T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl  �ڲ�����ṹ
 
* �����������
* �� �� ֵ��LLRP_tSTagReportData * llrp�ṹ�ı���ָ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/05/12    V1.0      lzy        ����
**************************************************************************/
LLRP_tSTagReportData *report_constructSingleTagReport(T_RdpLlrpSingleReportCtrl *ptSignalReportCtrl)
{

    LLRP_tSTagReportData *ptTagReportData = NULL;
    ptTagReportData = LLRP_TagReportData_construct();

    switch (ptSignalReportCtrl->ucReportType)
    {
    case ROSPEC_REPORT_DATA:
        {
            /*ָ��Ҫ�ϱ����������*/
            RdpReportDbgPrint("construct ROReportData\r\n");
            report_constructRoReport(ptTagReportData, ptSignalReportCtrl);
            RdpReportDbgPrint("construct ROReportData end\r\n");
            break;
        }
    case ACCESSSPEC_REPORT_DATA:
        {

            RdpReportDbgPrint("construct AccessReportData  \r\n");
            report_constructAccessReport(ptTagReportData, ptSignalReportCtrl);
            RdpReportDbgPrint("construct AccessReportData end \r\n");
            break;
        }
        /*����֪ͨ��Ϣֱ�ӷ��ͳ�ȥ*/
    default:
        {
            RdpReportDbgPrint("report_constructSingleTagReport not surport this report type ptSignalReportCtrl->ucReportType=%d\r\n",
                              ptSignalReportCtrl->ucReportType);
            break;
        }
    }

    return ptTagReportData;
}
/**************************************************************************
* �������ƣ�report_sendReportMsgAndFree
* �������������̨���ͱ���
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptReceiveMsg reportģ���յ�����Ϣ
*                             unsigned int dwMessageID Ӧ�÷��͵�MessageID
*                             LLRP_tSMessage *ptMessage ������Ϣָ��
* �����������
* �� �� ֵ��0 �ɹ� ����ʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 03/06/12    V1.0      lzy        ����
**************************************************************************/
unsigned int report_sendReportMsgAndFree(T_Msg *ptReceiveMsg, unsigned int dwMessageID , LLRP_tSMessage *ptMessage)
{
    unsigned int dwRet = RDP_SUCCESS;
    T_Msg *ptSendMsg = NULL;
    LLRP_tSErrorDetails pError;
    ptSendMsg = (T_Msg *)g_aucReportSendDataBuf;

    RdpReportDbgPrint("report_sendReportMsgAndFree dwMessageID=%d\r\n", dwMessageID);

    if (NULL == ptMessage)
    {
        RdpReportDbgPrint("no report\r\n");
        return RDP_FAILED;
    }

    ptMessage->MessageID = dwMessageID;
    ptSendMsg->ucLinkIndex = ptReceiveMsg->ucLinkIndex;
    ptSendMsg->ucLinkType = ptReceiveMsg->ucLinkType;
    ptSendMsg->mtype = ptReceiveMsg->mtype;
    ptSendMsg->wMsgSequence = ptReceiveMsg->wMsgSequence;
    ptSendMsg->wSenderTno = USP_RDP_REPORT_TNO;
    ptSendMsg->wReceiverTno = ptReceiveMsg->wSenderTno;
    if (EV_READER_EVENT_NOTIFICATION == ptReceiveMsg->wMsgID)
    {
        /*�Ķ����¼�֪ͨ�����ж������棬��Ȼ������������ lzy*/
        ptSendMsg->wMsgID = EV_USP_COMM_SendData;
    }
    else
    {
        ptSendMsg->wMsgID = EV_USP_COMM_SendReportData;
    }
    ptSendMsg->wMsgLen = READER_MAX_SEND_MSG_LEN - sizeof(T_Msg);
    RdpReportDbgPrint("ptSendMsg->ucLinkIndex = %d\r\n", ptSendMsg->ucLinkIndex);
    RdpReportDbgPrint("ptSendMsg->wReceiverTno = %d\r\n", ptSendMsg->wReceiverTno);
    RdpReportDbgPrint("ptSendMsg->wMsgID = 0x%x\r\n", ptSendMsg->wMsgID);
    RdpReportDbgPrint("ptMessage->MessageID = %d\r\n", ptMessage->MessageID);
    /*�������ⷢ��*/
    dwRet = usp_ltk_encodeMessage((unsigned char *)(ptSendMsg + 1), &ptSendMsg->wMsgLen, ptMessage, &pError);
    RdpReportDbgPrint("dwRet = %d\r\n", dwRet);
    if (RDP_SUCCESS != dwRet)
    {
        RdpReportDbgPrint("usp_ltk_encodeMessage Error ptSendMsg->dwRet=%d\r\n" , dwRet);
        usp_ltk_freeMessage(ptMessage);
        RDP_EXCEPT(RDP_ENCODE_MESSAGE_ERROR, 0, 0, 0, RDP_SEND_ID);
        return dwRet;
    }
    if (RDP_SUCCESS > RDP_SEND_MSG(push_socket_comm, ptSendMsg))
    {
        RdpReportDbgPrint("MsgSend Error1_Repor\r\n");
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, 0, 0, 0, RDP_SEND_ID);
        usp_ltk_freeMessage(ptMessage);
        return dwRet;
    }

    RdpReportDbgPrint("MsgSend Success_Report in  report_sendReportMsgAndFree\r\n");
    usp_ltk_freeMessage(ptMessage);
    return dwRet;

}
