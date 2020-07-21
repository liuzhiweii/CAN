/*********************************************************************
* ��Ȩ���� (C)2005, ����������ͨѶ�ɷ����޹�˾��
*
* �ļ����ƣ� FpgaApi.c
* �ļ���ʶ��
* ����ժҪ�� ʵ��FPGA��������ӿڷ�װ
* ����˵����
* ��ǰ�汾��
* ��    �ߣ� ft
* ������ڣ� 2006-09-29
*
* �޸ļ�¼1��
* �޸ļ�¼2��
* �޸ļ�¼3��
**********************************************************************/

/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "uhf_trace.h"
#include "uhf_epc.h"
#include "uhf_iso6b.h"
#include "uhf_gb.h"
#include "uhfFpgaRegDef.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "EPC.h"
#include "ISO6B.h"
#include "GB.h"
//#include "uhf_time.h"
#define FPGA_SWTICH   0

/***********************************************************
 *                     ��������                            *
***********************************************************/



#define FPGA_CMD_WRITE_REG_BYTE_LEN (6)
#define FPGA_CMD_READ_REG_BYTE_LEN  (6)

/* FPGA cpu interface bus bits width */
#define FPGA_DATA_BUS_WIDTH         (16)

/* use mmap mode to access fpga register */
#define FPGA_MMAP_ACCESS_REG

/***********************************************************
*                 �ļ��ڲ�ʹ�õĺ�                        *
***********************************************************/

#define RcpFpgaPrint(...) usp_oss_printf(USP_UHF_TNO,USP_OSS_PRINTF_LEVEL_DEBUG,__VA_ARGS__)

/***********************************************************
*               �ļ��ڲ�ʹ�õ���������                    *
***********************************************************/


/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/
extern  T_EpcCtl g_tEpcCtrl;
extern  T_GBCtl g_tGBCtrl;
/* end Add by gaoyu for DATTControl, 2007��8��17�� 9:50:29 */
/***********************************************************
 *                     ���ر���                            *
***********************************************************/
/* FPGA���ݻ��� */
WORD16 g_awFpgaReadBuf[FPGA_BUF_BYTE_SIZE / 2];

/* ʧ��Ӧ����һ�־͹��� */

/*
    R Counter Latch:
    14-BIT REFERENCE COUNTER : CONTROL BITS
*/

/***********************************************************
 *                     ȫ�ֺ���                            *
***********************************************************/



/***********************************************************
 *                     �ֲ�����                            *
***********************************************************/
static void StartFpgaProc(WORD16 wCmdType);
static WORD32 FpgaTestNCnt(WORD32 dwTestCnt);
static void StartFpgallrpProc(WORD16 dwCmdType);
static WORD32 FpgaTest(void);
static void StartFpgaProcessIsoCmd(void);


/**********************************************************************
* �������ƣ�uhf_mac_Init
* ��������������fpga�豸��ͬʱͨ�������ԼĴ���
                                ���жϵ�ǰ�Ƿ���������������һֱѭ��
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2012/3/16       RUP      zdx          ����
************************************************************************/
WORD32 uhf_mac_Init(void)
{
    WORD32 dwRet = USP_SUCCESS;
    WORD32 dwFpgaTestErrorCount;
#if FPGA_SWTICH
    dwRet = usp_drv_fpga_init();
#endif 
    if (USP_SUCCESS != dwRet)
    {
        printf("UHF:usp_drv_fpga_init is failed,dwRet = %d\r\n", dwRet);
        return dwRet;
    }

    /* ��ѯFPGA�߼��汾 */
    printf("UHF: FpgaVersion = 0x%04x \r\n", FpgaRegRead(REG_FPGA_VERSION));

    dwFpgaTestErrorCount = FpgaTestNCnt(100);

    if (dwFpgaTestErrorCount != 0)
    {
        //lapneed
        LAPInsert(FILE_FPGACTL, USP_UHF_FPGA_TEST_ABNORAML, dwFpgaTestErrorCount, 0);

        for (;;)
        {
            printf("UHF: dwFpgaBusTestErrorCount = %10u \r\n", dwFpgaTestErrorCount);
            sleep(1);
        }
    }

    return USP_SUCCESS;
}
/**********************************************************************
* �������ƣ� WORD32 uhf_readFpgaFrame(WORD16 *pwReadLen)
* ���������� ��ȡfpga fifo�е����� �ŵ�ȫ�ֻ�������
* ��������� ��
* ��������� WORD16 *pwReadLen��
* �� �� ֵ��     ��ȡ�ɹ�����0 ���󷵻ش�����
* ����˵����  ��
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2010\12\1     V04.00.00      zdx      ����
***********************************************************************/
#define UHF_FPGA_NEED_RESET_TIME (100)
WORD32 uhf_readFpgaFrame(WORD16 *pwReadLen)
{
    WORD32 dwCount = 0;
    int iProcErrNo = 0;
    WORD32 dwRetVal = USP_SUCCESS;
    WORD16 wIndex = 0;
    static WORD32 dwFpgaReadErrorTime = 0;

    if (NULL == pwReadLen)
    {
        uhfDevPrint("pwReadLen is NULL\r\n");
        LAPInsert(FILE_FPGACTL, USP_UHF_RCVED_PARA_ERROR, 0 , 0);
        return USP_UHF_FPGA_NO_FRAME;
    }

#if 1
    if(g_dwUSPUhfVirTagWorkFlag == TRUE)
    {
        dwRetVal = usp_vir_fpga_readFrame((BYTE *)g_awFpgaReadBuf, FPGA_BUF_BYTE_SIZE, &dwCount);
    }
    else
    {
        dwRetVal = usp_vir_fpga_readFrame((BYTE *)g_awFpgaReadBuf, FPGA_BUF_BYTE_SIZE, &dwCount);
    }
#else
#if FPGA_SWTICH

    dwRetVal = usp_drv_fpga_read_frame((BYTE *)g_awFpgaReadBuf, FPGA_BUF_BYTE_SIZE, &dwCount);
#endif
#endif

    uhfDevPrint("UHF: Fpga read Frame  %d  Bytes\r\n", dwCount);

    if (dwRetVal == USP_SUCCESS)
    {
        if (dwCount == 0)
        {
            *pwReadLen = 0;
            UHFErrorPrint("UHF read: no frame, wait for interrupt notification \r\n");
            LAPInsert(FILE_FPGACTL, USP_UHF_FPGA_NO_FRAME, iProcErrNo, 0);
            dwRetVal = USP_UHF_FPGA_NO_FRAME;
        }
        else
        {
            *pwReadLen = (WORD16)dwCount / 2; /*�����㷵�ص����ֽڵĳ���*/

            if (TRUE)//usp_oss_isPrintf(USP_UHF_TNO, USP_OSS_PRINTF_LEVEL_DEBUG))
            {
                uhfDevPrint("Fpga Fifo 0x");

                for (wIndex = 0; wIndex < *pwReadLen; wIndex++)
                {
                    uhfDevPrint("%04x ", g_awFpgaReadBuf[wIndex]);
                }

                uhfDevPrint("\r\n");
            }
        }
    }
    else
    {
        uhfDevPrint("Rcv Frame error\r\n");
        dwFpgaReadErrorTime++;

        if (dwFpgaReadErrorTime > UHF_FPGA_NEED_RESET_TIME)
        {
            dwFpgaReadErrorTime = 0;

        }

        LAPInsert(FILE_FPGACTL, dwRetVal, iProcErrNo, 0);
    }

    return dwRetVal;

}

/**********************************************************************
* �������ƣ� uhf_EpcStartProcAndTimer
* ���������� ����Fpga��ʼepc���̣���������ʱ��ʱ��
* ��������� wCmdType  ������
* �����������
* �� �� ֵ��    ��
* ����˵����������ΪFPGAһ��ִ����ϲ��ȴ�RCP����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2012\3\16   RUP     zdx      ����
***********************************************************************/

void uhf_EpcStartProcAndTimer(WORD16 wCmdType)
{
    uhf_startRcpTimer(FPGA_TIMEOUT_INTERVAL_IN_MS);
    StartFpgaProc(wCmdType);
}

/**********************************************************************
* �������ƣ� uhf_EpcStartLlrpProcAndTimer
* ���������� ����Fpga��ʼepc llrp���̣���������ʱ��ʱ��
* ��������� wCmdType  ������
* �����������
* �� �� ֵ��    ��
* ����˵����fpgaִ�����һ�κ�ȴ�rcp�·��µ�ָ�
                                 ֱ����㲻����ǩΪֹ
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2012\3\16   RUP     zdx      ����
***********************************************************************/

