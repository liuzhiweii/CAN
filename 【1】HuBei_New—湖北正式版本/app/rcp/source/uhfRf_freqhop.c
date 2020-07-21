/***************************************************************************
* ��Ȩ���� (C)2009, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ�uhfrf_freqhop.c
* �ļ���ʶ��
* ����ժҪ��6700Ƶ�ʿ��ƹ��ܵ�ʵ��
* ����˵����
* ��ǰ�汾��
* ��    �ߣ��Զ���
* ������ڣ�2012.3.20
*
* �޸ļ�¼1��
*    �޸����ڣ�2012.3.20
*    �� �� �ţ� RUP
*    �� �� �ˣ�
*    �޸����ݣ�����Ƶ�Ĺ���ȫ���ŵ����ļ��У�������
                                    ��ͬһ�������²�ͬЭ��ʱ��Ƶ��ֿ��Ĺ���
**********************************************************************/
#include "usp.h"
#include "common.h"
#if UHF_PROTOCOL_USE &&(UHF_BOARD_TYPE !=BOARD_TYPE_RBCB1)
#include "uhf_dbg.h"
#include "Board.h"
#include "RfCtl.h"
#include "uhf_rf.h"
//#include "uhf_time.h"



const T_USPUHFFreqTableEntry g_FreqTable800M[] = HOPTABLE800M;
const T_USPUHFFreqTableEntry g_FreqTable900M[] = HOPTABLE900M;
const T_USPUHFFreqTableEntry g_FreqTable800900M[] = HOPTABLE800900M;
static T_USPUHFFreqTableEntry *g_ptUHFFreqTable;     /*�豸����Ƶ��800M 900M�豸��ͬ*/

T_UhfRfAntFreqHopCtrl g_tFreqHopCtrl[READER_ANTENNA_COUNT + 1];

extern T_RfCtrl g_tRfCtrl;

static WORD32 SelectFreqPoint(WORD16 wAntIndex , T_UhfRfFreqHopCtrl *ptFreqHopCtrl);

/**************************************************************************
* �������ƣ�FreqHopInit
* ������������Ƶ���Ʊ�����ʼ��
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��  ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

void uhfRf_FreqHopInit(void)
{
    WORD32 dwChannelNoIndex = 0;
    WORD16 wAntHopIndex     = 0;      /*�������ߵ���Ƶ���ó�ʼ��*/
    WORD16 wIndex = 0;
	DEBUG_MSG("uhfRf_FreqHopInit start\n");
	fflush(stdout);

    if (g_tRfCtrl.wFreqType == PLL_TYPE_800M)   /*800M �豸֧��һ���й�800��׼����Ƶ��*/
    {
        g_ptUHFFreqTable = (T_USPUHFFreqTableEntry *) & g_FreqTable800M[0];
    }
    else if (PLL_TYPE_900M == g_tRfCtrl.wFreqType)
    {
        g_ptUHFFreqTable = (T_USPUHFFreqTableEntry *) & g_FreqTable900M[0];
    }
    else
    {
        /*800 900M��һ��ţ����*/
        g_ptUHFFreqTable = (T_USPUHFFreqTableEntry *) &g_FreqTable800900M[0];
    }

    /*�˴��ĳ�ʼ������������Э��Ĺ���ģʽ����
          Ϊ��Ƶ0��Ƶ�㣬0��Ƶ��id*/
    memset(&g_tFreqHopCtrl[0], 0x0,
           (READER_ANTENNA_COUNT + 1)*sizeof(T_UhfRfAntFreqHopCtrl));

    for(wIndex =0;wIndex <= READER_ANTENNA_COUNT;wIndex++)
    {
      /*��ʼ��Ϊ����Ƶ��*/
       g_tFreqHopCtrl[wIndex].tEpcFreqHopCtrl.wFixFreqChannel =10;
       g_tFreqHopCtrl[wIndex].tEpcFreqHopCtrl.wWorkChannel =10;
       g_tFreqHopCtrl[wIndex].tIso6BFreqHopCtrl.wFixFreqChannel =10;
       g_tFreqHopCtrl[wIndex].tIso6BFreqHopCtrl.wWorkChannel = 10;
       g_tFreqHopCtrl[wIndex].tGBFreqHopCtrl.wFixFreqChannel =10;
       g_tFreqHopCtrl[wIndex].tGBFreqHopCtrl.wWorkChannel = 10;
    }
    g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(10, 0);  /*��ʼ��Ϊ��Ƶ��0Ƶ����Ϊ0��Ƶ��*/
   
   DEBUG_MSG("uhfRf_FreqHopInit 1\n");
   fflush(stdout);

    /* �ϵ���һ������PLL */
    usp_uhf_rf_setPLLFreq(g_tRfCtrl.dwWorkFreq);


}

