/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvVer.h
* �ļ���ʶ��
* ����ժҪ�������� ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�       
* ������ڣ�
************************************************************************/

#ifndef _BSP_DRV_VER_H_
#define _BSP_DRV_VER_H_
/**************************************************************************
*                                           ȫ�ֺ���ԭ��                                                   *
**************************************************************************/

#define BSP_DRV_VER_LINUX_DEVICE "/dev/misc/drv_ver"

    //ʹ�ܰ����ж�
#define    VER_ENABLE_MODE    (0x00)
    //��ֹ�����ж�
#define    VER_DISABLE_MODE   (0x01)
    
unsigned int bsp_ver_init(void);
//�����������سɹ���ʾ�а����ж�
unsigned int bsp_ver_read(unsigned char* pucDataPacket,unsigned int dwDataLen,unsigned int *pdwReadLen);

unsigned int bsp_ver_ctrl(unsigned int dwCtrlMod);
#endif/* _BSP_DRV_VER_H_  */

