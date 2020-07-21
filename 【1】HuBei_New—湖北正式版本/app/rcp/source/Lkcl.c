#include "usp.h"
#if (UHF_PROTOCOL_USE)&&(UHF_BOARD_TYPE !=BOARD_TYPE_RBCB1)

#if (USP_UHF_LKCL_USE)
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf_rf.h"
#include "Board.h"
#include "RfCtl.h"
//#include "uhf_time.h"
#include "Lkcl.h"


extern T_UhfRfAntFreqHopCtrl g_tFreqHopCtrl[READER_ANTENNA_COUNT + 1];
extern T_RfCtrl        g_tRfCtrl;
extern T_RfInitPara    g_tRfInitPara;
T_USPUHFLKCLInitParas g_tInintLKCLAdj;
static T_InitPhase     g_InitPhase[] = UHF_RF_INIT_PHASE;
static T_SingleAntLKCLPara g_tReaderLKCLPara[READER_ANTENNA_COUNT + 1];
static WORD32 dwtemp;


/**************************************************************************
* �������ƣ�uhfRf_initLKCLParas
* ������������ȡһ��Ƶ�ʱ�й¶��������Ĵ�ſռ�
* ���������wAntIndex ���ߺ�
                                 wFreqTableID  Ƶ�ʱ�
 * ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/

void uhfRf_initLKCLParas(void)
{
   memset(g_tReaderLKCLPara, 0,
            sizeof(T_SingleAntLKCLPara) * (READER_ANTENNA_COUNT + 1));
}

/**************************************************************************
* �������ƣ�uhfRf_getLKCLParas
* ����������й¶����ֵ���ֵ�Ļ�ȡ
* ���������wAntIndex ���ߺ�
                                 wFreqTableID  Ƶ�ʱ�
                                 wFreqChnnelNo  Ƶ����
* ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/

static T_USPUHFLKCLParas *uhfRf_getLKCLParas(WORD16 wAntIndex, WORD16 wFreqTableID, WORD16 wFreqChnnelNo)
{

    WORD16 wLoopIndex = 0;

    /*û�ж�����*/
    if (0 == g_tReaderLKCLPara[wAntIndex].wLKCLTableLen)
    {
        return NULL;
    }

    /*�ж�����������ǲ��Ǻ͵�ǰ��Ƶ�ʱ�һ��*/
    for (wLoopIndex = 0;
         wLoopIndex < g_tReaderLKCLPara[wAntIndex].wLKCLTableLen; wLoopIndex++)
    {
        if (g_tReaderLKCLPara[wAntIndex].tSingleFreqTableLKCLPara[wLoopIndex].wHopTableID
            == wFreqTableID)
        {
            break;
        }
    }

    /*û���ҵ�*/
    if (wLoopIndex >= g_tReaderLKCLPara[wAntIndex].wLKCLTableLen)
    {
        return NULL;
    }

    /*�ҵ���*/
    return  &g_tReaderLKCLPara[wAntIndex]\
            .tSingleFreqTableLKCLPara[wLoopIndex].tAntLKCLPara[wFreqChnnelNo];

}

