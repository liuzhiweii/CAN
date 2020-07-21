/*********************************************************************
* ��Ȩ���� (C)2005, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�
* �ļ���ʶ��
* ����ժҪ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�
* ������ڣ�
*
* �޸ļ�¼1��
* �޸ļ�¼2��
* �޸ļ�¼3��
**********************************************************************/

/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include "usp.h"
#include "common.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RBCB1)
#include <math.h>
#include "uhf_dbg.h"
//#include "uhf_time.h"
#include "uhf_trace.h"
#include "uhf_rf.h"
#include "Board.h"
#include "RfCtl.h"
#include "FpgaCtl.h"
#include "Pll.h"
#include "Lkcl.h"


/***********************************************************
 *                     ��������                            *
***********************************************************/
#define FORWARD_FLAG 0
#define BACKWARD_FLAG 1

/***********************************************************
*                 �ļ��ڲ�ʹ�õĺ�                        *
***********************************************************/


/***********************************************************
*               �ļ��ڲ�ʹ�õ���������                    *
***********************************************************/


/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/
extern T_RfCtrl      g_tRfCtrl;
T_USPUHFRfPowerTable    g_tUhfRfPowerTable1;/*��¼��eeprom�еĶ�������*/
T_USPUHFRfPowerTable    g_tUhfRfPowerTable2;
T_USPUHFRfPowerTable    g_tUhfRfPowerTable3;
T_USPUHFRfPowerTable    g_tUhfRfPowerTable4;
T_USPUHFRfAntPower   g_atAntPower[READER_ANTENNA_COUNT + 1];
T_RfInitPara         g_tRfInitPara;
T_USPUHFGateVoltageCtrl  g_tUhfGateVoltageCtrl;

#if 0
/* ����״̬LED������� */
const T_AntStatus g_atAntStatLED[] = 
{
    {USP_OSS_LED_FLASH_ANT1_EXIST, USP_OSS_LED_FLASH_ANT1_NOT_EXIST, \
     USP_OSS_LED_FLASH_ANT1_WORK_ERR, USP_OSS_LED_FLASH_ANT1_WORKING},
    {USP_OSS_LED_FLASH_ANT2_EXIST, USP_OSS_LED_FLASH_ANT2_NOT_EXIST, \
     USP_OSS_LED_FLASH_ANT2_WORK_ERR, USP_OSS_LED_FLASH_ANT2_WORKING},
    {USP_OSS_LED_FLASH_ANT3_EXIST, USP_OSS_LED_FLASH_ANT3_NOT_EXIST, \
     USP_OSS_LED_FLASH_ANT3_WORK_ERR, USP_OSS_LED_FLASH_ANT3_WORKING},
    {USP_OSS_LED_FLASH_ANT4_EXIST, USP_OSS_LED_FLASH_ANT4_NOT_EXIST, \
     USP_OSS_LED_FLASH_ANT4_WORK_ERR, USP_OSS_LED_FLASH_ANT4_WORKING}
};
#endif
/***********************************************************
 *                     ���ر���                            *
***********************************************************/


/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/
static BYTE    ACGetPowerTableIndex(WORD16 wVoltage, BYTE ucPTNum,
                                    T_USPUHFRfPowerTableEntry *ptPowerTableREntry);
static WORD16  ComputeVSWR(WORD16 wPTarget, WORD16 wVoltage, WORD16 wAntIndex);
static WORD16  SetK0GetPower(WORD16 wK0, WORD16 wAntIndex);
static void uhfRf_InitACData(BYTE ucTableCount);
static WORD32  uhfRf_SetRfPower(WORD16 wAntIndex, WORD16 wPower);
static WORD32  uhfRf_setCalibrateFreq(WORD16 wAntIndex);
extern WORD32  usp_uhf_rf_readPowerTable(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr);
extern WORD32  usp_uhf_rf_readPRData(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr);

/**********************************************************************
* �������ƣ�void ClearAntPowerResult()
* ��������������洢�����߹���ֵ
* �����������
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/17       RUP       zdx
************************************************************************/
void usp_uhf_rf_ClearPowerResult()
{
    WORD16 wLoop = 0;

    for (wLoop = 0; wLoop <= READER_ANTENNA_COUNT; wLoop++)
    {
        g_atAntPower[wLoop].wVswr = 10000;
        g_atAntPower[wLoop].wResIndex = 0;
        memset(g_atAntPower[wLoop].atPowers, 0,
               UHF_RF_ANT_POWER_COUNT * sizeof(T_USPUHFRfPowerEntry));
    }
}

/**********************************************************************
* �������ƣ�T_AntCalibrate_Result* FindClibrateRes(WORD16 wAntIndex,WORD16
wTargetPower)
* �����������������ߺ͹��ʣ�Ѱ���Ƿ�֮ǰ���ù���Ӧ�Ĺ���
* �����������
* ���������
* �� �� ֵ��
                     NULL��    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/15           RUP              zdx
************************************************************************/
T_USPUHFRfPowerEntry *usp_uhf_rf_FindPowerEntry(WORD16 wAntIndex, WORD16 wTargetPower)
{
    WORD16 wLoop = 0;

    for (wLoop = 0; wLoop < UHF_RF_ANT_POWER_COUNT; wLoop++)
    {
        if (g_atAntPower[wAntIndex].atPowers[wLoop].wTargetPower ==
            wTargetPower)
        {
            return &g_atAntPower[wAntIndex].atPowers[wLoop];
        }
    }

    return NULL;
}
/**********************************************************************
* �������ƣ�usp_uhf_rf_SetPowerEntry
* ����������������ֵ�洢���ṹ��
* �����������
* ���������
* �� �� ֵ�����߲�����ָ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/8/31               RUP                 zdx
************************************************************************/
void usp_uhf_rf_SetPowerEntry(WORD16 wAntIndex, WORD16 wClibratePower,
                         WORD16 wTargetPower, WORD16 wClibrateK0,
                         WORD16 wDattVal)
{
    T_USPUHFRfPowerEntry *ptRfPowerEntry = NULL;
    WORD16 wTempIndex;
    WORD16 wIndex = g_atAntPower[wAntIndex].wResIndex;

    ptRfPowerEntry = &g_atAntPower[wAntIndex].atPowers[wIndex];
    ptRfPowerEntry->wCalibrateK0 = wClibrateK0;
    ptRfPowerEntry->wCalibratPower = wClibratePower;
    ptRfPowerEntry->wTargetPower = wTargetPower;
    ptRfPowerEntry->wDattVal = wDattVal;

    wTempIndex = wIndex + 1;
    g_atAntPower[wAntIndex].wResIndex =
        (wTempIndex >= UHF_RF_ANT_POWER_COUNT) ? 0 : wTempIndex;
}
/**********************************************************************
* �������ƣ�void RecordAntVswr(WORD16 wAntIndex,WORD16 wVswr)
* ��������������פ����
* �����������
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/8/31       RUP        zdx
************************************************************************/
inline void uhfRf_RecordAntVswr(WORD16 wAntIndex, WORD16 wVswr)
{
    g_atAntPower[wAntIndex].wVswr = wVswr;
}
/**********************************************************************
* �������ƣ�usp_uhf_rf_queryAntVswr
* ������������ȡפ����
* �����������
* ���������
* �� �� ֵ��   ����100�Ժ��פ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/8/31       RUP        zdx
************************************************************************/
WORD16 usp_uhf_rf_queryAntVswr(WORD16 wAntIndex)
{
    return g_atAntPower[wAntIndex].wVswr;
}
/**********************************************************************
* �������ƣ�void uhfRf_startAllAction(void)
* ������������Ƶ������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
#define ANT_NOT_INPOS_NOT_ALARMED (0)
#define ANT_NOT_INPOS_ALARMED     (1)

void uhfRf_dealInventAntStatus(WORD16 wAntIndex, WORD16 wAntStatus)
{
    static BYTE aucAntStatus[READER_ANTENNA_COUNT + 1] = {ANT_NOT_INPOS_NOT_ALARMED};

    if (RF_ANT_NOT_INPOS == wAntStatus)
    {
        if (ANT_NOT_INPOS_NOT_ALARMED == aucAntStatus[wAntIndex])
        {
            aucAntStatus[wAntIndex] = ANT_NOT_INPOS_ALARMED;
            LAPInsert(FILE_UHF_RFCTRL, USP_UHF_INVENT_ANT_NOT_INPOS, wAntIndex, 0);
        }
    }
    else
    {
        if (ANT_NOT_INPOS_ALARMED == aucAntStatus[wAntIndex])
        {
            aucAntStatus[wAntIndex] = ANT_NOT_INPOS_NOT_ALARMED;
        }
    }

}
/**********************************************************************
* �������ƣ�void uhfRf_startAllAction(void)
* ������������Ƶ������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhfRf_startAllAction(void)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wAntIndex = g_tRfCtrl.wAntIndex;
    struct timeval tStartTime;
    struct timeval tEndTime;
    GetCurrUsTime(&tStartTime);

    if (RF_ANT_NOT_INPOS == g_tRfCtrl.atRfAntCfg[wAntIndex].ucAntStause)
    {

        uhfRf_dealInventAntStatus(wAntIndex, RF_ANT_NOT_INPOS);
        uhfRfDevPrint("uhfRf:invent but ant %d not inpositon\r\n", wAntIndex);
        return USP_UHF_INVENT_ANT_NOT_INPOS;
    }

    /* ����ѡ�� */
    dwRet = usp_uhf_rf_selectAnt(wAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:Rf antenna set failed!dwRet = 0x%x\r\n");
        return dwRet;
    }

    dwRet = usp_uhf_rf_removeAntAlarm();

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    dwRet = usp_uhf_rf_setPLLFreq(g_tRfCtrl.dwWorkFreq);

    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("usp_uhf_rf_setPLLFreq error\r\n");
        return dwRet;
    }

    dwRet = uhfRf_SetRfPower(wAntIndex,
                             g_tRfCtrl.atRfAntCfg[wAntIndex].wInventPower * 100);

    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("uhfRf_SetRfPower error!!!  dwRet = 0x%x\r\n", dwRet);
        return dwRet;
    }

    /*����й¶������Ҫ�����΢��*/
    dwRet = uhfRf_FineLKCL(wAntIndex,
                           uhfRf_getFreqHopCtrl(wAntIndex , g_tRfCtrl.wProtocolType));

    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("RF post proc failed\r\n");
        return dwRet;
    }

    dwRet = uhfRf_dealAlarmReport(wAntIndex);

    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("check ant alarmed\r\n");
        return dwRet;
    }

    //RCP_LED_FLASH_ON(g_atAntStatLED[wAntIndex - 1].wAnt_Working);
    
    GetCurrUsTime(&tEndTime);
    uhfRfDevPrint("stage uhfRf_FineLKCL %dms\r\n",
                  CalMsTimeInterval(&tStartTime, &tEndTime));
    return USP_SUCCESS;

} /* void uhfRf_startAllAction(void) */
/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setTxPower(WORD32 dwRfTxPower)
* ����������������Ƶ���͹���
* ���������
            WORD32 dwRfTxPower �� �����趨ֵ
