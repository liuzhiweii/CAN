/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_ossLed.h
* �ļ���ʶ�� 
* ����ժҪ�� ���ƹ������ӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� 
* ������ڣ� 
************************************************************************/
#ifndef _BSP_OSS_LED_H
#define _BSP_OSS_LED_H

#define BSP_DRV_LED_LED_DATA_OFFSET     (0xDA>>1)
#define BSP_DRV_LED_ALARM_MASK          (0x01)
#define BSP_DRV_LED_ALARM_EN            (0x00)
#define BSP_DRV_LED_RUN_MASK            (0x02)
#define BSP_DRV_LED_RUN_EN              (0x00)
#define BSP_DRV_LED_PSAM_MASK           (0x04)
#define BSP_DRV_LED_PSAM_EN             (0x00)

unsigned int bsp_led_initl(void);
//���е�ucState:1�� 0:��
unsigned int bsp_led_run_ctl(unsigned char ucState);
//�澯��ucState:1�� 0:��
unsigned int bsp_led_alarm_ctl(unsigned char ucState);
//PSAM״ָ̬ʾ��ucState:1�� 0:��
unsigned int bsp_led_psam_ctl(unsigned char ucState);
#endif/*  _BSP_OSS_LED_H  */

