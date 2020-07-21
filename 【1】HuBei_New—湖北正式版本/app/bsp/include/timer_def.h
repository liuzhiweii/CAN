/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� timerDef.h
* �ļ���ʶ�� 
* ����ժҪ��timer�����ӿ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/

#ifndef _TIMER_DEF_H_
#define _TIMER_DEF_H_


typedef unsigned short T_TimerAddr;



/*�궨��*/
#define WRITE_TIMER_REG(OFFSET,DATA)   *((volatile T_TimerAddr*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))=(DATA)
#define READ_TIMER_REG(OFFSET)         *((volatile T_TimerAddr*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))
#define WRITE_TIMER_WORD32_REG(OFFSET,DATA)   *((volatile unsigned int*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))=(DATA)
#define READ_TIMER_WORD32_REG(OFFSET)         *((volatile unsigned int*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))
#define WRITE_TIMER_WORD16_REG(OFFSET,DATA)   *((volatile unsigned short*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))=(DATA)
#define READ_TIMER_WORD16_REG(OFFSET)         *((volatile unsigned short*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))
#define WRITE_TIMER_BYTE_REG(OFFSET,DATA)   *((volatile unsigned char*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))=(DATA)
#define READ_TIMER_BYTE_REG(OFFSET)         *((volatile unsigned char*)((unsigned int)g_pTimerUsrSpaceBaseAddr+OFFSET))
    extern volatile T_TimerAddr *g_pTimerUsrSpaceBaseAddr;
#ifdef TIMER_USE_INT_MODE
    #define WRITE_TIMER_REG_INT(OFFSET,DATA)   *((volatile unsigned short*)((unsigned int)g_pTimerKernelSpaceBaseAddr+OFFSET))=(DATA)
	#define READ_TIMER_REG_INT(OFFSET)         *((volatile unsigned short*)((unsigned int)g_pTimerKernelSpaceBaseAddr+OFFSET))
    #define WRITE_TIMER_BYTE_REG_INT(OFFSET,DATA)   *((volatile unsigned char*)((unsigned int)g_pTimerKernelSpaceBaseAddr+OFFSET))=(DATA)
	#define READ_TIMER_BYTE_REG_INT(OFFSET)         *((volatile unsigned char*)((unsigned int)g_pTimerKernelSpaceBaseAddr+OFFSET))
    extern volatile unsigned short *g_pTimerKernelSpaceBaseAddr;

	#define BSP_DRV_TIMER_IRQ_NUMBER       84     /*GTM3�жϺ�*/
	/*ioctl����*/
	#define    TIMER_IOCTL_BASE            'T'
	#define    TIMER_SLEEP_SET             _IOWR(TIMER_IOCTL_BASE, 1, unsigned long)
#endif

/*MPC8306 TIMER1 �Ĵ�������*/
#define TIMER_GTCFR1_REG_OFFSET      0x500    /*Timer1���üĴ���*/
#define TIMER_GTMDR1_REG_OFFSET      0x510    /*Timer1ģʽ�Ĵ���*/
#define TIMER_GTMDR2_REG_OFFSET      0x512    /*Timer2ģʽ�Ĵ���*/
#define TIMER_GTRFR1_REG_OFFSET      0x514    /*Timer1�ο��Ĵ���*/
#define TIMER_GTRFR2_REG_OFFSET      0x516    /*Timer2�ο��Ĵ���*/
#define TIMER_GTCPR1_REG_OFFSET      0x518    /*Timer1����Ĵ���*/
#define TIMER_GTCPR2_REG_OFFSET      0x51A    /*Timer2����Ĵ���*/
#define TIMER_GTCNR1_REG_OFFSET      0x51C    /*Timer1�����Ĵ���*/
#define TIMER_GTCNR2_REG_OFFSET      0x51E    /*Timer2�����Ĵ���*/
#define TIMER_GTEVR1_REG_OFFSET      0x530    /*Timer1�¼��Ĵ���*/
#define TIMER_GTEVR2_REG_OFFSET      0x532    /*Timer2�¼��Ĵ���*/
#define TIMER_GTPSR1_REG_OFFSET      0x538    /*Timer1��Ƶ�Ĵ���*/
#define TIMER_GTPSR2_REG_OFFSET      0x53A    /*Timer2��Ƶ�Ĵ���*/

#define TIMER_GTCFR2_REG_OFFSET      0x504    /*Timer3���üĴ���*/
#define TIMER_GTMDR3_REG_OFFSET      0x520    /*Timer3ģʽ�Ĵ���*/
#define TIMER_GTRFR3_REG_OFFSET      0x524    /*Timer3�ο��Ĵ���*/
#define TIMER_GTCPR3_REG_OFFSET      0x528    /*Timer3����Ĵ���*/
#define TIMER_GTCNR3_REG_OFFSET      0x52C    /*Timer3�����Ĵ���*/
#define TIMER_GTEVR3_REG_OFFSET      0x534    /*Timer3�¼��Ĵ���*/
#define TIMER_GTPSR3_REG_OFFSET      0x53C    /*Timer3��Ƶ�Ĵ���*/

