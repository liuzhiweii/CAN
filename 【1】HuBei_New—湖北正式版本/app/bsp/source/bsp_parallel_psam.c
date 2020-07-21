/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�ParallelPsamUsr.h
* �ļ���ʶ�� 
* ����ժҪ������psam����ʵ���û��ӿ�Դ����
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/

/***********************************************************
 *                ����ͷ�ļ�                               *
 ***********************************************************/
#include "bsp.h"


	
#include  <stdio.h>
#include  <fcntl.h>      /*�ļ����ƶ���*/
#include  <unistd.h>     /*Unix ��׼��������*/
#include  <errno.h>
#include  <stdlib.h>

#include  <sys/ioctl.h>  /* for ioctl() */
#include  <sys/mman.h>   /* mmap��صĺ궨�� */
#include <asm-generic/ioctl.h> 





/***********************************************************
 *                �ڲ��궨��                               *
***********************************************************/

//#define BSP_DRV_PSAM_USR_CODE_DEBUG

/***********************************************************
 *               ����ȫ�ֱ���                              *
***********************************************************/
static T_UsrPsamDevice s_atUsrPsamDev[]=
{
    {0,BSP_DRV_PSAM0_LINUX_DEVICE,-1},
    {0,BSP_DRV_PSAM1_LINUX_DEVICE,-1},
    {0,BSP_DRV_PSAM2_LINUX_DEVICE,-1},
    {0,BSP_DRV_PSAM3_LINUX_DEVICE,-1}
};


/**********************************************************************
* �������ƣ�bsp_psam_init
* ����������psam��ʼ������
* �����������
* �����������
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/
unsigned int bsp_psam_init (void)
{
    unsigned char ucLoop = 0;
    for(ucLoop = 0;ucLoop < BSP_DRV_PSAM_NR;ucLoop++)
    {
        if(0 == s_atUsrPsamDev[ucLoop].ucStatus)/*֮ǰδ��*/
        {
            s_atUsrPsamDev[ucLoop].sdwPsamDevFd = open(s_atUsrPsamDev[ucLoop].ucDevName, O_RDWR);
            if(s_atUsrPsamDev[ucLoop].sdwPsamDevFd < 0)
            {
                printf("\nopen psam %d error,errno = 0x%08x",ucLoop,errno);
                perror(":"); 
                return 1;
            }
            else
            {
                s_atUsrPsamDev[ucLoop].ucStatus = 1;
            }
        }    
        else
        {
            #ifdef BSP_DRV_PSAM_USR_CODE_DEBUG
                printf("\npsam %d init have been inited!\n",ucLoop);
            #endif
        }
    }
    return 0;
}

/**********************************************************************
* �������ƣ�bsp_psam_active
* ����������psam�����
* ���������ucSockId:���ۺ�
* ���������ptCardPara������PSAM���ÿ�Ƭ����
*           pdwPsamErrorCode����������ʱ�Ĵ�����

* �� �� ֵ��������
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
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
    {
        return 4;
    }
    /*���ͼ�������*/
    sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_ACTIVE, &tCardResetPara);
    if(sdwRet < 0)
    {
        return 5;
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
*           ucCmdLen:�����
*           pucCmdApdu��ָ������ָ��
* ���������
*           pucResLen����Ƭ��Ӧ����
*           pucResTpdu����Ƭ��Ӧ����ָ��
*           pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ ��������
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
        return 6;
    }
    if((NULL == pucCmdApdu) || (NULL == pucResLen) || (NULL == pucResTpdu) || (NULL == pdwPsamErrorCode))
    {
        return 7;
    }
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
    {
        return 8;
    }
  
    if(0 == ucCmdLen)
    {
        return 9;
    }
    tCardProcPara.ucSockId = ucSockId;
    tCardProcPara.ucCmdLen = ucCmdLen;
    memcpy(&tCardProcPara.aucCmd[0], pucCmdApdu, ucCmdLen);
    /*���ͼ�������*/
    sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_PROCESS, &tCardProcPara);
    if(sdwRet < 0)
    {
        return 10;
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
* ���������pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_deactive(unsigned char ucSockId,unsigned int *pdwPsamErrorCode)
{
    int sdwRet = 0;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 11;
    }
    if(NULL == pdwPsamErrorCode)
    {
        return 12;
    }
  
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
    {
        return 13;
    }
    /*����ȥ��������*/
    sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_DEACTIVE);
    if(sdwRet < 0)
    {
        return 14;
    }
    return 0;
}

/**********************************************************************
* �������ƣ�bsp_psam_clkfreq_set
* �����������޸�PSAM��CLK�ķ�Ƶϵ��
* ���������ucClkDiv����Ƶϵ�� 
* �� �� ֵ��������
* ����˵������
* �޸�����	   �汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_clkfreq_set(unsigned char ucClkDiv)
{
    int sdwRet = 0;
    if(s_atUsrPsamDev[0].sdwPsamDevFd < 0)
	{
		return 15;
	}
	if(ucClkDiv > BSP_DRV_PSAM_CLKFLEQ_MAX)
	{
        return 16;
	}
	sdwRet = ioctl(s_atUsrPsamDev[0].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_CLKSET, &ucClkDiv);
    if(sdwRet < 0)
    {
        return 17;
    }
    return 0;		
}

/**********************************************************************
* �������ƣ�bsp_psam_voltage_set
* �����������޸�PSAM���Ĺ����ѹ
* ���������ucSockId:���ۺ�;��ѹֵ 
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_voltage_set(unsigned char ucSockId,unsigned char ucVoltage)
{
    int sdwRet = 0;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 18;
    }
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
	{
		return 19;
	}
	if((ucVoltage != PSAM_VOLTAGE_3V3) && (ucVoltage != PSAM_VOLTAGE_1V8))
	{
        return 20;
	}
	sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_VOLTAGESET, &ucVoltage);
    if(sdwRet < 0)
    {
        return 21;
    }
    return 0;		
}

/**********************************************************************
* �������ƣ�bsp_psam_pps_set
* �����������޸�PSAM���Ƿ�Ƶ
* ���������ucSockId:���ۺ�;ucPPSFlag
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_pps_set(unsigned char ucSockId,unsigned char ucPPSFlag)
{
    int sdwRet = 0;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 22;
    }
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
	{
		return 23;
	}
	if((ucPPSFlag != 0) && (ucPPSFlag != 1))
	{
        return 24;
	}
	sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_SETPPS_STATE, &ucPPSFlag);
    if(sdwRet < 0)
    {
        return 25;
    }
    return 0;		
}


/**********************************************************************
* �������ƣ�bsp_psam_etu_set
* �����������޸�PSAM���Ƿ�ʹ��Ĭ��ETU
* ���������ucSockId:���ۺ�;ucEtuFlag
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_etu_set(unsigned char ucSockId,unsigned char ucEtuFlag)
{
    int sdwRet = 0;
    /*��μ��*/
    if(ucSockId >= BSP_DRV_PSAM_NR)
    {
        return 26;
    }
    if(s_atUsrPsamDev[ucSockId].sdwPsamDevFd < 0)
	{
		return 27;
	}
	if((ucEtuFlag != 0) && (ucEtuFlag != 1))
	{
        return 28;
	}
	sdwRet = ioctl(s_atUsrPsamDev[ucSockId].sdwPsamDevFd, BSP_PSAM_IOCTL_CMD_SETETU_STATE, &ucEtuFlag);
    if(sdwRet < 0)
    {
        return 29;
    }
    return 0;		
}

