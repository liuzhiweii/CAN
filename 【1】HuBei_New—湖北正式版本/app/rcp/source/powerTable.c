/***************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�PowerTable.c
* �ļ���ʶ��
* ����ժҪ����ȡ������е�����
* ����˵����
* ��ǰ�汾��
* ��    �ߣ��Զ���
* ������ڣ�2012.2.5
*
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
**********************************************************************/
#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RBCB1)
#include "uhf_dbg.h"
#include "Board.h"
#include "RfCtl.h"

extern T_RfInitPara   g_tRfInitPara;
extern T_USPUHFGateVoltageCtrl  g_tUhfGateVoltageCtrl;
/**************************************************************************
* �������ƣ�CalMinK0
* �������������������pr����ֵ��������������͵�
                                 ʱ���K0ֵ
* ���ʵı���
* �޸ĵı���
* ���������wPR1��PR1��ֵ
*                               wPR2��PR2��ֵ
*                            wInitK0:    д�뵽EEPROM�е�20DBʱ��Ӧ��K0ֵ
* �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/1/18    V1.0      �ڽ�        ����
* 12/3/5    V1.1      zdx        RUPƽ̨��
**************************************************************************/

static SWORD16 CalMinK0(WORD16 wPR1, WORD16 wPR2, WORD16 wInitK0)
{
    SWORD16 swDAC_PR1 = 0;
    SWORD16 swDAC_PR2 = 0;
    WORD16 wSign_bit = 0;  /* ����λ */
    SWORD32 sdwtemp1 = 0;
    SWORD16 swMinK0;

#if (UHF_BOARD_TYPE == BOARD_TYPE_RFCB2)
    swDAC_PR1 = wPR1;
    swDAC_PR2 = wPR2;
#else
    /* PR����ֵΪ14λ��Ч��ȡ��ǰ14λ Ϊ�˼�����Ҫ ������λ���Ͻ�����չ*/
    swDAC_PR1 = wPR1 & 0x3FFF;
    swDAC_PR2 = wPR2 & 0x3FFF;

    wSign_bit = swDAC_PR1 & 0x2000;
    swDAC_PR1 = swDAC_PR1 | (wSign_bit << 1); /* ʹ��bit13��ֵ����bit14��bit15 */
    swDAC_PR1 = swDAC_PR1 | (wSign_bit << 2);

    wSign_bit = swDAC_PR2 & 0x2000;
    swDAC_PR2 = swDAC_PR2 | (wSign_bit << 1); /* ʹ��bit13��ֵ����bit14��bit15 */
    swDAC_PR2 = swDAC_PR2 | (wSign_bit << 2);
#endif
    printf("DAC_H = 0x%x, DAC_L = 0x%x\r\n", swDAC_PR1, swDAC_PR2);
    sdwtemp1 = ((wInitK0 * ((swDAC_PR1 + swDAC_PR2) / 2))) / swDAC_PR1;
    printf("(InitK0 * ((DAC_L + DAC_H)/2))/DAC_H = 0x%x\r\n", sdwtemp1);

    /* ����õ���K0ֵӦ�����趨�ĳ�ʼK0ֵ������15%��Χ�� */
    /* ����K0Ϊ0x2980�����㣬15%Ϊ0x639����1593������MinK0�� -1593~1593
            ֮�䣬�����˷�Χ��Ϊ���� */
    if ((-1593 > sdwtemp1) || (sdwtemp1 > 1593))
    {
        swMinK0 = 0;
        printf("calculate MinK0 error!!!\r\n");
    }
    else
    {
        swMinK0 = (WORD16)sdwtemp1;
        printf("swMinK0 is 0x%x\r\n", swMinK0);
    }

    return swMinK0;

}


#if UHF_RF_PT_IN_EEPROM

/**************************************************************************
* �������ƣ�usp_uhf_rf_readPowerTable
* ������������EEPROM�ж�ȡ�����
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/3/5    V1.1      zdx        RUPƽ̨�� ����
***********************************************************************/

WORD32 usp_uhf_rf_readPowerTable(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wCliDataOff = 0;
    WORD16 wLoop = 0;
    WORD32 dwRcvLen = 0;

    switch(wPTAddr)
    {
        case UHF_RF_PT_ANT14:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT14;
            break;
        }
        case UHF_RF_PT_ANT23:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT23;
            break;
        }
    #if UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT
        case UHF_RF_PT_ANT3:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT3;
            break;
        }
        case UHF_RF_PT_ANT4:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT4;
            break;
        }
    #endif
        default:
        {
            uhfRfDevPrint("uhfRf: wPTAddr = 0x%x   error!!\r\n", wPTAddr);
            break;
        }
    }
    
    uhfRfDevPrint("\r\nRead PowerTable:\r\n");
