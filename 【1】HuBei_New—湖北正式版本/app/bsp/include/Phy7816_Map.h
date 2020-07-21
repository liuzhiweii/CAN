/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�phy7816-Map.h
* �ļ���ʶ�� 
* ����ժҪ��
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ�
* ������ڣ�2015.2.09
* 
************************************************************************/


/***********************************************************
 *                                      ����ͷ�ļ�                               *
 ***********************************************************/

#ifndef  __PHY7816_MAP_
#define  __PHY7816_MAP_



#include "Phy7816-3.h"



/**********************************************************/
/*            IO �˿ڵ���������                                                   */
/**********************************************************/


    typedef volatile unsigned short * T_DataInAddr;
    typedef volatile unsigned short * T_DataOutAddr;
    typedef unsigned short   T_DataPinBit;
    typedef volatile unsigned short *  T_PsamVirtAddr;
	typedef volatile unsigned short *  T_TimerAddr;







#define    BSP_PSAM_IOCTL_MAGIC_NUM            'F'
#define    BSP_PSAM_IOCTL_CMD_ACTIVE           _IOR(BSP_PSAM_IOCTL_MAGIC_NUM, 1, unsigned long)
#define    BSP_PSAM_IOCTL_CMD_DEACTIVE         _IO(BSP_PSAM_IOCTL_MAGIC_NUM, 2) 
#define    BSP_PSAM_IOCTL_CMD_PROCESS          _IOWR(BSP_PSAM_IOCTL_MAGIC_NUM, 3, unsigned long)
#define    BSP_PSAM_IOCTL_CMD_SELECT           _IOWR(BSP_PSAM_IOCTL_MAGIC_NUM, 4, unsigned long)
#define    BSP_PSAM_IOCTL_CMD_TIMER_EN         _IO(BSP_PSAM_IOCTL_MAGIC_NUM, 5)
#define    BSP_PSAM_IOCTL_CMD_TEST             _IOWR(BSP_PSAM_IOCTL_MAGIC_NUM, 6, unsigned long)
extern T_PsamVirtAddr g_pdwPsamKernelVirtBaseAddr;   /*psam�ں������׵�ַ*/
extern T_TimerAddr g_pdwTimerKernelVirtBaseAddr; /*TIMER�ں������׵�ַ*/












    #define   SOCK_VCC_REG_OFFSET(X)        (0xCC + (X << 1))  /*VCC�Ĵ���ƫ��*/
    #define   SOCK_VCC_MASK(X)              0x1                 /*λ����*/

    #define   SOCK_RST_REG_OFFSET(X)        (0x44 + (X << 1))   /*RST�Ĵ���ƫ��*/
    #define   SOCK_RST_MASK(X)              0x1                 /*λ����*/

    #define   SOCK_TX_REG_OFFSET(X)         (0xC8 + (X << 1))  /*TX�Ĵ���ƫ��*/
    #define   SOCK_TX_MASK(X)               0x1                 /*λ����*/

    #define   SOCK_RX_REG_OFFSET(X)         (0xB2 + (X << 3))  /*RX�Ĵ���ƫ��*/
    #define   SOCK_RX_MASK(X)               0x1                 /*λ����*/

    #define   SOCK_DIR_REG_OFFSET(X)        (0xC2 + (X << 1))  /*data�ܽŷ�����ƼĴ���ƫ��*/
    #define   SOCK_DIR_MASK(X)              0x1                 /*λ����*/
    #define   SOCK_DIR_OUTPUT               0x0                 /*ʹ��״̬*/
 
    #define   SOCK_CLK_EN_REG_OFFSET(X)     (0xB6 + (X << 1))  /*ʱ��ʹ�ܼĴ���ƫ��*/
    #define   SOCK_CLK_EN_MASK(X)           0x1                 /*λ����*/
    #define   SOCK_CLK_EN_STATUS            0x0                 /*ʹ��״̬*/

    #define   SELECT_PSAM(X)
    #define   PSAM_CLK_DIV_REG_OFFSET       0xFC               /*��Ƶϵ���Ĵ���ƫ��*/

    #define   WRITE_TIMER_REG(OFFSET,DATA)        *((volatile unsigned short*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))=(DATA)
    #define   READ_TIMER_REG(OFFSET)              *((volatile unsigned short*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))
    #define   WRITE_TIMER_unsigned char_REG(OFFSET,DATA)   *((volatile unsigned char*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))=(DATA)
    #define   READ_TIMER_unsigned char_REG(OFFSET)         *((volatile unsigned char*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))
    #define   WRITE_TIMER_unsigned int_REG(OFFSET,DATA) *((volatile unsigned int*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))=(DATA)
    #define   READ_TIMER_unsigned int_REG(OFFSET)       *((volatile unsigned int*)((unsigned int)g_pdwTimerKernelVirtBaseAddr+OFFSET))