/**************************************************************************
* �������ƣ�uhfRf_getFreqTableLKCLParas
* ������������ȡһ��Ƶ�ʱ�й¶��������Ĵ�ſռ�
* ���������wAntIndex ���ߺ�
                                 wFreqTableID  Ƶ�ʱ�
 * ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/

T_SingleFreqTableLKCLPara *uhfRf_getFreqTableLKCLParas(WORD16 wAntIndex, WORD16 wFreqTableID)
{

    T_SingleFreqTableLKCLPara *ptFreqTableLkclTable = NULL;

    /*�����ǰ��Ƶ���Ѿ����й�й¶�����ˣ��򷵻ؿ�ָ��*/
    if (NULL != uhfRf_getLKCLParas(wAntIndex, wFreqTableID, 0))
    {
        return NULL;
    }

    ptFreqTableLkclTable =
        &g_tReaderLKCLPara[wAntIndex]\
        .tSingleFreqTableLKCLPara[g_tReaderLKCLPara[wAntIndex].wLKCLIndex];

    g_tReaderLKCLPara[wAntIndex].wLKCLTableLen++;

    if (g_tReaderLKCLPara[wAntIndex].wLKCLTableLen > UHF_RF_MAX_LKCL_TABLE_LEN)
    {
        g_tReaderLKCLPara[wAntIndex].wLKCLTableLen = UHF_RF_MAX_LKCL_TABLE_LEN;
    }

    g_tReaderLKCLPara[wAntIndex].wLKCLIndex++;

    if (g_tReaderLKCLPara[wAntIndex].wLKCLIndex >= UHF_RF_MAX_LKCL_TABLE_LEN)
    {
        g_tReaderLKCLPara[wAntIndex].wLKCLTableLen = 0;
    }

    return ptFreqTableLkclTable;
}

/**************************************************************************
* �������ƣ�void RFCB_SetLKCLInitPara(void)
* ��������������й¶������ʱ�ĳ�ʼ������
* �����������
* ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
void uhfRf_SetLKCLInitPara(void)
{
    WORD16  wLoop = 0;

    g_tInintLKCLAdj.wInitX0      = UHF_RF_LKCL_INIT_X0;
    g_tInintLKCLAdj.wInitX1      = UHF_RF_LKCL_INIT_X1;
    g_tInintLKCLAdj.wInitY0      = UHF_RF_LKCL_INIT_Y0;
    g_tInintLKCLAdj.wInitY1      = UHF_RF_LKCL_INIT_Y1;

    g_tInintLKCLAdj.dwAdjThrHold = UHF_RF_LKCL_ADJ_THRHOLD;

    g_tInintLKCLAdj.wIntegralMax = UHF_RF_LKCL_INTERG_MAX;
    g_tInintLKCLAdj.wIntegralMin = UHF_RF_LKCL_INTERG_MIN;

    g_tInintLKCLAdj.wInitcos     = UHF_RF_LKCL_INIT_COS;
    g_tInintLKCLAdj.wInitSin     = UHF_RF_LKCL_INIT_SIN;

    g_tInintLKCLAdj.wStopThd     = UHF_RF_LKCL_STOP_THD;
    g_tInintLKCLAdj.wFineThrHold = UHF_RF_LKCL_FINE_THRHOD;

    g_tInintLKCLAdj.wDacChange   = UHF_RF_LKCL_DAC_CHANGE;

    uhfRf_initLKCLParas();

    /*���Ķ���������ʱ�����й¶�����ĳ�ʼֵ*/
    FpgaRegWrite(s_x0_reg, g_tInintLKCLAdj.wInitX0);
    FpgaRegWrite(s_x1_reg, g_tInintLKCLAdj.wInitX1);
    FpgaRegWrite(s_y0_reg, g_tInintLKCLAdj.wInitY0);
    FpgaRegWrite(s_y1_reg, g_tInintLKCLAdj.wInitY1);

    FpgaRegWrite(s_integral_max, g_tInintLKCLAdj.wIntegralMax);
    FpgaRegWrite(s_integral_min, g_tInintLKCLAdj.wIntegralMin);

    FpgaRegWrite(s_dac_change, g_tInintLKCLAdj.wDacChange);

    FpgaRegWrite(s_threhold_val, g_tInintLKCLAdj.wStopThd);

    FpgaRegWrite(s_cos_theta, g_tInintLKCLAdj.wInitcos);
    FpgaRegWrite(s_sin_theta, g_tInintLKCLAdj.wInitSin);

    /*��ʼ��Ϊֹͣй¶����״̬ͬʱ
         ����ʼ����ֵд�벢�ұ�֤�ڵ�ѹ��У׼��ʱ����ʱ��*/
    FpgaRegWrite(s_rfcl_start, 0);
    uhfRf_VAutoAdjust();

}

