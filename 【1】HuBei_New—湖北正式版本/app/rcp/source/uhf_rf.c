/***************************************************************************
* ��Ȩ���� (C)2011, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�uhfRf.c
* �ļ���ʶ��
* ����ժҪ������ƵЭ����Ƶ����ģ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ��Զ���
* ������ڣ�2011.12.1
**********************************************************************/

#include "usp.h"
#include "common.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RBCB1)
#include "uhf_dbg.h"
#include "Board.h"
#include "RfCtl.h"
#include "uhf_rf.h"
//#include "uhf_time.h"


/***********************************************************
 *                     ��������                            *
***********************************************************/

/***********************************************************
*                 �ļ��ڲ�ʹ�õĺ�                        *
***********************************************************/

#define UHF_RF_STATE_INIT          (0)
#define UHF_RF_STATE_NEED_CONFIG   (1)
#define UHF_RF_STATE_WORK          (2)

#define RF_REQUEST_CFG_TIME      (1000)
#define RF_AUTO_CLOSE_TIME       (3000)
#define RF_INFORM_MAX_COUNT      (5)
#define UHF_RF_MSG_MAX_SIZE      (2048)
#define RF_REQUEST_CFG_MAX_COUNT (20)

/***********************************************************
*               �ļ��ڲ�ʹ�õ���������                    *
***********************************************************/

/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/

/***********************************************************
 *                     ���ر���                            *
***********************************************************/

extern T_UhfRfAntFreqHopCtrl g_tFreqHopCtrl[READER_ANTENNA_COUNT + 1];             /* ��Ƶ���� */
T_RfCtrl      g_tRfCtrl;

static WORD16 g_awRcvBuf[512];
static WORD16 g_awSndBuf[512];
static WORD16 g_wUhfRfStat = UHF_RF_STATE_INIT;
/* 6600�豸���ͣ�00��01��04Ϊ800M��02��03��05Ϊ900M */
static BYTE g_auc6600DeviceType[] = {PLL_TYPE_800M, PLL_TYPE_800M, PLL_TYPE_900M, \
                                    PLL_TYPE_900M, PLL_TYPE_800M, PLL_TYPE_900M};
/* 6700�豸���ͣ�00~03��08Ϊ800M��04~07��09Ϊ900M */
static BYTE g_auc6700DeviceType[] = {PLL_TYPE_800M, PLL_TYPE_800M, PLL_TYPE_800M, \
                                    PLL_TYPE_800M, PLL_TYPE_900M, PLL_TYPE_900M, \
                                    PLL_TYPE_900M, PLL_TYPE_900M, PLL_TYPE_800M, PLL_TYPE_900M};

/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/

/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/

WORD16 IsFreqCfgChanged(T_USPUHFRfFreqCfg *ptFreqCfg, WORD16 wAntIndex,
                        WORD16 wProtocolType);
WORD32 uhfRf_CheckFrqPara(T_USPUHFRfFreqCfg *ptFreqCfg);
WORD32 uhfRf_SetInventCfgs(T_UhfInventRfConfigPara *ptInventRfConfigPara,
                           T_UhfInventRfConfigRes *ptInventRfCfgRes);
static void uhfRf_SetDefaultCfgs();
static WORD32 uhfRf_SendMsg(WORD16 wSendModule, T_Msg *ptSndMsg);
//extern WORD32 DmpTestProc(T_USPOamDmpTestItemHead *ptItemHead,
//                   T_USPOamDmpTestItemHeadRet *ptItemRetHead, WORD16 *pwResLen);


#if USP_IS_DISTRIBUTED
/**************************************************************************
* �������ƣ�SndPowerOnSuccess
* ����������������ģ�鷢���ϵ�ɹ���Ϣ
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
************************************************************************/
static void uhfRf_SndPowerOnSuccess()
{
    T_Msg *ptSndMsg = (T_Msg *)g_awSndBuf;
    ptSndMsg->mtype = 1;
    ptSndMsg->wMsgID = EV_USP_SCP_PowerOnSuccess;
    ptSndMsg->wSenderTno = USP_UHF_RF_TNO;
    ptSndMsg->wReceiverTno = USP_OSS_SCP_TNO;
    ptSndMsg->wMsgLen = sizeof(T_USPOssScpPowerOnAck);
    ((T_USPOssScpPowerOnAck *)(ptSndMsg + 1))->ucRound = 1;
    uhfRf_SendMsg(USP_OSS_SCP_TNO, ptSndMsg);
    uhfRfDevPrint("send power on success\r\n");
}

