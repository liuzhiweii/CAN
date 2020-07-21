#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_epc.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf_dbg.h"
//#include "uhf_time.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "uhf_epc.h"
#include "EPC.h"

static  WORD32 uhf_epc_startAccessInvent(WORD32 dwAntIndex,
                                        T_USPUHFEpcTagId *ptEpcTagId,
                                        WORD16 *pwCommand);
extern  WORD32 SetEpcAccessInventPara(T_USPUHFEpcTagId *ptEpcTagId,
                                          WORD16 *pwAccessInventCommand);
extern  WORD32 uhf_SetInventRfPara(WORD16 wAntIndex, WORD16 wProtoType, WORD16 wForDataRate,
                           WORD16 wRcvDataRate, WORD16 wModDepth,
                           WORD16 wModType, T_USPUHFInventRfPara *ptRfPara);


extern T_EpcCtl g_tEpcCtrl;
extern T_UhfCtrl g_tUhfCtrl;
static WORD16 g_wInventStatFlag = INVENTORY_HAS_FINISH;
/**********************************************************************
* �������ƣ�uhf_epc_startAccess
* ��������?  epc��ǩ��������Ŀ�ʼ�׶�
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00       zdx         ����
************************************************************************/
static WORD32 uhf_epc_startAccess(T_USPUHFEpcTagId *ptEpcTagId, WORD16 wAccessCommand)
{
    WORD32 dwRetVal = USP_SUCCESS;

    if (g_wInventStatFlag == INVENTORY_HAS_FINISH)
    {
        uhf_EpcStartProcAndTimer(wAccessCommand);
    }
    else
    {
        wAccessCommand |= CMD_REG_EPC_LLRP_OPERATION;
        uhf_EpcStartLlrpProcAndTimer(wAccessCommand);
    }

    return  dwRetVal;
}

