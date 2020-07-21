#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_gb.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "uhf_gb.h"
#include "GB.h"

/* RTDataRate   TRDataRate  RealTRDataRate   wTc   wTpri,   dwK   M   T1min  T2min  T4min*/
/// FIX ME !!
#define   GB_TIMES_PARA_TABLE {\
        {40, 320,320, 0, 3,4,GB_QUERY_M_1,  0, 0,  0},\
        {40,  80, 80,25,12,1,GB_QUERY_M_1,118,38,150},\
        {80,  80, 80,12,12,1,GB_QUERY_M_1,118,38, 75},\
        {80, 160,160,12, 6,1,GB_QUERY_M_1, 56,19, 75},\
        {80, 320,320,13, 3,3,GB_QUERY_M_1, 32, 1, 75},\
        {160,160,160, 6, 6,3,GB_QUERY_M_1, 54,19, 38},\
        {160,320,320, 6, 3,3,GB_QUERY_M_1, 26,10, 38},\
        {80,640,640, 6, 3,7,GB_QUERY_M_1, 13, 5, 38},\
        {105,640,640,10, 3,7,GB_QUERY_M_1, 24, 0, 58}\
}
extern T_GBCtl   g_tGBCtrl; 
extern T_UhfCtrl  g_tUhfCtrl;

const T_USPUHFGBRFModeTableEntry g_tGBRFModeTable[] = GB_RFMODE_TABLE;
const T_GB_TimesPara g_GBTimesParaTable[] = GB_TIMES_PARA_TABLE;

WORD32 uhf_GB_getModeParas(WORD16 wModeIndex, WORD16 wAntIndex);
//WORD32 uhf_GB_getLockPayLoad(T_USPUHFGBLockPayloadList *ptLockPayLoad,
//                              WORD32 *pdwLockPayLoad);