/**************************************************************************
* �������ƣ�uhfRf_initDev
* ������������ʼ����ģ�����õ����豸
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ�������㷵�ظ��ϲ�Ĵ����룬0�޴���
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 uhfRf_initDev()
{
    WORD32 dwRet = USP_SUCCESS;

    dwRet = usp_drv_fpga_init();

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("Init fpga faild\n");
        //LAPInsert(FILE_UHF_RF, dwRet, dwRet, 0);
        return dwRet;
    }

    dwRet = usp_drv_epld_init();

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("Init fpga faild\n");
        //LAPInsert(FILE_UHF_RF, dwRet, dwRet, 0);
        return dwRet;
    }

    dwRet = usp_drv_eeprom_init();

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("Init fpga faild\n");
        //LAPInsert(FILE_UHF_RF, dwRet, dwRet, 0);
        return dwRet;
    }

    return USP_SUCCESS;
}


/**************************************************************************
* �������ƣ�PRfInitProc
* ������������Ƶģ���ʼ��״̬����ʼ��Ӳ��
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��   0   ����,ʹ�������ϵ�
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 uhfRf_InitProc(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    uhfRfDevPrint("rf init proc\r\n");

    if (EV_USP_SCP_PowerOn == ptRcvMsg->wMsgID)
    {
        uhfRf_initDev();
        usp_uhf_rf_init();
        uhfRf_SetDefaultCfgs();

        ptSndMsg->mtype = 1;
        ptSndMsg->wMsgID = EV_USP_OAM_CMP_RCPConfigRequest;
        ptSndMsg->wSenderTno = USP_UHF_RF_TNO;
        ptSndMsg->wReceiverTno = usp_oss_scp_getTno(USP_RB_DEVID, usp_oss_scp_getSubTno(USP_OAM_CMP_TNO));;
        ptSndMsg->wMsgLen = 0;

        uhfRf_SendMsg(ptSndMsg->wReceiverTno , ptSndMsg);

        uhfRf_SndPowerOnSuccess();

        usp_oss_timer_setRelTimer(EV_USP_UHF_RF_CFG_REQUEST_TIMEOUT, RF_REQUEST_CFG_TIME, USP_UHF_RF_TNO, NULL);
        g_wUhfRfStat = UHF_RF_STATE_WORK;
    }
    else
    {
        uhfRfDevPrint("rf init state rcv invalid message\r\n");
    }

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�RfWorkProc
* ������������Ƶģ�鹤��״̬���������������ڹ���
״̬�½������ְ��������Ϣ��
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg  ���յ�����Ϣ
                                     T_Msg *ptSndMsg Ҫ���͵���Ϣ
* �����������
* �� �� ֵ��   0   ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 uhfRf_WorkProc(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    T_UhfInventRfConfigPara *ptInventRfcfgPara = NULL;
    T_UhfInventRfConfigRes  *ptInventRfcfgRes = NULL;
    struct timeval tStartTime;
    struct timeval tEndTime;
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wPLLConfigStatus = UHF_RF_PLL_CONFIGED;

    ptInventRfcfgPara = (T_UhfInventRfConfigPara *)(ptRcvMsg + 1);
    ptInventRfcfgRes = (T_UhfInventRfConfigRes *)(ptSndMsg + 1);

    GetCurrUsTime(&tStartTime);

    ptSndMsg->wMsgSequence = ptRcvMsg->wMsgSequence;
    uhfRfDevPrint("uhf rf workproc \r\n\r\n");

    switch (ptRcvMsg->wMsgID)
    {
        case EV_USP_OAM_CMP_CfgParaChg:
        case EV_USP_COMM_LinkOpened:
        case EV_USP_UHF_RF_CFG_REQUEST_TIMEOUT:
        {
            ptSndMsg->mtype = 1;
            ptSndMsg->wMsgID = EV_USP_OAM_CMP_RCPConfigRequest;
            ptSndMsg->wSenderTno = USP_UHF_RF_TNO;
            ptSndMsg->wReceiverTno = usp_oss_scp_getTno(USP_RB_DEVID, usp_oss_scp_getSubTno(USP_OAM_CMP_TNO));
            ptSndMsg->wMsgLen = 0;
            /*ÿ�ν��������һ�����໷�����ⶪʧ�澯*/
            uhfRf_CheckPLLStatus(&wPLLConfigStatus);
            uhfRf_SendMsg(ptSndMsg->wReceiverTno , ptSndMsg);
            usp_oss_timer_setRelTimer(EV_USP_UHF_RF_CFG_REQUEST_TIMEOUT, RF_REQUEST_CFG_TIME, USP_UHF_RF_TNO, NULL);
            break;

        }

        case EV_USP_OAM_CMP_RCPConfigRequestAck:
        {
            usp_oss_timer_killRelTimer(EV_USP_UHF_RF_CFG_REQUEST_TIMEOUT);
            uhfRfDevPrint("rf: rcved rf cfgs\r\n");
            uhfRf_SetCfgs((T_USPUHFReaderCfg *)(ptRcvMsg + 1));
            break;

        }

        case EV_USP_UHF_INVENT_RF_CFG:
        {
            uhfRfDevPrint("uhf rf rcved invent rf cfg request\r\n");
            dwRet = uhfRf_SetInventCfgs(ptInventRfcfgPara, ptInventRfcfgRes);
            ptInventRfcfgRes->wResult = dwRet;
            ptSndMsg->mtype = 1;
            ptSndMsg->wMsgID = EV_USP_UHF_INVENT_RF_CFG_RES;
            ptSndMsg->wSenderTno = USP_UHF_RF_TNO;
            ptSndMsg->wReceiverTno = usp_oss_scp_getTno(USP_RB_DEVID, usp_oss_scp_getSubTno(USP_UHF_TNO));
            ptSndMsg->wMsgLen = sizeof(T_UhfInventRfConfigRes);
            uhfRf_SendMsg(ptSndMsg->wReceiverTno, ptSndMsg);
            break;

        }
        
        case EV_USP_OAM_DMP_AppRequest:
        {
            uhfRf_CreateDmpResMsg(ptRcvMsg, ptSndMsg);
            ptSndMsg->wSenderTno = USP_UHF_RF_TNO;
            ptSndMsg->wReceiverTno = usp_oss_scp_getTno(USP_RB_DEVID, usp_oss_scp_getSubTno(USP_OAM_DMP_TNO));
            uhfRf_SendMsg(ptSndMsg->wReceiverTno, ptSndMsg);
            break;

        }

        default:
        {
            uhfRfDevPrint("rf work rcv invalid message %d\r\n", ptRcvMsg->wMsgID);
            break;
        }
    }

    GetCurrUsTime(&tEndTime);
    uhfRfDevPrint("rf work proc time %dms\r\n",
                  CalMsTimeInterval(&tStartTime, &tEndTime));
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhfRf_entry
* ������������Ƶ����ģ�������ں���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��   0   ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 usp_uhfRf_entry(void)
{
    WORD32 dwRet = USP_SUCCESS;
    static WORD16 wRfPowerState = RF_AMP_OFF;
    T_Msg *ptRcvMsg = (T_Msg *)g_awRcvBuf;
    T_Msg *ptSndMsg = (T_Msg *)g_awSndBuf;


    while (1)
    {
        if (RF_AMP_ON == wRfPowerState)
        {
            uhfRfDevPrint("uhfRf: set 3s timer\r\n");
            usp_oss_timer_setRelTimer(EV_USP_UHF_RF_TIME_OUT,
                                      RF_AUTO_CLOSE_TIME,
                                      USP_UHF_RF_TNO, NULL);
        }

        dwRet = usp_oss_scp_recvTaskMsg(USP_UHF_RF_TNO, ptRcvMsg, UHF_RF_MSG_MAX_SIZE, USP_OSS_IPC_FOREVERWAIT);

        if (RF_AMP_ON == wRfPowerState)
        {
            uhfRfDevPrint("uhfRf: kill 3s timer\r\n");
            usp_oss_timer_killRelTimer(EV_USP_UHF_RF_TIME_OUT);
        }

        if (USP_SUCCESS != dwRet)
        {
            //LAPInsert(FILE_UHF_RF, dwRet, ptRcvMsg->wMsgID, 0);
            uhfRfDevPrint("uhfRf:rcv mesg error\r\n");
            continue;
        }

        uhfRfDevPrint("uhfRf:rcv message id %x serialnu %d\r\n",
                      ptRcvMsg->wMsgID, ptRcvMsg->wMsgSequence);

        if (EV_USP_UHF_RF_TIME_OUT == ptRcvMsg->wMsgID)
        {
            uhfRfDevPrint("3s timer reach\r\n");

            wRfPowerState = RF_AMP_OFF;

            if (UHF_RF_STATE_WORK == g_wUhfRfStat)
            {
                uhfRfDevPrint("rf Really closed\r\n");
                usp_uhf_rf_txSet(UHF_RF_STOP_TX);
                usp_uhf_rf_setAmp(RF_AMP_OFF);
                SetDATTtoZero();
            }

            continue;
        }

        if (RF_AMP_OFF == wRfPowerState)
        {
            usp_uhf_rf_setAmp(RF_AMP_ON);
            wRfPowerState = RF_AMP_ON;
        }

        switch (g_wUhfRfStat)
        {
            case UHF_RF_STATE_INIT:
            {
                uhfRf_InitProc(ptRcvMsg, ptSndMsg);
                break;
            }
            case UHF_RF_STATE_WORK:
            {
                uhfRf_WorkProc(ptRcvMsg, ptSndMsg);
                break;
            }
            default:
            {

                break;
            }
        }
    }

    return USP_SUCCESS;
}
/**************************************************************************
* �������ƣ�uhfRf_SendMsg
* ������������Ƶ����ģ����Ϣ�����ܺ��������д�ģ
  ����Ϣ�ķ��Ͷ��ɴ˺�������
* ���ʵı���
* �޸ĵı���
* ���������WORD16 wSendModule ���ն������
                                 T_Msg *ptSndMsg   ������Ϣ
* �����������
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/
static WORD32 uhfRf_SendMsg(WORD16 wSendModule, T_Msg *ptSndMsg)
{
    /* send message to destination process */
    WORD32 dwRet = USP_SUCCESS;
    uhfRfDevPrint("uhfRf: Send msg id %x serialnu %d\r\n",
                  wSendModule, ptSndMsg->wMsgSequence);

    if (0xFFFF == wSendModule)
    {
        uhfRfDevPrint("no need to seed message\r\n");
        return USP_SUCCESS;

    }

    dwRet = usp_oss_scp_sendTaskMsg(wSendModule, ptSndMsg, USP_OSS_IPC_NOWAIT);

    if (USP_SUCCESS != dwRet)
    {
        /* ������Ϣʧ����δ�������Ϣ�����������źŴ��*/
        uhfRfDevPrint("msgsnd to %d  fail (errno = %d) \r\n", wSendModule, errno);
        //LAPInsert(FILE_UHF_RF, USP_UHF_SND_MSG_ERROR, dwRet, 0);
        return dwRet; /*����ʧ��*/
    }

    return USP_SUCCESS;

} /* WORD32 uhf_SendMsg(WORD16 wModuleID, T_Msg *ptMsg) */