void uhf_EpcStartLlrpProcAndTimer(WORD16 wCmdType)
{
    uhf_startRcpTimer(FPGA_TIMEOUT_INTERVAL_IN_MS);
    StartFpgallrpProc(wCmdType);
}
/**********************************************************************
* �������ƣ�void StartFpgaProc(WORD16 wCmdType)
* ��������������FPGAЭ�鴦��
* ���������
          WORD16 wCmdType �������� ����㡢����д��kill�ȣ�
* ���������
* �� �� ֵ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static void StartFpgaProc(WORD16 wCmdType)
{
    if (0 != wCmdType)
    {
        /* ��д�������� */
        FpgaRegWrite(REG_CPU_CMD_type, wCmdType);

    }

    /* ����FPGAЭ�鴦�� */
    /*
    Bit0        cmd_indic
    cmd_indic��0����Ϊ1��CPU����MAC���ʼ����
    CPU��Ҫ��д�����������ͼĴ���֮��֮�����д���λ��
    */

    FpgaRegWrite(REG_CPU_CMD_Trig, 0x00);
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x01);
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x00);


}

/**********************************************************************
* �������ƣ�WORD32 StartFpgaLlrpProc(WORD32 dwCmdType)
* ��������������FPGAЭ�鴦��
* ���������
            WORD32 dwCmdType �� �������� ����㡢����д��kill�ȣ�
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
static void StartFpgallrpProc(WORD16 dwCmdType)
{

    WORD16   wCpuTrigRegValue = 0;

    if (0 != dwCmdType)
    {
        FpgaRegWrite(REG_CPU_CMD_type, dwCmdType);
    }

    /* ����FPGA  LLRPЭ�鴦�� */

    /*
     Bit3        cmd_indic
     cmd_indic��0����Ϊ1��CPU����MAC���ʼLLRP ����ģʽ
     CPU��Ҫ��д�����������ͼĴ���֮��֮�����д���λ��
    */
    wCpuTrigRegValue = FpgaRegRead(REG_CPU_LLRP_CMD_Trig);
    FpgaRegWrite(REG_CPU_LLRP_CMD_Trig, ((~wCpuTrigRegValue) & 0x1));

} /* WORD32 StartFpgaProc(WORD32 dwCmdType) */

/**********************************************************************
* �������ƣ�WORD32 uhf_StopMacProc(void)
* ����������ֹͣFPGAЭ�鴦��ʹ��������
* ���ʵı���
* �޸ĵı���
* ���������
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
WORD32 uhf_StopMacProc(void)
{
    /* Bit1    stop mac
                  stop mac�� 0����Ϊ1��CPU����MAC����ֹͣ���� */
    FpgaRegWrite(REG_CPU_CMD_Trig, 0);
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x02);

    usleep(2000);
    return 0;

} /* WORD32 uhf_StopMacProc(void) */

/**********************************************************************
* �������ƣ� void uhf_ProtoHwInit(void)
* ���������� ����EPC��ص�Э������Լ�FPGA��صĿ��ƼĴ���
* ��������� ��
* ��������� ��
* �� �� ֵ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*
***********************************************************************/
void uhf_ProtoHwInit(void)
{

    uhf_epc_setTimePara();
    FpgaRegWrite(REG_COUPLE_TYPE, 0x01);
    FpgaRegWrite(REG_MOD_EN, 0x01);
    FpgaRegWrite(REG_DAC_SLEEP, 0x00);
    FpgaRegWrite(REG_FPGA_SAM_VALID_SIGNAL_CONTROL, 0x0);

    /* �½�������ֵ����ϵ���Ĵ�����
        signal_down_thresh_reg_factor��16λ����ַ��0055H
       Bit15-bit9������
       Bit7-bit0��Factor
       ���������޵���ֵ�Ĵ�������Χ��1-128��
       ��������Ϊ28 */
    FpgaRegWrite(0x0055, 0x0020);

    /* ����������ֵ����ϵ���Ĵ�����
        signal_up_thresh_reg_factor��16λ����ַ��0093H
        Bit15-bit9������
        Bit7-bit0��Factor
        ���������޵���ֵ�Ĵ�������Χ��1-128��
        ��������Ϊ���½�����ͬ��ֵ */
    FpgaRegWrite(0x0093, 0x0020);

    FpgaRegWrite(noise_test_delay_val_h, 0x0);
    FpgaRegWrite(noise_test_delay_val_l, 0xcb20);
    FpgaRegWrite(noise_test_sampoint_cnt, 0xfa0);
    FpgaRegWrite(noise_test_adj_val, 0x200); //0x40);

    FpgaRegWrite(G2_write_rcv_delay_val_h, WRITE_RCV_DELAY_H);
    FpgaRegWrite(G2_write_rcv_delay_val_l, WRITE_RCV_DELAY_L);

    FpgaRegWrite(REG_fir_sig_down_thresh_factor, 0x0040);
    FpgaRegWrite(REG_fir_sig_up_thresh_factor, 0x0040);

    FpgaRegWrite(REG_forward_filter_enable, 0x0000);
    FpgaRegWrite(REG_backward_filter_enable, 0x0003);


    FpgaRegWrite(REG_FPGA_AGC_VALUE, 0x000f); //AGC control value

    /*Ĭ������´�RSSI��FPGA���ʱ��RSSIֵͨ��fifo ����*/
    FpgaRegWrite(FPGA_RSSI_SWITCH, 0x1);

}
/**********************************************************************
* �������ƣ� uhf_epc_setTimePara
* ���������� ����EPC��ص�ʱ�����
* ��������� ��
* ��������� ��
* �� �� ֵ��
* ����˵�������е�tֵ��Ҫ����Э���Ӳ������Э�̺�ȷ��
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*
***********************************************************************/
#define FORWARD_FILTE_CLOSE 0x0

void uhf_epc_setTimePara(void)
{
    WORD16 wData = 0;
    WORD32 dwData = 0;

    /* EPC G2Э���ǰ����·���ʼĴ��� */
    FpgaRegWrite(G2_forlink_v, (WORD16)g_tEpcCtrl.RTDataRate);

    /* EPC G2Э��ķ�����·�������ʼĴ��� */
    if (g_tEpcCtrl.TRDataRate < 640)
    {
        wData = (WORD16)(g_tEpcCtrl.TRDataRate);
    }
    else /* ����ʵ�־�������,�����̨�趨640,ʵ���ϵ�FPGA��ʹ�÷������ʲ���640 */
    {
        wData = (WORD16)(g_tEpcCtrl.RealTRDataRate);
    }

    FpgaRegWrite(G2_reverse_link_v, wData);

    /*  EPC G2Э��ǰ��ǰ�������ͼĴ��� */
    wData = (WORD16)((g_tEpcCtrl.wRTcal_div_Tari << 5) | g_tEpcCtrl.wTRcal_div_halfTari);
    FpgaRegWrite(G2_forward_preamble_type, wData);

    /* EPC G2Э���T1ֵ�Ĵ���, ����T1Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=160. */

    wData = (WORD16)((g_tEpcCtrl.T1min - 10) * (FPGA_FREQUENCE / 1000000));
    FpgaRegWrite(G2_t1_value, wData);

    /* EPC G2Э���T2ֵ�Ĵ���, ����T2Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=130 */

    wData = (WORD16)(g_tEpcCtrl.T2min * 2 * (FPGA_FREQUENCE / 1000000)); //���ñȱ�׼t2min�Դ��ʱ�����

    if (g_tEpcCtrl.TRDataRate >= 320)
    {
        wData = (WORD16)(0); //(g_tEpcCtrl.T2min * 1 * (26000000/1000000)); // decrease T2 config to avoid tag repeat
    }

    FpgaRegWrite(G2_t2_value, wData);

    /* EPC G2Э���T4ֵ�Ĵ���, ����T4Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=130 */
    wData = (WORD16)(g_tEpcCtrl.T4min * 2 * (FPGA_FREQUENCE / 1000000));   //data
    FpgaRegWrite(G2_t4_value, wData);

    /* EPC G2Э���з�����·����ǰ����ռ�õ�ʱ��ֵ�Ĵ��� */
    /* û�п����Ƿ�ʹ����TRext, */
    if (g_tEpcCtrl.Query.M == EPC_QUERY_M_1)
    {
        FpgaRegWrite(G2_tag_code_type, 0x0); //00��FM0
        g_tEpcCtrl.Query.TRext = EPC_QUERY_TREXT_USE_PILOT_TONE; //for ͬ������㷨
    }
    else
    {
        FpgaRegWrite(G2_tag_code_type, 0x1); //01��Miller
    }

    if (g_tEpcCtrl.Query.M == EPC_QUERY_M_1)
    {
        //FM0
        //dwData = (WORD32)(((12) * g_tEpcCtrl.wTpri) + (6 * g_tEpcCtrl.wTpri));  //FM0 ǰ����ʱ��
        //dwData = dwData + (6 * g_tEpcCtrl.wTpri);//�Ӵ�һ������
        dwData = (WORD32)(12 + 6);   //FM0 ǰ����ʱ��
        dwData = dwData + 6 + 6; //�Ӵ�һ������
        dwData = dwData * g_tEpcCtrl.wTpri;
    }
    else
    {
        //Miller
        dwData = (WORD32)(16 + 6);  //Miller ǰ�����볤
        dwData = dwData + (6);       //�Ӵ�һ������
        dwData = dwData * (g_tEpcCtrl.wTpri * (1 << g_tEpcCtrl.Query.M));
    }


    dwData = dwData * (FPGA_FREQUENCE / 1000000); /* FPGA��Ƶ�ʳ���us */
    FpgaRegWrite(G2_reverse_preamble_t_val, (dwData & 0xffff));
    FpgaRegWrite(G2_reverse_preamble_t_val_h, ((dwData >> 16) & 0xffff));

    FpgaRegWrite(REG_forward_filter_enable, FORWARD_FILTE_CLOSE);
}

