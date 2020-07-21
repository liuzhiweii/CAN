/************************************************************************
* ��Ȩ���� (C)2011, ����ͨѶ����о���
*
* �ļ����ƣ�RFCB.c
* �ļ���ʶ��
* ����ժҪ�����Ӳ��С�İ�����й¶�������޸�
* ����˵����
* ��ǰ�汾�� ZXRIS6700 V04.00.00
* ��    �ߣ�zdx
* ������ڣ�2011-01-10
*
* �޸ļ�¼1��
*    �޸����ڣ�20120316
*    �� �� �ţ�  RUP
*    �� �� �ˣ� �����еĺ������еĺܴ���޸�
*    �޸����ݣ�
* �޸ļ�¼2����
************************************************************************/
#include "usp.h"
#if (UHF_PROTOCOL_USE) &&((UHF_BOARD_TYPE == BOARD_TYPE_RFCB) || (UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT))
#include "uhf_trace.h"
#include "uhf_dbg.h"
#include "uhf_rf.h"
#include "Board.h"
#include "RfCtl.h"
//#include "uhf_time.h"
#include "uhfFpgaRegDef.h"
#define	IO_SWITCH  0
static T_I2CValue g_tI2CValue;
static T_I2CFunValue g_tI2CFunValue;

static WORD32 InitI2CValue(void);
static WORD32 InitI2CFucValue(void);

extern T_UhfRfAntFreqHopCtrl g_tFreqHopCtrl[READER_ANTENNA_COUNT + 1];             /* ��Ƶ���� */
extern T_RfCtrl      g_tRfCtrl;
extern T_RfInitPara  g_tRfInitPara;
extern T_USPUHFGateVoltageCtrl  g_tUhfGateVoltageCtrl;

/**************************************************************************
* �������ƣ�WORD32 InitI2CValue(void)
* ������������ʼ��I2C��������Ƶ��ص�ַ��ֵ��
                                 ��������Ҫд��I2C����ʱ���бȽ�
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
                                 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
*11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
static void uhfRf_SetInitPara(void)
{
    g_tRfInitPara.w20DBAtten          = UHF_RF_20DB_FPGA_ATTEN;
    g_tRfInitPara.wAntOnlineThr       = UHF_RF_ANT_ONLINE_VALUE;
    g_tRfInitPara.wClibrateWaitTime   = UHF_RF_CLIBRATE_SLEEP;
    g_tRfInitPara.swForCompensatePower = RF_FOR_COMPENSATE_POWER;
    g_tRfInitPara.swRevCompensatePower = RF_REV_COMPENSATE_POWER;
    g_tRfInitPara.wFullPower          = UHF_RF_FULL_POWER;
    g_tRfInitPara.wMaxAtten           = UHF_RF_MAX_ATTEN;
    g_tRfInitPara.wMinAtten           = UHF_RF_MIN_ATTEN;
    g_tRfInitPara.wMaxK0              = UHF_RF_MAX_K0;
    g_tRfInitPara.wPraskDefaultHigh   = RF_PRASK_DAC_DEFAULT_HIGH_VALUE;
    g_tRfInitPara.wPraskDefaultLow    = RF_PRASK_DAC_DEFAULT_LOW_VALUE;
    g_tRfInitPara.wPraskMax           = RF_PRASK_DAC_MAX_VALUE;
    g_tRfInitPara.wPraskMin           = RF_PRASK_DAC_MIN_VALUE;
    g_tRfInitPara.wRcvCompensate      = UHF_RF_RVE_COPENSATE;
    g_tRfInitPara.wLKCLAutoAdjInitVal = UHF_RF_LKCL_AUTO_ADJ_INIT_VAL;
    g_tRfInitPara.wMaxPower           = UHF_RF_MAX_POWER;
    g_tRfInitPara.wGateVolDefault0    = UHF_RF_GATE_VOL_DEFAULT0;
    g_tRfInitPara.wGateVolDefault1    = UHF_RF_GATE_VOL_DEFAULT1;

#if  (UHF_BOARD_TYPE == BOARD_TYPE_RFCB)
    if (g_tI2CFunValue.ucRFCBType == 1)
    {
        /*�������ϵ��޸ĺ���޸Ĳ���*/
        g_tRfInitPara.wAntOnlineThr = 1.7;
    }
