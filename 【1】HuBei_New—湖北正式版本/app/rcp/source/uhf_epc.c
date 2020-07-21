#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_epc.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "EPC.h"
#include "uhf_epc.h"
//#include "uhf_time.h"

static void  SetEpcInventoryResDefault(T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp);
static void  SetEpcReadResDefault(T_USPUHFEpcReadRes *ptEPCReadResp);
static void  SetEpcLockResDefault(T_USPUHFEpcLockRes *ptEPCLockResp);
static void  SetEpcKillResDefault(T_USPUHFEpcKillRes *ptEPCKillResp);
static void  SetEpcWriteResDefault(T_USPUHFEpcWriteRes *ptEPCWriteResp);
static void  SetEpcBlockLockResDefault(T_USPUHFEpcBlockLockRes *ptEPCBlockLockResp);
static void  SetEpcReadLockResDefault(T_USPUHFEpcReadLockRes *ptEPCReadLockResp);
static void  SetEpcIPJQTResDefault(T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp);
extern void  uhf_EpcAccessStopRf();

T_EpcCtl g_tEpcCtrl;
extern T_UhfCtrl g_tUhfCtrl;

/**********************************************************************
* �������ƣ�uhf_Epc_InitCtrlStruct
* ������������ʼ��EPCЭ����صĿ��ƽṹ
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx          ����
************************************************************************/

void uhf_Epc_InitCtrlStruct()
{
    memset((char *)&g_tEpcCtrl, 0, sizeof(g_tEpcCtrl));

    g_tEpcCtrl.wModeIndex = 0xffff;  /*��ʾû������ modeindex*/
    g_tEpcCtrl.wRTModDepth = 100;
}

/**********************************************************************
* �������ƣ�void  uhf_Epc_Inventory_Process()
* ����������epc �����Ϣ����
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx          ����
************************************************************************/
#define FillEpcInventoryAbnomalFinishMsg(ptEpcTagInventoryResp,wErrorCode)  \
      ptEpcTagInventoryResp->tTagId.wTagWordLen=0; \
      ptEpcTagInventoryResp->wInventoryFinish = INVENTORY_HAS_FINISH; \
      ptEpcTagInventoryResp->wResult=wErrorCode;

