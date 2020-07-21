/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�phy7816-3.h
* �ļ���ʶ�� 
* ����ժҪ��psam����7816Э���ʵ��ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ�
* ������ڣ�2015.2.09
* 
************************************************************************/

/***********************************************************
 *                                      ����ͷ�ļ�                               *
 ***********************************************************/

#ifndef  __PHY7816_3__
#define  __PHY7816_3__


typedef struct tagT_7816_Ctl
{
    unsigned char   ucTimerClkRatio;   /* timer ��ʱƵ�ʺ�clkʱ��Ƶ�ʵı��� */
    unsigned char   ucPad;
    unsigned short wEtu;             /* ÿ��bit����ռ�õ�clk���� */

    unsigned short w1BitTime;
    unsigned short w1HalfBitTime;    /* 1 .5��bit������ռ�õ�timer���� */
    unsigned short w2HalfBitTime;   
    unsigned short w3HalfBitTime;   
    unsigned short w4HalfBitTime;   
    unsigned short w5HalfBitTime;   
    unsigned short w6HalfBitTime;   
    unsigned short w7HalfBitTime;   
    unsigned short w8HalfBitTime;   
    unsigned short w9HalfBitTime;   
    unsigned short w10HalfBitTime;
    unsigned short w11HalfBitTime;
    unsigned int dwOverflowFin;     /* ���ճ�ʱʱtimer������������ */
    unsigned int dwTimeFin;         

    unsigned char   ucTS;
    unsigned char   ucT0;
    unsigned char   ucTA1;
    unsigned char   ucTB1;
    unsigned char   ucTC1;
    unsigned char   ucTD1;
    unsigned char   ucTA2;
    unsigned char   ucTB2;
    unsigned char   ucTC2;
    unsigned char   ucTD2;
    unsigned char   ucGardEtu;
    unsigned char   ucPPSEn;

    unsigned int dwAtrLen;
    unsigned char   aucAtr[32];

    unsigned int dwHistLen;
    unsigned char   aucHistory[32];
            
}T_Phy7816_Ctl;

#define  PHY7816_DEBUG(...)

#define PROCESS_COMPLETED     0x9000
#define TRANSFER_RESPONSE     0x61
#define ABORTED_WRONG_LENGTH  0x67
#define ABORTED_LE_LENGTH     0x6C
#define ABORTED_INVALID_INS   0x6D00
#define CLASS_NOT_SUPPORTED   0x6E00
#define INCORRECT_P1P2        0x6A86




extern T_Phy7816_Ctl g_atPhy7816_Ctl[BSP_DRV_PSAM_NR];

void Phy7816_DelayClk(unsigned int dwClkNum);
void Phy7816_DelayEtu(unsigned char ucSockId, unsigned char ucEtuNum);
unsigned int Phy7816_Rcv(unsigned char ucSockId, unsigned char *pucBuf, unsigned short wLen);
unsigned int Phy7816_Snd(unsigned char ucSockId, unsigned char *pucBuf, unsigned short wLen);
unsigned int Phy7816_Active(unsigned char ucSockId, T_CardPara *ptCardPara);
void Phy7816_Deactive(unsigned char ucSockId);
unsigned int CPU_CardCmdProc(unsigned char ucSockId, unsigned char ucCmdLen, unsigned char *pucCmdApdu, unsigned char *pucResLen, unsigned char 
*pucResTpdu);
unsigned int Phy7816_RcvHistory(unsigned char ucSockId);
#endif