#endif

/**************************************************************************
* �������ƣ�uhfRf_CreateDmpResMsg
* ��������������DMP�ṩ����Ϣ��������Ƶ����ϲ���
* ���ʵı���
* �޸ĵı���
* ���������T_Msg *ptRcvMsg
* ��������� T_Msg *ptSndMsg
* �� �� ֵ��   ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 2012/3��2    RUP      �Զ���        ����
**************************************************************************/

void uhfRf_CreateDmpResMsg(T_Msg *ptRcvMsg, T_Msg *ptSndMsg)
{
    //T_USPOamDmpTestItemHead *ptTestItemHead
    //= (T_USPOamDmpTestItemHead *)(ptRcvMsg + 1);
    //T_USPOamDmpTestItemHeadRet *ptItemRetHead
    //= (T_USPOamDmpTestItemHeadRet *)(ptSndMsg + 1);
    WORD16 wResLen = 0;

   // DmpTestProc(ptTestItemHead, ptItemRetHead, &wResLen);
    ptSndMsg->wMsgID = EV_USP_OAM_DMP_AppResponse;
    //ptSndMsg->wMsgLen
    //= sizeof(T_USPOamDmpTestItemHeadRet)
    //  + sizeof(T_USPOamDmpDataItemRet);
    ptSndMsg->mtype = 1;
}