/**************************************************************************
* �������ƣ�uhfRf_checkLBTStatus
* �������������LBT��״̬��Ŀǰû�в���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��  ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/
WORD32 uhfRf_checkLBTStatus(void)
{
    WORD32 dwRetVal = LBT_STATUS_IDLE;

    return (dwRetVal);
}

/**********************************************************************
* �������ƣ�  WORD16 uhfRf_FreqHopCtrl(void)
* ����������  ͨ������PLL���໷�Ĳ�����ʹ�Ķ���ʵ����Ƶ��ʽ����?* ���������  ��
* ���������  ��
* �� �� ֵ��     0 ��ȷ ���� ����
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2010/12/5        V4.0.0   zdx     �Ż�����
***********************************************************************/
WORD16 uhfRf_FreqHopCtrl(WORD16 wAntIndex, WORD16 wProtoType)
{
    WORD32 dwIndex;
    WORD32 dwRetVal;
    WORD32 wMsHopInterval = 0;
    struct timeval tCurrentTime;
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtoType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;

    }
    else if (UHF_RF_PROTOCOL_GB == wProtoType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
    }

    if (ptMemFreqHopCtrl->wFreqWorkType == FIXED_FREQ)
    {
        /* ����Ҫ������PLL */
        ptMemFreqHopCtrl->wWorkChannel = ptMemFreqHopCtrl->wFixFreqChannel;
        g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(
                                   ptMemFreqHopCtrl->wWorkChannel,
                                   ptMemFreqHopCtrl->wFreqTableID);

    }
    else
    {
#ifdef SUPPORT_HOP_FREQ
        GetCurrUsTime(&tCurrentTime);
        wMsHopInterval = CalMsTimeInterval(&ptMemFreqHopCtrl->tHopClock, &tCurrentTime);
        uhfRfDevPrint("Hoptimer scope is %d the timeinterval in ms is %d\r\n",
                      ptMemFreqHopCtrl->wHopTimer * 10, wMsHopInterval);

        if (wMsHopInterval > (ptMemFreqHopCtrl->wHopTimer) * 10)   /*��̨�·�����Ƶ�����10MSΪ��λ*/
        {
            GetCurrUsTime(&ptMemFreqHopCtrl->tHopClock);
            uhfRfDevPrint("Freq hop happens\r\n");
            /* ��ȡ��ǰ���ߵ�LBT���� */
            ptMemFreqHopCtrl->dwLbtEnable
            = g_tRfCtrl.atRfAntCfg[wAntIndex].ucAntennaLBT;

            if (0 == ptMemFreqHopCtrl->dwLbtEnable)
            {
                /* LBT���ܹر� */
                /*  ѡ��һ��Ƶ�� */
                SelectFreqPoint(wAntIndex, ptMemFreqHopCtrl);
            }
            else
            {
                /* LBT���ܴ� */
                for (dwIndex = 0; dwIndex < ptMemFreqHopCtrl->dwMaxLbtCheckTimes; dwIndex++)
                {
                    /*  ѡ��һ��Ƶ�� */
                    SelectFreqPoint(wAntIndex, ptMemFreqHopCtrl);

                    /* ����Ƶ���״̬ */
                    dwRetVal = uhfRf_checkLBTStatus();

                    if (LBT_STATUS_IDLE == dwRetVal)
                    {
                        /* Ƶ�����,���� */
                        break;
                    }
                }
            }
        }             /*�ﵽ����Ƶ�����������Ƶ*/
        else
        {
            g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(
                                       ptMemFreqHopCtrl->wWorkChannel,
                                       ptMemFreqHopCtrl->wFreqTableID);
            uhfRfDevPrint("No need to hop \r\n");
        } /*û�дﵽ��Ƶʱ�̣�ֱ�ӷ���*/

#endif
    }

    return USP_SUCCESS;

} /* WORD16 uhfRf_FreqHopCtrl(void) */

