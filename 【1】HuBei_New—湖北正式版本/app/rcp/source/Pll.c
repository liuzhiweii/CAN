#include "usp.h"
#include "common.h"
#if UHF_PROTOCOL_USE
#include "uhf_dbg.h"

#if USP_UHF_ADF_4350_USE
#include "uhf_rf.h"
#include "Board.h"

#include "RfCtl.h"
#include "Pll.h"

extern T_RfCtrl  g_tRfCtrl;
T_RfPllUsrCfg    g_tRfPllUsrCfg;
/**********************************************************************
* �������ƣ�uhfRf_getPLLStatus
* ����������������໷��״̬
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��0 ʧ�� 1 ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

WORD32 uhfRf_getPLLStatus(void)
{
    WORD32 dwData;

    dwData = FpgaRegRead(ADF4350_LOCK_STATUS);

    if (1 == dwData)
    {
        /* 0��PLL���� */
        //uhfRfDevPrint("pll has locked\r\n");
        return (RF_PLL_STATUS_OK);
    }
    else
    {
        /* 1�� PLLʧ�� */
        //uhfRfDevPrint("pll unlock\r\n");
        return (RF_PLL_STATUS_ERROR);

    }
}



#define CfgPllReg(dwData)  \
 dwCheckTimes = 0;\
 FpgaRegWrite(ADF4350_CFG_DATA_H, (WORD16)(dwData >> 16));\
 FpgaRegWrite(ADF4350_CFG_DATA_L, (WORD16)(dwData & 0xffff));\
 FpgaRegWrite(ADF4350_CFG_START, 0);\
 FpgaRegWrite(ADF4350_CFG_START, 1);\
 uhfRfDevPrint("high 0x%x ",(WORD16)(dwData >> 16));\
 uhfRfDevPrint("low 0x%x ",(WORD16)(dwData & 0xffff));\
 while ((1 != FpgaRegRead(ADF4350_CFG_FINISH))\
          && ((dwCheckTimes++) < PLL_CHECK_MAX_TIMES))\
 {\
 }\
 uhfRfDevPrint("pll check time %x\r\n", dwCheckTimes);\
 if(dwCheckTimes >= PLL_CHECK_MAX_TIMES) \
 {\
   uhfRfDevPrint("Write PLL regs error\r\n");\
 }




/* ��һ��
0x00580005
0x0020402c
0xb
0x61081e42
0x08008011  */

WORD32 uhfRf_PLLChipCfg(WORD32 dwFreq, WORD16 wNeedReset)
{
    WORD32   dwInt = 0;
    WORD32   dwFrac = 0;
    WORD32   dwR0Data = 0;
    volatile WORD32 dwCheckTimes = 0;
    float fFrac = 0.0; //����ʱ����м����������

    if (PLL_NEED_RESET == wNeedReset)
    {
        CfgPllReg(ADF4350_R5_VAL); /*R5*/
        CfgPllReg(ADF4350_R4_VAL); /*R4*/
        CfgPllReg(ADF4350_R3_VAL); /*R3*/
        CfgPllReg(ADF4350_R2_VAL); /*R2*/
        CfgPllReg(ADF4350_R1_VAL); /*R1*/
    }

    dwInt  = dwFreq / 7680;  /*[f/7.68] fΪMHZ*/
    fFrac = (((double)dwFreq / 7680) - dwInt) * 1536; /*FRAC=��f/7.68-INT��*1536*/
    dwFrac = fFrac;
    /*R0ΪƵ���趨�Ĵ�����BIT31=0,BIT30~BIT15ΪINT��
         ���������������֣�BIT14~BIT3ΪFRAC��
         ��������С�����֣�BIT2~BIT0=0*/
    dwR0Data = (((dwInt & 0xffff) << 15) | ((dwFrac & 0xfff) << 3)) & 0x7ffffff8;
    uhfRfDevPrint("dwInt %d dwFrac %d reg %x\r\n", dwInt, dwFrac, dwR0Data);
    CfgPllReg(dwR0Data); /*R0*/

    dwCheckTimes = 0;

    while ((RF_PLL_STATUS_OK != uhfRf_getPLLStatus())
           && ((dwCheckTimes++) < PLL_CHECK_MAX_TIMES))
    {
    }

    uhfRfDevPrint("uhfRf:pll lock check %d time\r\n", dwCheckTimes);

    if (dwCheckTimes >= PLL_CHECK_MAX_TIMES)
    {
        uhfRfDevPrint("uhfRf: freq %d set pll failed!\r\n", dwFreq);
        LAPInsert(FILE_RFCB2, USP_UHF_PLL_LOSE, dwFreq, dwCheckTimes);
        return USP_UHF_PLL_CFG_ERROR;
    }

    return USP_SUCCESS;

}
#endif


