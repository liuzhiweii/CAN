#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_gb.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "GB.h"
#include "uhf_gb.h"
//#include "uhf_time.h"

static void  SetGBInventoryResDefault(T_USPUHFGBInventoryRes *ptGBTagInventoryResp);
static void  SetGBReadResDefault(T_USPUHFGBReadRes *ptGBReadResp);
static void  SetGBLockResDefault(T_USPUHFGBLockRes *ptGBLockResp);
static void  SetGBKillResDefault(T_USPUHFGBKillRes *ptGBKillResp);
static void  SetGBWriteResDefault(T_USPUHFGBWriteRes *ptGBWriteResp);
static void  SetGBEraseResDefault(T_USPUHFGBEraseRes *ptGBEraseResp);
extern void  uhf_GBAccessStopRf();

T_GBCtl g_tGBCtrl;
extern T_UhfCtrl g_tUhfCtrl;

/**********************************************************************
* �������ƣ�uhf_GB_InitCtrlStruct
* ������������ʼ��GBЭ����صĿ��ƽṹ
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx          ����
************************************************************************/

void uhf_GB_InitCtrlStruct()
{
    memset(&g_tGBCtrl, 0x0, sizeof(g_tGBCtrl));

    g_tGBCtrl.wModeIndex = 0xffff;  /*��ʾû������ modeindex*/
    g_tGBCtrl.wRTModDepth = 100;
}

/**********************************************************************
* �������ƣ�void CreateGBInventoryResponseMsg(void)
* �������������GB��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/23    V0.0.1       lxc         ����
************************************************************************/
static void SetGBInventoryResDefault(T_USPUHFGBInventoryRes *ptGBTagInventoryResp)
{
    ptGBTagInventoryResp->wProtoType    = UHF_PROTO_TYPE_GB;
    ptGBTagInventoryResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBTagInventoryResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptGBTagInventoryResp->wCRC          = 0xffff;
}


