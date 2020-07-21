/***************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�ufhllrpEpc.c
* �ļ���ʶ��
* ����ժҪ��LLRPEpcЭ��ģ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2012.05.17 
**********************************************************************/


/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include"rdppub.h"
#include"rdpapppub.h" 
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

unsigned int g_dwLlrpTagAccessErrTimes = LLRP_INIT_ACCESS_ERROR_TIMES;
T_RdpLlrpClientRequestOpSet g_tClientRequestOpSet;
unsigned int g_bLlrpEpcAccessFlag = FALSE;
/*���������Ϣ*/
extern unsigned char g_aucRdpLlrpSndMsg[RDP_MAX_LEN_MSG];
extern unsigned char g_aucRdpLlrpRcvMsg[RDP_MAX_LEN_MSG];
extern T_RdpLlrpPwdSys g_tRdpPwdSys;
extern T_RdpConfig g_tRdpConfig;
extern T_RdpLlrpReaderDispatch g_tRdpLlrpReaderDispatch;
extern unsigned char g_ucLocalReportDataBuffer[LLRP_MAX_REPORT_BUFFER];
extern unsigned short int g_wMsgSequence;
extern unsigned short int g_wInnerMsgSequence;
extern T_RdpLlrpDynamicAccess g_tRdpLlrpDynamicAccessLinkHead[MAX_LINK_NUM];

extern void *req_socket_rcp;
extern void *pair_socket_rule;
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
* �������ƣ�llrp_setEpcAccessFlag
* ��������������EpcAcces���ʱ�ʶ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ������Կ���ʱ�ʶ
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_setEpcAccessFlag()
{
    g_bLlrpEpcAccessFlag = TRUE;
}