#endif
}

/**************************************************************************
* �������ƣ�WORD32 InitI2CValue(void)
* ������������ʼ��I2C��������Ƶ��ص�ַ��ֵ��
                                 ��������Ҫд��I2C����ʱ���бȽ�
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
                                 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
*11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 uhfRf_BoardInit(void)
{
    printf("UHF:Board RFCB0\r\n");
    WORD32 dwRet = USP_SUCCESS;
    /*ѡ���߼�ΪRBCB0 FPGA �汾*/
    FpgaRegWrite(0x75, 1);
#if IO_SWITCH
    dwRet = usp_drv_ioexpander_init();
#endif
    if (dwRet != 0)
    {
        printf("UHF: init ioexpander failure %d!!!\r\n", dwRet);
        return dwRet;
    }

    InitI2CValue();
    InitI2CFucValue();

    /*��ʼ����Ƶ�����Ĳ���*/
    uhfRf_SetInitPara();

    SetDATTtoZero();
    
    uhfRf_SetLKCLInitPara();

    /*פ������������ʼ��*/
    uhfRf_initVswrAlarm();

    /*��Ƶ�����õ��Ʒ�ʽ*/
    usp_uhf_rf_setRTModuleType(MODULE_TYPE_PR);
    /*����Ĭ�ϵ������*/
    usp_uhf_rf_setModulateDepth(100);

    usp_uhf_rf_setRevDatt(0x3f);
    uhfRf_SetRevLowNoEn(LOW_NOISE_DISABLE);

    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelBackward);
    FpgaRegWrite(prask_dac_low_value_reg, g_tRfInitPara.wPraskDefaultLow);
    FpgaRegWrite(prask_dac_high_value_reg, g_tRfInitPara.wPraskDefaultHigh);
    FpgaRegWrite(ask_dac_high_value_reg, 0x1fff);
    FpgaRegWrite(REG_DAC_DC_VALUE_REG, 0x0000);
    FpgaRegWrite(REG_GATE_VOLTAGE_CTRL0, UHF_RF_GATE_VOL_DEFAULT0);
    FpgaRegWrite(REG_GATE_VOLTAGE_CTRL1, UHF_RF_GATE_VOL_DEFAULT1);
    /*դѹ�ĵ�����Ϊ����д�룬��Ҫ100us����ʱ*/
    FpgaRegWrite(REG_GATE_VOLTAGE_WR_TRIGER,0);
    FpgaRegWrite(REG_GATE_VOLTAGE_WR_TRIGER,1);
    usleep(100);
    
    /*�ϵ�ʱ��Ϊ����������״̬*/
    FpgaRegWrite(s_rfcl_start, 0x2);
    
    return USP_SUCCESS;

}

/**************************************************************************
* �������ƣ�WORD32 uhfRf_BoardReset(void)
* �������������³�ʼ��PURM�����Ƶ��������
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
                                 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
*11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 uhfRf_BoardReset(void)
{
    uhfRfDevPrint("Board RFCB0\r\n");
    WORD32 dwRet = USP_SUCCESS;
    /*ѡ���߼�ΪRBCB0 FPGA �汾*/
    FpgaRegWrite(0x75, 1);
#if IO_SWITCH
    dwRet = usp_drv_ioexpander_init();