#if USP_UHF_PLL_2531_USE

#include "uhfFpgaRegDef.h"
#include "uhf_rf.h"
#include "Board.h"
#include "RfCtl.h"
#include "Pll.h"

extern T_RfCtrl      g_tRfCtrl;
T_RfPllUsrCfg    g_tRfPllUsrCfg;
#define MX2513_REG_PLL_DATA_H_write(wData)  FpgaRegWrite(REG_PLL_DATA_H,wData)
#define MX2513_REG_PLL_DATA_L_write(wData)  FpgaRegWrite(REG_PLL_DATA_L,wData)
#define REG_PLL_TRIG_Write(wData)           FpgaRegWrite(REG_PLL_TRIG, wData)

/**********************************************************************
* �������ƣ�uhfRf_getPLLStatus
* ����������������໷��״̬
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��1 ʧ�� 0 ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhfRf_getPLLStatus(void)
{
    WORD32 dwData;

    dwData = FpgaRegRead(PLL2531_LOCK_STATUS);

    if (1 == dwData)
    {
        /* 1�� PLLʧ�� */
        return (RF_PLL_STATUS_ERROR);
    }
    else
    {
        /* 0��PLL���� */
        return (RF_PLL_STATUS_OK);
    }
}


/**********************************************************************
* �������ƣ�WORD32 Wait_PLL_Write_Ready(void)
* �������������PLL�Ĵ����Ƿ��д
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��WORD32
             0   ��  �Ĵ�����д
             ������  �Ĵ�������д
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
#define MAX_PLL_WRITE_READY_WAIT    (2)
WORD32 Wait_PLL_Write_Ready(void)
{
    WORD16 wData;
    int iCount;

    /* ���PLL�Ĵ����Ƿ��д */
    for (iCount = 0; iCount < MAX_PLL_WRITE_READY_WAIT; iCount++)
    {
        wData = FpgaRegRead(REG_PLL_READY);

        if (wData == PLL_CFG_RDY)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    if (iCount == MAX_PLL_WRITE_READY_WAIT)
    {
        /* FPGA�ж�PLL���п��ƵļĴ�����ʱ���޷���������״̬ */
        uhfDevPrint("\n\r\n UHF: error Wait_PLL_Write_Ready timeout !!! \r\n\r\n");
        return 1;

    }

    return (0);

} /* WORD32 Wait_PLL_Write_Ready(void) */