/**********************************************************************
* �������ƣ�void uhf_EpcSetSelectPara()
* ����������дFPGA��select��������Ĵ�����
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
void uhf_EpcSetSelectPara(WORD16 wTruncate, WORD16 wMemBank,
                          WORD16 wAction, WORD16 wTarget)
{
    WORD16 wData;
    wData = ((wTruncate & 0x1) << 8) | ((wMemBank & 0x3) << 6)
            | ((wAction & 0x7) << 3) | ((wTarget & 0x7) << 0);
    uhfDevPrint("MemBank %d, Action %d wTarget %d truncate %d\r\n",
                wMemBank, wAction, wTarget, wTruncate);
    FpgaRegWrite(REG_G2_Select_Para, wData);

} /* void FpgaTestRegWrite(WORD16 wData) */

/**********************************************************************
* �������ƣ�void uhf_EpcSetMacQueryPara(void)
* ����������дFPGA��Query��������Ĵ�����
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
void uhf_EpcSetMacQueryPara(WORD16 wQ, WORD16 wM, WORD16 wDR)
{
    WORD16 wData = 0;
    WORD16 wTRext = 0;

    if (wM == EPC_QUERY_M_1)
    {
        wTRext = EPC_QUERY_TREXT_USE_PILOT_TONE;
    }
    else
    {
        wTRext = EPC_QUERY_TREXT_NO_PILOT_TONE;
    }

    wData = ((wQ & 0xF) << 9)/*Q*/
            | ((EPC_QUERY_TARGET_B & 0x1) << 8)/*Target*/
            | ((0x0 & 0x3) << 6)/*session*/
            | ((0x0 & 0x3) << 4)/*sel*/
            | ((wTRext & 0x1) << 3)/*TRext*/
            | ((wM & 0x3) << 1)/*M*/
            | ((wDR & 0x1) << 0);/*DR*/
    FpgaRegWrite(REG_G2_Query_Para, wData);

} /* void uhf_EpcSetMacQueryPara(void) */

/**********************************************************************
* �������ƣ�WORD32 FpgaTest(void)
* ����������FPGA���ܲ��Ժ��������������߲��ԡ�
* ���ʵı���
* �޸ĵı���
* ���������
* ���������WORD32
                0��  FPGA��������
             ������  FPGA������������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static WORD32 FpgaTest(void)
{
    WORD32 dwIndex;
    WORD16 wWriteData;
    WORD16 wReadData;
    WORD32 dwReadCount = 0;
    WORD32 dwTestResult = 0;

    /* FPGA���� */
    for (dwIndex = 0 ; dwIndex < FPGA_DATA_BUS_WIDTH; dwIndex++)
    {
        wWriteData = (0x1 << dwIndex);

        FpgaRegWrite(REG_Fpga_test, wWriteData);

        for (dwReadCount = 0; dwReadCount < 3; dwReadCount++)
        {
            wReadData = FpgaRegRead(REG_Fpga_test);
            wReadData = ~wReadData;

            if (wWriteData != wReadData)
            {
                printf("fpga test error write is 0x%04x, read is 0x%04x\r\n", wWriteData, wReadData);
                dwTestResult = 1;
            }

        }

        wWriteData = ~wWriteData;
        FpgaRegWrite(REG_Fpga_test, wWriteData);

        for (dwReadCount = 0; dwReadCount < 3; dwReadCount++)
        {
            wReadData = FpgaRegRead(REG_Fpga_test);
            wReadData = ~wReadData;

            if (wWriteData != wReadData)
            {
                printf("fpga test error�� write is 0x%04x, read is 0x%04x\r\n", wWriteData, wReadData);
                dwTestResult = 1;
            }
        }

    }

    /*FpgaTest success*/

    return (dwTestResult);

} /* WORD32 FpgaTest(void) */
/**********************************************************************
* �������ƣ�FpgaTestNCnt
* ����������fpga���ԼĴ�������֤����
* ���������
* ���������WORD32
                0��  FPGA��������
             ������  fpga����������������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

static WORD32 FpgaTestNCnt(WORD32 dwTestCnt)
{
    WORD32 dwIndex;
    WORD32 dwFpgaTestOkCount = 0;
    WORD32 dwFpgaTestErrorCount = 0;
    WORD32 dwFpgaTotalTestCount = 0;

    time_t timep;
    time(&timep);
    printf("UHF: Fpga bus Test begin  at :%s \r\n", ctime(&timep));

    /* FPGA���ܲ��� ���Բ�ͨ��ϵͳ����*/
    for (dwIndex = 0; dwIndex < dwTestCnt; dwIndex++)
    {
        if (0 == FpgaTest())
        {
            dwFpgaTestOkCount++;
        }
        else
        {
            dwFpgaTestErrorCount++;
            sleep(2);
        }

        if (dwIndex % 1000 == 0)
        {
            printf(".");
        }
    }

    dwFpgaTotalTestCount += dwTestCnt;
    printf("UHF: dwFpgaTotalTestCount = %10u \r\n", dwFpgaTotalTestCount);
    printf("UHF: dwFpgaTestOkCount    = %10u \r\n", dwFpgaTestOkCount);
    printf("UHF: dwFpgaTestErrorCount = %10u \r\n", dwFpgaTestErrorCount);
    sleep(1);

    return dwFpgaTestErrorCount;

}

/**********************************************************************
* �������ƣ�uhf_epc_setSelectMask
* ��������������select�����mask�����ֵ
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
#define ROUND_BITS_TO_WORDS(BitNum)  (((BitNum) + 15)/16)

void uhf_epc_setSelectMask(WORD16 wAddr, WORD16 wMaskBitLen, WORD16 *pwData)
{
    WORD32 dwIndex;
    WORD16 wEbvAddr = 0;

    /* ����127 �ڶ���EBV��Ҫ��ֵ ,���½�ֵת��ΪEBV ��ʾ*/
    wEbvAddr = ((wAddr >> 7) > 0) ? 0x8000 : 0;
    wEbvAddr = ((wAddr << 1) & 0x7f00) | (wAddr & 0x7f) | wEbvAddr;

    FpgaRegWrite(REG_G2_Select_Addr, wEbvAddr);
    RcpFpgaPrint("MaskBit EbvPointer %x ", wEbvAddr);
    /* ע����Ҫ��չEBV */

    /* MaskBitLength */
    FpgaRegWrite(REG_G2_Select_Mask_BitLen, wMaskBitLen);
    RcpFpgaPrint("MaskBitLength %d\r\nMaskData ", wMaskBitLen);


    /* select����MASK�Ĵ��� */
    for (dwIndex = 0; dwIndex < ROUND_BITS_TO_WORDS(wMaskBitLen); dwIndex++)
    {
        FpgaRegWrite(REG_G2_Select_Mask_Start + dwIndex, pwData[dwIndex]);
        RcpFpgaPrint(" %4x ", pwData[dwIndex]);
    }

    RcpFpgaPrint("\r\n");

}

/**********************************************************************
* �������ƣ�void SetEpcReadPara(T_LLRPC1G2Read *ptEpcReadPara)
* ��������������fpga������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
void uhf_EpcSetMacReadPara(WORD16 wMemBank, WORD16 wAddr, WORD16 wWordCount)
{
    /* read����MemBank�Ĵ��� */
    FpgaRegWrite(REG_G2_Read_Membank, (WORD16)(wMemBank & 0x3));
    RcpFpgaPrint("ptFpgaEpcRead->wMemBank:    0x%04x \r\n", wMemBank);

    /* read�����ַ�Ĵ��� */    /* ע����Ҫ��չEBV */
    FpgaRegWrite(REG_G2_Read_Addr, (wAddr & 0x7f));
    RcpFpgaPrint("ptFpgaEpcRead->wWordPointer: 0x%04x \r\n", wAddr);

    /* read������ָ����Ĵ���*/
    FpgaRegWrite(REG_G2_Read_WordCount, wWordCount);
    RcpFpgaPrint("ptFpgaEpcRead->wReadWordsLen: 0x%04x \r\n", wWordCount);

} /* static int uhf_EpcSetMacReadPara(T_FpgaEpcRead *ptFpgaEpcRead) */