#endif
    if (dwRet != 0)
    {
        uhfRfDevPrint("UHF: init ioexpander failure %d!!!\r\n", dwRet);
        return dwRet;
    }


    /*פ������������ʼ��*/
    uhfRf_initVswrAlarm();

    SetDATTtoZero();

    uhfRf_VAutoAdjust();

    /*��Ƶ�����õ��Ʒ�ʽ*/
    usp_uhf_rf_setRTModuleType(MODULE_TYPE_PR);
    /*����Ĭ�ϵ������*/
    usp_uhf_rf_setModulateDepth(100);

    usp_uhf_rf_setRevDatt(0x3f);
    uhfRf_SetRevLowNoEn(LOW_NOISE_DISABLE);

    usp_uhf_rf_setPowerDir(POWER_DETECT_CTRL_SelBackward);
    FpgaRegWrite(prask_dac_low_value_reg, g_tRfInitPara.wPraskDefaultLow);
    FpgaRegWrite(prask_dac_high_value_reg, g_tRfInitPara.wPraskDefaultHigh);
    FpgaRegWrite(ask_dac_high_value_reg, 0x1fff);
    FpgaRegWrite(REG_DAC_DC_VALUE_REG, 0x0000);
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�WORD32 InitI2CValue(void)
* ������������ʼ��I2C��������Ƶ��ص�ַ��ֵ��
                                 ��������Ҫд��I2C����ʱ���бȽ�
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
                                 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