#if	EEPROM_SWITCH

    usp_drv_eeprom_rcv(wCliDataOff, 2,
                       (BYTE *)&ptPowerTable->wK0, &dwRcvLen);
#endif
    wCliDataOff += 2;

    if (ptPowerTable->wK0 > 0x8000)
    {
        uhfRfDevPrint("uhfRf: powertable abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PT_DATA_ABNORAML,
                  wCliDataOff, ptPowerTable->wK0);
        return USP_UHF_RF_PT_DATA_ABNORAML;
    }

    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
#if	EEPROM_SWITCH

        usp_drv_eeprom_rcv(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atFwTable[wLoop].swPower, &dwRcvLen);
        wCliDataOff += 2;
        usp_drv_eeprom_rcv(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atFwTable[wLoop].wVoltage, &dwRcvLen);
#endif
		wCliDataOff += 2;
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
#if	EEPROM_SWITCH

        usp_drv_eeprom_rcv(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atBwTable[wLoop].swPower, &dwRcvLen);
        wCliDataOff += 2;
        usp_drv_eeprom_rcv(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atBwTable[wLoop].wVoltage, &dwRcvLen);
#endif
		wCliDataOff += 2;
    }

    uhfRfDevPrint("0x%x\r\n", ptPowerTable->wK0);

    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atFwTable[wLoop].swPower,
                      ptPowerTable->atFwTable[wLoop].wVoltage);
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atBwTable[wLoop].swPower,
                      ptPowerTable->atBwTable[wLoop].wVoltage);
    }

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_writePowerTable
* ������������EEPROM��д��������
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/10/09      V1.1     �ڽ�       RUPƽ̨�� ����
***********************************************************************/

WORD32 usp_uhf_rf_writePowerTable(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wCliDataOff = 0;
    WORD16 wLoop = 0;
    WORD32 dwSndLen = 0;
    WORD32 dwRet = 0;

    switch(wPTAddr)
    {
        case UHF_RF_PT_ANT14:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT14;
            break;
        }
        case UHF_RF_PT_ANT23:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT23;
            break;
        }
    #if UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT
        case UHF_RF_PT_ANT3:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT3;
            break;
        }
        case UHF_RF_PT_ANT4:
        {
            wCliDataOff = RF_CLI_DATA_ADDR_ANT4;
            break;
        }
    #endif
        default:
        {
            uhfRfDevPrint("uhfRf: wPTAddr = 0x%x   error!!\r\n", wPTAddr);
            break;
        }
    }

    if (ptPowerTable->wK0 > 0x8000)
    {
        uhfRfDevPrint("uhfRf: powertable abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PT_DATA_ABNORAML,
                  wCliDataOff, ptPowerTable->wK0);
        return USP_UHF_RF_PT_DATA_ABNORAML;
    }
    
    uhfRfDevPrint("\r\nWrite PowerTable:\r\n");
#if	EEPROM_SWITCH

    usp_drv_eeprom_snd(wCliDataOff, 2,
                       (BYTE *)&ptPowerTable->wK0, &dwSndLen);
#endif
    wCliDataOff += 2;

    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
#if	EEPROM_SWITCH

        usp_drv_eeprom_snd(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atFwTable[wLoop].swPower, &dwSndLen);
        wCliDataOff += 2;
        usp_drv_eeprom_snd(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atFwTable[wLoop].wVoltage, &dwSndLen);
#endif

		wCliDataOff += 2;
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
#if	EEPROM_SWITCH

        usp_drv_eeprom_snd(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atBwTable[wLoop].swPower, &dwSndLen);
        wCliDataOff += 2;
        usp_drv_eeprom_snd(wCliDataOff, 2,
                           (BYTE *)&ptPowerTable->atBwTable[wLoop].wVoltage, &dwSndLen);
#endif
		wCliDataOff += 2;
    }

    uhfRfDevPrint("0x%x\r\n", ptPowerTable->wK0);

    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atFwTable[wLoop].swPower,
                      ptPowerTable->atFwTable[wLoop].wVoltage);
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atBwTable[wLoop].swPower,
                      ptPowerTable->atBwTable[wLoop].wVoltage);
    }

    return USP_SUCCESS;
}