/**************************************************************************
* �������ƣ�uhfRf_SetInventCfgs
* �������������ʱ����������Ƶ����
* ���ʵı���
* �޸ĵı���
* ���������T_UhfInventRfConfigPara *ptInventRfConfigPara  ��Ƶ����
* ��������� T_UhfInventRfConfigRes *ptInventRfCfgRes  ���÷���
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 uhfRf_SetInventCfgs(T_UhfInventRfConfigPara *ptInventRfConfigPara,
                           T_UhfInventRfConfigRes *ptInventRfCfgRes)
{
    WORD32 dwRetVal = USP_SUCCESS;

    /* ������Ƶ���� */
    dwRetVal = uhfRf_CalRFPara(ptInventRfConfigPara);

    if (USP_SUCCESS != dwRetVal)
    {
        return dwRetVal;
    }

    ptInventRfCfgRes->wAntIndex  = g_tRfCtrl.wAntIndex;
    ptInventRfCfgRes->wChannelNo =
        uhfRf_getFreqWorkChannel(g_tRfCtrl.wAntIndex,
                                 ptInventRfConfigPara->wProtocolType);

    uhfRfDevPrint("uhfRf:rtn ant %d chnnel no %d\r\n",
                  ptInventRfCfgRes->wAntIndex,
                  ptInventRfCfgRes->wChannelNo);

    /* ��Ƶ���� */
    dwRetVal = uhfRf_startAllAction();
    /*wzp modify delete USP_VIR_TAG_USE   and set 1*/
    #if 1
        return USP_SUCCESS;
    #endif

    if (dwRetVal != 0)
    {
        UHFErrorPrint("uhfRf: uhfRf_startAllAction failed !!! \r\n");
        return dwRetVal;
    }


    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�SetDefaultCfgs