* ���������
* �� �� ֵ��
            WORD32
                         0��    �ɹ�
                      ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 usp_uhf_rf_setTxPower(WORD16 wAntIndex, WORD16 wTargetPower)
{
    T_USPUHFRfPowerEntry *ptPowerEntry = NULL;

    ptPowerEntry = usp_uhf_rf_FindPowerEntry(wAntIndex, wTargetPower);

    if (NULL == ptPowerEntry)
    {
        uhfRfDevPrint("uhfRf: get clibrated entry error\r\n");
        return  USP_UHF_RF_ANT_SET_ERROR;
    }

    /* add by zongjie 2013.1.10 */
    /* ��Ӳ��������ӣ�Ϊ�˽�����������й©�źŹ�������� */
    FpgaRegWrite(REG_DAC_DC_VALUE_REG, 0);/* �ڴ��ز�֮ǰ����0x87�Ĵ���дΪ0 */
    //usp_oss_vos_usleep(DAC_DC_CHANGE_DELAY);/* wzp add zhushi��ʱһ��ʱ�� */
    /* end 2013.1.10 */
    usp_uhf_rf_writeCtrlWord(ptPowerEntry->wCalibrateK0);
    /* �������ù��Ź��� д��ǰ��DATT ˥��ֵ */
    usp_uhf_rf_setACDatt(ptPowerEntry->wDattVal);
    usp_uhf_rf_WriteAntDatt();
    /* ��ȡ��ǰ���ߵĶ��깦��ֵ */
    usp_uhf_rf_SetLO2Datt(ptPowerEntry->wCalibratPower);
    uhfRfDevPrint("uhfRf: use clibrate power %d\r\n", ptPowerEntry->wCalibratPower);
    
    return USP_SUCCESS;

}

/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_txSet(WORD32 dwRfTxAction)
* ����������ǰ����Ƶ���ʷ��俪�ؿ���
* ���������
            WORD32 dwRfTxAction
                                UHF_RF_START_TX��������Ƶ���ʷ��ͣ�
                                UHF_RF_STOP_TX�� �ر���Ƶ���ʷ���

* ���������
* �� �� ֵ��
            WORD32
                         0��    �ɹ�
                      ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
void usp_uhf_rf_txSet(WORD16 wRfTxAction)
{
#if 0
    if (wRfTxAction != FpgaRegRead(SINGLETONE_SEND_CTRL))
    {
        FpgaRegWrite(SINGLETONE_SEND_CTRL, wRfTxAction);
    }
#endif
} /* WORD32 usp_uhf_rf_txSet(WORD32 dwRfTxAction) */
/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setModulateDepth(WORD32 dwModulateDepth)
* ������������Ƶ�����������
* ���ʵı���
* �޸ĵı���
* ���������
            WORD32 dwModulateDepth ��Ƶ������ȣ�
* ���������
* �� �� ֵ��
            WORD32
                         0��    �ɹ�
                      ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010.1.4     V0.0.1       zdx   �޸�
************************************************************************/
void usp_uhf_rf_setModulateDepth(WORD32 dwModulateDepth)
{
    WORD16 wData = 0;

    if (dwModulateDepth > 100)
    {
        dwModulateDepth = 100;
    }

    wData = (WORD16)(((WORD32)(RF_ASK_DAC_HIGH_VALUE * (100 - dwModulateDepth))) / 100);
    FpgaRegWrite(REG_module_depth, wData);

} /* WORD32 usp_uhf_rf_setModulateDepth(WORD32 dwModulateDepth) */


/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setRFPortocolType(WORD32 dwModulateDepth)
* ������������Ƶ�����������
* ���ʵı���
* �޸ĵı���
* ��������� 00 epc 01 iso6B
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010.1.4     V0.0.1       zdx   �޸�
************************************************************************/
void usp_uhf_rf_setRFPortocolType(WORD16 wType)
{
    FpgaRegWrite(REG_RF_PROTOCOL_TYPE,wType);
}

/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setModulateDepth(WORD32 dwModulateDepth)
* ������������Ƶ�����������
* ���ʵı���
* �޸ĵı���
* ���������
            WORD32 dwModulateDepth ��Ƶ���Ʒ�ʽ
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010.1.4     V0.0.1       zdx   �޸�
************************************************************************/
void usp_uhf_rf_setRTModuleType(WORD16 wType)
{
    FpgaRegWrite(reg_module_type, wType);
}

/**********************************************************************
  * �������ƣ�void SetLKCLPower()
  * ��������������й¶������ʱ����Ҫ���������ó�30DB
  * �����������
  * ���������
  * �� �� ֵ��
              WORD32      0��    �ɹ�
                        ������    ʧ��
  * ����˵������
  * �޸�����      �汾��  �޸���      �޸�����
  * ---------------------------------------------------------------------
  * 2006/9/16    V0.0.1       ft          ����
  ************************************************************************/
void uhfRf_SetLKCLPowerPara(BYTE ucAntIndex)
{
    T_USPUHFRfPowerEntry *ptPowerEntry = NULL;

    ptPowerEntry = usp_uhf_rf_FindPowerEntry(ucAntIndex, LKCL_SET_POWER);

    if (NULL == ptPowerEntry)
    {
        usp_uhf_rf_writeCtrlWord(g_atAntPower[ucAntIndex].ptPowerTable->wK0);
        usp_uhf_rf_setACDatt(UHF_RF_LKCL_ATTEN);
        usp_uhf_rf_WriteAntDatt();
        usp_uhf_rf_SetLO2Datt(LKCL_SET_POWER);
        uhfRfDevPrint("uhfRf: LKCL power set use default\r\n");
    }
    else
    {
        usp_uhf_rf_writeCtrlWord(ptPowerEntry->wCalibrateK0);
        usp_uhf_rf_setACDatt(ptPowerEntry->wDattVal);
        usp_uhf_rf_WriteAntDatt();
        usp_uhf_rf_SetLO2Datt(ptPowerEntry->wCalibratPower);
        uhfRfDevPrint("uhfRf:Set LKCL 30 DB\r\n");
    }

}

/**********************************************************************
   * �������ƣ�void uhfRf_SetLKCLPower()
   * ��������������й¶������ʱ����Ҫ���������ó�30DB
   * �����������
   * ���������
   * �� �� ֵ��
                            WORD32      0��    �ɹ�
                            ������    ʧ��
   * ����˵������
   * �޸�����      �汾��  �޸���      �޸�����
   * ---------------------------------------------------------------------
   * 2006/9/16    V0.0.1       ft          ����
   ************************************************************************/
WORD32 uhfRf_SetLKCLPower(BYTE ucAntIndex)
{
    WORD32 dwRet = USP_SUCCESS;

    dwRet = usp_uhf_rf_autoCalibrate(ucAntIndex, LKCL_SET_POWER);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:set lkcl 30db power error\r\n");
        return dwRet;
    }

    uhfRf_SetLKCLPowerPara(ucAntIndex);
    return USP_SUCCESS;
}