#if ((UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT) || (UHF_BOARD_TYPE == BOARD_TYPE_RFCB7))
/**************************************************************************
* �������ƣ�usp_uhf_rf_readGateVoltageValue
* ������������EEPROM�ж�ȡդѹ��ֵ
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵������
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/08/02      V1.0       �ڽ�      ����
* 12/10/09      V1.0       �ڽ�      ȥ�����е�ȫ�ֱ���������һ�����  
**************************************************************************/
WORD32 usp_uhf_rf_readGateVoltageValue(T_USPUHFGateVoltageCtrl *ptGateVolCtrl)
{
    WORD32 dwRcvLen = 0; 
#if	EEPROM_SWITCH    
    usp_drv_eeprom_rcv(RF_EEPROM_GATE_VOL_VALUE0, 2,
                       (BYTE *)&ptGateVolCtrl->wGateVoltage0, &dwRcvLen);
    usp_drv_eeprom_rcv(RF_EEPROM_GATE_VOL_VALUE1, 2,
                       (BYTE *)&ptGateVolCtrl->wGateVoltage1, &dwRcvLen);
#endif
    if (ptGateVolCtrl->wGateVoltage0 > UHF_RF_GATE_VOL_0_MAX ||\
        ptGateVolCtrl->wGateVoltage0 < UHF_RF_GATE_VOL_0_MIN ||\
        ptGateVolCtrl->wGateVoltage1 > UHF_RF_GATE_VOL_1_MAX ||\
        ptGateVolCtrl->wGateVoltage1 < UHF_RF_GATE_VOL_1_MIN)
    {
        uhfRfDevPrint("uhfRf: GateVoltageCtrl WORD Abnormal!! 0x%x, 0x%x\r\n",\ 
                        ptGateVolCtrl->wGateVoltage0,\
                        ptGateVolCtrl->wGateVoltage1);
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_GATE_VOL_ABNORMAL,
                  ptGateVolCtrl->wGateVoltage0, ptGateVolCtrl->wGateVoltage1);
        return USP_UHF_RF_GATE_VOL_ABNORMAL;
    }
    
    uhfRfDevPrint("uhfRf: GateVoltageCtrl WORD 0x%x, 0x%x\r\n",\ 
                        ptGateVolCtrl->wGateVoltage0,\
                        ptGateVolCtrl->wGateVoltage1);

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_writeGateVoltageValue
* ������������EEPROM��д��դѹ��ֵ
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵������
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/10/09      V1.0       �ڽ�        ����
**************************************************************************/
WORD32 usp_uhf_rf_writeGateVoltageValue(T_USPUHFGateVoltageCtrl *ptGateVolCtrl)
{
    WORD32 dwSndLen = 0; 

    if (ptGateVolCtrl->wGateVoltage0 > UHF_RF_GATE_VOL_0_MAX ||\
        ptGateVolCtrl->wGateVoltage0 < UHF_RF_GATE_VOL_0_MIN ||\
        ptGateVolCtrl->wGateVoltage1 > UHF_RF_GATE_VOL_1_MAX ||\
        ptGateVolCtrl->wGateVoltage1 < UHF_RF_GATE_VOL_1_MIN)
    {
        uhfRfDevPrint("uhfRf: GateVoltageCtrl WORD Abnormal!! 0x%x, 0x%x\r\n",\ 
                        ptGateVolCtrl->wGateVoltage0,\
                        ptGateVolCtrl->wGateVoltage1);
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_GATE_VOL_ABNORMAL,
                  ptGateVolCtrl->wGateVoltage0, ptGateVolCtrl->wGateVoltage1);
        return USP_UHF_RF_GATE_VOL_ABNORMAL;
    }    
#if	EEPROM_SWITCH    
    usp_drv_eeprom_snd(RF_EEPROM_GATE_VOL_VALUE0, 2,
                       (BYTE *)&ptGateVolCtrl->wGateVoltage0, &dwSndLen);
    usp_drv_eeprom_snd(RF_EEPROM_GATE_VOL_VALUE1, 2,
                       (BYTE *)&ptGateVolCtrl->wGateVoltage1, &dwSndLen);
#endif
    uhfRfDevPrint("uhfRf: GateVoltageCtrl WORD 0x%x, 0x%x\r\n",\ 
                        ptGateVolCtrl->wGateVoltage0,\
                        ptGateVolCtrl->wGateVoltage1);

    return USP_SUCCESS;
}
#endif

