/***************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�ufhllrpGB.c
* �ļ���ʶ��
* ����ժҪ��LLRPISO6BЭ��ģ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�lzy
* ������ڣ�2014.02.20
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

extern unsigned char g_ucLocalReportDataBuffer[LLRP_MAX_REPORT_BUFFER];
extern unsigned char g_aucRdpLlrpSndMsg[RDP_MAX_LEN_MSG];
extern unsigned char g_aucRdpLlrpRcvMsg[RDP_MAX_LEN_MSG];
extern T_RdpConfig g_tRdpConfig;
extern T_RdpLlrpReaderDispatch g_tRdpLlrpReaderDispatch;
extern unsigned short int g_wMsgSequence;
extern unsigned short int g_wInnerMsgSequence;
extern T_RdpLlrpPwdSys g_tRdpPwdSys;

unsigned int g_bLlrpGBAccessFlag = FALSE;

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
static void llrp_refreshGBReadOpResult(T_USPUHFGBReadRes *ptRcpGBReadRes);
static void llrp_refreshGBWriteOpResult(T_USPUHFGBWriteRes *ptRcpGBWriteRes);
static void llrp_refreshGBEraseOpResult(T_USPUHFGBEraseRes *ptRcpGBEraseRes);
static void llrp_refreshGBLockOpResult(T_USPUHFGBLockRes *ptRcpGBLockRes);
static void llrp_refreshGBKillOpResult(T_USPUHFGBKillRes *ptRcpGBKillRes);

/**************************************************************************
* �������ƣ�llrp_assembleGBInvProtocolPara
* ���������������������е�Э�����
* ���ʵı���
* �޸ĵı���
* ���������unsigned char ucAntennaIndex
                             ����ID
                            T_RdpLlrpProtocolInvSet  *ptTempProtocolInvSet
                             ��������µ�GBЭ��Ĳ���
* ���������T_USPUHFGBInventory *ptUSPUHFGBInventory
                            ���� ������
* �� �� ֵ��   0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/03/20    V1.0       lzy        ����
*****************************************************************************/
void llrp_assembleGBInvPara( unsigned char ucAntennaIndex,
                              T_RdpLlrpProtocolInvSet *ptTempProtocolPara,
                              T_USPUHFGBInventory *ptUSPUHFGBInventory)
{
    unsigned short int wloop = 0;
    T_USPRdpLlrpAntennaConfiguration *ptAntConfig = NULL;
    T_USPRdpLlrpAntennaExtConfiguration *ptAntExtConfig = NULL;
    T_USPRdpLlrpGBFilterList *ptGBFilterList = NULL;
    T_USPUHFGBInventoryMaskList *ptGBInventoryMaskList = NULL;
    RdpLlrpDbgPrint("(LLRP) llrp_assembleGBInvPara in \r\n");
    ptUSPUHFGBInventory->ucAntennaID = ucAntennaIndex;
    ptUSPUHFGBInventory->ucProtoType = ptTempProtocolPara->eProtocolID;
    ptAntConfig = &g_tRdpConfig.tUSPRdpLlrpAntennaConfiguration[ucAntennaIndex - 1];
    ptAntExtConfig = &g_tRdpConfig.trAntennaExtConfiguration[ucAntennaIndex - 1];
    /*�������ü̳� ֱ�Ӹ�ϵͳĬ�ϵ�ֵ*/
    ptGBInventoryMaskList = &ptUSPUHFGBInventory->tGBInventoryPara.tGBInventoryMaskList;
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.bIsDefaultCfgFlag == TRUE)
    {
    	ptUSPUHFGBInventory->tGBInventoryPara.wCCN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCCN;
	 ptUSPUHFGBInventory->tGBInventoryPara.wCIN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCIN;
        ptUSPUHFGBInventory->tGBInventoryPara.wSession
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucSession;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureyAction
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureyAction;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureySel
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureySel;
	ptUSPUHFGBInventory->tGBInventoryPara.wModeindex
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBRFControl.wModeIndex;
       ptGBFilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBFilter;
        ptGBInventoryMaskList->wMaskNum = ptGBFilterList->wFilterNum;
        for (wloop = 0;wloop < ptGBInventoryMaskList->wMaskNum;wloop++)
        {

            ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortAction
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareAction;
	     ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortTarget
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareTarget;
            memcpy(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData, \
                   ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.ucMaskData, \
                   sizeof(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData));
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskBitPointer \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskBitPointer;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskLength \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskLength;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMemBank \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMemBank;
        }
		
        memcpy (ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannel,
                sizeof(ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.wFreqHopMode;
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.wHopTimer;
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFixFreqChannel;
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFreqTableID;
        ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType
        = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFreqWorkType;
        ptUSPUHFGBInventory->tInventoryRfPara.wTransmitPower
        = ptAntExtConfig->tTransmitPower.wTransmitPowerGB;
    }
    else
    {
        if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.bRFTransmitterCtrl == TRUE)
        {
            memcpy (ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    sizeof(ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.wFreqHopMode;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.wHopTimer;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFixFreqChannel;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFreqTableID;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFreqWorkType;
            ptUSPUHFGBInventory->tInventoryRfPara.wTransmitPower
            = ptAntExtConfig->tTransmitPower.wTransmitPowerGB;
        }
        else
        {
            memcpy (ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannel,
                    sizeof(ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannel));
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel.wFreqChannelNum
            = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.tFreqHopConfig.tfreqChannel.wFreqChannelNum;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wFreqHopMode = LLRP_FreqHopType_Sequence;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.wHopTimer = 1;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFixFreqChannel = ptAntExtConfig->tUSPRdpLlrpRfFreqCfg.tUSPUHFRfFreqCfgGB.wFixFreqChannel;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqTableID
            = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpRFTransmitterSettings.wHopTableID;
            ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqWorkType = TRUE;
            ptUSPUHFGBInventory->tInventoryRfPara.wTransmitPower
            = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpRFTransmitterSettings.wTransmitPower;
            /*������Ƶ Э���ʼ����Ƶ��*/
            llrp_frequencyTransfer(ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.wFreqTableID,
                                   &ptUSPUHFGBInventory->tInventoryRfPara.tFreqCfg.tFreqHopConfig.tfreqChannel);
        }

        /*���Э��̳У�ֱ����ϵͳĬ��ֵ*/
        if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpGBInvCtrl.bIsDefaultCfgFlag == TRUE)
        {
            ptUSPUHFGBInventory->tGBInventoryPara.wCCN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCCN;
	 ptUSPUHFGBInventory->tGBInventoryPara.wCIN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCIN;
        ptUSPUHFGBInventory->tGBInventoryPara.wSession
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucSession;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureyAction
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureyAction;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureySel
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureySel;
	ptUSPUHFGBInventory->tGBInventoryPara.wModeindex
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBRFControl.wModeIndex;
       ptGBFilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBFilter;
        ptGBInventoryMaskList->wMaskNum = ptGBFilterList->wFilterNum;
        for (wloop = 0;wloop < ptGBInventoryMaskList->wMaskNum;wloop++)
        {

            ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortAction
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareAction;
	     ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortTarget
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareTarget;
            memcpy(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData, \
                   ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.ucMaskData, \
                   sizeof(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData));
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskBitPointer \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskBitPointer;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskLength \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskLength;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMemBank \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMemBank;
        }

        }
        else
        {
            llrp_assembleGBInvProtocolPara( ucAntennaIndex, ptTempProtocolPara, ptUSPUHFGBInventory);

        }
	 RdpLlrpDbgPrint("(LLRP) llrp_assembleGBInvPara out \r\n");
    }
}
/**************************************************************************
* �������ƣ�llrp_assembleGBInvProtocolPara
* ���������������������е�Э�����
* ���ʵı���
* �޸ĵı���
* ���������unsigned char ucAntennaIndex
                             ����ID
                            T_RdpLlrpProtocolInvSet  *ptTempProtocolInvSet
                             ��������µ�GBЭ��Ĳ���
* ���������T_USPUHFGBInventory *ptUSPUHFGBInventory
                            ���� ������
* �� �� ֵ��   0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
void llrp_assembleGBInvProtocolPara( unsigned char ucAntennaIndex,T_RdpLlrpProtocolInvSet *ptTempProtocolPara,T_USPUHFGBInventory *ptUSPUHFGBInventory)
{
    unsigned short int wloop = 0;
    T_USPRdpLlrpAntennaConfiguration *ptAntConfig = NULL;
    T_USPRdpLlrpAntennaExtConfiguration *ptAntExtConfig = NULL;
    T_USPRdpLlrpGBFilterList *ptGBFilterList = NULL;
    T_USPUHFGBInventoryMaskList *ptGBInventoryMaskList = NULL;
    RdpLlrpDbgPrint("(LLRP) llrp_assembleGBInvProtocolPara in \r\n");
    ptAntConfig = &g_tRdpConfig.tUSPRdpLlrpAntennaConfiguration[ucAntennaIndex - 1];
    ptAntExtConfig = &g_tRdpConfig.trAntennaExtConfiguration[ucAntennaIndex - 1];
    ptGBInventoryMaskList = &ptUSPUHFGBInventory->tGBInventoryPara.tGBInventoryMaskList;
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpGBInvCtrl.bGBFilterctrl == TRUE)
    {
        ptGBFilterList = &ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings .tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBFilter;
        ptGBInventoryMaskList->wMaskNum = ptGBFilterList->wFilterNum;
        for (wloop = 0;wloop < ptGBInventoryMaskList->wMaskNum;wloop++)
        {

            ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortAction
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareAction;
	     ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortTarget
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareTarget;
            memcpy(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData, \
                   ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.ucMaskData, \
                   sizeof(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData));
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskBitPointer \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskBitPointer;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskLength \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskLength;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMemBank \
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMemBank;
        }
    }
    else
    {
        ptGBFilterList = &ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration \
                           .tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBFilter;
        ptGBInventoryMaskList->wMaskNum = ptGBFilterList->wFilterNum;

        for (wloop = 0;wloop < ptGBInventoryMaskList->wMaskNum;wloop++)
        {
            ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortAction
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareAction;
	     ptGBInventoryMaskList->tGBInventoryMask[wloop].ucSortTarget
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTagInventoryStateAwareFilterAction.ucGBStateAwareTarget;
            memcpy(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData, \
                   ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.ucMaskData, \
                   sizeof(ptGBInventoryMaskList->tGBInventoryMask[wloop].ucMaskData));
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskBitPointer
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskBitPointer;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMaskLength
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMaskLength;
            ptGBInventoryMaskList->tGBInventoryMask[wloop].wMemBank
            = ptGBFilterList->atUSPRdpLlrpGBFilter[wloop].tUSPRdpLlrpGBTargetTagMask.wMemBank;
        }
    }
    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpGBInvCtrl.bGBRFCtrl == TRUE)
    {
        ptUSPUHFGBInventory->tGBInventoryPara.wModeindex
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBRFControl.wModeIndex;
    }
    else
    {

        ptUSPUHFGBInventory->tGBInventoryPara.wModeindex
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings \
          .tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBRFControl.wModeIndex;
    }

    if (ptTempProtocolPara->tRdpLlrpAntConfigCtrl.tRdpLlrpAirProtocolInvCtrl.tRdpLlrpGBInvCtrl.bGBSingulationCtrl == TRUE)
    {
        ptUSPUHFGBInventory->tGBInventoryPara.wCCN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCCN;
	 ptUSPUHFGBInventory->tGBInventoryPara.wCIN
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCIN;
        ptUSPUHFGBInventory->tGBInventoryPara.wSession
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucSession;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureyAction
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureyAction;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureySel
        = ptAntConfig->tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureySel;
    }
    else
    {
	ptUSPUHFGBInventory->tGBInventoryPara.wCCN
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCCN;
	 ptUSPUHFGBInventory->tGBInventoryPara.wCIN
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.wCIN;
        ptUSPUHFGBInventory->tGBInventoryPara.wSession
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucSession;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureyAction
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureyAction;
	ptUSPUHFGBInventory->tGBInventoryPara.ucGBQureySel
        = ptTempProtocolPara->tUSPRdpLlrpAntennaConfiguration.tUSPRdpLlrpAirProtocolInventoryCommandSettings.tUSPRdpLlrpGBInventoryCommand.tUSPRdpLlrpGBSingulationControl.ucGBQureySel;
    }
    RdpLlrpDbgPrint("(LLRP) llrp_assembleGBInvProtocolPara out \r\n");
}

/**************************************************************************
* �������ƣ�llrp_tagGBInvRsp
* ��������������Rcp��GB��㷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
void llrp_tagGBInvRsp(T_Msg *ptRcvMsg)
{
    unsigned int dwRet = 0;
    T_USPUHFGBInventoryRes *ptTempTagGBInvRsp = NULL;
    T_RdpLlrpROCtrlAndParam *ptRunRoCtrlAndPara = NULL;
    T_RdpLlrpAI *ptRdpLlrpAI = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptMatchAccessCtrlAndPara = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBInvRsp\r\n");
    if (g_tRdpLlrpReaderDispatch.ptReaderRoSpecData->tActiveRO.ptNext == NULL)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, 0, 0, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBInvRsp active ro is null\r\n");
        return ;
    }
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBInvRsp wMsgSequence error\r\n");
        return ;
    }
    /*ɱ��ʱ��*/
    LLRP_STOP_TIMER(TFpgaResp);
    /*��õ�ǰ���е�AI*/
    ptRdpLlrpAI = llrp_getRunAI();
    ptTempTagGBInvRsp = (T_USPUHFGBInventoryRes *) (ptRcvMsg + 1);
    /*RO��Report������ ����Trigger�ϱ�*/
    if (ptTempTagGBInvRsp->wResult == 0)
    {
        /*������㵽��ǩ*/
        RdpLlrpDbgPrint("(LLRP)RCP Fpga Return success \r\n");
        if (ptTempTagGBInvRsp->wInventoryFinish == 0)
        {
            /*���Trigger�Ѿ����� �˳����� ��ȡ���Ժ�ı�ǩ*/
            RdpLlrpDbgPrint("(LLRP)RCP Fpga  Return an Tag\r\n");
            ptRdpLlrpAI->tRdpLlrpAIStopTrigger \
            .pfusp_rdp_llrp_AccmulateForAIStopTrigger((void *)&ptRdpLlrpAI->tRdpLlrpAIStopTrigger);
            if ( ptRdpLlrpAI->tRdpLlrpAIStopTrigger \
                    .pfusp_rdp_llrp_judgeHappenForAIStopTrigger((void *)&ptRdpLlrpAI->tRdpLlrpAIStopTrigger) == TRUE )
            {
                RdpLlrpDbgPrint("(LLRP)active AI stoptrigger happen need not to save tag\r\n");
                llrp_sendNextGBInv();
            }
            else
            {
                   llrp_ClearGBAccessFlag();
                    /*ƥ��,�ɹ��˳������ɹ����챨��*/
                    dwRet = llrp_matchAccess(LLRP_AirProtocols_GB,
                                             ptTempTagGBInvRsp->wAntennaIndex,
                                             &ptTempTagGBInvRsp->tTagId);
                    if ( dwRet == LLRP_ACCESS_MATCH_FAIL )
                    {
                        /*���챨��*/
                        RdpLlrpDbgPrint("(LLRP)match Access fail,need to build an Ro Report and insert active Ro report link\r\n");
                        llrp_constructLocalROReport(LLRP_AirProtocols_GB,
                                                    ptTempTagGBInvRsp->wAntennaIndex,
                                                    ptTempTagGBInvRsp->wRssi,
                                                    ptTempTagGBInvRsp->wPC,
                                                    LLRP_DEFAULT_REPORT_WORD16,
                                                    ptTempTagGBInvRsp->wChannelIndex,
                                                    ptTempTagGBInvRsp->tTagId.wTagWordLen,
                                                    ptTempTagGBInvRsp->tTagId.aucTagData);
                        ptRunRoCtrlAndPara = llrp_getRunRO();
                        if (ptRunRoCtrlAndPara == NULL)
                        {
                            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
                            return ;
                        }
                        /*���ӱ�����ۺ��жϱ����Ƿ��ϱ�*/
                        ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport \
                        .pfusp_rdp_llrp_JudgeReportNumForReportTrigger((void *)&ptRunRoCtrlAndPara->tRdpLlrpROSpec.tRdpLlrpROReport);
                        llrp_sendNextGBInv();
                    }
                    else
                    {
                        RdpLlrpDbgPrint("(LLRP)match Access success,inventory over begin do Access Op\r\n");
                        ptMatchAccessCtrlAndPara = llrp_getRunAccess();
                        if (ptMatchAccessCtrlAndPara == NULL)
                        {
                            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
                            return ;
                        }
                        /*��ʼ��Ao��stoptrigger */
                        ptMatchAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessStopTrigger \
                        .pfusp_rdp_llrp_initAOStopTrigger(ptMatchAccessCtrlAndPara);
                        /*����ƥ��ı�ǩ*/
                        ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen = ptTempTagGBInvRsp->tTagId.wTagWordLen * sizeof(unsigned short int);
                        memcpy(ptMatchAccessCtrlAndPara->tMatchUID.aucTagData,
                               ptTempTagGBInvRsp->tTagId.aucTagData, ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen);
                        /*�·�ָ��*/
			   RdpLlrpDbgPrint("(LLRP)ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen =%d \r\n",ptMatchAccessCtrlAndPara->tMatchUID.wTagDataLen);
                        llrp_nextTagAccessOp(&ptMatchAccessCtrlAndPara->tRdpLlrpAccess.tRdpLlrpAccessCmdCtrl.tRdpLlrpListOPCtrlSet,
                                             LLRP_AirProtocols_GB, &ptMatchAccessCtrlAndPara->tMatchUID);
                    }
            }
        }
        /*�������*/
        else
        {
            RdpLlrpDbgPrint("(LLRP)RCP Fpga  Return GB finish frame \r\n");
            /*��Ӧ��Ӧ�Ľ�������Ȼ�����Ro,AI,AP�ĵ���*/
            llrp_dispatchTagInv();
        } /*���������*/
    }
    /*ʧ�ܽ��������������һ����*/
    else
    {
        RdpLlrpDbgPrint("(LLRP)RCP Fpga Return GB failed frame\r\n");
        if (ptTempTagGBInvRsp->wInventoryFinish == 0)
        {
            RdpLlrpDbgPrint("(LLRP)Rcp Fpga return GB failed and need to  continue GB inventory \r\n");
            llrp_sendNextGBInv();
        }
        else
        {
            RdpLlrpDbgPrint("(LLRP)Rcp Fpga return GB failed and need to continue a big GB inventory \r\n");
            llrp_dispatchTagInv();
        }
    }
    /*RO��Report���� ֪��*/
}
/**************************************************************************
* �������ƣ�llrp_refreshGBReadOpResult
* ��������������GB�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
static void llrp_refreshGBReadOpResult(T_USPUHFGBReadRes *ptRcpGBReadRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpGBRead *ptGBReadOp = NULL;
    T_RdpLlrpGBReadOpResult *ptGBReadOpResult = NULL;
    if (ptRcpGBReadRes->wResultLength > 500)
    {
        ptRcpGBReadRes->wResultLength = 32;
        RDP_EXCEPT(RDP_INVALID_STATE, ptRcpGBReadRes->wResultLength, 0, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)GB read return an error len =%d\r\n", ptRcpGBReadRes->wResultLength);
    }
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptGBReadOp = (T_RdpLlrpGBRead *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    ptGBReadOpResult = (T_RdpLlrpGBReadOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];
    ptGBReadOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptGBReadOpResult->dwResult = ptRcpGBReadRes->wResult;
    ptGBReadOpResult->wDataLength = ptRcpGBReadRes->wResultLength;
    memcpy(ptGBReadOpResult->aucData, ptRcpGBReadRes->awReadData, ptRcpGBReadRes->wResultLength * sizeof(unsigned short int));
    /*ȡ��GB������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpGBReadRes->wResult;
    /*���¼��������ֱ��¼ԭʼ�����ȣ���ʼ��ַ�����滺���ַ���������ʵ���ȵ�
       ǰ���������ǲ�������ʹ�õĽ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptGBReadOp->wWordCount;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptGBReadOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen
    = struct_fixed_len(T_RdpLlrpGBReadOpResult, aucData) + ptGBReadOp->wWordCount * sizeof(unsigned short int);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)GB Read Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)MsgLen: %d\r\n", ptRunSingleReportCtrl->wMsgLen);
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptGBReadOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptGBReadOpResult->dwResult);
    RdpLlrpDbgPrint("(LLRP)DataLength: %d\r\n", ptGBReadOpResult->wDataLength);
    RdpLlrpDbgPrint("(LLRP)GB read data:");
    for (dwTestIndex = 0 ;dwTestIndex < ptGBReadOpResult->wDataLength*sizeof(unsigned short int);dwTestIndex++)
    {
        RdpDbgPrintRaw("0x%02x ", ptGBReadOpResult->aucData[dwTestIndex]);
    }
    RdpLlrpDbgPrint("\r\n");
    if (ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType == LLRP_GBReadCustomUID)
    {
        RdpLlrpDbgPrint("read customer uid need cpy read Result to UID area\r\n");
        if (ptGBReadOpResult->dwResult== 0)
        {
            ptRunSingleReportCtrl->tUID.wTagDataLen = ptGBReadOpResult->wDataLength * sizeof(unsigned short int);
            memcpy(ptRunSingleReportCtrl->tUID.aucTagData, ptGBReadOpResult->aucData, ptGBReadOpResult->wDataLength*sizeof(unsigned short int));
        }
        else
        {
            ptRunSingleReportCtrl->tUID.wTagDataLen = 0;
        }
    }

}
/**************************************************************************
* �������ƣ�llrp_tagGBReadRsp
* ��������������Rcp��GB��㷵����Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
void llrp_tagGBReadRsp(T_Msg *ptRcvMsg)
{
    unsigned int dwOpReportIndex = 0;
    T_USPUHFGBReadRes *ptGB_ReadResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBReadRsp\r\n");
    /*�ǵ�һ������*/
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBReadRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaReadResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptGB_ReadResponse = (T_USPUHFGBReadRes*)(ptRcvMsg + 1);
    //llrp_changeRcpResultToLlrpForGBRead(&ptGB_ReadResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_GB,
                                        ptGB_ReadResponse->wAntennaIndex,
                                        ptGB_ReadResponse->wRssi,
                                        0xffff,
                                        0xffff,
                                        ptGB_ReadResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshGBReadOpResult(ptGB_ReadResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshGBReadOpResult(ptGB_ReadResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshGBWriteOpResult
* ��������������GB��д���
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
static void llrp_refreshGBWriteOpResult(T_USPUHFGBWriteRes *ptRcpGBWriteRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpGBWrite *ptGBWriteOp = NULL;
    T_RdpLlrpGBWriteOpResult *ptGBWriteOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptGBWriteOp = (T_RdpLlrpGBWrite *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    ptGBWriteOpResult = (T_RdpLlrpGBWriteOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptGBWriteOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptGBWriteOpResult->dwResult = ptRcpGBWriteRes->wResult;
    ptGBWriteOpResult->wNumWordsWritten = ptRcpGBWriteRes->wResultLength;
    /*ȡ��GBд����ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpGBWriteRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptGBWriteOp->wDataLength;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptGBWriteOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpGBWriteOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)GB Write Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptGBWriteOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptGBWriteOpResult->dwResult);
    RdpLlrpDbgPrint("(LLRP)NumWordsWritten: %d\r\n", ptGBWriteOpResult->wNumWordsWritten);
}

/**************************************************************************
* �������ƣ�llrp_tagGBWriteRsp
* ��������������Rcp��GBд������Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
void llrp_tagGBWriteRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpGBWriteOpResult *ptGBWriteOpResult = NULL;
    T_USPUHFGBWriteRes *ptGB_WriteResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBWriteRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBWriteRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaWriteResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptGB_WriteResponse = (T_USPUHFGBWriteRes*)(ptRcvMsg + 1);
    //llrp_changeRcpResultToLlrpForGBWrite(&ptGB_WriteResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_GB,
                                        ptGB_WriteResponse->wAntennaIndex,
                                        ptGB_WriteResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptGB_WriteResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshGBWriteOpResult(ptGB_WriteResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshGBWriteOpResult(ptGB_WriteResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshGBEraseOpResult
* ��������������GB�Ĳ������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
static void llrp_refreshGBEraseOpResult(T_USPUHFGBEraseRes *ptRcpGBEraseRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpGBErase *ptGBEraseOp = NULL;
    T_RdpLlrpGBEraseOpResult *ptGBEraseOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptGBEraseOp = (T_RdpLlrpGBErase *) & ptRunListOpSet->tSingleOp[ptRunListOpSet->wOpIndex];
    ptGBEraseOpResult = (T_RdpLlrpGBEraseOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptGBEraseOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptGBEraseOpResult->dwResult = ptRcpGBEraseRes->wResult;
    ptGBEraseOpResult->wNumWordsErase = ptRcpGBEraseRes->wResultLength;
    /*ȡ��GB��������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpGBEraseRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = ptGBEraseOp->wWordCount;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = ptGBEraseOp->wWordPointer;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpGBEraseOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)GB Erase Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptGBEraseOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptGBEraseOpResult->dwResult);
    RdpLlrpDbgPrint("(LLRP)wNumWordsErase: %d\r\n", ptGBEraseOpResult->wNumWordsErase);
}

/**************************************************************************
* �������ƣ�llrp_tagGBEraseRsp
* ��������������Rcp��GB����������Ϣ
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
void llrp_tagGBEraseRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpGBEraseOpResult *ptGBEraseOpResult = NULL;
    T_USPUHFGBEraseRes *ptGB_EraseResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBEraseRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*�������쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBEraseRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaEraseResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptGB_EraseResponse = (T_USPUHFGBEraseRes*)(ptRcvMsg + 1);
    //llrp_changeRcpResultToLlrpForGBErase(&ptGB_EraseResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_GB,
                                        ptGB_EraseResponse->wAntennaIndex,
                                        ptGB_EraseResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptGB_EraseResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshGBEraseOpResult(ptGB_EraseResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshGBEraseOpResult(ptGB_EraseResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshGBLockOpResult
* ��������������GB�������
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
static void llrp_refreshGBLockOpResult(T_USPUHFGBLockRes *ptRcpGBLockRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpGBLockOpResult *ptGBLockOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptGBLockOpResult = (T_RdpLlrpGBLockOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptGBLockOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptGBLockOpResult->dwResult = ptRcpGBLockRes->wResult;
    /*ȡ��GB������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpGBLockRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpGBLockOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)GB Lock Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptGBLockOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptGBLockOpResult->dwResult);
}
/**************************************************************************
* �������ƣ�llrp_tagGBLockRsp
* ��������������Rcp����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy          ����
*****************************************************************************/
void llrp_tagGBLockRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpGBLockOpResult *ptGBLockOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFGBLockRes *ptGB_LockResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBLockRsp\r\n");
    /* �쳣������ô��������*/
    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBLockRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaLockResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptGB_LockResponse = (T_USPUHFGBLockRes*)(ptRcvMsg + 1);
    //llrp_changeRcpResultToLlrpForGBLock(&ptGB_LockResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_GB,
                                        ptGB_LockResponse->wAntennaIndex,
                                        ptGB_LockResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptGB_LockResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshGBLockOpResult(ptGB_LockResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshGBLockOpResult(ptGB_LockResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_refreshGBKillOpResult
* ��������������GB�Ķ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy        ����
*****************************************************************************/
static void llrp_refreshGBKillOpResult(T_USPUHFGBKillRes *ptRcpGBKillRes)
{
    unsigned int dwTestIndex = 0;
    unsigned int dwOpCtrlIndex = 0;
    unsigned int dwOpResultAddr = 0;
    T_RdpLlrpSingleReportCtrl *ptRunSingleReportCtrl = NULL;
    T_RdpLlrpAccessTagReport *ptAccessTagReport = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunListOpSet = NULL;
    T_RdpLlrpGBKillOpResult *ptGBKillOpResult = NULL;
    ptRunSingleReportCtrl = (T_RdpLlrpSingleReportCtrl *)g_ucLocalReportDataBuffer;
    ptAccessTagReport = &ptRunSingleReportCtrl->tRdpLlrpTagReport.tAccessTagReport;
    ptRunListOpSet = llrp_getRunListOpSet();
    if (ptRunListOpSet == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    dwOpResultAddr = ptAccessTagReport->tID.tGBID.wTagDataLen * sizeof(unsigned short int) - sizeof(unsigned short int);
    if (ptAccessTagReport->dwOpNum > 0)
    {
        dwOpResultAddr = ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultAddr
                         + ptAccessTagReport->tListofOpResultCtrl[ptAccessTagReport->dwOpNum - 1].wOpResultlen;
    }
    ptAccessTagReport->dwOpNum++;
    dwOpCtrlIndex = ptAccessTagReport->dwOpNum - 1;
    ptGBKillOpResult = (T_RdpLlrpGBKillOpResult *) & ptAccessTagReport->aucData[dwOpResultAddr];

    ptGBKillOpResult->wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptGBKillOpResult->dwResult= ptRcpGBKillRes->wResult;
    /*ȡ��GB������ָ��*/
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wResult = ptRcpGBKillRes->wResult;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalLen = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpOriginalAddr = 0;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultAddr = dwOpResultAddr;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen = sizeof(T_RdpLlrpGBKillOpResult);
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpType = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpType;
    ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpSpecID = ptRunListOpSet->tRdpLlrpOPCtr[ptRunListOpSet->wOpIndex].wOpID;
    ptRunSingleReportCtrl->wMsgLen += ptAccessTagReport->tListofOpResultCtrl[dwOpCtrlIndex].wOpResultlen;
    RdpLlrpDbgPrint("(LLRP)GB Kill Rsp result:\r\n");
    RdpLlrpDbgPrint("(LLRP)OpSpecID: %d\r\n", ptGBKillOpResult->wOpSpecID);
    RdpLlrpDbgPrint("(LLRP)Result: %d\r\n", ptGBKillOpResult->dwResult);
}
/**************************************************************************
* �������ƣ�llrp_tagGBKillRsp
* ��������������Rcpɱ����Ϣ����
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ���������
* �� �� ֵ��   
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/21    V1.0       lzy         ����
*****************************************************************************/
void llrp_tagGBKillRsp(T_Msg *ptRcvMsg)
{
    T_RdpLlrpGBKillOpResult *ptGBKillOpResult = NULL;
    unsigned int dwOpReportIndex = 0;
    T_USPUHFGBKillRes *ptGB_KillResponse = NULL;
    T_RdpLlrpListOPCtrlSet *ptRunLsitOpSet = NULL;
    T_RdpLlrpAccessCtrlAndParam *ptRunAccess = NULL;
    /*�ǵ�һ������*/
    RdpLlrpDbgPrint("(LLRP)llrp_tagGBKillRsp\r\n");
    /* �쳣������ô��������*/

    if (ptRcvMsg->wMsgSequence != g_wMsgSequence)
    {
        /*д���쳣�ָ�״̬��*/
        RDP_EXCEPT(RDP_RO_STATE_ERROR, g_wMsgSequence, ptRcvMsg->wMsgSequence, 0, RDP_LLRP_GB_ID);
        RdpLlrpDbgPrint("(LLRP)llrp_tagGBKillRsp wMsgSequence error\r\n");
        return ;
    }
    LLRP_STOP_TIMER(TFpgaKillResp);
    ptRunAccess = llrp_getRunAccess();
    if (ptRunAccess == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return ;
    }
    ptRunLsitOpSet = llrp_getRunListOpSet();
    ptGB_KillResponse = (T_USPUHFGBKillRes*)(ptRcvMsg + 1);
    //llrp_changeRcpResultToLlrpForGBKill(&ptGB_KillResponse->wResult);
    if (ptRunLsitOpSet->wOpIndex == 0)
    {
        /*���û��幹������*/
        /*������صĺ�ro���Ƶ�����*/
        RdpLlrpDbgPrint("(LLRP)first Opindex need construct AoReport head\r\n");
        llrp_constructLocalAOReportHead(LLRP_AirProtocols_GB,
                                        ptGB_KillResponse->wAntennaIndex,
                                        ptGB_KillResponse->wRssi,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        LLRP_DEFAULT_REPORT_WORD16,
                                        ptGB_KillResponse->wChannelIndex,
                                        ptRunAccess->tMatchUID.wTagDataLen,
                                        ptRunAccess->tMatchUID.aucTagData);

        llrp_refreshGBKillOpResult(ptGB_KillResponse);
    }
    /*���ǵ�һ���������������²������ */
    else
    {
        llrp_refreshGBKillOpResult(ptGB_KillResponse);
    }
    ptRunLsitOpSet->wOpIndex++;
    llrp_dispatchTagAccess();
}
/**************************************************************************
* �������ƣ�llrp_getGBAccessPassword
* ����������GB��Կ�����Ķ�ȡ����ȡ��Կ
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��   0 �ɹ� 
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
unsigned int llrp_getGBAccessPassword(T_USPUHFGBTagId *ptGBTagId, unsigned short int wAntID, unsigned int *pdwAccessPassword)
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
    T_USPUHFGBRead *ptGBRead = NULL;
    T_USPUHFGBReadRes *ptGBReadRsp = NULL;
    /*���û���κ���Կ ����������Կ���� ������Ĭ�ϵ���Կ��ʽ*/
    for (dwIndex = 0;dwIndex < g_tRdpPwdSys.dwPwdIndexNum;dwIndex++)
    {
        /*EPC*/
        if (g_tRdpPwdSys.atPwdIndexPara[dwIndex].dwMembank ==LLRP_MB_EPC)
        {
            RdpLlrpDbgPrint("(LLRP)pwdIndex is GB membank need not read \r\n");
            memcpy(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wPwdIndexBuffer,
                   &ptGBTagId->aucTagData[(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordPointer - 1)*sizeof(unsigned short int)],
                   g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordCount*sizeof(unsigned short int));
            continue;
        }
        T_Msg *ptSndMsg = (T_Msg *)ucSndBuf;
        T_Msg *ptRcvMsg = (T_Msg *)ucRevBuf;
        ptSndMsg->mtype = 1;
        ptSndMsg->wMsgID = EV_USP_UHF_GB_READ;
        ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wDstTno = USP_UHF_TNO;
        ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
        ptSndMsg->wReceiverTno = USP_UHF_TNO;
        ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wInnerMsgSequence);
        ptGBRead = (T_USPUHFGBRead *)(ptSndMsg + 1);
        ptGBRead->dwAntIndex = wAntID;
        ptGBRead->tGBReadPara.ucMB = g_tRdpPwdSys.atPwdIndexPara[dwIndex].dwMembank;
        ptGBRead->tGBReadPara.wWordPointer = g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordPointer;
        ptGBRead->tGBReadPara.wWordCount = g_tRdpPwdSys.atPwdIndexPara[dwIndex].wWordCount;
        /*ƥ��ģ����λ��Ҫת�����ֵ�λ�����wpointΪ15 Ҳ��0��ʼ��*/
        ptGBRead->tGBReadPara.dwAccessPassword = 0;
        ptGBRead->tTagId.wTagWordLen = ptGBTagId->wTagWordLen;
        memcpy(ptGBRead->tTagId.aucTagData, ptGBTagId->aucTagData, (ptGBTagId->wTagWordLen)*sizeof(unsigned short int));
        ptSndMsg->wMsgLen = sizeof(T_USPUHFGBRead) + ptGBTagId->wTagWordLen * sizeof(unsigned short int);
        RdpLlrpDbgPrint("(LLRP)GB get pwdIndex Read\r\n");
        RdpLlrpDbgPrint("(LLRP)AntIndex :%d\r\n", ptGBRead->dwAntIndex);
        RdpLlrpDbgPrint("(LLRP)AccessPassword :0x%x\r\n", ptGBRead->tGBReadPara.dwAccessPassword);
        RdpLlrpDbgPrint("(LLRP)ucMB :%d\r\n", ptGBRead->tGBReadPara.ucMB);
        RdpLlrpDbgPrint("(LLRP)wWordCount :%d\r\n", ptGBRead->tGBReadPara.wWordCount);
        RdpLlrpDbgPrint("(LLRP)wWordPointer :%d\r\n", ptGBRead->tGBReadPara.wWordPointer);
        RdpLlrpDbgPrint("(LLRP)GBId:");
        for (dwTestIndex = 0;dwTestIndex < ptGBRead->tTagId.wTagWordLen*sizeof(unsigned short int);dwTestIndex++)
        {
            RdpDbgPrintRaw("0x%02x  ", ptGBRead->tTagId.aucTagData[dwTestIndex]);
        }
        RdpLlrpDbgPrint("\r\n");
        if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
        {
            /*����ʧ��д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_GB_ID);
            break;
        }
        memset(&tSyncMsg, 0xff, sizeof(T_Msg));
        tSyncMsg.wMsgID = EV_USP_UHF_GB_READ_RES;
        tSyncMsg.wMsgSequence = g_wInnerMsgSequence;
        dwRet = RDP_RECV_SYNC_MSG(req_socket_rcp, ptRcvMsg, RDP_MAX_LEN_MSG);
        if (dwRet < 0 )
        {
            /*д���쳣*/
            RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptRcvMsg->wMsgID, ptRcvMsg->wMsgLen, 0, RDP_LLRP_GB_ID);
            break;
        }
        ptGBReadRsp = (T_USPUHFGBReadRes *)(ptRcvMsg + 1);
        RdpLlrpDbgPrint("(LLRP)pwdIndex read wResult:%d\r\n", ptGBReadRsp->wResult);
        if ( ptGBReadRsp->wResult != 0)
        {
             RdpLlrpDbgPrint("(LLRP)access password error result: %d\r\n", ptGBReadRsp->wResult);
            //break;
        }
        else
        {
            memcpy(g_tRdpPwdSys.atPwdIndexPara[dwIndex].wPwdIndexBuffer, ptGBReadRsp->awReadData, ptGBReadRsp->wResultLength*sizeof(unsigned short int));
        }
    }
    if (dwIndex == g_tRdpPwdSys.dwPwdIndexNum)
    {
        RdpLlrpDbgPrint("(LLRP)get PwdIndex  success pfusp_lib_uhf_getPassword=0x%x\r\n", g_tRdpPwdSys.pfusp_lib_uhf_getPassword);
        if (NULL == g_tRdpPwdSys.pfusp_lib_uhf_getPassword)
        {
            RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
            RdpLlrpDbgPrint("(LLRP)error happened\r\n");
            return LLRP_GB_READ_PWDINDEX_FAIL;
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
            return LLRP_GB_READ_PWDINDEX_FAIL;
        }
        RdpLlrpDbgPrint("(LLRP)use  getPassword success accesspwd=%x\r\n", *pdwAccessPassword);
        return LLRP_GB_READ_PWDINDEX_SUCCESS;
    }
    else
    {
        RdpLlrpDbgPrint("(LLRP)get PwdIndex  failed \r\n");
        return LLRP_GB_READ_PWDINDEX_FAIL;
    }
}
/**************************************************************************
* �������ƣ�llrp_GBMatchRead
* ����������ƥ��GB������
* ���ʵı���
* �޸ĵı���
* ��������TT_USPUHFGBTagId *ptGBTagId,
                             unsigned short int wAntID,
                             T_RdpLlrpGBTargetTag *ptGBTargetTag,
                             T_USPUHFGBReadRes **ptGBReadRsp,
                             T_RdpLlrpAccess*ptMatchAccess
* ���������T_USPUHFGBReadRes **ptGBReadRsp
                             ���ؽ��ָ��
* �� �� ֵ��   LLRP_ACCESS_MATCH_SUCCESS: ƥ��ɹ���
                             LLRP_ACCESS_MATCH_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy          ����
*****************************************************************************/
unsigned int llrp_GBMatchRead(T_USPUHFGBTagId *ptGBTagId, unsigned short int wAntID, T_RdpLlrpGBTargetTag *ptGBTargetTag, T_USPUHFGBReadRes **ptGBReadRsp, T_RdpLlrpAccess*ptMatchAccess)
{
    T_Msg tSyncMsg;
    T_Msg *ptSyncMsg[] = {&tSyncMsg, NULL};
    int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned int dwTestIndex = 0;
    unsigned char ucRevBuf[8192] = {0};
    T_USPUHFGBRead *ptGBRead = NULL;
    T_Msg *ptSndMsg = (T_Msg *)g_aucRdpLlrpSndMsg;
    T_Msg *ptRcvMsg = (T_Msg *)ucRevBuf;
    unsigned int dwAccessPassword = 0;
    if (g_tRdpPwdSys.dwPwdIndexNum == 0)
    {

        RdpLlrpDbgPrint("(LLRP)there is not PwdIndex use default Access funs\r\n");
        //dwRet = llrp_getPassword(ptMatchAccess, &dwAccessPassword);
        dwAccessPassword = 0;
        if (dwRet != 0)
        {
            RdpLlrpDbgPrint("(LLRP)use default getPassword failed\r\n");
            return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
        }
    }
    else
    {
        dwRet = llrp_getGBAccessPassword(ptGBTagId, wAntID, &dwAccessPassword);
        if (dwRet != 0)
        {
            return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
        }
    }
    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgID = EV_USP_UHF_GB_MATCH_READ;
    ptSndMsg->wSrcTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wDstTno = USP_UHF_TNO;
    ptSndMsg->wSenderTno = USP_RDP_LLRP_TNO;
    ptSndMsg->wReceiverTno = USP_UHF_TNO;
    ptSndMsg->wMsgSequence = USP_LIB_SCP_MSGSEQ(g_wMsgSequence);
    ptGBRead = (T_USPUHFGBRead *)(ptSndMsg + 1);
    ptGBRead->dwAntIndex = wAntID;
    ptGBRead->tGBReadPara.ucMB = ptGBTargetTag->ucMB;
    /*ƥ��ģ����λ��Ҫת�����ֵ�λ�����wpointΪ15 Ҳ��0��ʼ��*/
    ptGBRead->tGBReadPara.wWordPointer = ptGBTargetTag->wPointer / 16;
    if (ptGBTargetTag->wPointer % 16 == 0 )
    {
        ptGBRead->tGBReadPara.wWordCount = BIT_CHANGETO_WORD16(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount);

    }
    else
    {
        ptGBRead->tGBReadPara.wWordCount = ( (ptGBTargetTag->wPointer % 16) > 0 ? 1 : 0) \
                                             + ((ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount > (16 - ptGBTargetTag->wPointer % 16)) ? \
                                                BIT_CHANGETO_WORD16(ptGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount - (16 - ptGBTargetTag->wPointer % 16)) : 0);
    }
    ptGBRead->tGBReadPara.dwAccessPassword = dwAccessPassword;
    llrp_setGBAccessFlag();
    ptGBRead->tTagId.wTagWordLen = ptGBTagId->wTagWordLen;
    memcpy(ptGBRead->tTagId.aucTagData, ptGBTagId->aucTagData, (ptGBTagId->wTagWordLen)*sizeof(unsigned short int));
    ptSndMsg->wMsgLen = sizeof(T_USPUHFGBRead) + ptGBTagId->wTagWordLen * sizeof(unsigned short int);
    RdpLlrpDbgPrint("(LLRP)GB Match Read\r\n");
    RdpLlrpDbgPrint("(LLRP)AntIndex :%d\r\n", ptGBRead->dwAntIndex);
    RdpLlrpDbgPrint("(LLRP)AccessPassword :0x%x\r\n", ptGBRead->tGBReadPara.dwAccessPassword);
    RdpLlrpDbgPrint("(LLRP)ucMB :%d\r\n", ptGBRead->tGBReadPara.ucMB);
    RdpLlrpDbgPrint("(LLRP)wWordCount :%d\r\n", ptGBRead->tGBReadPara.wWordCount);
    RdpLlrpDbgPrint("(LLRP)wWordPointer :%d\r\n", ptGBRead->tGBReadPara.wWordPointer);
    RdpLlrpDbgPrint("(LLRP)GBId:");
    for (dwTestIndex = 0;dwTestIndex < ptGBRead->tTagId.wTagWordLen*sizeof(unsigned short int);dwTestIndex++)
    {
        RdpDbgPrintRaw("0x%02x  ", ptGBRead->tTagId.aucTagData[dwTestIndex]);
    }
    RdpLlrpDbgPrint("\r\n");
    if (0 > RDP_SEND_MSG(req_socket_rcp, ptSndMsg))
    {
        /*����ʧ��д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptSndMsg->wMsgID, ptSndMsg->wMsgLen, 0, RDP_LLRP_GB_ID);
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    memset(&tSyncMsg, 0xff, sizeof(T_Msg));
    tSyncMsg.wMsgID = EV_USP_UHF_GB_MATCH_READ_RES;
    tSyncMsg.wMsgSequence = g_wMsgSequence;
    dwRet = RDP_RECV_SYNC_MSG(req_socket_rcp, ptRcvMsg, RDP_MAX_LEN_MSG);
    if (dwRet < 0 )
    {
        /*д���쳣*/
        RDP_EXCEPT(RDP_SEND_MESSGAE_ERROR, ptRcvMsg->wMsgID, ptRcvMsg->wMsgLen, 0, RDP_LLRP_GB_ID);
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    *ptGBReadRsp = (T_USPUHFGBReadRes *)(ptRcvMsg + 1);
    RdpLlrpDbgPrint("(LLRP)matchread wResult:%d\r\n", (*ptGBReadRsp)->wResult);
    if ((*ptGBReadRsp)->wResult != 0)
    {
        return LLRP_ACCESSPATTERN_MATCHREAD_FAIL;
    }
    return LLRP_ACCESSPATTERN_MATCHREAD_SUCCESS;
}

/**************************************************************************
* �������ƣ�llrp_GBMatchAccessPattern
* ����������GBƥ��ģ�����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   LLRP_GBMatchTAG_SUCCESS: ƥ��ɹ���
                             LLRP_GBMatchTAG_FAIL: ƥ��ʧ�ܣ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
unsigned int llrp_GBMatchAccessPattern(unsigned int dwByteLen, unsigned char *pucMatchData , T_RdpLlrpGBTargetTag *ptMatchGBTargetTag)
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
    dwTagMaskAndDataCmpTimes = dwTagMaskAndDataOffset = BIT_CHANGETO_BYTE(ptMatchGBTargetTag->tRdpLlrpMatchParam.wMaskBitCount);
    /*����GB��ƥ��ĵڼ����ֽ�*/
    /*����ֽڵĵڼ���bit*/
    dwTagOffsetBits = (ptMatchGBTargetTag->wPointer) % 8;
    ptMatchData = &ptMatchGBTargetTag->tRdpLlrpMatchParam;
    memcpy(aucTagData, pucMatchData, dwByteLen);
    for (dwIndex = 0; dwIndex < dwTagMaskAndDataCmpTimes; dwIndex++, dwTagDataIndex++)
    {
        /*ȡ��ƥ���GB��ĵ�һ��ƥ���ֽ�*/
        ucTempTagDataFirstHaft = aucTagData[dwTagDataIndex];
        /*������һ��ƥ����ֽ�*/
        ucTempTagDataFirstHaft = ucTempTagDataFirstHaft << dwTagOffsetBits;
        ucTempTagDataLastHaft = aucTagData[dwTagDataIndex + 1];
        ucTempTagDataLastHaft = ucTempTagDataLastHaft >> (8 - dwTagOffsetBits);
        aucTagData[dwTagDataIndex] = ucTempTagDataFirstHaft | ucTempTagDataLastHaft;

        /*�����ƥ�����ģ�� Ӧ�� ��ǩ���Ȱ���λ��ƥ������ ���
         Ȼ���ں������������������� 1^0 = 1 ˵�����ݲ�ƥ��  1&0 =0 ˵��������λ ���Ϊ0
        1^0 =1 ���� 1&1=1 ˵������λ ��ô�����Ͳ�����ģ���� �˳�*/
        RdpLlrpWarningPrint("(LLRP)GB Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                            aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                            ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
        if ((ptMatchGBTargetTag->bMatch == TRUE) \
                && (((aucTagData[dwTagDataIndex] \
                      ^ ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset])\
                     & (ptMatchData->aucTagData[dwIndex])) != 0))
        {

            RdpLlrpWarningPrint("(LLRP)access match pattern is equal the byte cmp fail  GB Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                                aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                                ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
            return LLRP_GBMATCHTAG_FAIL;
        }
        /*����ǲ�ƥ�����ģ�� Ӧ�� ��ǩ���Ȱ���λ��ƥ������ ͬ��
        Ȼ���ں�������������*/
        if ((ptMatchGBTargetTag->bMatch == FALSE)
                && (((~(aucTagData[dwTagDataIndex] \
                        ^ ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]))
                     & (ptMatchData->aucTagData[dwIndex])) != 0))
        {
            RdpLlrpWarningPrint("(LLRP)access match pattern is not equal the byte cmp fail  GB Tagdata :0x%2x,mask data:0x%2x,pattern data:0x%2x\r\n",
                                aucTagData[dwTagDataIndex], ptMatchData->aucTagData[dwIndex],
                                ptMatchData->aucTagData[dwIndex + dwTagMaskAndDataOffset]);
            return LLRP_GBMATCHTAG_FAIL;
        }
    }
    return LLRP_GBMATCHTAG_SUCCESS;
}
/**************************************************************************
* �������ƣ�llrp_setGBAccessFlag
* ��������������GBAcces���ʱ�ʶ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ������Կ���ʱ�ʶ
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
void llrp_setGBAccessFlag()
{
    g_bLlrpGBAccessFlag = TRUE;
}
/**************************************************************************
* �������ƣ�llrp_ClearGBAccessFlag
* ��������������GBAcces���ʱ�ʶ
* ���ʵı���
* �޸ĵı���
* �����������
* ���������
* �� �� ֵ��   ������Կ���ʱ�ʶ
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
void llrp_ClearGBAccessFlag()
{
    g_bLlrpGBAccessFlag = FALSE;
}
/**************************************************************************
* �������ƣ�llrp_getOpGBAccessPassword
* �������������Op��������Կ
* ���ʵı���
* �޸ĵı���
* ���������unsigned int dwOpAccessPassword,
                              T_USPUHFGBTagId * ptGBTagId, 
                              unsigned short int wAntID
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20   V1.0       lzy        ����
*****************************************************************************/
unsigned int llrp_getOpGBAccessPassword(unsigned int dwOpAccessPassword, T_USPUHFGBTagId * ptGBTagId, unsigned short int wAntID)
{
    unsigned int dwRet = 0;
    unsigned int dwAccessPassword = 0;
    if (g_bLlrpGBAccessFlag == TRUE)
    {
        RdpLlrpDbgPrint("(LLRP)g_bLlrpGBAccessFlag has been set return 0\r\n");
        return 0;
    }
    else
    {
        if (g_tRdpPwdSys.dwPwdIndexNum == 0)
        {
            RdpLlrpDbgPrint("(LLRP)PwdIndexNum is zero return op AccessPassword =0x%x\r\n",dwOpAccessPassword);
            llrp_setGBAccessFlag();
            return dwOpAccessPassword;
        }
        else
        {
            dwRet = llrp_getGBAccessPassword(ptGBTagId, wAntID, &dwAccessPassword);
            if (dwRet == 0)
            {
                RdpLlrpDbgPrint("(LLRP)PwdIndexNum is  not zero return psam AccessPassword\r\n");
                llrp_setGBAccessFlag();
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
* �������ƣ�llrp_nextGBAccessOp
* �����������·�access����һ��Op
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptSndMsg ,
                            �·�����Ϣ
                            T_USPUHFGBTagId *ptGBTagId
                            ��ǩ��GB����
* ���������
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2014/02/20    V1.0       lzy        ����
*****************************************************************************/
unsigned int llrp_nextGBAccessOp(T_Msg *ptSndMsg , T_RdpLlrpListOPCtrlSet*ptlistOpCtrlSet, void *ptTagID)
{
    unsigned int dwRet = 0;
    unsigned int dwIndex = 0;
    unsigned char *ptByte = NULL;
    unsigned int dwOpIndex = 0;
    unsigned short int wLoop=0;
    T_RdpLlrpAntInv *ptRunAntInv = NULL;
    T_RdpLlrpUID *ptGBTagId = NULL;
    T_RdpLlrpGBRead *ptTempLLRPGBRead = NULL;
    T_RdpLlrpGBWrite*ptTempLLRPGBWrite = NULL;
    T_RdpLlrpGBLock *ptTempLLRPGBLock = NULL;
    T_RdpLlrpGBKill *ptTempLLRPGBKill = NULL;
    T_USPUHFGBRead *ptUSPUHFGBRead = NULL;
    T_USPUHFGBWrite *ptUSPUHFGBWrite = NULL;
    T_USPUHFGBLock *ptUSPUHFGBLock = NULL;
    T_USPUHFGBKill *ptUSPUHFGBKill = NULL;
    T_USPUHFGBTagId *ptUHFGBTagId = NULL;
    T_RdpLlrpGBErase*ptTempLLRPGBErase = NULL;
    T_USPUHFGBErase *ptUSPUHFGBErase = NULL;
	
    dwOpIndex = ptlistOpCtrlSet->wOpIndex;
    ptRunAntInv = llrp_getRunAnt();
    if (ptRunAntInv == NULL)
    {
        RDP_EXCEPT(RDP_NULLPOINTER_EXCEPTION, 0, 0, 0, RDP_LLRP_GB_ID);
        return RDP_NULLPOINTER_EXCEPTION;
    }
    ptGBTagId =(T_RdpLlrpUID *)ptTagID;
    RdpLlrpDbgPrint("(LLRP)dwOpIndex:%d,wOpType:%d wTagDataLen:%d\r\n", dwOpIndex, ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType,ptGBTagId->wTagDataLen);
    switch (ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType)
    {
    case LLRP_GBOperationType_Read:
    case LLRP_GBReadCustomUID:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_GBOperationType_Read\r\n");
            ptUSPUHFGBRead = (T_USPUHFGBRead *)(ptSndMsg + 1);
            ptTempLLRPGBRead = (T_RdpLlrpGBRead *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFGBRead->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFGBRead->tGBReadPara.ucMB = ptTempLLRPGBRead->ucMB;
            ptUSPUHFGBRead->tGBReadPara.wWordPointer = ptTempLLRPGBRead->wWordPointer;
            ptUSPUHFGBRead->tGBReadPara.wWordCount = ptTempLLRPGBRead->wWordCount;
            ptUSPUHFGBRead->tTagId.wTagWordLen = ptGBTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFGBRead->tTagId.aucTagData, ptGBTagId->aucTagData, ptGBTagId->wTagDataLen);
            ptUSPUHFGBRead->tGBReadPara.dwAccessPassword = llrp_getOpGBAccessPassword(ptTempLLRPGBRead->dwReadPassword, &ptUSPUHFGBRead->tTagId, ptUSPUHFGBRead->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_GB_READ;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFGBRead) + ptGBTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaReadResp, TFpgaRespCount);
            break;
        }
    case LLRP_GBOperationType_Write:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_GBOperationType_Write\r\n");
	     
            ptUSPUHFGBWrite = (T_USPUHFGBWrite *)(ptSndMsg + 1);
            ptTempLLRPGBWrite = (T_RdpLlrpGBWrite *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFGBWrite->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFGBWrite->tGBWritePara.ucMB = ptTempLLRPGBWrite->ucMB;
            ptUSPUHFGBWrite->tGBWritePara.wWordPointer = ptTempLLRPGBWrite->wWordPointer;
            ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen = ptTempLLRPGBWrite->wDataLength;
            memcpy(ptUSPUHFGBWrite->tGBWritePara.aucData,
                   ptTempLLRPGBWrite->aucData,
                   ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen*sizeof(unsigned short int));
            RdpLlrpDbgPrint("(LLRP)ptGBTagId->wTagDataLen =%d  ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen=%d\r\n",
				ptGBTagId->wTagDataLen,ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen);
            ptUHFGBTagId = &(ptUSPUHFGBWrite->tTagId);
            ptUHFGBTagId->wTagWordLen = ptGBTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUHFGBTagId->aucTagData, ptGBTagId->aucTagData, ptGBTagId->wTagDataLen);
            ptUSPUHFGBWrite->tGBWritePara.dwAccessPassword = llrp_getOpGBAccessPassword(ptTempLLRPGBWrite->dwWritePassword, ptUHFGBTagId, ptUSPUHFGBWrite->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_GB_WRITE;
            ptSndMsg->wMsgLen = struct_fixed_len(T_USPUHFGBWrite, tTagId)
                                + struct_fixed_len(T_USPUHFGBTagId, aucTagData)
                                + ptUHFGBTagId->wTagWordLen * sizeof(unsigned short int);
		RdpLlrpDbgPrint("(LLRP)   %d  ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen=%d   %d  ptUSPUHFGBWrite->tTagId.wTagWordLen=%d ptSndMsg->wMsgLen=%d\r\n",
		 	struct_fixed_len(T_USPUHFGBWrite, tTagId),
		 	ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen,
		 	struct_fixed_len(T_USPUHFGBTagId, aucTagData),
		 	ptUSPUHFGBWrite->tTagId.wTagWordLen,
		 	ptSndMsg->wMsgLen);
	      RdpLlrpDbgPrint("(LLRP)  wWordPointer:%d wWriteWordLen:%d ucMB:%d\r\n",ptUSPUHFGBWrite->tGBWritePara.wWordPointer,
		  	ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen,
		  	ptUSPUHFGBWrite->tGBWritePara.ucMB);
	      for(wLoop=0;wLoop<ptUSPUHFGBWrite->tGBWritePara.wWriteWordLen*2;wLoop++)
	      	{
	      	     RdpDbgPrintRaw("0x%x ",ptUSPUHFGBWrite->tGBWritePara.aucData[wLoop]);
	      	}
		RdpLlrpDbgPrint("\r\n");
	      RdpLlrpDbgPrint("ptUSPUHFGBWrite->tTagId.wTagWordLen=%d\r\n",ptUSPUHFGBWrite->tTagId.wTagWordLen);
	       for(wLoop=0;wLoop<ptUSPUHFGBWrite->tTagId.wTagWordLen*2;wLoop++)
	      	{
	      	     RdpDbgPrintRaw("0x%x ",ptUSPUHFGBWrite->tTagId.aucTagData[wLoop]);
	      	}
		RdpLlrpDbgPrint("\r\n");
            LLRP_START_TIMER(TFpgaWriteResp, TFpgaRespCount);
            break;
        }
    case LLRP_GBOperationType_Erase:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_GBOperationType_Erase\r\n");
            ptUSPUHFGBErase = (T_USPUHFGBErase *)(ptSndMsg + 1);
            ptTempLLRPGBErase = (T_RdpLlrpGBErase *) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFGBErase->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFGBErase->tGBErasePara.ucMB = ptTempLLRPGBErase->ucMB;
            ptUSPUHFGBErase->tGBErasePara.wWordPointer = ptTempLLRPGBErase->wWordPointer;
            ptUSPUHFGBErase->tGBErasePara.wEraseWordLen = ptTempLLRPGBErase->wWordCount;
            ptUSPUHFGBErase->tTagId.wTagWordLen = ptGBTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFGBErase->tTagId.aucTagData, ptGBTagId->aucTagData, ptGBTagId->wTagDataLen);
            ptUSPUHFGBErase->tGBErasePara.dwAccessPassword = llrp_getOpGBAccessPassword(ptTempLLRPGBErase->dwWritePassword, &ptUSPUHFGBErase->tTagId, ptUSPUHFGBErase->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_GB_ERASE;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFGBErase) + ptGBTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaEraseResp, TFpgaRespCount);
            break;
        }
    case LLRP_GBOperationType_Lock:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_GBOperationType_Lock\r\n");
            ptUSPUHFGBLock = (T_USPUHFGBLock *)(ptSndMsg + 1);
            ptTempLLRPGBLock = (T_RdpLlrpGBLock*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFGBLock->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
	     ptUSPUHFGBLock->tGBLockPara.ucMB= ptTempLLRPGBLock->ucMB;
	     ptUSPUHFGBLock->tGBLockPara.ucConfig= ptTempLLRPGBLock->ucConfig;
            if(LLRP_GBLockConfig_MemoryMode==ptTempLLRPGBLock->ucConfig)
	    	  {
	    	      ptUSPUHFGBLock->tGBLockPara.ucAeraMode=ptTempLLRPGBLock->tRdpLlrpGBLockAction.ucGBLockMemoryMode;
	    	      RdpLlrpDbgPrint("(LLRP)ucGBLockMemoryMode \r\n");
	    	  }
	    	  else
	    	  {
	    	      ptUSPUHFGBLock->tGBLockPara.ucSecureMode=ptTempLLRPGBLock->tRdpLlrpGBLockAction.ucGBLockSecureMode;
	    	      RdpLlrpDbgPrint("(LLRP)ucGBLockSecureMode \r\n");
	    	  }
            ptUSPUHFGBLock->tTagId.wTagWordLen = ptGBTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFGBLock->tTagId.aucTagData, ptGBTagId->aucTagData, ptGBTagId->wTagDataLen);
            ptUSPUHFGBLock->tGBLockPara.dwAccessPassword = llrp_getOpGBAccessPassword(ptTempLLRPGBLock->dwLockPassword, &ptUSPUHFGBLock->tTagId, ptUSPUHFGBLock->dwAntIndex);
            ptSndMsg->wMsgID = EV_USP_UHF_GB_LOCK;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFGBLock) + ptGBTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaLockResp, TFpgaRespCount);
            break;
        }
    case LLRP_GBOperationType_Kill:
        {
            RdpLlrpDbgPrint("(LLRP)LLRP_GBOperationType_Kill\r\n");
            ptUSPUHFGBKill = (T_USPUHFGBKill *)(ptSndMsg + 1);
            ptTempLLRPGBKill = (T_RdpLlrpGBKill*) & ptlistOpCtrlSet->tSingleOp[dwOpIndex];
            ptUSPUHFGBKill->dwAntIndex = (unsigned int)ptRunAntInv->ucAntIndex;
            ptUSPUHFGBKill->tGBKillPara.dwKillPassword = ptTempLLRPGBKill->dwKillPassword;
            ptUSPUHFGBKill->tTagId.wTagWordLen = ptGBTagId->wTagDataLen / sizeof(unsigned short int);
            memcpy(ptUSPUHFGBKill->tTagId.aucTagData, ptGBTagId->aucTagData, ptGBTagId->wTagDataLen);
            ptSndMsg->wMsgID = EV_USP_UHF_GB_KILL;
            ptSndMsg->wMsgLen = sizeof(T_USPUHFGBKill) + ptGBTagId->wTagDataLen;
            LLRP_START_TIMER(TFpgaKillResp, TFpgaRespCount);
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
            RdpLlrpDbgPrint("(LLRP)receive noknown optype :%d\r\n", ptlistOpCtrlSet->tRdpLlrpOPCtr[dwOpIndex].wOpType);
        }
        break;
    }
    return dwRet;
}