/**************************************************************************
* �������ƣ�llrp_clearEpcAccessFlag
* �������������EpcAcces���ʱ�ʶ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ���ص���Կ
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_clearEpcAccessFlag()
{
    g_bLlrpEpcAccessFlag = FALSE;
}
/**************************************************************************
* �������ƣ�llrp_getPassword
* ������������ȡ��Կ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ���ص���Կ
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_getPassword(void * Para , unsigned int *pdwAccessPassword)
{
    T_RdpLlrpAccess *ptMatchAccess = NULL;
    ptMatchAccess = (T_RdpLlrpAccess *)Para;
    *pdwAccessPassword = ptMatchAccess->tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet.tRdpLlrpOPCtr[0].dwAccessPassword;
    return 0;
}
/**************************************************************************
* �������ƣ�llrp_frequencyTransfer
* ����������Ƶ�ʱ�����ת��
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int whopid, 
                             ��Ƶ����
* ���������T_USPUHFFreqHopChannel *ptFreqChannel
* �� �� ֵ��  ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_frequencyTransfer(unsigned short int whopid, T_USPUHFFreqHopChannel *ptFreqChannel)

{
    unsigned char ucIndex = 0;
   RdpLlrpDbgPrint("(LLRP) llrp_frequencyTransfer \r\n");
    if (whopid == 0)
    {
        ptFreqChannel->wFreqChannelNum = 20;
    }
    else
    {
        ptFreqChannel->wFreqChannelNum = 50;
    }

    for (ucIndex = 0; ucIndex < ptFreqChannel->wFreqChannelNum; ucIndex++)
    {
        ptFreqChannel->wFreqChannel[ucIndex] = ucIndex;

    }

}
/**************************************************************************
* �������ƣ�llrp_assembleEpcInvProtocolPara
* ���������������������е�Э�����
* ���ʵı���
* �޸ĵı���
* ���������unsigned char ucAntennaIndex
                             ����ID
                            T_RdpLlrpProtocolInvSet  *ptTempProtocolInvSet
                             ��������µ�EPCЭ��Ĳ���
* ���������T_USPUHFEpcInventory *ptUSPUHFEpcInventory
                            ���� ������
* �� �� ֵ��   0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_assembleEpcInvProtocolPara( unsigned char ucAntennaIndex,
                                      T_RdpLlrpProtocolInvSet*ptTempProtocolPara,
                                      T_USPUHFEpcInventory *ptUSPUHFEpcInventory)
{
    unsigned short int wloop = 0;
    T_USPRdpLlrpAntennaConfiguration *ptAntConfig = NULL;
    T_USPRdpLlrpAntennaExtConfiguration *ptAntExtConfig = NULL;
    T_USPRdpLlrpC1G2FilterList *ptC1G2FilterList = NULL;
    T_USPUHFEpcInventoryMaskList *ptEpcInventoryMaskList = NULL;
    ptAntConfig = &g_tRdpConfig.tUSPRdpLlrpAntennaConfiguration[ucAntennaIndex - 1];
    ptAntExtConfig = &g_tRdpConfig.trAntennaExtConfiguration[ucAntennaIndex - 1];
    ptEpcInventoryMaskList = &ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList;
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpC1G2InvCtrl.bC1G2Filterctrl == TRUE)
    {
        ptC1G2FilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter;
        ptEpcInventoryMaskList->wMaskNum = ptC1G2FilterList->wFilterNum;
        for (wloop = 0;wloop < ptEpcInventoryMaskList->wMaskNum;wloop++)
        {

            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].eSelectAction
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpC1G2TagInventoryStateUnawareFilterAction.eC1G2StateUnawareAction;
            memcpy(ptEpcInventoryMaskList->tEpcInventoryMask[wloop].ucMaskData, \
                   ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.ucMaskData, \
                   sizeof(ptEpcInventoryMaskList->tEpcInventoryMask[wloop].ucMaskData));
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMaskBitPointer \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMaskBitPointer;
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMaskLength \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMaskLength;
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMemBank \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMemBank;
        }
    }
    else
    {
        ptC1G2FilterList = &ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration \
                           .tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter;
        ptEpcInventoryMaskList->wMaskNum = ptC1G2FilterList->wFilterNum;

        for (wloop = 0;wloop < ptEpcInventoryMaskList->wMaskNum;wloop++)
        {
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].eSelectAction
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpC1G2TagInventoryStateUnawareFilterAction.eC1G2StateUnawareAction;
            memcpy(ptEpcInventoryMaskList->tEpcInventoryMask[wloop].ucMaskData, \
                   ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.ucMaskData, \
                   sizeof(ptEpcInventoryMaskList->tEpcInventoryMask[wloop].ucMaskData));
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMaskBitPointer
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMaskBitPointer;
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMaskLength
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMaskLength;
            ptEpcInventoryMaskList->tEpcInventoryMask[wloop].wMemBank
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[wloop].tUSPRdpLlrpEPCTargetTagMask.wMemBank;
        }
    }
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpC1G2InvCtrl.bC1G2RFCtrl == TRUE)
    {
        ptUSPUHFEpcInventory->tEpcInventoryPara.wModeindex
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2RFControl.wModeIndex;
    }
    else
    {

        ptUSPUHFEpcInventory->tEpcInventoryPara.wModeindex
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings \
          .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2RFControl.wModeIndex;
    }

    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpC1G2InvCtrl.bC1G2SingulationCtrl == TRUE)
    {
        ptUSPUHFEpcInventory->tEpcInventoryPara.wEstimatedTagNum
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.wTagPopulation;
        ptUSPUHFEpcInventory->tEpcInventoryPara.wSession
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.ucSession;
    }
    else
    {
        ptUSPUHFEpcInventory->tEpcInventoryPara.wEstimatedTagNum
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings \
          .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.wTagPopulation;
        ptUSPUHFEpcInventory->tEpcInventoryPara.wSession
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings \
          .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.ucSession;
    }
}
/**************************************************************************
* �������ƣ�llrp_assembleEpcInvPara
* ���������������������·���RCP
* ���ʵı���
* �޸ĵı���
* ���������unsigned char ucAntennaIndex
                             ����ID
                            T_RdpLlrpProtocolInvSet  *ptTempProtocolInvSet
                             ��������µ�EPCЭ��Ĳ���
* ���������T_USPUHFEpcInventory *ptUSPUHFEpcInventory
                            ���� ������
* �� �� ֵ��   0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_assembleEpcInvPara( unsigned char ucAntennaIndex,
                              T_RdpLlrpProtocolInvSet *ptTempProtocolPara,
                              T_USPUHFEpcInventory *ptUSPUHFEpcInventory)
{
    unsigned int dwloop = 0;
    T_USPRdpLlrpAntennaConfiguration *ptAntConfig = NULL;
    T_USPRdpLlrpAntennaExtConfiguration *ptAntExtConfig = NULL;
    T_USPRdpLlrpC1G2FilterList *ptC1G2FilterList = NULL;
    T_USPUHFEpcInventoryMaskList *ptEpcInventoryMaskList = NULL;
    ptUSPUHFEpcInventory->ucAntennaID = ucAntennaIndex;
    ptUSPUHFEpcInventory->ucProtoType = ptTempProtocolPara->eProtocolID;
    ptAntConfig = &g_tRdpConfig.tUSPRdpLlrpAntennaConfiguration[ucAntennaIndex - 1];
    ptAntExtConfig = &g_tRdpConfig.trAntennaExtConfiguration[ucAntennaIndex - 1];
    /*�������ü̳� ֱ�Ӹ�ϵͳĬ�ϵ�ֵ*/
    ptEpcInventoryMaskList = &ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList;
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.bIsDefaultCfgFlag == TRUE)
    {

        ptUSPUHFEpcInventory->tEpcInventoryPara.wEstimatedTagNum
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.wTagPopulation;

        ptUSPUHFEpcInventory->tEpcInventoryPara.wModeindex
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2RFControl.wModeIndex;
        ptUSPUHFEpcInventory->tEpcInventoryPara.wSession
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.ucSession;
        ptEpcInventoryMaskList->wMaskNum
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter.wFilterNum;

        ptC1G2FilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter;

        for (dwloop = 0;dwloop < ptEpcInventoryMaskList->wMaskNum;dwloop++)
        {

            ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].eSelectAction
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpC1G2TagInventoryStateUnawareFilterAction.eC1G2StateUnawareAction;
            memcpy(ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList.tEpcInventoryMask[dwloop].ucMaskData, \
                   ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.ucMaskData, \
                   sizeof(ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList.tEpcInventoryMask[dwloop].ucMaskData));
            ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMaskBitPointer \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMaskBitPointer;
            ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMaskLength \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMaskLength;
            ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMemBank \
            = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMemBank;
        }
        memcpy (ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannel,
                sizeof(ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.wFreqHopMode;
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.wHopTimer;
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFixFreqChannel;
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFreqTableID;
        ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFreqWorkType;
        ptUSPUHFEpcInventory->tInventoryRfPara.wTransmitPower
        = ptAntExtConfig->tTransmitPower.wTransmitPower6C;
    }
    else
    {
        if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.bRFTransmitterCtrl == TRUE)
        {
            memcpy (ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    sizeof(ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.wFreqHopMode;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.wHopTimer;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFixFreqChannel;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFreqTableID;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFreqWorkType;
            ptUSPUHFEpcInventory->tInventoryRfPara.wTransmitPower
            = ptAntExtConfig->tTransmitPower.wTransmitPower6C;
        }
        else
        {
            memcpy (ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    sizeof(ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode = LLRP_FreqHopType_Sequence;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer = 1;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfg6C.wFixFreqChannel;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
            = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpRFTransmitterSettings.wHopTableID;
            ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType = TRUE;
            ptUSPUHFEpcInventory->tInventoryRfPara.wTransmitPower
            = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpRFTransmitterSettings.wTransmitPower;
            /*������Ƶ Э���ʼ����Ƶ��*/
            llrp_frequencyTransfer(ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.wFreqTableID,
                                   &ptUSPUHFEpcInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel);
        }

        /*���Э��̳У�ֱ����ϵͳĬ��ֵ*/
        if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpC1G2InvCtrl.bIsDefaultCfgFlag == TRUE)
        {
            ptUSPUHFEpcInventory->tEpcInventoryPara.wEstimatedTagNum
            = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.wTagPopulation;
            ptUSPUHFEpcInventory->tEpcInventoryPara.wModeindex
            = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2RFControl.wModeIndex;
            ptUSPUHFEpcInventory->tEpcInventoryPara.wSession
            = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2SingulationControl.ucSession;
            ptEpcInventoryMaskList->wMaskNum
            = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter.wFilterNum;

            ptC1G2FilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpC1G2InventoryCommand.tUSPRdpLlrpC1G2Filter;
            for (dwloop = 0;dwloop < ptEpcInventoryMaskList->wMaskNum;dwloop++)
            {
                ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].eSelectAction
                = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpC1G2TagInventoryStateUnawareFilterAction.eC1G2StateUnawareAction;
                memcpy(ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList.tEpcInventoryMask[dwloop].ucMaskData, \
                       ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.ucMaskData, \
                       sizeof(ptUSPUHFEpcInventory->tEpcInventoryPara.tEpcInventoryMaskList.tEpcInventoryMask[dwloop].ucMaskData));
                ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMaskBitPointer \
                = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMaskBitPointer;
                ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMaskLength \
                = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMaskLength;
                ptEpcInventoryMaskList->tEpcInventoryMask[dwloop].wMemBank \
                = ptC1G2FilterList->atUSPRdpLlrpC1G2Filter[dwloop].tUSPRdpLlrpEPCTargetTagMask.wMemBank;
            }

        }
        else
        {
            llrp_assembleEpcInvProtocolPara( ucAntennaIndex, ptTempProtocolPara, ptUSPUHFEpcInventory);

        }
    }
}
/**************************************************************************
* �������ƣ�llrp_EpcPwdIndexRead
* ����������epc��Կ�����Ķ�ȡ����ȡ��Կ
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��   0 �ɹ� 
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_getEpcAccessPassword(T_USPUHFEpcTagId *ptEpcTagId, unsigned short int wAntID, unsigned int *pdwAccessPassword)
{
    T_Msg tSyncMsg;
    T_Msg *ptSyncMsg[] = {&tSyncMsg, NULL};
    int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned int dwTestIndex = 0;
    unsigned short int wLoop = 0;
    unsigned short int wLoop1 = 0;
    unsigned char ucRevBuf[8192] = {0};
    unsigned char ucSndBuf[1024] = {0};
    T_USPUHFEpcRead *ptEpcRead = NULL;
    T_USPUHFEpcReadRes *ptEpcReadRsp = NULL;
    /*���û���κ���Կ ����������Կ���� ������Ĭ�ϵ���Կ��ʽ*/
    for (dwIndex = 0;dwIndex < g_tRdpPwdSys.dwPwdIndexNum;dwIndex++)
    {
        /*impinj��ǩ��Epc�� Ҳ��������ϵ�Epc�������*/
        if (g_tRdpPwdSys.atPwdIndexPara[dwIndex].dwMembank == 1)
        {
            RdpLlrpDbgPrint("(LLRP)pwdIndex is epc membank need not read \r\n");
            memcpy(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wPwdIndexBuffer,
                   &ptEpcTagId->aucTagData[(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordPointer - 2)*sizeof(unsigned short int)],
                   g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordCount*sizeof(unsigned short int));
            continue;
        }
        T_Msg *ptSndMsg = (T_Msg *)ucSndBuf;
        T_Msg *ptRcvMsg = (T_Msg *)ucRevBuf;
        ptSndMsg->mtype = 1;
        ptSndMsg->wMsgID = EV_USP_UHF_EPC_READ;
        ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wDstTno = USP_UHF_TNO;
        ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wReceiverTno = USP_UHF_TNO;
        ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wInnerMsgSequence);
        ptEpcRead = (T_USPUHFEpcRead *)(ptSndMsg + 1);
        ptEpcRead->dwAntIndex = wAntID;
        ptEpcRead->tEpcReadPara.ucMB = g_tRdpPwdSys.atPwdIndexPara[dwIndex].dwMembank;
        ptEpcRead->tEpcReadPara.wWordPointer = g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordPointer;
        ptEpcRead->tEpcReadPara.wWordCount = g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordCount;
        /*ƥ��ģ����λ��Ҫת�����ֵ�λ�����wpointΪ15 Ҳ��0��ʼ��*/
        ptEpcRead->tEpcReadPara.dwAccessPassword = 0;
        /*�����漰��impinj��ģʽת��0������ģʽת��1ģʽת��*/
        ptEpcRead->tEpcReadPara.dwUhfBitCommand = 0;
        ptEpcRead->tTagId.wTagWordLen = ptEpcTagId->wTagWordLen;
        memcpy(ptEpcRead->tTagId.aucTagData, ptEpcTagId->aucTagData, (ptEpcTagId->wTagWordLen)*sizeof(unsigned short int));
        ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcRead) + ptEpcTagId->wTagWordLen * sizeof(unsigned short int);
        RdpLlrpDbgPrint("(LLRP)Epc get pwdIndex Read\r\n");
        RdpLlrpDbgPrint("(LLRP)AntIndex :%d\r\n", ptEpcRead->dwAntIndex);
        RdpLlrpDbgPrint("(LLRP)AccessPassword :0x%x\r\n", ptEpcRead->tEpcReadPara.dwAccessPassword);
        RdpLlrpDbgPrint("(LLRP)ucMB :%d\r\n", ptEpcRead->tEpcReadPara.ucMB);
        RdpLlrpDbgPrint("(LLRP)wWordCount :%d\r\n", ptEpcRead->tEpcReadPara.wWordCount);
        RdpLlrpDbgPrint("(LLRP)wWordPointer :%d\r\n", ptEpcRead->tEpcReadPara.wWordPointer);
        RdpLlrpDbgPrint("(LLRP)EpcId:");
        for (dwTestIndex = 0;dwTestIndex < ptEpcRead->tTagId.wTagWordLen*sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("0x%02x  ", ptEpcRead->tTagId.aucTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
        {
            /*����ʧ��д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
            break;
        }
        memset(&tSyncMsg, 0xff, sizeof(T_Msg));
        tSyncMsg.wMsgID = EV_USP_UHF_EPC_READ_RES;
        tSyncMsg.wMsgSequence = g_wInnerMsgSequence;
        dwRet = RDP_RECV_SYNC_MSG(req_socket_rcp, ptRcvMsg, RDP_MAX_LEN_MSG);
        if (dwRet < 0 )
        {
            /*д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptRcvMsg->wMsgID, ptRcvMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
            break;
        }
        ptEpcReadRsp = (T_USPUHFEpcReadRes *)(ptRcvMsg + 1);
        RdpLlrpDbgPrint("(LLRP)pwdIndex read wResult:%d\r\n", ptEpcReadRsp->wResult);
        if ( ptEpcReadRsp->wResult != 0)
        {
            if ( ptEpcReadRsp->wResult == USP_UHF_EPC_ACCESS_NOT_PERMIT)
            {
                RdpLlrpDbgPrint("(LLRP)access password error accumulate g_dwLlrpTagAccessErrTimes: %d\r\n", g_dwLlrpTagAccessErrTimes);
                g_dwLlrpTagAccessErrTimes++;
            }
            //break;
        }
        else
        {
            memcpy(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wPwdIndexBuffer, ptEpcReadRsp->awReadData, ptEpcReadRsp->wResultLength*sizeof(unsigned short int));

        }
    }
    if (dwIndex == g_tRdpPwdSys.dwPwdIndexNum)
    {
        RdpLlrpDbgPrint("(LLRP)get PwdIndex  success pfusp_lib_uhf_getPassword=0x%x\r\n", g_tRdpPwdSys.pfusp_lib_uhf_getPassword);
        if (NULL == g_tRdpPwdSys.pfusp_lib_uhf_getPassword)
        {
            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
            RdpLlrpDbgPrint("(LLRP)error happened\r\n");
            return LLRP_EPC_READ_PWDINDEX_FAIL;
        }
        RdpLlrpDbgPrint("(LLRP)g_tRdpPwdSys.dwPwdIndexNum=%d\r\n", g_tRdpPwdSys.dwPwdIndexNum);
        for (wLoop = 0;wLoop < g_tRdpPwdSys.dwPwdIndexNum;wLoop++)
        {
            RdpLlrpDbgPrint("(LLRP)MB=%d,WordPointor=%d,wWordCount=%d\r\n",
                            g_tRdpPwdSys.atPwdIndexPara[wLoop].dwMembank,
                            g_tRdpPwdSys.atPwdIndexPara[wLoop].wWordPointer,
                            g_tRdpPwdSys.atPwdIndexPara[wLoop].wWordCount);

            RdpLlrpDbgPrint("input is:\r\n");
            for (wLoop1 = 0;wLoop1 < 6;wLoop1++)
            {
                RdpDbgPrintRaw("0x%x ", g_tRdpPwdSys.atPwdIndexPara[wLoop].wPwdIndexBuffer[wLoop1]);
            }
            RdpLlrpDbgPrint("\r\n");
        }
        dwRet = g_tRdpPwdSys.pfusp_lib_uhf_getPassword(g_tRdpPwdSys.atPwdIndexPara, g_tRdpPwdSys.dwPwdIndexNum, pdwAccessPassword);
        if (dwRet != 0)
        {
            RdpLlrpDbgPrint("(LLRP)use  getPassword failed\r\n");
            return LLRP_EPC_READ_PWDINDEX_FAIL;
        }
        RdpLlrpDbgPrint("(LLRP)use  getPassword success accesspwd=%x\r\n", *pdwAccessPassword);
        return LLRP_EPC_READ_PWDINDEX_SUCCESS;
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)get PwdIndex  failed \r\n");
        return LLRP_EPC_READ_PWDINDEX_FAIL;
    }
}
/**************************************************************************
* �������ƣ�llrp_EpcMatchRead
* ����������ƥ��Epc������
* ���ʵı���
* �޸ĵı���
* ��������TT_USPUHFEpcTagId *ptEpcTagId,
                             unsigned short int wAntID,
                             unsigned int dwRcpCommand ,
                             T_RdpLlrpC1G2TargetTag *ptEpcTargetTag,
                             T_USPUHFEpcReadRes **ptEpcReadRsp,
                             T_RdpLlrpAccess*ptMatchAccess
* ���������T_USPUHFEpcReadRes **ptEpcReadRsp
                             ���ؽ��ָ��
* �� �� ֵ��   LLRP_ACCESS_MATCH_SUCCESS: ƥ��ɹ���
                             LLRP_ACCESS_MATCH_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_EpcMatchRead(T_USPUHFEpcTagId *ptEpcTagId, unsigned short int wAntID, unsigned int dwRcpCommand , T_RdpLlrpC1G2TargetTag *ptEpcTargetTag, T_USPUHFEpcReadRes **ptEpcReadRsp, T_RdpLlrpAccess*ptMatchAccess)
{
    T_Msg tSyncMsg;
    T_Msg *ptSyncMsg[] = {&tSyncMsg, NULL};
    int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned int dwTestIndex = 0;
    unsigned char ucRevBuf[8192] = {0};
    T_USPUHFEpcRead *ptEpcRead = NULL;
    T_Msg *ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    T_Msg *ptRcvMsg = (T_Msg *)ucRevBuf;
    unsigned int dwAccessPassword = 0;
    if (g_tRdpPwdSys.dwPwdIndexNum == 0)
    {

        RdpLlrpDbgPrint("(LLRP)there is not PwdIndex use default Access funs\r\n");
        dwRet = llrp_getPassword(ptMatchAccess, &dwAccessPassword);
        if (dwRet != 0)
        {
            RdpLlrpDbgPrint("(LLRP)use default getPassword failed\r\n");
            return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
        }
    }
    else
    {
        dwRet = llrp_getEpcAccessPassword(ptEpcTagId, wAntID, &dwAccessPassword);
        if (dwRet != 0)
        {
            return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
        }
    }
    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgID = EV_USP_UHF_EPC_MATCH_READ;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_UHF_TNO;
    ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wReceiverTno = USP_UHF_TNO;
    ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
    ptEpcRead = (T_USPUHFEpcRead *)(ptSndMsg + 1);
    ptEpcRead->dwAntIndex = wAntID;
    ptEpcRead->tEpcReadPara.ucMB = ptEpcTargetTag->ucMB;
    /*ƥ��ģ����λ��Ҫת�����ֵ�λ�����wpointΪ15 Ҳ��0��ʼ��*/
    ptEpcRead->tEpcReadPara.wWordPointer = ptEpcTargetTag->wPointer / 16;
    if (ptEpcTargetTag->wPointer % 16 == 0 )
    {
        ptEpcRead->tEpcReadPara.wWordCount = BIT_CHANGETO_WORD16(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount);

    }
    else
    {
        ptEpcRead->tEpcReadPara.wWordCount = ( (ptEpcTargetTag->wPointer % 16) > 0 ? 1 : 0) \
                                             + ((ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount > (16 - ptEpcTargetTag->wPointer % 16)) ? \
                                                BIT_CHANGETO_WORD16(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount - (16 - ptEpcTargetTag->wPointer % 16)) : 0);
    }
    ptEpcRead->tEpcReadPara.dwAccessPassword = dwAccessPassword;
    llrp_setEpcAccessFlag();
    /*�����漰��impinj��ģʽת��0������ģʽת��1ģʽת��*/
    ptEpcRead->tEpcReadPara.dwUhfBitCommand = dwRcpCommand;
    ptEpcRead->tTagId.wTagWordLen = ptEpcTagId->wTagWordLen;
    memcpy(ptEpcRead->tTagId.aucTagData, ptEpcTagId->aucTagData, (ptEpcTagId->wTagWordLen)*sizeof(unsigned short int));
    //ptEpcRead->tTagId=*ptEpcTagId;
    ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcRead) + ptEpcTagId->wTagWordLen * sizeof(unsigned short int);
    RdpLlrpDbgPrint("(LLRP)Epc Match Read\r\n");
    RdpLlrpDbgPrint("(LLRP)AntIndex :%d\r\n", ptEpcRead->dwAntIndex);
    RdpLlrpDbgPrint("(LLRP)AccessPassword :0x%x\r\n", ptEpcRead->tEpcReadPara.dwAccessPassword);
    RdpLlrpDbgPrint("(LLRP)ucMB :%d\r\n", ptEpcRead->tEpcReadPara.ucMB);
    RdpLlrpDbgPrint("(LLRP)wWordCount :%d\r\n", ptEpcRead->tEpcReadPara.wWordCount);
    RdpLlrpDbgPrint("(LLRP)wWordPointer :%d\r\n", ptEpcRead->tEpcReadPara.wWordPointer);
    RdpLlrpDbgPrint("(LLRP)EpcId:");
    for (dwTestIndex = 0;dwTestIndex < ptEpcRead->tTagId.wTagWordLen*sizeof(unsigned short int);dwTestIndex++)
    {
        RdpDbgPrintRaw("0x%02x  ", ptEpcRead->tTagId.aucTagData[dwTestIndex]);
    }
    RdpLlrpDbgPrint("\r\n");
    if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
    {
        /*����ʧ��д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    memset(&tSyncMsg, 0xff, sizeof(T_Msg));
    tSyncMsg.wMsgID = EV_USP_UHF_EPC_MATCH_READ_RES;
    tSyncMsg.wMsgSequence = g_wMsgSequence;
    dwRet = RDP_RECV_SYNC_MSG(req_socket_rcp, ptRcvMsg, RDP_MAX_LEN_MSG);
    if (dwRet <0 )
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptRcvMsg->wMsgID, ptRcvMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    *ptEpcReadRsp = (T_USPUHFEpcReadRes *)(ptRcvMsg + 1);
    RdpLlrpDbgPrint("(LLRP)matchread wResult:%d\r\n", (*ptEpcReadRsp)->wResult);
    if ((*ptEpcReadRsp)->wResult != 0)
    {
        if ( (*ptEpcReadRsp)->wResult == USP_UHF_EPC_ACCESS_NOT_PERMIT)
        {
            RdpLlrpDbgPrint("access password error accumulate g_dwLlrpTagAccessErrTimes: %d\r\n", g_dwLlrpTagAccessErrTimes);
            g_dwLlrpTagAccessErrTimes++;
        }
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    return LLRP_ACCESSPATTERN_MATCHREAD_SUCCESS;
}
/**************************************************************************
* �������ƣ�llrp_EpcMatchAccessPattern
* ����������epcƥ��ģ�����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_EPCMatchTAG_SUCCESS: ƥ��ɹ���
                             LLRP_EPCMatchTAG_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_EpcMatchAccessPattern(unsigned int dwByteLen, unsigned char *pucMatchData , T_RdpLlrpC1G2TargetTag *ptMatchEpcTargetTag)
{
    unsigned char ucTempTagDataFirstHaft = 0;
    unsigned char ucTempTagDataLastHaft = 0;
    unsigned char aucTagData[256] = {0};
    unsigned char dwTagDataIndex = 0;
    unsigned int dwIndex = 0;
    unsigned int dwTagMaskAndDataOffset = 0;
    unsigned int dwTagMaskAndDataCmpTimes = 0;
    unsigned int dwTagOffsetBits = 0;
    T_RdpLlrpMatchParam *ptMatchData = NULL;
    dwTagMaskAndDataCmpTimes = dwTagMaskAndDataOffset = BIT_CHANGETO_BYTE(ptMatchEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount);
    /*����EPC��ƥ��ĵڼ����ֽ�*/
    /*����ֽڵĵڼ���bit*/
    dwTagOffsetBits = (ptMatchEpcTargetTag->wPointer) % 8;
    ptMatchData = &ptMatchEpcTargetTag->tRdpLlrpMatchParam;
    memcpy(aucTagData, pucMatchData, dwByteLen);
    for (dwIndex = 0; dwIndex < dwTagMaskAndDataCmpTimes; dwIndex++, dwTagDataIndex++)
    {
        /*ȡ��ƥ���epc��ĵ�һ��ƥ���ֽ�*/
        ucTempTagDataFirstHaft = aucTagData[dwTagDataIndex];
        /*������һ��ƥ����ֽ�*/
        ucTempTagDataFirstHaft = ucTempTagDataFirstHaft << dwTagOffsetBits;
        ucTempTagDataLastHaft = aucTagData[dwTagDataIndex + 1];
        ucTempTagDataLastHaft = ucTempTagDataLastHaft >> (8 - dwTagOffsetBits);
        aucTagData[dwTagDataIndex] = ucTempTagDataFirstHaft | ucTempTagDataLastHaft;

        /*�����ƥ�����ģ�� Ӧ�� ��ǩ���Ȱ���λ��ƥ������ ���
         Ȼ���ں������������������� 1^0 = 1 ˵�����ݲ�ƥ��  1&0 =0 ˵��������λ ���Ϊ0
        1^0 =1 ���� 1&1=1 ˵������λ ��ô�����Ͳ�����ģ���� �˳�*/
        RdpLlrpWarningPrint("(LLRP)epc Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                            aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                            ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
        if ((ptMatchEpcTargetTag->bMatch == TRUE) \
                && (((aucTagData[dwTagDataIndex] \
                      ^ ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset])\
                     & (ptMatchData->aucTagData[dwIndex])) != 0))
        {

            RdpLlrpWarningPrint("(LLRP)access match pattern is equal the byte cmp fail  epc Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                                aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                                ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
            return LLRP_EPCMATCHTAG_FAIL;
        }
        /*����ǲ�ƥ�����ģ�� Ӧ�� ��ǩ���Ȱ���λ��ƥ������ ͬ��
        Ȼ���ں�������������*/
        if ((ptMatchEpcTargetTag->bMatch == FALSE)
                && (((~(aucTagData[dwTagDataIndex] \
                        ^ ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]))
                     & (ptMatchData->aucTagData[dwIndex])) != 0))
        {
            RdpLlrpWarningPrint("(LLRP)access match pattern is not equal the byte cmp fail  epc Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                                aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                                ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
            return LLRP_EPCMATCHTAG_FAIL;
        }
    }
    return LLRP_EPCMATCHTAG_SUCCESS;
}

/**************************************************************************
* �������ƣ�llrp_matchAccessTagPattern
* ����������ƥ��Accessָ����ƥ��ģ��
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_ACCESS_MATCH_SUCCESS: ƥ��ɹ���
                             LLRP_ACCESS_MATCH_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_matchAccessTagPattern(T_RdpLlrpAccess *ptMatchAccess, void *pTagID, unsigned short int wAntID)
{
    unsigned int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned int dwTestIndex = 0;
    unsigned int dwByteLen = 0;
    T_RdpLlrpC1G2TargetTag *ptEpcTargetTag = NULL;
    T_USPUHFEpcReadRes *ptEpcReadRsp = NULL;
    T_RdpLlrpISO6BTargetTag *ptISO6BTargetTag = NULL;
    unsigned char *pucMatchData = NULL;
    T_USPUHFEpcTagId *ptEpcTagId = NULL;
    T_USPUHFISO6BTagId *ptISO6BTagId = NULL;
    T_USPUHFISO6BReadRes *ptISO6BReadRsp = NULL;
    T_RdpLlrpGBTargetTag *ptGBTargetTag = NULL;
    T_USPUHFGBReadRes *ptGBReadRsp = NULL;
    T_USPUHFGBTagId *ptGBTagId = NULL;
    for (dwIndex = 0; dwIndex < ptMatchAccess->tRdpLlrpAccessCmdCtrl.tAirProtocolTag.dwTagPatternNum;dwIndex++)
    {
        RdpLlrpDbgPrint("(LLRP)%d Tag Pattern match begin\r\n", dwIndex + 1);
        if (ptMatchAccess->eProtocolID == LLRP_AirProtocols_EPCGlobalClass1Gen2)
        {
            ptEpcTagId = (T_USPUHFEpcTagId *)pTagID;
            ptEpcTargetTag = (T_RdpLlrpC1G2TargetTag *) & ptMatchAccess->tRdpLlrpAccessCmdCtrl.tAirProtocolTag.tTagPattern[dwIndex];
            if (BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wDataBitCount))
            {
                /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                RdpLlrpDbgPrint("(LLRP)masklen is longer than dataPatern match failed masklen:%d, datapatern Len:%d\r\n",
                                BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount), BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wDataBitCount));
                break;
            }
            if (ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount == 0)
            {
                RdpLlrpDbgPrint("(LLRP)match bits are zero return match success\r\n");
                continue;
            }
            else
            {
                if ((ptEpcTargetTag->ucMB != 1 && ptMatchAccess->tRdpLlrpAccessCmdCtrl.bCircumstanceSetting == FALSE )
                        || ptMatchAccess->tRdpLlrpAccessCmdCtrl.bCircumstanceSetting == TRUE)
                {
                    /*�·�������*/
                    /*�ȴ�����Ӧ����ʱ*/
                    RdpLlrpDbgPrint("(LLRP)access match need to read not Epc Tagdata\r\n");
                    dwRet = llrp_EpcMatchRead(ptEpcTagId, wAntID,
                                              ptMatchAccess->tRdpLlrpAccessCmdCtrl.bCircumstanceSetting,
                                              ptEpcTargetTag, &ptEpcReadRsp, ptMatchAccess);
                    if (dwRet == LLRP_ACCESSPATTERN_MATCHREAD_FAIL)
                    {
                        /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                        RdpLlrpDbgPrint("(LLRP)match read failed\r\n");
                        break;
                    }
                    /*�����ĳ��� ֱ���Ƿ��صĽ����ȥƫ�����ֽ�*/
                    dwByteLen = ptEpcReadRsp->wResultLength * sizeof(unsigned short int) - ptEpcTargetTag->wPointer % 16 / 8;
                    pucMatchData = (unsigned char *) & ptEpcReadRsp->awReadData[0];
                    /*��ΪEpc���ض��������Ϊ��λ��
                       ����Ҫȷ���Ӷ������һ���ֽڿ�ʼƥ��
                       ���ǵڶ����ֽڿ�ʼƥ��*/
                    pucMatchData = (pucMatchData + ptEpcTargetTag->wPointer % 16 / 8);
                }
                /*Epc�� Ҫ��ƫ��*/
                else
                {
                    /*�ж�һ�³��ȣ�С����ʼ��ַƫ������ ֱ���˳�*/
                    if ( (ptEpcTagId->wTagWordLen)*sizeof(unsigned short int) > ((ptEpcTargetTag->wPointer - 32) / 8))
                    {

                        dwByteLen = (ptEpcTagId->wTagWordLen) * sizeof(unsigned short int) - ((ptEpcTargetTag->wPointer - 32) / 8);
                        pucMatchData = &ptEpcTagId->aucTagData[(ptEpcTargetTag->wPointer - 32) / 8];
                    }
                    else
                    {
                        RdpLlrpDbgPrint("epc match begin byte addr %02x\r\n,Epc len %02x", (ptEpcTargetTag->wPointer - 32) / 8, (ptEpcTagId->wTagWordLen)*sizeof(unsigned short int));
                        break;
                    }
                }
                /*��ӡ��ƥ������*/
                if (BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > dwByteLen)
                {
                    /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                    RdpLlrpDbgPrint("(LLRP)lenth diff match fail masklen :%d, Byte Len :%d\r\n",
                                    BIT_CHANGETO_BYTE(ptEpcTargetTag->tRdpLlrpMatchParam.wMaskBitCount), dwByteLen);
                    break;
                }
                /*ֱ�ӵ���ƥ�俴�Ƿ�ɹ�*/
                /*�ɹ� ������һ��ƥ�䣬���ɹ�ֱ���˳�����ƥ�䲻�ɹ�*/
                dwRet = llrp_EpcMatchAccessPattern(dwByteLen, pucMatchData, ptEpcTargetTag);
                if (dwRet == LLRP_EPCMATCHTAG_FAIL)
                {
                    break;
                }
            }
        }
        else if (ptMatchAccess->eProtocolID == LLRP_AirProtocols_ISO18000_6B)
        {
            ptISO6BTagId = (T_USPUHFISO6BTagId *)pTagID;
            ptISO6BTargetTag = (T_RdpLlrpISO6BTargetTag *) & ptMatchAccess->tRdpLlrpAccessCmdCtrl.tAirProtocolTag.tTagPattern[dwIndex];
            if (BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wDataBitCount))
            {
                /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                RdpLlrpDbgPrint("(LLRP)masklen is longer than dataPatern match failed masklen:%d, datapatern Len:%d\r\n",
                                BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount),
                                BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wDataBitCount));
                break;
            }
            if (ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount == 0)
            {
                RdpLlrpDbgPrint("(LLRP)match bits are zero return match success\r\n");
                continue;
            }
            else
            {
                if (ptISO6BTargetTag->wBytePtr + BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > 8)
                {
                    RdpLlrpWarningPrint("(LLRP)access match need to read ISO6B Tagdata\r\n");
                    dwRet = llrp_ISO6BMatchRead(ptISO6BTagId, wAntID, ptISO6BTargetTag, &ptISO6BReadRsp);
                    if (dwRet == LLRP_ACCESSPATTERN_MATCHREAD_FAIL)
                    {
                        break;
                    }
                    dwByteLen = BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount);
                    pucMatchData = &ptISO6BReadRsp->aucReadData[0];
                }
                else
                {
                    dwByteLen = BIT_CHANGETO_BYTE(ptISO6BTargetTag->tRdpLlrpMatchParam.wMaskBitCount);
                    pucMatchData = &ptISO6BTagId->aucTagData[0];
                }
                /*ֱ�ӵ���ƥ�俴�Ƿ�ɹ�*/
                /*�ɹ� ������һ��ƥ�䣬���ɹ�ֱ���˳�����ƥ�䲻�ɹ�*/
                dwRet = llrp_ISO6BMatchAccessPattern(dwByteLen, pucMatchData, ptISO6BTargetTag);
                if (dwRet == LLRP_ISO6BMATCHTAG_FAIL)
                {
                    break;
                }
            }
        }
	else if (ptMatchAccess->eProtocolID == LLRP_AirProtocols_GB)
        {
            ptGBTagId = (T_USPUHFGBTagId *)pTagID;
            ptGBTargetTag = (T_RdpLlrpGBTargetTag *) & ptMatchAccess->tRdpLlrpAccessCmdCtrl.tAirProtocolTag.tTagPattern[dwIndex];
            if (BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wDataBitCount))
            {
                /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                RdpLlrpDbgPrint("(LLRP)masklen is longer than dataPatern match failed masklen:%d, datapatern Len:%d\r\n",
                                BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount), BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wDataBitCount));
                break;
            }
            if (ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount == 0)
            {
                RdpLlrpDbgPrint("(LLRP)match bits are zero return match success\r\n");
                continue;
            }
            else
            {
                if (ptGBTargetTag->ucMB != LLRP_MB_EPC)
                {
                    /*�·�������*/
                    /*�ȴ�����Ӧ����ʱ*/
                    RdpLlrpDbgPrint("(LLRP)access match need to read not GB Tagdata\r\n");
                    dwRet = llrp_GBMatchRead(ptGBTagId, wAntID,
                                              ptGBTargetTag, &ptGBReadRsp, ptMatchAccess);
                    if (dwRet == LLRP_ACCESSPATTERN_MATCHREAD_FAIL)
                    {
                        /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                        RdpLlrpDbgPrint("(LLRP)match read failed\r\n");
                        break;
                    }
                    /*�����ĳ��� ֱ���Ƿ��صĽ����ȥƫ�����ֽ�*/
                    dwByteLen = ptGBReadRsp->wResultLength * sizeof(unsigned short int) - ptGBTargetTag->wPointer % 16 / 8;
                    pucMatchData = (unsigned char *) & ptGBReadRsp->awReadData[0];
                    /*��ΪGB���ض��������Ϊ��λ��
                       ����Ҫȷ���Ӷ������һ���ֽڿ�ʼƥ��
                       ���ǵڶ����ֽڿ�ʼƥ��*/
                    pucMatchData = (pucMatchData + ptGBTargetTag->wPointer % 16 / 8);
                }
                /*GB�� Ҫ��ƫ��*/
                else
                {
                    /*�ж�һ�³��ȣ�С����ʼ��ַƫ������ ֱ���˳�*/
                    if ( (ptGBTagId->wTagWordLen)*sizeof(unsigned short int) > ((ptGBTargetTag->wPointer - 16) / 8))
                    {

                        dwByteLen = (ptGBTagId->wTagWordLen) * sizeof(unsigned short int) - ((ptGBTargetTag->wPointer - 16) / 8);
                        pucMatchData = &ptGBTagId->aucTagData[(ptGBTargetTag->wPointer - 16) / 8];
                    }
                    else
                    {
                        RdpLlrpDbgPrint("GB match begin byte addr %02x\r\n,GB len %02x", (ptGBTargetTag->wPointer - 16) / 8, (ptGBTagId->wTagWordLen)*sizeof(unsigned short int));
                        break;
                    }
                }
                /*��ӡ��ƥ������*/
                if (BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount) > dwByteLen)
                {
                    /*ƥ���ʧ�� ��ƥ��ֱ��ʧ��*/
                    RdpLlrpDbgPrint("(LLRP)lenth diff match fail masklen :%d, Byte Len :%d\r\n",
                                    BIT_CHANGETO_BYTE(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount), dwByteLen);
                    break;
                }
                /*ֱ�ӵ���ƥ�俴�Ƿ�ɹ�*/
                /*�ɹ� ������һ��ƥ�䣬���ɹ�ֱ���˳�����ƥ�䲻�ɹ�*/
                dwRet = llrp_GBMatchAccessPattern(dwByteLen, pucMatchData, ptGBTargetTag);
                if (dwRet == LLRP_GBMATCHTAG_FAIL)
                {
                    break;
                }
            }
        }
        else
        {
            /*��¼�쳣*/
            RdpLlrpDbgPrint("(LLRP)match unknow protocal eProtocolID=%d\r\n", ptMatchAccess->eProtocolID);
            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, ptMatchAccess->eProtocolID, 0, 0, RDP_LLRP_EPC_ID);
            break;
        }
    }
    if (dwIndex == ptMatchAccess->tRdpLlrpAccessCmdCtrl.tAirProtocolTag.dwTagPatternNum)
    {
        return LLRP_ACCESSPATTERN_MATCH_SUCCESS;
    }
    else
    {
        return LLRP_ACCESSPATTERN_MATCH_FAIL;
    }
}
/**************************************************************************
* �������ƣ�llrp_matchAccessTagPattern
* ����������ƥ��Access�Ĺ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_ACCESS_MATCH_SUCCESS: ƥ��ɹ���
                             LLRP_ACCESS_MATCH_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_matchAccess(LLRP_tEAirProtocols eProtocolID, unsigned short int wAntID, void *pTaID )
{
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAccessEntry *ptAccessCursor = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptAccessCtrlAndParam = NULL;
    T_RdpLlrpAccess *ptMatchAccess = NULL;
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return LLRP_ACCESS_MATCH_FAIL;
    }
    for (ptAccessCursor = g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tEnableAccessQueue.ptNext;
            ptAccessCursor != NULL; ptAccessCursor = ptAccessCursor->ptNext)
    {
        RdpLlrpDbgPrint("(LLRP)current match dwAccessIndex :%d\r\n", ptAccessCursor->dwAccessIndex);
        ptAccessCtrlAndParam = &g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessCursor->dwAccessIndex];
        ptMatchAccess = &ptAccessCtrlAndParam->tRdpLlrpAccess;
        if (ptAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex  != ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkIndex
            || ptAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType != ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkType)
        {
            RdpLlrpDbgPrint("(LLRP)ucLinkIndex or  ucLinkType is not equal when matching access ucLinkIndex:%d ,current ucLinkIndex :%d,access ucLinkType:%d ,current ucLinkType :%d\r\n",
                            ptAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkIndex, ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkIndex,
                            ptAccessCtrlAndParam->tRdpLlrpAccessDispatchCtrl.ucLinkType, ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkType);
            continue;
        }
        if ( ptMatchAccess->eProtocolID != eProtocolID)
        {
            RdpLlrpDbgPrint("(LLRP)protocol is not equal when matching access protocol:%d ,current protocol :%d\r\n",
                            ptMatchAccess->eProtocolID, eProtocolID);
            continue;
        }
        if ( ptMatchAccess->dwROSpecID != 0 && ptMatchAccess->dwROSpecID != ptRunRoCtrlAndPara->tRdpLlrpROSpec.dwROSpecID)
        {
            RdpLlrpDbgPrint("(LLRP)ROSpecID is not equal when matching,access match RoID:%d,current RoId: %d\r\n",
                            ptMatchAccess->dwROSpecID, ptRunRoCtrlAndPara->tRdpLlrpROSpec.dwROSpecID);
            continue;
        }
        if (ptMatchAccess->wAntennaID != 0 && ptMatchAccess->wAntennaID != wAntID)
        {
            RdpLlrpDbgPrint("(LLRP)wAntID is not equal when matching access AntId:%d,current AntId:%d\r\n",
                            ptMatchAccess->wAntennaID, wAntID);
            continue;
        }

        if (llrp_matchAccessTagPattern(ptMatchAccess, pTaID, wAntID) == LLRP_ACCESSPATTERN_MATCH_FAIL)
        {
            RdpLlrpDbgPrint("(LLRP)llrp_matchAccessTagPattern failed:\r\n");
            continue;
        }
        else
        {
            /*ƥ��ɹ�����ѭ��*/
            RdpLlrpDbgPrint("(LLRP)matchAccessTagPattern success:\r\n");
            break;
        }
    }

    if (ptAccessCursor == NULL)
    {
        RdpLlrpDbgPrint("(LLRP)no match Access and match fail\r\n");
        return LLRP_ACCESS_MATCH_FAIL;
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)match success access id :%d \r\n",
                        g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRdpLlrpAccessCtrlAndParam[ptAccessCursor->dwAccessIndex].tRdpLlrpAccess.dwAccessSpecID);
        llrp_buildAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess,
                                           ptAccessCursor->dwAccessIndex);
        return LLRP_ACCESS_MATCH_SUCCESS;
    }
}
/**************************************************************************
* �������ƣ�llrp_deleteActiveROAndSndRsp
* ����������ɾ��ActiveRO��Ĳ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_deleteActiveROAndSndRsp()
{
    T_Msg tMsg;
    unsigned int dwROIndex = 0;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    RdpLlrpDbgPrint("(LLRP)come into llrp_deleteActiveROAndSndRsp\r\n");
    /*endofRO�ϱ�����Ĵ���*/
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    llrp_freeReportToROReportLink(&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink);

    llrp_reportReaderEventNotification(LLRP_NotificationEventType_ROSpec_Event , LLRP_ROSpecEventType_End_Of_ROSpec);

    llrp_findAndFreeAnNodeFoAllQueueOfRO(&g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tAddQueuehead,
                                         g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex,
                                         g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwPriority);

    llrp_findAndFreeAnNodeFoAllQueueOfRO(&g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO,
                                         g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex,
                                         g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwPriority);
    /*ɨ��Ready��������Ro*/
    tMsg.mtype = 1;
    tMsg.ucLinkIndex = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkIndex;
    tMsg.ucLinkType = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkType;
    tMsg.wReceiverTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wReceiverTno;
    tMsg.wSenderTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wSenderTno;
    /*delete activeRo�ڴ������֮���ٷ�����Ӧ,���ﲻ��Ҫ���� ��Ϣ�ţ���Ҫ����dwDeleteRoID ��Ϊ��0�����*/
    llrp_sendRuleRspMsg(&tMsg, LLRP_StatusCode_M_Success, EV_DELETE_ROSPEC_RESPONSE,
                        ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDeleteRoMsgID, ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDeleteRoID);
    memset(ptRunRoCtrlAndPara , 0, sizeof(T_RdpLlrpROCtrlAndParam));
    if ( llrp_cfgChangeRefresh() != LLRP_CFG_CHANGE)
    {
        RdpLlrpDbgPrint("(LLRP)after deleteActiveRO and begin scan\r\n");
        g_tRdpLlrpReaderDispatch.pfScanROToRun(g_tRdpLlrpReaderDispatch.ptReaderRoSpecData);
    }
}
/**************************************************************************
* �������ƣ�llrp_disableActiveROAndSndRsp
* ������������ֹactiveRo��disable
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_disableActiveROAndSndRsp()
{
    T_Msg tMsg;
    unsigned int dwROIndex = 0;
    T_RdpLlrpROEntry *ptActiveEntry = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    /*endofRO�ϱ�����Ĵ���*/
    RdpLlrpDbgPrint("(LLRP)come into llrp_disableActiveROAndSndRsp\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    llrp_freeReportToROReportLink(&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport.tActiveRoReportLink);
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.eCurrentState = LLRP_ROSpecState_Disabled;
    llrp_reportReaderEventNotification(LLRP_NotificationEventType_ROSpec_Event , LLRP_ROSpecEventType_End_Of_ROSpec);
    /*activeRoת����disable������*/
    ptActiveEntry = llrp_findAndGetAnNodeFoAllQueueOfRO(&g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwPriority);
    if (ptActiveEntry == NULL)
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)can not find activeRO 0\r\n");
    }
    else
    {
        llrp_insertAnNodetoAllQueueOfRO(ptActiveEntry, &g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tDisableRoQueuehead);
    }
    /*ɨ��Ready��������Ro*/
    tMsg.mtype = 1;
    tMsg.ucLinkIndex = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkIndex;
    tMsg.ucLinkType = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkType;
    tMsg.wReceiverTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wReceiverTno;
    tMsg.wSenderTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wSenderTno;
    /*disable activeRo�ڴ������֮���ٷ�����Ӧ,���ﲻ��Ҫ���� ��Ϣ�ţ���Ҫ����dwDisableRoID ��Ϊ��0�����*/
    llrp_sendRuleRspMsg(&tMsg, LLRP_StatusCode_M_Success, EV_DISABLE_ROSPEC_RESPONSE,
                        ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDisableRoMsgID, ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDisableRoID);
    if ( llrp_cfgChangeRefresh() != LLRP_CFG_CHANGE)
    {
        RdpLlrpDbgPrint("(LLRP)after disableActiveRO and begin scan\r\n");
        g_tRdpLlrpReaderDispatch.pfScanROToRun(g_tRdpLlrpReaderDispatch.ptReaderRoSpecData);
    }
}

