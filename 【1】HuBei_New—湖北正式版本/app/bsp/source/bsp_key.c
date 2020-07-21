/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� keyUsr.c
* �ļ���ʶ�� 
* ����ժҪ��key����ʵ���û�̬�ӿڳ���
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

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h> 


/***********************************************************
 *                                      ȫ�ֱ���                                         *
***********************************************************/


/***********************************************************
 *                                      ���ر���                                         *
***********************************************************/
static volatile int s_sdwDrvKeyFd   = -1;
static volatile unsigned int s_dwDrvKeyInitFag  = 0;

/**************************************************************************
* �������ƣ�unsigned int bsp_key_init
* �������������� ģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_key_init(void)
{
    unsigned int dwRetVal= 0;
    if (0 == s_dwDrvKeyInitFag)
    {
        s_sdwDrvKeyFd = open(BSP_DRV_KEY_LINUX_DEVICE, O_RDWR);
        if (-1 != s_sdwDrvKeyFd)
        {
            s_dwDrvKeyInitFag = 1;
        }
        else
        {
            dwRetVal =  1;
        }
    }
    else
    {
        return 0;
    }
    return dwRetVal;
}
/**************************************************************************
* �������ƣ�bsp_key_read
* ������������key֡���ݺ���
* ���ʵı���
* �޸ĵı���
* ���������pucDataPacket���������ݴ��Ŀ��ָ��
*           dwDataLen��Ҫ��ȡ���ݵĳ���
* ���������pdwReadLen��ʵ�ʶ������ݵĳ���
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_key_read(unsigned char* pucDataPacket,unsigned int dwDataLen,unsigned int *pdwReadLen)
{
    int sdwRet = 0;

    #if 0
    if((NULL == pucDataPacket) || (NULL == pdwReadLen))
    {
        return BSP_ERROR_DRV_KEY_READ_FRAME_PARA;
	}
	#endif
    if(s_sdwDrvKeyFd <= 0)
    {
        return 2;
	}
	sdwRet = read(s_sdwDrvKeyFd,pucDataPacket,dwDataLen);
	if(sdwRet < 0)
	{
		printf("bsp_key_read read faile.error = 0x%x\r\n", sdwRet);
		return 3;
	}
	else
	{
        *pdwReadLen = sdwRet;
		return 0;
	}

}
/**************************************************************************
* �������ƣ�bsp_key_ctrl
* ����������key����
* ���ʵı���
* �޸ĵı���
* ���������dwCtrlMod��key��������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_key_ctrl(unsigned int dwCtrlMod)
{
        int sdwRet = 0;

        sdwRet = ioctl(s_sdwDrvKeyFd,dwCtrlMod);
        if (sdwRet < 0)
        {
            printf("bsp_key_ctrl ioctl error = 0x%08x",sdwRet);
            return 4;
        }

        return 0;
}