* ����������������û���յ����ְ�������Ϣʱ����
                                 ��Ƶ����
* ���ʵı���
* �޸ĵı���
* �����������
* ��������� T_UhfInventRfConfigRes *ptInventRfCfgRes  ���÷���
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

static void uhfRf_SetDefaultCfgs()
{
    WORD16 wAntIndex  =  1;
    T_USPUHFRfFreqCfg *ptFreqCfg = NULL;
    WORD16  wNeedCfgPower = NO_NEED_RESET_POWER;   /* AntPowerChg */
    BYTE aucPowerTable[] = RF_POWER_TABLE;
    BYTE ucPowerIndex = 5; /*Ĭ��5�Ź���*/

    g_tRfCtrl.wLKCLSwitch = 1;
    g_tRfCtrl.wRcvDatt = 0x3f;

    for (wAntIndex = 1; wAntIndex <= READER_ANTENNA_COUNT; wAntIndex++)
    {
        ucPowerIndex = 8;

        g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[0]
        = aucPowerTable[ucPowerIndex];
        g_tRfCtrl.atRfAntCfg[wAntIndex].wRcvOrSnd = 0;
        g_tRfCtrl.atRfAntCfg[wAntIndex].wAgcDatt = 0x3f;
        g_tRfCtrl.atRfAntCfg[wAntIndex].wInventPower
        = aucPowerTable[ucPowerIndex];
        g_tRfCtrl.atRfAntCfg[wAntIndex].ucRevFilterSwitch = 1;

    }

    memset(g_tFreqHopCtrl, 0,
           sizeof(T_UhfRfAntFreqHopCtrl) * (READER_ANTENNA_COUNT + 1));

    uhfRf_SetRevLowNoEn(LOW_NOISE_DISABLE);
    usp_uhf_rf_setRevDatt(g_tRfCtrl.wRcvDatt);

}


