#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_epc.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "uhf_epc.h"
#include "EPC.h"

/* RTDataRate   TRDataRate  RealTRDataRate   Tari   wTpri,   wFT   wRTcal_div_Tari   wTRcal_div_halfTari    DR*/
#define   EPC_TIMES_PARA_TABLE {\
    {40, 40, 40, 25,25, 4,3,16,EPC_QUERY_DR_8,       EPC_QUERY_M_1,238,75,150},\
    {40, 80, 80, 25,13, 4,3, 8,EPC_QUERY_DR_8,       EPC_QUERY_M_1,118,38,150},\
    {80, 80, 80, 12,13, 4,3,16,EPC_QUERY_DR_8,       EPC_QUERY_M_1,118,38,75},\
    {80, 160,160,12, 6, 7,3, 8,EPC_QUERY_DR_8,       EPC_QUERY_M_1, 56,19,75},\
    {80, 320,320,13, 3,10,3,11,EPC_QUERY_DR_64_DIV_3,EPC_QUERY_M_1, 32, 1,75},\
    {160,160,160, 6, 6,10,3,16,EPC_QUERY_DR_8,       EPC_QUERY_M_1, 54,19,38 },\
    {160,320,320, 6, 3,10,3, 8,EPC_QUERY_DR_8,       EPC_QUERY_M_1, 26,10,38 },\
    {160,640,640, 6, 2,15,3,11,EPC_QUERY_DR_64_DIV_3,EPC_QUERY_M_1, 13, 5,38 },\
    {105,640,640, 10, 3,10,3,7,EPC_QUERY_DR_64_DIV_3,EPC_QUERY_M_1, 24, 0,58 }\
}

#define uhf_EpcSetQtCommand(CommandPara,FpgaCommand)    \
 if(((CommandPara)&0x1)!=0)\
 {\
     uhf_EpcSetMacIPJQT(EPC_IPJ_QT_WRITE,EPC_IPJ_QT_NOT_PERSIS,\
                           EPC_IPJ_QT_NOT_REDUCE,EPC_IPJ_QT_MEM_PRIVATE);\
     (FpgaCommand) |= CMD_REG_EPC_IPJ_QT;\
 }


extern T_EpcCtl   g_tEpcCtrl;
extern T_UhfCtrl  g_tUhfCtrl;


const T_USPUHFEpcRFModeTableEntry g_tEpcRFModeTable[] = EPC_RFMODE_TABLE;
const T_EPC_TimesPara g_EpcTimesParaTable[] = EPC_TIMES_PARA_TABLE;

WORD32 uhf_epc_getModeParas(WORD16 wModeIndex, WORD16 wAntIndex);
WORD32 uhf_epc_getLockPayLoad(T_USPUHFEpcLockPayloadList *ptLockPayLoad,
                              WORD32 *pdwLockPayLoad);


/**********************************************************************
* �������ƣ�WORD32 SetEpcAccessInventPara(T_USPUHFEpcTagId *ptEpcTagId,WORD16 *pwAccessInventCommand)
* ����������EPC���ʿ�ʼ��������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00         zdx         ����
************************************************************************/
#define MEMBANKEPC         (1)
#define EPCSTARTBITADDR  (32)
#define EPCBITLEN             (96)