/**********************************************************************
* �������ƣ�void MarkInventoryStart()
* ��������?  �����㿪ʼ
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00       zdx         ����
************************************************************************/
void MarkInventoryStart()
{
    g_wInventStatFlag = INVENTORY_NOT_FINISH;
}
/**********************************************************************
* �������ƣ�void MarkInventoryFinish()
* ��������?  ���������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00       zdx         ����
************************************************************************/
void MarkInventoryFinish()
{
    g_wInventStatFlag = INVENTORY_HAS_FINISH;
}
/**********************************************************************
* �������ƣ�void EpcAccessRfCtl()
* ��������? ���ݱ�־���Ʒ��ʺ��Ƕ��رչ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00       zdx         ����
************************************************************************/
void uhf_EpcAccessStopRf()
{
    if (g_wInventStatFlag == INVENTORY_HAS_FINISH)
    {
        uhf_StopTx();
    }
    else
    {
        /*��LLRP���ʵ�����м䲻��Ҫ�رչ���*/
        return ;
    }
}
/**********************************************************************
* �������ƣ�uhf_epc_startAccessInvent(T_USPUHFEpcTagId *ptEpcTagId,WORD16 *pwCommand)
* ������������������������Ƿ��ͷ���ǰ�����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/6/13    V1.30.00       zdx         ����
************************************************************************/
static WORD32 uhf_epc_startAccessInvent(WORD32 dwAntIndex,
                                        T_USPUHFEpcTagId *ptEpcTagId,
                                        WORD16 *pwCommand)
{
    WORD32 dwRetVal = USP_SUCCESS;

    if (g_wInventStatFlag == INVENTORY_HAS_FINISH)
    {

        dwRetVal = SetEpcAccessInventPara(ptEpcTagId, pwCommand);

        if (dwRetVal != USP_SUCCESS)
        {
            UHFErrorPrint("UHF: Set access inventory failed \r\n");
            return dwRetVal;
        }

        dwRetVal = uhf_SetInventRfPara(dwAntIndex,
                                       UHF_PROTO_TYPE_EPC,
                                       g_tEpcCtrl.RTDataRate,
                                       g_tEpcCtrl.TRDataRate,
                                       g_tEpcCtrl.wRTModDepth,
                                       g_tEpcCtrl.RTModulateType,
                                       NULL);

        if (USP_SUCCESS != dwRetVal)
        {
            return dwRetVal;
        }

    }

    return dwRetVal;
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
WORD32 uhf_epc_startInventory(T_USPUHFEpcInventory *ptEpcTagInventory)
{
    WORD32 dwRetVal = USP_SUCCESS;
    T_USPUHFEpcInventoryPara *ptEPCInventoryPara = NULL;
    WORD16 wFpgaInventoryCommand = 0; /*��FPGA �·�������BITλ����*/
    struct timeval Epc_Invent_StartTime;
    struct timeval Epc_Invent_EndTime;

    GetCurrUsTime(&Epc_Invent_StartTime);
    
    
    /* ����Э����� */
    ptEPCInventoryPara = &(ptEpcTagInventory->tEpcInventoryPara);
    dwRetVal = SetEpcInventoryPara(ptEPCInventoryPara,
                                   ptEpcTagInventory->ucAntennaID,
                                   &wFpgaInventoryCommand);

    if (USP_SUCCESS != dwRetVal)
    {
        return dwRetVal;
    }

    GetCurrUsTime(&Epc_Invent_EndTime);
    uhfDevPrint("uhf SetEpcInventoryPara time %dms\r\n",
                CalMsTimeInterval(&Epc_Invent_StartTime, &Epc_Invent_EndTime));

    dwRetVal = uhf_SetInventRfPara(ptEpcTagInventory->ucAntennaID,
                                   UHF_PROTO_TYPE_EPC,
                                   g_tEpcCtrl.RTDataRate,
                                   g_tEpcCtrl.TRDataRate,
                                   g_tEpcCtrl.wRTModDepth,
                                   g_tEpcCtrl.RTModulateType,
                                   &ptEpcTagInventory->tInventoryRfPara);
    GetCurrUsTime(&Epc_Invent_EndTime);
    uhfDevPrint("uhf uhf_SetInventRfPara time %dus\r\n",
                CalMsTimeInterval(&Epc_Invent_StartTime, &Epc_Invent_EndTime));

    if (USP_SUCCESS != dwRetVal)
    {
        return dwRetVal;
    }

    uhf_EpcStartProcAndTimer(wFpgaInventoryCommand);
    uhfDevPrint("EPC Inventory start FpgaCommand %x\r\n", wFpgaInventoryCommand);

    //TraceToMacMsg(EV_EPC_Inventory);
    GetCurrUsTime(&Epc_Invent_EndTime);
    UHFAlertPrint("uhf uhf_SetInventEnd time %dms\r\n",
                CalMsTimeInterval(&Epc_Invent_StartTime, &Epc_Invent_EndTime));
    
    return dwRetVal;

}

/**********************************************************************
* �������ƣ�WORD32  uhf_epc_startQuery ()
* ����������EPC������㴥��
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startQuery()
{
    uhf_EpcStartLlrpProcAndTimer(CMD_REG_EPC_QUERY | CMD_REG_EPC_LLRP_OPERATION);
    return USP_SUCCESS;
}

/**********************************************************************
* �������ƣ�WORD32  uhf_epc_startRead(T_C1G2Read *ptRcpTagRead)
* ����������EPC����ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startRead(T_USPUHFEpcRead *ptEpcRead)
{

    WORD16 wMacEpcReadCommand = 0;
    WORD32 dwRet = USP_SUCCESS;

    if (ptEpcRead->tEpcReadPara.wWordCount > EPC_MAX_LENGTH)
    {
        //LAPInsert(FILE_UHF_EPC_START, USP_UHF_RCVED_PARA_ERROR, ptEpcRead->tEpcReadPara.wWordCount, 0);
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if (ptEpcRead->tEpcReadPara.wWordPointer > EPC_READ_WRITE_MAX_LEN
        || ptEpcRead->tEpcReadPara.ucMB > EPC_MAX_MEM_BLOCK_LEN)
    {
        uhfDevPrint("Para error\r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }


    /*���ض����Ƚ��ʱ��FPGA���صĳ�����ȷ��ȥ������*/
    g_tEpcCtrl.tEpcRead.wReadWordsLen = ptEpcRead->tEpcReadPara.wWordCount;

    SetEpcReadPara(&ptEpcRead->tEpcReadPara, &wMacEpcReadCommand);

    dwRet = uhf_epc_startAccessInvent(ptEpcRead->dwAntIndex, &ptEpcRead->tTagId,
                                      &wMacEpcReadCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }


    dwRet = uhf_epc_startAccess(&ptEpcRead->tTagId, wMacEpcReadCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_Read);

    uhfDevPrint("FPGA Read Command %x\r\n", wMacEpcReadCommand);
    return USP_SUCCESS;

}