/**********************************************************************
* �������ƣ�void SetEPCWrtRtyTime(WORD16 wWriteLength)
* ��������������д��ĳ����ж���Ҫ���ԵĴ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/6/17        V3.00.00      ZDX         �޸�
************************************************************************/
void SetEPCWrtRtyTime(WORD16 wWriteLength)
{
    WORD16 wFpgaRetryRegVal = 0;
    wFpgaRetryRegVal = FpgaRegRead(REG_FPGA_WRITE_READ_RETRY_TIME);

    if (wWriteLength <= 5)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x03;
    }
    else if (wWriteLength <= 16)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x08;
    }
    else if (wWriteLength < 32)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x10;
    }
    else
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x20;
    }

    FpgaRegWrite(REG_FPGA_WRITE_READ_RETRY_TIME, wFpgaRetryRegVal);
}


/**********************************************************************
* �������ƣ�void uhf_EpcSetMacWritePara(WORD16 wMemBank,WORD16 wAddr,WORD16 wWrWordLen,BYTE *ucdata)
* ����������
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
void uhf_EpcSetMacWritePara(WORD16 wMemBank, WORD16 wAddr, WORD16 wWrWordLen, BYTE *ucdata)
{
    WORD16 wIndex = 0;
    /* Write����MemBank�Ĵ��� */
    FpgaRegWrite(REG_G2_Write_Membank, (WORD16)(wMemBank & 0x3));
    RcpFpgaPrint("MemBank %d", wMemBank);

    /* Write�����ַ�Ĵ��� */
    FpgaRegWrite(REG_G2_Write_Addr, (wAddr & 0x7f));
    RcpFpgaPrint("WordPointer  %d", wAddr);
    /* ע����Ҫ��չEBV */

    /* Write������ָ����Ĵ��� */
    FpgaRegWrite(REG_G2_Write_WordCount, wWrWordLen);
    RcpFpgaPrint("WriteWordsLen  %d \r\nWrite Data", wWrWordLen);

    /*����FPGA�����󣬲�ͬ��д����д���Դ�����ͬ�����д��ĳɹ���*/
    SetEPCWrtRtyTime(wWrWordLen);

    /*��������FPGA
    �Ŀ��Ƴ�ʱʱ��*/
    FpgaRegWrite(G2_write_rcv_delay_val_h, 0x0007);
    FpgaRegWrite(G2_write_rcv_delay_val_l, 0x6590);

    /* write�������ݼĴ��� */
    for (wIndex = 0; wIndex < wWrWordLen; wIndex++)
    {
        FpgaRegWrite(REG_G2_BlockWrite_Data_Start + wIndex,
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
        RcpFpgaPrint("%04x",
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
    }

    RcpFpgaPrint("\r\n");
} /* static int uhf_EpcSetMacWritePara(T_FpgaEpcWrite *ptFpgaEpcWrite) */

/**********************************************************************
* �������ƣ�static int uhf_EpcSetMacWritePara(T_FpgaEpcRead *ptFpgaEpcWrite)
* ����������
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
void uhf_EpcSetMacBlockWritePara(WORD16 wMemBank, WORD16 wAddr, WORD16 wWrWordLen, BYTE *ucdata)
{
    WORD16 wIndex = 0;
    WORD32 dwBlockWriteDelayTime = EPC_BLOCK_START_DELAY;
    /* Write����MemBank�Ĵ��� */
    FpgaRegWrite(REG_G2_Block_Write_Membank, (WORD16)(wMemBank & 0x3));
    RcpFpgaPrint("MemBank %d", wMemBank);

    /* Write�����ַ�Ĵ��� */
    FpgaRegWrite(REG_G2_Block_Write_Adress, (wAddr & 0x7f));
    RcpFpgaPrint("WordPointer  %d", wAddr);
    /* ע����Ҫ��չEBV */

    /* Write������ָ����Ĵ��� */
    FpgaRegWrite(REG_G2_Block_Write_Length, wWrWordLen);
    RcpFpgaPrint("WriteWordsLen  %d \r\nWrite Data ", wWrWordLen);

    /* write�������ݼĴ��� */
    for (wIndex = 0; wIndex < wWrWordLen; wIndex++)
    {
        FpgaRegWrite(REG_G2_BlockWrite_Data_Start + wIndex,
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
        RcpFpgaPrint("%04x",
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
    }

    RcpFpgaPrint("\r\n");


    wWrWordLen = (wWrWordLen > 1) ? (wWrWordLen - 1) : 0;
    dwBlockWriteDelayTime += ((FPGA_FREQUENCE/1000)* wWrWordLen * 2);
    FpgaRegWrite(G2_write_rcv_delay_val_h, (WORD16)(dwBlockWriteDelayTime >> 16));
    FpgaRegWrite(G2_write_rcv_delay_val_l, (WORD16)(dwBlockWriteDelayTime & 0xffff));

} /* static int uhf_EpcSetMacWritePara(T_FpgaEpcWrite *ptFpgaEpcWrite) */

/**********************************************************************
* �������ƣ�uhf_EpcSetMacLockPara
* ��������������epc��������
* �����������������
* �����������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

void uhf_EpcSetMacLockPara(WORD32 dwLockPayLoad)
{

    /* payload_mask */
    FpgaRegWrite(REG_G2_Loc_payload_mask, ((dwLockPayLoad >> 10) & 0x3FF));
    RcpFpgaPrint("G2_Loc_payload_mask   = 0x%04x \r\n", (dwLockPayLoad >> 10) & 0x3FF);

    /* payload_Action */
    FpgaRegWrite(REG_G2_Loc_payload_Action, (dwLockPayLoad & 0x3FF));
    RcpFpgaPrint("G2_Loc_payload_Action = 0x%04x \r\n", dwLockPayLoad & 0x3FF);

}

/**********************************************************************
* �������ƣ�uhf_EpcSetMacAccessPara
* ��������������epc��������������������ʱ��Ҫ
* ���������������Կ
* �����������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

void  uhf_EpcSetMacAccessPara(WORD32 dwAccessPassword)
{
    /* wAccessPassHigh */
    FpgaRegWrite(REG_G2_Access_Password_h, (WORD16)((dwAccessPassword >> 16) & 0xffff));
    /* wAccessPassLow */
    FpgaRegWrite(REG_G2_Access_Password_L, (WORD16)(dwAccessPassword & 0xffff));
    RcpFpgaPrint("uhf: access password   = 0x%x \r\n", dwAccessPassword);

}
/**********************************************************************
* �������ƣ�uhf_EpcSetMacKillPara
* ��������������epcɱ������ɱ��ʱ��Ҫ
* ���������ɱ����Կ
* ��������� ��
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

void uhf_EpcSetMacKillPara(WORD32 dwKillPassword)
{

    RcpFpgaPrint("UHF: wKillPassWord = 0x%04x \n", dwKillPassword);
    /* wKillPassHigh */
    FpgaRegWrite(REG_G2_Kill_Password_h, (WORD16)((dwKillPassword >> 16) & 0xffff));

    /* wKillPassLow */
    FpgaRegWrite(REG_G2_Kill_Password_L, (WORD16)(dwKillPassword & 0xffff));


    FpgaRegWrite(REG_G2_Kill_RFU, 0x0000);

}
/**********************************************************************
* �������ƣ�uhf_EpcSetMacKillPara
* ��������������epc �����Ĳ���
* ���������ucReadLock  1���������״̬��0��������
* ��������� ��
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
void uhf_EpcSetMacBlockLock(BYTE ucReadLock, WORD16 wMemBank, WORD16 wBlockPtr,
                            WORD16 wBlockRang, BYTE *ucdata)
{
    WORD16      wBlockLockCmd = 0;
    WORD16      wIndex = 0;
    wBlockLockCmd = wMemBank & 0x3;

    if (ucReadLock == 1)
    {
        wBlockLockCmd |= 0x4;
    }

    /* membank */
    FpgaRegWrite(REG_G2_Block_Lock_MemBank, wBlockLockCmd);
    /* ע����Ҫ��չEBV */
    RcpFpgaPrint("UHF: REG_G2_Block_Lock_MemBank  = %d \r\n", wBlockLockCmd);

    /* wBlockPtr*/
    FpgaRegWrite(REG_G2_Blcokpermalock_EVB, wBlockPtr);
    RcpFpgaPrint("UHF: REG_G2_Blcokpermalock_EVB  = %d \r\n", wBlockPtr);

    /* blankrange */
    FpgaRegWrite(REG_G2_Blockpermalock_Blockrange, wBlockRang);
    RcpFpgaPrint("UHF: REG_G2_Blockpermalock_Blockrange  = %d \r\n", wBlockRang);

    if (ucReadLock == 1)
    {
        RcpFpgaPrint("Mask is");

        for (wIndex = 0; wIndex < wBlockRang; wIndex++)
        {
            FpgaRegWrite(REG_G2_BlockWrite_Data_Start + wIndex,
                         (WORD16)((((WORD16)ucdata[wIndex * 2]) << 8) | ucdata[wIndex * 2 + 1]));
            RcpFpgaPrint("0x%02x%02x", ucdata[wIndex * 2], ucdata[wIndex * 2 + 1]);
        }

        RcpFpgaPrint("\r\n");
    }
}