WORD32 SetEpcAccessInventPara(T_USPUHFEpcTagId *ptEpcTagId, WORD16 *pwAccessInventCommand)
{
    WORD16 actionTable[] = {4, 5, 6, 1, 0, 2}; /*���ڽ�LLRP ��׼��ACTION ��ӦΪEPC��׼��ACTION */

    uhf_epc_setTimePara();

    /* MACЭ���Ĵ��� */
    //SetProtoType(UHF_PROTO_TYPE_EPC);

    uhf_epc_setSelectMask(EPCSTARTBITADDR, ptEpcTagId->wTagWordLen * 16, (WORD16 *)ptEpcTagId->aucTagData);

    uhf_EpcSetSelectPara(0x0/*truncate*/, MEMBANKEPC/*membank*/,
                         actionTable[0]/*Action*/, 0x0/*target*/);

    *pwAccessInventCommand |= CMD_REG_EPC_SELECT;

    /* ��дEpc_Query������� g_tEpcCtrl.Query.DR
     g_tEpcCtrl.Query.M  ��uhf_epc_getModeParas����������*/
    uhf_EpcSetMacQueryPara(0,/*Qֵ*/g_tEpcCtrl.Query.M/*M*/,
                           g_tEpcCtrl.Query.DR/*DR*/);
    *pwAccessInventCommand |= CMD_REG_EPC_QUERY;

    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�WORD32  uhf_epc_startInventory (T_EPC_TagInventory *ptEpcTagInventory)
* ����������EPC������̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
#define SELECT_ACTION_LEN 6
WORD32 SetEpcInventoryPara(T_USPUHFEpcInventoryPara *ptEPCInventoryPara,
                           WORD16 wAntInde, WORD16 *pwFpgaInventoryCommand)
{
    WORD16 actionTable[] = {4, 5, 6, 1, 0, 2}; /*���ڽ�LLRP ��׼��ACTION ��ӦΪEPC��׼��ACTION */
    WORD32 dwRet = USP_SUCCESS;
    T_USPUHFEpcInventoryMaskList *pEpcMaskList = NULL;
    WORD16 wAction = 0;

    /* ������Ƶģʽ������ʱ����� */
    dwRet = uhf_epc_getModeParas(ptEPCInventoryPara->wModeindex, wAntInde);

    if (USP_SUCCESS != dwRet)
    {
        return dwRet;
    }

    uhf_epc_setTimePara();


    /* MACЭ���Ĵ��� */
    //SetProtoType(UHF_PROTO_TYPE_EPC);

    pEpcMaskList = &ptEPCInventoryPara->tEpcInventoryMaskList;

    if (pEpcMaskList->tEpcInventoryMask[0].wMaskLength
        > EPC_MAX_MASK_WORD_NUM * 16)
    {
        uhfDevPrint("MaskWordLen Error!\r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }

    uhf_epc_setSelectMask(pEpcMaskList->tEpcInventoryMask[0].wMaskBitPointer,
                          pEpcMaskList->tEpcInventoryMask[0].wMaskLength,
                          (WORD16 *)pEpcMaskList->tEpcInventoryMask[0].ucMaskData);

    wAction = pEpcMaskList->tEpcInventoryMask[0].eSelectAction;

    if (wAction > SELECT_ACTION_LEN)
    {
        uhfDevPrint("Rcv invalied action number\r\n");
        wAction = 0;
    }

    /* дFPGA��select��������Ĵ��� */
    uhf_EpcSetSelectPara(0x0/*truncate*/, pEpcMaskList->tEpcInventoryMask[0].wMemBank/*membank*/,
                         actionTable[wAction]/*Action*/, 0x0/*target*/);
    *pwFpgaInventoryCommand |= CMD_REG_EPC_SELECT;

    /* ��дuhf_Epc_Query������� g_tEpcCtrl.Query.DR
       g_tEpcCtrl.Query.M  ��uhf_epc_getModeParas����������*/
    uhf_EpcSetMacQueryPara(uhf_getInventQ(ptEPCInventoryPara->wEstimatedTagNum),/*Qֵ*/
                           g_tEpcCtrl.Query.M/*M*/, g_tEpcCtrl.Query.DR/*DR*/);

    *pwFpgaInventoryCommand |= (CMD_REG_EPC_QUERY | CMD_REG_EPC_LLRP_OPERATION);
    return dwRet;

}


/**********************************************************************
* �������ƣ�WORD32  SetEpcReadPara(T_LLRPC1G2Read *ptEPCReadCommand,WORD16 *pwFpgaEpcReadCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
void SetEpcReadPara(T_USPUHFEpcReadPara *ptEPCReadPara,
                    WORD16 *pwEpcReadCommand)
{

    if (ptEPCReadPara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCReadPara->dwAccessPassword);
        *pwEpcReadCommand |= CMD_REG_EPC_ACCESS;
    }

    /*�Ƿ���Ҫ����qt����*/
    uhf_EpcSetQtCommand(ptEPCReadPara->dwUhfBitCommand, *pwEpcReadCommand);
    /* ��дEpc_Read������� */
    uhf_EpcSetMacReadPara(ptEPCReadPara->ucMB, ptEPCReadPara->wWordPointer,
                          ptEPCReadPara->wWordCount);
    *pwEpcReadCommand |= CMD_REG_EPC_READ;
}


/**********************************************************************
* �������ƣ�WORD32  SetEpcBlockLockPara(T_LLRPC1G2BlockLock *ptEPCBlockLockPara,WORD16 *pwFpgaEpcBlockLockCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
void SetEpcReadLockPara(T_USPUHFEpcReadLockPara *ptEPCReadLockPara,
                        WORD16 *pwmacEpcReadLockCommand)
{

    if (ptEPCReadLockPara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCReadLockPara->dwAccessPassword);
        *pwmacEpcReadLockCommand |= CMD_REG_EPC_ACCESS;
    }

    /* ��дEpc_Lock������� */
    uhf_EpcSetMacReadLock((WORD16)ptEPCReadLockPara->ucMask);
    *pwmacEpcReadLockCommand |= (CMD_REG_EPC_READ_LOCK);
}



/**********************************************************************
* �������ƣ�WORD32  SetEpcBlockLockPara(T_LLRPC1G2BlockLock *ptEPCBlockLockPara,WORD16 *pwFpgaEpcBlockLockCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
#define BLOCK_LOCK_MAX_LEN 512
WORD32 SetEpcBlockLockPara(T_USPUHFEpcBlockLockPara *ptEPCBlockLockPara,
                           WORD16 *pwMacEpcBlockLockCommand)
{
    if (ptEPCBlockLockPara->wBlockRange > BLOCK_LOCK_MAX_LEN)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if (ptEPCBlockLockPara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCBlockLockPara->dwAccessPassword);
        *pwMacEpcBlockLockCommand |= CMD_REG_EPC_ACCESS;
    }

    /* ��дEpc_Lock������� */
    uhf_EpcSetMacBlockLock(ptEPCBlockLockPara->ucReadLock, ptEPCBlockLockPara->ucMB,
                           ptEPCBlockLockPara->wBlockPtr, ptEPCBlockLockPara->wBlockRange,
                           ptEPCBlockLockPara->ucMask);
    *pwMacEpcBlockLockCommand |= (CMD_REG_EPC_BLOCK_LOCK);

    return USP_SUCCESS;
}