/**************************************************************************
* �������ƣ�SetRfCfgs
* �������������յ���Ƶ�����Ժ����ñ�Ҫ����
* ���ʵı���
* �޸ĵı���
* �����������
* ���������T_UhfRfConfigPara *ptRfConfigPara ���ְ巢�͹�������Ƶ����
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/
void uhfRf_SetCfgs(T_USPUHFReaderCfg *ptRfConfigPara)
{
    WORD16 wAntIndex  =  1;
    T_USPUHFRfFreqCfg *ptFreqCfg = NULL;
    WORD16  wNeedCfgPower = NO_NEED_RESET_POWER;   /* AntPowerChg */
    BYTE aucPowerTable[] = RF_POWER_TABLE;
    BYTE ucPowerIndex = 0;
    WORD16 wLoop = 0;
    WORD16 wPower = 0;
    WORD16 wPowerChanged = 0;

	DEBUG_MSG("uhfRf_SetCfgs start\n");
	fflush(stdout);

    /*�����ֲ�������������������ֺͼ�⵽�Ĳ�һ��
           ���޸�Ϊ���ô���õ�*/
    
    if (g_tRfCtrl.wFreqType != ptRfConfigPara->wFreqType)
    {
        g_tRfCtrl.wFreqType = ptRfConfigPara->wFreqType;
        uhfRfDevPrint("uhfRf: rcved diffrent freq type %d\r\n",
                                                g_tRfCtrl.wFreqType);
        uhfRf_FreqHopInit();
    }
    
    g_tRfCtrl.wLKCLSwitch = ptRfConfigPara->ucTurnOnLKCL;
    g_tRfCtrl.wRcvDatt =  ptRfConfigPara->ucRecvDATT;

    uhfRfDevPrint("uhfRf:rf lkcl %d rcvdata %x  freqtype %d\r\n",
                  g_tRfCtrl.wLKCLSwitch, g_tRfCtrl.wRcvDatt,
                  g_tRfCtrl.wFreqType);
	DEBUG_MSG("uhfRf_SetCfgs 1\n");
	fflush(stdout);

    for (wAntIndex = 1; wAntIndex <= READER_ANTENNA_COUNT; wAntIndex++)
    {
        uhfRfDevPrint("uhfRf: rf cfg ant %d", wAntIndex);
        /*��������ȷ�Ĺ���ֵ���뵽�������ݽṹ��*/
        uhfRfDevPrint("power index");

        for (wLoop = 0; wLoop < UHF_RF_ANT_POWER_COUNT; wLoop++)
        {
            ucPowerIndex = ptRfConfigPara->tAntRfCommonCfg[wAntIndex - 1]\
                           .aucAntPower[wLoop];
            uhfRfDevPrint("%d ", ucPowerIndex);

            if (ucPowerIndex < sizeof(aucPowerTable))
            {
                wPower = aucPowerTable[ucPowerIndex];

                if (g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[wLoop] != wPower)
                {
                    g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[wLoop] = wPower;
                    wPowerChanged = 1;
                }
            }
            else
            {
                g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[wLoop] = 0;
            }
        }

        uhfRfDevPrint("\r\n");
		
		DEBUG_MSG("uhfRf_SetCfgs 1\n");
		fflush(stdout);
        /*��ʼ������Ϊ0�Ź���*/
        g_tRfCtrl.atRfAntCfg[wAntIndex].wInventPower =
            g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[0];
        g_tRfCtrl.atRfAntCfg[wAntIndex].wRcvOrSnd =
            ptRfConfigPara->tAntRfCommonCfg[wAntIndex - 1].ucForandRevConfig;
        g_tRfCtrl.atRfAntCfg[wAntIndex].wAgcDatt
        = ptRfConfigPara->tAntRfCommonCfg[wAntIndex - 1].wLbtDattValue;

        uhfRfDevPrint("uhfRf: power %d wAgcDatt 0x%x\r\n",
                      g_tRfCtrl.atRfAntCfg[wAntIndex].aucAntPower[0],
                      g_tRfCtrl.atRfAntCfg[wAntIndex].wAgcDatt);

        g_tRfCtrl.atRfAntCfg[wAntIndex].ucRevFilterSwitch = 1;


        ptFreqCfg
        = &ptRfConfigPara->tAntEpcRfFreqCfg[wAntIndex - 1];

        /*6c Э���Ƶ�ʱ�����*/
        if (USP_SUCCESS != uhfRf_CheckFrqPara(ptFreqCfg))
        {
            //LAPInsert(FILE_UHF_RF, USP_UHF_RCVED_PARA_ERROR, 0, 0);
            return;
        }

        if (1 == IsFreqCfgChanged(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_EPC))
        {
            uhfRf_SetFreqCfg(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_EPC);
        }

        ptFreqCfg
        = &ptRfConfigPara->tAntGBRfFreqCfg[wAntIndex - 1];

        /*GB Э���Ƶ�ʱ�����*/
        if (USP_SUCCESS != uhfRf_CheckFrqPara(ptFreqCfg))
        {
            //LAPInsert(FILE_UHF_RF, USP_UHF_RCVED_PARA_ERROR, 0, 0);
            return;
        }

        if (1 == IsFreqCfgChanged(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_GB))
        {
            uhfRf_SetFreqCfg(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_GB);
        }

        /*6bЭ���Ƶ�ʱ�����*/
        ptFreqCfg
        = &ptRfConfigPara->tAntISO6BRfFreqCfg[wAntIndex - 1];


        if (USP_SUCCESS != uhfRf_CheckFrqPara(ptFreqCfg))
        {
            //LAPInsert(FILE_UHF_RF, USP_UHF_RCVED_PARA_ERROR, 0, 0);
            return;
        }

        if (1 == IsFreqCfgChanged(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_ISO6B))
        {
            uhfRf_SetFreqCfg(ptFreqCfg, wAntIndex, UHF_RF_PROTOCOL_ISO6B);
        }

    }

    usp_uhf_rf_setRevDatt(g_tRfCtrl.wRcvDatt);

    uhfRfDevPrint("uhfRf:wwww wPowerChanged = %d\r\n",wPowerChanged);

    if (1 == wPowerChanged)
    {
        uhfRf_RefreshPower();
        uhfRfDevPrint("uhfRf:RfPowerSet happend\r\n");
    }

}