/**********************************************************
LMX2531��������

Parameter         Min  Typ  Max  Units
Frequency Range - 880  915  933  MHz
Step Size       -      200  250  kHz
PLL_DATA_H��16λ����ַ��005FH    Bit7-bit0��24λPLL���������ݸ߰�λ
PLL_DATA_L��16λ����ַ��0060H    Bit15-bit0��24λPLL���������ݵ�16λ

PLL���ô����Ĵ��� PLL_TRIG��  16λ�� ��ַ��0061H
1������PLL����ģ�鿪ʼ����PLL,��ʾ�����׼�����������ݣ�Ҫȥ�߼���ʼ���͡�
0��null

PLL׼����  PLL_READY��16λ����ַ��0063H
1����ǰ�߼������Ѿ�������ɣ����ѿ�ʼ��һ��PLL���ݴ��͡�
0�����ڴ���PLL��������


PLL��ʧ��״ָ̬ʾ��PLL_STATUS��16λ����ַ��0064H
0�� PLL����
1��PLLʧ��

��������:
���໷�Ĵ���	ƫ�Ƶ�ַ005FH	ƫ�Ƶ�ַ0060H��16λ��
R5 =0x840005	     0084	             0005
R5 =0x800005		 0080			     0005
R5 =0x8007F5		 0080		 		 07F5
R12=0x01048C	     0001			 	 048C
R9 =0x000BA9		 0000				 0BA9
R8 =0x030008		 0003				 0008
R7 =0x000207		 0000				 0207
R6 =0x18E656		 0018				 E656
R4 =0x3C0004		 003C				 0004
R3 =0xECC0F3		 00EC				 C0F3
R2 =0x77A022		 0077				 A022
R1 R0

Reg	23	22	21	20	19	18	17	16	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
R0	<-                     N[7��0]	<-                                 NUM[11��0]   0	0	0	0
R1	0	0	1	1	1	1	1	  N[10��8]	<-                        NUM[21��12]	0	0	0	1


N  = [F/6.5]����������       --------��ʽ��2��
NUM= 65000*��F/6.5-N��    --------��ʽ��3��

************************************************************/
/**********************************************************************
* �������ƣ�WORD32 PLLCfgMX2531(WORD32 dwRfFreq)
* ����������PLL���໷����
* ���ʵı���
* �޸ĵı���
* ���������WORD32 dwRfFreq : PLL����Ƶ��
* ���������
* �� �� ֵ��WORD32
                         0��    �ɹ�
                      ������    ʧ��

* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 uhfRf_PLLChipCfg(WORD32 dwRfFreq, WORD16 wNeedReset)
{

    WORD32 dwFreData;
    WORD32 dwData;
    WORD32 dwN;
    WORD32 dwNum;
    static WORD16 wResetPll = PLL_NEED_RESET;

    if (RF_PLL_STATUS_OK != uhfRf_getPLLStatus() || wNeedReset == PLL_NEED_RESET)
    {
        /* ��������ȫ���Ĵ�����������ֻ����R0 R1 */
        wResetPll = PLL_NEED_RESET;
    }

    //DbgPrintf("UHF: befin to config Mx2531, dwRfFreq = %u \r\n", dwRfFreq);

    /* modify by mk for pll1700e 080127*/
    /* ���Ƶ���Ƿ�����Ч��Χ�� */
    if (g_tRfCtrl.wFreqType == PLL_TYPE_900M)
    {
        if ((dwRfFreq < MIN_RF_FREQUENCY_MX2531_LQ1742) || (dwRfFreq > MAX_RF_FREQUENCY_MX2531_LQ1742))
        {
            /* Ƶ�㳬����Ч��Χ��ʹ��Ĭ��Ƶ�� */
            uhfDevPrint("\r\n\r\n UHF: dwRfFreq = %uld (dwRfFreq < 880000) || (dwRfFreq > 933000) !!!!! \r\n\r\n", dwRfFreq);
            dwRfFreq = DEFAULT_900M_FREQUENCY;
            uhfDevPrint("UHF: use dwDefaultWorkFreq = %d \r\n", dwRfFreq);

        }
    }
    else if (g_tRfCtrl.wFreqType == PLL_TYPE_800M)
    {
        if ((dwRfFreq < MIN_RF_FREQUENCY_MX2531_LQ1700E) || (dwRfFreq > MAX_RF_FREQUENCY_MX2531_LQ1700E))
        {
            /* Ƶ�㳬����Ч��Χ��ʹ��Ĭ��Ƶ�� */
            uhfDevPrint("\r\n\r\n UHF: dwRfFreq = %uld (dwRfFreq < 831000) || (dwRfFreq > 885000) !!!!! \r\n\r\n", dwRfFreq);

            dwRfFreq = DEFAULT_800M_FREQUENCY;
            uhfDevPrint("UHF: use dwDefaultWorkFreq = %d \r\n", dwRfFreq);
        }

    }
    else
    {
        uhfDevPrint("UHF : Unknown PLL type !\r\n");
        return 1;
    }

    /************************************************************************/
    /*                       first force PLL to be losed                                                                   */
    /************************************************************************/



    if (wResetPll == PLL_NEED_RESET)
    {
        /* �ȴ�FPGA���� */
        Wait_PLL_Write_Ready();

        /*����ȫ��R0-R5*/
        dwData = (WORD32)0x840005;  /*R5 = 0x840005*/
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));   /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));              /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x800005;  /* R5 = 0x800005 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));    /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));             /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x8007F5;  /* R5 = 0x8007F5 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�8λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x01048C;  /* R12 = 0x01048C */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x000BA9;  /* R9 = 0x000BA9 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x030008;  /* R8 = 0x030008 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x000207;  /* R7 = 0x000207 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x18E656;  /* R6 = 0x000207 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x3C0004;  /* R4 = 0x3C0004 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0xECC0F3;  /* R3 = 0xECC0F3 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x77A022;  /* R2 = 0x77A022 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(3000);

        // usleep(2*1000);

        /* �ȴ�FPGA���� */
        Wait_PLL_Write_Ready();

        /*���� R0 R1*/
        dwFreData = 500000;                                         /*500Mhz*/

        dwN = (dwFreData * 10) / 65;                             /* N=[F/6.5]���������� */
        dwNum = 65000 * ((dwFreData * 10) / 65 - dwN) ;  /*NUM= 65000*��F/6.5-N��*/

        dwData = (WORD32)(0x3E0001) | (WORD32)((dwN & 0x0700) << 6) | (WORD32)((dwNum & 0x3FF000) >> 8); /*R1*/
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);
        /* �ȴ�FPGA���� */
        Wait_PLL_Write_Ready();

        dwData = (WORD32)((dwN & 0xFF) << 16) | (WORD32)((dwNum & 0x0FFF) << 4);    /*R0*/
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);

        usleep(2 * 1000);

        /* �ȴ�FPGA���� */
        Wait_PLL_Write_Ready();
        /* check PLL status */

        if (RF_PLL_STATUS_OK == uhfRf_getPLLStatus())
        {
            uhfDevPrint("\r\nRCP: PLL should losed but not losed  at 500MHz !!! \r\n");
            LAPInsert(FILE_FPGAAPI, USP_UHF_PLL_LOSE, 1, 0);
        }
        else
        {
            //DbgPrintf("UHF: PLL is losed \r\n");
        }
    }

    /************************************************************************/
    /*                       begin to config PLL                                                                              */
    /************************************************************************/
    if (wResetPll == PLL_NEED_RESET)
    {
        /*����ȫ��R0-R5*/
        dwData = (WORD32)0x840005;  /*R5 = 0x840005*/
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));   /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));              /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x800005;  /* R5 = 0x800005 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));    /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));             /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);

        dwData = (WORD32)0x8007F5;  /* R5 = 0x8007F5 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�8λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x01048C;  /* R12 = 0x01048C */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x000BA9;  /* R9 = 0x000BA9 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x030008;  /* R8 = 0x030008 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x000207;  /* R7 = 0x000207 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x18E656;  /* R6 = 0x000207 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x3C0004;  /* R4 = 0x3C0004 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0xECC0F3;  /* R3 = 0xECC0F3 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(1000);


        dwData = (WORD32)0x77A022;  /* R2 = 0x77A022 */
        MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
        MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
        /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
        REG_PLL_TRIG_Write(0);
        REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
        usleep(3000);


    }

    /* �ȴ�FPGA���� */
    Wait_PLL_Write_Ready();

    /*DCO disable*/
    dwData = (WORD32)0x8007D5;  /* R5 = 0x8007F5 */
    MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
    MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�8λ���� 0060H*/
    /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
    REG_PLL_TRIG_Write(0);
    REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
    usleep(500);
    /*DCO enable*/
    dwData = (WORD32)0x8007F5;  /* R5 = 0x8007F5 */
    MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
    MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�8λ���� 0060H*/
    /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
    REG_PLL_TRIG_Write(0);
    REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
    usleep(500);

    /*���� R0 R1*/
    dwFreData = dwRfFreq;
    dwN = (dwFreData / 100) / 65;                            /* N=[F/6.5]���������� */
    dwNum = 10 * dwFreData - 65000 * dwN ; /*NUM= 65000*��F/6.5-N��*/

    dwData = (WORD32)(0x3E0001) | (WORD32)((dwN & 0x0700) << 6) | (WORD32)((dwNum & 0x3FF000) >> 8); /*R1*/
    MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
    MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
    /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
    REG_PLL_TRIG_Write(0);
    REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);
    usleep(1000);
    /* �ȴ�FPGA���� */
    Wait_PLL_Write_Ready();

    dwData = (WORD32)((dwN & 0xFF) << 16) | (WORD32)((dwNum & 0x0FFF) << 4);    /*R0*/
    MX2513_REG_PLL_DATA_H_write((WORD16)((dwData >> 16) & 0xff));  /* MX2513_PLL �ĸ�8λ���� 005FH*/
    MX2513_REG_PLL_DATA_L_write((WORD16)(dwData & 0xffff));            /* MX2513_PLL �ĵ�16λ���� 0060H*/
    /* ֪ͨFPGA��ʼдPLL�Ĵ��� */
    REG_PLL_TRIG_Write(0);
    REG_PLL_TRIG_Write(PLL_DATA_READY_TRIG);

    // DbgPrintf("UHF:MX2531_PLL _CFG Freq = %d, N = %d, NUM=%d, R1 = 0x%08x, R0 = 0x%08x\r\n",
    //                         dwFreData,dwN,dwNum,((WORD32)(0x3E0001) | (WORD32)((dwN & 0x0700) << 6) | (WORD32)((dwNum & 0x3FF000) >> 8))
    //                         ,dwData);


    if (wResetPll == PLL_NEED_RESET)
    {

        usleep(2 * 1000);
        wResetPll = PLL_NO_NEED_RESET;
    }
    else
    {
        usleep(2 * 1000);
    }

    /* �ȴ�FPGA���� */
    Wait_PLL_Write_Ready();

    if (RF_PLL_STATUS_OK != uhfRf_getPLLStatus())
    {
        uhfDevPrint("\r\nRCP: config PLL failed !!!!! \r\n");
        uhfDevPrint("UHF: dwRfFreq = %d \r\n", dwRfFreq);
        LAPInsert(FILE_FPGAAPI, USP_UHF_PLL_LOSE, 1, 0);
        return (1);
    }

    return (0);
} /*WORD32 PLLCfgMX2531(WORD32 dwRfFreq)*/
#endif