/**********************************************************************
* �������ƣ�WORD32 uhf_epc_startWrite(T_C1G2Write *ptRcpTagWrite)
* ����������EPCд��ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startWrite(T_USPUHFEpcWrite *ptEpcWrite)
{
    WORD16 wMacEpcWriteCommand = 0;
    WORD32 dwRet = USP_SUCCESS;

    if (ptEpcWrite->tEpcWritePara.wWriteWordLen > EPC_MAX_LENGTH)
    {
        //LAPInsert(FILE_UHF_EPC_START, USP_UHF_RCVED_PARA_ERROR, ptEpcWrite->tEpcWritePara.wWriteWordLen, 0);
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if (ptEpcWrite->tEpcWritePara.wWordPointer > EPC_READ_WRITE_MAX_LEN
        || ptEpcWrite->tEpcWritePara.ucMB > EPC_MAX_MEM_BLOCK_LEN)
    {
        uhfDevPrint("Para error\r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }


    uhfDevPrint("UHF:write membank %d start word %d length %d\r\n",
                ptEpcWrite->tEpcWritePara.ucMB, ptEpcWrite->tEpcWritePara.wWordPointer,
                ptEpcWrite->tEpcWritePara.wWriteWordLen);
    SetEpcWritePara(&ptEpcWrite->tEpcWritePara, &wMacEpcWriteCommand);

    dwRet = uhf_epc_startAccessInvent(ptEpcWrite->dwAntIndex, &ptEpcWrite->tTagId,
                                      &wMacEpcWriteCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }


    dwRet = uhf_epc_startAccess(&ptEpcWrite->tTagId, wMacEpcWriteCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_Write);

    uhfDevPrint("FPGA Write Command %x\r\n", wMacEpcWriteCommand);

    return USP_SUCCESS;

}

/**********************************************************************
* �������ƣ�WORD32 uhf_epc_startLock(T_C1G2Lock *ptRcpTagLock)
* ����������EPC lock��ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startLock(T_USPUHFEpcLock *ptEpcLock)
{
    WORD16 wMacEpcLockCommand = 0;
    WORD32 dwRet = USP_SUCCESS;

    dwRet = SetEpcLockPara(&ptEpcLock->tEpcLockPara, &wMacEpcLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    dwRet = uhf_epc_startAccessInvent(ptEpcLock->dwAntIndex, &ptEpcLock->tTagId,
                                      &wMacEpcLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }


    dwRet = uhf_epc_startAccess(&ptEpcLock->tTagId, wMacEpcLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_Lock);

    uhfDevPrint("Fpga Lock  Command %x\r\n", wMacEpcLockCommand);
    return dwRet;
}


/**********************************************************************
* �������ƣ�WORD32 uhf_epc_startKill(T_C1G2Kill*ptRcpTagKill)
* ����������EPC Kill��ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startKill(T_USPUHFEpcKill *ptEPCKill)
{
    WORD16 wMacEpcKillCommand = 0;
    WORD32 dwRet = USP_SUCCESS;


    SetEpcKillPara(&ptEPCKill->tEpcKillPara, &wMacEpcKillCommand);

    dwRet = uhf_epc_startAccessInvent(ptEPCKill->dwAntIndex, &ptEPCKill->tTagId,
                                      &wMacEpcKillCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }


    dwRet = uhf_epc_startAccess(&ptEPCKill->tTagId, wMacEpcKillCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_Kill);

    return (0);
}




/**********************************************************************
* �������ƣ�WORD32  uhf_epc_startBlockLock(T_USPUHFEpcBlockLock *ptRcpTagBlockLock)
* ����������EPC BlockLock��ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhf_epc_startBlockLock(T_USPUHFEpcBlockLock *ptEpcBlockLock)
{

    WORD16 wMacEpcBlockLockCommand = 0;
    WORD32 dwRet = USP_SUCCESS;


    g_tEpcCtrl.tEpcBlockLock.ucReadLock = ptEpcBlockLock->tEpcBlockLockPara.ucReadLock;

    dwRet = SetEpcBlockLockPara(&ptEpcBlockLock->tEpcBlockLockPara, &wMacEpcBlockLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }

    dwRet = uhf_epc_startAccessInvent(ptEpcBlockLock->dwAntIndex, &ptEpcBlockLock->tTagId,
                                      &wMacEpcBlockLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }


    dwRet = uhf_epc_startAccess(&ptEpcBlockLock->tTagId, wMacEpcBlockLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_BlockLock);

    return dwRet;
}
/**********************************************************************
* �������ƣ�
                   WORD32 uhf_epc_startReadLock(T_USPUHFEpcReadLock *ptEpcReadLock)
* ����������EPC������ǩ���̴���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/2/28   V0.0.1      cj        ����
************************************************************************/
WORD32 uhf_epc_startReadLock(T_USPUHFEpcReadLock *ptEpcReadLock)
{
    WORD16 wMacEpcReadLockCommand = 0;
    WORD32 dwRet = USP_SUCCESS;


    SetEpcReadLockPara(&ptEpcReadLock->tEpcReadLockPara, &wMacEpcReadLockCommand);

    dwRet = uhf_epc_startAccessInvent(ptEpcReadLock->dwAntIndex, &ptEpcReadLock->tTagId,
                                      &wMacEpcReadLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }

    dwRet = uhf_epc_startAccess(&ptEpcReadLock->tTagId, wMacEpcReadLockCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_ReadLock);

    return dwRet;
}
/**********************************************************************
* �������ƣ�uhf_epc_startIPJQT
* ����������EPC impinjqt ��ǩ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/12/14     RUP        zdx       ����
************************************************************************/
WORD32 uhf_epc_startIPJQT(T_USPUHFEpcIPJQT *ptEpcIPJQT)
{
    WORD16 wFpgaEpcIPJQTCommand = 0;
    WORD32 dwRet = USP_SUCCESS;

    SetEpcImpinjQtPara(&ptEpcIPJQT->tEpcIPJQTPara, &wFpgaEpcIPJQTCommand);

    dwRet = uhf_epc_startAccessInvent(ptEpcIPJQT->dwAntIndex, &ptEpcIPJQT->tTagId,
                                      &wFpgaEpcIPJQTCommand);

    if (dwRet != USP_SUCCESS)
    {
        return  dwRet;
    }

    dwRet = uhf_epc_startAccess(&ptEpcIPJQT->tTagId, wFpgaEpcIPJQTCommand);

    if (dwRet != USP_SUCCESS)
    {
        return dwRet;
    }

    //TraceToMacMsg(EV_EPC_IPJQT);
    return USP_SUCCESS;
}

#endif