#define EUMBBAR  0xFC040000

#define GTCCR0   0x1100
#define GTBCR0   0x1110
#define GTVPR0   0x1120
#define GTDR0    0x1130






/**********************************************************/
/*                     ��ʱ�Ӽ�������صĺ�                              */
/**********************************************************/










#define PHY7816_INIT_CLK()   

#define PHY7816_INIT_TIMER() \
do \
{\
        unsigned int dwRegData = 0;\
        WRITE_TIMER_unsigned char_REG(TIMER_GTCFR1_REG_OFFSET,0x33);\
        dwRegData = READ_TIMER_unsigned char_REG(TIMER_GTCFR1_REG_OFFSET);\
        WRITE_TIMER_unsigned char_REG(TIMER_GTCFR1_REG_OFFSET,(dwRegData | 0x80));\
        WRITE_TIMER_REG(TIMER_GTPSR1_REG_OFFSET,0x0000);\
        WRITE_TIMER_REG(TIMER_GTPSR2_REG_OFFSET,0x0000);\
        WRITE_TIMER_REG(TIMER_GTMDR2_REG_OFFSET,0x0402);\
        WRITE_TIMER_REG(TIMER_GTMDR1_REG_OFFSET,0x0000);\
        WRITE_TIMER_REG(TIMER_GTEVR1_REG_OFFSET,0x0003);\
        WRITE_TIMER_REG(TIMER_GTEVR2_REG_OFFSET,0x0003);\
        WRITE_TIMER_REG(TIMER_GTRFR1_REG_OFFSET,0xFFFF);\
        WRITE_TIMER_REG(TIMER_GTRFR2_REG_OFFSET,0xFFFF);\
        WRITE_TIMER_REG(TIMER_GTCNR1_REG_OFFSET,0x0000);\
        WRITE_TIMER_REG(TIMER_GTCNR2_REG_OFFSET,0x0000);\
        WRITE_TIMER_unsigned char_REG(TIMER_GTCFR1_REG_OFFSET,0xDD);\
}while(0)

	
    #define PHY7816_CLR_TIMER()        PHY7816_INIT_TIMER()

    #define PHY7816_TIMER              READ_TIMER_unsigned int_REG(TIMER_GTCNR1_REG_OFFSET)
    #define PHY7816_TIMER_WITH         32
    #define PHY7816_TIMER_CLK_RATIO   ((g_pdwPsamKernelVirtBaseAddr[PSAM_CLK_DIV_REG_OFFSET]+1)*2)




/**********************************************************/
/*                 ��Э�������صĺ�                                        */
/**********************************************************/

#define  PHY7816_GARDTIME              12   
#define  PHY7816_START_ETU             372
#define  PHY7816_DETECT_START_ETU      1
#define  PHY7816_PPS_EN                1

/**********************************************************/
/*                 �Ϳ��ۺ���صĺ�                                              */
/**********************************************************/

#define  Phy7816_GetCtlPtr(ucSockId, ptCtl)  (ptCtl = &g_atPhy7816_Ctl[ucSockId])



/* �ṩ�ĺ����ӿ� */

extern void Phy7816_InitClk(unsigned char ucSockID);
extern void Phy7816_InitTimer(unsigned char ucSockID);
extern void Phy7816_SelectSock(unsigned char ucSockId);
extern void Phy7816_SetClkOn(unsigned char ucSockId);
extern void Phy7816_SetClkOff(unsigned char ucSockId);
extern void Phy7816_SetVccOutput(unsigned char ucSockId);
extern void Phy7816_SetRstOutput(unsigned char ucSockId);
extern void Phy7816_SetVccHigh(unsigned char ucSockId);
extern void Phy7816_SetVccLow(unsigned char ucSockId);
extern void Phy7816_SetRstHigh(unsigned char ucSockId);
extern void Phy7816_SetRstLow(unsigned char ucSockId);
extern void Phy7816_SetDataOut(unsigned char ucSockId);
extern void Phy7816_SetDataLow(unsigned char ucSockId);
extern void Phy7816_SetDataIn(unsigned char ucSockId);
extern T_DataInAddr Phy7816_GetDataPortAddr(unsigned char ucSockId);
extern T_DataInAddr Phy7816_GetDataOutAddr(unsigned char ucSockId);
extern T_DataPinBit Phy7816_GetDataPinBit(unsigned char ucSockId);



#endif





