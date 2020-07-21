/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� psamUsr.c
* �ļ���ʶ�� 
* ����ժҪ��psam����ʵ���û��ӿ�Դ����
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


	

#include  <stdio.h>
#include  <fcntl.h>      /*�ļ����ƶ���*/
#include  <unistd.h>     /*Unix ��׼��������*/
#include  <errno.h>
#include  <stdlib.h>

#include  <sys/ioctl.h>  /* for ioctl() */
#include  <sys/mman.h>   /* mmap��صĺ궨�� */

#include "Phy7816_Map.h"

#if 0

/***********************************************************
 *                                      �ڲ��궨��                               *
***********************************************************/

#define BSP_DRV_PSAM_USR_CODE_DEBUG
/***********************************************************
 *                                      ���ر���                                     *
***********************************************************/
 static int g_sdwPsamModuleFd = -1;
/**********************************************************************
* �������ƣ�bsp_psam_init
* ����������psam��ʼ������
* �����������
* �����������
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/
unsigned int bsp_psam_init (void)
{
        if(g_sdwPsamModuleFd < 0)/*֮ǰδ��*/
        {
            g_sdwPsamModuleFd = open(BSP_DRV_PSAM_LINUX_DEVICE, O_RDWR);
            if(g_sdwPsamModuleFd < 0)
            {
                #ifdef BSP_DRV_PSAM_USR_CODE_DEBUG
                    perror("\nopen funtion in psam init error:"); 
                #endif
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            #ifdef BSP_DRV_PSAM_USR_CODE_DEBUG
                printf("psam init have been inited!\n");
            #endif
            return 0; 
        }
    return 0;
}
/**********************************************************************
* �������ƣ�bsp_psam_active
* ����������psam�����
* ���������ucSockId:���ۺ�
* ���������
*                               ptCardPara������PSAM���ÿ�Ƭ����
*                               pdwPsamErrorCode����������ʱ�Ĵ�����

* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/
unsigned int bsp_psam_active (unsigned char ucSockId, T_CardPara *ptCardPara,unsigned int *pdwPsamErrorCode)
{
    
    int sdwRet = 0;
    T_CardResetPara tCardResetPara;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 2;
    }
    if((NULL == ptCardPara) || (NULL == pdwPsamErrorCode))
    {
        return 3;
    }
    if(g_sdwPsamModuleFd < 0)
    {
        return 4;
    }

    /*���ͼ�������*/
    tCardResetPara.ucSockId = ucSockId;
    sdwRet = ioctl(g_sdwPsamModuleFd, BSP_PSAM_IOCTL_CMD_ACTIVE, &tCardResetPara);
    return 5;
    if(sdwRet < 0)
    {
        return 6;
    }
    else /*��������*/
    {
        ptCardPara->wEtu = tCardResetPara.wEtu;
        ptCardPara->dwAtrLen = tCardResetPara.dwAtrLen;
        memcpy(ptCardPara->aucAtr, tCardResetPara.aucAtr,tCardResetPara.dwAtrLen);
        ptCardPara->dwHistLen = tCardResetPara.dwHistLen;
        memcpy(ptCardPara->aucHistory, tCardResetPara.aucHistory,tCardResetPara.dwHistLen);
        *pdwPsamErrorCode = tCardResetPara.dwErrCode; 
        return 0;
    }

}
/**********************************************************************
* �������ƣ�bsp_psam_cmd
* ����������psam�������
* ���������ucSockId:���ۺ�
*                               ucCmdLen:�����
*                               pucCmdApdu��ָ������ָ��
* ���������
*                               pucResLen����Ƭ��Ӧ����
*                               pucResTpdu����Ƭ��Ӧ����ָ��
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_cmd(unsigned char ucSockId, unsigned char ucCmdLen, unsigned char *pucCmdApdu, unsigned char *pucResLen, unsigned char *pucResTpdu,unsigned int *pdwPsamErrorCode)
{
    int sdwRet = 0;
    T_CardProcPara tCardProcPara;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 7;
    }
    if((NULL == pucCmdApdu) || (NULL == pucResLen) || (NULL == pucResTpdu) || (NULL == pdwPsamErrorCode))
    {
        return 8;
    }
    if(g_sdwPsamModuleFd < 0)
    {
        return 9;
    }
    if(0 == ucCmdLen)
    {
        return 10;
    }
    tCardProcPara.ucSockId = ucSockId;
    tCardProcPara.ucCmdLen = ucCmdLen;
    memcpy(&tCardProcPara.aucCmd[0], pucCmdApdu, ucCmdLen);
    /*���ͼ�������*/
    sdwRet = ioctl(g_sdwPsamModuleFd, BSP_PSAM_IOCTL_CMD_PROCESS, &tCardProcPara);

    if(sdwRet < 0)
    {
        return 11;
    }
    else  /*��������*/
    {
        *pucResLen = tCardProcPara.ucResLen;
        memcpy(pucResTpdu, &tCardProcPara.aucRes[0], tCardProcPara.ucResLen);
        *pdwPsamErrorCode = tCardProcPara.dwErrCode;
        return 0;
    }

}

/**********************************************************************
* �������ƣ�bsp_psam_deactive
* ����������psamȥ�����
* ���������ucSockId:���ۺ�
* ���������
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_deactive(unsigned char ucSockId,unsigned int *pdwPsamErrorCode)
{
    int sdwRet = 0;
    T_CardResetPara tCardResetPara;
    if(ucSockId >= BSP_DRV_PSAM_NR)/*���ۺŴ���*/
    {
        return 12;
    }
    if(NULL == pdwPsamErrorCode)
    {
        return 13;
    }
    if(g_sdwPsamModuleFd < 0)
    {
        return 14;
    }
    tCardResetPara.ucSockId = ucSockId;
    /*���ͼ�������*/
    sdwRet = ioctl(g_sdwPsamModuleFd, BSP_PSAM_IOCTL_CMD_DEACTIVE, &tCardResetPara);
    if(sdwRet < 0)
    {
        return 15;
    }
    else  /*��������*/
    {
        *pdwPsamErrorCode = tCardResetPara.dwErrCode;
        return 0;
    }
	   
}

/**********************************************************************
* �������ƣ�bsp_psam_clkfreq_set
* �����������޸�PSAM��CLK�ķ�Ƶϵ��
* ���������ucClkDiv����Ƶϵ�� 
* ���������
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_clkfreq_set(unsigned char ucClkDiv)
{

    volatile unsigned char * pucPsamVirtRegAddr = NULL;
    if(g_sdwPsamModuleFd < 0)
    {
    	return 16;
    }
    if(ucClkDiv > BSP_DRV_PSAM_CLKFLEQ_MAX)
    {
        return 17;
    }
    pucPsamVirtRegAddr = (volatile unsigned char *)mmap(NULL, BSP_DRV_PSAM_ADDR_WIDTH,
                PROT_READ | PROT_WRITE, MAP_SHARED, g_sdwPsamModuleFd, 0);
    if(MAP_FAILED == pucPsamVirtRegAddr)
    {
    	return 18;
    }
    pucPsamVirtRegAddr [PSAM_CLK_DIV_REG_OFFSET] = ucClkDiv;

    return 0;

}
#endif