/**********************************************************************
* �������ƣ�T_USPUHFRfFreqCfg
* ���������� ȷ����ǰ����Ƶ�����Ƿ�ı�
* ��������� ptFreqCfg Ҫ���õ���Ƶ����
                                  wAntIndex  ���ߺ�
                                  wProtocolType Э������
* ��������� ��
* �� �� ֵ��      0 ��ͬ 1�ı�
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010\11\23               zdx                          ����
* 2012\3\20                zdx         ���ݵ����߶�Э��Ƶ�ʿ����޸�
***********************************************************************/
WORD16 IsFreqCfgChanged(T_USPUHFRfFreqCfg *ptFreqCfg, WORD16 wAntIndex,
                        WORD16 wProtocolType)
{
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;

    }
    else if (UHF_RF_PROTOCOL_GB == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
    }

    return ((ptMemFreqHopCtrl->wFreqTableID != ptFreqCfg->wFreqTableID)
            || (ptMemFreqHopCtrl->wFreqWorkType != ptFreqCfg->wFreqWorkType)
            || ((ptMemFreqHopCtrl->wFreqWorkType == FIXED_FREQ) && (ptMemFreqHopCtrl->wFixFreqChannel != ptFreqCfg->wFixFreqChannel))
#ifdef SUPPORT_HOP_FREQ
            || ((ptMemFreqHopCtrl->wFreqWorkType == FREQ_HOP) && ((ptMemFreqHopCtrl->wHopChannelNum != ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannelNum)
                    || (ptMemFreqHopCtrl->wFreqHopMode != ptFreqCfg->tFreqHopConfig.wFreqHopMode)
                    || (ptMemFreqHopCtrl->wHopTimer != ptFreqCfg->tFreqHopConfig.wHopTimer)
                    || memcmp(&(ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannel[0]), &(ptMemFreqHopCtrl->wHopChannelNo[0]), (ptMemFreqHopCtrl->wHopChannelNum * 2))))
#endif
           );

}