/*��ʼ����ʱ��,TIMER3�����ж�ģʽ*/
	/*
	1.ֹͣtimer3;
	3.����3����Ƶϵ��
	4.����3����Ƶϵ����ʱ��Դ�����ն�ʱ��ʱ��Ƶ��10KHz��
	5.����¼��Ĵ�����
	6.���òο��Ĵ�����
	7.��������0;
	*/
	#define TIMER_INIT_INT(REF_VALUE)\
	do\
	{\
	    WRITE_TIMER_BYTE_REG_INT(TIMER_GTCFR2_REG_OFFSET,0x33);\
	    WRITE_TIMER_REG_INT(TIMER_GTPSR3_REG_OFFSET,0x0063);\
        WRITE_TIMER_REG_INT(TIMER_GTMDR3_REG_OFFSET,0x841A);\
        WRITE_TIMER_REG_INT(TIMER_GTEVR3_REG_OFFSET,0x0003);\
        WRITE_TIMER_REG_INT(TIMER_GTRFR3_REG_OFFSET,REF_VALUE);\
        WRITE_TIMER_REG_INT(TIMER_GTCNR3_REG_OFFSET,0x0000);\
	    WRITE_TIMER_BYTE_REG_INT(TIMER_GTCFR2_REG_OFFSET,0x2D);\
	}while(0)

    /*�رն�ʱ��*/

	#define TIMER_STOP_INT()\
	do\
	{\
	    WRITE_TIMER_BYTE_REG_INT(TIMER_GTCFR2_REG_OFFSET,0xFF);\
	}while(0)


/*��ʼ����ʱ��*/
/*
1.ֹͣtimer1��timer2;
2.����timer1��timer2������
3.����2����Ƶϵ��
4.����1����Ƶϵ����ʱ��Դ�����ն�ʱ��ʱ��Ƶ��1MHz��
5.����¼��Ĵ�����
6.���òο��Ĵ�����
7.��������0;
*/
#define TIMER_INIT()\
do\
{\
    unsigned int dwRegData = 0;\
    WRITE_TIMER_BYTE_REG(TIMER_GTCFR1_REG_OFFSET,0x33);\
    dwRegData = READ_TIMER_BYTE_REG(TIMER_GTCFR1_REG_OFFSET);\
    WRITE_TIMER_BYTE_REG(TIMER_GTCFR1_REG_OFFSET,(dwRegData | 0x80));\
    WRITE_TIMER_REG(TIMER_GTPSR1_REG_OFFSET,0x0000);\
    WRITE_TIMER_REG(TIMER_GTPSR2_REG_OFFSET,0x0000);\
    WRITE_TIMER_REG(TIMER_GTMDR2_REG_OFFSET,0x8402);\
    WRITE_TIMER_REG(TIMER_GTMDR1_REG_OFFSET,0x0000);\
    WRITE_TIMER_REG(TIMER_GTEVR1_REG_OFFSET,0x0003);\
    WRITE_TIMER_REG(TIMER_GTEVR2_REG_OFFSET,0x0003);\
    WRITE_TIMER_REG(TIMER_GTRFR1_REG_OFFSET,0xFFFF);\
    WRITE_TIMER_REG(TIMER_GTRFR2_REG_OFFSET,0xFFFF);\
    WRITE_TIMER_REG(TIMER_GTCNR1_REG_OFFSET,0x0000);\
    WRITE_TIMER_REG(TIMER_GTCNR2_REG_OFFSET,0x0000);\
}while(0)

/*������ʱ��*/

#define TIMER_START()\
do\
{\
    WRITE_TIMER_BYTE_REG(TIMER_GTCFR1_REG_OFFSET,0xDD);\
}while(0)


/*�رն�ʱ��*/

#define TIMER_STOP()\
do\
{\
    WRITE_TIMER_BYTE_REG(TIMER_GTCFR1_REG_OFFSET,0xFF);\
}while(0)

/*���㶨ʱ��*/
#define TIMER_CLEAR()\
do\
{\
    TIMER_INIT();\
    TIMER_START();\
}while(0)

/*��ȡ��ǰ����ֵ*/
#define GET_CURRENT_COUNTER()\
do\
{\
    dwCurrenCounter = READ_TIMER_WORD32_REG(TIMER_GTCNR1_REG_OFFSET);\
}while(0)

#endif