/**************************************************************************
* ��������:
* ��������: ��λ����
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2008��9��13��          V1.0              XXXX                XXXX
**************************************************************************/
static Double MW2dBm(Double value)
{
    return 10 * log10(value);
}

/**************************************************************************
* ��������:
* ��������:  ��λ����
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2008��9��13��          V1.0              XXXX                XXXX
**************************************************************************/
static Double DBm2mW(Double value)
{
    return pow(10, value / 10);
}

/**************************************************************************
* ��������:uhfRf_RefreshPower
* ��������:  �������ý��й��ʵ����¶���
* ���ʵı�: ��
* �޸ĵı�: ��
* �������: ��
* �������:��
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2012��1��13��        RUP           XXXX                XXXX
**************************************************************************/
#define POWER_INITIALIZED    (1)
#define POWER_NOT_INITIALIZED (0)
void uhfRf_RefreshPower()
{
    static WORD16 wPowerInited = POWER_NOT_INITIALIZED;

    usp_uhf_rf_removeAntAlarm();
    usp_uhf_rf_setAmp(RF_AMP_ON);
    sleep(g_tRfInitPara.wClibrateWaitTime);

    if (POWER_NOT_INITIALIZED == wPowerInited)
    {
        if (g_tRfCtrl.wLKCLSwitch == 1)
        {
            uhfRf_ReaderLKCLProc();
        }

        wPowerInited = POWER_INITIALIZED;
    }
#if UHF_BOARD_TYPE == BOARD_TYPE_RFCB7
    /* 6600C1/C2��й©������ɺ󣬹ر�й©����DACʱ�ӣ������ɢ��������� */
    FpgaRegWrite(LKCL_DAC_CLK_CTRL, LKCL_DAC_CLK_OFF);
#endif
    ACPowerSetProc();
}

/**************************************************************************
* ��������: ACPowerSetProc
* ��������: �ϵ��Ժ����λ�����߽����ϵ��Զ�����
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:        WORD32 �ϵ��Զ�����ִ�гɹ�
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  07/06/13          V1.0              XXXX                XXXX
**************************************************************************/
WORD32 ACPowerSetProc()
{
    BYTE ucAntIndex = 0;
    WORD16 wPTarget = 0;
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wLoop = 0;
    WORD16 wPaWarmed = 0;

    usp_uhf_rf_ClearPowerResult();

    for (ucAntIndex = 1; ucAntIndex <= READER_ANTENNA_COUNT; ucAntIndex++)
    {
        dwRet = usp_uhf_rf_getAntStatus(ucAntIndex);

        if (USP_SUCCESS == dwRet)
        {
#if (UHF_BOARD_TYPE == BOARD_TYPE_RFCB2)||(UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT)

            if (0 == wPaWarmed)
            {
                uhfRf_warmPA(UHF_RF_WARM_PA_POWER, ucAntIndex);
                wPaWarmed = 1;
            }

#endif
            g_tRfCtrl.atRfAntCfg[ucAntIndex].ucAntStause = RF_ANT_INPOS;

           // RCP_LED_FLASH_ON(g_atAntStatLED[ucAntIndex - 1].wAnt_Exist);
            //RCP_LED_FLASH_OFF(g_atAntStatLED[ucAntIndex - 1].wAnt_Not_Exist);

            for (wLoop = 0; wLoop < UHF_RF_ANT_POWER_COUNT; wLoop++)
            {
                wPTarget = g_tRfCtrl.atRfAntCfg[ucAntIndex].aucAntPower[wLoop] * 100;

                if (0 == wPTarget)
                {
                    break;
                }
                else
                {
                    dwRet = usp_uhf_rf_autoCalibrate(ucAntIndex, wPTarget);

                    if (USP_SUCCESS == dwRet)
                    {
                        uhfRfDevPrint("Ant%d power set %ddb\r\n", ucAntIndex, wPTarget);
                    }
                    else
                    {
                        uhfRfDevPrint("Ant%d power set %ddb failed, dwRet = %d\r\n", ucAntIndex, wPTarget, dwRet);
                    }
                }
            }

        }
        else
        {
            g_tRfCtrl.atRfAntCfg[ucAntIndex].ucAntStause = RF_ANT_NOT_INPOS;
            
            //RCP_LED_FLASH_OFF(g_atAntStatLED[ucAntIndex - 1].wAnt_Exist);
            //RCP_LED_FLASH_ON(g_atAntStatLED[ucAntIndex - 1].wAnt_Not_Exist);
            
            uhfRfDevPrint("Ant %d is absent\r\n", ucAntIndex);
        }

    }

    return USP_SUCCESS;
}
/**************************************************************************
* �������ƣ�ACAutoCalibrateProc
* ������������ָ�������߽��ж������
* ���������BYTE ucAntIndex          ��������
*                               WORD16 wPTarget      Ŀ�깦��
* ���������
* �� �� ֵ��    ����ɹ�  READER_AC_SUCCESS
*                               ����ʧ��  ����
* ����˵����
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 02/08/01	     V1.0	    XXXX	      XXXX
**************************************************************************/
WORD32 usp_uhf_rf_autoCalibrate(BYTE ucAntIndex, WORD16 wPTarget)
{
    WORD32 dwRet = 0;
    T_USPUHFRfPowerEntry *ptPowerEntry = NULL;
    BYTE   aucAlarmInfo[20];
    WORD32 dwAlarmInfoSize = 0;

    uhfRfDevPrint("uhfRf:wPTarget = %d\r\n", wPTarget);
    ptPowerEntry = usp_uhf_rf_FindPowerEntry(ucAntIndex, wPTarget);

    if (NULL != ptPowerEntry)
    {
        uhfRfDevPrint("uhfRf:the power has been autoclibrated\r\n");
        return USP_SUCCESS;
    }

    /* ѡ������*/
    dwRet = usp_uhf_rf_selectAnt(ucAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:Rf antenna set failed!\r\n");
        return USP_UHF_ANT_SET_ERROR;
    }

    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelForward);
    /* д������*/
    usp_uhf_rf_WriteAntDatt();

    usp_uhf_rf_setAmp(RF_AMP_ON);
    /*ȥ�����ܵ�פ������*/
    usp_uhf_rf_removeAntAlarm();

    /*����ʱ��Ҫ��������ʵ�ʷ�����Ƶ��ֵ*/
    dwRet = uhfRf_setCalibrateFreq(ucAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfrf: ac freq set error\r\n");
        return USP_UHF_PLL_CFG_ERROR;
    }

    usp_uhf_rf_txSet(UHF_RF_START_TX);
    dwRet = usp_uhf_rf_CalibrateK0Datt(wPTarget, ucAntIndex);
    usp_uhf_rf_txSet(UHF_RF_STOP_TX);        /* �ع��Ź���Ƶ*/

    if (USP_SUCCESS != dwRet)
    {
        /* ����ʧ�ܣ��ϱ��澯 */
        //RCP_LED_FLASH_ON(g_atAntStatLED[ucAntIndex - 1].wAnt_Work_Err);
        LAPInsert(FILE_UHF_RFCTRL, dwRet, wPTarget, 0);
        uhfRfDevPrint("uhfRf:Send Clibrate Fail to AMP!\r\n");
        dwAlarmInfoSize = snprintf((char *)aucAlarmInfo, sizeof(aucAlarmInfo), "%d", ucAntIndex);
        uhf_amp_report(EV_USP_OAM_AMP_InformReport,
                       R_RF_AC_FAILED_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_SERIOUS, aucAlarmInfo, dwAlarmInfoSize);

        return dwRet;
    }

    return USP_SUCCESS;

}