/**********************************************************************
* �������ƣ� uhfRf_SetFreqCfg
* ���������� ����Ƶ����ز���������������ʹ��
* ��������� ptFreqCfg Ҫ���õ���Ƶ����
                                  wAntIndex  ���ߺ�
                                  wProtocolType Э������
* ��������� Ƶ��ֵ
* �� �� ֵ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010\11\23               zdx                          ����
* 2012\3\20               zdx            ���ݵ����߶�Э����Ƶ���������޸�
***********************************************************************/
void uhfRf_SetFreqCfg(T_USPUHFRfFreqCfg *ptFreqCfg, WORD16 wAntIndex,
                      WORD16 wProtocolType)
{
    WORD32  dwChannelNoIndex = 0;
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;
        uhfRfDevPrint("uhfRf:EPC Freq Changed \r\n");
    }
    else if (UHF_RF_PROTOCOL_GB == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
        uhfRfDevPrint("uhfRf:GB Freq Changed \r\n");
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
        uhfRfDevPrint("uhfRf:ISO6B Freq Changed \r\n");
    }

    uhfRfDevPrint("uhfRf:EPC Freq Changed \r\n");
    ptMemFreqHopCtrl->wFreqWorkType = ptFreqCfg->wFreqWorkType; //��Ƶ��Ƶ����
    ptMemFreqHopCtrl->wFreqTableID = ptFreqCfg->wFreqTableID;
    uhfRfDevPrint("uhfRf:FreqWorkType = %d  HopTableID= %d\r\n",
                  ptMemFreqHopCtrl->wFreqWorkType, ptMemFreqHopCtrl->wFreqTableID);

    if (ptMemFreqHopCtrl->wFreqWorkType == FIXED_FREQ)
    {
        ptMemFreqHopCtrl->wFixFreqChannel = ptFreqCfg->wFixFreqChannel;
        ptMemFreqHopCtrl->wWorkChannel = ptFreqCfg->wFixFreqChannel;
        uhfRfDevPrint("uhfrf:fixed Freq chanel %d\r\n", ptMemFreqHopCtrl->wWorkChannel);
        g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(ptMemFreqHopCtrl->wWorkChannel,
                               ptMemFreqHopCtrl->wFreqTableID);
    }
    else
    {
#ifdef SUPPORT_HOP_FREQ
        ptMemFreqHopCtrl->wHopTimer = ptFreqCfg->tFreqHopConfig.wHopTimer;
        ptMemFreqHopCtrl->wHopChannelNum =
            ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannelNum;
        //�Զ�����Ƶ����

        uhfRfDevPrint("uhfRf:Freq  channelNum %d  ", ptMemFreqHopCtrl->wHopChannelNum);

        ptMemFreqHopCtrl->wFreqHopMode = ptFreqCfg->tFreqHopConfig.wFreqHopMode;
        //��Ƶ��ʽ ����� ˳����

        /*�Զ�����Ƶ���*/
        for (dwChannelNoIndex = 0;
             dwChannelNoIndex < ptMemFreqHopCtrl->wHopChannelNum;
             dwChannelNoIndex++)
        {
            ptMemFreqHopCtrl->wHopChannelNo[dwChannelNoIndex] =
                ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannel[dwChannelNoIndex];
            uhfRfDevPrint("%d ", ptMemFreqHopCtrl->wHopChannelNo[dwChannelNoIndex]);
        }

        uhfRfDevPrint("\r\n");

        if ((ptMemFreqHopCtrl->wFreqHopMode == FREQ_HOP_SCAN_STEP_CHANNEL)
            || (ptMemFreqHopCtrl->wFreqHopMode == FREQ_HOP_SCAN_RANDOM_CHANNEL))
        {
            /* ����������㷨��ʼ�����*/
            srand((int)(time(0)));
            ptMemFreqHopCtrl->wHopChannel = (rand() % ptMemFreqHopCtrl->wHopChannelNum);
        }
        else
        {
            ptMemFreqHopCtrl->wHopChannel = 0;
        }

        ptMemFreqHopCtrl->wWorkChannel =
            ptMemFreqHopCtrl->wHopChannelNo[ptMemFreqHopCtrl->wHopChannel];

        GetCurrUsTime(&ptMemFreqHopCtrl->tHopClock);
        g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(ptMemFreqHopCtrl->wWorkChannel,
                               ptMemFreqHopCtrl->wFreqTableID);
#endif
    }
}
/**************************************************************************
* �������ƣ�CheckFrqPara
* �������������Ӻ�̨���������Ƿ���ȷ
* ���ʵı���
* �޸ĵı���
* ���������T_USPUHFRfFreqCfg *ptFreqCfg ��Ƶ����
* �����������
* �� �� ֵ��  USP_SUCCESS ��ȷ
                               USP_UHF_RCVED_PARA_ERROR  �쳣
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
**************************************************************************/