/**************************************************************************
* �������ƣ�void uhfRf_ResetLKCLInitPara(void)
* ��������������й¶������ʱ�ĳ�ʼ������
* �����������
* ���������
* �� �� ֵ��    ��
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
void uhfRf_VAutoAdjust(void)
{
    SWORD16 swAdcQ = 0;
    SWORD16 swAdcI = 0;
    SWORD32 sdwAdcValue = g_tRfInitPara.wLKCLAutoAdjInitVal;

    FpgaRegWrite(REG_LKCL_SWITCH, 1);
    usp_uhf_rf_setAmp(RF_AMP_OFF);
    usp_uhf_rf_txSet(UHF_RF_STOP_TX);
    usp_uhf_rf_setRevDatt(0);

    /*ÿ�μ��ǰ��Ҫ��ʼ��һ��*/
    FpgaRegWrite(REG_I_LKCL_ADJ1, sdwAdcValue);
    FpgaRegWrite(REG_I_LKCL_ADJ2, sdwAdcValue);
    usleep(100000);

    swAdcI = FpgaRegRead(I_ADC_idata);
    swAdcQ = FpgaRegRead(I_ADC_qdata);
    printf("uhfRf:I %d q %d fixed value %d\r\n", swAdcI, swAdcQ, sdwAdcValue);
    swAdcI = sdwAdcValue + (swAdcI * 8);
    swAdcQ = sdwAdcValue + (swAdcQ * 8);
    printf("uhfRf:caculated i %x q %x\r\n", swAdcI, swAdcQ);
    FpgaRegWrite(REG_I_LKCL_ADJ1, swAdcI);
    FpgaRegWrite(REG_I_LKCL_ADJ2, swAdcQ);
}