/**************************************************************************
* �������ƣ�ACGetPowerTableIndex
* �������������ݵ�ѹֵ���Ҽ�������
* ���ʵı�
* �޸ĵı�
* ���������WORD16 wVoltage ��ѹֵ
* ���������
* �� �� ֵ��    WORD32  g_atPowerTable���е�ѹ����������������
* ����˵����
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 02/08/01	     V1.0	    XXXX	      XXXX
**************************************************************************/
static BYTE ACGetPowerTableIndex(WORD16 wVoltage, BYTE ucPTNum,
                                 T_USPUHFRfPowerTableEntry *ptPowerTableREntry)
{
    BYTE ucLoop = 0;

    /* �����ȡ�ĵ�ѹ�ȱ�����͵�ѹ���ͣ�ʹ��ǰ������¼��Ϊ��������*/
    if (wVoltage < ptPowerTableREntry[0].wVoltage)
    {
        return 0;
    }

    /* ����Ѱ�ҵ�ѹ���ڵ�����*/
    for (ucLoop = 0; ucLoop < (ucPTNum - 2); ucLoop++)
    {
        if (wVoltage >= ptPowerTableREntry[ucLoop].wVoltage
            && wVoltage <= ptPowerTableREntry[ucLoop + 1].wVoltage)
        {
            return ucLoop;
        }
    }

    /* �����ȡ��ѹֵ�ȱ��дθߵ�ѹ����,ʹ�����������¼��Ϊ��������*/
    return ucLoop;


}
//#if BOARD_TYPE_RFCB7
#if 0
/**************************************************************************
* �������ƣ�ACComputePower
* �������������RFCB7ʹ�õĶ����㷨������ָ�����ߵ�ǰ�Ĺ���
*           ����1��4ʹ�õĹ�ʽ:Pout=(Ln((D1+k)/34.599))/0.1099
*           ����2��3ʹ�õĹ�ʽ:Pout=(Ln((D2+k)/36.242))/0.1081
* ���ʵı�
* �޸ĵı�
* ���������WORD16 wVoltage ��ѹֵ
*           BYTE ucAntIndex  ��������
* ���������
* �� �� ֵ��WORD16 wCurPower ��ǰ�Ĺ���
* ����˵����
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2012/08/24  V1.0       �ڽ�        ����
**************************************************************************/
SWORD16 uhfRf_ComputePower(WORD16 wVoltage, BYTE ucAntIndex, BYTE flag)
{
    SWORD16 swCurPower = 0;
    Double  dCompensate = 0.0;/* ���㹦�ʲ���ֵ */

    if ((1 == ucAntIndex) || (4 == ucAntIndex))
    {
        swCurPower = (SWORD16)(log(((Double)wVoltage + dCompensate)/34.599)/0.1099);
    }
    else if ((2 == ucAntIndex) || (3 == ucAntIndex))
    {
        swCurPower = (SWORD16)(log(((Double)wVoltage + dCompensate)/36.242)/0.1081);
    }
    else
    {
        uhfRfDevPrint("AntIndex = %d   error!!\r\n", ucAntIndex);
    }
    uhfRfDevPrint("wVoltage = 0x%x, swCurPower = %d\r\n", wVoltage, swCurPower);

    return swCurPower;
}

#else
/**************************************************************************
* �������ƣ�usp_uhf_rf_ComputePower
* ��������������ָ�����ߵ�ǰ�Ĺ���
* ���ʵı�
* �޸ĵı�
* ���������WORD16 wVoltage ��ѹֵ
*                               BYTE ucAntIndex  ��������
* ���������
* �� �� ֵ��    WORD16 wCurPower ��ǰ�Ĺ���
* ����˵����
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 02/08/01	     V1.0	    XXXX	      XXXX
**************************************************************************/
SWORD16 usp_uhf_rf_ComputePower(WORD16 wVoltage, BYTE ucAntIndex, BYTE flag)
{
    BYTE ucTableIndex = 0;
    SWORD16 swPLow = 0;
    SWORD16 swPHigh = 0;
    WORD16 wVLow = 0;
    WORD16 wVHigh = 0;
    SWORD16 swCurPower = 0;
    T_USPUHFRfPowerTable   *ptPowerTable = g_atAntPower[ucAntIndex].ptPowerTable;
    T_USPUHFRfPowerTableEntry  *ptPowerTableREntry = NULL;

    if (FORWARD_FLAG == flag)
    {
        ptPowerTableREntry = ptPowerTable->atFwTable;
        ucTableIndex = ACGetPowerTableIndex(wVoltage, RF_FORWARD_PT_NUM,
                                            ptPowerTableREntry);
    }
    else
    {
        ptPowerTableREntry = ptPowerTable->atBwTable;
        ucTableIndex = ACGetPowerTableIndex(wVoltage, RF_BACKWARD_PT_NUM,
                                            ptPowerTableREntry);
    }



    /* ���㵱ǰ��K0 �͹���ֵ*/
    wVLow = ptPowerTableREntry[ucTableIndex].wVoltage;
    wVHigh = ptPowerTableREntry[ucTableIndex + 1].wVoltage;
    swPLow = ptPowerTableREntry[ucTableIndex].swPower;
    swPHigh = ptPowerTableREntry[ucTableIndex + 1].swPower;
    swCurPower = swPLow + ((wVoltage - wVLow) * (swPHigh - swPLow)) / (wVHigh - wVLow);

    return swCurPower;
}
#endif
/**************************************************************************
* �������ƣ�uhfRf_WriteCtrlWord
* �������������ÿ�����K0
* ���ʵı�
* �޸ĵı�
* ���������WORD16 wCrtlWord  д��Ŀ�����K0
* ���������
* �� �� ֵ��    WORD16  �ɹ�����0
* ����˵����
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 02/08/01	     V1.0	    XXXX	      XXXX
*12/6/4            BT          zdx           ���Ӷ���K0ֵΪ��ֵʱ�Ŀ���
**************************************************************************/
#define UHF_RF_MAX_POSITIVE_K0    (0x6000)
#define UHF_RF_MIN_NEGATIVE_K0    (0xA000)
void usp_uhf_rf_writeCtrlWord(WORD16 wCrtlWord)
{
    if ((wCrtlWord > UHF_RF_MIN_NEGATIVE_K0) || (wCrtlWord < UHF_RF_MAX_POSITIVE_K0))
    {
        FpgaRegWrite(REG_FORWARD_PWADJ, wCrtlWord);
    }
    else
    {
        //LAPInsert(FILE_UHF_RFCTRL, USP_UHF_RF_K0_ABNORMAL, wCrtlWord, 0);
    }

    uhfRfDevPrint("uhfRf:write ctl word %x\r\n", wCrtlWord);
}

/**************************************************************************
* ��������:usp_uhf_rf_readDetectVal
* ��������: ��ȡ��⵽�ĵ�ѹֵ
* ���ʵı�:
* �޸ĵı�:
* �������: B
* �������:
* ����ֵ:       WORD16    ��ȡ���ĵ�ѹֵ
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  12/8/2              RUP               zdx        
**************************************************************************/
#define DETECTCOUNT 5
WORD16 usp_uhf_rf_readDetectVal()  
{
  WORD16 wLoop=0;
  WORD16 wReadData=0;
  
  for(wLoop=0;wLoop<DETECTCOUNT;wLoop++)
  {
   // wReadData += FpgaRegRead(POWER_DETECT_VAL);
  }
  
  wReadData/=DETECTCOUNT;
  return wReadData;
}

/**************************************************************************
* ��������: usp_uhf_rf_getAntStatus
* ��������: �ж������Ƿ���λ
* ���ʵı�:
* �޸ĵı�:
* �������: BYTE ucAntIndex ��������
* �������:
* ����ֵ:       WORD32    0 ������λ
*                                               ���� ���߲���λ
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  07/06/13          V1.0              XXXX                XXXX
*  12/5/3              RUP               zdx         ���ӶԷ������פ�������ı仯
**************************************************************************/