/**********************************************************************
* �������ƣ�uhf_EpcSetMacReadLock
* �����������������ɵĶ����������������ö������Ŀ�
* ���������wReadLockMask�����������룬���е�λΪ0�������
                                1 ��������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/1/28    V0.0.1          chenjie        ����
************************************************************************/
void uhf_EpcSetMacReadLock(WORD16 wReadLockMask)
{
    /*Read Lock д������*/
    FpgaRegWrite(REG_G2_Blockreadlock_mask, wReadLockMask);

    RcpFpgaPrint("UHF: REG_G2_Blockreadlock_mask  = 0x%04x \n", wReadLockMask);
}

/**********************************************************************
* �������ƣ�uhf_EpcSetMacIPJQT
* ��������������impinj��ǩfpga��صļĴ���
* ���������ucReadOrWrite 0 �����1����д
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2011/9/14    V4.60.00      zdx         ����
************************************************************************/
void uhf_EpcSetMacIPJQT(BYTE ucReadOrWrite, BYTE ucPersistence,
                        BYTE ucQT_SR, BYTE ucQT_MEM)
{
    FpgaRegWrite(REG_I_QT_WR_PER, (((ucReadOrWrite & 0x1) << 1) | (ucPersistence & 0x1)));
    FpgaRegWrite(REG_I_QT_PAYLOAD, (((ucQT_SR & 0x1) << 15) | ((ucQT_MEM & 0x1) << 14)));
    uhfDevPrint("uhf:readorwrite %d persistence %d SR %d Memory map %d\r\n",
                ucReadOrWrite, ucPersistence, ucQT_SR, ucQT_MEM);

}

/**********************************************************************
* �������ƣ� void uhf_ISO6BSetProtoPara(T_ISO6BCtl *ptISO6BCtrl)
* ���������� ����FPGA 6BЭ�����
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
#define FORWARD_FILTE_OPEN 0x3
void uhf_ISO6BSetProtoPara(WORD16 wFowardLinkDataRate, WORD16 wReverseLinkDataRate,
                           WORD16 wT1_value, WORD16 wdelimeters, WORD16 wISO6BModulateDepth)
{
    SetProtoType(UHF_PROTO_TYPE_ISO6B);
    /* ǰ����������40K����������Ϊ40K */
    FpgaRegWrite(G2_forlink_v, wFowardLinkDataRate);
    FpgaRegWrite(G2_reverse_link_v, wReverseLinkDataRate);

    /* ���ճ�ʱ������T1����Ҫ���õĽϴ�0x5000�������� */
    FpgaRegWrite(G2_t1_value, wT1_value);/*guanyi modify 2007.09.26*/

    /* ����ı�����EPCЭ�鹲�õļĴ������ڿ�ʼEPCЭ��ǩ��Ҫ��������ΪEPC��Ҫ�Ĳ��� */

    /* MACЭ���Ĵ��� */
    FpgaRegWrite(ISO18000_6B_forward_preamble_type, wdelimeters);

    /*��FPGAǰ���˲���*/
    FpgaRegWrite(REG_forward_filter_enable, FORWARD_FILTE_OPEN);

}
/**********************************************************************
* �������ƣ� uhf_ISO6BSetUIDPara
* ���������� ����ISOЭ�������uid��
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/

void uhf_ISO6BSetUIDPara(BYTE *pucUID)
{
    FpgaRegWrite(REG_18000B_access_UID_3, ((((WORD16)pucUID[0]) << 8) | ((WORD16)pucUID[1])));
    FpgaRegWrite(REG_18000B_access_UID_2, ((((WORD16)pucUID[2]) << 8) | ((WORD16)pucUID[3])));
    FpgaRegWrite(REG_18000B_access_UID_1, ((((WORD16)pucUID[4]) << 8) | ((WORD16)pucUID[5])));
    FpgaRegWrite(REG_18000B_access_UID_0, ((((WORD16)pucUID[6]) << 8) | ((WORD16)pucUID[7])));
}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetRead8BytePara
* ���������� д��fpga��8bye���������
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/

void uhf_ISO6BSetRead8BytePara(WORD16 wStartAddr)
{
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_READ);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);
}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetReadVrblPara
* ���������� д��fpga�б䳤���������
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/

void uhf_ISO6BSetReadVrblPara(WORD16 wStartAddr, WORD16 wFpgaReadLen)
{
    /* ��д���ζ���ز���*/
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_Variable_READ);
    /*Э��涨������ʱ��ȡ���ȼ�1*/
    FpgaRegWrite(REG_18000B_Read_Variable_Len, wFpgaReadLen);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);
}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetWriteBytePara
* ���������� д��fpga����ͨд����Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_ISO6BSetWriteBytePara(WORD16 wStartAddr, BYTE *pucWriteData)
{
    /* ��д����д��ز���*/
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_WriteByte);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);
    /* ��д����д����*/
    FpgaRegWrite(REG_18000B_write_data_0, ((WORD16)*pucWriteData));
    FpgaRegWrite(REG_18000B_write_continue, 0x0001);

}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetWrite4BytePara
* ���������� д��fpga��4�ֽ�д����Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_ISO6BSetWrite4BytePara(WORD16 wStartAddr, BYTE *pucWriteData)
{
    /* ��д����д��ز���*/
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_Write4Bytes);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);
    /* ��д����д����*/
    FpgaRegWrite(REG_18000B_write_data_0, ((((WORD16)pucWriteData[2]) << 8) | ((WORD16)pucWriteData[3])));
    FpgaRegWrite(REG_18000B_write_data_1, ((((WORD16)pucWriteData[0]) << 8) | ((WORD16)pucWriteData[1])));
    /* ��д����*/
    FpgaRegWrite(REG_18000B_write_bytemask, 0xff);
    FpgaRegWrite(REG_18000B_write_continue, 0x0001);
}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetLockBytePara
* ���������� д��fpga���ֽ���������Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/

void uhf_ISO6BSetLockBytePara(WORD16 wStartAddr)
{
    /* ��д����д��ز���*/
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_Lock);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);

}

/**********************************************************************
* �������ƣ�uhf_ISO6BSetLockQueryPara
* ���������� д��fpga���ֽ�������ѯ����Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_ISO6BSetLockQueryPara(WORD16 wStartAddr)
{
    /* ��д����д��ز���*/
    FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_direct_access);
    FpgaRegWrite(REG_18000B_accesscmd_type, ISO18000B_accesscmd_type_LockQuery);
    FpgaRegWrite(REG_18000B_access_Address, wStartAddr);
}