#if USP_UHF_PLL_2541_USE
#include "uhf_rf.h"
#include "Board.h"

#include "RfCtl.h"
#include "Pll.h"

extern T_RfCtrl  g_tRfCtrl;
T_RfPllUsrCfg    g_tRfPllUsrCfg = 
{
                .dwUsrR6=MX2541_R6_VAL,
                .dwUsrR3=MX2541_R3_VAL,
};

/**********************************************************************
* �������ƣ�uhfRf_getPLLStatus
* ����������������໷��״̬
* ���ʵı���
* �޸ĵı���
* ���������
* ���������
* �� �� ֵ��0 ʧ�� 1 ����
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/

WORD32 uhfRf_getPLLStatus(void)
{
    WORD32 dwData;
	DEBUG_MSG("uhfRf_getPLLStatus start\n");
	fflush(stdout);

    dwData = FpgaRegRead(LMX2541_LOCK_STATUS);

    if (1 == dwData)
    {
        /* 1��PLL���� */
        //uhfRfDevPrint("pll has locked\r\n");
        return (RF_PLL_STATUS_OK);
    }
    else
    {
		DEBUG_MSG("RF_PLL_STATUS_ERROR\n");
		fflush(stdout);
        /* 0�� PLLʧ�� */
        //uhfRfDevPrint("pll unlock\r\n");
        return (RF_PLL_STATUS_ERROR);

    }
}