#define  UHF_RF_ABNORMAL_K0 (0)
WORD32 usp_uhf_rf_getAntStatus(WORD16 wAntIndex)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wReadData = 0;
    WORD16 wVSWR = 0;
    WORD16 wLoop = 0;
    SWORD16 swPower = 0;
    BYTE   aucAlarmInfo[20];
    WORD32 dwAlarmInfoSize = 0;

    /* ѡ������RFCB ������Ҫ���µ���������*/
    dwRet = usp_uhf_rf_selectAnt(wAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:Rf antenna set failed! %d\r\n", dwRet);
        return USP_UHF_ANT_SET_ERROR;
    }

    usp_uhf_rf_WriteAntDatt();

    if (UHF_RF_ABNORMAL_K0 == g_atAntPower[wAntIndex].ptPowerTable->wK0)
    {
        /*�������ݴ���ʱֱ�ӷ��ش��� ͨ��
               �ڶ������ݶ�ȡʱ�жϵĽ��������*/
        uhfRfDevPrint("uhfRf: abnormal powertable data!!!!!!!!!!!\r\n");
        return USP_UHF_ANT_NOT_INPOS_ERROR;
    }

    /* ȥ��פ������ */
    usp_uhf_rf_removeAntAlarm();
    /*������*/
    usp_uhf_rf_setAmp(RF_AMP_ON);
    usp_uhf_rf_txSet(UHF_RF_START_TX);
    usleep(1000);
    /* ���ù���ֵΪ20db �Ĺ���ֵ0x250  Datt ˥��ֵ����Ϊ�������*/
    usp_uhf_rf_setACDatt(g_tRfInitPara.w20DBAtten);
    usp_uhf_rf_WriteAntDatt();
    usp_uhf_rf_writeCtrlWord(g_atAntPower[wAntIndex].ptPowerTable->wK0);
    /* ���ù��ʼ����ƼĴ�����ⷴ����*/
    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelForward);
    /* �򿪹��ʼ��ADC��ʱ�� */
    FpgaRegWrite(POWER_DETECT_ADC_CLK_CTRL, POWER_DETECT_ADC_CLK_ON);

    /* ��ʱ�ȴ�*/
    usleep(UHFRF_AC_WAIT_TIME);

    if (USP_UHF_ANT_IS_PROTECTED == uhfRf_IsAntProtected())
    {
        wVSWR = 10000;
    }
    else
    {
        /* ��ȡ���ʼ��ֵ�Ĵ�����ֵ*/
        wReadData = usp_uhf_rf_readDetectVal();
        uhfRfDevPrint("uhfRf:Foward Voltage detect 0X%x ", wReadData);
        swPower = usp_uhf_rf_ComputePower(wReadData, wAntIndex, FORWARD_FLAG);
        uhfRfDevPrint("Computed power is %d\r\n", swPower);
        swPower += g_tRfInitPara.swForCompensatePower;

        if (swPower <= 0)
        {
            wVSWR = 10000;
        }
        else
        {
            usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelBackward);
            usleep(UHFRF_AC_WAIT_TIME);
            wReadData= usp_uhf_rf_readDetectVal();
            wVSWR = ComputeVSWR(swPower, wReadData, wAntIndex);
        }
    }

    if (USP_UHF_ANT_IS_PROTECTED == uhfRf_IsAntProtected())
    {
        /*���Ӽ�ⷴ���vswr���ж�*/
        wVSWR = 10000;
    }

    usp_uhf_rf_txSet(UHF_RF_STOP_TX);
    /* �رչ��ʼ��ADC��ʱ�� */
    FpgaRegWrite(POWER_DETECT_ADC_CLK_CTRL, POWER_DETECT_ADC_CLK_OFF);
    uhfRf_RecordAntVswr(wAntIndex, wVSWR);
    uhfRfDevPrint("uhfRf:Ant %d VSWR is %d !\r\n", wAntIndex, wVSWR);

    if (g_tRfInitPara.wAntOnlineThr < wVSWR)
    {
        dwAlarmInfoSize = snprintf((char *)aucAlarmInfo, sizeof(aucAlarmInfo), "%d", wAntIndex);
        uhf_amp_report(EV_USP_OAM_AMP_InformReport,
                       R_RF_AC_ANTABSENT_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_SERIOUS, aucAlarmInfo, dwAlarmInfoSize);
        return USP_UHF_ANT_NOT_INPOS_ERROR; /*���߲���λ*/
    }

    return USP_SUCCESS;
}

/**************************************************************************
* ��������: usp_uhf_rf_CalibrateK0Datt
* ��������: ���ݵ�ǰ���ʺͿ����ֶ�ָ�����߽��бջ���������
* ���ʵı�:
* �޸ĵı�:
* �������: WORD16 wPCurrent    ���ߵ�ǰ����
*                           WORD16 wPTarget      ����Ŀ�깦��
*                           WORD16 wCurCtrlWord ��ǰ������
*                           BYTE ucAntIndex           ����������
* �������:
* ����ֵ:
* ����˵��   �˺����еĴ�ӡ��Ϣ׷�ٶ�����������̣����ڶ�λ����������㷨���ֵ��������µĲ��ɹ�����
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  07/06/13          V1.0              XXXX                XXXX
**************************************************************************/
WORD32 usp_uhf_rf_CalibrateK0Datt(WORD16 wPTarget, WORD16 wAntIndex)
{
    WORD16  wK0 = g_atAntPower[wAntIndex].ptPowerTable->wK0;
    SWORD16 swPowerDiff = 0;
    WORD16  wDattVal = 0;
    WORD16  wDetectPower = 0;

    if (wPTarget > g_tRfInitPara.wMaxPower)
    {
        uhfRfDevPrint("uhfrf:power exceed max power\r\n");
        return USP_UHF_RF_POWER_EXCEEDED;
    }

    //uhfRf_warmPA(wPTarget,wAntIndex);
    /*��DATT �ֵ�*/
    wDetectPower = usp_uhf_rf_GetPowerDatt(wAntIndex, wPTarget, &wDattVal, wK0);

    swPowerDiff = wDetectPower - wPTarget;

    if (abs(swPowerDiff) > 1000)
    {
        return USP_UHF_CLIBRITE_ABNORAML;
    }

    if (abs(swPowerDiff) > 25)
    {
        /*��K0΢��*/
        wDetectPower = usp_uhf_rf_GetPowerK0(wAntIndex, wPTarget,
                                        wDattVal, &wK0, wDetectPower);
    }

    usp_uhf_rf_SetPowerEntry(wAntIndex, wDetectPower, wPTarget, wK0, wDattVal);
    uhfRfDevPrint("uhfRf: ant %d calibrate power %d Success!\r\n\r\n",
                  wAntIndex, wDetectPower);

    return USP_SUCCESS;
}

/**********************************************************************
* ��������: usp_uhf_rf_GetPowerK0
* ��������:ͨ��K0����΢��
* �������: WORD16 wAntIndex    ���ߺ�
*                           WORD16 wPTarget      ����Ŀ�깦��
*                           WORD16 wFpgaAtt  ��ǰ������
*                           WORD16 *pwK0  ��ɺ��K0
                             WORD16 wDetectPower ����datt������Ĺ���ֵ
* �������:
* ����ֵ:     WORD16  ������⵽�Ĺ���ֵ
* ����˵��  �˺��������ʿ��Ƶ�0.5db֮��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
* 12/01/11          V1.0              �Զ���               ����
***********************************************************************/
WORD16 usp_uhf_rf_GetPowerK0(WORD16 wAntIndex, WORD16 wPTarget,
                               WORD16 wFpgaAtt, WORD16 *pwK0, WORD16 wDetectPower)
{
    WORD16  wK0AdjLoop = 0;
    WORD16  wK0 = *pwK0;
    SWORD16 swPowerDiff = 0;
    float   fPowValue = 0.0;

    usp_uhf_rf_setACDatt(wFpgaAtt);
    usp_uhf_rf_WriteAntDatt();


    for (wK0AdjLoop = 0; wK0AdjLoop < UHF_RF_MAX_AC_ADJUST; wK0AdjLoop++)
    {
        swPowerDiff = wPTarget - wDetectPower;

        /*С��0.25db �˳���������*/
        if (abs(swPowerDiff) <= 25)
        {
            break;
        }

        /*��ʽK1=K0*(10^((P1-P0)/20)) ����P1ΪĿ�깦��
                P2Ϊ��ǰ���㹦�ʣ����ΪҪ���õ�K0*/
        fPowValue = pow(10, (float)swPowerDiff / 2000);
        uhfRfDevPrint("uhfRf: %d fPowValue %f\r\n", swPowerDiff, fPowValue);
        /*������ֹ����ֵ����ʱ����������*/
        fPowValue = (fPowValue > 1.5) ? 1.5 : fPowValue;

        wK0 = wK0 * fPowValue;

        /*����K0��ֵ����̫��*/
        if (wK0 > g_tRfInitPara.wMaxK0)
        {
            wK0 = UHF_RF_MAX_K0;
            uhfRfDevPrint("uhfRf:Alram K0 is too High %x!\r\n", wK0);
        }

        wDetectPower = SetK0GetPower(wK0, wAntIndex);

    }

    *pwK0 = wK0;
    return  wDetectPower;
}

/**********************************************************************
* ��������: uhfRf_warmPA
* ��������:PA�ڼ��֮ǰӦ����Ԥ��һ��
* �������: WORD16 wAntIndex    ���ߺ�
*                           WORD16 wPTarget      ����Ŀ�깦��
*                           WORD16 wK0
* �������:
* ����ֵ:     ��
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
* 12/01/11          V1.0              �Զ���               ����
***********************************************************************/

void uhfRf_warmPA(WORD16 wPTarget, WORD16 wAntIndex)
{
    WORD16  wFpgaAtten = 0;
    WORD16  wPowerAtt = 0;
    WORD16  wK0 = g_atAntPower[wAntIndex].ptPowerTable->wK0;
    /*�����ʼ˥��ֵ*/
    wPowerAtt = g_tRfInitPara.wFullPower - wPTarget;
    wPowerAtt = (wPowerAtt <= g_tRfInitPara.wMinAtten) ? g_tRfInitPara.wMinAtten : wPowerAtt;
    uhfRfDevPrint("Change wPowerAtt to %d and ", wPowerAtt);
    wFpgaAtten = usp_uhf_rf_TransToFpgaAtten(wPowerAtt);
    usp_uhf_rf_setACDatt(wFpgaAtten);
    usp_uhf_rf_WriteAntDatt();
    usp_uhf_rf_writeCtrlWord(wK0);
    /* ���ù��ʼ����ƼĴ������ǰ����*/
    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelForward);

    sleep(UHFRF_PA_WARM_TIME);    /* ��ʱ�ȴ�*/

}