/**********************************************************************
* �������ƣ�WORD32  SetEpcKillPara(T_LLRPC1G2Kill *ptEPCKillCommand,WORD16 *pwFpgaEpcKillCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
void SetEpcKillPara(T_USPUHFEpcKillPara *ptEPCKillPara, WORD16 *pwMacEpcKillCommand)
{
    /* ��дEpc_Lock������� */
    uhf_EpcSetMacKillPara(ptEPCKillPara->dwKillPassword);
    *pwMacEpcKillCommand |= (CMD_REG_EPC_KILL);

}



/**********************************************************************
* �������ƣ�WORD32  SetEpcReadPara(T_LLRPC1G2Read *ptEPCReadCommand,WORD16 *pwFpgaEpcReadCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
WORD32 SetEpcLockPara(T_USPUHFEPcLockPara *ptEPCLockPara,
                      WORD16 *pwFpgaEpcLockCommand)
{
    WORD32 dwLockPayLoad = 0;
    WORD32 dwRet = USP_SUCCESS;

    if (ptEPCLockPara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCLockPara->dwAccessPassword);
        *pwFpgaEpcLockCommand |= CMD_REG_EPC_ACCESS;
    }

    /* ��дEpc_Lock������� */
    dwRet = uhf_epc_getLockPayLoad(&ptEPCLockPara->tEpcLockPayLoad, &dwLockPayLoad);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    uhf_EpcSetMacLockPara(dwLockPayLoad);
    *pwFpgaEpcLockCommand |= (CMD_REG_EPC_LOCK);

    return dwRet;
}