void usp_uhf_epc_inventory(T_USPUHFEpcInventory *ptEpcInventory,
                           T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;
    struct timeval Epc_Invent_StartTime;
    struct timeval Epc_Invent_EndTime;
    uhfDevPrint("\r\nuhf_Epc_Inventory Start \r\n");
    GetCurrUsTime(&Epc_Invent_StartTime);
    
    MarkInventoryStart();
    dwRetVal = uhf_epc_startInventory(ptEpcInventory);
    GetCurrUsTime(&Epc_Invent_EndTime);
    uhfDevPrint("uhf start inventory use time %dms\r\n",
                CalMsTimeInterval(&Epc_Invent_StartTime, &Epc_Invent_EndTime));
    //dwRetVal = USP_SUCCESS; /*wzp test ignore rf error!!!!!!!!!!!!!*/

    if (USP_SUCCESS != dwRetVal)
    {
        uhfDevPrint("uhf_epc_startInventory error\r\n");
        FillEpcInventoryAbnomalFinishMsg(ptEpcTagInventoryResp, dwRetVal);
        uhf_StopMacProc();
        uhf_StopTx();
    }
    else
    {
        uhfDevPrint("uhf_epc_startInventory success\r\n");
        dwMacRetVal = uhf_Epc_Invetory_Resp(ptEpcTagInventoryResp);
        GetCurrUsTime(&Epc_Invent_EndTime);
        uhfDevPrint("uhf uhf_Epc_Invetory_Resp time %dms\r\n",
                    CalMsTimeInterval(&Epc_Invent_StartTime, &Epc_Invent_EndTime));
        
        if (MAC_INVENTORY_PROCESS_FINISH == dwMacRetVal)
        {
            MarkInventoryFinish();
            uhf_StopTx();
        }
        else if (MAC_INVENTORY_PROCESS_SUCCESS == dwMacRetVal)
        {
            /*����м䲻�رչ���*/
        }
        else
        {
            MarkInventoryFinish();
            FillEpcInventoryAbnomalFinishMsg(ptEpcTagInventoryResp, dwMacRetVal);
            uhf_StopMacProc();
            uhf_StopTx();
        }

    }
    SetEpcInventoryResDefault(ptEpcTagInventoryResp);
    uhf_stopRcpTimer();
    
    uhfDevPrint("uhf_Epc_Inventory ends\r\n");
}
/**********************************************************************
* �������ƣ�void uhf_Epc_Query_Process(T_EPC_TagInventoryReponse *ptEpcTagInventoryResp)
* ����������epc С�����Ϣ����
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/12/3    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_query(T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;
    uhfDevPrint("\r\nuhf_Epc_LLRPQuery_Start\r\n");

    dwRetVal = uhf_epc_startQuery();

    if (USP_SUCCESS != dwRetVal)
    {
        FillEpcInventoryAbnomalFinishMsg(ptEpcTagInventoryResp, dwRetVal);
        uhf_StopMacProc();
        uhf_StopTx();
    }
    else
    {

        dwMacRetVal = uhf_Epc_Invetory_Resp(ptEpcTagInventoryResp);

        if (MAC_INVENTORY_PROCESS_FINISH == dwMacRetVal)
        {
            MarkInventoryFinish();
            uhf_StopTx();
        }
        else if (MAC_INVENTORY_PROCESS_SUCCESS == dwMacRetVal)
        {
            /*����м䲻�رչ���*/
        }
        else
        {
            MarkInventoryFinish();
            FillEpcInventoryAbnomalFinishMsg(ptEpcTagInventoryResp, dwMacRetVal);
            uhf_StopMacProc();
            uhf_StopTx();
        }
    }

    uhf_stopRcpTimer();
    SetEpcInventoryResDefault(ptEpcTagInventoryResp);
    uhfDevPrint("Epc Query ends \r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_read(T_USPUHFEpcRead *ptEpcRead,
                      T_USPUHFEpcReadRes *ptEPCReadResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc Read start \r\n");
    dwRetVal = uhf_epc_startRead(ptEpcRead);

    if (USP_SUCCESS != dwRetVal)
    {
        ptEPCReadResp->wResult = dwRetVal;
        ptEPCReadResp->wResultLength = 0;
    }
    else
    {
        dwMacRetVal = uhf_Epc_Read_Resp(ptEPCReadResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEPCReadResp->wResult = dwMacRetVal;
            ptEPCReadResp->wResultLength = 0;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcReadResDefault(ptEPCReadResp);
    uhfDevPrint("Epc Read ends \r\n");
}
/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_write(T_USPUHFEpcWrite *ptEpcWrite,
                       T_USPUHFEpcWriteRes *ptEPCWriteResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc Write start \r\n");

    /*���д��ֵ�Ƿ��������д��Ҫ��*/
    if (g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tEPCWriteRssiCfg.ucTurnOnFilter == 1)
    {
        if ((g_tEpcCtrl.wLastTagRssi) <
            g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tEPCWriteRssiCfg.wRSSIThreshHold)
        {
            UHFAlertPrint("epc write throw a tag!\r\n");
            ptEPCWriteResp->wResult = USP_UHF_EPC_RSSI_NOT_ENOUGH;
            ptEPCWriteResp->wResultLength = 0;
            SetEpcWriteResDefault(ptEPCWriteResp);
            return;
        }
    }

    dwRetVal = uhf_epc_startWrite(ptEpcWrite);

    if (dwRetVal != USP_SUCCESS)
    {
        ptEPCWriteResp->wResult = dwRetVal;
        ptEPCWriteResp->wResultLength = 0;
    }
    else
    {
        dwMacRetVal = uhf_Epc_Write_Resp(ptEPCWriteResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEPCWriteResp->wResult = dwMacRetVal;
            ptEPCWriteResp->wResultLength = 0;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcWriteResDefault(ptEPCWriteResp);
    uhfDevPrint("Epc Write ends \r\n");
}
/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ������Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_lock(T_USPUHFEpcLock *ptEpcLock, T_USPUHFEpcLockRes *ptEPCLockResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc Lock start \r\n");
    dwRetVal = uhf_epc_startLock(ptEpcLock);

    if (dwRetVal != USP_SUCCESS)
    {
        ptEPCLockResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_Epc_Lock_Resp(ptEPCLockResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEPCLockResp->wResult = dwMacRetVal;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcLockResDefault(ptEPCLockResp);
    uhfDevPrint("Epc Lock ends\r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ������Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_kill(T_USPUHFEpcKill *ptEpcKill, T_USPUHFEpcKillRes *ptEPCKillResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc Kill start \r\n");
    dwRetVal = uhf_epc_startKill(ptEpcKill);

    if (dwRetVal != USP_SUCCESS)
    {
        ptEPCKillResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_Epc_Kill_Resp(ptEPCKillResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            ptEPCKillResp->wResult = dwMacRetVal;
            uhf_StopMacProc();
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcKillResDefault(ptEPCKillResp);
    uhfDevPrint("Epc Kill ends\r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_blocklock(T_USPUHFEpcBlockLock *ptEpcBlockLock,
                           T_USPUHFEpcBlockLockRes *ptEPCBlockLockResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc BlockLock start \r\n");
    dwRetVal = uhf_epc_startBlockLock(ptEpcBlockLock);

    if (USP_SUCCESS != dwRetVal)
    {
        ptEPCBlockLockResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_Epc_BlockLock_Resp(ptEPCBlockLockResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEPCBlockLockResp->wResult = dwMacRetVal;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcBlockLockResDefault(ptEPCBlockLockResp);
    uhfDevPrint("Epc BlockLock ends\r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_Epc_Read_Process()
* ����������epc ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_readlock(T_USPUHFEpcReadLock *ptEpcReadLock,
                          T_USPUHFEpcReadLockRes *ptEPCReadLockResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc ReadLock start \r\n");
    dwRetVal = uhf_epc_startReadLock(ptEpcReadLock);

    if (USP_SUCCESS != dwRetVal)
    {
        ptEPCReadLockResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_Epc_ReadLock_Resp(ptEPCReadLockResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEPCReadLockResp->wResult = dwMacRetVal;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcReadLockResDefault(ptEPCReadLockResp);
    uhfDevPrint("Epc ReadLock ends \r\n");
}

/**********************************************************************
* �������ƣ�usp_uhf_epc_ipjqt
* ����������epc  IPJQT������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_epc_ipjqt(T_USPUHFEpcIPJQT *ptEpcIPJQT,
                       T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nEpc ImpinjQt start \r\n");
    dwRetVal = uhf_epc_startIPJQT(ptEpcIPJQT);

    if (USP_SUCCESS != dwRetVal)
    {
        ptEpcIPJQTResp->wResult = dwRetVal;
    }
    else
    {
        ptEpcIPJQTResp->wReadOrWrite
        = ptEpcIPJQT->tEpcIPJQTPara.wReadOrWrite;
        dwMacRetVal = uhf_Epc_IPJQT_Resp(ptEpcIPJQTResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptEpcIPJQTResp->wResult = dwMacRetVal;
        }
    }

    uhf_stopRcpTimer();
    uhf_EpcAccessStopRf();
    SetEpcIPJQTResDefault(ptEpcIPJQTResp);
    uhfDevPrint("Epc ImpinjQt ends \r\n");
}


/**********************************************************************
* �������ƣ�void CreateEpcInventoryResponseMsg(void)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcInventoryResDefault(T_USPUHFEpcInventoryRes *ptEpcTagInventoryResp)
{
    ptEpcTagInventoryResp->wProtoType    = UHF_PROTO_TYPE_EPC;
    ptEpcTagInventoryResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEpcTagInventoryResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEpcTagInventoryResp->wCRC          = 0xffff;
}


/**********************************************************************
* �������ƣ�void SetEpcInventoryResDefault(void)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcReadResDefault(T_USPUHFEpcReadRes *ptEPCReadResp)
{
    ptEPCReadResp->wProtoType    = UHF_PROTO_TYPE_EPC;
    ptEPCReadResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCReadResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEPCReadResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}


/**********************************************************************
* �������ƣ�void SetEpcInventoryResDefault(void)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static  void SetEpcWriteResDefault(T_USPUHFEpcWriteRes *ptEPCWriteResp)
{
    ptEPCWriteResp->wProtoType = UHF_PROTO_TYPE_EPC;
    ptEPCWriteResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCWriteResp->wChannelIndex =  g_tUhfCtrl.wWorkChannel;
    ptEPCWriteResp->wRssi = g_tEpcCtrl.wLastTagRssi;
}


/**********************************************************************
* �������ƣ�void SetEpcLockResDefault(void)
* �������������EPC�������ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcLockResDefault(T_USPUHFEpcLockRes *ptEPCLockResp)
{
    ptEPCLockResp->wProtoType = UHF_PROTO_TYPE_EPC;
    ptEPCLockResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCLockResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEPCLockResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}


/**********************************************************************
* �������ƣ�void SetEpcLockResDefault(void)
* �������������EPC�������ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcKillResDefault(T_USPUHFEpcKillRes *ptEPCKillResp)
{
    ptEPCKillResp->wProtoType = UHF_PROTO_TYPE_EPC;
    ptEPCKillResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCKillResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEPCKillResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void SetEpcBlockLockResDefault(void)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcBlockLockResDefault(T_USPUHFEpcBlockLockRes *ptEPCBlockLockResp)
{
    ptEPCBlockLockResp->wProtoType = UHF_PROTO_TYPE_EPC;
    ptEPCBlockLockResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCBlockLockResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEPCBlockLockResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}


/**********************************************************************
* �������ƣ�void SetEpcReadLockResDefault(void)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcReadLockResDefault(T_USPUHFEpcReadLockRes *ptEPCReadLockResp)
{
    ptEPCReadLockResp->wProtoType    = UHF_PROTO_TYPE_EPC;
    ptEPCReadLockResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEPCReadLockResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEPCReadLockResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�static void SetEpcIPJQTResDefault(T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp)
* �������������EPC��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx         ����
************************************************************************/
static void SetEpcIPJQTResDefault(T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp)
{
    ptEpcIPJQTResp->wProtoType    = UHF_PROTO_TYPE_EPC;
    ptEpcIPJQTResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptEpcIPJQTResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptEpcIPJQTResp->wRssi         = g_tEpcCtrl.wLastTagRssi;
}

#endif