/**************************************************************************
* �������ƣ�llrp_stopActiveRO
* ����������ֹͣactiveRo��inactive
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_stopActiveRO()
{
    T_Msg tMsg;
    unsigned int dwROIndex = 0;
    T_RdpLlrpROEntry *ptActiveEntry = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    /*endofRO�ϱ�����Ĵ���*/
    RdpLlrpDbgPrint("(LLRP)come into llrp_stopActiveRO\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
    .pfusp_rdp_llrp_EndROForReportTrigger((void*)&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.eCurrentState = LLRP_ROSpecState_Inactive;
    llrp_reportReaderEventNotification(LLRP_NotificationEventType_ROSpec_Event , LLRP_ROSpecEventType_End_Of_ROSpec);
    /*activeRoת����disable������*/
    ptActiveEntry = llrp_findAndGetAnNodeFoAllQueueOfRO(&g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwPriority);
    if (ptActiveEntry == NULL)
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)can not find activeRO\r\n");
        return ;
    }
    else
    {
        llrp_insertAnNodetoAllQueueOfRO(ptActiveEntry, &g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tInactiveRoQueuehead);
    }
    LLRP_STOP_TIMER(EV_RDP_LLRP_RULETIMER_BEGIN + ptActiveEntry->dwROIndex);
    tMsg.mtype = 1;
    tMsg.ucLinkIndex = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkIndex;
    tMsg.ucLinkType = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.ucLinkType;
    tMsg.wReceiverTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wReceiverTno;
    tMsg.wSenderTno = ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.wSenderTno;
    /*stop activeRo�ڴ������֮���ٷ�����Ӧ*/
    llrp_sendRuleRspMsg(&tMsg, LLRP_StatusCode_M_Success, EV_STOP_ROSPEC_RESPONSE,
                        ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwStopRoMsgID, ptRunRoCtrlAndPara->tRdpLlrpROSpec.dwROSpecID);
    /*ɨ��Ready��������Ro*/
    if ( llrp_cfgChangeRefresh() != LLRP_CFG_CHANGE)
    {
        RdpLlrpDbgPrint("(LLRP)after stopActiveRO and begin scan\r\n");
        g_tRdpLlrpReaderDispatch.pfScanROToRun(g_tRdpLlrpReaderDispatch.ptReaderRoSpecData);
    }
}
/**************************************************************************
* �������ƣ�llrp_activeROTerminate
* ����������Ro������ִ�н���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_activeROTerminate()
{
    unsigned int dwROIndex = 0;
    T_RdpLlrpROEntry *ptActiveEntry = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    /*endofRO�ϱ�����Ĵ���*/
    RdpLlrpDbgPrint("(LLRP)come into llrp_activeROTerminate\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
    .pfusp_rdp_llrp_EndROForReportTrigger((void *)&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.eCurrentState = LLRP_ROSpecState_Inactive;
    ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDispatchStatus = LLRP_RODISPATCH_STATE_INACTIVE;
    /*activeRoת����disable������*/
    llrp_reportReaderEventNotification(LLRP_NotificationEventType_ROSpec_Event , LLRP_ROSpecEventType_End_Of_ROSpec);
    ptActiveEntry = llrp_findAndGetAnNodeFoAllQueueOfRO(&g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwROIndex,
                    g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext->dwPriority);
    if (ptActiveEntry == NULL)
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)can not find activeRO \r\n");
    }
    else
    {
        llrp_insertAnNodetoAllQueueOfRO(ptActiveEntry, &g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tInactiveRoQueuehead);
    }

    /*���������Ե�Ro,�鿴�Ƿ�ת��Ready ������*/
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStartTrigger.pfusp_rdp_llrp_PeriodStartTrigger((void *)ptActiveEntry);
    /*ɨ��Ready��������Ro*/
    if ( llrp_cfgChangeRefresh() != LLRP_CFG_CHANGE)
    {
        RdpLlrpDbgPrint("(LLRP)activeROTerminate and begin scan\r\n");
        g_tRdpLlrpReaderDispatch.pfScanROToRun(g_tRdpLlrpReaderDispatch.ptReaderRoSpecData);
    }
}

/**************************************************************************
* �������ƣ�llrp_sendNextEpcInv
* �����������·��������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_sendNextEpcInv()
{
    T_Msg *ptSndMsg = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_sendNextEpcInv\r\n");
    memset(g_aucRdpLlrpSndMsg, 0, sizeof(g_aucRdpLlrpSndMsg));
    ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    ptSndMsg->wMsgID = EV_USP_UHF_EPC_QUERY;
    ptSndMsg->wMsgLen = 0;
    ptSndMsg->mtype = LLRP_SYN_MSG;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_UHF_TNO;
    ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wReceiverTno = USP_UHF_TNO;
    ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
    if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_DISPATCH_ID);
    }
    /*������Ϣ���ض�ʱ�� �Է�fpga����Ӧ*/
    LLRP_START_TIMER(TFpgaResp, TFpgaRespCount);
}
/**************************************************************************
* �������ƣ�llrp_sendNextISO6BInv
* �����������·��������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_sendNextISO6BInv()
{
    T_Msg *ptSndMsg = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_sendNextISO6BInv\r\n");
    memset(g_aucRdpLlrpSndMsg, 0, sizeof(g_aucRdpLlrpSndMsg));
    ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    ptSndMsg->wMsgID = EV_USP_UHF_ISO6B_GETTAG;
    ptSndMsg->wMsgLen = 0;
    ptSndMsg->mtype = LLRP_SYN_MSG;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_UHF_TNO;
    ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wReceiverTno = USP_UHF_TNO;
    ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
    if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_DISPATCH_ID);
    }
    /*������Ϣ���ض�ʱ�� �Է�fpga����Ӧ*/
    LLRP_START_TIMER(TFpgaResp, TFpgaRespCount);
}
/**************************************************************************
* �������ƣ�llrp_sendNextGBInv
* �����������·��������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/204    V1.0       lzy        ����
*****************************************************************************/
void llrp_sendNextGBInv()
{
    T_Msg *ptSndMsg = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_sendNextGBInv\r\n");
    memset(g_aucRdpLlrpSndMsg, 0, sizeof(g_aucRdpLlrpSndMsg));
    ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    ptSndMsg->wMsgID = EV_USP_UHF_GB_QUERY;
    ptSndMsg->wMsgLen = 0;
    ptSndMsg->mtype = LLRP_SYN_MSG;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_UHF_TNO;
    ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wReceiverTno = USP_UHF_TNO;
    ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
    if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_DISPATCH_ID);
    }
    /*������Ϣ���ض�ʱ�� �Է�fpga����Ӧ*/
    LLRP_START_TIMER(TFpgaResp, TFpgaRespCount);
}
/**************************************************************************
* �������ƣ�llrp_sendNextEpcInv
* �����������·��������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_sendNextInv(LLRP_tEAirProtocols eProtocolID)
{

    if (eProtocolID == LLRP_AirProtocols_EPCGlobalClass1Gen2)
    {
        /*����EpcС���*/
        llrp_sendNextEpcInv();
    }
    else if (eProtocolID == LLRP_AirProtocols_ISO18000_6B )
    {
        /*����ISO6BС���*/
        llrp_sendNextISO6BInv();
    }
    else if (eProtocolID == LLRP_AirProtocols_GB )
    {
        /*����GBС���*/
        llrp_sendNextGBInv();
    }
    else
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_LLRP_PROTO_ERROR, eProtocolID, 0, 0, RDP_LLRP_EPC_ID);
    }

}

