/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvKey.h
* �ļ���ʶ�� 
* ����ժҪ�������жϽӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� 
* ������ڣ� 
************************************************************************/
#ifndef _BSP_OSS_KEY_H
#define _BSP_OSS_KEY_H
//ʹ�ܰ����ж�
#define    KEY_ENABLE_MODE    (0x00)
//��ֹ�����ж�
#define    KEY_DISABLE_MODE   (0x01)

#define BSP_DRV_KEY_LINUX_DEVICE "/dev/misc/drv_key"


unsigned int bsp_key_init(void);
//�����������سɹ���ʾ�а����ж�
unsigned int bsp_key_read(unsigned char* pucDataPacket,unsigned int dwDataLen,unsigned int *pdwReadLen);

unsigned int bsp_key_ctrl(unsigned int dwCtrlMod);
#endif/*  _BSP_OSS_KEY_H  */