#if UHF_RF_ADJUST_PR_BALANCE
/**************************************************************************
* �������ƣ�usp_uhf_rf_readPRData
* ������������EEPROM�ж�ȡPR�����ֵ
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����PR1��Ӧdac_high_value  PR2��Ӧ����dac_low_value
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/3/5    V1.1      zdx        RUPƽ̨�� ����
**************************************************************************/

WORD32 usp_uhf_rf_readPRData(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wPRDataOff = 0;
    WORD32 dwRcvLen = 0;

    if (UHF_RF_PT_ANT14 == wPTAddr)
    {
        wPRDataOff  = RF_EEPROM_PR_VALUE_ANT14;
    }
    else
    {
        wPRDataOff  = RF_EEPROM_PR_VALUE_ANT23;
    }
#if	EEPROM_SWITCH

    usp_drv_eeprom_rcv(wPRDataOff, 2,
                       (BYTE *)&ptPowerTable->wPR2Value, &dwRcvLen);
    wPRDataOff += 2;
    usp_drv_eeprom_rcv(wPRDataOff, 2,
                       (BYTE *)&ptPowerTable->wPR1Value, &dwRcvLen);
#endif
    if (ptPowerTable->wPR1Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR1Value < g_tRfInitPara.wPraskMin ||
        ptPowerTable->wPR2Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR2Value < g_tRfInitPara.wPraskMin)
    {
        printf("uhfRf: pr data abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PR_DATA_ABNORAML,
                  wPRDataOff, ptPowerTable->wPR2Value);
        return USP_UHF_RF_PR_DATA_ABNORAML;
    }

    ptPowerTable->swMinK0 = CalMinK0(ptPowerTable->wPR1Value,
                                     ptPowerTable->wPR2Value,
                                     ptPowerTable->wK0);

    printf("uhfrf: prwave balance pr1 %x pr2 %x swMinK0 %x\r\n",
           ptPowerTable->wPR1Value,
           ptPowerTable->wPR2Value,
           ptPowerTable->swMinK0);
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_writePRData
* ������������EEPROM��д��PR�����ֵ
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����PR1��Ӧdac_high_value  PR2��Ӧ����dac_low_value
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/10/09    V1.1       �ڽ�        RUPƽ̨�� ����
**************************************************************************/
WORD32 usp_uhf_rf_writePRData(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wPRDataOff = 0;
    WORD32 dwSndLen   = 0;
    WORD32 dwRcvLen   = 0;
    WORD16 wPR1Value  = 0;
    WORD16 wPR2Value  = 0;
    WORD32 dwRet      = USP_SUCCESS;

    if (UHF_RF_PT_ANT14 == wPTAddr)
    {
        wPRDataOff  = RF_EEPROM_PR_VALUE_ANT14;
    }
    else
    {
        wPRDataOff  = RF_EEPROM_PR_VALUE_ANT23;
    }

    if (ptPowerTable->wPR1Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR1Value < g_tRfInitPara.wPraskMin ||
        ptPowerTable->wPR2Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR2Value < g_tRfInitPara.wPraskMin)
    {
        printf("usp_uhf_rf_writePRData: pr data abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PR_DATA_ABNORAML,
                  wPRDataOff, ptPowerTable->wPR2Value);
        return USP_UHF_RF_PR_DATA_ABNORAML;
    }
#if	EEPROM_SWITCH

    usp_drv_eeprom_snd(wPRDataOff, 2,
                       (BYTE *)&ptPowerTable->wPR2Value, &dwSndLen);
    wPRDataOff += 2;
    usp_drv_eeprom_snd(wPRDataOff, 2,
                       (BYTE *)&ptPowerTable->wPR1Value, &dwSndLen);
#endif
    printf("usp_uhf_rf_writePRData: prwave balance pr1 %x pr2 %x\r\n",
           ptPowerTable->wPR1Value,
           ptPowerTable->wPR2Value);

    return USP_SUCCESS;
}

#endif

#else
/**************************************************************************
* �������ƣ�usp_uhf_rf_readPowerTable
* ������������ϵͳ�������ж�ȡ�����
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/3/5    V1.1      zdx        RUPƽ̨�� ����
***********************************************************************/

WORD32 usp_uhf_rf_readPowerTable(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wLoop = 0;
    T_USPUHFFlashRfData tUhfFlashData;

    if (USP_SUCCESS != usp_oss_sysdata_read((BYTE *)"uhf_rf_data", (BYTE *)&tUhfFlashData, sizeof(T_USPUHFFlashRfData)))
    {
        return  USP_UHF_RF_PT_DATA_ABNORAML;
    }

    if (UHF_RF_PT_ANT14 == wPTAddr)
    {
        memcpy((BYTE *)ptPowerTable, &tUhfFlashData.tUhfRfPowerTable1, sizeof(T_USPUHFRfPowerTable));
    }
    else
    {
        memcpy((BYTE *)ptPowerTable, &tUhfFlashData.tUhfRfPowerTable2, sizeof(T_USPUHFRfPowerTable));
    }

    uhfRfDevPrint("\r\nRead PowerTable in sysdata:\r\n");

    if (ptPowerTable->wK0 > 0x8000)
    {
        uhfRfDevPrint("uhfRf: powertable abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PT_DATA_ABNORAML,
                  0, ptPowerTable->wK0);
        return USP_UHF_RF_PT_DATA_ABNORAML;
    }

    uhfRfDevPrint("0x%x\r\n", ptPowerTable->wK0);

    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atFwTable[wLoop].swPower,
                      ptPowerTable->atFwTable[wLoop].wVoltage);
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", ptPowerTable->atBwTable[wLoop].swPower,
                      ptPowerTable->atBwTable[wLoop].wVoltage);
    }

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_writePowerTable
* ������������ϵͳ��������д�붨���
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/10/09    V1.2       �ڽ�        ����
***********************************************************************/