WORD32 uhfRf_CheckFrqPara(T_USPUHFRfFreqCfg *ptFreqCfg)
{
    if ((ptFreqCfg->wFreqTableID > g_tRfCtrl.wMaxFreqTable) ||
        ((ptFreqCfg->wFixFreqChannel) > (uhfRf_GetHopTable(ptFreqCfg->wFreqTableID)->wFrequeceNum))
#ifdef SUPPORT_HOP_FREQ
        || ((ptFreqCfg->wFreqWorkType == 1)
            && (ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannelNum > MAX_FREQ_HOP_NUM))
#endif
       )
    {
        uhfRfDevPrint("uhfRf:Rcved invalid Freq Cfg Para FreqTableID %d channel %d num %d\r\n",
                      ptFreqCfg->wFreqTableID, ptFreqCfg->wFixFreqChannel,
                      ptFreqCfg->tFreqHopConfig.tfreqChannel.wFreqChannelNum);
        return USP_UHF_RCVED_PARA_ERROR;
    }

    return USP_SUCCESS;

}

/**********************************************************************
* �������ƣ� uhfRf_GetHopTable
* ��������������Ƶ�ʱ��ÿ��ƽṹ
* ��������� ���ߺ�
* ���������
* �� �� ֵ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010\11\23               zdx                          ����
***********************************************************************/

T_USPUHFFreqTableEntry *uhfRf_GetHopTable(WORD16 wHopTableId)
{

    return &g_ptUHFFreqTable[wHopTableId];
}



/**************************************************************************
* �������ƣ�uhfRf_ConvChannelToFreq
* ������������Ƶ����ת��Ϊ��Ӧ��Ƶ��
* ���ʵı���
* �޸ĵı���
* ���������WORD16 wChannelNo   �����õ�Ƶ����
                                 WORD16 wHopTableID  ��ʹ�õ�Ƶ����
* �����������
* �� �� ֵ��  ��ȡ������Ƶ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 11/11/23    RUP      �Զ���        ����
* 12/3/20      RUP     �Զ���         ����ֵ���ػ�õ�Ƶ��
**************************************************************************/

WORD32 uhfRf_ConvChannelToFreq(WORD16 wChannelNo, WORD16 wHopTableID)
{
    /* Ƶ�����쳣Ƶ���趨Ϊ0��Ƶ��*/
    if ((wChannelNo > g_ptUHFFreqTable[wHopTableID].wFrequeceNum - 1))
    {
        UHFErrorPrint("uhfrf: ChannelNo is %d abnormal ! \r\n", wChannelNo);
        //LAPInsert(FILE_UHF_RF, USP_UHF_CHANNEL_NO_ERROR, wChannelNo, 0);
        return g_ptUHFFreqTable[wHopTableID].dwFrequeces[0];
    }

    return g_ptUHFFreqTable[wHopTableID].dwFrequeces[wChannelNo];

}

/**********************************************************************
* �������ƣ�SelectFreqPoint
* ��������������Ƶ�ʵ�����ѡ��Ƶ��
* ���������wAntIndex ���ߺ�
                                 ptFreqHopCtrl ��ǰ��Ƶ�Ŀ��ƽṹ
* �������������ȫ�ֱ�����workfreq
* �� �� ֵ��    0 �ɹ� ���� ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
* 12/3/20      RUP     �Զ���         ���ݵ����߶�Ƶ�������޸�
************************************************************************/
static WORD32 SelectFreqPoint(WORD16 wAntIndex , T_UhfRfFreqHopCtrl *ptFreqHopCtrl)
{

    if (NULL == ptFreqHopCtrl)
    {
        return  USP_UHF_POINTER_NULL_ERROR;

    }

    ptFreqHopCtrl->wWorkChannel =
        ptFreqHopCtrl->wHopChannelNo[ptFreqHopCtrl->wHopChannel];
    /* ��¼��ǰƵ����*/
    uhfRfDevPrint("HopChannel %d WorkChannel %d\r\n",
                  ptFreqHopCtrl->wHopChannel, ptFreqHopCtrl->wWorkChannel);


    g_tRfCtrl.dwWorkFreq = uhfRf_ConvChannelToFreq(ptFreqHopCtrl->wWorkChannel,
                           ptFreqHopCtrl->wFreqTableID);

    if (ptFreqHopCtrl->wFreqHopMode == FREQ_HOP_SCAN_CHANNEL)
    {
        //��Ƶ��ɨ��
        ptFreqHopCtrl->wHopChannel++;

        if (ptFreqHopCtrl->wHopChannel >= ptFreqHopCtrl->wHopChannelNum)
        {
            ptFreqHopCtrl->wHopChannel = 0;
        }
    }
    else if (ptFreqHopCtrl->wFreqHopMode == FREQ_HOP_SCAN_RANDOM_CHANNEL)
    {
        //Ƶ�����������
        ptFreqHopCtrl->wHopChannel = (rand() % ptFreqHopCtrl->wHopChannelNum);

    }
    else
    {
        /* ����ѭ��������Ƶ*/
        /* Ni+1=(Ni+INT((Nl-Nf)/2)) MOD ��Nl-Nf+1)��
                 ���г�ʼNiΪNf��Nl֮�������������Ni+1=0,��Ni+1=Nl*/
        ptFreqHopCtrl->wHopChannel =
            ((ptFreqHopCtrl->wHopChannel + 1)\
             + (WORD16)((ptFreqHopCtrl->wHopChannelNum - 1) / 2))\
            % (ptFreqHopCtrl->wHopChannelNum);

        if (ptFreqHopCtrl->wHopChannel == 0)
        {
            ptFreqHopCtrl->wHopChannel = ptFreqHopCtrl->wHopChannelNum;
        }

        /* �Ļص���ʵƵ��Ƶ��������� */
        ptFreqHopCtrl->wHopChannel--;
    }

    return USP_SUCCESS;

}

