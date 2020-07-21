/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� timerUsr.c
* �ļ���ʶ�� 
* ����ժҪ��timer����ʵ���û�̬�ӿڳ���
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/

/***********************************************************
 *                                      ����ͷ�ļ�                                   *
 ***********************************************************/

#include "bsp.h"

#include "timer_def.h"

#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h> 
#include <stdio.h>



/***********************************************************
 *                                       ��������                                       *
***********************************************************/


/***********************************************************
*                             �ļ��ڲ�ʹ�õĺ�                             *
***********************************************************/
#define BSP_DRV_TIMER_USR_CODE_DEBUG

/***********************************************************
*                     �ļ��ڲ�ʹ�õ���������                    *
***********************************************************/


/***********************************************************
 *                                      ȫ�ֱ���                                         *
***********************************************************/

volatile T_TimerAddr *g_pTimerUsrSpaceBaseAddr = NULL;


/***********************************************************
 *                                      ���ر���                                         *
***********************************************************/

static volatile int s_TimerFd = -1;



/**************************************************************************
* �������ƣ�bsp_timer_init
* ����������timerģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_timer_init(void)
{
    if (s_TimerFd < 0)
    {
        s_TimerFd = open(BSP_DRV_TIMER_LINUX_DEVICE, O_RDWR);
		if (s_TimerFd > 0)
        {
            g_pTimerUsrSpaceBaseAddr = (T_TimerAddr *)mmap(0, BSP_DRV_TIMER_ADDR_WIDTH, 
                                                      PROT_READ | PROT_WRITE, 
                                                      MAP_SHARED, s_TimerFd, 0);
            
			if (MAP_FAILED == (void *)g_pTimerUsrSpaceBaseAddr)
            {
			    printf("mmap funtion in Timer init error,ErrorCode = 0x%08x!\n",(unsigned int)g_pTimerUsrSpaceBaseAddr);
				return 1;
            }
            else
            {
                #ifdef BSP_DRV_TIMER_USR_CODE_DEBUG
			        printf("Timer mmap Succeed !\n");
		            printf("The base address of Timer in physical space = 0x%08x \n", (unsigned int)BSP_DRV_TIMER_BASE_ADDR);
		            printf("The base address of Timer in usr space = 0x%08x \n", (unsigned int)g_pTimerUsrSpaceBaseAddr);
			    #endif
                TIMER_INIT();
                TIMER_START();
				return 0;
            }
        }
        else
        {

			printf("open funtion in Timer init error,ErrorCode = 0x%08x!\n",s_TimerFd);
			return 1;
        }
    }
	else
	{
        printf("timer has been inited!\n");
		return 0;
	}
	
}

/**************************************************************************
* �������ƣ�bsp_timer_get_counter
* ������������ȡ��ǰ����ֵ
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
**************************************************************************/	
unsigned int bsp_timer_get_counter(unsigned int *pdwCounter)
{
    unsigned int dwCurrenCounter = 0;
    GET_CURRENT_COUNTER();
    *pdwCounter = dwCurrenCounter;
    return 0;
}
/**************************************************************************
* �������ƣ�bsp_timer_control
* ����������������ֹͣ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/	
unsigned int bsp_timer_control(unsigned int dwCmd)
{
  
	/*������ʱ��,�Ӽ����Ĵ���ԭ��ֵ��ʼ����*/
	if(BSP_DRV_TIMER_START== dwCmd) 
    {
        TIMER_START();
    }
	/*ֹͣ��ʱ��,�����Ĵ���ֵ����Ϊֹͣʱ�̵�ֵ*/
	else if(BSP_DRV_TIMER_STOP == dwCmd)
	{
		TIMER_STOP();
    }
	/*���㶨ʱ��,�����Ĵ������㿪ʼ����*/
	else if(BSP_DRV_TIMER_CLEAR == dwCmd)
	{
		TIMER_CLEAR();
    }
	else
	{
        return 2;
	}

	return 0;
}

/**************************************************************************
* �������ƣ�bsp_timer_usleep
* ����������΢�뼶����ӳٺ���
* ���ʵı���
* �޸ĵı���
* ���������unsigned int dwUsec: Ҫ�ӳٵ�΢����
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
void bsp_timer_usleep(unsigned int dwUsec)
{    
    unsigned int           dwEnd;
    unsigned int           dwPower = 0;
    volatile unsigned int  dwPre;
    volatile unsigned int  dwNow;
    #if (BSP_CPU_TYPE == BSP_CPU_MPC8241)       /* 13MHz, 31Bits, Subtracter */
	    bsp_timer_get_counter(&dwNow);
	    dwEnd = dwNow - dwUsec * 13;

	    do
	    {
	        dwPre = dwNow;
	        bsp_timer_get_counter(&dwNow);
	        if (dwNow > dwPre)
	        {
	            dwPower = 0x80000000;            
	        }
	        dwNow += dwPower;
	    } while ((int)(dwEnd) - (int)(dwNow) < 0);    
	#else                           /* 1MHz, 32Bits, Adder */
	    bsp_timer_get_counter(&dwNow);
	    dwEnd = dwNow + dwUsec;
	    
	    do
	    {        
	        bsp_timer_get_counter(&dwNow);
	    } while ((int)(dwNow) - (int)(dwEnd) < 0);
	#endif


    return;
}
/**************************************************************************
* �������ƣ�bsp_timer_msleep
* �������������뼶����ӳٺ���
* ���ʵı���
* �޸ĵı���
* ���������unsigned int dwMsec: Ҫ�ӳٵĺ�����
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_timer_msleep(unsigned int dwMsec)
{    
    #ifdef TIMER_USE_INT_MODE
		int sdwRet = 0;
		if(s_TimerFd <= 0)
	    {
	        return BSP_ERR_DRV_TIMER_INIT_FD;
		}
		dwMsec = dwMsec * 10;/*10K Hz*/
		if(dwMsec > 0xFFFF)
		{
            return BSP_ERR_DRV_TIMER_CONTROL_PARA;
		}
		sdwRet = ioctl(s_TimerFd,TIMER_SLEEP_SET,&dwMsec);
		if (sdwRet < 0)
		{
	        printf("bsp_timer_msleep ioctl error = 0x%08x",sdwRet);
		    return BSP_ERR_DRV_TIMER_CONTROL_PARA;
		}
	#endif
    return 0;
}	