/**********************************************************************
* �������ƣ�uhf_ISO6BSetInventoryPara
* ���������� д��fpga���������Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_ISO6BSetInventoryPara(WORD16 wISOCommand, WORD16 wMaskByteAddr,
                               WORD16 wInventQ, WORD16 wMask, WORD16 wDataReadStartAddr, BYTE *pucMaskData)
{
    WORD16 wIndex = 0;

    SetProtoType(UHF_PROTO_TYPE_ISO6B);
    FpgaRegWrite(REG_18000B_selectcmd_type, wISOCommand);
    FpgaRegWrite(REG_18000B_sel_address, wMaskByteAddr);
    FpgaRegWrite(REG_FPGA_ISO6B_INVENTQ, wInventQ);

    FpgaRegWrite(REG_18000B_sel_mask, (wMask & 0x00ff));
    uhfDevPrint("ISOCommand %d MaskByteAddr %d InventQ %d Mask %d\r\n",
                wISOCommand, wMaskByteAddr, wInventQ, wMask);

    /*��REG_18000B_accesscmd_type �Ĵ�������read_varible ���������
    	   ����Ϊ��0x51��FPGA�������ֵȥ�ж���������ĳ���
    	   ���������ʱ��Ҫ������0*/
    FpgaRegWrite(REG_18000B_accesscmd_type, 0x0);

    if (wISOCommand >= GROUP_SELECT_EQ_FLAGS)
    {
        FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_noinit_invent);
        /*��ʹ��FLAGAS��������Ҫд�������*/
        FpgaRegWrite(REG_18000B_sel_flags_Byte_data, (WORD16)pucMaskData[0]);
    }
    else
    {
        FpgaRegWrite(REG_18000B_cmd_mode, ISO18000B_cmd_mode_invent);
        /*��ʹ��������������Ҫд�������*/
        FpgaRegWrite(REG_18000B_sel_worddata_3, (((WORD16)(pucMaskData[0]) << 8) | pucMaskData[1]));
        FpgaRegWrite(REG_18000B_sel_worddata_2, (((WORD16)(pucMaskData[2]) << 8) | pucMaskData[3]));
        FpgaRegWrite(REG_18000B_sel_worddata_1, (((WORD16)(pucMaskData[4]) << 8) | pucMaskData[5]));
        FpgaRegWrite(REG_18000B_sel_worddata_0, (((WORD16)(pucMaskData[6]) << 8) | pucMaskData[7]));
    }

    uhfDevPrint("Mask data ");

    for (wIndex = 0; wIndex < 8; wIndex++)
    {
        uhfDevPrint("%2x", pucMaskData[wIndex]);
    }

    uhfDevPrint("\r\n");

    /*���µ�ַ��Ϊ��ȷ������㵽��ǩ����dataread����ʹ
    						��ǩ���뵽DATAExchange����ʱ��ȡ����ʼ��ַ*/
    FpgaRegWrite(REG_18000B_access_Address, wDataReadStartAddr);

}

//////////////////////
/// GB Operation  lxc
//////////////////////
/**********************************************************************
* �������ƣ�void SetGBWrtRtyTime(WORD16 wWriteLength)
* ��������������д��ĳ����ж���Ҫ���ԵĴ���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/17        V3.00.00      lxc        �޸�
************************************************************************/
void SetGBWrtRtyTime(WORD16 wWriteLength)
{
    WORD16 wFpgaRetryRegVal = 0;
    wFpgaRetryRegVal = FpgaRegRead(REG_GB_FPGA_WRITE_READ_RETRY_TIME);

    if (wWriteLength <= 5)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x03;
    }
    else if (wWriteLength <= 16)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x08;
    }
    else if (wWriteLength < 32)
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x10;
    }
    else
    {
        wFpgaRetryRegVal = (wFpgaRetryRegVal & (0xff00)) | 0x20;
    }

    FpgaRegWrite(REG_GB_FPGA_WRITE_READ_RETRY_TIME, wFpgaRetryRegVal);
}


/**********************************************************************
* �������ƣ� uhf_GBStartLlrpProcAndTimer
* ���������� ����Fpga��ʼepc llrp���̣���������ʱ��ʱ��
* ��������� wCmdType  ������
* �����������
* �� �� ֵ��    ��
* ����˵����fpgaִ�����һ�κ�ȴ�rcp�·��µ�ָ�
                                 ֱ����㲻����ǩΪֹ
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2012\3\16   RUP     zdx      ����
***********************************************************************/

void uhf_GBStartLlrpProcAndTimer(WORD16 wCmdType)
{
    uhf_startRcpTimer(FPGA_TIMEOUT_INTERVAL_IN_MS);
    StartFpgallrpProc(wCmdType);
}

/**********************************************************************
* �������ƣ� uhf_GB_setTimePara
* ���������� ����GB��ص�ʱ�����
* ��������� ��
* ��������� ��
* �� �� ֵ��
* ����˵�������е�tֵ��Ҫ����Э���Ӳ������Э�̺�ȷ��
* �޸�����2014.2        �汾��     �޸���  lxc        �޸�����
* -----------------------------------------------
*
***********************************************************************/

void uhf_GB_setTimePara(void)  // �ǲ�����Ҫ�����¹���Э����������ʱ��
{
    WORD16 wData = 0;
    WORD32 dwData = 0;

    /* GBЭ���ǰ����·���ʼĴ��� */ /* �������д0 ��1 */
    FpgaRegWrite(G2_forlink_v, (WORD16)g_tGBCtrl.RTDataRate);

    /* GBЭ��ķ�����·�������ʼĴ��� */
    if (g_tGBCtrl.TRDataRate < 640)
    {
        wData = (WORD16)(g_tGBCtrl.TRDataRate);
    }
    else /* ����ʵ�־�������,�����̨�趨640,ʵ���ϵ�FPGA��ʹ�÷������ʲ���640 */
    {
        wData = (WORD16)(g_tGBCtrl.RealTRDataRate);
    }
    FpgaRegWrite(G2_reverse_link_v, wData);

    /*  GB Э��ǰ��ǰ�������ͼĴ��� */ /* ���궨�� */
    //wData = (WORD16)((g_tGBCtrl.wRTcal_div_Tari << 5) | g_tGBCtrl.wTRcal_div_halfTari);
    //FpgaRegWrite(G2_forward_preamble_type, wData);

    /* GB Э���T1ֵ�Ĵ���, ����T1Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=160. */
    //wData = (WORD16)((g_tGBCtrl.T1min - 10) * (FPGA_FREQUENCE / 1000000));
     wData = (WORD16)(40 * (FPGA_FREQUENCE / 1000000));
    FpgaRegWrite(G2_t1_value, wData);

    /* GB Э���T2ֵ�Ĵ���, ����T2Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=130 */
    wData = (WORD16)(g_tGBCtrl.T2min * 2 * (FPGA_FREQUENCE / 1000000)); //���ñȱ�׼t2min�Դ��ʱ�����

    if (g_tGBCtrl.TRDataRate >= 320)
    {
        wData = (WORD16)(0); //(g_tGBCtrl.T2min * 1 * (26000000/1000000)); // decrease T2 config to avoid tag repeat
    }
    wData = (WORD16)(63 * (FPGA_FREQUENCE / 1000000));
    FpgaRegWrite(G2_t2_value, wData);

    /* GBЭ���T4ֵ�Ĵ���, ����T4Ϊ5uS�����ֵΪg_tRcpCtrl.dwFpgaClock��0.000005=130 */
    wData = (WORD16)(g_tGBCtrl.T4min * 2 * (FPGA_FREQUENCE / 1000000));   //data
    FpgaRegWrite(G2_t4_value, wData);
    
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// lxc
    /* GB Э���з�����·����ǰ����ռ�õ�ʱ��ֵ�Ĵ��� */
    /* û�п����Ƿ�ʹ����TRext, */
    if (g_tGBCtrl.Query.ucM == GB_QUERY_M_1)
    {
        FpgaRegWrite(G2_tag_code_type, 0x0); //00��FM0
        //TRext��ʾ�Ƿ����ǰ���ź�
        g_tGBCtrl.Query.ucTRext = EPC_QUERY_TREXT_USE_PILOT_TONE; //for ͬ������㷨
    }
    else
    {
        FpgaRegWrite(G2_tag_code_type, 0x1); //01��Miller
    }

    //����ǰ����ռ��ʱ��lxc
    if (g_tGBCtrl.Query.ucM == GB_QUERY_M_1)
    {
        //FM0
        //dwData = (WORD32)(((12) * g_tEpcCtrl.wTpri) + (6 * g_tEpcCtrl.wTpri));  //FM0 ǰ����ʱ��
        //dwData = dwData + (6 * g_tEpcCtrl.wTpri);//�Ӵ�һ������
        dwData = (WORD32)(12 + 8);   //FM0 ǰ����ʱ��
        dwData = dwData + 6 + 6; //�Ӵ�һ������
        dwData = dwData * g_tGBCtrl.wTpri;
    }
    else
    {
        //Miller
        dwData = (WORD32)(16 + 8);  //Miller ǰ�����볤
        dwData = dwData + (6);       //�Ӵ�һ������
        dwData = dwData * (g_tGBCtrl.wTpri * (1 << g_tGBCtrl.Query.ucM));
    }

    RcpFpgaPrint("G2_reverse_preamble_t_val :%d", dwData);
    dwData = dwData * (FPGA_FREQUENCE / 1000000);
    //FpgaRegWrite(G2_reverse_preamble_t_val, (dwData & 0xffff));
    //FpgaRegWrite(G2_reverse_preamble_t_val_h, ((dwData >> 16) & 0xffff));

    FpgaRegWrite(G2_reverse_preamble_t_val, 0xdc6);
    FpgaRegWrite(G2_reverse_preamble_t_val_h, 0x2);
    //ǰ���˲�ʹ�ܹر�
    FpgaRegWrite(REG_forward_filter_enable, FORWARD_FILTE_CLOSE);
}