#define CfgPllReg(dwData)  \
 dwCheckTimes = 0;\
 FpgaRegWrite(LMX2541_CFG_DATA_H, (WORD16)(dwData >> 16));\
 FpgaRegWrite(LMX2541_CFG_DATA_L, (WORD16)(dwData & 0xffff));\
 FpgaRegWrite(LMX2541_CFG_START, 0);\
 FpgaRegWrite(LMX2541_CFG_START, 1);\
 uhfRfDevPrint("uhfRf:high 0x%4x ",(WORD16)(dwData >> 16));\
 uhfRfDevPrint("low 0x%4x ",(WORD16)(dwData & 0xffff));\
 while ((1 != FpgaRegRead(LMX2541_CFG_FINISH))\
          && ((dwCheckTimes++) < PLL_CHECK_MAX_TIMES))\
 {\
 }\
 uhfRfDevPrint("pll check time 0x%4x\r\n", dwCheckTimes);\
 if(dwCheckTimes >= PLL_CHECK_MAX_TIMES) \
 {\
   uhfRfDevPrint("uhfRf:Write PLL regs error\r\n");\
 }

 


/* ��һ��
0x00580005
0x0020402c
0xb
0x61081e42
0x08008011  */

WORD32 uhfRf_PLLChipCfg(WORD32 dwFreq, WORD16 wNeedReset)
{
    WORD32   dwR0Data = 0;
    WORD32   dwR1Data = 0;
    WORD32   dwPLL_Num = 0;
    WORD32   dwPLL_N = 0;
    WORD32   dwPLL_R = 0;
    volatile WORD32 dwCheckTimes = 0;
    float    fFrac = 0.0; //����ʱ����м����������

    if (PLL_NEED_RESET == wNeedReset)
    {
        CfgPllReg(MX2541_R7_VAL); 
        CfgPllReg(MX2541_R13_VAL); 
        CfgPllReg(MX2541_R12_VAL); 
        CfgPllReg(MX2541_R9_VAL); 
        CfgPllReg(MX2541_R8_VAL);
        CfgPllReg(g_tRfPllUsrCfg.dwUsrR6);
        CfgPllReg(MX2541_R5_VAL);
        CfgPllReg(MX2541_R4_VAL);
        CfgPllReg(g_tRfPllUsrCfg.dwUsrR3);
        CfgPllReg(MX2541_R2_VAL);
    }

    /*��Ƶ����ΪFref=40MHz��Fpdf=0.125MHz��VCO���3��Ƶ�����Ƶ�����ù�ʽ���£�
          f=0.125*��INT��/3
          INT=[f*3/0.125]����[]Ϊȡ���㣩
          ���磺f=840.125MHz  INT=20163��4EC3 hex��
          ת��ΪR0�Ĵ���ʱ��������4λ���õ�EC30����R0=0x0000 EC30*/

    fFrac = ((double)dwFreq * 3)/125; /*FRAC=��f/7.68-INT��*1536*/
    dwPLL_N = fFrac;
    dwR0Data = (dwPLL_N << 4) & 0xffff;
    /*  */
    dwPLL_R = 40000 / 125;
    dwR1Data = ((dwPLL_N & 0xf000) << 4) | (dwPLL_R << 4) | 0x01;
    uhfRfDevPrint("uhfRf:dwPLL_N 0x%x  dwFrac %d  dwR0Data 0x%x  dwR1Data 0x%x\r\n", dwPLL_N, dwFreq, dwR0Data, dwR1Data);

    CfgPllReg(dwR1Data);
    CfgPllReg(dwR0Data);

    dwCheckTimes = 0;
    while ((RF_PLL_STATUS_OK != uhfRf_getPLLStatus())
           && ((dwCheckTimes++) < PLL_CHECK_MAX_TIMES))
    {
    }
    uhfRfDevPrint("uhfRf:pll lock check %d time\r\n", dwCheckTimes);

    if (dwCheckTimes >= PLL_CHECK_MAX_TIMES)
    {
        uhfRfDevPrint("uhfRf: freq %d set pll failed!\r\n", dwFreq);
        LAPInsert(FILE_RFCB2, USP_UHF_PLL_LOSE, dwFreq, dwCheckTimes);
        return USP_UHF_PLL_CFG_ERROR;
    }
    return USP_SUCCESS;

}
#endif

#endif