/**************************************************************************
* �������ƣ�RfReset
* ������������ʼ����Ƶ������������ò�ͬ��Ƶ���
                                ��ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/
WORD32 uhfRf_Reset()
{
    usp_uhf_rf_setAmp(RF_AMP_OFF); //�رչ��ŵ�Դ

    /* LBT���ܹر� */
    //LBT_switch(LBT_OFF);

    /*��ʼ��Ӳ��������ز���*/
    uhfRf_BoardReset();

    return USP_SUCCESS;
}


/**************************************************************************
* �������ƣ�RfInit
* ������������ʼ����Ƶ������������ò�ͬ��Ƶ���
                                ��ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��   0   �������������ؽ������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/
WORD32 usp_uhf_rf_init()
{

    WORD16 wFreqType = 0;

    uhfRf_InitCtrlStruct();

    uhfRfDevPrint("enter usp_uhf_rf_init\r\n");
    wFreqType = usp_uhf_rf_GetFreqType();
    uhfRfDevPrint("UHF: FreType = %d  0==800M   1==900M\r\n", wFreqType);

    if (wFreqType == PLL_TYPE_800900M)
    {
        g_tRfCtrl.wFreqType = PLL_TYPE_800900M;
        g_tRfCtrl.wMaxFreqTable = FREQ800900MTABLE_NUM;
    }
    else if(wFreqType == PLL_TYPE_900M)
    {
        g_tRfCtrl.wFreqType = PLL_TYPE_900M;
        g_tRfCtrl.wMaxFreqTable = FREQ900MTABLE_NUM;
    }else
    {
        g_tRfCtrl.wFreqType = PLL_TYPE_800M;
        g_tRfCtrl.wMaxFreqTable = FREQ800MTABLE_NUM;
    }

    usp_uhf_rf_setAmp(RF_AMP_OFF); //�رչ��ŵ�Դ

    /* LBT���ܹر� */
    //LBT_switch(LBT_OFF);

    /* ��ʼ����Ƶ��800M 900M���Ǵ�idΪ0��ʼ*/
    uhfRf_FreqHopInit();

    /*��ʼ��Ӳ��������ز���*/
    uhfRf_BoardInit();

    /*��ʼ��ʱ�趨���ʳ�ʼ��ֵ*/
    usp_uhf_rf_AutoCalibrateInit();

    return USP_SUCCESS;


}

/**************************************************************************
* �������ƣ�GetFreqType
* ��������������Ķ�����Ƶ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��   0   �������������ؽ������
* ����˵����RUP֮ǰ�İ汾���õ���eeprom�е�����
                                ֮�������Ƶ���д�epld�Ĵ����л�ȡ
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
* 12/3/21      RUP     zdx      �����˶���RUP��epld��ȡƵ�����͵�
* 12/09/07             �ڽ�          ����6600C1/C2��6700C1/C2��Ƶ������
**************************************************************************/
#define EEPROM_READER_SERIAL  (0x000e)
WORD16 usp_uhf_rf_GetFreqType()
{
    WORD32 dwRet = USP_SUCCESS;
    WORD32 dwRcvLen = 0;
    BYTE   ucFreqType = PLL_TYPE_800M;

#ifdef FREQ_TYPE_STORE_IN_EPLD

    BYTE  ucFreqEpldType = 0;
    dwRet = usp_drv_epld_read(0x5, &ucFreqEpldType);

    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("uhf:read epld error! %x\r\n", dwRet);
        return 0;
    }

    if (0x5 == ucFreqEpldType)
    {
        ucFreqType = PLL_TYPE_800M;
    }
    else if (0x6 == ucFreqEpldType)
    {
        ucFreqType = PLL_TYPE_900M;
    }
    else
    {
        ucFreqType = PLL_TYPE_800900M;
    }

#else
    BYTE   aucFreqType[2] = {0};
#if EEPROM_SWITCH
    dwRet = usp_drv_eeprom_rcv(EEPROM_READER_SERIAL, 2, aucFreqType, &dwRcvLen);
#endif
    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("uhf:read eeprom error! addr %x\r\n", dwRet);
    }

    uhfRfDevPrint("ReaderSerial is ...%2x%2x\r\n", aucFreqType[0], aucFreqType[1]);

    /*��ȡ�Ķ����ĳ������кţ������Ϊ800M����900M*/
    /* Ŀǰֻ��800M��900M�����豸��800M/900M��һ�İ汾��ʱû�й滮 */
    if (0 == aucFreqType[0])/* 6600 */
    {
        ucFreqType = g_auc6600DeviceType[aucFreqType[1]];
    }
    else/* 6700 */
    {
        ucFreqType = g_auc6700DeviceType[aucFreqType[1]];
    }