/**********************************************************************
* �������ƣ�WORD32  SetEpcReadPara(T_LLRPC1G2Read *ptEPCReadCommand,WORD16 *pwFpgaEpcReadCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
WORD32 SetEpcImpinjQtPara(T_USPUHFEpcIPJQTPara *ptEPCIPJQTPara,
                          WORD16 *pwFpgaEpcImpinjQtCommand)
{
    WORD32 dwRet = USP_SUCCESS;

    if (ptEPCIPJQTPara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCIPJQTPara->dwAccessPassword);
        *pwFpgaEpcImpinjQtCommand |= CMD_REG_EPC_ACCESS;
    }

    uhf_EpcSetMacIPJQT(ptEPCIPJQTPara->wReadOrWrite, ptEPCIPJQTPara->wPersistence,
                       ptEPCIPJQTPara->wAccessRange, ptEPCIPJQTPara->wDataProfile);

    *pwFpgaEpcImpinjQtCommand |= (CMD_REG_EPC_IPJ_QT | CMD_REG_EPC_LLRP_OPERATION);

    return dwRet;
}


/**********************************************************************
* �������ƣ�WORD32  SetEpcReadPara(T_LLRPC1G2Read *ptEPCReadCommand,WORD16 *pwFpgaEpcReadCommand)
* ����������EPC���ö�ȡ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24      V0.0.1         zdx         ����
************************************************************************/
void SetEpcWritePara(T_USPUHFEpcWritePara *ptEPCWritePara,
                     WORD16 *pwEpcWriteCommand)
{
    WORD32 dwIsBlockWrite = 1;

    dwIsBlockWrite = ptEPCWritePara->ucWriteType;
    g_tEpcCtrl.tEpcWrite.wWriteWordsLen = ptEPCWritePara->wWriteWordLen;
    g_tEpcCtrl.tEpcWrite.wIsBlockWrite = dwIsBlockWrite;

    if (0 == dwIsBlockWrite)
    {
        uhf_EpcSetMacWritePara(ptEPCWritePara->ucMB, ptEPCWritePara->wWordPointer,
                               ptEPCWritePara->wWriteWordLen, ptEPCWritePara->aucData);
        *pwEpcWriteCommand |= CMD_REG_EPC_WRITE;
    }
    else
    {
        uhf_EpcSetMacBlockWritePara(ptEPCWritePara->ucMB, ptEPCWritePara->wWordPointer,
                                    ptEPCWritePara->wWriteWordLen, ptEPCWritePara->aucData);
        *pwEpcWriteCommand |= CMD_REG_EPC_BLOCK_WRITE;
    }

    /*�Ƿ���Ҫ����qt����*/
    uhf_EpcSetQtCommand(ptEPCWritePara->dwUhfBitCommand, *pwEpcWriteCommand);

    if (ptEPCWritePara->dwAccessPassword != 0)
    {
        uhf_EpcSetMacAccessPara(ptEPCWritePara->dwAccessPassword);
        *pwEpcWriteCommand |= CMD_REG_EPC_ACCESS;
    }

}