*11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
static WORD32 InitI2CValue(void)
{
#if 0
    WORD32 dwValue = 0;
    WORD32 dwRet = USP_SUCCESS;
    dwRet = usp_drv_ioexpander_rcv(USP_IOE_RFCB_TYPE_ID, &dwValue);

    if (dwRet != 0)
    {
        return  dwRet;
    }

    g_tI2CValue.ucADR38 = (BYTE)dwValue;

    dwRet = usp_drv_ioexpander_rcv(USP_IOE_ICB_FUN_ALL, &dwValue);

    if (dwRet != 0)
    {
        return  dwRet;
    }

    g_tI2CValue.ucADR39 = (BYTE)dwValue;

    dwRet = usp_drv_ioexpander_rcv(USP_IOE_ICC_FUN_ALL, &dwValue);

    if (dwRet != 0)
    {
        return  dwRet;
    }

    g_tI2CValue.ucADR3A = (BYTE)dwValue;

    dwRet = usp_drv_ioexpander_rcv(USP_IOE_ICD_FUN_ALL, &dwValue);

    if (dwRet != 0)
    {
        return  dwRet;
    }

    g_tI2CValue.ucADR3B = (BYTE)dwValue;

    dwRet = usp_drv_ioexpander_rcv(USP_IOE_RFCB_VER_ID, &dwValue);

    if (dwRet != 0)
    {
        return  dwRet;
    }

    g_tI2CValue.ucADR3C = (BYTE)dwValue;
	#endif
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�WORD32 InitI2CFucValue(void)
* ������������ʼ��I2C�����ϸ���������ֵ�����ñ�����
                                 WORD32 InitI2CValue(void)֮��
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
                                 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
*11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
static WORD32 InitI2CFucValue(void)
{
    g_tI2CFunValue.ucRFCBType = g_tI2CValue.ucADR38;
    g_tI2CFunValue.ucRFCBVer = g_tI2CValue.ucADR3C;
    g_tI2CFunValue.ucAntIndex = (g_tI2CValue.ucADR39 & USP_IOE_ANT_CONTROL_MASK) >> USP_IOE_ANT_CONTROL_POS;
    g_tI2CFunValue.ucFwDatt = (g_tI2CValue.ucADR39 & USP_IOE_RX_DATT_MASK) >> USP_IOE_RX_DATT_POS;
    g_tI2CFunValue.ucLinkCho = (g_tI2CValue.ucADR3B & USP_IOE_PWRDET_SEL_MASK) >> USP_IOE_PWRDET_SEL_POS;
    g_tI2CFunValue.ucLo2Datt = (g_tI2CValue.ucADR3B & USP_IOE_RX_LO_DATT_MASK) >> USP_IOE_RX_LO_DATT_POS;
    g_tI2CFunValue.ucLowNoEn = (g_tI2CValue.ucADR3A & USP_IOE_RX_LNA_EN_MASK) >> USP_IOE_RX_LNA_EN_POS;
    g_tI2CFunValue.ucRwCryseCho = (g_tI2CValue.ucADR3A & USP_IOE_RX_LO_SEL_MASK) >> USP_IOE_RX_LO_SEL_POS;
    g_tI2CFunValue.ucRwDatt =  (g_tI2CValue.ucADR3A & USP_IOE_RX_DATT_MASK) >> USP_IOE_RX_DATT_POS;
    g_tI2CFunValue.ucRwFilter = (g_tI2CValue.ucADR3B & USP_IOE_ADG704_A_MASK) >> USP_IOE_ADG704_A_POS;
    g_tI2CFunValue.ucLKCLSwitch = 0; /*�ϵ�ر�*/
    return USP_SUCCESS;
}
/**************************************************************************
* �������ƣ�����RFCB_SetACDatt
* ��������������DATT˥��ֵ
* ���������д��Ĺ���DATT˥��ֵ
* ���������
* �� �� ֵ��    WORD16  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 09/07/29	     V03.00.00	    XXXX	      XXXX
**************************************************************************/
WORD32 usp_uhf_rf_setACDatt(WORD16 wAttenuation)
{
    WORD16 wTempAttenuation;
    wTempAttenuation = (~(wAttenuation >> 1)) & (0x1f);
    uhfRfDevPrint("uhfRf:The Real Attenuation is %d\r\n", wTempAttenuation);
    uhfRfDevPrint("uhfRf:The Atten limit is %d Fpga Atten %x\r\n",
                  UHF_RF_MIN_REAL_ATTEN, wAttenuation);

    g_tI2CFunValue.ucFwDatt = ((BYTE)(wAttenuation) & (0x3f));

    return 0;
}


/**************************************************************************
* �������ƣ�uhfRf_setPowerToZero
* ����������д�����PRֵ��������Ĺ�����Сʱ
                                ��K0ֵ
* �����������ǰ��������ֵ
* �����������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2012/02/17	         �Զ���           ����
**************************************************************************/

void uhfRf_setPowerToZero()
{
    uhfDevPrint("wAntIndex=%d\r\n",g_tRfCtrl.wAntIndex);
    WORD16 wMinK0 = uhfRf_getPRMinK0(g_tRfCtrl.wAntIndex);
    /*ֹͣ�����ز���ʱ��ѿ�������С����С����й¶*/
    FpgaRegWrite(REG_FORWARD_PWADJ, wMinK0);   
    /* add by zongjie 2013.1.10 */
    /* ��Ӳ��������ӣ�Ϊ�˽�����������й©�źŹ�������� */
    /* �ڹر��ز�֮�󣬽�0x87�Ĵ���дΪ0x1fff����ʹK0ֵ�����ã������������й© */
    FpgaRegWrite(REG_DAC_DC_VALUE_REG, 0x1fff);
    //usp_oss_vos_usleep(DAC_DC_CHANGE_DELAY);
    /* end 2013.1.10 */
}

/**************************************************************************
* �������ƣ�uhfRf_SetForDataRate
* ������������������ǰ�����ʣ�ֻ��RUP������
* �����������
* ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2012/02/17	         �Զ���           ����
**************************************************************************/

void  uhfRf_SetForDataRate(WORD16 wForRate)
{

}

/**************************************************************************
* �������ƣ�Set_DATT_to_0
* ��������������DATTΪȫ˥
* �����������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2011/06/17	          �ڽ�           ����
**************************************************************************/
WORD32 SetDATTtoZero(void)
{
    g_tI2CFunValue.ucFwDatt = 0;
    usp_uhf_rf_WriteAntDatt();
    return USP_SUCCESS;
}
/**************************************************************************
* ��������: void usp_uhf_rf_setPowerDir(WORD16 wDirect)
* ��������: ���ù��ʼ��ķ���
* �������: WORD16 wDirect ����
                              POWER_DETECT_CTRL_SelBackward
* �������:
* ����ֵ:       WORD32    0 �ɹ�
*                                               ���� ����
* ����˵��
* �޸�����   �汾��   �޸���     �޸�����
* -------------------------------------------
*  11/01/15          V1.0              zdx             ����
**************************************************************************/
WORD32  usp_uhf_rf_setPowerDir(WORD16 wDirect)
{
    WORD32 dwRet = USP_SUCCESS;
    BYTE ucI2CVal = 0;

    if (wDirect > POWER_DETECT_CTRL_SelForward)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }
    else
    {
        g_tI2CFunValue.ucLinkCho = wDirect;
        ucI2CVal = GetAdr3BVal;
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICD_FUN_ALL, (WORD32)ucI2CVal);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }
    }

    g_tI2CValue.ucADR3B = ucI2CVal;
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�uhfRf_SetRevLowNoEn
* ���������������ʹ�ܺ���,Ŀǰ��Ϊ��ʹ��
* ����������Ƿ�ʹ��
* ���������
* �� �� ֵ��
            WORD32
                         0��    �ɹ�
                      ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2009/8/3    V03.00.00       zdx          �޸�
************************************************************************/

WORD32 uhfRf_SetRevLowNoEn(BYTE ucLowNoEn)
{
    WORD32 dwRet = USP_SUCCESS;
    BYTE ucI2CVal = 0;

    g_tI2CFunValue.ucLowNoEn = (ucLowNoEn) & (0x1);
    ucI2CVal = GetAdr3AVal;
#if IO_SWITCH
    dwRet = usp_drv_ioexpander_snd(USP_IOE_ICC_FUN_ALL, (WORD32)ucI2CVal);
#endif
    if (dwRet != USP_SUCCESS)
    {
        uhfRfDevPrint("Set Low En Error\r\n");
        return dwRet;
    }

    g_tI2CValue.ucADR3A = ucI2CVal;
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�WORD32 usp_uhf_rf_selectAnt(WORD32 dwAntennaIndex)
* ��������������ѡ��
* ���������
            WORD32 dwAntennaIndex ���������� 0 ~ 3��
* ���������
* �� �� ֵ��
            WORD32
                         0��    �ɹ�
                      ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2009/8/3    V03.00.00       zdx          �޸�
************************************************************************/
WORD32 usp_uhf_rf_selectAnt(WORD16 wAntIndex)
{
    WORD32 dwRet = USP_SUCCESS;
    BYTE ucI2CValue = 0;

    if ((wAntIndex == 0) || (wAntIndex > 4))
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    /*д��PR�����ֵ*/
    FpgaRegWrite(prask_dac_high_value_reg, uhfRf_getPRDacHigh(wAntIndex));
    FpgaRegWrite(prask_dac_low_value_reg, uhfRf_getPRDacLow(wAntIndex));

    /* �˰汾ֻ�ɹ������շ�һ��ģʽ���Ҳ�֧��LBT */
    g_tI2CFunValue.ucRwCryseCho = DEMOD_LO_SEL_COUPLE;

    ucI2CValue = GetAdr3AVal;

    if (g_tI2CValue.ucADR3A != ucI2CValue)
    {
        uhfRfDevPrint("Write reverse I2C Datt\r\n");
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICC_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR3A = ucI2CValue;
    }

    if (1 == wAntIndex)
    {
        g_tI2CFunValue.ucAntIndex = 0x2;
    }
    else if (2 == wAntIndex)
    {
        g_tI2CFunValue.ucAntIndex = 0x0;
    }
    else if (3 == wAntIndex)
    {
        g_tI2CFunValue.ucAntIndex = 0x3;
    }
    else if (4 == wAntIndex)
    {
        g_tI2CFunValue.ucAntIndex = 0x1;
    }


    uhfRfDevPrint("uhfRf:Selceted FPGA %x\r\n", g_tI2CFunValue.ucAntIndex);
    return 0;

}


/**************************************************************************
* �������ƣ�RFCB_ACWriteAntAndDatt
* ��������������DATT˥��ֵ�����ߺ�
* ������������ߺź�˥��ֵ��ɵ�һ���ֽڵ�I2C ����
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 usp_uhf_rf_WriteAntDatt()
{
    BYTE ucI2CValue = 0;
    BYTE ucI2CClrValue = 0;
    WORD32 dwRet = USP_SUCCESS;

    ucI2CValue = GetAdr39Val;

    if (g_tI2CValue.ucADR39 != ucI2CValue)
    {
        ucI2CClrValue = Adr39ValClrDatt;

        if (g_tI2CValue.ucADR39 != ucI2CClrValue)
        {
            uhfRfDevPrint("uhfRf:Write I2C Datt 0x%x ", ucI2CClrValue);
#if IO_SWITCH
            dwRet = usp_drv_ioexpander_snd(USP_IOE_ICB_FUN_ALL, (WORD32)ucI2CClrValue);
#endif
            if (dwRet != USP_SUCCESS)
            {
                return dwRet;
            }

            g_tI2CValue.ucADR39 = ucI2CClrValue;
        }

        uhfRfDevPrint("uhfRf:Write I2C Datt 0x%x\r\n", ucI2CValue);
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICB_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR39 = ucI2CValue;
    }

    return dwRet;
}

/**************************************************************************
* �������ƣ�RFCB_SetRevDatt
* ��������������DATT����
* ������������ߺź�˥��ֵ��ɵ�һ���ֽڵ�I2C ����
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 usp_uhf_rf_setRevDatt(WORD16 wDattVal)
{
    BYTE ucI2CValue = 0;
    WORD32 dwRet = USP_SUCCESS;

    g_tI2CFunValue.ucRwDatt = wDattVal & 0x3f;
    ucI2CValue = GetAdr3AVal;

    if (g_tI2CValue.ucADR3A != ucI2CValue)
    {
        uhfRfDevPrint("uhfRf:Write I2C Reverse Datt %x\r\n", ucI2CValue);
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICC_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR3A = ucI2CValue;
    }

    return dwRet;
}

/**************************************************************************
* �������ƣ�WORD32 usp_uhf_rf_setRevFilter(BYTE ucFilter)
* ���������������˲�������
* ����������˲������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 usp_uhf_rf_setRevFilter(BYTE ucFilter)
{
    BYTE ucI2CValue = 0;
    WORD32 dwRet = USP_SUCCESS;

    g_tI2CFunValue.ucRwFilter = (ucFilter & 0x03);
    ucI2CValue = GetAdr3BVal;

    if (g_tI2CValue.ucADR3B != ucI2CValue)
    {
        uhfRfDevPrint("Write I2C Reverse Filter %x\r\n", ucI2CValue);
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICD_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR3B = ucI2CValue;
    }

    return dwRet;
}



/**************************************************************************
* �������ƣ�SetLO2Datt
* �������������÷��������ı����˥��ֵ�Ĵ���
* ���������д��Ĺ���DATT˥��ֵ
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 usp_uhf_rf_SetLO2Datt(WORD16 wCurPower)
{
    WORD16 wData = 0;
    WORD16 wLoop = 0;
    WORD16 awAdjustTable[5] = {800, 400, 200, 100 , 50};
    WORD32 dwRet = USP_SUCCESS;
    BYTE   ucI2CValue = 0;

    wData = (g_tI2CValue.ucADR3B & USP_IOE_RX_LO_DATT_MASK) >> USP_IOE_RX_LO_DATT_POS;

    /* ���ݵ����㷨���е��� */
    if (2700 >= wCurPower)
    {
        wData |= 0x001f;
    }
    else if (2700 < wCurPower && 4250 > wCurPower)
    {
        /* ��ȡ��Ҫ�����ֵ */
        wCurPower = wCurPower - 2700;
        wData = wData | 0x001f;

        for (wLoop = 0; wLoop < 5; wLoop++)
        {
            if (wCurPower >= awAdjustTable[wLoop])
            {
                wData &= ~((WORD16)(1 << (4 - wLoop)));
                wCurPower -= awAdjustTable[wLoop];
            }
        }

    }
    else
    {
        wData = wData & 0xffe0;
    }
   /*���һλ�����ƣ����*/
    g_tI2CFunValue.ucLo2Datt = (wData & 0x1e);

    ucI2CValue = GetAdr3BVal;

    if (g_tI2CValue.ucADR3B != ucI2CValue)
    {
        uhfRfDevPrint("uhfRf:Write I2C Lo2Datt\r\n");
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICD_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR3B = ucI2CValue;
    }


    uhfRfDevPrint("uhfRf:Use Lo2 Datt 0x%x \r\n", wData);
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�uhfRf_SetLKCLSwitch
* ��������������й¶�������أ�����ʱ��
* �������������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 uhfRf_SetLKCLSwitch(BYTE ucSwitch)
{
    WORD32 dwRet = USP_SUCCESS;
    BYTE   ucI2CValue = 0;
    
    uhfRfDevPrint("uhfRf:lkcl switch %d\r\n",ucSwitch);
    
    g_tI2CFunValue.ucLKCLSwitch= (ucSwitch & 0x01);
    ucI2CValue = GetAdr3BVal;
    if (g_tI2CValue.ucADR3B != ucI2CValue)
    {
        uhfRfDevPrint("uhfRf:Write I2C lkclswitch\r\n");
#if IO_SWITCH
        dwRet = usp_drv_ioexpander_snd(USP_IOE_ICD_FUN_ALL, (WORD32)ucI2CValue);
#endif
        if (dwRet != USP_SUCCESS)
        {
            return dwRet;
        }

        g_tI2CValue.ucADR3B = ucI2CValue;
    }
    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�WORD32 uhfRf_ChooseRevFilter(WORD16 wLinkRate, BYTE ucSwitch)
* ������������������ѡ���˲���
* ���������
* ���������
* �� �� ֵ��    WORD32  �ɹ�����0
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
#define FILTER40      0x0
#define FILTER80160   0x1
#define FILTER320640  0x2
#define NOFILTER      0x3
WORD32 uhfRf_ChooseRevFilter(WORD16 wLinkRate, BYTE ucSwitch)
{
    uhfRfDevPrint("Filter sWitch %d\r\n", ucSwitch);

    if (ucSwitch == 0)
    {
        usp_uhf_rf_setRevFilter(NOFILTER);
    }
    else
    {
        if (wLinkRate > 160)
        {
            usp_uhf_rf_setRevFilter(FILTER320640);
        }
        else if (wLinkRate > 40)
        {
            usp_uhf_rf_setRevFilter(FILTER80160);
        }
        else
        {
            usp_uhf_rf_setRevFilter(FILTER40);
        }
    }

    return USP_SUCCESS;
}


/**************************************************************************
* �������ƣ�uhfRf_SetAmp
* ����������epld���ƵĹ��ŵ�Դ�Ĵ������ص������ͬʱ
                                 �������Ϊ0��
* ���������0 �ر� 1 ��
* ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 12/02/17	     RUP       zdx	        ����
* 12/06/7	     RUP       zongjie	             ������bt�汾�ж���դѹ�Ŀ���
**************************************************************************/

void usp_uhf_rf_setAmp(WORD16 wAction)
{
#if (UHF_BOARD_TYPE == BOARD_TYPE_RFCB_BT)
    /* �򿪹���ʱ�����ȴ�դѹ���ٴ򿪹��� */
    if (RF_AMP_ON == wAction)
    {
       FpgaRegWrite(REG_GATE_VOLTAGE_CTRL0, g_tUhfGateVoltageCtrl.wGateVoltage0);
       FpgaRegWrite(REG_GATE_VOLTAGE_CTRL1, g_tUhfGateVoltageCtrl.wGateVoltage1);
       uhf_epld_reg_write(EPLD_RF_AMP_CTRL_REG, wAction);
    }
    else/* �رչ���ʱ���ȹرչ��ţ��ٹر�դѹ */
    {
       uhf_epld_reg_write(EPLD_RF_AMP_CTRL_REG, wAction);
       FpgaRegWrite(REG_GATE_VOLTAGE_CTRL0, 0);
       FpgaRegWrite(REG_GATE_VOLTAGE_CTRL1, 0);
    }
#else
      uhf_epld_reg_write(EPLD_RF_AMP_CTRL_REG, wAction);
#endif
}


/**********************************************************************
* �������ƣ�usp_uhf_rf_setDemodLoSel
* ������������Ƶ����ʽ
* ���������
            WORD16 wType  ����ѡ�� 1  ǰ����� 0 ���໷
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010.1.4     V0.0.1       zdx   �޸�
************************************************************************/
void usp_uhf_rf_setDemodLoSel(WORD16 wType)
{
   FpgaRegWrite(REG_DEMOD_LO_SEL, wType);
}


#endif