/**********************************************************************
* �������ƣ�void SetGBInventoryResDefault(void)
* �������������GB��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/23    V0.0.1       lxc         ����
************************************************************************/
static void SetGBReadResDefault(T_USPUHFGBReadRes *ptGBReadResp)
{
    ptGBReadResp->wProtoType    = UHF_PROTO_TYPE_GB;
    ptGBReadResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBReadResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptGBReadResp->wRssi         = g_tGBCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void SetGBInventoryResDefault(void)
* �������������GB��㷵�ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/23    V0.0.1       lxc         ����
************************************************************************/
static  void SetGBWriteResDefault(T_USPUHFGBWriteRes *ptGBWriteResp)
{
    ptGBWriteResp->wProtoType = UHF_PROTO_TYPE_GB;
    ptGBWriteResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBWriteResp->wChannelIndex =  g_tUhfCtrl.wWorkChannel;
    ptGBWriteResp->wRssi = g_tGBCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void SetGBLockResDefault(void)
* �������������GB�������ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/1/23    V0.0.1       lxc         ����
************************************************************************/
static void SetGBLockResDefault(T_USPUHFGBLockRes *ptGBLockResp)
{
    ptGBLockResp->wProtoType = UHF_PROTO_TYPE_GB;
    ptGBLockResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBLockResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptGBLockResp->wRssi         = g_tGBCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void SetGBEraseResDefault(void)
* �������������GB�������ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/1/23    V0.0.1       lxc         ����
************************************************************************/
static void SetGBEraseResDefault(T_USPUHFGBEraseRes *ptGBEraseResp)
{
    ptGBEraseResp->wProtoType = UHF_PROTO_TYPE_GB;
    ptGBEraseResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBEraseResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptGBEraseResp->wRssi         = g_tGBCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void SetGBLockResDefault(void)
* �������������GB�������ؽṹ�еĹ�������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/23    V0.0.1       lxc         ����
************************************************************************/
static void SetGBKillResDefault(T_USPUHFGBKillRes *ptGBKillResp)
{
    ptGBKillResp->wProtoType = UHF_PROTO_TYPE_GB;
    ptGBKillResp->wAntennaIndex = g_tUhfCtrl.wAntIndex;
    ptGBKillResp->wChannelIndex = g_tUhfCtrl.wWorkChannel;
    ptGBKillResp->wRssi         = g_tGBCtrl.wLastTagRssi;
}

/**********************************************************************
* �������ƣ�void  uhf_gb_Inventory()
* ����������GB �����Ϣ����
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/23    V0.0.1       zdx          ����
************************************************************************/
#define FillGBInventoryAbnomalFinishMsg(ptGBTagInventoryResp,wErrorCode)  \
          ptGBTagInventoryResp->tTagId.wTagWordLen=0; \
          ptGBTagInventoryResp->wInventoryFinish = INVENTORY_HAS_FINISH; \
          ptGBTagInventoryResp->wResult=wErrorCode;

void usp_uhf_gb_inventory(T_USPUHFGBInventory *ptGBInventory,
                           T_USPUHFGBInventoryRes *ptGBTagInventoryResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;
    struct timeval GB_Invent_StartTime;
    struct timeval GB_Invent_EndTime;
    uhfDevPrint("\r\nuhf_GB_Inventory Start \r\n");
    GetCurrUsTime(&GB_Invent_StartTime);
    
    MarkInventoryStart(); /* inventory_not_finish */
    dwRetVal = uhf_GB_startInventory(ptGBInventory);
    GetCurrUsTime(&GB_Invent_EndTime);
    uhfDevPrint("uhf start inventory use time %dms\r\n",
    CalMsTimeInterval(&GB_Invent_StartTime, &GB_Invent_EndTime));

#ifndef RCP_RES_DEBUG
    if (USP_SUCCESS != dwRetVal)
#else
    if(0)
#endif
    {
        uhfDevPrint("\r\nuhf_GB_startInventory ERROR!!! \r\n");
#ifndef GB_DEBUG
        FillGBInventoryAbnomalFinishMsg(ptGBTagInventoryResp, dwRetVal);
#endif
        uhf_StopMacProc();
        uhf_StopTx();
    }
    else
    {
        dwMacRetVal = uhf_GB_Invetory_Resp(ptGBTagInventoryResp);

        GetCurrUsTime(&GB_Invent_EndTime);
        uhfDevPrint("uhf uhf_GB_Invetory_Resp time %dms\r\n",
                    CalMsTimeInterval(&GB_Invent_StartTime, &GB_Invent_EndTime));

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
            FillGBInventoryAbnomalFinishMsg(ptGBTagInventoryResp, dwMacRetVal);
            uhf_StopMacProc();
            uhf_StopTx();
        }

    }

    SetGBInventoryResDefault(ptGBTagInventoryResp);
    uhf_stopRcpTimer();
    
    uhfDevPrint("uhf_GB_Inventory ends\r\n");
}
/**********************************************************************
* �������ƣ�void usp_uhf_gb_query(T_USPUHFGBInventoryRes *ptGBTagInventoryResp)
* ����������GB С�����Ϣ����
* ���������
* ���������
* �� �� ֵ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/    V0.0.1       lxc          ����
************************************************************************/
void usp_uhf_gb_query(T_USPUHFGBInventoryRes *ptGBTagInventoryResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;
    uhfDevPrint("\r\nuhf_GB_LLRPQuery_Start\r\n");

    dwRetVal = uhf_GB_startQuery();

    if (USP_SUCCESS != dwRetVal)
    {
        FillGBInventoryAbnomalFinishMsg(ptGBTagInventoryResp, dwRetVal);
        uhf_StopMacProc();
        uhf_StopTx();
    }
    else
    {

        dwMacRetVal = uhf_GB_Invetory_Resp(ptGBTagInventoryResp);

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
            FillGBInventoryAbnomalFinishMsg(ptGBTagInventoryResp, dwMacRetVal);
            uhf_StopMacProc();
            uhf_StopTx();
        }
    }

    uhf_stopRcpTimer();
    SetGBInventoryResDefault(ptGBTagInventoryResp);
    uhfDevPrint("GB Query ends \r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_GB_Read_Process()
* ����������GB ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24    V0.0.1       lxc          ����
************************************************************************/
void usp_uhf_gb_read(T_USPUHFGBRead *ptGBRead,
                      T_USPUHFGBReadRes *ptGBReadResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nGB Read start \r\n"); 
    dwRetVal = uhf_GB_startRead(ptGBRead); //������������Э����Ĵ�����������������

    if (USP_SUCCESS != dwRetVal)
    {
        ptGBReadResp->wResult = dwRetVal;
        ptGBReadResp->wResultLength = 0;
    }
    else
    {
        dwMacRetVal = uhf_GB_Read_Resp(ptGBReadResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptGBReadResp->wResult = dwMacRetVal;
            ptGBReadResp->wResultLength = 0;
        }
    }

    uhf_stopRcpTimer();
    uhf_GBAccessStopRf();
    SetGBReadResDefault(ptGBReadResp);
    uhfDevPrint("GB Read ends \r\n");
}


/**********************************************************************
* �������ƣ�void  uhf_gb_Read_Process()
* ����������gb ����Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24    V0.0.1       lxc          ����
************************************************************************/
void usp_uhf_gb_write(T_USPUHFGBWrite *ptGBWrite,
                       T_USPUHFGBWriteRes *ptGBWriteResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nGB Write start \r\n");

    /*���д��ֵ�Ƿ��������д��Ҫ��*/
    /* �ϲ㴫���ֵ���¹����Ƿ���Ҫ???? */
    if (g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tGBWriteRssiCfg.ucTurnOnFilter == 1)
    {
        if ((g_tGBCtrl.wLastTagRssi) <
            g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tGBWriteRssiCfg.wRSSIThreshHold)
        {
            UHFAlertPrint("GB write throw a tag!\r\n");
            ptGBWriteResp->wResult = USP_UHF_GB_RSSI_NOT_ENOUGH;
            ptGBWriteResp->wResultLength = 0;
            SetGBWriteResDefault(ptGBWriteResp);
            return;
        }
    }

    dwRetVal = uhf_GB_startWrite(ptGBWrite);

    if (dwRetVal != USP_SUCCESS)
    {
        ptGBWriteResp->wResult = dwRetVal;
        ptGBWriteResp->wResultLength = 0;
    }
    else
    {
        dwMacRetVal = uhf_GB_Write_Resp(ptGBWriteResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptGBWriteResp->wResult = dwMacRetVal;
            ptGBWriteResp->wResultLength = 0;
        }
    }

    uhf_stopRcpTimer();
    uhf_GBAccessStopRf();
    SetGBWriteResDefault(ptGBWriteResp);
    uhfDevPrint("GB Write ends \r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_GB_Erase_Process()
* ����������GB ������Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/24    V0.0.1       zdx          ����
************************************************************************/
void usp_uhf_gb_erase(T_USPUHFGBErase *ptGBErase,
                                                T_USPUHFGBEraseRes *ptGBEraseResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nGB Erase start \r\n");

    /*���д��ֵ�Ƿ��������д��Ҫ��*/
    if (g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tGBWriteRssiCfg.ucTurnOnFilter == 1)
    {
        if ((g_tGBCtrl.wLastTagRssi) <
            g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tGBWriteRssiCfg.wRSSIThreshHold)
        {
            UHFAlertPrint("GB Erase throw a tag!\r\n");
            ptGBEraseResp->wResult = USP_UHF_GB_RSSI_NOT_ENOUGH;
            ptGBEraseResp->wResultLength = 0;
            SetGBEraseResDefault(ptGBEraseResp);
            return;
        }
    }

    dwRetVal = uhf_GB_startErase(ptGBErase);

    if (dwRetVal != USP_SUCCESS)
    {
        ptGBEraseResp->wResult = dwRetVal;
        ptGBEraseResp->wResultLength = 0;
    }
    else
    {
        dwMacRetVal = uhf_GB_Erase_Resp(ptGBEraseResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptGBEraseResp->wResult = dwMacRetVal;
            ptGBEraseResp->wResultLength = 0;
        }
    }

    uhf_stopRcpTimer();
    uhf_GBAccessStopRf();
    SetGBEraseResDefault(ptGBEraseResp);
    uhfDevPrint("GB Write ends \r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_gb_Read_Process()
* ����������GB ������Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/24    V0.0.1       lxc          ����
************************************************************************/
void usp_uhf_gb_lock(T_USPUHFGBLock *ptGBLock, T_USPUHFGBLockRes *ptGBLockResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nGB Lock start \r\n");
    dwRetVal = uhf_GB_startLock(ptGBLock); 

    if (dwRetVal != USP_SUCCESS)
    {
        ptGBLockResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_GB_Lock_Resp(ptGBLockResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            uhf_StopMacProc();
            ptGBLockResp->wResult = dwMacRetVal;
        }
    }

    uhf_stopRcpTimer();
    uhf_GBAccessStopRf();
    SetGBLockResDefault(ptGBLockResp);
    uhfDevPrint("GB Lock ends\r\n");
}

/**********************************************************************
* �������ƣ�void  uhf_GB_Read_Process()
* ����������GB ������Ϣ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/11/24    V0.0.1       lxc          ����
************************************************************************/
void usp_uhf_gb_kill(T_USPUHFGBKill *ptGBKill, T_USPUHFGBKillRes *ptGBKillResp)
{
    WORD32 dwRetVal = 0;
    WORD32 dwMacRetVal = 0;

    uhfDevPrint("\r\nGB Kill start \r\n");
    dwRetVal = uhf_GB_startKill(ptGBKill);

    if (dwRetVal != USP_SUCCESS)
    {
        ptGBKillResp->wResult = dwRetVal;
    }
    else
    {
        dwMacRetVal = uhf_GB_Kill_Resp(ptGBKillResp);

        if (USP_SUCCESS != dwMacRetVal)
        {
            ptGBKillResp->wResult = dwMacRetVal;
            uhf_StopMacProc();
        }
    }

    uhf_stopRcpTimer();
    uhf_GBAccessStopRf();
    SetGBKillResDefault(ptGBKillResp);
    uhfDevPrint("GB Kill ends\r\n");
}

#endif