/**********************************************************************
* ��������: usp_uhf_rf_GetPowerDatt
* ��������: ����Ŀ�깦�ʽ����ʿ��Ƶ�0.5db֮��
* �������: WORD16 wAntIndex    ���ߺ�
*                           WORD16 wPTarget      ����Ŀ�깦��
*                           WORD16 *pwFpgaAtt  ��ǰ������
*                           WORD16 wK0
* �������:
* ����ֵ:     WORD16  ������⵽�Ĺ���ֵ
* ����˵��  �˺��������ʿ��Ƶ�0.5db֮��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
* 12/01/11          V1.0              �Զ���               ����
***********************************************************************/
WORD16 usp_uhf_rf_GetPowerDatt(WORD16 wAntIndex, WORD16 wPTarget,
                                 WORD16 *pwFpgaAtt, WORD16 wK0)
{
    SWORD16 swPowerDiff = 0;
    WORD16  wAttenAdjLoop = 0;
    WORD16  wFpgaAtten = 0;
    WORD16  wDetectPower = 0;
    WORD16  wPowerAtt = 0;

    /*�����ʼ˥��ֵ*/
    wPowerAtt = g_tRfInitPara.wFullPower - wPTarget;

    for (wAttenAdjLoop = 0; wAttenAdjLoop < UHF_RF_MAX_AC_ADJUST; wAttenAdjLoop++)
    {
        wPowerAtt = (wPowerAtt <= g_tRfInitPara.wMinAtten) ? g_tRfInitPara.wMinAtten : wPowerAtt;
        uhfRfDevPrint("uhfRf:Change wPowerAtt to %d and ", wPowerAtt);
        wFpgaAtten = usp_uhf_rf_TransToFpgaAtten(wPowerAtt);
        usp_uhf_rf_setACDatt(wFpgaAtten);
        usp_uhf_rf_WriteAntDatt();
        wDetectPower = SetK0GetPower(wK0, wAntIndex);
        swPowerDiff = wDetectPower - wPTarget;

        /*�Ѿ������˥�����ǻ���С��Ŀ��ֵ*/
        if ((g_tRfInitPara.wMaxAtten == wPowerAtt) && (0 < swPowerDiff))
        {
            break;
        }

        /*�Ѿ�����С˥�����ǻ��Ǵ���Ŀ��ֵ*/
        if ((g_tRfInitPara.wMinAtten == wPowerAtt) && (0 > swPowerDiff))
        {
            break;
        }

        /*С��0.25db �˳���������*/
        if (abs(swPowerDiff) <= 25)
        {
            break;
        }

        /*�����쳣��Ӳ�������⣬���ٵ���*/
        if (abs(swPowerDiff) > 1000)
        {
            LAPInsert(FILE_UHF_RFCTRL, USP_UHF_CLIBRITE_ABNORAML, swPowerDiff, 2);
            uhfRfDevPrint("uhfRf:att adjust power detected %d db\r\n", wDetectPower);
            break;
        }

        /*���ݼ�⹦��ֵ����˥��ֵ*/
        if ((swPowerDiff < 0) && (abs(swPowerDiff) > wPowerAtt))
        {
            /*����wPowerAtt += swPowerDiff  ���ָ�ֵ�����*/
            wPowerAtt = 0;
        }
        else
        {
            wPowerAtt += swPowerDiff;
        }
    }

    *pwFpgaAtt = wFpgaAtten;
    return wDetectPower;
}


WORD16 usp_uhf_rf_TransToFpgaAtten(WORD16 wAttenuation)
{
    WORD16 wDiff = 0;
    uhfRfDevPrint("uhfRf:Receive Attenuation %d\r\n", wAttenuation);
    wDiff = wAttenuation - wAttenuation / 50 * 50;

    if (wDiff < 25)
    {
        wAttenuation = wAttenuation / 50 * 50;
    }
    else
    {
        wAttenuation = (wAttenuation / 50 + 1) * 50;
    }

    wAttenuation = (wAttenuation < g_tRfInitPara.wMaxAtten) ? wAttenuation : g_tRfInitPara.wMaxAtten;
    uhfRfDevPrint("uhfRf:Transfer  Attenuation to  %d\r\n", wAttenuation);

    wAttenuation = ((wAttenuation / 100) << 1) | ((wAttenuation / 50) % 2);
    return (~wAttenuation) & (0x3F);
}

/**************************************************************************
* ��������: SetK0GetPower
* ��������:����K0�Ժ�ͨ����ȡ��ѹ�������Ǽ��Ĺ���
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:   ��⵽�Ĺ���ֵ
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2007��7��27��          V1.0              XXXX                XXXX
**************************************************************************/

static WORD16 SetK0GetPower(WORD16 wK0, WORD16 wAntIndex)
{
    SWORD16 swPower;
    WORD16 wVoltage;
    WORD16 wLoop = 0;
    usp_uhf_rf_writeCtrlWord(wK0);
    /* ���ù��ʼ����ƼĴ������ǰ����*/
    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelForward);
    /* �򿪹��ʼ��ADC��ʱ�� */
    FpgaRegWrite(POWER_DETECT_ADC_CLK_CTRL, POWER_DETECT_ADC_CLK_ON);
    //usp_uhf_rf_setAmp(RF_AMP_ON);
    usleep(UHFRF_AC_WAIT_TIME);    /* ��ʱ�ȴ�*/
    /* ��ȡ��ѹֵ����wVoltage*/
    wVoltage = usp_uhf_rf_readDetectVal();
    swPower = usp_uhf_rf_ComputePower(wVoltage, wAntIndex, FORWARD_FLAG);
    uhfRfDevPrint("uhfRf:set K0 0x%x get voltage 0x%x power %d\r\n", wK0, wVoltage, swPower);

    if (swPower < 0)
    {
        uhfRfDevPrint("uhfRf:RF is Abnomal now \r\n");
        //LAPInsert(FILE_UHF_RFCTRL, USP_UHF_CLIBRITE_ABNORAML, swPower, 0);
        swPower = 0;
    }
    /* �رչ��ʼ��ADC��ʱ�� */
    FpgaRegWrite(POWER_DETECT_ADC_CLK_CTRL, POWER_DETECT_ADC_CLK_OFF);
    /*�ж϶�������Ƿ񳬹������*/
    return (WORD16)swPower;

}

/**************************************************************************
* ��������: usp_uhf_rf_AutoCalibrateInit
* ��������: ȫ�ֱ�����ʼ��
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2007��7��27��          V1.0              XXXX                XXXX
**************************************************************************/
void usp_uhf_rf_AutoCalibrateInit()
{
    WORD32 dwRcvLen = 0;
    BYTE ucPowerTableCount = 0;
#if EEPROM_SWITCH
    usp_drv_eeprom_rcv(POWER_TABLE_COUNT, 1, &ucPowerTableCount, &dwRcvLen);
#endif
    uhfRfDevPrint("uhfRf:ucPowerTableCount = %d\r\n", ucPowerTableCount);
    
    uhfRf_InitACData(ucPowerTableCount);
    memset(&g_atAntPower, 0, sizeof(g_atAntPower));
    g_atAntPower[0].ptPowerTable = &g_tUhfRfPowerTable1;
    g_atAntPower[1].ptPowerTable = &g_tUhfRfPowerTable1;
    g_atAntPower[2].ptPowerTable = &g_tUhfRfPowerTable2;
    
    if (FOUR_POWER_TABLE == ucPowerTableCount)
    {
        g_atAntPower[3].ptPowerTable = &g_tUhfRfPowerTable3;
        g_atAntPower[4].ptPowerTable = &g_tUhfRfPowerTable4;
    }
    else
    {
        g_atAntPower[3].ptPowerTable = &g_tUhfRfPowerTable2;
        g_atAntPower[4].ptPowerTable = &g_tUhfRfPowerTable1;
    }
    
}

/**************************************************************************
* ��������:
* ��������: ����פ����
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:פ����*100(פ���Ⱦ�ȷ��С�������λ)
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2008��9��13��          V1.0              XXXX                XXXX
**************************************************************************/
static WORD16 ComputeVSWR(WORD16 wPTarget, WORD16 wVoltage, WORD16 wAntIndex)
{
    SWORD16 swPower = 0;
    Double returnLoss = 0.0;
    Double dVSWR = 0.0;
    Double dPower = 0.0;

    dPower = (double)wPTarget / (double)100;

    swPower = usp_uhf_rf_ComputePower(wVoltage, wAntIndex, BACKWARD_FLAG);
    uhfRfDevPrint("uhfRf:Detect Power is %d\r\n", swPower);

    swPower += g_tRfInitPara.swRevCompensatePower;

    /*zdx modified  DBm2mW(-10) to DBm2mW(-8)*/
    if (swPower < -999)
    {
        dVSWR = 1.02;
    }
    else
    {
        returnLoss = (double)dPower - (MW2dBm(DBm2mW((double)swPower / (double)100) - DBm2mW(-10)) + g_tRfInitPara.wRcvCompensate);
        uhfRfDevPrint("uhfRf:returnLoss is %f\r\n", returnLoss);

        if (returnLoss < 1)
        {
            dVSWR = 100;
        }
        else if (returnLoss > 40)
        {
            dVSWR = 1.02;
        }
        else
        {
            dVSWR = (1 + pow(10, (returnLoss / (-20)))) / (1 - pow(10, (returnLoss / (-20))));
        }
    }


    if (dVSWR < 1.02)
    {
        dVSWR = 1.02;
        uhfRfDevPrint("uhfRf:~~~~~~VSWR Comput error !~~~~~~~~~\r\n");
    }

    uhfRfDevPrint("uhfRf:Backward Power = %d, wPTarget = %d\r\n", swPower, wPTarget);
    uhfRfDevPrint("uhfRf:ReturnLoss = %0.2f, VSWR = %0.2f\r\n", returnLoss, dVSWR);
    uhfRfDevPrint("uhfRf:come in ComputeVSWR out\r\n");
    return (WORD16)(dVSWR * 100);
}