/**********************************************************************
* �������ƣ� WORD16 uhf_GB_getModeParas(WORD16 ModeIndex)
* ���������� ���ݸ�����ModeIndex��ֵȷ������,���Ʒ�ʽ��������뷽ʽ��PIEֵ��Ŀǰ�����䣩
* ���ʵı�
* �޸ĵı�
* ���������
* ���������
* �� �� ֵ��
* ����˵����  һ�´�LLRP ��׼�Ķ������32λתΪ16 λ�ٿ���
* �޸�����2014.2        �汾��     �޸���      lxc    �޸�����
* -----------------------------------------------
*
***********************************************************************/
WORD32 uhf_GB_getModeParas(WORD16 wModeIndex, WORD16 wAntIndex)
{
    WORD32 dwIndex = 0;
    WORD16 wModulateIndex = 0;
    WORD16 ModulateTable[] = {3, 2, 1}; /* ����ģʽת�� */
    static const WORD16 wGBModeTableLen = sizeof(g_tGBRFModeTable) / sizeof(T_USPUHFGBRFModeTableEntry);

    /* ������⣬����ֵ�ķ�Χ��0-20*/
    uhfDevPrint("GB modeindex == %d AntennaIndex %d wGBModeTableLen = %d\r\n",
                wModeIndex, wAntIndex, wGBModeTableLen);

    if (wModeIndex == 0)
    {
        uhfDevPrint("GB eForwardLinkModulation:%d,dwFawardValue:%d,dwBDRValue:%d\r\n", g_tGBRFModeTable[wModeIndex].eForwardLinkModulation,g_tGBRFModeTable[wModeIndex].dwFawardValue,
                    g_tGBRFModeTable[wModeIndex].dwBDRValue);

        if (wModeIndex == g_tGBCtrl.wModeIndex) /* Ĭ����0xffff */
        {
            uhfDevPrint(" wwwww  wModeIndex == g_tGBCtrl.wModeIndex\r\n");
            return USP_SUCCESS;
        }
        else
        {
            if (wModeIndex >= wGBModeTableLen)
            {
                wModeIndex = 0;
            }

            g_tGBCtrl.wModeIndex = wModeIndex;
        }

        
        wModulateIndex = (WORD16)g_tGBRFModeTable[wModeIndex].eForwardLinkModulation;
        g_tGBCtrl.RTModulateType = ModulateTable[wModulateIndex]; /* DSB */
        g_tGBCtrl.RTDataRate = (WORD16)(g_tGBRFModeTable[wModeIndex].dwFawardValue/1000);
        g_tGBCtrl.TRDataRate = (WORD16)(g_tGBRFModeTable[wModeIndex].dwBDRValue/1000);
        g_tGBCtrl.Query.ucM = (WORD16)g_tGBRFModeTable[wModeIndex].eMValue;    
	 uhfDevPrint("GB wModeIndex:%d,dwFawardValue%d dwBDRValue: %d \r\n",wModeIndex,
                    g_tGBRFModeTable[wModeIndex].dwFawardValue, g_tGBRFModeTable[wModeIndex].dwBDRValue);
        uhfDevPrint("GB the match rate is %d  and %d RTModulateType %d  DataEncodeType %d\r\n",wModeIndex,
                    g_tGBCtrl.RTDataRate, g_tGBCtrl.TRDataRate, g_tGBCtrl.RTModulateType,g_tGBCtrl.Query.ucM);
    }
    for (dwIndex = 0; dwIndex < sizeof(g_GBTimesParaTable) / sizeof(g_GBTimesParaTable[0]); dwIndex++)
    {
        if ((g_GBTimesParaTable[dwIndex].RTDataRate == g_tGBCtrl.RTDataRate)
            && (g_GBTimesParaTable[dwIndex].TRDataRate == g_tGBCtrl.TRDataRate))
        {
            break;
        }
    }

    if (dwIndex < sizeof(g_GBTimesParaTable) / sizeof(g_GBTimesParaTable[0]))
    {
        uhfDevPrint("UHF: wwwww user set rf link data rate is found \r\n");
    }
    else
    {
        /* �����ҵ�ƥ����������ã�ʹ��Ĭ�ϵ��������� */
        uhfDevPrint("UHF: user set rf link data rate is invid ,use default config \r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }

    uhfDevPrint("find the GB time para is %d\r\n", dwIndex);

    /* ������·�������þ���ʱ����� */
    g_tGBCtrl.RealTRDataRate = g_GBTimesParaTable[dwIndex].RealTRDataRate;

    g_tGBCtrl.wTc = g_GBTimesParaTable[dwIndex].wTc;

    /// ���DR����ʲô? ��GB���Ǹ���������Divide  Ratio, �뷴����·������������
    //g_tGBCtrl.Query.DR = g_GBTimesParaTable[dwIndex].DR;
    
    g_tGBCtrl.Query.ucReLinkRateDivisor = g_GBTimesParaTable[dwIndex].dwK;  //lxc ������������
    g_tGBCtrl.wTpri = g_GBTimesParaTable[dwIndex].wTpri;
    g_tGBCtrl.T1min = g_GBTimesParaTable[dwIndex].T1min;
    g_tGBCtrl.T2min = g_GBTimesParaTable[dwIndex].T2min;
    g_tGBCtrl.T4min = g_GBTimesParaTable[dwIndex].T4min;

    return (0);

}

/**********************************************************************
* �������ƣ�WORD32  SetGBInventoryPara (T_USPUHFGBInventoryPara *ptGBInventoryPara,
                                                                             WORD16 wAntInde, 
                                                                             WORD16 *pwFpgaInventoryCommand)
* ����������GB������̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
#define SELECT_ACTION_LEN (6)
WORD32 SetGBInventoryPara(T_USPUHFGBInventoryPara *ptGBInventoryPara,
                           WORD16 wAntInde, WORD16 *pwFpgaInventoryCommand)
{
    // ��ʲô�õ�lxc ,������Ҫ����fpga ���¶���
    //FIX ME!!
    //WORD16 actionTable[] = {4, 5, 6, 1, 0, 2}; /*���ڽ�LLRP ��׼��ACTION ��ӦΪEPC��׼��ACTION */
    WORD32 dwRet = USP_SUCCESS;
    T_USPUHFGBInventoryMaskList *pGBMaskList = NULL;
    WORD16 wAction = 0;

    /* ������Ƶģʽ������ʱ����� */ /* ���ʱ��ֵ�ǲ����б任 lxc */
    /* wModeindex �ǲ��Ƕ���10000  lxc*/
    dwRet = uhf_GB_getModeParas(ptGBInventoryPara->wModeindex, wAntInde);

    if (USP_SUCCESS != dwRet)
    {
        return dwRet;
    }

    //���÷������ʣ�����ǰ�����ر��˲� Timing T1,T2,T3 need to be fixed with Yong
    uhf_GB_setTimePara();  

    /* MACЭ���Ĵ��� */
    /* Maybe change */
    /* 0004h  10 ��ʾ�й���׼#define  UHF_PROTO_TYPE_GB   0x2 -- lxc*/
    //SetProtoType(UHF_PROTO_TYPE_GB);

    pGBMaskList = &ptGBInventoryPara->tGBInventoryMaskList;

    if (pGBMaskList->tGBInventoryMask[0].wMaskLength
        > GB_MAX_MASK_WORD_NUM * 16)
    {
        uhfDevPrint("MaskWordLen Error!\r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }

    //��������
    uhf_GB_setSelectMask(pGBMaskList->tGBInventoryMask[0].wMaskBitPointer,
                                      pGBMaskList->tGBInventoryMask[0].wMaskLength,
                    (WORD16 *)pGBMaskList->tGBInventoryMask[0].ucMaskData);

    wAction = pGBMaskList->tGBInventoryMask[0].ucSortAction;  
    
    uhfDevPrint("Action number ucSortAction = %d\r\n",wAction);
    if (wAction > SELECT_ACTION_LEN)
    {
        uhfDevPrint("Rcv invalied action number\r\n");
        wAction = 0;
    }

    /* дFPGA��sort ��������Ĵ��� */
    /// Bit15~Bit12  ����	Bit11~Bit10   ����	Bit9~Bit6   Ŀ��	Bit5~Bit0  �洢��lxc
    // �¹�����4��action����ô��LLRP�е�action��Ӧ?
    // FIX ME !! Action Para
    #if 0
    uhf_GBSetSelectPara(wAction/*Action (rule)*/, 
                                       0x0,/*target*/
                                       pGBMaskList->tGBInventoryMask[0].wMemBank/*membank*/);
    *pwFpgaInventoryCommand |= CMD_REG_GB_SORT;
    #endif

    /* ��дuhf_GB_Query������� g_tGBCtrl.Query.DR
       g_tGBCtrl.Query.M  ��uhf_GB_getModeParas����������*/

    //   ���� Bit15��Bit14
   // ����    Bit13��Bit10
   // �Ự   Bit9��Bit8
   // Ŀ��     Bit7
   //   TRext  Bit6
  //  ������·��������    Bit5��Bit2 
   // ����ѡ��Bit1��Bit0      lxc
   // Ԥ����ǩ������ȥ����

    /* ���������Ƿ���Ҫ����? ����һ����Σ���Ϊquery���������� */
    uhf_GBSetMacQueryPara(g_tGBCtrl.Query.ucM/*M*/, g_tGBCtrl.Query.ucReLinkRateDivisor/*DR*/);

    *pwFpgaInventoryCommand |= (CMD_REG_GB_QUERY);
    return dwRet;

}


/**********************************************************************
* �������ƣ�WORD32  SetGBReadPara(T_USPUHFGBReadPara *ptGBReadPara,
                                                                     WORD16 *pwGBReadCommand)
* ����������GB���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24      V0.0.1         lxc         ����
************************************************************************/
void SetGBReadPara(T_USPUHFGBReadPara *ptGBReadPara,
                    WORD16 *pwGBReadCommand)
{

    /* �����̣����ȷ����������ȡ���� (fpga ����)
                               ����access  ����ȷ�0-15bit���ٷ�16-31bit  (�ߵ�λͬʱд��)
                               ���Ͷ�����
    */
    
    /* �����ͻ�ȡ��������������fpga ����?  fpga ����
        �����ǿ����16λ��������Ľ����˭�������Ĳ���fpga��
        �����Ƿ���Ҫ�·�����access���fpga��
        �Ƿ���Ҫ���ÿ������ ?   ͬʱд�����εĿ������
        lxc
    */

    
    BYTE       ucAccessType = ACCESS_READ;  //�������

    /* ��ȡread ����ķ������� */
    if (ptGBReadPara->dwAccessPassword != 0)
    {
        uhf_GBSetMacAccessPara(ptGBReadPara->dwAccessPassword, ptGBReadPara->ucMB,ucAccessType);
        *pwGBReadCommand |= CMD_REG_GB_ACCESS;
    }

    /* ��дGB_Read������� lxc*/
    uhf_GBSetMacReadPara(ptGBReadPara->ucMB, ptGBReadPara->wWordPointer,
                          ptGBReadPara->wWordCount);
    *pwGBReadCommand |= CMD_REG_GB_READ;
}

/**********************************************************************
* �������ƣ�WORD32  SetGBReadPara(T_USPUHFGBWritePara *ptGBReadCommand,
                                                                      WORD16 *pwFpgaGBReadCommand)
* ����������GB���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24      V0.0.1         lxc         ����
************************************************************************/

void SetGBWritePara(T_USPUHFGBWritePara *ptGBWritePara,
                     WORD16 *pwGBWriteCommand)
{
    g_tGBCtrl.tGBWrite.wWriteWordsLen = ptGBWritePara->wWriteWordLen;

    BYTE ucAccessType = ACCESS_WRITE;

    if (ptGBWritePara->dwAccessPassword != 0)
    {
        uhf_GBSetMacAccessPara(ptGBWritePara->dwAccessPassword, ptGBWritePara->ucMB, ucAccessType);
        *pwGBWriteCommand |= CMD_REG_GB_ACCESS;
    }

    uhf_GBSetMacWritePara(ptGBWritePara->ucMB, ptGBWritePara->wWordPointer,
                               ptGBWritePara->wWriteWordLen, ptGBWritePara->aucData);
    *pwGBWriteCommand |= CMD_REG_GB_WRITE;
}

/**********************************************************************
* �������ƣ�WORD32 SetGBLockPara(T_USPUHFGBLockPara *ptGBLockPara,
                                                                   WORD16 *pwFpgaGBLockCommand)
* ����������GB����������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24      V0.0.1         lxc         ����
************************************************************************/

WORD32 SetGBLockPara(T_USPUHFGBLockPara *ptGBLockPara,
                      WORD16 *pwFpgaGBLockCommand)
{
    WORD32 dwLockPayLoad = 0;
    WORD32 dwRet = USP_SUCCESS;
    BYTE       ucAccessType = ACCESS_LOCK;

    if (ptGBLockPara->dwAccessPassword != 0)
    {
        uhf_GBSetMacAccessPara(ptGBLockPara->dwAccessPassword, (BYTE)ptGBLockPara->ucMB, ucAccessType);
        *pwFpgaGBLockCommand |= CMD_REG_GB_ACCESS;
    }
#if 0
    /* ��дGB_Lock������� */
    dwRet = uhf_epc_getLockPayLoad(&ptEPCLockPara->tEpcLockPayLoad, &dwLockPayLoad);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }
#endif

    uhf_GBSetMacLockPara(ptGBLockPara->ucMB, ptGBLockPara->ucConfig, 
                                      ptGBLockPara->ucAeraMode,ptGBLockPara->ucSecureMode);
    *pwFpgaGBLockCommand |= (CMD_REG_GB_LOCK);

    return dwRet;
}

/**********************************************************************
* �������ƣ� void SetGBErasePara(T_USPUHFGBErasePara *ptGBErasePara,
                                                              WORD16 *pwGBEraseCommand)
* ���������� ������������
* ���ʵı�
* �޸ĵı�
* ���������
* ���������
* �� �� ֵ��
* ����˵����
* �޸�����     2014.2      lxc   
* -----------------------------------------------
*
***********************************************************************/
void SetGBErasePara(T_USPUHFGBErasePara *ptGBErasePara,
                     WORD16 *pwGBEraseCommand)
{
    g_tGBCtrl.tGBErase.wEraseWordsLen = ptGBErasePara->wEraseWordLen;
    // ��������Ҫ��������
    //BYTE ucAccessType = ACCESS_WRITE;

    //if (ptGBErasePara->dwAccessPassword != 0)
    //{
    //    uhf_EpcSetMacAccessPara(ptGBErasePara->dwAccessPassword, ptGBErasePara->ucMB, ucAccessType);
    //    *pwGBEraseCommand |= CMD_REG_EPC_ACCESS;
    //}

    uhf_GBSetMacErasePara(ptGBErasePara->ucMB, ptGBErasePara->wWordPointer,
                               ptGBErasePara->wEraseWordLen);
    *pwGBEraseCommand |= CMD_REG_GB_WRITE;
}

/**********************************************************************
* �������ƣ�void SetGBKillPara(T_USPUHFGBKillPara *ptGBKillPara, 
                                                         WORD16 *pwMacGBKillCommand)
* ����������GB����ɱ������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24      V0.0.1         lxc         ����
************************************************************************/
/* δ��� */
void SetGBKillPara(T_USPUHFGBKillPara *ptGBKillPara, WORD16 *pwMacGBKillCommand)
{
    BYTE ucAccessType = ACCESS_KILL;
    /* ��д����������� */
    if (ptGBKillPara->dwKillPassword!= 0)
    {
        uhf_GBSetMacAccessPara(ptGBKillPara->dwKillPassword,0,ucAccessType);
        *pwMacGBKillCommand |= CMD_REG_GB_ACCESS;
    }
    /* ��дGB_Lock������� */
    uhf_GBSetMacKillPara(ptGBKillPara->dwKillPassword);
    *pwMacGBKillCommand |= (CMD_REG_GB_KILL);
}

/**********************************************************************
* �������ƣ�WORD32 SetGBAccessInventPara(T_USPUHFGBTagId *ptGBTagId,WORD16 *pwAccessInventCommand)
* ����������GB���ʿ�ʼ��������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/13    V1.30.00         lxc         ����
************************************************************************/
#define MEMBANKGB         (1)
#define GBSTARTBITADDR  (32)
#define GBBITLEN             (96)
WORD32 SetGBAccessInventPara(T_USPUHFGBTagId *ptGBTagId, WORD16 *pwAccessInventCommand)
{
    WORD16 actionTable[] = {4, 5, 6, 1, 0, 2}; /*���ڽ�LLRP ��׼��ACTION ��ӦΪEPC��׼��ACTION */

    uhf_GB_setTimePara();

    /* MACЭ���Ĵ��� */
    //SetProtoType(UHF_PROTO_TYPE_GB);  //���Խ׶Σ����ϲ㶼��EPC���ֲ��䣬��FPGA�����¹���

    // ����Ĵ�����ʲô��?    uhf_epc_setSelectMask(EPCSTARTBITADDR, ptEpcTagId->wTagWordLen * 16, (WORD16 *)ptEpcTagId->aucTagData);

    //ʹ����һ���洢������Ϣ����ƥ����Ҫȷ��
    //�¹����е�rule ��actionTable ���ƣ�������ô��Ӧ����ȷ��
    //ȥ��֮ǰ�Ľض�/�ǽض� 
    uhf_GBSetSelectPara(0x0/*wRule*/, 0x0/*target*/,MEMBANKGB/*membank*/);

    *pwAccessInventCommand |= CMD_REG_GB_SORT;

    /* ��дGB_Query������� g_tGBCtrl.Query.DR
     g_tGBCtrl.Query.M  ��uhf_GB_getModeParas����������*/
    uhf_GBSetMacQueryPara(g_tGBCtrl.Query.ucM/*M*/,
                                            g_tGBCtrl.Query.ucReLinkRateDivisor/*DR*/);
    *pwAccessInventCommand |= CMD_REG_GB_QUERY;

    return USP_SUCCESS;
}
#endif