WORD32 usp_uhf_rf_writePowerTable(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    WORD16 wLoop = 0;
    T_USPUHFFlashRfData tUhfFlashData;
    WORD32 dwRet = 0;

    if (ptPowerTable->wK0 > 0x8000)
    {
        uhfRfDevPrint("uhfRf: powertable abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PT_DATA_ABNORAML, 0, ptPowerTable->wK0);
        return USP_UHF_RF_PT_DATA_ABNORAML;
    }

    /* ��ȡFLASH�еĶ������� */
    if (USP_SUCCESS != usp_oss_sysdata_read((BYTE *)"uhf_rf_data", (BYTE *)&tUhfFlashData, sizeof(T_USPUHFFlashRfData)))
    {
        return  USP_UHF_RF_PT_DATA_ABNORAML;
    }

    /* FLASH�����������׶������ݣ��滻FLASH��������Ӧ�Ķ������� */
    if (UHF_RF_PT_ANT14 == wPTAddr)
    {
        memcpy(&tUhfFlashData.tUhfRfPowerTable1, (BYTE *)ptPowerTable, sizeof(T_USPUHFRfPowerTable));
    }
    else
    {
        memcpy(&tUhfFlashData.tUhfRfPowerTable2, (BYTE *)ptPowerTable, sizeof(T_USPUHFRfPowerTable));
    }

    /* д��FLASH */
    uhfRfDevPrint("\r\nWrite PowerTable in sysdata:\r\n");

    if (USP_SUCCESS != usp_oss_sysdata_write((BYTE *)"uhf_rf_data", (BYTE *)&tUhfFlashData, sizeof(T_USPUHFFlashRfData)))
    {
        return  USP_UHF_RF_PT_DATA_ABNORAML;
    }

    /* ��ӡ��һ�׶������� */
    uhfRfDevPrint("0x%x\r\n", ptPowerTable->wK0);
        
    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", tUhfFlashData.tUhfRfPowerTable1.atFwTable[wLoop].swPower,
                      tUhfFlashData.tUhfRfPowerTable1.atFwTable[wLoop].wVoltage);
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", tUhfFlashData.tUhfRfPowerTable1.atBwTable[wLoop].swPower,
                      tUhfFlashData.tUhfRfPowerTable1.atBwTable[wLoop].wVoltage);
    }

    /* ��ӡ�ڶ��׶������� */
    uhfRfDevPrint("\r\n0x%x\r\n", ptPowerTable->wK0);
        
    for (wLoop = 0; wLoop < RF_FORWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", tUhfFlashData.tUhfRfPowerTable2.atFwTable[wLoop].swPower,
                      tUhfFlashData.tUhfRfPowerTable2.atFwTable[wLoop].wVoltage);
    }

    for (wLoop = 0; wLoop < RF_BACKWARD_PT_NUM; wLoop++)
    {
        uhfRfDevPrint("%d 0x%x\r\n", tUhfFlashData.tUhfRfPowerTable2.atBwTable[wLoop].swPower,
                      tUhfFlashData.tUhfRfPowerTable2.atBwTable[wLoop].wVoltage);
    }

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_readPRData
* ������������EEPROM�ж�ȡPR�����ֵ
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����PR1��Ӧdac_high_value  PR2��Ӧ����dac_low_value
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/3/5    V1.1      zdx        RUPƽ̨�� ����
**************************************************************************/