/**************************************************************************
* �������ƣ�llrp_selectNextAI
* ����������ѡ����һ��AI���в���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_RO_DONE: RO�е�����AI����
                             LLRP_AI_CONTINUE_INV AI�������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_selectNextAI()
{
    unsigned int dwRet = LLRP_AI_DONE_INV;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAI *ptRunLlrpAI = NULL;
    T_RdpLlrpProtocolInvSet *ptLlrpProtocolInvSet = NULL;
    T_RdpLlrpAntInv *ptLlrpAntInv = NULL;
    RdpLlrpDbgPrint("(LLRP)select next AI\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return LLRP_RO_DONE_INV;
    }
    ptRunLlrpAI = llrp_getRunAI();
    /*��AI N�γ��Ե�AIָ����һ��AI*/
    ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAntIndex = 0;
    ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkProtocolIndex = 0;
    if (ptRunLlrpAI->tRdpLlrpAIStopTrigger.tRdpLlrpAIParamForStopTrigger.wNumberOfAttempts == 0)
    {
        RdpLlrpDbgPrint("(LLRP)not config wNumberOfAttempts AI index %d is over\r\n", ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex);
        dwRet = LLRP_AI_DONE_INV;
        /*����AI����������*/
        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
        .pfusp_rdp_llrp_EndAIForReportTrigger(&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
        llrp_reportReaderEventNotification(LLRP_NotificationEventType_AISpec_Event , LLRP_AISpecEventType_End_Of_AISpec);
        ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex++;
        /*ת���µ�AI���г�ʼ��AI��stopTrigger������Ro�ı��津������*/
        if (ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex < ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpAISet.wAINum)
        {
            ptRunLlrpAI = llrp_getRunAI();
            ptRunLlrpAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_initAIForAIStopTrigger(&ptRunLlrpAI->tRdpLlrpAIStopTrigger);
        }
    }
    else
    {
        ptRunLlrpAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_AccmulateExecuteTimesForAIStopTrigger(&ptRunLlrpAI->tRdpLlrpAIStopTrigger);
        if (ptRunLlrpAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_judgeHappenForAIStopTrigger(&ptRunLlrpAI->tRdpLlrpAIStopTrigger) == TRUE)
        {
            RdpLlrpDbgPrint("(LLRP)AI stoptigger happened and current AI index %d is over\r\n", ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex);
            dwRet = LLRP_AI_DONE_INV;
            /*����AI����������*/
            ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
            .pfusp_rdp_llrp_EndAIForReportTrigger(&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
            llrp_reportReaderEventNotification(LLRP_NotificationEventType_AISpec_Event , LLRP_AISpecEventType_End_Of_AISpec);
            ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex++;
            /*ת���µ�AI���г�ʼ��AI��stopTrigger������Ro�ı��津������*/

            if (ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex < ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpAISet.wAINum)
            {
                ptRunLlrpAI = llrp_getRunAI();
                ptRunLlrpAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_initAIForAIStopTrigger(&ptRunLlrpAI->tRdpLlrpAIStopTrigger);
            }
        }
        else
        {
            RdpLlrpDbgPrint("(LLRP)AI execute times does not reach ,AI index:%d continue to do\r\n", ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex);
            dwRet = LLRP_AI_CONTINUE_INV;
        }

    }
    if (ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAIIndex == ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpAISet.wAINum)
    {
        /*����RO��������*/
        RdpLlrpDbgPrint("(LLRP)all AI has execued over\r\n");
        llrp_activeROTerminate();
        dwRet = LLRP_RO_DONE_INV;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�llrp_selectNextAP
* ����������ѡ����һ�����ߺ�Э�鹤��
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_AI_DONE: AI��һ��ִ����� 
                            LLRP_AI_CONTINUE_INV AI�������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_selectNextAP()
{
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAI *ptLlrpAI = NULL;
    T_RdpLlrpProtocolInvSet *ptLlrpProtocolInvSet = NULL;
    T_RdpLlrpAntInv *ptLlrpAntInv = NULL;
    RdpLlrpDbgPrint("(LLRP)select next AP\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return LLRP_RO_DONE_INV;
    }
    ptLlrpAI = llrp_getRunAI();
    ptLlrpAntInv = llrp_getRunAnt();
    ptLlrpProtocolInvSet = llrp_getRunAP();
    ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkProtocolIndex++;
    if (ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkProtocolIndex >= ptLlrpAntInv->ucProtocolNum)
    {
        ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkProtocolIndex = 0;
        ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAntIndex++;
        if (ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAntIndex >= ptLlrpAI->tRdpLlrpAntsInvSet.ucAntNum)
        {
            ptRunRoCtrlAndPara->tRdpLlrpActiveROCtrlIndex.ucWorkAntIndex = 0;
            return LLRP_AI_NEXT_INV;
        }
        else
        {
            return LLRP_AP_CONTINUE_INV;
        }
    }
    else
    {
        return LLRP_AP_CONTINUE_INV;
    }
}

/**************************************************************************
* �������ƣ�llrp_activeAITerminate
* ����������AIִ�����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_activeAITerminate()
{
    unsigned int dwROIndex = 0;
    T_RdpLlrpROEntry *ptActiveEntry;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    RdpLlrpDbgPrint("(LLRP)current AI is over\r\n");
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    /*endofRO�ϱ�����Ĵ���*/
    if (llrp_selectNextAI() != LLRP_RO_DONE_INV)
    {
        /*ѡ����һ��AI ִ�� AIִ�г�ʼ��*/
        llrp_tagInventory(ptRunRoCtrlAndPara);
    }
}

/**************************************************************************
* �������ƣ�llrp_dispatchTagInv
* ����������������һ�����ı�ǩ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_AI_DONE: AI��һ��ִ����� 
                             LLRP_AI_CONTINUE_INV AI�������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_dispatchTagInv()
{
    unsigned int dwRet = 0;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAI *ptRunAI = NULL;
    ptRunRoCtrlAndPara = llrp_getRunRO();
    if (ptRunRoCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunAI = llrp_getRunAI();
    if (ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDispatchStatus == LLRP_RODISPATCH_STATE_DELETE)
    {
        /*����delete��Ϣ*/
        llrp_deleteActiveROAndSndRsp();
        return ;
    }
    if (ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDispatchStatus == LLRP_RODISPATCH_STATE_DISABLE)
    {
        /*����disable��Ϣ*/
        llrp_disableActiveROAndSndRsp();
        return ;
    }
    if (ptRunRoCtrlAndPara->tRdpLlrpRODispatchCtrl.dwDispatchStatus == LLRP_RODISPATCH_STATE_INACTIVE)
    {
        /*����stop��Ϣ*/
        llrp_stopActiveRO();
        return ;
    }
    /*�ж�RO��stopTrigger�ķ���*/
    ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStopTrigger.pfusp_rdp_llrp_endofAPDealforStopTrigger(ptRunRoCtrlAndPara);
    if (ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROBoundary.tRdpLlrpROStopTrigger.pfusp_rdp_llrp_judgeHappenforStopTrigger(ptRunRoCtrlAndPara) == TRUE)
    {
        /*stopTrigger ��������ROֹͣ*/
        RdpLlrpDbgPrint("(LLRP)RO stopTrigger has happened\r\n");
        llrp_activeROTerminate();
    }
    else
    {
        /*�ж�AI�Ľ��������Ƿ���*/
        ptRunAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_endAPForAIStopTrigger(&ptRunAI->tRdpLlrpAIStopTrigger);
        if (ptRunAI->tRdpLlrpAIStopTrigger.pfusp_rdp_llrp_judgeHappenForAIStopTrigger(&ptRunAI->tRdpLlrpAIStopTrigger) == TRUE)
        {
            /*AI���������Ĵ���*/
            llrp_activeAITerminate();
        }
        else
        {
            /*ѡȡ��һ��AP���в���*/
            if (llrp_selectNextAP() == LLRP_AP_CONTINUE_INV)
            {
                llrp_tagInventory(ptRunRoCtrlAndPara);
            }
            /*LLRP_AI_NEXT_INV*/
            else
            {
                /*AI���������Ĵ���*/
                llrp_activeAITerminate();
            }
        }
    }
}

/**************************************************************************
* �������ƣ�llrp_tagEpcInvRsp
* ��������������Rcp��EPC��㷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcInvRsp(T_Msg *ptRcvMsg)
{
    unsigned int dwRet = 0;
    T_USPUHFEpcInventoryRes *ptTempTagEpcInvRsp = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAI *ptRdpLlrpAI = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptMatchAccessCtrlAndPara = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcInvRsp\r\n");
    if (g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext == NULL)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, 0, 0, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcInvRsp active ro is null\r\n");
        return ;
    }
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcInvRsp wMsgSequence error\r\n");
        return ;
    }
    /*ɱ��ʱ��*/
    LLRP_STOP_TIMER(TFpgaResp);
    /*��õ�ǰ���е�AI*/
    ptRdpLlrpAI = llrp_getRunAI();
    ptTempTagEpcInvRsp = (T_USPUHFEpcInventoryRes *) (ptRcvMsg + 1);
    /*RO��Report������ ����Trigger�ϱ�*/
    if (ptTempTagEpcInvRsp->wResult == 0)
    {
        /*������㵽��ǩ*/
        RdpLlrpDbgPrint("(LLRP)RCP Fpga Return success \r\n");
        if (ptTempTagEpcInvRsp->wInventoryFinish == 0)
        {
            /*���Trigger�Ѿ����� �˳����� ��ȡ���Ժ�ı�ǩ*/
            RdpLlrpDbgPrint("(LLRP)RCP Fpga  Return an Tag\r\n");
            ptRdpLlrpAI->tRdpLlrpAIStopTrigger \
            .pfusp_rdp_llrp_AccmulateForAIStopTrigger((void *)&ptRdpLlrpAI->tRdpLlrpAIStopTrigger);
            if ( ptRdpLlrpAI->tRdpLlrpAIStopTrigger \
                    .pfusp_rdp_llrp_judgeHappenForAIStopTrigger((void *)&ptRdpLlrpAI->tRdpLlrpAIStopTrigger) == TRUE )
            {
                RdpLlrpDbgPrint("(LLRP)active AI stoptrigger happen need not to save tag\r\n");
                llrp_sendNextEpcInv();
            }
            else
            {
                /*access����������γ����趨Ϊ���ʴ���*/
                if (g_dwLlrpTagAccessErrTimes >= LLRP_MAX_ACCESS_ERROR_TIMES)
                {
                    g_dwLlrpTagAccessErrTimes = LLRP_INIT_ACCESS_ERROR_TIMES;
                    RdpLlrpDbgPrint("(LLRP)tag access error for three times, throw away the tag and continue inv\r\n");
                    llrp_sendNextEpcInv();
                }
                else
                {
                    /*ƥ��,�ɹ��˳������ɹ����챨��*/
                    llrp_clearEpcAccessFlag();
                    dwRet = llrp_matchAccess(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                             ptTempTagEpcInvRsp->wAntennaIndex,
                                             &ptTempTagEpcInvRsp->tTagId);
                    if ( dwRet == LLRP_ACCESS_MATCH_FAIL )
                    {
                        /*���챨��*/
                        RdpLlrpDbgPrint("(LLRP)match Access fail,need to build an Ro Report and insert active Ro report link\r\n");
                        llrp_constructLocalROReport(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                                    ptTempTagEpcInvRsp->wAntennaIndex,
                                                    ptTempTagEpcInvRsp->wRssi,
                                                    ptTempTagEpcInvRsp->wPC,
                                                    ptTempTagEpcInvRsp->wCRC,
                                                    ptTempTagEpcInvRsp->wChannelIndex,
                                                    ptTempTagEpcInvRsp->tTagId.wTagWordLen,
                                                    ptTempTagEpcInvRsp->tTagId.aucTagData);
                        ptRunRoCtrlAndPara = llrp_getRunRO();
                        if (ptRunRoCtrlAndPara == NULL)
                        {
                            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
                            return ;
                        }
                        /*���ӱ�����ۺ��жϱ����Ƿ��ϱ�*/
                        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
                        .pfusp_rdp_llrp_JudgeReportNumForReportTrigger((void *)&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
                        llrp_sendNextEpcInv();
                    }
                    else
                    {
                        RdpLlrpDbgPrint("(LLRP)match Access success,inventory over begin do Access Op\r\n");
                        ptMatchAccessCtrlAndPara = llrp_getRunAccess();
                        if (ptMatchAccessCtrlAndPara == NULL)
                        {
                            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
                            return ;
                        }
                        /*��ʼ��Ao��stoptrigger */
                        ptMatchAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessStopTrigger \
                        .pfusp_rdp_llrp_initAOStopTrigger(ptMatchAccessCtrlAndPara);
                        /*����ƥ��ı�ǩ*/
                        ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen = ptTempTagEpcInvRsp->tTagId.wTagWordLen * sizeof(unsigned short int);
                        memcpy(ptMatchAccessCtrlAndPara->tMatchUID.aucTagData,
                               ptTempTagEpcInvRsp->tTagId.aucTagData, ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen);
                        /*�·�ָ��*/
                        llrp_nextTagAccessOp(&ptMatchAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet,
                                             LLRP_AirProtocols_EPCGlobalClass1Gen2, &ptMatchAccessCtrlAndPara->tMatchUID);
                    }
                }
            }
        }
        /*�������*/
        else
        {
            RdpLlrpDbgPrint("(LLRP)RCP Fpga  Return epc finish frame \r\n");
            /*һ����������access���ʴ������Ϊ0*/
            g_dwLlrpTagAccessErrTimes = LLRP_INIT_ACCESS_ERROR_TIMES;
            /*��Ӧ��Ӧ�Ľ�������Ȼ�����Ro,AI,AP�ĵ���*/
            llrp_dispatchTagInv();
        } /*���������*/
    }
    /*ʧ�ܽ��������������һ����*/
    else
    {
        RdpLlrpDbgPrint("(LLRP)RCP Fpga Return epc failed frame\r\n");
        g_dwLlrpTagAccessErrTimes = LLRP_INIT_ACCESS_ERROR_TIMES;
        if (ptTempTagEpcInvRsp->wInventoryFinish == 0)
        {
            RdpLlrpDbgPrint("(LLRP)Rcp Fpga return Epc failed and need to  continue epc inventory \r\n");
            llrp_sendNextEpcInv();
        }
        else
        {
            RdpLlrpDbgPrint("(LLRP)Rcp Fpga return Epc failed and need to continue a big epc inventory \r\n");
            llrp_dispatchTagInv();
        }
    }
    /*RO��Report���� ֪��*/
}
/**************************************************************************
* �������ƣ�llrp_changeRcpResultToLlrpForEpcRead
* ����������ת��Rcp�����뵽Llrp��׼
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int *pwResult
                             Rcp�Ĵ�����
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_changeRcpResultToLlrpForEpcRead(unsigned short int *pwResult)
{
    unsigned short int wResult = 0;
    wResult = *pwResult;

    switch (wResult)
    {
    case 0:
        {
            *pwResult = LLRP_C1G2ReadResultType_Success;
            break;
        }
    case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:

        {
            *pwResult = LLRP_C1G2ReadResultType_Nonspecific_Reader_Error;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
        {
            /*������ط��ʴ�����Ѵ���Ĵ�����1*/
            g_dwLlrpTagAccessErrTimes++;
            *pwResult = LLRP_C1G2ReadResultType_Nonspecific_Tag_Error;
            break;
        }

    default:
        {
            *pwResult = LLRP_C1G2ReadResultType_No_Response_From_Tag;
            RdpLlrpDbgPrint("(LLRP)There is not error code could be express\r\n");
            break;
        }
    }

    RdpLlrpDbgPrint("(LLRP)changeResult_RcpToLlrp_EpcRead wResult : %d\r\n", *pwResult);
}
/**************************************************************************
* �������ƣ�llrp_getRunListOpSet
* ������������������ִ�е�op����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
T_RdpLlrpListOPCtrlSet *llrp_getRunListOpSet()
{
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return NULL;
    }
    if (ptRunAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.bClientRequest == TRUE)
    {
        RdpLlrpDbgPrint("(LLRP)the access is doing clientrequest need continue to do\r\n");
        return &g_tClientRequestOpSet.tListOp;
    }
    else
    {
        //RdpLlrpDbgPrint("(LLRP)the access is doing access op need continue to do\r\n");
        return &ptRunAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet;
    }
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcReadOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcReadOpResult(T_USPUHFEpcReadRes *ptRcpEpcReadRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpC1G2Read *ptEpcReadOp = NULL;
    T_RdpLlrpEpcReadOpResult *ptEpcReadOpResult = NULL;
    if (ptRcpEpcReadRes->wResultLength > 500)
    {
        ptRcpEpcReadRes->wResultLength = 32;
        RDP_EXCEPT(RDP_INVALID_STATE, ptRcpEpcReadRes->wResultLength, 0, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)epc read return an error len =%d\r\n", ptRcpEpcReadRes->wResultLength);
    }
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcReadOp = (T_RdpLlrpC1G2Read *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    ptEpcReadOpResult = (T_RdpLlrpEpcReadOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];
    ptEpcReadOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcReadOpResult->eResult = ptRcpEpcReadRes->wResult;
    ptEpcReadOpResult->wDataLength = ptRcpEpcReadRes->wResultLength;
    memcpy(ptEpcReadOpResult->aucData, ptRcpEpcReadRes->awReadData, ptRcpEpcReadRes->wResultLength * sizeof(unsigned short int));
    /*ȡ��Epc������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcReadRes->wResult;
    /*���¼��������ֱ��¼ԭʼ�����ȣ���ʼ��ַ�����滺���ַ���������ʵ���ȵ�
       ǰ���������ǲ�������ʹ�õĽ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptEpcReadOp->wWordCount;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptEpcReadOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen
    = struct_fixed_len(T_RdpLlrpEpcReadOpResult, aucData) + ptEpcReadOp->wWordCount * sizeof(unsigned short int);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)epc Read Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)MsgLen: %d\r\n", ptRunSingleReportCtrl->wMsgLen);
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcReadOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcReadOpResult->eResult);
    RdpLlrpDbgPrint("(LLRP)DataLength: %d\r\n", ptEpcReadOpResult->wDataLength);
    RdpLlrpDbgPrint("(LLRP)Epc read data:");
    for (dwTestIndex = 0 ;dwTestIndex < ptEpcReadOpResult->wDataLength*sizeof(unsigned short int);dwTestIndex++)
    {
        RdpDbgPrintRaw("0x%02x ", ptEpcReadOpResult->aucData[dwTestIndex]);
    }
    RdpLlrpDbgPrint("\r\n");
    if (ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType == LLRP_C1G2ReadCustomUID)
    {
        RdpLlrpDbgPrint("read customer uid need cpy read Result to UID area\r\n");
        if (ptEpcReadOpResult->eResult == 0)
        {
            ptRunSingleReportCtrl->tUID.wTagDataLen = ptEpcReadOpResult->wDataLength * sizeof(unsigned short int);
            memcpy(ptRunSingleReportCtrl->tUID.aucTagData, ptEpcReadOpResult->aucData, ptEpcReadOpResult->wDataLength*sizeof(unsigned short int));
        }
        else
        {
            ptRunSingleReportCtrl->tUID.wTagDataLen = 0;
        }
    }

}
/**************************************************************************
* �������ƣ�llrp_deleteActiveAccess
* ����������ɾ��ActiveAccess��Ĳ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_deleteActiveAccess()
{
    unsigned int dwRet = 0;
    T_Msg *ptSndMsg = (T_Msg *) g_aucRdpLlrpSndMsg;
    T_RdpLlrpRuleRsp *ptRdpLlrpRuleRsp = NULL;
    unsigned int dwROIndex = 0;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    /*endofRO�ϱ�����Ĵ���*/
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    /*���Report�ı���Ļ���*/
    memset(g_ucLocalReportDataBuffer, 0, sizeof(g_ucLocalReportDataBuffer));
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tEnableAccessQueue,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_clearAccessListforGetAccessRsp(0);
    llrp_deleteAnNodeForDynamicAccessQueue(&g_tRdpLlrpDynamicAccessLinkHead[ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkIndex], ptRunAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID);
    RdpLlrpDbgPrint("(LLRP) send EV_RDP_DELETE_OPERATION_COUNT_ACCESS msg to Rule\r\n");
    ptSndMsg->mtype = LLRP_SYN_MSG;
    ptSndMsg->ucLinkIndex = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkIndex;
    ptSndMsg->ucLinkType = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkType;
    /*����wSenderTno��wReceiverTno��ʶ·�ɵ�Դ��Ŀ��
     wSrcTno��wDstTno ��ʾ������ת�Ľڵ�*/
    ptSndMsg->wSenderTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wSenderTno;
    ptSndMsg->wReceiverTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wReceiverTno;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_RDP_RULE_TNO;
    ptSndMsg->wMsgID = EV_RDP_DELETE_OPERATION_COUNT_ACCESS;
    /*������ǵ�ַָ�� ����������ָ����ռ��ַ����*/
    ptSndMsg->wMsgLen = sizeof(unsigned int);
    ptRdpLlrpRuleRsp = (T_RdpLlrpRuleRsp *)(ptSndMsg + 1);
    ptRdpLlrpRuleRsp->dwRuleID = ptRunAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID;
    ptRdpLlrpRuleRsp->ptRuleRspAddr = NULL;
    dwRet = RDP_SEND_MSG(pair_socket_rule, ptSndMsg);
    if (dwRet < 0)
    {
        RdpLlrpDbgPrint("(LLRP)msgsend error\r\n");
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, dwRet, 0, 0, RDP_LLRP_EPC_ID);
    }
    memset(ptRunAccessCtrlAndPara, 0, sizeof(T_RdpLlrpAccessCtrlAndParam));
}

/**************************************************************************
* �������ƣ�llrp_deleteActiveAccessAndSndRsp
* ����������ɾ��ActiveAccess��Ĳ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_deleteActiveAccessAndSndRsp()
{
    T_Msg tMsg;
    unsigned int dwROIndex = 0;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    /*endofRO�ϱ�����Ĵ���*/
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    /*���Report�ı���Ļ���*/
    memset(g_ucLocalReportDataBuffer, 0, sizeof(g_ucLocalReportDataBuffer));

    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tAddAccessQueue,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tEnableAccessQueue,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    tMsg.mtype = 1;
    tMsg.ucLinkIndex = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkIndex;
    tMsg.ucLinkType = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkType;
    tMsg.wReceiverTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wReceiverTno;
    tMsg.wSenderTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wSenderTno;
    llrp_sendRuleRspMsg(&tMsg, LLRP_StatusCode_M_Success, EV_DELETE_ACCESSSPEC_RESPONSE,
                        ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDeleteAccessMsgID, ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDeleteAccessID);
    memset(ptRunAccessCtrlAndPara, 0, sizeof(T_RdpLlrpAccessCtrlAndParam));
}
/**************************************************************************
* �������ƣ�llrp_disableActiveAccessAndSndRsp
* ������������ֹActiveAccess�Ĳ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_disableActiveAccessAndSndRsp()
{
    T_Msg tMsg;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    /*�������*/
    memset(g_ucLocalReportDataBuffer, 0, sizeof(g_ucLocalReportDataBuffer));
    ptRunAccessCtrlAndPara->tRdpLlrpAccess.eCurrentState = LLRP_AccessSpecState_Disabled;
    /*activeRoת����disable������*/
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tEnableAccessQueue,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
    tMsg.mtype = 1;
    tMsg.ucLinkIndex = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkIndex;
    tMsg.ucLinkType = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.ucLinkType;
    tMsg.wReceiverTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wReceiverTno;
    tMsg.wSenderTno = ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.wSenderTno;
    llrp_sendRuleRspMsg(&tMsg, LLRP_StatusCode_M_Success, EV_DISABLE_ACCESSSPEC_RESPONSE,
                        ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDisableAccessMsgID, ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDisableAccessID);

}

