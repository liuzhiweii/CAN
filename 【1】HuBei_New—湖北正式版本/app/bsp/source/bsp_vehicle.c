/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� verUsr.c
* �ļ���ʶ�� 
* ����ժҪ��ver����ʵ���û�̬�ӿڳ���
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

#include <fcntl.h>
#include <sys/mman.h> 


/***********************************************************
 *                                      ȫ�ֱ���                                         *
***********************************************************/


/***********************************************************
 *                                      ���ر���                                         *
***********************************************************/
static volatile int s_sdwDrvVerFd   = -1;
static volatile unsigned int s_dwDrvVerInitFag  = 0;

/**************************************************************************
* �������ƣ�unsigned int bsp_ver_init
* ����������������ģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_ver_init(void)
{
    unsigned int dwRetVal= 0;
    if (0 == s_dwDrvVerInitFag)
    {
        s_sdwDrvVerFd = open(BSP_DRV_VER_LINUX_DEVICE, O_RDWR);
        if (-1 != s_sdwDrvVerFd)
        {
            s_dwDrvVerInitFag = 1;
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
* �������ƣ�bsp_ver_read
* ������������ver֡���ݺ���
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
unsigned int bsp_ver_read(unsigned char* pucDataPacket,unsigned int dwDataLen,unsigned int *pdwReadLen)
{
    int sdwRet = 0;
    unsigned int dwVerEven = 0;


    if((NULL == pucDataPacket) || (NULL == pdwReadLen))
    {
        return 2;
	}

    if(s_sdwDrvVerFd <= 0)
    {
        return 3;
	}
	sdwRet = read(s_sdwDrvVerFd,pucDataPacket,dwDataLen);
	if(sdwRet < 0)
	{
		printf("bsp_ver_read read faile.error = 0x%x\r\n", sdwRet);
		return 4;
	}
	else
	{
        *pdwReadLen = sdwRet;
		return 0;
	}

}
/**************************************************************************
* �������ƣ�bsp_ver_ctrl
* ��������������������
* ���ʵı���
* �޸ĵı���
* ���������dwCtrlMod��ver��������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_ver_ctrl(unsigned int dwCtrlMod)
{
        int sdwRet = 0;

        sdwRet = ioctl(s_sdwDrvVerFd,dwCtrlMod);
        if (sdwRet < 0)
        {
            printf("bsp_ver_ctrl ioctl error = 0x%08x",sdwRet);
            return 5;
        }

        return 0;
}