WORD32 usp_uhf_rf_readPRData(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{

    uhfRfDevPrint("uhfrf: prwave balance pr1 %x pr2 %x swMinK0 %x\r\n",
                  ptPowerTable->wPR1Value,
                  ptPowerTable->wPR2Value,
                  ptPowerTable->swMinK0);


    if (ptPowerTable->wPR1Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR1Value < g_tRfInitPara.wPraskMin ||
        ptPowerTable->wPR2Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR2Value < g_tRfInitPara.wPraskMin)
    {
        uhfRfDevPrint("uhfRf: pr data abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PR_DATA_ABNORAML,
                  ptPowerTable->wPR1Value, ptPowerTable->wPR2Value);
        return USP_UHF_RF_PR_DATA_ABNORAML;
    }

    ptPowerTable->swMinK0 = CalMinK0(ptPowerTable->wPR1Value,
                                     ptPowerTable->wPR2Value,
                                     ptPowerTable->wK0);

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�usp_uhf_rf_writePRData
* ������������EEPROM��д��PR�����ֵ
* ���ʵı���
* �޸ĵı���
* ���������ptPowerTable��������ָ��
*                               wPTAddr����������ţ���С�İ����Ķ���
                                 ֧�����׶�������
* * �����������
* �� �� ֵ��USP_SUCCESS���ɹ�������Ϊʧ��
* ����˵����PR1��Ӧdac_high_value  PR2��Ӧ����dac_low_value
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 12/10/09    V1.1       �ڽ�        ����
**************************************************************************/

WORD32 usp_uhf_rf_writePRData(T_USPUHFRfPowerTable *ptPowerTable, WORD16 wPTAddr)
{
    T_USPUHFFlashRfData tUhfFlashData;
    WORD32 dwRet = USP_SUCCESS;

    uhfRfDevPrint("uhfrf: prwave balance pr1 %x pr2 %x swMinK0 %x\r\n",
                  ptPowerTable->wPR1Value,
                  ptPowerTable->wPR2Value,
                  ptPowerTable->swMinK0);


    if (ptPowerTable->wPR1Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR1Value < g_tRfInitPara.wPraskMin ||
        ptPowerTable->wPR2Value > g_tRfInitPara.wPraskMax ||
        ptPowerTable->wPR2Value < g_tRfInitPara.wPraskMin)
    {
        uhfRfDevPrint("uhfRf: pr data abnormal\r\n");
        LAPInsert(FILE_UHF_RF, USP_UHF_RF_PR_DATA_ABNORAML,
                  ptPowerTable->wPR1Value, ptPowerTable->wPR2Value);
        return USP_UHF_RF_PR_DATA_ABNORAML;
    }

    /* ��ȡFLASH�еĶ������� */
    if (USP_SUCCESS != usp_oss_sysdata_read((BYTE *)"uhf_rf_data", (BYTE *)&tUhfFlashData, sizeof(T_USPUHFFlashRfData)))
    {
        return  USP_UHF_RF_PT_DATA_ABNORAML;
    }

    /* �޸�PR����ֵ */
    if (UHF_RF_PT_ANT14 == wPTAddr)
    {
        tUhfFlashData.tUhfRfPowerTable1.wPR1Value = ptPowerTable->wPR1Value;
        tUhfFlashData.tUhfRfPowerTable1.wPR2Value = ptPowerTable->wPR2Value;
    }
    else
    {
        tUhfFlashData.tUhfRfPowerTable2.wPR1Value = ptPowerTable->wPR1Value;
        tUhfFlashData.tUhfRfPowerTable2.wPR2Value = ptPowerTable->wPR2Value;
    }

    /* д��FLASH */
    uhfRfDevPrint("\r\nPowerTable in sysdata:\r\n");

    if (USP_SUCCESS != usp_oss_sysdata_write((BYTE *)"uhf_rf_data", (BYTE *)&tUhfFlashData, sizeof(T_USPUHFFlashRfData)))
    {
        return  USP_UHF_RF_PT_DATA_ABNORAML;
    }

    return USP_SUCCESS;
}

#endif
#endif