#endif

    return ucFreqType;
}


/**********************************************************************
* �������ƣ� uhfRf_CalRFPara
* ���������� ������Ƶ����
* ��������� ������
* ���������
* �� �� ֵ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010\11\23               zdx                          ����
***********************************************************************/
WORD32 uhfRf_CalRFPara(T_UhfInventRfConfigPara *ptInventRfConfigPara)
{
    WORD16 wPowerIndex = 0;
    WORD16 wAntIndex = 0;
    T_USPUHFRfFreqCfg *ptFreqCfg = NULL;
    T_AntennaCfg *ptAntCfgs = NULL;
    BYTE aucPowerTable[] = RF_POWER_TABLE;


    if (NULL == ptInventRfConfigPara)
    {
        uhfRfDevPrint("ptInventRfConfigPara NULL\r\n");
        return  USP_UHF_RCVED_PARA_ERROR;
    }

    wAntIndex = ptInventRfConfigPara->wAntIndex;

    if ((wAntIndex > READER_ANTENNA_COUNT) || (wAntIndex == 0))
    {
        uhfRfDevPrint("Anttna Index  %d not present\r\n", wAntIndex);
        return USP_UHF_RCVED_PARA_ERROR;
    }

    g_tRfCtrl.wAntIndex = wAntIndex;
    uhfRfDevPrint("the antennaindex is %d\r\n", wAntIndex);

    g_tRfCtrl.wProtocolType = ptInventRfConfigPara->wProtocolType;

    usp_uhf_rf_setRFPortocolType(g_tRfCtrl.wProtocolType);
    
    ptAntCfgs =  &g_tRfCtrl.atRfAntCfg[wAntIndex];
    uhfRf_ChooseRevFilter(ptInventRfConfigPara->wRevDataRate,
                          g_tRfCtrl.atRfAntCfg[wAntIndex].ucRevFilterSwitch);

    /* R=>T����ģʽ       00������    01��DSB-ASK     10��SSB-ASK              11��PR-ASK*/
    usp_uhf_rf_setRTModuleType(ptInventRfConfigPara->wModulateType);
    uhfRf_SetForDataRate(ptInventRfConfigPara->wForDataRate);
    /*����EPC�ĵ������*/
    usp_uhf_rf_setModulateDepth(ptInventRfConfigPara->wModulateDepth);
    uhfRfDevPrint("rf %d/%d modulate type %d  modulate depth %d\r\n",
                  ptInventRfConfigPara->wForDataRate,
                  ptInventRfConfigPara->wRevDataRate,
                  ptInventRfConfigPara->wModulateType,
                  ptInventRfConfigPara->wModulateDepth);

    /* ��¼������� */

#if USE_INVENTORY_RF_CONFIG

    /*����·��Ľṹ��û����Ƶ����������ʹ��Ĭ������*/
    if (RF_CFG_NOT_EXIST == ptInventRfConfigPara->wRfCfgExist)
    {
        uhfRf_FreqHopCtrl(wAntIndex, ptInventRfConfigPara->wProtocolType);
        return  USP_SUCCESS;
    }

    wPowerIndex = ptInventRfConfigPara->tRfCfg.wTransmitPower;

    if (wPowerIndex > 18)
    {
        UHFErrorPrint(" PowerIndex  error the value is  %d \r\n", wPowerIndex);
        wPowerIndex = 16;
    }

    ptAntCfgs->wInventPower = aucPowerTable[wPowerIndex];
    uhfRfDevPrint("rf invent power %d\r\n", ptAntCfgs->wInventPower);

    ptFreqCfg = &(ptInventRfConfigPara->tRfCfg.tFreqCfg);

    if (USP_SUCCESS != uhfRf_CheckFrqPara(ptFreqCfg))
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if (1 == IsFreqCfgChanged(ptFreqCfg, wAntIndex, ptInventRfConfigPara->wProtocolType))
    {
        uhfRf_SetFreqCfg(ptFreqCfg, wAntIndex, ptInventRfConfigPara->wProtocolType);
    }
    else
#endif
        uhfRf_FreqHopCtrl(wAntIndex, ptInventRfConfigPara->wProtocolType);

    return USP_SUCCESS;

}

#endif