/**************************************************************************
* �������ƣ�static WORD32 uhfRf_SetLKCLFreq(WORD32 dwFreq)
* ����������й¶������ʱ������Ƶ�ʼ�¼��Ƶ�ʵ�ȫ��
                                 ����
* ���������deFreq ���õ�Ƶ��
* ���������
* �� �� ֵ��    �ɹ�����0��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
static WORD32 uhfRf_SetLKCLFreq(WORD32 dwFreq)
{
    g_tRfCtrl.dwWorkFreq = dwFreq;
    return usp_uhf_rf_setPLLFreq(g_tRfCtrl.dwWorkFreq);
}
/**************************************************************************
* �� �� ֵ��    �ɹ�����0��������
* �������ƣ�WORD32 RFCB_RFFreqLKCL(T_FreqHopCtrl *ptAntFreqHopCtrl, BYTE ucAntIndex)
* ����������й¶�������̴������һ������
* ���������T_FreqHopCtrl *ptAntFreqHopCtrl ��ǰ���ߵ�Ƶ����
                                     BYTE ucAntIndex  ���ߺ�
* ���������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 uhfRf_FreqLKCL(T_USPUHFFreqTableEntry *ptHopTable, BYTE ucAntIndex)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wLoop = 0;
    WORD32 dwFreq = 0;
    WORD32 dwVswrAlarmVal = 0;
    T_SingleFreqTableLKCLPara *ptAntLKCLPara =
        uhfRf_getFreqTableLKCLParas(ucAntIndex, ptHopTable->wHopTableID);

    /*�����NULL ˵���Ѿ���������*/
    if (NULL == ptAntLKCLPara)
    {
        uhfRfDevPrint("uhfRf: has been lkcled\r\n");
        return USP_SUCCESS;
    }

    uhfRfDevPrint("uhfRf:Ant index %d LKCLStart\r\n", ucAntIndex);
    ptAntLKCLPara->wHopTableID = ptHopTable->wHopTableID;

    dwRet = usp_uhf_rf_selectAnt(ucAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:Rf antenna set failed!  dwRet = 0x%x\r\n", dwRet);
        return USP_UHF_RF_ANT_SET_ERROR;
    }

    /* д������*/
    usp_uhf_rf_WriteAntDatt();
    
    dwRet = uhfRf_SetLKCLPower(ucAntIndex);

    if (USP_SUCCESS != dwRet)
    {
        uhfRfDevPrint("uhfRf:Set 30DB Error happens use default!  dwRet = 0x%x\r\n", dwRet);
    }

    dwVswrAlarmVal = uhfRf_IsAntProtected();

    if (USP_UHF_ANT_IS_PROTECTED == dwVswrAlarmVal)
    {
        uhfRfDevPrint("uhfRf:Vswr is Protected Cannot LKCL\r\n");
        usp_uhf_rf_txSet(UHF_RF_STOP_TX);
        return USP_UHF_ANT_IS_PROTECTED;
    }
    uhfRf_SetLKCLSwitch(UHF_RF_FAST_LKCL_ON);
    usp_uhf_rf_txSet(UHF_RF_START_TX);

    /* �ȴ���Ƶ�ź��ȶ� */
    //USP_OSS_VOS_SLEEP_USEC(1000); /*��ʱ����Ƶ�ȶ�*/
    //usp_drv_timer_usleep(1500);
    
    while (wLoop < ptHopTable->wFrequeceNum)
    {
        dwFreq = ptHopTable->dwFrequeces[wLoop];
        dwRet = uhfRf_SetLKCLFreq(dwFreq);

        if (USP_SUCCESS != dwRet)
        {
            break;
        }

        /*���ö���ɹ����K0��DATT*/
        uhfRf_SetLKCLPowerPara(ucAntIndex);
        //USP_OSS_VOS_SLEEP_USEC(1000); /*��ʱ����Ƶ�ȶ�*/
        //usp_drv_timer_usleep(1500);
        
        dwRet = uhfRf_FPGALKCLProc(&g_tInintLKCLAdj,
                                   &ptAntLKCLPara->tAntLKCLPara[wLoop],
                                   &ptAntLKCLPara->wTryIndex);

        if (USP_SUCCESS != dwRet)
        {
            if (UHF_RF_LKCL_STOP_THD_STAGE2 != g_tInintLKCLAdj.wStopThd)
            {
                g_tInintLKCLAdj.dwAdjThrHold = UHF_RF_LKCL_ADJ_THRHOLD_STAGE2;
                g_tInintLKCLAdj.wStopThd = UHF_RF_LKCL_STOP_THD_STAGE2;

            }
            else
            {
                LAPInsert(FILE_RFCB2, USP_UHF_LKCL_FAILED, dwFreq, 0);
                uhfRfDevPrint("uhfRf:============lck Freq %d Failed===========\r\n", dwFreq);
                break;
            }
        }
        else
        {
            wLoop++;  /*ת����һ��Ƶ�����й¶����*/
            uhfRfDevPrint("uhfRf:=========LKCL Freq %d Success stopthd %d=============\r\n", 
                                                   dwFreq,g_tInintLKCLAdj.wStopThd);

            /*ÿ�ζ���������С��Լ��ֵ��ʼ*/
            g_tInintLKCLAdj.dwAdjThrHold = UHF_RF_LKCL_ADJ_THRHOLD;
            g_tInintLKCLAdj.wStopThd = UHF_RF_LKCL_STOP_THD;
        }
    }

    usp_uhf_rf_txSet(UHF_RF_STOP_TX);
    uhfRf_SetLKCLSwitch(UHF_RF_FAST_LKCL_OFF);
    return dwRet;
}
/**********************************************************************
* �������ƣ�FPGALKCLProcTry
* �������������һ���ǶȽ���й¶��������,�ļ��ں���
* ���������T_USPUHFLKCLInitParas *ptInintLKCLAdj  й¶�����ĳ�ʼ������
                                 T_USPUHFLKCLParas *ptLKCLSetPara �����ɹ���ļ�¼ֵ
* ���������
* �� �� ֵ    ���ɹ�����0 ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/12/12	        RUPƽ̨��	    zdx	      ����
***********************************************************************/
WORD32 usp_uhf_rf_FPGALKCLProcTry(T_USPUHFLKCLInitParas *ptInintLKCLAdj,
                                    T_USPUHFLKCLParas *ptLKCLSetPara)
{
    WORD16 wDigLoop = 0;
    WORD16 wIsStoped = 0;
    WORD32 dwRet = USP_SUCCESS;
    SWORD16 swI0 = 0;
    SWORD16 swQ0 = 0;
    WORD32  dwI2Q20 = 0;

    FpgaRegWrite(s_x0_reg, ptInintLKCLAdj->wInitX0);
    FpgaRegWrite(s_x1_reg, ptInintLKCLAdj->wInitX1);
    FpgaRegWrite(s_y0_reg, ptInintLKCLAdj->wInitY0);
    FpgaRegWrite(s_y1_reg, ptInintLKCLAdj->wInitY1);

    FpgaRegWrite(s_integral_max, ptInintLKCLAdj->wIntegralMax);
    FpgaRegWrite(s_integral_min, ptInintLKCLAdj->wIntegralMin);

    FpgaRegWrite(s_dac_change, ptInintLKCLAdj->wDacChange);

    FpgaRegWrite(s_threhold_val, ptInintLKCLAdj->wStopThd);

    FpgaRegWrite(s_cos_theta, ptInintLKCLAdj->wInitcos);
    FpgaRegWrite(s_sin_theta, ptInintLKCLAdj->wInitSin);

    FpgaRegWrite(s_rfcl_start, 0x0);
    usleep(100);
    FpgaRegWrite(s_rfcl_start, 0x1);


    while (wDigLoop < UHF_RF_MAX_LKCL_ADJ_COUNT)
    {
        //usleep(50000);
        //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_ADJ_RESOLUTION_TIME);
        wIsStoped = FpgaRegRead(I_stop);

        if (wIsStoped == 1)
        {
            swI0 = FpgaRegRead(I_ADC_idata);
            swQ0 = FpgaRegRead(I_ADC_qdata);
            dwI2Q20 = swI0 * swI0 + swQ0 * swQ0;
            uhfRfDevPrint("Fine LKCL Value %x\r\n", dwI2Q20);

            if (dwI2Q20 < ptInintLKCLAdj->dwAdjThrHold)
            {

                ptLKCLSetPara->wX0 = FpgaRegRead(I_i1_data);
                ptLKCLSetPara->wX1 = FpgaRegRead(I_i2_data);
                ptLKCLSetPara->wY0 = FpgaRegRead(I_q1_data);
                ptLKCLSetPara->wY1 = FpgaRegRead(I_q2_data);

                FpgaRegWrite(s_x0_reg, ptLKCLSetPara->wX0);
                FpgaRegWrite(s_x1_reg, ptLKCLSetPara->wX1);
                FpgaRegWrite(s_y0_reg, ptLKCLSetPara->wY0);
                FpgaRegWrite(s_y1_reg, ptLKCLSetPara->wY1);
                
                /*����ÿ��Ƶ���΢���Ƕ�*/
                ptLKCLSetPara->wInitCos = ptInintLKCLAdj->wInitcos;
                ptLKCLSetPara->wInitSin = ptInintLKCLAdj->wInitSin;

                uhfRfDevPrint("uhfRf:lkcl loop %d reslts %x %x %x %x\r\n",wDigLoop, ptLKCLSetPara->wX0,
                           ptLKCLSetPara->wX1, ptLKCLSetPara->wY0, ptLKCLSetPara->wY1);

                ptLKCLSetPara->wIsCrossed = FpgaRegRead(s_dac_change);
                ptLKCLSetPara->wIsConfiged = 1;

                FpgaRegWrite(s_rfcl_start, 0x2);
                break;
            }
            else
            {
                uhfRfDevPrint("uhfRf:eb is 1 but e5 e6 not right\r\n");
            }
        }

        wDigLoop++;
        uhfRfDevPrint("%d", wIsStoped);
    }

    if (wDigLoop >= UHF_RF_MAX_LKCL_ADJ_COUNT)
    {
        uhfRfDevPrint("uhfRf:LKCL Failed\r\n");
        FpgaRegWrite(s_rfcl_start, 0x2);
        ptLKCLSetPara->wIsConfiged = 0;

        dwRet = FPGA_RFADJ_CANNOT_STOP;
    }

    return dwRet;
}