/**********************************************************************
* �������ƣ� WORD16 uhf_epc_getModeParas(WORD16 ModeIndex)
* ���������� ���ݸ�����ModeIndex��ֵȷ��GEN2��ص�ʱ�����,���Ʒ�ʽ��������뷽ʽ��PIEֵ��Ŀǰ�����䣩
* ���ʵı�
* �޸ĵı�
* ���������
* ���������
* �� �� ֵ��
* ����˵����  һ�´�LLRP ��׼�Ķ������32λתΪ16 λ�ٿ���
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*
***********************************************************************/
WORD32 uhf_epc_getModeParas(WORD16 wModeIndex, WORD16 wAntIndex)
{
    WORD32 dwIndex = 0;
    WORD16 wModulateIndex = 0;
    WORD16 ModulateTable[] = {3, 2, 1};
    static const WORD16 wEpcModeTableLen = sizeof(g_tEpcRFModeTable) / sizeof(T_USPUHFEpcRFModeTableEntry);

    /* ������⣬����ֵ�ķ�Χ��0-20*/
    uhfDevPrint("modeindex == %d AntennaIndex %d wEpcModeTableLen = %d\r\n",
                wModeIndex, wAntIndex, wEpcModeTableLen);

    if (wModeIndex == ZTE_RF_MODEINDEX)
    {
        g_tEpcCtrl.wModeIndex = wModeIndex;
        g_tEpcCtrl.RTDataRate = g_tUhfCtrl.atRfAntCfg[wAntIndex].tEpcUserDefPara.wForDataRate;
        g_tEpcCtrl.TRDataRate = g_tUhfCtrl.atRfAntCfg[wAntIndex].tEpcUserDefPara.wRevDataRate;
        wModulateIndex = g_tUhfCtrl.atRfAntCfg[wAntIndex].tEpcUserDefPara.eModulateType;
        g_tEpcCtrl.RTModulateType = ModulateTable[wModulateIndex];
        g_tEpcCtrl.Query.M = g_tUhfCtrl.atRfAntCfg[wAntIndex].tEpcUserDefPara.eDataEncodeType;
        uhfDevPrint("the match rate is %d  and %d RTModulateType %d\r\n",
                    g_tEpcCtrl.RTDataRate, g_tEpcCtrl.TRDataRate, g_tEpcCtrl.RTModulateType);

    }
    else
    {

        uhfDevPrint("%d,%d,%d\r\n", g_tEpcRFModeTable[wModeIndex].eForwardLinkModulation,
                    g_tEpcRFModeTable[wModeIndex].dwMinTariValue, g_tEpcRFModeTable[wModeIndex].dwBDRValue);

        if (wModeIndex == g_tEpcCtrl.wModeIndex)
        {
            return USP_SUCCESS;
        }
        else
        {
            if (wModeIndex >= wEpcModeTableLen)
            {
                wModeIndex = 0;
            }

            g_tEpcCtrl.wModeIndex = wModeIndex;
        }

        wModulateIndex = (WORD16)g_tEpcRFModeTable[wModeIndex].eForwardLinkModulation;
        g_tEpcCtrl.RTModulateType = ModulateTable[wModulateIndex];
        g_tEpcCtrl.RTDataRate = (WORD16)((WORD32)(1000000) /
                                         g_tEpcRFModeTable[wModeIndex].dwMinTariValue);
        g_tEpcCtrl.TRDataRate = (WORD16)(g_tEpcRFModeTable[wModeIndex].dwBDRValue / 1000);   //��λ����Ϊ��ʱ���һ��
        g_tEpcCtrl.Query.M = (WORD16)g_tEpcRFModeTable[wModeIndex].eMValue;
        uhfDevPrint("the match rate is %d  and %d and %d \r\n",
                    g_tEpcCtrl.RTDataRate, g_tEpcCtrl.TRDataRate,
                    g_tEpcCtrl.RTModulateType);
    }

    for (dwIndex = 0; dwIndex < sizeof(g_EpcTimesParaTable) / sizeof(g_EpcTimesParaTable[0]); dwIndex++)
    {
        if ((g_EpcTimesParaTable[dwIndex].RTDataRate == g_tEpcCtrl.RTDataRate)
            && (g_EpcTimesParaTable[dwIndex].TRDataRate == g_tEpcCtrl.TRDataRate))
        {
            break;
        }
    }

    if (dwIndex < sizeof(g_EpcTimesParaTable) / sizeof(g_EpcTimesParaTable[0]))
        {}
    else
    {
        /* �����ҵ�ƥ����������ã�ʹ��Ĭ�ϵ��������� */
        uhfDevPrint("UHF: user set rf link data rate is invid ,use default config \r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }

    uhfDevPrint("find the epc time para is %d\r\n", dwIndex);

    /* ������·�������þ���ʱ����� */
    g_tEpcCtrl.RealTRDataRate = g_EpcTimesParaTable[dwIndex].RealTRDataRate;

    g_tEpcCtrl.wRTcal_div_Tari = g_EpcTimesParaTable[dwIndex].wRTcal_div_Tari;
    g_tEpcCtrl.wTRcal_div_halfTari = g_EpcTimesParaTable[dwIndex].wTRcal_div_halfTari;

    g_tEpcCtrl.wTpri = g_EpcTimesParaTable[dwIndex].wTpri;

    g_tEpcCtrl.Query.DR = g_EpcTimesParaTable[dwIndex].DR;

    g_tEpcCtrl.T1min = g_EpcTimesParaTable[dwIndex].T1min;
    g_tEpcCtrl.T2min = g_EpcTimesParaTable[dwIndex].T2min;
    g_tEpcCtrl.T4min = g_EpcTimesParaTable[dwIndex].T4min;

    return (0);

}


WORD32 uhf_epc_getLockPayLoad(T_USPUHFEpcLockPayloadList *ptLockPayLoad,
                              WORD32 *pdwLockPayLoad)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD16 tmpVar;
    WORD16 wShiftEpcLockAction[] = {8, 6, 4, 2, 0};
    WORD16 wLockDataField;
    WORD16 wActionIndex;
    WORD16 wActionTable[] = {2, 3, 1, 0};

    if (ptLockPayLoad->wPayLoadNum > 5)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    for (tmpVar = 0; tmpVar < ptLockPayLoad->wPayLoadNum; tmpVar++)
    {

        wLockDataField = ptLockPayLoad->taEpcLockPayLoad[tmpVar].DataField;
        wActionIndex = ptLockPayLoad->taEpcLockPayLoad[tmpVar].Privilege;

        if ((wLockDataField > 5) || (wActionIndex > 4))
        {
            dwRet = USP_UHF_RCVED_PARA_ERROR;
            break;
        }

        uhfDevPrint("the %d loop, DataFiled is %d,and the Action is %d\r\n",
                    tmpVar, wLockDataField, wActionIndex);
        /*zdx needmodify ���ӵ������������ʱ�Ĵ���*/
        *pdwLockPayLoad |= (0x3 << (wShiftEpcLockAction[wLockDataField] + 10));
        uhfDevPrint("the first PayLoad is %d\t", *pdwLockPayLoad);
        *pdwLockPayLoad |=
            (wActionTable[wActionIndex] << wShiftEpcLockAction[wLockDataField]);
        uhfDevPrint("the second  PayLoad is %d\t", *pdwLockPayLoad);
    }

    return dwRet;
}

#endif