/**********************************************************************
* �������ƣ�uhf_epc_setSelectMask
* ��������������select�����mask�����ֵ
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
///// �Ƿ���Ҫ����lxc
////  ��Ҫ���룬���Ҷ���һ������ָ��Ĵ���
////  EBV ��ʾָ����ʼ��ַ��������е�ָ����ͬ
////  sort����ָ��Ĵ�������ַ��C002H
void uhf_GB_setSelectMask(WORD16 wAddr, WORD16 wMaskBitLen, WORD16 *pwData)
{
    WORD32 dwIndex;
    WORD16 wEbvAddr = 0;//�¹����е�ָ��pointer

     wEbvAddr = wAddr;

    /* ����127 �ڶ���EBV��Ҫ��ֵ ,���½�ֵת��ΪEBV ��ʾ*/
    //wEbvAddr = ((wAddr >> 7) > 0) ? 0x8000 : 0;
    //wEbvAddr = ((wAddr << 1) & 0x7f00) | (wAddr & 0x7f) | wEbvAddr;

    // sort ��������Ĵ���
    // �¹���û���ر�ָ���Ƿ����EBV��ʽ������
    // дC002h������ָ��Ĵ���
    //lxc
    FpgaRegWrite(REG_GB_Sort_Cmd_Ptr, wEbvAddr); 
    RcpFpgaPrint("MaskBit Sort_Cmd_Ptr %x ", wEbvAddr);
    /* ע����Ҫ��չEBV */

    /* MaskBitLength */
    /* ?	sort����ȼĴ�������ַ��C003H   lxc */
    FpgaRegWrite(REG_GB_Sort_Cmd_BitLen, wMaskBitLen);
    RcpFpgaPrint("Sort_Cmd_BitLen %d\r\nMaskData ", wMaskBitLen);


    /* select����MASK�Ĵ��� */
    // sort��������Ĵ����������������DPRAM��ʽʵ�֣�
    // ��ַ�ռ�Ϊ10000H��1FFFFH���ܹ�128K����ַΪ10000H��ʵ��ռ��64�ֽ�
    //����ĳ��Ⱥ����ݵĳ������
    //lxc
    for (dwIndex = 0; dwIndex < ROUND_BITS_TO_WORDS(wMaskBitLen); dwIndex++)
    {
        FpgaRegWrite(REG_GB_Select_Mask_Start + dwIndex, pwData[dwIndex]);
        RcpFpgaPrint(" %4x ", pwData[dwIndex]);
    }

    RcpFpgaPrint("\r\n");

}

/**********************************************************************
* �������ƣ�void uhf_GBSetSelectPara()
* ����������дFPGA��select��������Ĵ�����
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1      lxc          ����
************************************************************************/

void uhf_GBSetSelectPara(WORD16 wRule,WORD16 wTarget, WORD16 wMemBank)
{
    WORD16 wData;
     /// ����ṹ�仯
     /// Bit15~Bit12  ����	Bit11~Bit10   ����	Bit9~Bit6   Ŀ��	Bit5~Bit0  �洢��lxc
    wData = (((wRule & 0x3) << 10) |( (wTarget & 0xF) << 6) | ((wMemBank & 0x3F) << 0));
    uhfDevPrint("wRule %d, wTarget %d wMemBank %d wData:0x%x\r\n",
                        wRule, wTarget, wMemBank,wData);
    //// sort��������Ĵ�������ַ��C001H  lxc
    wData=0x31;
    FpgaRegWrite(REG_GB_Sort_Para, wData);

} /* void FpgaTestRegWrite(WORD16 wData) */

/**********************************************************************
* �������ƣ�void uhf_GBSetMacQueryPara(void)
* ����������дFPGA��Query��������Ĵ�����
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
//��ǰ�и�Qֵ������û��Qֵ���������?
void uhf_GBSetMacQueryPara(BYTE ucM, BYTE ucReLinkRateDivisor)
{

   // ���� Bit15��Bit14
   // ����    Bit13��Bit10  => sel
   // �Ự   Bit9��Bit8   => session
   // Ŀ��     Bit7            => Target
   // TRext  Bit6                  => TRext
  //  ������·��������    Bit5��Bit2   => ucDr
   // ����ѡ��Bit1��Bit0      lxc    =>  ucM
    WORD16 wData = 0;
    BYTE       ucTRext = 1;
    BYTE       ucTarget = 1;
    BYTE       ucSession = 0;
    BYTE       ucSel = 0;

    if (ucM == GB_QUERY_M_1) 
    {
        ucTRext = GB_QUERY_TREXT_USE_PILOT_TONE;
    }
    else
    {
        ucTRext = GB_QUERY_TREXT_NO_PILOT_TONE;
    }

        wData = ((ucSel & 0xF) << 10) /*sel*/                 //((0x0 & 0x3) << 4) 
            | ((ucSession & 0x3) << 8)/*session*/            //((0x0 & 0x3) << 6)
            | ((ucTarget & 0x1) << 7)/*Target*/               //EPC_QUERY_TARGET_B in EPC
            | ((ucTRext & 0x1) << 6)/*TRext*/ 
            | ((ucReLinkRateDivisor & 0xF) << 2)/*DR*/ // ������·��������
            | ((ucM & 0x3) << 0);/*M*/                           // ����ѡ��

    uhfDevPrint("GBSetMacQueryPara wData = 0x%x\r\n",wData);

    
    ///  Query��������Ĵ�������ַ��C004H    lxc
    wData=0x4c;
    FpgaRegWrite(REG_GB_Query_Para, wData);

} /* void uhf_EpcSetMacQueryPara(void) */

/**********************************************************************
* �������ƣ� uhf_GBStartProcAndTimer
* ���������� ����Fpga��ʼGB���̣���������ʱ��ʱ��
* ��������� wCmdType  ������
* �����������
* �� �� ֵ��    ��
* ����˵����������ΪFPGAһ��ִ����ϲ��ȴ�RCP����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
*2014\2\16   RUP    lxc      ����
***********************************************************************/

void uhf_GBStartProcAndTimer(WORD16 wCmdType)
{
    uhf_startRcpTimer(FPGA_TIMEOUT_INTERVAL_IN_MS);
    StartFpgaProc(wCmdType);
}

/**********************************************************************
* �������ƣ�uhf_GBSetMacAccessPara
* ��������������GB��������������������ʱ��Ҫ
* ���������������Կ
* �����������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
/* need change */
void  uhf_GBSetMacAccessPara(WORD32 dwAccessPassword,BYTE ucMB,BYTE ucAccessType)
{
    WORD16 wData = 0;
    
    wData =(ucAccessType << 6) |( (ucMB & 0x3F) << 0) ;  
    FpgaRegWrite(REG_GB_Access_Membank, wData);

    /* wAccessPassHigh */
    FpgaRegWrite(REG_GB_Access_Password_H, (WORD16)((dwAccessPassword >> 16) & 0xffff));
    /* wAccessPassLow */
    FpgaRegWrite(REG_GB_Access_Password_L, (WORD16)(dwAccessPassword & 0xffff));

    RcpFpgaPrint("uhf: GB access password   = 0x%x \r\n", dwAccessPassword);
    
#if 0
    switch(ucAccessType){

        case ACCESS_READ:
            //��ȡ���ʿ���ȷ�0110���ٷ�0111
            //�����ȿ���ʵ��һ��д������Ҫ��fpga  Э��
            wData =((0b0111) << 10) | ((0b0110) << 6) |( (ucMB & 0x3F) << 0) ;
            break;

        case ACCESS_WRITE:
            //д����ʿ���ȷ�1000���ٷ�1001
            wData =((0b0111) << 10) | ((0b0110) << 6) |( (ucMB & 0x3F) << 0) ;
            break;
            
        case ACCESS_KILL:
            break;
            
        case ACCESS_LOCK;
            break;
            
        default:
            break;
    }

    FpgaRegWrite(REG_GB_Access_Membank, wData);
    /* wAccessPassHigh */
    FpgaRegWrite(REG_GB_Access_Password_H, (WORD16)((dwAccessPassword >> 16) & 0xffff));
    /* wAccessPassLow */
    FpgaRegWrite(REG_GB_Access_Password_L, (WORD16)(dwAccessPassword & 0xffff));
    RcpFpgaPrint("uhf: access password   = 0x%x \r\n", dwAccessPassword);
#endif
}