/**************************************************************************
* �������ƣ�static WORD32 uhfRf_FPGALKCLProc(T_USPUHFLKCLInitParas *ptInintLKCLAdj,
                                  T_USPUHFLKCLParas *ptLKCLSetPara,
                                  WORD16 *pwTryindex)
* �������������һ��Ƶ�ʽ���һ��й¶��������
* ���������T_LKCLInitParas *ptInintLKCLAdj й¶������ʼ������
                                 T_LKCLParas *ptLKCLSetPara  й¶�����ɹ������˽ṹ��

* ���������
* �� �� ֵ��    �ɹ�����0   ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
**************************************************************************/
WORD32 uhfRf_FPGALKCLProc(T_USPUHFLKCLInitParas *ptInintLKCLAdj,
                                 T_USPUHFLKCLParas *ptLKCLSetPara,
                                 WORD16 *pwTryindex)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wLoopCount = 0;
    WORD16 wLoop = 0;

    /*wLoopCount ������Ҫ�����ĽǶȵĸ���,ͬʱҲ�ǽǶȵ�����*/
    wLoopCount = sizeof(g_InitPhase) / sizeof(T_InitPhase);

    for (wLoop = 0; wLoop < wLoopCount; wLoop++, (*pwTryindex)++)
    {
        if (*pwTryindex >= wLoopCount)
        {
            *pwTryindex = 0;
        }

        ptInintLKCLAdj->wInitcos = g_InitPhase[(*pwTryindex)].swCos;
        ptInintLKCLAdj->wInitSin = g_InitPhase[(*pwTryindex)].swSin;
        uhfRfDevPrint("uhfRf:Try cos %d  sin %d \r\n", ptInintLKCLAdj->wInitcos, ptInintLKCLAdj->wInitSin);
        dwRet = usp_uhf_rf_FPGALKCLProcTry(ptInintLKCLAdj, ptLKCLSetPara);

        if (dwRet != USP_SUCCESS)
        {
            continue;

        }
        else
        {
            break;
        }

    }

    if (wLoop >= wLoopCount)
    {
        dwRet = FPGA_RFADJ_CANNOT_STOP;
    }

    return dwRet;
}
/**************************************************************************
* �������ƣ�WORD32 uhfRf_FineLKCL(BYTE ucAntIndex, T_FreqHopCtrl *ptFreqHopCtrl)
* ��������������ʱ����й¶�����Ҫ����й¶����΢��
* ���������BYTE ucAntIndex ���ߺ�
                                T_FreqHopCtrl *ptFreqHopCtrl
                                �˱�����Ҫ��ȷ���Ƿ������Ƶ�����õĸı�
* ���������
* �� �� ֵ��    �ɹ�����0   ��������
* ����˵����
* �޸�����     �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 11/01/17	     V05.00.00	    zdx	      ����
* -----------------------------------------------
* 12/01/16	     V05.00.00	    �ڽ�      ����Ӳ��Ҫ�󣬽�6600C1/C2����й©�����ر�
**************************************************************************/