/**************************************************************************
* �������ƣ�llrp_endActiveAccess
* ����������Access��������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_endActiveAccess()
{
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDispatchStatus = LLRP_ACCESSDISPATCH_STATE_ENABLE;
    llrp_findAndFreeAnNodeFoAllQueueOfAccess(&g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess,
            g_tRdpLlrpReaderDispatch.ptReaderAccessSpecData->tRunAccess.ptNext->dwAccessIndex);
}
/**************************************************************************
* �������ƣ�llrp_dispatchTagAccess
* ��������������Access�ķ��ʲ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_dispatchTagAccess()
{
    T_RdpLlrpAccessCtrlAndParam *ptRunAccessCtrlAndPara = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpCtrlSet = NULL;
    LLRP_tEAirProtocols eCurProtocolID;
    ptRunAccessCtrlAndPara = llrp_getRunAccess();
    if (ptRunAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    eCurProtocolID = ptRunAccessCtrlAndPara->tRdpLlrpAccess.eProtocolID;
    ptRunListOpCtrlSet = llrp_getRunListOpSet();
    RdpLlrpDbgPrint("(LLRP)ptRunListOpCtrlSet->wOpIndex: %d,ptRunListOpCtrlSet->wOPNum:%d\r\n",
                    ptRunListOpCtrlSet->wOpIndex, ptRunListOpCtrlSet->wOPNum);
    if (ptRunListOpCtrlSet->wOpIndex == ptRunListOpCtrlSet->wOPNum)
    {
        if (ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDispatchStatus == LLRP_ACCESSDISPATCH_STATE_DELETE)
        {
            /*�յ�deleteAO��Ϣ*/
            RdpLlrpDbgPrint("(LLRP)llrp has already received an delete active ao msg ,need to delete active ao\r\n");
            llrp_deleteActiveAccessAndSndRsp();
        }
        else if (ptRunAccessCtrlAndPara->tRdpLlrpAccessDispatchCtrl.dwDispatchStatus == LLRP_ACCESSDISPATCH_STATE_DISABLE)
        {
            /*�յ�Disable��Ϣ*/
            RdpLlrpDbgPrint("(LLRP)llrp has already received an disable active ao msg ,need to disable active ao\r\n");
            llrp_disableActiveAccessAndSndRsp();
        }
        else
        {
            ptRunAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessReport \
            .pfusp_rdp_llrp_sendOneAOReportForAOReportTrigger((T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer);
            /*����RO�����������ж��Ƿ�ﵽ�ϱ����� */
            if ( ptRunAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessStopTrigger \
                    .pfusp_rdp_llrp_IncreaseConditionAndJudeEndForAOStopTrigger(&ptRunAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessStopTrigger)
                    == LLRP_ACCESS_DELETE)
            {
                /*DeleteAccess����*/
                RdpLlrpDbgPrint("(LLRP)access run times reach ,need to delete access\r\n");
                llrp_deleteActiveAccess();
            }
            else
            {
                /*access��������û��op����ʼ���*/
                RdpLlrpDbgPrint("(LLRP)all access op done start inv\r\n");
                llrp_endActiveAccess();
            }
        }
        /*���챨����߲��뵽Ro������*/
        llrp_sendNextInv(eCurProtocolID);
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)select next op to do\r\n");
        llrp_nextTagAccessOp(ptRunListOpCtrlSet, ptRunAccessCtrlAndPara->tRdpLlrpAccess.eProtocolID, &ptRunAccessCtrlAndPara->tMatchUID);
    }

}

