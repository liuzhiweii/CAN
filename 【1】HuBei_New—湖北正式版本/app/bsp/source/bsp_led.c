/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� oss_led.c
* �ļ���ʶ�� 
* ����ժҪ��oss_led����ʵ���û�̬�ӿڳ���
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

/***********************************************************
 *                                       ��������                                       *
***********************************************************/
#define OSS_LED_ALARM                                        (0)
#define OSS_LED_RUN                                          (1)
#define OSS_LED_PSAM                                         (2)

volatile unsigned short * g_pvBSPDrvLedVirtStartAddr0 = NULL;

#define WRITE_LED_REG(OFFSET,DATA)   g_pvBSPDrvLedVirtStartAddr0[OFFSET]=(T_EpldData)DATA
#define READ_LED_REG(OFFSET)         g_pvBSPDrvLedVirtStartAddr0[OFFSET]


//Led����λ����
static unsigned int s_aLedList[][2]=
{
    {BSP_DRV_LED_ALARM_MASK, BSP_DRV_LED_ALARM_EN},
    {BSP_DRV_LED_RUN_MASK, BSP_DRV_LED_RUN_EN},
    {BSP_DRV_LED_PSAM_MASK, BSP_DRV_LED_PSAM_EN}
};

/**************************************************************************
* �������ƣ�unsigned int bsp_led_set
* ��������������led״̬����
* ���ʵı���
* �޸ĵı���
* ���������dwLedNo:led��;dwState:״̬,
*                               0:OFF;1:ON;
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
static unsigned int bsp_led_set(unsigned int dwLedIndex, unsigned int dwState)
{
    unsigned int dwData = 0;

    dwData = READ_LED_REG(BSP_DRV_LED_LED_DATA_OFFSET);
    if (dwState)
    {
        if (s_aLedList[dwLedIndex][1])
        {
            dwData |= s_aLedList[dwLedIndex][0];
        }
        else
        {
            dwData &= (~s_aLedList[dwLedIndex][0]);
        }
    }
    else
    {
        if (s_aLedList[dwLedIndex][1])
        {
            dwData &= (~s_aLedList[dwLedIndex][0]);
        }
        else
        {
            dwData |= s_aLedList[dwLedIndex][0];
        }
    }
    WRITE_LED_REG(s_aLedList[dwLedIndex][0], dwData);
    return 0;
}
/**************************************************************************
* �������ƣ�unsigned int oss_led_run_ctl
* ����������run  �� ���ƺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_led_initl(void)
{
    g_pvBSPDrvLedVirtStartAddr0 = p_epld_start_addr;
    return 0;
}
/**************************************************************************
* �������ƣ�unsigned int oss_led_run_ctl
* ����������run  �� ���ƺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_led_run_ctl(unsigned char ucState)
{
    unsigned short reg_value;
	
    reg_value = p_epld_start_addr[0x00DA>>1];

	reg_value = (reg_value & 0xfffd);

	ucState = (ucState == 0) ? 0x02: 0;
	
    reg_value |= ucState;

	p_epld_start_addr[0x00DA>>1] = reg_value;
	
	#if 0
    switch (ucState)
    {
        case 0:
        {
            bsp_led_set(OSS_LED_RUN, 0);
            break;
        }
        case 1:
        {
            bsp_led_set(OSS_LED_RUN, 1);
            break;
        }
        default:
        {
            break;
        }
    }
	#endif
	
    return 0;
}

/**************************************************************************
* �������ƣ�unsigned int oss_led_alarm_ctl
* ����������alarm  �� ���ƺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����ucState:0 for off,1 for on
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_led_alarm_ctl(unsigned char ucState)
{

	unsigned short reg_value;
		
	reg_value = p_epld_start_addr[0x00DA>>1];
	
	reg_value = (reg_value & 0xfffe);
	
	ucState = (ucState == 0) ? 0x01: 0;
		
	reg_value |= ucState;
	
	p_epld_start_addr[0x00DA>>1] = reg_value;
		

    #if 0
    switch (ucState)
    {
        case 0:
        {
            bsp_led_set(OSS_LED_ALARM, 0);
            break;
        }
        case 1:
        {
            bsp_led_set(OSS_LED_ALARM, 1);
            break;
        }
        default:
        {
            break;
        }
    }
	#endif
	
    return 0;
}

/**************************************************************************
* �������ƣ�unsigned int oss_led_psam_ctl
* ����������PSAM  �� ���ƺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����ucState:0 for off,1 for on
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_led_psam_ctl(unsigned char ucState)
{
    unsigned short reg_value;
		
	reg_value = p_epld_start_addr[0x00DA>>1];
	
	reg_value = (reg_value & 0xfffb);
	
	ucState = (ucState == 0) ? 0x04: 0;
		
	reg_value |= ucState;
	
	p_epld_start_addr[0x00DA>>1] = reg_value;
	
    #if 0
    switch (ucState)
    {
        case 0:
        {
            bsp_led_set(OSS_LED_PSAM, 0);
            break;
        }
        case 1:
        {
            bsp_led_set(OSS_LED_PSAM, 1);
            break;
        }
        default:
        {
            break;
        }
    }
	#endif
    return 0;
}