WORD32 uhfRf_FineLKCL(BYTE ucAntIndex, T_UhfRfFreqHopCtrl *ptFreqHopCtrl)
{
    T_USPUHFLKCLParas *ptLKCLSetPara = NULL;
    WORD32  dwRet = USP_SUCCESS;
    SWORD16 swI0 = 0;
    SWORD16 swQ0 = 0;
    WORD32  dwI2Q20 = 0;
    WORD32  dwI2Q21 = 0;

    ptLKCLSetPara =
        uhfRf_getLKCLParas(ucAntIndex, ptFreqHopCtrl->wFreqTableID,
                           ptFreqHopCtrl->wWorkChannel);

    uhfRfDevPrint("uhfRf:Ant %d WorkChannel %d\r\n", ucAntIndex,
                  ptFreqHopCtrl->wWorkChannel);

    /*����Ƶ��û�����û��ߺͶ���ʱʹ��HopTableID��һ��ʱ*/
    if ((NULL == ptLKCLSetPara)
        || (ptLKCLSetPara->wIsConfiged == 0))
    {
        FpgaRegWrite(s_x0_reg, 0x7fff);
        FpgaRegWrite(s_x1_reg, 0x7fff);
        FpgaRegWrite(s_y0_reg, 0x7fff);
        FpgaRegWrite(s_y1_reg, 0x7fff);

        FpgaRegWrite(s_rfcl_start, 0);
#if (!USP_IS_DISTRIBUTED)
       usp_uhf_rf_txSet(UHF_RF_START_TX);
       
       //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_WAIT_TIME);
       //usp_drv_timer_usleep(1520);
#endif
    }
    else
    {
        usp_uhf_rf_txSet(UHF_RF_START_TX);

    #if UHF_BOARD_TYPE != BOARD_TYPE_RFCB7
        /* 6600C1/C2���򿪿���й©���� */
        uhfRf_SetLKCLSwitch(UHF_RF_FAST_LKCL_ON);
    #endif
        FpgaRegWrite(s_x0_reg, ptLKCLSetPara->wX0);
        FpgaRegWrite(s_x1_reg, ptLKCLSetPara->wX1);
        FpgaRegWrite(s_y0_reg, ptLKCLSetPara->wY0);
        FpgaRegWrite(s_y1_reg, ptLKCLSetPara->wY1);
        uhfRfDevPrint("uhfRf:X0 0x%x  X1 0x%x Y0 0x%x Y1 0x%x\r\n", ptLKCLSetPara->wX0,
                      ptLKCLSetPara->wX1, ptLKCLSetPara->wY0, ptLKCLSetPara->wY1);

        FpgaRegWrite(s_rfcl_start, 0);
        /*RFCB2 400us    PURM RFCB  1.5ms*/
        //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_RX_OPEN_WAIT_TIME);
        
        swI0 = FpgaRegRead(I_ADC_idata);
        swQ0 = FpgaRegRead(I_ADC_qdata);
        dwI2Q20 = swI0 * swI0 + swQ0 * swQ0;

        uhfRfDevPrint("uhfRf:Fine LKCL Value i %d  q %d i2q2 %d\r\n", swI0, swQ0, dwI2Q20);

        if (dwI2Q20 > g_tInintLKCLAdj.dwAdjThrHold)
        {
            uhfRfDevPrint("uhfRf:Need to Fine LKCL Para Cnt %d\r\n",
                          ptLKCLSetPara->wCheckFineCnt);
            ptLKCLSetPara->wCheckFineCnt++;

            if (ptLKCLSetPara->wCheckFineCnt > g_tInintLKCLAdj.wFineThrHold)
            {
                uhfRfDevPrint("uhfRf:Fine LKCL \r\n");


                FpgaRegWrite(s_cos_theta, ptLKCLSetPara->wInitCos);
                FpgaRegWrite(s_sin_theta, ptLKCLSetPara->wInitSin);

                uhfRfDevPrint("uhfRf:Cos %d sin %d",ptLKCLSetPara->wInitCos,ptLKCLSetPara->wInitSin);
                FpgaRegWrite(s_rfcl_start, 1);
                //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_FINE_WAIT_TIME); /*1ms*/
                swI0 = FpgaRegRead(I_ADC_idata);
                swQ0 = FpgaRegRead(I_ADC_qdata);
                dwI2Q21 = swI0 * swI0 + swQ0 * swQ0;
                uhfRfDevPrint("uhfRf:Fine LKCL dwI2Q21 = %d\r\n", dwI2Q21);

                if (dwI2Q21 <= dwI2Q20)
                {
                    uhfRfDevPrint("uhfRf:Fine LKCL success\r\n");
                    ptLKCLSetPara->wX0 = FpgaRegRead(I_i1_data);
                    ptLKCLSetPara->wX1 = FpgaRegRead(I_i2_data);
                    ptLKCLSetPara->wY0 = FpgaRegRead(I_q1_data);
                    ptLKCLSetPara->wY1 = FpgaRegRead(I_q2_data);
                    ptLKCLSetPara->wCheckFineCnt = 0;
                }
                else
                {
                    uhfRfDevPrint("uhfRf:Fine LKCL Failed\r\n");
                    FpgaRegWrite(s_x0_reg, ptLKCLSetPara->wX0);
                    FpgaRegWrite(s_x1_reg, ptLKCLSetPara->wX1);
                    FpgaRegWrite(s_y0_reg, ptLKCLSetPara->wY0);
                    FpgaRegWrite(s_y1_reg, ptLKCLSetPara->wY1);

                    FpgaRegWrite(s_rfcl_start, 0);
                    //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_RX_OPEN_WAIT_TIME); /*300us*/
                }
            }
        }
        else
        {
            ptLKCLSetPara->wCheckFineCnt = 0;
            uhfRfDevPrint("uhfRf:No need to Fine LKCL Para\r\n");
        }
        uhfRf_SetLKCLSwitch(UHF_RF_FAST_LKCL_OFF);
#if USP_IS_DISTRIBUTED
        /*RFCB2��Ҫ�رյ������ͣ������ְ��ٿ����źŷ���*/
        usp_uhf_rf_txSet(UHF_RF_STOP_TX);
#endif
        
    }

    FpgaRegWrite(s_rfcl_start, 2);

    return  dwRet;
}


