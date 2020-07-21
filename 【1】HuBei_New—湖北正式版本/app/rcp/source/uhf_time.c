#include "usp.h"
#if (UHF_PROTOCOL_USE)

void GetCurrUsTime(struct timeval *ptTimeVal)
{
    //struct timeval get_time1;

    if (gettimeofday(ptTimeVal, NULL) != 0)
    {
        //usp_oss_printf(USP_UHF_TNO, USP_OSS_PRINTF_LEVEL_DEBUG, "can not get time\r\n") ;

    }
    else
    {
        //printf("UHF: time: %ld second : %ld us \r\n", ptTimeVal->tv_sec, ptTimeVal->tv_usec) ;
    }
}

/**********************************************************************
* �������ƣ�static void uhf_startRcpTimer( void )
* ����������������ʱ����
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
void uhf_startRcpTimer(WORD32 dwMS)
{
    struct itimerval tim_ticks;
    tim_ticks.it_value.tv_sec = (dwMS / 1000); //s
    tim_ticks.it_value.tv_usec = (dwMS % 1000) * 1000; //us
    tim_ticks.it_interval.tv_sec = (dwMS / 1000);
    tim_ticks.it_interval.tv_usec = (dwMS % 1000) * 1000;
    //10; /* ����it_valueΪ0;���ߵ�������������Ϻ󣬶�it_intervalΪ0����ʱ���������ֹͣ������ */

    setitimer(ITIMER_REAL, &tim_ticks, 0);
} /* static void uhf_startRcpTimer( void ) */

/**********************************************************************
* �������ƣ�static void uhf_stopRcpTimer( void )
* ����������ֹͣ��ʱ����
* ���������
* ���������
* �� �� ֵ��
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2006/9/16    V0.0.1       ft          ����
************************************************************************/
void uhf_stopRcpTimer(void)
{
    struct itimerval tim_ticks;
    tim_ticks.it_value.tv_sec = 0; //s
    tim_ticks.it_value.tv_usec = 0; //us
    tim_ticks.it_interval.tv_sec = 0;
    tim_ticks.it_interval.tv_usec = 0; //10; /* ����it_valueΪ0;���ߵ�������������Ϻ󣬶�it_intervalΪ0����ʱ���������ֹͣ������ */

    setitimer(ITIMER_REAL, &tim_ticks, 0);
} /* static void uhf_stopRcpTimer( void ) */

WORD32 CalMsTimeInterval(struct timeval *ptStartTime, struct timeval *ptCurrTime)
{
    WORD32 use_ms_time;
    use_ms_time = (ptCurrTime->tv_sec - ptStartTime->tv_sec) * 1000
                  + ((ptCurrTime->tv_usec - ptStartTime->tv_usec)/1000) ;
    return (use_ms_time);
}


#endif