/**************************************************************************
* ��������:
* ��������: ��EEPROM�ж�ȡ20dbm�Ŀ����ֺ͹��ʵ�ѹ���ձ�
* ���ʵı�:
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2008��9��13��          V1.0              XXXX                XXXX
**************************************************************************/
static void uhfRf_InitACData(BYTE ucTableCount)
{
    WORD32 dwRet = USP_SUCCESS;
    /*��һ�ε���������Ϊ�ɹ�*/
    dwRet = usp_uhf_rf_readPowerTable(&g_tUhfRfPowerTable1, UHF_RF_PT_ANT14);
    if (USP_SUCCESS != dwRet)
    {
        /*��K0ֵ����Ϊ0�����Ժ�����̲�������*/
        g_tUhfRfPowerTable1.wK0 = 0;
    }

    dwRet = usp_uhf_rf_readPowerTable(&g_tUhfRfPowerTable2, UHF_RF_PT_ANT23);

    if (USP_SUCCESS != dwRet)
    {
        memcpy(&g_tUhfRfPowerTable2, &g_tUhfRfPowerTable1,
               sizeof(T_USPUHFRfPowerTable));
    }

    if (FOUR_POWER_TABLE == ucTableCount)
    {
        dwRet = usp_uhf_rf_readPowerTable(&g_tUhfRfPowerTable3, UHF_RF_PT_ANT3);
        if (USP_SUCCESS != dwRet)
        {
            memcpy(&g_tUhfRfPowerTable3, &g_tUhfRfPowerTable2,
                   sizeof(T_USPUHFRfPowerTable));
        }

        dwRet = usp_uhf_rf_readPowerTable(&g_tUhfRfPowerTable4, UHF_RF_PT_ANT4);

        if (USP_SUCCESS != dwRet)
        {
            memcpy(&g_tUhfRfPowerTable4, &g_tUhfRfPowerTable1,
                   sizeof(T_USPUHFRfPowerTable));
        }
    }

#if ((UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT) || (UHF_BOARD_TYPE == BOARD_TYPE_RFCB7))
    /* ��EEPROM�ж�ȡդѹֵ */
    dwRet = usp_uhf_rf_readGateVoltageValue(&g_tUhfGateVoltageCtrl);

    if (USP_SUCCESS != dwRet)
    {
        g_tUhfGateVoltageCtrl.wGateVoltage0 = UHF_RF_GATE_VOL_DEFAULT0;
        g_tUhfGateVoltageCtrl.wGateVoltage1 = UHF_RF_GATE_VOL_DEFAULT1;
    }
#endif

#if UHF_RF_ADJUST_PR_BALANCE
    dwRet = usp_uhf_rf_readPRData(&g_tUhfRfPowerTable1, UHF_RF_PT_ANT14);

    if (USP_SUCCESS != dwRet)
    {
        g_tUhfRfPowerTable1.wPR1Value = g_tRfInitPara.wPraskDefaultHigh;
        g_tUhfRfPowerTable1.wPR2Value = g_tRfInitPara.wPraskDefaultLow;
        g_tUhfRfPowerTable1.swMinK0   = 0;
    }

    dwRet = usp_uhf_rf_readPRData(&g_tUhfRfPowerTable2, UHF_RF_PT_ANT23);

    if (USP_SUCCESS != dwRet)
    {
        g_tUhfRfPowerTable2.wPR1Value = g_tUhfRfPowerTable1.wPR1Value;
        g_tUhfRfPowerTable2.wPR2Value = g_tUhfRfPowerTable1.wPR2Value;
        g_tUhfRfPowerTable2.swMinK0   = g_tUhfRfPowerTable1.swMinK0;
    }

    g_tUhfRfPowerTable3.wPR1Value = g_tUhfRfPowerTable2.wPR1Value;
    g_tUhfRfPowerTable3.wPR2Value = g_tUhfRfPowerTable2.wPR2Value;
    g_tUhfRfPowerTable3.swMinK0   = g_tUhfRfPowerTable2.swMinK0;

    g_tUhfRfPowerTable4.wPR1Value = g_tUhfRfPowerTable1.wPR1Value;
    g_tUhfRfPowerTable4.wPR2Value = g_tUhfRfPowerTable1.wPR2Value;
    g_tUhfRfPowerTable4.swMinK0   = g_tUhfRfPowerTable1.swMinK0;
#else
    /*���û�����������Ĭ�ϵ�ֵ*/
    g_tUhfRfPowerTable1.wPR1Value = g_tRfInitPara.wPraskDefaultHigh;
    g_tUhfRfPowerTable1.wPR2Value = g_tRfInitPara.wPraskDefaultLow;
    g_tUhfRfPowerTable1.swMinK0   = 0;

    g_tUhfRfPowerTable2.wPR1Value = g_tRfInitPara.wPraskDefaultHigh;
    g_tUhfRfPowerTable2.wPR2Value = g_tRfInitPara.wPraskDefaultLow;
    g_tUhfRfPowerTable2.swMinK0   = 0;

    g_tUhfRfPowerTable3.wPR1Value = g_tRfInitPara.wPraskDefaultHigh;
    g_tUhfRfPowerTable3.wPR2Value = g_tRfInitPara.wPraskDefaultLow;
    g_tUhfRfPowerTable3.swMinK0   = 0;

    g_tUhfRfPowerTable4.wPR1Value = g_tRfInitPara.wPraskDefaultHigh;
    g_tUhfRfPowerTable4.wPR2Value = g_tRfInitPara.wPraskDefaultLow;
    g_tUhfRfPowerTable4.swMinK0   = 0;

#endif

}

/**************************************************************************
* ��������: uhfRf_setCalibrateFreq
* ��������: �ڹ�������ǰ��������Ҫ��Ƶ�ʣ���epc��Ϊ׼
* ���ʵı�: ��
* �޸ĵı�:
* �������:
* �������:
* ����ֵ:
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  2008��9��13��          V1.0              XXXX                XXXX
**************************************************************************/
///// need to be FIXED ??
static WORD32 uhfRf_setCalibrateFreq(WORD16 wAntIndex)
{
    g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(
                               uhfRf_getFreqWorkChannel(wAntIndex, UHF_RF_PROTOCOL_EPC),
                               uhfRf_getFreqTableId(wAntIndex, UHF_RF_PROTOCOL_EPC));
    return usp_uhf_rf_setPLLFreq(g_tRfCtrl.dwWorkFreq);

}