/**********************************************************************
* �������ƣ�void uhfRf_ReaderLKCLProc()
* �����������Ը������߽���й¶����
* �����������
* ���������
            �ɹ���0
            ʧ�ܣ�������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2009/12/1           V5.0.0           zdx            ����
************************************************************************/
#define LKCLPOWER 3000
void uhfRf_ReaderLKCLProc()
{
    WORD16 wAntIndex = 1;
    T_USPUHFFreqTableEntry *ptHopTable = NULL;
    WORD32 dwRet = USP_SUCCESS;

    for (wAntIndex = 1; wAntIndex <= READER_ANTENNA_COUNT; wAntIndex++)
    {
        dwRet = usp_uhf_rf_getAntStatus(wAntIndex);

        if (USP_SUCCESS == dwRet)
        {

            ptHopTable =
                uhfRf_GetHopTable(g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl.wFreqTableID);

            if (ptHopTable != NULL)
            {
                uhfRf_FreqLKCL(ptHopTable, wAntIndex);
            }

            ptHopTable =
                uhfRf_GetHopTable(g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl.wFreqTableID);

            if (ptHopTable != NULL)
            {
                uhfRf_FreqLKCL(ptHopTable, wAntIndex);
            }
            
            ptHopTable =
                uhfRf_GetHopTable(g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl.wFreqTableID);

            if (ptHopTable != NULL)
            {
                uhfRf_FreqLKCL(ptHopTable, wAntIndex);
            }
        }
        else
        {
            uhfRfDevPrint("uhfRf:No Need to LKCL Ant%d is absent!\r\n", wAntIndex);
        }
    }
}

#else
#include "Board.h"
#include "uhf_rf.h"
#include "RfCtl.h"

void uhfRf_ReaderLKCLProc()
{

}

WORD32 uhfRf_FineLKCL(BYTE ucAntIndex, T_UhfRfFreqHopCtrl *ptFreqHopCtrl)
{
    usp_uhf_rf_txSet(UHF_RF_START_TX);
    /*1.5ms ����Ƶ��ʱ����֤�ȶ��ͱ�ǩ����*/
    //USP_OSS_VOS_SLEEP_USEC(UHF_RF_LKCL_WAIT_TIME);
    //usp_drv_timer_usleep(1520);
    return USP_SUCCESS;
}
#endif


#endif