/**************************************************************************
* �������ƣ�llrp_tagEpcReadRsp
* ��������������Rcp��EPC��㷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcReadRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcReadOpResult *ptEpcReadOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcReadRes *ptEPC_ReadResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcReadRsp\r\n");
    /*�ǵ�һ������*/
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcReadRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaReadResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_ReadResponse = (T_USPUHFEpcReadRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcRead(&ptEPC_ReadResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_ReadResponse->wAntennaIndex,
                                        ptEPC_ReadResponse->wRssi,
                                        0xffff,
                                        0xffff,
                                        ptEPC_ReadResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcReadOpResult(ptEPC_ReadResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcReadOpResult(ptEPC_ReadResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcReadOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcVeriyReadOpResult(T_USPUHFEpcReadRes *ptRcpEpcReadRes)
{
    T_Msg tSyncMsg;
    T_Msg *ptSyncMsg[] = {&tSyncMsg, NULL};
    int dwRet = 0;
    unsigned char ucRevBuf[8192] = {0};
    T_Msg *ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    T_Msg *ptRcvMsg = (T_Msg *)ucRevBuf;
    T_USPUHFEpcWrite *ptUSPUHFEpcWrite = NULL;
    T_USPUHFEpcWriteRes *ptUSPUHFEpcWriteRes = NULL;
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpAntInv *ptRunLlrpAntInv = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpC1G2Read *ptEpcReadOp = NULL;
    T_RdpLlrpEpcWriteVerifyResult *ptEpcWriteVerifyResult = NULL;
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunListOpSet = llrp_getRunListOpSet();
    ptRunLlrpAntInv = llrp_getRunAnt();
    ptEpcReadOp = (T_RdpLlrpC1G2Read *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    if ( ptRcpEpcReadRes->wResult == 0 )
    {
        RdpLlrpDbgPrint("(LLRP) Rcp return read success need send write msg\r\n");
        ptSndMsg->mtype = 1;
        ptSndMsg->wMsgID = EV_USP_UHF_EPC_VERIFY_WRITE;
        ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wDstTno = USP_UHF_TNO;
        ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wReceiverTno = USP_UHF_TNO;
        ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
        ptUSPUHFEpcWrite = (T_USPUHFEpcWrite *)(ptSndMsg + 1);
        ptUSPUHFEpcWrite->tEpcWritePara.dwAccessPassword = ptEpcReadOp->dwAccessPassword;
        ptUSPUHFEpcWrite->tEpcWritePara.wWordPointer = ptEpcReadOp->wWordPointer;
        ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen = ptEpcReadOp->wWordCount;
        ptUSPUHFEpcWrite->dwAntIndex = ptRunLlrpAntInv->ucAntIndex;
        ptUSPUHFEpcWrite->tEpcWritePara.dwUhfBitCommand = 0;
        ptUSPUHFEpcWrite->tEpcWritePara.ucWriteType = 0;
        memcpy(ptUSPUHFEpcWrite->tEpcWritePara.aucData, ptRcpEpcReadRes->awReadData, ptRcpEpcReadRes->wResultLength*sizeof(unsigned short int));
        ptUSPUHFEpcWrite->tTagId.wTagWordLen = ptRunAccess->tMatchUID.wTagDataLen / sizeof(unsigned short int);
        memcpy(ptUSPUHFEpcWrite->tTagId.aucTagData, ptRunAccess->tMatchUID.aucTagData, ptRunAccess->tMatchUID.wTagDataLen);
        ptSndMsg->wMsgLen = struct_fixed_len(T_USPUHFEpcWrite, tEpcWritePara)
                            + struct_fixed_len(T_USPUHFEpcWritePara, aucData)
                            + ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen * sizeof(unsigned short int)
                            + struct_fixed_len(T_USPUHFEpcTagId, aucTagData)
                            + ptUSPUHFEpcWrite->tTagId.wTagWordLen * sizeof(unsigned short int);
        RdpLlrpDbgPrint("dwAntIndex:%d\r\n", ptUSPUHFEpcWrite->dwAntIndex);
        RdpLlrpDbgPrint("dwAccessPassword:%d\r\n", ptUSPUHFEpcWrite->tEpcWritePara.dwAccessPassword);
        RdpLlrpDbgPrint("ucMB:%d\r\n", ptUSPUHFEpcWrite->tEpcWritePara.ucMB);
        RdpLlrpDbgPrint("wWordPointer:%d\r\n", ptUSPUHFEpcWrite->tEpcWritePara.wWordPointer);
        RdpLlrpDbgPrint("wWriteWordLen:%d\r\n", ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen);
        RdpLlrpDbgPrint("ucWriteType:%d\r\n", ptUSPUHFEpcWrite->tEpcWritePara.ucWriteType);
        RdpLlrpDbgPrint("write data:");
        for (dwTestIndex = 0;dwTestIndex < ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen;dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x ", ptUSPUHFEpcWrite->tEpcWritePara.aucData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        RdpLlrpDbgPrint("TagId data:");
        for (dwTestIndex = 0;dwTestIndex < ptUSPUHFEpcWrite->tTagId.wTagWordLen;dwTestIndex++)
        {
            RdpDbgPrintRaw("%04x ", ptUSPUHFEpcWrite->tTagId.aucTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        memset(&tSyncMsg, 0xff, sizeof(T_Msg));
        tSyncMsg.wMsgID = EV_USP_UHF_EPC_VERIFY_WRITE_RES;
        tSyncMsg.wMsgSequence = g_wMsgSequence;
        if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
        {
            /*����ʧ��д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
            RdpLlrpDbgPrint("Llrp send msg error\r\n");
            dwRet = RDP_SEND_MESSGAE_ERROR;
        }
        else
        {
            dwRet = RDP_RECV_SYNC_MSG(req_socket_rcp, ptRcvMsg, RDP_MAX_LEN_MSG);
            if (dwRet < 0 )
            {
                /*д���쳣*/
                RDP_EXCEPT(RDP_RECV_MESSGAE_ERROR, ptRcvMsg->wMsgID, ptRcvMsg->wMsgLen, 0, RDP_LLRP_EPC_ID);
                RdpLlrpDbgPrint("Llrp rcv msg error\r\n");
                dwRet = RDP_RECV_MESSGAE_ERROR;
            }

        }
        ptUSPUHFEpcWriteRes = (T_USPUHFEpcWriteRes *)(ptRcvMsg + 1);
        if (ptUSPUHFEpcWriteRes->wResult != 0)
        {
            RdpLlrpDbgPrint("(LLRP) Rcp return Write failed \r\n");
            dwRet = RDP_LLRP_VERIYREAD_ERROR;
        }
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP) Rcp return read failed \r\n");
        dwRet = RDP_LLRP_VERIYREAD_ERROR;
    }
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;

    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptEpcWriteVerifyResult = (T_RdpLlrpEpcWriteVerifyResult *) & ptAccessTagReport->aucData[dwOpResultAddr];
    ptEpcWriteVerifyResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    if ( dwRet == 0 )
    {
        ptEpcWriteVerifyResult->dwResult = 0;
    }
    else
    {
        ptEpcWriteVerifyResult->dwResult = 1;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcReadRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptEpcReadOp->wWordCount;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptEpcReadOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcWriteVerifyResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
}
/**************************************************************************
* �������ƣ�llrp_tagEpcVeriyReadRsp
* ��������������Rcp��EPC��У�鷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcVeriyReadRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcReadOpResult *ptEpcReadOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcReadRes *ptEPC_ReadResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcVeriyReadRsp\r\n");
    /*�ǵ�һ������*/
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcReadRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaReadResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_ReadResponse = (T_USPUHFEpcReadRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcRead(&ptEPC_ReadResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_ReadResponse->wAntennaIndex,
                                        ptEPC_ReadResponse->wRssi,
                                        0xffff,
                                        0xffff,
                                        ptEPC_ReadResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcVeriyReadOpResult(ptEPC_ReadResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcVeriyReadOpResult(ptEPC_ReadResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}

/**************************************************************************
* �������ƣ�llrp_changeRcpResultToLlrpForEpcWrite
* ����������ת��Rcp�����뵽Llrp��׼
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int *pwResult
                             Rcp�Ĵ�����
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_changeRcpResultToLlrpForEpcWrite(unsigned short int *pwResult)
{
    unsigned short int wResult = 0;
    wResult = *pwResult;
    switch (wResult)
    {
    case 0:
        {
            *pwResult = LLRP_C1G2WriteResultType_Success;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_TAG_MEM_LOCATION_NOTEXIST:
        {
            *pwResult = LLRP_C1G2WriteResultType_Tag_Memory_Overrun_Error;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
        {
            /*������ط��ʴ�����Ѵ���Ĵ�����1*/
            g_dwLlrpTagAccessErrTimes++;
            *pwResult = LLRP_C1G2WriteResultType_Tag_Memory_Locked_Error;
            break;
        }

    case(unsigned short int)USP_UHF_EPC_TAG_POWER_NOT_ENOUGH:
        {
            *pwResult = LLRP_C1G2WriteResultType_Insufficient_Power;
            break;
        }
    case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:
        {
            *pwResult = LLRP_C1G2WriteResultType_Nonspecific_Reader_Error;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_TAG_UKNOWN_ERROR:
        {
            *pwResult = LLRP_C1G2WriteResultType_Nonspecific_Tag_Error;
            break;
        }
    default:
        {
            *pwResult = LLRP_C1G2WriteResultType_No_Response_From_Tag;

            RdpLlrpDbgPrint("(LLRP)There is not error code could be express\r\n");
            break;
        }
    }
    RdpLlrpDbgPrint("(LLRP)in ChangeResult_RcpToLlrp_EpcWrite *pwResult = %d\r\n", *pwResult);
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcWriteOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcWriteOpResult(T_USPUHFEpcWriteRes *ptRcpEpcWriteRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpC1G2Write *ptEpcWriteOp = NULL;
    T_RdpLlrpEpcWriteOpResult *ptEpcWriteOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcWriteOp = (T_RdpLlrpC1G2Write *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    ptEpcWriteOpResult = (T_RdpLlrpEpcWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptEpcWriteOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcWriteOpResult->eResult = ptRcpEpcWriteRes->wResult;
    ptEpcWriteOpResult->wNumWordsWritten = ptRcpEpcWriteRes->wResultLength;
    /*ȡ��Epc������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcWriteRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptEpcWriteOp->wDataLength;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptEpcWriteOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcWriteOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)epc Write Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcWriteOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcWriteOpResult->eResult);
    RdpLlrpDbgPrint("(LLRP)NumWordsWritten: %d\r\n", ptEpcWriteOpResult->wNumWordsWritten);
}

/**************************************************************************
* �������ƣ�llrp_tagEpcWriteRsp
* ��������������Rcp��EPC��㷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcWriteRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcWriteOpResult *ptEpcWriteOpResult = NULL;
    T_USPUHFEpcWriteRes *ptEPC_WriteResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcWriteRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcWriteRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaWriteResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_WriteResponse = (T_USPUHFEpcWriteRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcWrite(&ptEPC_WriteResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_WriteResponse->wAntennaIndex,
                                        ptEPC_WriteResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_WriteResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcWriteOpResult(ptEPC_WriteResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcWriteOpResult(ptEPC_WriteResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_changeRcpResultToLlrpForEpcLock
* ����������ת��Rcp�����뵽Llrp��׼
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int *pwResult
                             Rcp�Ĵ�����
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_changeRcpResultToLlrpForEpcLock(unsigned short int *pwResult)
{
    unsigned short int wResult = 0;
    wResult = *pwResult;
    switch (wResult)
    {
    case 0:
        {
            *pwResult = LLRP_C1G2LockResultType_Success;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_TAG_POWER_NOT_ENOUGH:
        {
            *pwResult = LLRP_C1G2LockResultType_Insufficient_Power;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
        {
            /*������ط��ʴ�����Ѵ���Ĵ�����1*/
            g_dwLlrpTagAccessErrTimes++;
            *pwResult = LLRP_C1G2LockResultType_Nonspecific_Tag_Error;
            break;
        }
    case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:
        {
            *pwResult = LLRP_C1G2LockResultType_Nonspecific_Reader_Error;
            break;
        }
    default:
        {
            *pwResult = LLRP_C1G2LockResultType_No_Response_From_Tag;
            RdpLlrpDbgPrint("(LLRP)There is not error code could be expressrn");
            break;
        }
    }
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcLockOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcLockOpResult(T_USPUHFEpcLockRes *ptRcpEpcLockRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpEpcLockOpResult *ptEpcLockOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcLockOpResult = (T_RdpLlrpEpcLockOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptEpcLockOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcLockOpResult->eResult = ptRcpEpcLockRes->wResult;
    /*ȡ��Epc������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcLockRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcLockOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)epc Lock Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcLockOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcLockOpResult->eResult);
}
/**************************************************************************
* �������ƣ�llrp_tagEpcLockRsp
* ��������������Rcp����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcLockRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcLockOpResult *ptEpcLockOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcLockRes *ptEPC_LockResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcLockRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcLockRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaLockResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_LockResponse = (T_USPUHFEpcLockRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcLock(&ptEPC_LockResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_LockResponse->wAntennaIndex,
                                        ptEPC_LockResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_LockResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcLockOpResult(ptEPC_LockResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcLockOpResult(ptEPC_LockResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_changeRcpResultToLlrpForEpcKill
* ����������ת��Rcp�����뵽Llrp��׼
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int *pwResult
                             Rcp�Ĵ�����
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_changeRcpResultToLlrpForEpcKill(unsigned short int *pwResult)
{
    unsigned short int wResult = 0;
    wResult = *pwResult;
    switch (wResult)
    {
    case 0:
        {
            *pwResult = LLRP_C1G2ReadResultType_Success;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
        {
            /*������ط��ʴ�����Ѵ���Ĵ�����1*/
            g_dwLlrpTagAccessErrTimes ++;
            *pwResult = LLRP_C1G2KillResultType_Nonspecific_Tag_Error;
            break;
        }
    case(unsigned short int)USP_UHF_EPC_TAG_POWER_NOT_ENOUGH:
        {
            *pwResult = LLRP_C1G2KillResultType_Insufficient_Power;
            break;
        }
    case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:
        {
            *pwResult = LLRP_C1G2KillResultType_Nonspecific_Reader_Error;
            break;
        }
    default:
        {
            *pwResult = LLRP_C1G2KillResultType_No_Response_From_Tag;
            RdpLlrpDbgPrint("(LLRP)There is not error code could be expressrn");
            break;
        }
    }

    RdpLlrpDbgPrint("(LLRP)in ChangeResult_RcpToLlrp_EpcKill *pwResult = %d\r\n", *pwResult);
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcKillOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcKillOpResult(T_USPUHFEpcKillRes *ptRcpEpcKillRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpEpcKillOpResult *ptEpcKillOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcKillOpResult = (T_RdpLlrpEpcKillOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptEpcKillOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcKillOpResult->eResult = ptRcpEpcKillRes->wResult;
    /*ȡ��Epc������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcKillRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcKillOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)epc Kill Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcKillOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcKillOpResult->eResult);
}
/**************************************************************************
* �������ƣ�llrp_tagEpcKillRsp
* ��������������Rcpɱ����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcKillRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcKillOpResult *ptEpcKillOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcKillRes *ptEPC_KillResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcKillRsp\r\n");
    /* �쳣������ô��������*/

    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcKillRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaKillResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_KillResponse = (T_USPUHFEpcKillRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcKill(&ptEPC_KillResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_KillResponse->wAntennaIndex,
                                        ptEPC_KillResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_KillResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcKillOpResult(ptEPC_KillResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcKillOpResult(ptEPC_KillResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcBlockLockOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcBlockLockOpResult(T_USPUHFEpcBlockLockRes *ptRcpEpcBlockLockRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpEpcBlcokLockOpResult *ptEpcBlockLockOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcBlockLockOpResult = (T_RdpLlrpEpcBlcokLockOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptEpcBlockLockOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcBlockLockOpResult->wReadLock = ptRcpEpcBlockLockRes->ucReadLock;
    ptEpcBlockLockOpResult->dwResult = ptRcpEpcBlockLockRes->wResult;
    RdpLlrpDbgPrint("(LLRP)epc BlockLock Rsp result:\r\n");
    if (ptEpcBlockLockOpResult->dwResult == 0)
    {
        ptEpcBlockLockOpResult->wReadLock = ptRcpEpcBlockLockRes->ucReadLock;
        /*����ѯ �Ѳ�ѯ�����ݷ���*/
        if (ptEpcBlockLockOpResult->wReadLock == 0)
        {
            ptEpcBlockLockOpResult->wLockStateLen = ptRcpEpcBlockLockRes->wLockStateLen;
            memcpy(ptEpcBlockLockOpResult->ucLockState, ptRcpEpcBlockLockRes->ucLockState,
                   ptEpcBlockLockOpResult->wLockStateLen);
            ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen
            = struct_fixed_len(T_RdpLlrpEpcBlcokLockOpResult, ucLockState) + ptEpcBlockLockOpResult->wLockStateLen;
            RdpLlrpDbgPrint("(LLRP)lockstate:");
            for (dwTestIndex = 0;dwTestIndex < ptEpcBlockLockOpResult->wLockStateLen;dwTestIndex++)
            {
                RdpDbgPrintRaw("0x%2x ", ptEpcBlockLockOpResult->ucLockState[dwTestIndex]);
            }
            RdpLlrpDbgPrint("\r\n");
        }
        else
        {
            ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcBlcokLockOpResult);
        }
    }
    else
    {
        ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcBlcokLockOpResult);
    }
    /*ȡ��Epc������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcBlockLockRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcBlockLockOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcBlockLockOpResult->dwResult);
    RdpLlrpDbgPrint("(LLRP)LockStateLen: %d\r\n", ptEpcBlockLockOpResult->wLockStateLen);
    RdpLlrpDbgPrint("(LLRP)ReadLock: %d\r\n", ptEpcBlockLockOpResult->wReadLock);
}
/**************************************************************************
* �������ƣ�llrp_tagEpcBlockLockRsp
* ��������������Rcpɱ����������Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcBlockLockRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcBlcokLockOpResult *ptEpcBlockLockOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcBlockLockRes *ptEPC_BlockLockResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcBlockLockRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcBlockLockRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaBlockLockResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_BlockLockResponse = (T_USPUHFEpcBlockLockRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcLock(&ptEPC_BlockLockResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_BlockLockResponse->wAntennaIndex,
                                        ptEPC_BlockLockResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_BlockLockResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcBlockLockOpResult(ptEPC_BlockLockResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcBlockLockOpResult(ptEPC_BlockLockResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}

/**************************************************************************
* �������ƣ�llrp_refreshEpcReadLockOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcReadLockOpResult(T_USPUHFEpcReadLockRes *ptRcpEpcReadLockRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpEpcAlienReadLockOpResult *ptEpcReadLockOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptEpcReadLockOpResult = (T_RdpLlrpEpcAlienReadLockOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptEpcReadLockOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptEpcReadLockOpResult->dwResult = ptRcpEpcReadLockRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcAlienReadLockOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcReadLockRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)epc ReadLock Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptEpcReadLockOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcReadLockOpResult->dwResult);
}

/**************************************************************************
* �������ƣ�llrp_tagEpcReadLockRsp
* ��������������Rcpɱ����������Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcReadLockRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpEpcAlienReadLockOpResult *ptEpcReadLockOpResult = NULL;
    T_USPUHFEpcReadLockRes *ptEPC_ReadLockResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcReadLockRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcReadLockRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaAlienReadLockResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_ReadLockResponse = (T_USPUHFEpcReadLockRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcLock(&ptEPC_ReadLockResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_ReadLockResponse->wAntennaIndex,
                                        ptEPC_ReadLockResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_ReadLockResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcReadLockOpResult(ptEPC_ReadLockResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcReadLockOpResult(ptEPC_ReadLockResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_changeRcpResultToLlrpForEpcIPJQT
* ����������ת��Rcp�����뵽Llrp��׼
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int *pwResult,
                             Rcp������
                             unsigned short int *pwReadOrWrite
                             ��inpinjget����set
                             
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_changeRcpResultToLlrpForEpcIPJQT(unsigned short int *pwResult, unsigned short int wReadOrWrite)
{
    unsigned short int wResult = 0;
    wResult = *pwResult;
    if (0 == wReadOrWrite)
    {
        switch (wResult)
        {
        case 0:
            {
                *pwResult = 0;
                break;
            }
        case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
            {
                /*������ط��ʴ�����Ѵ���Ĵ�����1*/
                g_dwLlrpTagAccessErrTimes ++;
                *pwResult = 4;
                break;
            }
        case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:
            {
                *pwResult = 1;
                break;
            }

        default:
            {
                *pwResult = 3;
                RdpLlrpDbgPrint("(LLRP)There is not error code could be expressrn");
                break;
            }
        }

    }
    else if (1 == wReadOrWrite)
    {
        switch (wResult)
        {
        case 0:
            {
                *pwResult = 0;
                break;
            }
        case(unsigned short int)USP_UHF_EPC_ACCESS_NOT_PERMIT:
            {
                /*������ط��ʴ�����Ѵ���Ĵ�����1*/
                g_dwLlrpTagAccessErrTimes ++;
                *pwResult = 5;
                break;
            }
        case(unsigned short int)USP_UHF_EPC_TAG_POWER_NOT_ENOUGH:
            {
                *pwResult = 1;
                break;
            }
        case(unsigned short int)USP_UHF_FPGA_UNKNOW_FRAME:
            {
                *pwResult = 2;
                break;
            }
        default:
            {
                *pwResult = 4;
                RdpLlrpDbgPrint("(LLRP)There is not error code could be expressrn");
                break;
            }
        }
    }
    else
    {
        *pwResult = 3;
    }
    RdpLlrpDbgPrint("(LLRP)in llrp_changeRcpResultToLlrpForEpcIPJQT *pwResult = %d\r\n", *pwResult);
}
/**************************************************************************
* �������ƣ�llrp_refreshEpcIPJQTOpResult
* ��������������Epc�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_refreshEpcIPJQTOpResult(T_USPUHFEpcIPJQTRes *ptRcpEpcIPJQTRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpEpcImpinjGetQTOpResult *ptEpcImpinjGetQTOpResult = NULL;
    T_RdpLlrpEpcImpinjSetQTOpResult *ptEpcImpinjSetQTOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tEpcID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    if (ptRcpEpcIPJQTRes->wReadOrWrite == 1)
    {
        ptEpcImpinjSetQTOpResult = (T_RdpLlrpEpcImpinjSetQTOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];
        ptEpcImpinjSetQTOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
        ptEpcImpinjSetQTOpResult->wResult = ptRcpEpcIPJQTRes->wResult;
        ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcImpinjSetQTOpResult);
        RdpLlrpDbgPrint("(LLRP)epc IPJQT Set Rsp result:\r\n");
        RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcImpinjSetQTOpResult->wResult);
    }
    else
    {
        ptEpcImpinjGetQTOpResult = (T_RdpLlrpEpcImpinjGetQTOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];
        ptEpcImpinjGetQTOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
        ptEpcImpinjGetQTOpResult->wResult = ptRcpEpcIPJQTRes->wResult;
        ptEpcImpinjGetQTOpResult->wAccessRange = ptRcpEpcIPJQTRes->wAccessRange;
        ptEpcImpinjGetQTOpResult->wDataProfile = ptRcpEpcIPJQTRes->wDataProfile;
        ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpEpcImpinjGetQTOpResult);
        RdpLlrpDbgPrint("(LLRP)epc IPJQTGet Rsp result:\r\n");
        RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptEpcImpinjGetQTOpResult->wResult);
        RdpLlrpDbgPrint("(LLRP)AccessRange: %d\r\n", ptEpcImpinjGetQTOpResult->wAccessRange);
        RdpLlrpDbgPrint("(LLRP)DataProfile: %d\r\n", ptEpcImpinjGetQTOpResult->wDataProfile);
    }
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpEpcIPJQTRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
}

/**************************************************************************
* �������ƣ�llrp_tagEpcIPJQTRsp
* ��������������Rcpimpinjָ����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_tagEpcIPJQTRsp(T_Msg *ptRcvMsg)
{

    unsigned int dwOpReportIndex = 0;
    T_USPUHFEpcIPJQTRes *ptEPC_IPJQTResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagEpcIPJQTRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_EPC_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagEpcIPJQTRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaIPJQTResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptEPC_IPJQTResponse = (T_USPUHFEpcIPJQTRes*)(ptRcvMsg + 1);
    llrp_changeRcpResultToLlrpForEpcIPJQT(&ptEPC_IPJQTResponse->wResult, ptEPC_IPJQTResponse->wReadOrWrite);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_EPCGlobalClass1Gen2,
                                        ptEPC_IPJQTResponse->wAntennaIndex,
                                        ptEPC_IPJQTResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptEPC_IPJQTResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshEpcIPJQTOpResult(ptEPC_IPJQTResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshEpcIPJQTOpResult(ptEPC_IPJQTResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_clientRequestOp
* ���������������̨����
* ���ʵı���
* �޸ĵı���
* ���������void *ptLocalSingleReportCtrl
                            
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_clientRequestOp(void *ptLocalSingleReportCtrl)
{
    unsigned int dwRet = 0;
    T_Msg *ptSndMsg = NULL;
    LLRP_tSTagReportData *ptTagReportData = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptRunListOpSet = llrp_getRunListOpSet();
    LLRP_tSCLIENT_REQUEST_OP *ptTempLLRP_tSCLIENT_REQUEST_OP = LLRP_CLIENT_REQUEST_OP_construct();
    ptTagReportData = LLRP_TagReportData_construct();
    /**/
    g_tClientRequestOpSet.eProtocolID = ptRunAccess->tRdpLlrpAccess.eProtocolID;
    g_tClientRequestOpSet.tMatchUID = ptRunAccess->tMatchUID;
    if (ptRunListOpSet->wOpIndex == 0)
    {
        RdpLlrpDbgPrint("(LLRP)clientrequest is first Op need construct an Local AO head\r\n");
        llrp_constructLocalAOReportHead(ptRunAccess->tRdpLlrpAccess.eProtocolID,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);
    }
    report_constructAccessReport(ptTagReportData, (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer);
    LLRP_CLIENT_REQUEST_OP_setTagReportData(ptTempLLRP_tSCLIENT_REQUEST_OP, ptTagReportData );
    ptTempLLRP_tSCLIENT_REQUEST_OP->hdr.MessageID = ptRunAccess->tRdpLlrpAccessDispatchCtrl.dwMessageID;
    ptSndMsg->mtype = LLRP_SYN_MSG;
    ptSndMsg->ucLinkIndex = ptRunAccess->tRdpLlrpAccessDispatchCtrl.ucLinkIndex;
    ptSndMsg->ucLinkType = ptRunAccess->tRdpLlrpAccessDispatchCtrl.ucLinkType;
    /*����wSenderTno��wReceiverTno��ʶ·�ɵ�Դ��Ŀ��
     wSrcTno��wDstTno ��ʾ������ת�Ľڵ�*/
    ptSndMsg->wSenderTno = ptRunAccess->tRdpLlrpAccessDispatchCtrl.wReceiverTno;
    ptSndMsg->wReceiverTno = ptRunAccess->tRdpLlrpAccessDispatchCtrl.wSenderTno;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_RDP_RULE_TNO;
    /*�������� �ص����� ��̨ �� �����ڲ������*/
    ptSndMsg->wMsgID = EV_CLIENT_REQUEST_OP;
    /*������ǵ�ַָ�� ����������ָ����ռ��ַ����*/
    ptSndMsg->wMsgLen = sizeof(ptTempLLRP_tSCLIENT_REQUEST_OP);
    *(LLRP_tSCLIENT_REQUEST_OP **)(ptSndMsg + 1) = ptTempLLRP_tSCLIENT_REQUEST_OP;
    RdpLlrpDbgPrint("(LLRP)send clientRequestOp to rule addr=0x%x\r\n", ptTempLLRP_tSCLIENT_REQUEST_OP);
    dwRet = RDP_SEND_MSG(pair_socket_rule, ptSndMsg);
    if (dwRet < 0)
    {
        RdpLlrpDbgPrint("(LLRP)msgsend error\r\n");
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, dwRet, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    /*����clientRequest��ʱʱ��Ϊ10s*/
    LLRP_START_TIMER(TClientRequestOpSpecTimeout, LLRP_CLIENTREQUEST_TIMEOUT);
}
/**************************************************************************
* �������ƣ�llrp_clientRequestRsp
* ����������clientrequest����Ӧ��Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg,
                             ���յ���Ϣ
                            T_RdpLlrpClientRequestOpSet *ptClientRequestOpSet,
                            clientrequestrsp��access��ͬƴ�ӵ�op����
                            
  LLRP_tSCLIENT_REQUEST_OP_RESPONSE *ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE
                           ��Ӧ��Ϣ
                            
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_clientRequestRsp(T_Msg *ptRcvMsg )
{
    LLRP_tSCLIENT_REQUEST_OP_RESPONSE *ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE = *(LLRP_tSCLIENT_REQUEST_OP_RESPONSE **)(ptRcvMsg + 1);
    T_RdpLlrpClientRequestOpSet *ptClientRequestOpSet = &g_tClientRequestOpSet;
    unsigned int dwAccessOpIndex = 0;
    unsigned int dwClientOpIndex = 0;
    LLRP_tSParameter *ptTempLLRP_tSParameter = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptTempAccessCtrlAndPara = NULL;
    T_RdpLlrpListOPCtrlSet *ptAccessListOpSet = NULL;
    LLRP_tSEPCData *ptLLRP_tSEPCData = NULL;
    LLRP_tSUIDData *ptLLRP_tSUIDData = NULL;
    LLRP_tSGBData *ptLLRP_tSGBData = NULL;
    unsigned short int wLoop = 0;
    RdpLlrpDbgPrint("(LLRP)receive an EV_CLIENT_REQUEST_OP_RESPONSE msg\r\n");
    LLRP_STOP_TIMER(TClientRequestOpSpecTimeout);
    ptTempAccessCtrlAndPara = llrp_getRunAccess();
    if (ptTempAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptAccessListOpSet = (T_RdpLlrpListOPCtrlSet *) & ptTempAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet;
    memset(&ptClientRequestOpSet->tListOp, 0, sizeof(T_RdpLlrpListOPCtrlSet));
    /*clientRequest��Ӧ��Ҫ����g_tclientRequestAccess ,��ԭ���Ĳ��������������access�У��������β���*/
    if (ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE == NULL)
    {
        /*û�л�Ӧ����ִ��Access */
        RdpLlrpDbgPrint("(LLRP)clientrequest not rsp need to continue\r\n");
        ptAccessListOpSet->wOpIndex++;
        llrp_dispatchTagAccess();
        return ;
    }
    if (ptTempAccessCtrlAndPara->tRdpLlrpAccess.eProtocolID == LLRP_AirProtocols_EPCGlobalClass1Gen2)
    {
        ptLLRP_tSEPCData = (LLRP_tSEPCData *)ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->pEPCParameter;
        if ((ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->AccessSpecID
                != ptTempAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID)
                || memcmp(ptLLRP_tSEPCData->EPC.pValue, \
                          ptClientRequestOpSet->tMatchUID.aucTagData, ptClientRequestOpSet->tMatchUID.wTagDataLen) != 0
                || (NULL == ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec))
        {
            RdpLlrpDbgPrint("(LLRP)the AccessID is not right or EPC is not match need to do next AO op\r\n");
            RdpLlrpDbgPrint("(LLRP)pClientRequestResponse->AccessSpecID=%d,dwAccessSpecID=%d,ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE=0x%x\r\n",
                            ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->AccessSpecID, ptTempAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID, ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
            RdpLlrpDbgPrint("(LLRP)EPC.nValue=%d,ptClientRequestOpSet->tMatchUID.wTagDataLen=%d\r\n",
                            ptLLRP_tSEPCData->EPC.nBit / 8, ptClientRequestOpSet->tMatchUID.wTagDataLen);
            for (wLoop = 0;wLoop < ptLLRP_tSEPCData->EPC.nBit / 8;wLoop++)
            {
                RdpDbgPrintRaw("(LLRP)Clientepc[%d]=%d,  matchUid[%d]=%d\r\n",
                                wLoop, ptLLRP_tSEPCData->EPC.pValue[wLoop], wLoop, ptClientRequestOpSet->tMatchUID.aucTagData[wLoop]);
            }
            usp_ltk_freeMessage((LLRP_tSMessage * )ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
            ptAccessListOpSet->wOpIndex++;
            llrp_dispatchTagAccess();
            return ;
        }
        RdpLlrpDbgPrint("(LLRP)cpy Op before client request but not do \r\n");
        for (dwAccessOpIndex = 0; dwAccessOpIndex < ptAccessListOpSet->wOpIndex;
                dwAccessOpIndex++, ptClientRequestOpSet->tListOp.wOpIndex++, ptClientRequestOpSet->tListOp.wOPNum++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
            RdpLlrpDbgPrint("(LLRP)before wOpindex :%d,clientrequest OpIndex  %d,wOPNum=%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                            ptClientRequestOpSet->tListOp.wOPNum);

        }
        RdpLlrpDbgPrint("(LLRP)first do clientrequest  Opindex %d\r\n", ptClientRequestOpSet->tListOp.wOpIndex);

        llrp_transferAccessEpcOpPara(&ptClientRequestOpSet->tListOp,
                                     (LLRP_tSParameter *) ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec );
        RdpLlrpDbgPrint("(LLRP)wOpindex :%d,clientrequest OpIndex  %d,wOPNum:%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                        ptClientRequestOpSet->tListOp.wOPNum);
        RdpLlrpDbgPrint("(LLRP)insert Access op into clientrequest Access op set ptAccessListOpSet->wOpIndex:%d\r\n", ptAccessListOpSet->wOpIndex);
        for (dwAccessOpIndex = ptAccessListOpSet->wOpIndex + 1, dwClientOpIndex = ptClientRequestOpSet->tListOp.wOPNum;
                dwAccessOpIndex < ptAccessListOpSet->wOPNum;
                dwAccessOpIndex++, dwClientOpIndex++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[dwClientOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[dwClientOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
            ptClientRequestOpSet->tListOp.wOPNum++;
            RdpLlrpDbgPrint("(LLRP)after wOpindex :%d,clientrequest OpIndex  %d,wOPNum:%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                            ptClientRequestOpSet->tListOp.wOPNum);
        }
        RdpLlrpDbgPrint("(LLRP)ptAccessListOpSet->wOpIndex: %d,ptAccessListOpSet->wOPNum:%d\r\n", ptAccessListOpSet->wOpIndex, ptAccessListOpSet->wOPNum);


    }
    if (ptTempAccessCtrlAndPara->tRdpLlrpAccess.eProtocolID == LLRP_AirProtocols_ISO18000_6B)
    {
        /*ISO6B ����ô�� �ǲ�����Ҫ�Ӹ����� ����ֱ����EPC*/llrp_u8v_t UID;
        ptLLRP_tSUIDData = (LLRP_tSUIDData *)ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->pEPCParameter;
        if ((ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->AccessSpecID
                != ptTempAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID)
                || memcmp(ptLLRP_tSUIDData->UID.pValue, \
                          ptClientRequestOpSet->tMatchUID.aucTagData, ptClientRequestOpSet->tMatchUID.wTagDataLen) != 0
                || (NULL == ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec))
        {
            RdpLlrpDbgPrint("(LLRP)the AccessID is not right or UID is not match need to do next AO op\r\n");
            usp_ltk_freeMessage((LLRP_tSMessage * )ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
            ptAccessListOpSet->wOpIndex++;
            llrp_dispatchTagAccess();
            return ;

        }
        for (dwAccessOpIndex = 0; dwAccessOpIndex < ptAccessListOpSet->wOpIndex;
                dwAccessOpIndex++, ptClientRequestOpSet->tListOp.wOpIndex++, ptClientRequestOpSet->tListOp.wOPNum++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
        }
        RdpLlrpDbgPrint("(LLRP)first do clientrequest  Opindex %d\r\n", ptClientRequestOpSet->tListOp.wOpIndex);
        llrp_transferAccessISO6BOpPara(&ptClientRequestOpSet->tListOp,
                                       (LLRP_tSParameter *) ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec );
        for (dwAccessOpIndex = ptAccessListOpSet->wOpIndex + 1, dwClientOpIndex = ptClientRequestOpSet->tListOp.wOPNum;
                dwAccessOpIndex < ptAccessListOpSet->wOPNum;
                dwAccessOpIndex++, dwClientOpIndex++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[dwClientOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[dwClientOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
            ptClientRequestOpSet->tListOp.wOPNum++;
        }

    }
    else if (ptTempAccessCtrlAndPara->tRdpLlrpAccess.eProtocolID == LLRP_AirProtocols_GB)
    {
        ptLLRP_tSGBData = (LLRP_tSGBData *)ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->pEPCParameter;
        if ((ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->AccessSpecID
                != ptTempAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID)
                || memcmp(ptLLRP_tSGBData->GBID.pValue, \
                          ptClientRequestOpSet->tMatchUID.aucTagData, ptClientRequestOpSet->tMatchUID.wTagDataLen) != 0
                || (NULL == ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec))
        {
            RdpLlrpDbgPrint("(LLRP)the AccessID is not right or GBID is not match need to do next AO op\r\n");
            RdpLlrpDbgPrint("(LLRP)pClientRequestResponse->AccessSpecID=%d,dwAccessSpecID=%d,ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE=0x%x\r\n",
                            ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->AccessSpecID, ptTempAccessCtrlAndPara->tRdpLlrpAccess.dwAccessSpecID, ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
            RdpLlrpDbgPrint("(LLRP)GB.nValue=%d,ptClientRequestOpSet->tMatchUID.wTagDataLen=%d\r\n",
                            ptLLRP_tSGBData->GBID.nBit / 8, ptClientRequestOpSet->tMatchUID.wTagDataLen);
            for (wLoop = 0;wLoop < ptLLRP_tSGBData->GBID.nBit / 8;wLoop++)
            {
                RdpDbgPrintRaw("(LLRP)ClientGB[%d]=%d,  matchUid[%d]=%d\r\n",
                                wLoop, ptLLRP_tSGBData->GBID.pValue[wLoop], wLoop, ptClientRequestOpSet->tMatchUID.aucTagData[wLoop]);
            }
            usp_ltk_freeMessage((LLRP_tSMessage * )ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
            ptAccessListOpSet->wOpIndex++;
            llrp_dispatchTagAccess();
            return ;
        }
        RdpLlrpDbgPrint("(LLRP)cpy Op before client request but not do \r\n");
        for (dwAccessOpIndex = 0; dwAccessOpIndex < ptAccessListOpSet->wOpIndex;
                dwAccessOpIndex++, ptClientRequestOpSet->tListOp.wOpIndex++, ptClientRequestOpSet->tListOp.wOPNum++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[ptClientRequestOpSet->tListOp.wOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
            RdpLlrpDbgPrint("(LLRP)before wOpindex :%d,clientrequest OpIndex  %d,wOPNum=%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                            ptClientRequestOpSet->tListOp.wOPNum);

        }
        RdpLlrpDbgPrint("(LLRP)first do clientrequest  Opindex %d\r\n", ptClientRequestOpSet->tListOp.wOpIndex);

        llrp_transferAccessGBOpPara(&ptClientRequestOpSet->tListOp,
                                     (LLRP_tSParameter *) ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE->pClientRequestResponse->listAccessCommandOpSpec );
        RdpLlrpDbgPrint("(LLRP)wOpindex :%d,clientrequest OpIndex  %d,wOPNum:%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                        ptClientRequestOpSet->tListOp.wOPNum);
        RdpLlrpDbgPrint("(LLRP)insert Access op into clientrequest Access op set ptAccessListOpSet->wOpIndex:%d\r\n", ptAccessListOpSet->wOpIndex);
        for (dwAccessOpIndex = ptAccessListOpSet->wOpIndex + 1, dwClientOpIndex = ptClientRequestOpSet->tListOp.wOPNum;
                dwAccessOpIndex < ptAccessListOpSet->wOPNum;
                dwAccessOpIndex++, dwClientOpIndex++)
        {
            memcpy(&ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[dwClientOpIndex], &ptAccessListOpSet->tRdpLlrpOPCtr[dwAccessOpIndex], sizeof(T_RdpLlrpOPCtrl));
            memcpy(&ptClientRequestOpSet->tListOp.tSingleOp[dwClientOpIndex], &ptAccessListOpSet->tSingleOp[dwAccessOpIndex], sizeof(T_OpBuffer));
            ptClientRequestOpSet->tListOp.wOPNum++;
            RdpLlrpDbgPrint("(LLRP)after wOpindex :%d,clientrequest OpIndex  %d,wOPNum:%d\r\n", ptClientRequestOpSet->tListOp.wOpIndex, ptClientRequestOpSet->tListOp.tRdpLlrpOPCtr[ptClientRequestOpSet->tListOp.wOpIndex].wOpType,
                            ptClientRequestOpSet->tListOp.wOPNum);
        }
        RdpLlrpDbgPrint("(LLRP)ptAccessListOpSet->wOpIndex: %d,ptAccessListOpSet->wOPNum:%d\r\n", ptAccessListOpSet->wOpIndex, ptAccessListOpSet->wOPNum);


    }
    /*����AO�����ͷ��,��ȫ�ֱ���ֻҪ�з��ؾͲ����ظ�����*/
    //llrp_constructLocalAOReportHeadForClientReuqest();
    /*���õ�ǰ��accessִ��clientRequestִ��*/
    usp_ltk_freeMessage((LLRP_tSMessage * )ptLLRP_tSCLIENT_REQUEST_OP_RESPONSE);
    ptTempAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.bClientRequest = TRUE;
    llrp_nextTagAccessOp(&ptClientRequestOpSet->tListOp, ptClientRequestOpSet->eProtocolID, &ptClientRequestOpSet->tMatchUID);
}
/**************************************************************************
* �������ƣ�ClientRequestOpSpecResponse
* �����������·�access����һ��Op
* ���ʵı���
* �޸ĵı���
* ���������unsigned short int wProtoType,
                            Э������Epc��ISO6B
                            void *ptTagID
                            Epc��
                            
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_clientRequestOpTimeout(T_RdpLlrpClientRequestOpSet *ptClientRequestOpSet)
{
    unsigned int dwOpIndex = 0;
    unsigned int dwAccessOpIndex = 0;
    unsigned int dwClientOpIndex = 0;
    T_RdpLlrpAccessCtrlAndParam *ptTempAccessCtrlAndPara = NULL;
    T_RdpLlrpAntsInvSet *ptRunAntSets = NULL;
    T_RdpLlrpListOPCtrlSet *ptAccessListOpSet = NULL;
    ptTempAccessCtrlAndPara = llrp_getRunAccess();
    if (ptTempAccessCtrlAndPara == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return ;
    }
    ptAccessListOpSet = (T_RdpLlrpListOPCtrlSet *) & ptTempAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet;
    /*����Ao������Ϣͷ */
    //llrp_constructLocalAOReportHeadForClientReuqest();
    /*�����·�Access�е�op,����Э���UID��clientRequest��ȡ��*/
    ptAccessListOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_getOpEpcAccessPassword
* �������������Op��������Կ
* ���ʵı���
* �޸ĵı���
* ���������unsigned int dwOpAccessPassword,
                              T_USPUHFEpcTagId * ptEpcTagId, 
                              unsigned short int wAntID
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_getOpEpcAccessPassword(unsigned int dwOpAccessPassword, T_USPUHFEpcTagId * ptEpcTagId, unsigned short int wAntID)
{
    unsigned int dwRet = 0;
    unsigned int dwAccessPassword = 0;
    if (g_bLlrpEpcAccessFlag == TRUE)
    {
        RdpLlrpDbgPrint("(LLRP)g_bLlrpEpcAccessFlag has been set return 0\r\n");
        return 0;
    }
    else
    {
        if (g_tRdpPwdSys.dwPwdIndexNum == 0)
        {
            RdpLlrpDbgPrint("(LLRP)PwdIndexNum is zero return op AccessPassword =0x%x\r\n",dwOpAccessPassword);
            llrp_setEpcAccessFlag();
            return dwOpAccessPassword;
        }
        else
        {
            dwRet = llrp_getEpcAccessPassword(ptEpcTagId, wAntID, &dwAccessPassword);
            if (dwRet == 0)
            {
                RdpLlrpDbgPrint("(LLRP)PwdIndexNum is  not zero return psam AccessPassword\r\n");
                llrp_setEpcAccessFlag();
                return dwAccessPassword;
            }
            else
            {
                return 0;
            }
        }
    }
}
/**************************************************************************
* �������ƣ�llrp_nextEpcAccessOp
* �����������·�access����һ��Op
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptSndMsg ,
                            �·�����Ϣ
                            T_USPUHFEpcTagId *ptEpcTagId
                            ��ǩ��EPC����
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_nextEpcAccessOp(T_Msg *ptSndMsg , T_RdpLlrpListOPCtrlSet*ptlistOpCtrlSet, void *ptTagID)
{
    unsigned int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned char *ptByte = NULL;
    unsigned int dwOpIndex = 0;
    T_RdpLlrpAntInv *ptRunAntInv = NULL;
    T_RdpLlrpUID *ptEpcTagId = NULL;
    T_RdpLlrpC1G2Read *ptTempLLRPC1G2Read = NULL;
    T_RdpLlrpC1G2Write*ptTempLLRPC1G2Write = NULL;
    T_RdpLlrpC1G2Lock *ptTempLLRPC1G2Lock = NULL;
    T_RdpLlrpC1G2BlockLock *ptTempLLRPC1G2BlockLock = NULL;
    T_RdpLlrpC1G2ReadLock *ptTempLLRPC1G2ReadLock = NULL;
    T_RdpLlrpC1G2Kill *ptTempLLRPC1G2Kill = NULL;
    T_RdpLlrpC1G2IPJQT *ptTempLLRPC1G2IPJQT = NULL;
    T_USPUHFEpcRead *ptUSPUHFEpcRead = NULL;
    T_USPUHFEpcWrite *ptUSPUHFEpcWrite = NULL;
    T_USPUHFEpcLock *ptUSPUHFEpcLock = NULL;
    T_USPUHFEpcBlockLock *ptUSPUHFEpcBlockLock = NULL;
    T_USPUHFEpcReadLock *ptUSPUHFEpcReadLock = NULL;
    T_USPUHFEpcKill *ptUSPUHFEpcKill = NULL;
    T_USPUHFEpcIPJQT*ptUSPUHFEpcIPJQT = NULL;
    T_USPUHFEpcTagId *ptUHFEpcTagId = NULL;
    dwOpIndex = ptlistOpCtrlSet->wOpIndex;
    ptRunAntInv = llrp_getRunAnt();
    if (ptRunAntInv == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return RDP_NULLPOINTER_EXCEPTION;
    }
    ptEpcTagId = ptTagID;
    RdpLlrpDbgPrint("(LLRP)dwOpIndex:%d,wOpType:%d\r\n", dwOpIndex, ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType);
    switch (ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType)
    {
    case LLRP_C1GOperationType_Read:
    case LLRP_C1G2ReadCustomUID:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_C1GOperationType_Read\r\n");
            ptUSPUHFEpcRead = (T_USPUHFEpcRead *)(ptSndMsg + 1);
            ptTempLLRPC1G2Read = (T_RdpLlrpC1G2Read *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcRead->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcRead->tEpcReadPara.ucMB = ptTempLLRPC1G2Read->ucMB;
            ptUSPUHFEpcRead->tEpcReadPara.wWordPointer = ptTempLLRPC1G2Read->wWordPointer;
            ptUSPUHFEpcRead->tEpcReadPara.wWordCount = ptTempLLRPC1G2Read->wWordCount;
            ptUSPUHFEpcRead->tEpcReadPara.dwUhfBitCommand = ptTempLLRPC1G2Read->dwRCPCommand;
            ptUSPUHFEpcRead->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcRead->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcRead->tEpcReadPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2Read->dwAccessPassword, &ptUSPUHFEpcRead->tTagId, ptUSPUHFEpcRead->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_READ;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcRead) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaReadResp, TFpgaRespCount);
            break;
        }
    case LLRP_C1GOperationType_Write:
    case LLRP_C1GOperationType_BLOCKWrite:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_C1GOperationType_Write\r\n");
            ptUSPUHFEpcWrite = (T_USPUHFEpcWrite *)(ptSndMsg + 1);
            ptTempLLRPC1G2Write = (T_RdpLlrpC1G2Write *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcWrite->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcWrite->tEpcWritePara.ucMB = ptTempLLRPC1G2Write->ucMB;
            ptUSPUHFEpcWrite->tEpcWritePara.dwUhfBitCommand = ptTempLLRPC1G2Write->dwRCPCommand;
            ptUSPUHFEpcWrite->tEpcWritePara.wWordPointer = ptTempLLRPC1G2Write->wWordPointer;
            ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen = ptTempLLRPC1G2Write->wDataLength;
            memcpy(ptUSPUHFEpcWrite->tEpcWritePara.aucData,
                   ptTempLLRPC1G2Write->aucData,
                   ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen*sizeof(unsigned short int));
            ptUSPUHFEpcWrite->tEpcWritePara.ucWriteType = ((ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType == LLRP_C1GOperationType_BLOCKWrite) ? 1 : 0);

            ptByte = (unsigned char *)ptUSPUHFEpcWrite;
            ptByte = ptByte + struct_fixed_len(T_USPUHFEpcWrite, tEpcWritePara) + struct_fixed_len(T_USPUHFEpcWritePara, aucData) + ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen * sizeof(unsigned short int);
            ptUHFEpcTagId = (T_USPUHFEpcTagId *)ptByte;
            ptUHFEpcTagId->wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUHFEpcTagId->aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcWrite->tEpcWritePara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2Write->dwAccessPassword, ptUHFEpcTagId, ptUSPUHFEpcWrite->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_WRITE;
            ptSndMsg->wMsgLen = struct_fixed_len(T_USPUHFEpcWrite, tEpcWritePara)
                                + struct_fixed_len(T_USPUHFEpcWritePara, aucData)
                                + ptUSPUHFEpcWrite->tEpcWritePara.wWriteWordLen * sizeof(unsigned short int)
                                + struct_fixed_len(T_USPUHFEpcTagId, aucTagData)
                                + ptUSPUHFEpcWrite->tTagId.wTagWordLen * sizeof(unsigned short int);
            LLRP_START_TIMER(TFpgaWriteResp, TFpgaRespCount);
            break;
        }
    case LLRP_C1GOperationType_Lock:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_C1GOperationType_Lock\r\n");
            ptUSPUHFEpcLock = (T_USPUHFEpcLock *)(ptSndMsg + 1);
            ptTempLLRPC1G2Lock = (T_RdpLlrpC1G2Lock*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcLock->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcLock->tEpcLockPara.dwUhfBitCommand = ptTempLLRPC1G2Lock->dwRCPCommand;
            ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.wPayLoadNum = ptTempLLRPC1G2Lock->tRdpLlrpC1G2LockPayLoad.wPayLoadNum;
            memcpy(ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.taEpcLockPayLoad,
                   ptTempLLRPC1G2Lock->tRdpLlrpC1G2LockPayLoad.tRdpLlrpC1G2LockPayLoad,
                   sizeof(ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.taEpcLockPayLoad));

            for (dwIndex = 0;dwIndex < ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.wPayLoadNum;dwIndex++)
            {
                RdpLlrpWarningPrint("(LLRP)lock payload %d\r\n");
                RdpLlrpWarningPrint("(LLRP)lock privilege:%d\r\n", ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.taEpcLockPayLoad[dwIndex].Privilege);
                RdpLlrpWarningPrint("(LLRP)lock DataField:%d\r\n", ptUSPUHFEpcLock->tEpcLockPara.tEpcLockPayLoad.taEpcLockPayLoad[dwIndex].DataField);
            }
            ptUSPUHFEpcLock->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcLock->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcLock->tEpcLockPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2Lock->dwAccessPassword, &ptUSPUHFEpcLock->tTagId, ptUSPUHFEpcLock->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_LOCK;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcLock) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_C1G2_BLOCKLOCK:
    case LLRP_C1G2_BLOCKLOCKQUERY:
        {

            RdpLlrpDbgPrint("(LLRP)LLRP_C1GOperationType_BlockLock or LLRP_C1G2_BLOCKLOCKQUERY\r\n");
            ptUSPUHFEpcBlockLock = (T_USPUHFEpcBlockLock *)(ptSndMsg + 1);
            ptTempLLRPC1G2BlockLock = (T_RdpLlrpC1G2BlockLock*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcBlockLock->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.dwUhfBitCommand = ptTempLLRPC1G2BlockLock->dwRCPCommand;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.ucMB = ptTempLLRPC1G2BlockLock->ucMB;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.ucRFU = ptTempLLRPC1G2BlockLock->ucRFU;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.wBlockPtr = ptTempLLRPC1G2BlockLock->wBlockPtr;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.wBlockRange = ptTempLLRPC1G2BlockLock->wBlockRange;
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.ucReadLock = ptTempLLRPC1G2BlockLock->ucReadLock;
            memcpy(ptUSPUHFEpcBlockLock->tEpcBlockLockPara.ucMask, ptTempLLRPC1G2BlockLock->ucMask, sizeof(ptTempLLRPC1G2BlockLock->ucMask));
            ptUSPUHFEpcBlockLock->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcBlockLock->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcBlockLock->tEpcBlockLockPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2BlockLock->dwAccessPassword, &ptUSPUHFEpcBlockLock->tTagId, ptUSPUHFEpcBlockLock->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_BLOCKLOCK;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcBlockLock) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaBlockLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_C1G2_ALIENREADLOCK:
        {
            ptUSPUHFEpcReadLock = (T_USPUHFEpcReadLock *)(ptSndMsg + 1);
            ptTempLLRPC1G2ReadLock = (T_RdpLlrpC1G2ReadLock *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcReadLock->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcReadLock->tEpcReadLockPara.ucMask = ptTempLLRPC1G2ReadLock->ucMask;
            ptUSPUHFEpcReadLock->tEpcReadLockPara.dwUhfBitCommand = ptTempLLRPC1G2ReadLock->dwRCPCommand;
            ptUSPUHFEpcReadLock->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcReadLock->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcReadLock->tEpcReadLockPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2ReadLock->dwAccessPassword, &ptUSPUHFEpcReadLock->tTagId, ptUSPUHFEpcReadLock->dwAntIndex);
	        RdpLlrpDbgPrint("(LLRP)ptUSPUHFEpcReadLock->tEpcReadLockPara.ucMask = %d.\r\n",
                            ptUSPUHFEpcReadLock->tEpcReadLockPara.ucMask);
            RdpLlrpDbgPrint("(LLRP)ptUSPUHFEpcReadLock->tEpcReadLockPara.dwAccessPassword = 0x%x.\r\n",
                            ptUSPUHFEpcReadLock->tEpcReadLockPara.dwAccessPassword);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_READLOCK;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcReadLock) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaAlienReadLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_C1GOperationType_Kill:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_C1GOperationType_Kill\r\n");
            ptUSPUHFEpcKill = (T_USPUHFEpcKill *)(ptSndMsg + 1);
            ptTempLLRPC1G2Kill = (T_RdpLlrpC1G2Kill*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcKill->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcKill->tEpcKillPara.dwKillPassword = ptTempLLRPC1G2Kill->wKillPassword;
            ptUSPUHFEpcKill->tEpcKillPara.dwUhfBitCommand = ptTempLLRPC1G2Kill->dwRCPCommand;
            ptUSPUHFEpcKill->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcKill->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_KILL;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcKill) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaKillResp, TFpgaRespCount);
            break;
        }
    case LLRP_ImpinjSetQTConfig:
    case LLRP_ImpinjGetQTConfig:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ImpinjQT\r\n");
            ptUSPUHFEpcIPJQT = (T_USPUHFEpcIPJQT *)(ptSndMsg + 1);
            ptTempLLRPC1G2IPJQT = (T_RdpLlrpC1G2IPJQT*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcIPJQT->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.dwUhfBitCommand = ptTempLLRPC1G2IPJQT->dwRCPCommand;
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wAccessRange = ptTempLLRPC1G2IPJQT->wAccessRange;
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wDataProfile = ptTempLLRPC1G2IPJQT->wDataProfile;
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wPersistence = ptTempLLRPC1G2IPJQT->wPersistence;
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wReadOrWrite = ptTempLLRPC1G2IPJQT->wReadOrWrite;
            ptUSPUHFEpcIPJQT->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcIPJQT->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcIPJQT->tEpcIPJQTPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2IPJQT->dwAccessPassword, &ptUSPUHFEpcIPJQT->tTagId, ptUSPUHFEpcIPJQT->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_IPJQT;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcIPJQT) + ptEpcTagId->wTagDataLen;

            RdpLlrpDbgPrint("wAccessRange=%d    %d\r\n", ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wAccessRange, ptTempLLRPC1G2IPJQT->wAccessRange);
            RdpLlrpDbgPrint("wDataProfile=%d    %d\r\n", ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wDataProfile, ptTempLLRPC1G2IPJQT->wDataProfile);
            RdpLlrpDbgPrint("wPersistence=%d    %d\r\n", ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wPersistence, ptTempLLRPC1G2IPJQT->wPersistence);
            RdpLlrpDbgPrint("wReadOrWrite=%d    %d\r\n", ptUSPUHFEpcIPJQT->tEpcIPJQTPara.wReadOrWrite, ptTempLLRPC1G2IPJQT->wReadOrWrite);
            LLRP_START_TIMER(TFpgaIPJQTResp, TFpgaRespCount);
            break;
        }
    case LLRP_ClientRequestOpSpec:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ClientRequestOpSpec\r\n");
            llrp_clientRequestOp((void *)g_ucLocalReportDataBuffer);
            dwRet = LLRP_ClientRequestOpSpec;
            break;
        }
    case LLRP_C1G2WriteVerify:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_C1G2WriteVerify\r\n");
            ptUSPUHFEpcRead = (T_USPUHFEpcRead *)(ptSndMsg + 1);
            ptTempLLRPC1G2Read = (T_RdpLlrpC1G2Read *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFEpcRead->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFEpcRead->tEpcReadPara.ucMB = ptTempLLRPC1G2Read->ucMB;
            ptUSPUHFEpcRead->tEpcReadPara.wWordPointer = ptTempLLRPC1G2Read->wWordPointer;
            ptUSPUHFEpcRead->tEpcReadPara.wWordCount = ptTempLLRPC1G2Read->wWordCount;
            ptUSPUHFEpcRead->tEpcReadPara.dwUhfBitCommand = ptTempLLRPC1G2Read->dwRCPCommand;
            ptUSPUHFEpcRead->tTagId.wTagWordLen = ptEpcTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFEpcRead->tTagId.aucTagData, ptEpcTagId->aucTagData, ptEpcTagId->wTagDataLen);
            ptUSPUHFEpcRead->tEpcReadPara.dwAccessPassword = llrp_getOpEpcAccessPassword(ptTempLLRPC1G2Read->dwAccessPassword, &ptUSPUHFEpcRead->tTagId, ptUSPUHFEpcRead->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_EPC_VERIFY_READ;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFEpcRead) + ptEpcTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaReadResp, TFpgaRespCount);
            break;
        }
    default:
        {
            dwRet = RDP_LLRP_UNSUPORT_OPTYPE;
            RdpLlrpDbgPrint("(LLRP)receive noknown optype :%d\r\n", ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType);
        }
        break;
    }
    return dwRet;
}
/**************************************************************************
* �������ƣ�lllrp_nextISO6BAccessOp
* �����������·�access����һ��Op
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptSndMsg 
                             �·�����Ϣ
                            T_USPUHFISO6BTagId *ptISO6BTagId
                            ��ǩ��ISO6B ����
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
unsigned int llrp_nextISO6BAccessOp(T_Msg *ptSndMsg , T_RdpLlrpListOPCtrlSet*ptlistOpCtrlSet, void *ptTagID)
{
    unsigned int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned char *ptByte = NULL;
    unsigned int dwOpIndex = 0;
    T_RdpLlrpAntInv *ptRunAntInv = NULL;
    T_RdpLlrpUID *ptISO6BTagId = NULL;
    T_USPUHFISO6BRead *ptTempLLRPISO6BRead = NULL;
    T_USPUHFISO6BRead *ptUSPUHFISO6BRead = NULL;
    T_USPUHFISO6BWrite *ptTempLLRPISO6BWrite = NULL;
    T_USPUHFISO6BWrite *ptUSPUHFISO6BWrite = NULL;
    T_USPUHFISO6BLock *ptTempLLRPISO6BLock = NULL;
    T_USPUHFISO6BLock *ptUSPUHFISO6BLock = NULL;
    T_USPUHFISO6BLockQuery *ptTempLLRPISO6BLockQuery = NULL;
    T_USPUHFISO6BLockQuery *ptUSPUHFISO6BLockQuery = NULL;
    T_USPUHFEpcWrite *ptUSPUHFEpcWrite = NULL;
    T_USPUHFEpcLock *ptUSPUHFEpcLock = NULL;
    T_USPUHFEpcKill *ptUSPUHFEpcKill = NULL;
    T_USPUHFISO6BTagId *ptUHFISO6BTagId = NULL;
    dwOpIndex = ptlistOpCtrlSet->wOpIndex;
    ptRunAntInv = llrp_getRunAnt();
    if (ptRunAntInv == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_EPC_ID);
        return RDP_NULLPOINTER_EXCEPTION;
    }
    ptISO6BTagId = ptTagID;
    RdpLlrpDbgPrint("(LLRP)dwOpIndex:%d,wOpType:%d\r\n", dwOpIndex, ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType);
    switch (ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType)
    {
    case LLRP_ISO6BOperationType_Read:
    case LLRP_ISO6BReadCustomUID:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ISO6BOperationType_Read\r\n");
            ptUSPUHFISO6BRead = (T_USPUHFISO6BRead *)(ptSndMsg + 1);
            ptTempLLRPISO6BRead = (T_USPUHFISO6BRead *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFISO6BRead->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFISO6BRead->ucBytePtr = ptTempLLRPISO6BRead->ucBytePtr;
            ptUSPUHFISO6BRead->ucByteCount = ptTempLLRPISO6BRead->ucByteCount;
            ptUSPUHFISO6BRead->tISO6BReadctl.ucCommandType = ptTempLLRPISO6BRead->tISO6BReadctl.ucCommandType;
            ptUSPUHFISO6BRead->tISO6BReadctl.ucRetryTimes = ptTempLLRPISO6BRead->tISO6BReadctl.ucRetryTimes;
            ptUSPUHFISO6BRead->tTagId.wTagDataLen = ptISO6BTagId->wTagDataLen;
            memcpy(ptUSPUHFISO6BRead->tTagId.aucTagData, ptISO6BTagId->aucTagData, ptISO6BTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_ISO6B_READ;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BRead);
            LLRP_START_TIMER(TFpgaReadResp, TFpgaRespCount);
            break;
        }
    case LLRP_ISO6BOperationType_Write:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ISO6BOperationType_Write\r\n");
            ptUSPUHFISO6BWrite = (T_USPUHFISO6BWrite *)(ptSndMsg + 1);
            ptTempLLRPISO6BWrite = (T_USPUHFISO6BWrite *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFISO6BWrite->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFISO6BWrite->ucBytePtr = ptTempLLRPISO6BWrite->ucBytePtr;
            ptUSPUHFISO6BWrite->ucByteCount = ptTempLLRPISO6BWrite->ucByteCount;
            ptUSPUHFISO6BWrite->tISO6BWriteCtl.ucCommandType = ptTempLLRPISO6BWrite->tISO6BWriteCtl.ucCommandType;
            ptUSPUHFISO6BWrite->tISO6BWriteCtl.ucRetryTimes = ptTempLLRPISO6BWrite->tISO6BWriteCtl.ucRetryTimes;
            ptUSPUHFISO6BWrite->tISO6BWriteCtl.ucVerifyTimes = ptTempLLRPISO6BWrite->tISO6BWriteCtl.ucVerifyTimes;
            memcpy(ptUSPUHFISO6BWrite->aucWriteData, ptTempLLRPISO6BWrite->aucWriteData, ptUSPUHFISO6BWrite->ucByteCount);
            ptUSPUHFISO6BWrite->tTagId.wTagDataLen = ptISO6BTagId->wTagDataLen;
            memcpy(ptUSPUHFISO6BWrite->tTagId.aucTagData, ptISO6BTagId->aucTagData, ptISO6BTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_ISO6B_WRITE;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BWrite);
            LLRP_START_TIMER(TFpgaWriteResp, TFpgaRespCount);
            break;
        }
    case LLRP_ISO6BOperationType_Lock:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ISO6BOperationType_Lock\r\n");
            ptUSPUHFISO6BLock = (T_USPUHFISO6BLock *)(ptSndMsg + 1);
            ptTempLLRPISO6BLock = (T_USPUHFISO6BLock *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFISO6BLock->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFISO6BLock->ucBytePtr = ptTempLLRPISO6BLock->ucBytePtr;
            ptUSPUHFISO6BLock->ucByteCount = ptTempLLRPISO6BLock->ucByteCount;
            ptUSPUHFISO6BLock->tISO6BLockCtl.ucRetryTimes = ptTempLLRPISO6BLock->tISO6BLockCtl.ucRetryTimes;
            ptUSPUHFISO6BLock->tISO6BLockCtl.ucVerifyTimes = ptTempLLRPISO6BLock->tISO6BLockCtl.ucRetryTimes;
            ptUSPUHFISO6BLock->tTagId.wTagDataLen = ptISO6BTagId->wTagDataLen;
            memcpy(ptUSPUHFISO6BLock->tTagId.aucTagData, ptISO6BTagId->aucTagData, ptISO6BTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_ISO6B_LOCK;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BLock);
            LLRP_START_TIMER(TFpgaLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_ISO6BOperationType_QueryLock:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ISO6BOperationType_LockQuery\r\n");
            ptUSPUHFISO6BLockQuery = (T_USPUHFISO6BLockQuery*)(ptSndMsg + 1);
            ptTempLLRPISO6BLockQuery = (T_USPUHFISO6BLockQuery *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFISO6BLockQuery->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFISO6BLockQuery->ucBytePtr = ptTempLLRPISO6BLockQuery->ucBytePtr;
            ptUSPUHFISO6BLockQuery->ucByteCount = ptTempLLRPISO6BLockQuery->ucByteCount;
            ptUSPUHFISO6BLockQuery->tISO6BLockQueryCtl.ucRetryTimes = ptTempLLRPISO6BLockQuery->tISO6BLockQueryCtl.ucRetryTimes;
            ptUSPUHFISO6BLockQuery->tTagId.wTagDataLen = ptISO6BTagId->wTagDataLen;
            memcpy(ptUSPUHFISO6BLockQuery->tTagId.aucTagData, ptISO6BTagId->aucTagData, ptISO6BTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_ISO6B_LOCKQUERY;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFISO6BLockQuery);
            LLRP_START_TIMER(TFpgaLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_ClientRequestOpSpec:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_ClientRequestOpSpec\r\n");
            llrp_clientRequestOp((void *)g_ucLocalReportDataBuffer);
            dwRet = LLRP_ClientRequestOpSpec;
            break;
        }
    default:
        {
            dwRet = RDP_LLRP_UNSUPORT_OPTYPE;
            RdpLlrpDbgPrint("(LLRP)receive noknown optype =%d\r\n", ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType);
            break;
        }
    }
    return dwRet;
}



/**************************************************************************
* �������ƣ�llrp_dealRcpReadRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpReadRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcReadRes * ptEPC_ReadResponse = NULL;
    T_USPUHFISO6BReadRes * ptISO6B_ReadResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_C1GOperationType_Read:
    case LLRP_C1G2ReadCustomUID:
        ptEPC_ReadResponse = (T_USPUHFEpcReadRes *)(ptRcvMsg + 1);
        ptEPC_ReadResponse->wChannelIndex = 0xffff;
        ptEPC_ReadResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptEPC_ReadResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptEPC_ReadResponse->wResult = ERR_FPGA_TIMEOUT;
        ptEPC_ReadResponse->wResultLength = 0;
        ptEPC_ReadResponse->wRssi = 0xffff;
        llrp_tagEpcReadRsp(ptRcvMsg);
        break;
    case LLRP_ISO6BOperationType_Read:
    case LLRP_ISO6BReadCustomUID:
        ptISO6B_ReadResponse = (T_USPUHFISO6BReadRes *)(ptRcvMsg + 1);
        ptISO6B_ReadResponse->wChannelIndex = 0xffff;
        ptISO6B_ReadResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptISO6B_ReadResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptISO6B_ReadResponse->wResult = ERR_FPGA_TIMEOUT;
        ptISO6B_ReadResponse->wResultLength = 0;
        ptISO6B_ReadResponse->wRssi = 0xffff;
        llrp_tagISO6BReadRsp(ptRcvMsg);
        break;
    case LLRP_C1G2WriteVerify:
        ptEPC_ReadResponse = (T_USPUHFEpcReadRes *)(ptRcvMsg + 1);
        ptEPC_ReadResponse->wChannelIndex = 0xffff;
        ptEPC_ReadResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptEPC_ReadResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptEPC_ReadResponse->wResult = ERR_FPGA_TIMEOUT;
        ptEPC_ReadResponse->wResultLength = 0;
        ptEPC_ReadResponse->wRssi = 0xffff;
        llrp_tagEpcVeriyReadRsp(ptRcvMsg);
        break;
    default:
        break;
    }
}

/**************************************************************************
* �������ƣ�llrp_dealRcpWriteRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpWriteRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcWriteRes * ptEPC_WriteResponse = NULL;
    T_USPUHFISO6BWriteRes * ptISO6B_WriteResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_C1GOperationType_Write:
        ptEPC_WriteResponse = (T_USPUHFEpcWriteRes *)(ptRcvMsg + 1);
        ptEPC_WriteResponse->wChannelIndex = 0xffff;
        ptEPC_WriteResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptEPC_WriteResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptEPC_WriteResponse->wResult = ERR_FPGA_TIMEOUT;
        ptEPC_WriteResponse->wResultLength = 0;
        ptEPC_WriteResponse->wRssi = 0xffff;
        llrp_tagEpcWriteRsp(ptRcvMsg);
        break;
    case LLRP_ISO6BOperationType_Write:
        ptISO6B_WriteResponse = (T_USPUHFISO6BWriteRes *)(ptRcvMsg + 1);
        ptISO6B_WriteResponse->wChannelIndex = 0xffff;
        ptISO6B_WriteResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptISO6B_WriteResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptISO6B_WriteResponse->wResult = ERR_FPGA_TIMEOUT;
        ptISO6B_WriteResponse->wResultLength = 0;
        ptISO6B_WriteResponse->wRssi = 0xffff;
        llrp_tagISO6BWriteRsp(ptRcvMsg);
        break;
    default:
        break;
    }
}
/**************************************************************************
* �������ƣ�llrp_dealRcpLockRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpLockRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcLockRes * ptEPC_LockResponse = NULL;
    T_USPUHFISO6BLockRes * ptISO6B_LockResponse = NULL;
    T_USPUHFISO6BLockQueryRes * ptISO6B_LockQueryResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_C1GOperationType_Lock:
        ptEPC_LockResponse = (T_USPUHFEpcLockRes *)(ptRcvMsg + 1);
        ptEPC_LockResponse->wChannelIndex = 0xffff;
        ptEPC_LockResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptEPC_LockResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptEPC_LockResponse->wResult = ERR_FPGA_TIMEOUT;
        ptEPC_LockResponse->wRssi = 0xffff;
        llrp_tagEpcLockRsp(ptRcvMsg);
        break;
    case LLRP_ISO6BOperationType_Lock:
        ptISO6B_LockResponse = (T_USPUHFISO6BLockRes *)(ptRcvMsg + 1);
        ptISO6B_LockResponse->wChannelIndex = 0xffff;
        ptISO6B_LockResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptISO6B_LockResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptISO6B_LockResponse->wResult = ERR_FPGA_TIMEOUT;
        ptISO6B_LockResponse->wResultLength = 0;
        ptISO6B_LockResponse->wRssi = 0xffff;
        llrp_tagISO6BLockRsp(ptRcvMsg);
        break;
    case LLRP_ISO6BOperationType_QueryLock:
        ptISO6B_LockQueryResponse = (T_USPUHFISO6BLockQueryRes *)(ptRcvMsg + 1);
        ptISO6B_LockQueryResponse->wChannelIndex = 0xffff;
        ptISO6B_LockQueryResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptISO6B_LockQueryResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptISO6B_LockQueryResponse->wResult = ERR_FPGA_TIMEOUT;
        ptISO6B_LockQueryResponse->wResultLength = 0;
        ptISO6B_LockQueryResponse->wRssi = 0xffff;
        llrp_tagISO6BLockQueryRsp(ptRcvMsg);

        break;
    default:
        break;
    }
}
/**************************************************************************
* �������ƣ�llrp_dealRcpKillRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpKillRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcKillRes * ptEPC_KillResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_C1GOperationType_Kill:
        ptEPC_KillResponse = (T_USPUHFEpcKillRes *)(ptRcvMsg + 1);
        ptEPC_KillResponse->wChannelIndex = 0xffff;
        ptEPC_KillResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
        ptEPC_KillResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
        ptEPC_KillResponse->wResult = ERR_FPGA_TIMEOUT;
        ptEPC_KillResponse->wRssi = 0xffff;
        llrp_tagEpcKillRsp(ptRcvMsg);
        break;
    default:
        break;
    }
}
/**************************************************************************
* �������ƣ�llrp_dealRcpBlockLockRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpBlockLockRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcBlockLockRes * ptEPC_BlockLockResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    ptEPC_BlockLockResponse = (T_USPUHFEpcBlockLockRes *)(ptRcvMsg + 1);
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_C1G2_BLOCKLOCK:
        ptEPC_BlockLockResponse->ucReadLock = 1;
        break;
    case LLRP_C1G2_BLOCKLOCKQUERY:
        ptEPC_BlockLockResponse->ucReadLock = 0;
        break;
    default:
        break;
    }
    ptEPC_BlockLockResponse = (T_USPUHFEpcBlockLockRes *)(ptRcvMsg + 1);
    ptEPC_BlockLockResponse->wChannelIndex = 0xffff;
    ptEPC_BlockLockResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
    ptEPC_BlockLockResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
    ptEPC_BlockLockResponse->wResult = ERR_FPGA_TIMEOUT;
    ptEPC_BlockLockResponse->wRssi = 0xffff;
    llrp_tagEpcBlockLockRsp(ptRcvMsg);
}
/**************************************************************************
* �������ƣ�llrp_dealRcpReadLockRspTimeOut
* ����������Rcp����ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpReadLockRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcReadLockRes * ptEPC_ReadLockResponse = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    ptEPC_ReadLockResponse = (T_USPUHFEpcReadLockRes *)(ptRcvMsg + 1);
    ptEPC_ReadLockResponse->wChannelIndex = 0xffff;
    ptEPC_ReadLockResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
    ptEPC_ReadLockResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
    ptEPC_ReadLockResponse->wResult = ERR_FPGA_TIMEOUT;
    ptEPC_ReadLockResponse->wRssi = 0xffff;
    llrp_tagEpcReadLockRsp(ptRcvMsg);
}
/**************************************************************************
* �������ƣ�llrp_dealRcpIPJQTRspTimeOut
* ����������Rcpimpinj��Ӧ��ʱ�ĳ�ʱ��Ӧ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
static void llrp_dealRcpIPJQTRspTimeOut(T_Msg *ptRcvMsg)
{
    T_USPUHFEpcIPJQTRes * ptEPC_IPJQTResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRdpLlrpListOpCtrlSet = NULL;
    T_RdpLlrpAntInv *ptRdpLLrpAntInv = NULL;
    T_RdpLlrpProtocolInvSet *ptRdpLlrpProtocolInvSet = NULL;
    ptRdpLlrpListOpCtrlSet = llrp_getRunListOpSet();
    ptRdpLLrpAntInv = llrp_getRunAnt();
    ptRdpLlrpProtocolInvSet = llrp_getRunAP();
    ptEPC_IPJQTResponse = (T_USPUHFEpcIPJQTRes *)(ptRcvMsg + 1);
    switch (ptRdpLlrpListOpCtrlSet->tRdpLlrpOPCtr[ptRdpLlrpListOpCtrlSet->wOpIndex].wOpType)
    {
    case LLRP_ImpinjGetQTConfig:
        ptEPC_IPJQTResponse->wReadOrWrite = 0;
        break;
    case LLRP_ImpinjSetQTConfig:
        ptEPC_IPJQTResponse->wReadOrWrite = 1;
        break;
    default:
        break;

    }
    ptEPC_IPJQTResponse->wChannelIndex = 0xffff;
    ptEPC_IPJQTResponse->wAntennaIndex = ptRdpLLrpAntInv->ucAntIndex;
    ptEPC_IPJQTResponse->wProtoType = ptRdpLlrpProtocolInvSet->eProtocolID;
    ptEPC_IPJQTResponse->wResult = ERR_FPGA_TIMEOUT;
    ptEPC_IPJQTResponse->wRssi = 0xffff;
    llrp_tagEpcIPJQTRsp(ptRcvMsg);
}
/**************************************************************************
* �������ƣ�llrp_dealRcpRspTimeOut
* ����������Rcp���س�ʱ�Ĵ���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/02/24    V1.0       ����        ����
*****************************************************************************/
void llrp_dealRcpRspTimeOut(T_Msg *ptRcvMsg)
{
   /*ҵ�����ʱ�䳬ʱ����Ϣ���кŸ�ֵ��������Ϣ����������ҪУ��*/
    ptRcvMsg->wMsgSequence = g_wMsgSequence;
    switch (ptRcvMsg->wMsgID)
    {
    case TFpgaResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an Inv timeout\r\n");
            llrp_dispatchTagInv();
            break;
        }
    case TFpgaReadResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an Read timeout\r\n");
            llrp_dealRcpReadRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaWriteResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an Write timeout\r\n");
            llrp_dealRcpWriteRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaLockResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an lock timeout\r\n");
            llrp_dealRcpLockRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaKillResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an Kill timeout\r\n");
            llrp_dealRcpKillRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaBlockLockResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an BlockLock timeout\r\n");
            llrp_dealRcpBlockLockRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaAlienReadLockResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an AlienReadlock timeout\r\n");
            llrp_dealRcpReadLockRspTimeOut(ptRcvMsg);
            break;
        }
    case TFpgaIPJQTResp:
        {
            RdpLlrpDbgPrint("(LLRP)receive an ipj timeout\r\n");
            llrp_dealRcpIPJQTRspTimeOut(ptRcvMsg);
            break;
        }
    case TClientRequestOpSpecTimeout:
        {
            RdpLlrpDbgPrint("(LLRP)receive an clientreques timeout\r\n");
            llrp_clientRequestOpTimeout(&g_tClientRequestOpSet);
            break;
        }
    default:
        {
            break;
        }
    }
}