/**********************************************************************
* �������ƣ�void SetGBReadPara(BYTE ucMemBank, WORD16 wAddr, WORD16 wWordCount)
* ��������������fpga������
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
void uhf_GBSetMacReadPara(BYTE ucMemBank, WORD16 wAddr, WORD16 wWordCount)
{
    /* read����MemBank�Ĵ��� */
    FpgaRegWrite(REG_GB_Read_Membank, (WORD16)(ucMemBank & 0x3));
    RcpFpgaPrint("www ptFpgaGBRead->wMemBank:    0x%04x \r\n", ucMemBank);

    /* read�����ַ�Ĵ��� */    /* ע����Ҫ��չEBV */
    FpgaRegWrite(REG_GB_Read_Addr, (wAddr & 0x7f));
    RcpFpgaPrint("www ptFpgaGBRead->wWordPointer: 0x%04x \r\n", wAddr);

    /* read������ָ����Ĵ���*/
    FpgaRegWrite(REG_GB_Read_WordCount, wWordCount);
    RcpFpgaPrint("www ptFpgaGBRead->wReadWordsLen: 0x%04x \r\n", wWordCount);

} /* static int uhf_GBSetMacReadPara(T_FpgaGBRead *ptFpgaGBRead) */

/**********************************************************************
* �������ƣ�void uhf_EpcSetMacWritePara(WORD16 wMemBank,WORD16 wAddr,
                                                                          WORD16 wWrWordLen,BYTE *ucdata)
* ����������
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
void uhf_GBSetMacWritePara(WORD16 wMemBank, WORD16 wAddr, WORD16 wWrWordLen, BYTE *ucdata)
{
    WORD16 wIndex = 0;
    /* Write����MemBank�Ĵ��� */
    FpgaRegWrite(REG_GB_Write_Membank, (WORD16)(wMemBank & 0x3));
    RcpFpgaPrint("MemBank %d", wMemBank);

    /* Write�����ַ�Ĵ��� */
    FpgaRegWrite(REG_GB_Write_Addr, (wAddr & 0x7f));
    RcpFpgaPrint("WordPointer  %d", wAddr);

    /* Write������ָ����Ĵ��� */
    FpgaRegWrite(REG_GB_Write_WordCount, wWrWordLen);
    RcpFpgaPrint("WriteWordsLen  %d \r\nWrite Data", wWrWordLen);

    /*����FPGA�����󣬲�ͬ��д����д���Դ�����ͬ�����д��ĳɹ���*/
    SetGBWrtRtyTime(wWrWordLen);

    /*��������FPGA
    �Ŀ��Ƴ�ʱʱ��*/
    //FpgaRegWrite(G2_write_rcv_delay_val_h, 0x0007);
    //FpgaRegWrite(G2_write_rcv_delay_val_l, 0x6590);

    /* write�������ݼĴ��� */
    for (wIndex = 0; wIndex < wWrWordLen; wIndex++)
    {
        FpgaRegWrite(REG_GB_BlockWrite_Data_Start + wIndex,
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
        RcpFpgaPrint("%04x",
                     ((((WORD16)ucdata[2 * wIndex]) << 8) | ((WORD16)ucdata[2 * wIndex + 1])));
    }

    RcpFpgaPrint("\r\n");
} /* static int uhf_EpcSetMacWritePara(T_FpgaEpcWrite *ptFpgaEpcWrite) */

/**********************************************************************
* �������ƣ�uhf_GBSetMacReadLock
* ����������
* ���������
                                1 ��������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/28    V0.0.1          lxc        ����
************************************************************************/
void uhf_GBSetMacLockPara(BYTE ucDataArea,BYTE ucAreaCfg, BYTE ucAreaAction, BYTE ucSecAction )
{
    WORD16 wData = 0;

    if (0 == ucAreaCfg) //���� �洢��
        wData = ((ucAreaAction << 8) & 0x3) | ((ucAreaCfg << 6) & 0x3) | ((ucDataArea << 0) & 0x3F);
    else // ���ð�ȫ��
        wData = ((ucSecAction << 8) & 0x3) | ((ucAreaCfg << 6) & 0x3) | ((ucDataArea << 0) & 0x3F);
    /* payload_mask */
    FpgaRegWrite(REG_GB_Lock_Para, wData);
    RcpFpgaPrint("REG_GB_Lock_Para   = 0x%04x \r\n", wData);
}

/**********************************************************************
* �������ƣ�void uhf_GBSetMacErasePara
* �������������ò�д����
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
void uhf_GBSetMacErasePara(WORD16 wMemBank, WORD16 wAddr, WORD16 wErWordLen)
{
    /* Erase����MemBank�Ĵ��� */
    FpgaRegWrite(REG_GB_Erase_Membank, (WORD16)(wMemBank & 0x3));
    RcpFpgaPrint("MemBank %d", wMemBank);

    /* Erase�����ַ�Ĵ��� */
    FpgaRegWrite(REG_GB_Erase_Membank, (wAddr & 0x7f));
    RcpFpgaPrint("WordPointer  %d", wAddr);

    /* Erase�����ָ����Ĵ��� */
    FpgaRegWrite(REG_GB_Erase_WordCount, wErWordLen);
    RcpFpgaPrint("EraseWordsLen  %d \r\nWrite Data", wErWordLen);

    /*����FPGA�����󣬲�ͬ��д����д���Դ�����ͬ�����д��ĳɹ���*/
    //SetGBWrtRtyTime(wErWordLen);

    RcpFpgaPrint("\r\n");
} /* static int uhf_EpcSetMacWritePara(T_FpgaEpcWrite *ptFpgaEpcWrite) */

/**********************************************************************
* �������ƣ�uhf_GBSetMacKillPara
* ��������������GBɱ������ɱ��ʱ��Ҫ
* ���������ɱ����Կ
* ��������� ��
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2014/2/16    V0.0.1       lxc          ����
************************************************************************/
/// FIX ME !! fpga ��û�и���kill����ļĴ�����ַ
void uhf_GBSetMacKillPara(WORD32 dwKillPassword)
{

    RcpFpgaPrint("UHF: wKillPassWord = 0x%04x \n", dwKillPassword);
    /* wKillPassHigh */
    FpgaRegWrite(REG_GB_Kill_Password_H, (WORD16)((dwKillPassword >> 16) & 0xffff));

    /* wKillPassLow */
    FpgaRegWrite(REG_GB_Kill_Password_L, (WORD16)(dwKillPassword & 0xffff));


    FpgaRegWrite(REG_GB_Kill_RFU, 0x0000);

}


/**********************************************************************
* �������ƣ�StartFpgaProcessIsoCmd
* ���������� д��fpga���������Ĳ���
* ��������� ��
* �� �� ֵ��     ��
* ����˵��������FPGA��ʼ����ISOЭ������
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
static void StartFpgaProcessIsoCmd(void)
{
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x00);
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x01);
    FpgaRegWrite(REG_CPU_CMD_Trig, 0x00);
}

/**********************************************************************
* �������ƣ�uhf_IsoStartProcAndTimer
* ���������� ����fpga����ͬʱ������ʱ��
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_IsoStartProcAndTimer(void)
{

    StartFpgaProcessIsoCmd();
#if FPGA_SWTICH
    usp_drv_fpga_int_enable();
#endif
    uhf_startRcpTimer(FPGA_TIMEOUT_INTERVAL_IN_MS);
}


/**********************************************************************
* �������ƣ�uhf_TxSet
* �������������ذ���ߵ��źŷ��Ϳ�ʼ����
* ��������� ��
* �� �� ֵ��     ��
* ����˵����
* �޸�����        �汾��     �޸���          �޸�����
* -----------------------------------------------
* 2010/11/26           V04.00.00           zdx                   ����
***********************************************************************/
void uhf_TxSet(WORD16 wFpgaTxAction)
{

    if (wFpgaTxAction != FpgaRegRead(REG_RF_TX_Ctrl))
    {
        FpgaRegWrite(REG_RF_TX_Ctrl, wFpgaTxAction);
    }

} /* WORD32 usp_uhf_rf_txSet(WORD32 dwRfTxAction) */

/**********************************************************************
* �������ƣ�void uhf_StopTx(void)
* �������������нӿ�Э�����̽�����
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
#define UHF_STOP_TX_WAIT_TIME  (50)
void uhf_StopTx(void)
{
    /* �ر���Ƶ���ʷ��� */
    uhf_TxSet(STOP_FPGA_RF_TX);
    /*�˴������ڹر�ǰ�����ʱ��Ȼ����ܽ�K0��ֵ
          ����Ϊ��С������ᵼ��Ƶ��ռ�ô�����Ӱ������
          �Ķ���*/
    //USP_OSS_VOS_SLEEP_USEC(UHF_STOP_TX_WAIT_TIME);
#if (!USP_IS_DISTRIBUTED)
    /*�ڷֲ�ʽϵͳ�в����д˸��Ż�*/
    uhfRf_setPowerToZero();
   
#endif

} /* void uhf_StopTx(void) */


#endif


/**************************************************** file end ********************************************************************/