void SendAntAbnoramlAlarm(WORD16 wAntIndex, WORD32 dwAlarmType)
{

    uhfRfDevPrint("Ant %d don't present.\r\n", wAntIndex);

    /* �ϱ��澯,�������߹��ʺ�״̬*/
    if (1 == wAntIndex)
    {
        uhf_amp_report(dwAlarmType,
                       R_RF_AC_ANT1_VSWR_PROTECTED_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
    }
    else if (2 == wAntIndex)
    {
        uhf_amp_report(dwAlarmType,
                       R_RF_AC_ANT2_VSWR_PROTECTED_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
    }
    else if (3 == wAntIndex)
    {
        uhf_amp_report(dwAlarmType,
                       R_RF_AC_ANT3_VSWR_PROTECTED_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
    }
    else
    {
        uhf_amp_report(dwAlarmType,
                       R_RF_AC_ANT4_VSWR_PROTECTED_ALARM, 0,
                       USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
    }

}


void SendAntABSAlarm(BYTE ucAntIndex)
{
    // char ucSprintfBuf[50] = {0};    /* �ϱ��澯����Ϣ�����Ȳ�����49���ַ� */
    // BYTE ucSprintfLen = 0;
    uhfRfDevPrint("Ant %d don't present.\r\n", ucAntIndex);
    /* �ϱ��澯,�������߹��ʺ�״̬*/
    SendAlarm
}


static WORD32 uhfRf_SetRfPower(WORD16 wAntIndex, WORD16 wPower)
{
    /*��ǰ���ߵĹ���ֵ�����ı�����Ҫ���¶���*/
    WORD32 dwRet = USP_SUCCESS;

    if (wPower < UHF_RF_MIN_POWER || wPower > g_tRfInitPara.wMaxPower)
    {
        uhfRfDevPrint("uhfRf: rcv power %ddb abnormal\r\n", wPower);
        return USP_UHF_RF_POWER_EXCEEDED;
    }

    if (NULL == usp_uhf_rf_FindPowerEntry(wAntIndex, wPower))
    {
        dwRet = usp_uhf_rf_autoCalibrate(wAntIndex, wPower);
        uhfRfDevPrint("uhfRf:invent autocalibrate power %ddb\r\n", wPower);

        if (USP_SUCCESS != dwRet)
        {
            uhfRfDevPrint("uhfRf:invent autocalibrate failed!\r\n");
            return dwRet;
        }
        
    }

    /* ������Ƶ���͹��� ��д��K0�Ĺ��̣����RO�·��Ĺ���ֵ�����˱仯��Ҫ���¶���*/
    dwRet = usp_uhf_rf_setTxPower(wAntIndex, wPower);

    if (USP_SUCCESS != dwRet)
    {
        return dwRet;
    }

    return USP_SUCCESS;
}


/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setPLLFreq(WORD32 dwRfFreq)
* ����������PLL���໷����
* ���ʵı���
* �޸ĵı���
* ���������WORD32 dwRfFreq : PLL����Ƶ��
* ���������
* �� �� ֵ��WORD32
                         0��    �ɹ�
                      ������    ʧ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 usp_uhf_rf_setPLLFreq(WORD32 dwRfFreq)
{

    WORD32 dwRetVal = USP_SUCCESS;
    WORD16 wNeedReset = PLL_NO_NEED_RESET;
    /*�ϵ�ʱ������໷ʧ���������쳣*/
    static WORD16 wPllConfigStatus = UHF_RF_PLL_NOT_CONFIGED;
    WORD16 wAmpState = RF_AMP_OFF;
	DEBUG_MSG("usp_uhf_rf_setPLLFreq start\n");
	fflush(stdout);

    uhfRfDevPrint("uhfRf:uhfrf Frequence %d\r\n", dwRfFreq);

    wNeedReset = uhfRf_CheckPLLStatus(&wPllConfigStatus);/* ������໷״̬ */
   
    if ((dwRfFreq == g_tRfCtrl.dwPllWorkFreq) && (wNeedReset != PLL_NEED_RESET))
    {
        return USP_SUCCESS;
    }
	DEBUG_MSG("uhfRf_GetAmpState start\n");
	fflush(stdout);

    /*Ӳ�������ڹ�������ʱ�رչ��ŵ�Դ2009.9.9*/
    wAmpState = uhfRf_GetAmpState();
	DEBUG_MSG("usp_uhf_rf_setAmp start\n");
	fflush(stdout);
    usp_uhf_rf_setAmp(RF_AMP_OFF);
	DEBUG_MSG("uhfRf_PLLChipCfg start\n");
	fflush(stdout);
    dwRetVal = uhfRf_PLLChipCfg(dwRfFreq, wNeedReset);
    usp_uhf_rf_setAmp(wAmpState);
	DEBUG_MSG("usp_uhf_rf_setAmp end\n");
	fflush(stdout);

    if (dwRetVal != 0)
    {
        uhfRfDevPrint("uhfRf:hardware PLL config fail\r\n");
        g_tRfCtrl.dwPllWorkFreq = 0;
        LAPInsert(FILE_FPGAAPI, USP_UHF_PLL_LOSE, dwRetVal, 0);
    }
    else
    {
        g_tRfCtrl.dwPllWorkFreq = dwRfFreq;
        uhfRfDevPrint("uhfRf:hardware PLL config success\r\n");
    }

    /* �������໷��ɺ��ٴμ�����໷�Ƿ�ʧ�� */
    uhfRf_CheckPLLStatus(&wPllConfigStatus);

    return dwRetVal;

}
/**********************************************************************
* �������ƣ�uhfRf_CheckPLLStatus
* �����������鿴PLL��״̬
* ���ʵı���
* �޸ĵı���
* ���������WORD16 *wPLLConfigStatus: ���໷�Ƿ����ù�
* ���������
* �� �� ֵ���Ƿ���Ҫ�����������໷

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/8/31             �ڽ�        ����  
************************************************************************/
WORD16 uhfRf_CheckPLLStatus(WORD16 *pwPLLConfigStatus)
{
    WORD32 dwRetVal = USP_SUCCESS;
    static WORD16 wPLLAlarmFlag = PLL_NO_ALARM;
    WORD16 wPLLResetStatus = PLL_NO_NEED_RESET;
	DEBUG_MSG("uhfRf_CheckPLLStatus start\n");
	fflush(stdout);

    if (UHF_RF_PLL_NOT_CONFIGED == *pwPLLConfigStatus)/* ���໷֮ǰû�����ù� */
    {
		DEBUG_MSG("UHF_RF_PLL_NOT_CONFIGED == *pwPLLConfigStatus\n");
		fflush(stdout);
        wPLLResetStatus = PLL_NEED_RESET;/* ������������ */
        *pwPLLConfigStatus = UHF_RF_PLL_CONFIGED;
    }
    else
    {
    /* ������໷��״̬ */
    dwRetVal = uhfRf_getPLLStatus();


    if (RF_PLL_STATUS_OK != dwRetVal)
    {
            /* PLL״̬���ԣ���Ҫ�������� */
        uhfRfDevPrint("\r\nuhfRf:RF PLL should be lock but loesed !!! \r\n\r\n");
            /* PLL���������м�⵽Ӧ��������û�������Ĵ�*/
            LAPInsert(FILE_RFCB, USP_UHF_PLL_LOSE, dwRetVal, 0);
            /* ���͸澯 */
            uhf_amp_report(EV_USP_OAM_AMP_AlarmReport,
                           R_RF_PLL_BEING_LOSED_ALARM, 0,
                           USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
            wPLLAlarmFlag = PLL_ALARMED;

        /* ��������ȫ���Ĵ�����������ֻ����N Counter */
            wPLLResetStatus = PLL_NEED_RESET;
    }
    else
    {
        /* PLL״̬OK  */
        if (PLL_ALARMED == wPLLAlarmFlag)
        {
            uhf_amp_report(EV_USP_OAM_AMP_AlarmRestoreReport,
                           R_RF_PLL_BEING_LOSED_ALARM, 0,
                           USP_OAM_AMP_ALARM_LEVEL_FATAL, NULL, 0);
            wPLLAlarmFlag = PLL_NO_ALARM;
        }
    }
    }

    return wPLLResetStatus;
}

/**********************************************************************
* �������ƣ�uhfRf_getPRMinK0
* �������������ذ��粻ƽ�����ں��PR DAC��highֵ
* ���ʵı���
* �޸ĵı���
* ���������wAntIndex ���ߺ�
* ���������
* �� �� ֵ��WORD16 �洢��PRֵ

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/3/5  �Զ���         ����  RUPƽ̨��
************************************************************************/

WORD16 uhfRf_getPRMinK0(WORD16 wAntIndex)
{
    return g_atAntPower[wAntIndex].ptPowerTable->swMinK0;
}


/**********************************************************************
* �������ƣ�uhfRf_getPRDacHigh
* �������������ذ��粻ƽ�����ں��PR DAC��highֵ
* ���ʵı���
* �޸ĵı���
* ���������wAntIndex ���ߺ�
* ���������
* �� �� ֵ��WORD16 �洢��PRֵ

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/3/5  �Զ���         ����  RUPƽ̨��
************************************************************************/

WORD16 uhfRf_getPRDacHigh(WORD16 wAntIndex)
{
    return g_atAntPower[wAntIndex].ptPowerTable->wPR1Value;
}


/**********************************************************************
* �������ƣ�uhfRf_getPRDacHigh
* �������������ذ��粻ƽ�����ں��PR DAC��lowֵ
* ���ʵı���
* �޸ĵı���
* ���������wAntIndex ���ߺ�
* ���������
* �� �� ֵ��WORD16 �洢��PRֵ

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/3/5  �Զ���         ����  RUPƽ̨��
************************************************************************/

WORD16 uhfRf_getPRDacLow(WORD16 wAntIndex)
{
    return g_atAntPower[wAntIndex].ptPowerTable->wPR2Value;
}


/**********************************************************************
* �������ƣ�uhfRf_InitCtrlStruct
* ������������ʼ����Ƶ�Ĳ���
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/3/5  �Զ���         ����  RUPƽ̨��
************************************************************************/

void uhfRf_InitCtrlStruct()
{
    BYTE ucForAntIndex = 0;
    memset(&g_tRfCtrl, 0x0, sizeof(g_tRfCtrl));

    for (ucForAntIndex = 0; ucForAntIndex < (READER_ANTENNA_COUNT + 1); ucForAntIndex++)
    {
        g_tRfCtrl.atRfAntCfg[ucForAntIndex].wRcvOrSnd = 3;
        g_tRfCtrl.atRfAntCfg[ucForAntIndex].aucAntPower[0] = 20;
    }

    g_tRfCtrl.wAntIndex = 1;

}
#endif

