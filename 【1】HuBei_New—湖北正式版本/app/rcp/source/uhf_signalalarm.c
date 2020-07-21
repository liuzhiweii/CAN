#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
/* UHFͳ����Ϣ  */
typedef struct
{
    WORD32 dwSigIONum;
    WORD32 dwSigINTNum;
    WORD32 dwSigNum;

    WORD32 dwEpcInvnetErrNum;
    WORD32 dwEpcInvnetSignalNum;

    WORD32 dwIsoSignalNum;
    WORD32 dwIsoTagNum;
} T_UhfStatis;

/* Rcpͳ����Ϣ  */
static T_UhfStatis tRcpStatis;


/**********************************************************************
* �������ƣ�static void signal_io_process(int sig)
* ����������SIGIO�źŴ�����
* ���ʵı���
* �޸ĵı���
* ���������int sig ���ź�����
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static void signal_io_process(int sig)
{
    tRcpStatis.dwSigIONum++;
}

/**********************************************************************
* �������ƣ�static void signal_alarm_process(int sig)
* ����������SIGALRM�źŴ�����
* ���ʵı���
* �޸ĵı���
* ���������int sig ���ź�����
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
static void signal_alarm_process(int sig)
{
    tRcpStatis.dwSigINTNum++;
    printf("sigalarm happened\r\n");
}
/**********************************************************************
* �������ƣ�WORD32 InitSIGALRM(void)
* ������������װ SIGALRM �źŴ�����
* ���������
* ���������
* �� �� ֵ��WORD32
                    0��    �ɹ�
                 ������    ʧ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
WORD32 InitUhfSIGALRM(void)
{
    struct sigaction act;
    int iRetVal;

    sigemptyset(&act.sa_mask);
    act.sa_handler = signal_alarm_process;
    act.sa_flags = 0;

    /* bind the signal process function */
    iRetVal = sigaction(SIGALRM, &act, 0);

    if (iRetVal != 0)
    {
        perror("UHF: sigaction(SIGALRM) error ");
        exit(1);
    }

    return 0;

} /* WORD32 InitSIGALRM(void) */
#endif