/**********************************************************************
* �������ƣ�uhfRf_getFreqTableId
* ������������õ�ǰ��Ƶ�ʱ�����
* ���������wAntIndex ���ߺ�
                                 wProtocolType Э������
* �����������
* �� �� ֵ��    Ƶ�ʱ�ID
* ����˵����Ϊ��ȡƵ���Լ�й¶����ʱ׼��
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 12/3/20      RUP     �Զ���         ����
************************************************************************/

WORD16 uhfRf_getFreqTableId(WORD16 wAntIndex, WORD16 wProtocolType)
{
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;
    }
    else if (UHF_RF_PROTOCOL_GB == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
    }

    return  ptMemFreqHopCtrl->wFreqTableID;
}


/**********************************************************************
* �������ƣ�uhfRf_getFreqWorkChannel
* ������������õ�ǰ�Ĺ���Ƶ����
* ���������wAntIndex ���ߺ�
                                 wProtocolType Э������
* �����������
* �� �� ֵ��    Ƶ�ʱ�ID
* ����˵����Ϊ��ȡƵ���Լ�й¶����ʱ׼��
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 12/3/20      RUP     �Զ���         ����
************************************************************************/

WORD16 uhfRf_getFreqWorkChannel(WORD16 wAntIndex, WORD16 wProtocolType)
{
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;

    }
    else if (UHF_RF_PROTOCOL_GB == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
    }

    return  ptMemFreqHopCtrl->wWorkChannel;
}


/**********************************************************************
* �������ƣ�uhfRf_getFreqHopCtrl
* ������������õ�ǰ�Ĺ���Ƶ����
* ���������wAntIndex ���ߺ�
                                 wProtocolType Э������
* �����������
* �� �� ֵ��    Ƶ�ʱ�ID
* ����˵����Ϊ��ȡƵ���Լ�й¶����ʱ׼��
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 12/3/20      RUP     �Զ���         ����
************************************************************************/

T_UhfRfFreqHopCtrl *uhfRf_getFreqHopCtrl(WORD16 wAntIndex, WORD16 wProtocolType)
{
    T_UhfRfFreqHopCtrl *ptMemFreqHopCtrl = NULL;

    if (UHF_RF_PROTOCOL_EPC == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tEpcFreqHopCtrl;

    }
    else if (UHF_RF_PROTOCOL_GB == wProtocolType)
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tGBFreqHopCtrl;
    }
    else
    {
        ptMemFreqHopCtrl = &g_tFreqHopCtrl[wAntIndex].tIso6BFreqHopCtrl;
    }

    return  ptMemFreqHopCtrl;
}

#endif
