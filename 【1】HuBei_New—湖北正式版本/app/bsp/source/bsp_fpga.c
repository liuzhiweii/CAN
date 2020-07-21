/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� fpgaUsr.c
* �ļ���ʶ�� 
* ����ժҪ��fpga����ʵ���û�̬�ӿڳ���
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/
/***********************************************************
 *                                      ����ͷ�ļ�                                   *
 ***********************************************************/

#include "bsp/bsp_fpga.h"

#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h> 
#include <asm-generic/ioctl.h> 
#include <stdio.h>



/***********************************************************
 *                                      ȫ�ֱ���                                    *
***********************************************************/
/*FPGA�û��ռ��׵�ַ,���ڼ��ݾɵ�Ӧ�ó���,
   ��Ӧ�ó��򲻽���ʹ��*/
T_FpgaVirtAddr g_pvBSPDrvFpgaVirtStartAddr0 = NULL;


/***********************************************************
 *                                      ���ر���                                    *
***********************************************************/
/*FPGA�豸�ļ�������*/	
static int  s_sdwFpgaFd = -1; 

/*FPGA�û��ռ��ַ*/
static T_FpgaVirtAddr  s_pFpgaUsrSpaceBaseAddr = NULL;	 


/***********************************************************
 *                                      �ڲ��궨��                                *
***********************************************************/
/*д����*/
#undef WRITE_FPGA_REG
#define WRITE_FPGA_REG(OFFSET,DATA)   s_pFpgaUsrSpaceBaseAddr[OFFSET]=(T_FpgaData)DATA

/*������*/
#undef READ_FPGA_REG
#define READ_FPGA_REG(OFFSET)         (T_FpgaData)s_pFpgaUsrSpaceBaseAddr[OFFSET]

/*��ӡ����*/
//#define BSP_DRV_FPGA_DEBUG


/**************************************************************************
* �������ƣ�bsp_fpga_init
* ����������fpga ģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_fpga_init(void)
{
    int sdwRet = 0;
	if (s_sdwFpgaFd < 0)/*δ��ʼ��*/
	{
	    s_sdwFpgaFd = open("/dev/misc/drv_fpga", O_RDWR);
		if (s_sdwFpgaFd > 0)
		{
             s_pFpgaUsrSpaceBaseAddr = (T_FpgaVirtAddr)mmap(0, BSP_DRV_FPGA_ADDR_WIDTH, 
                                                           PROT_READ | PROT_WRITE, 
                                                           MAP_SHARED, s_sdwFpgaFd, 0);
             
             if (NULL == s_pFpgaUsrSpaceBaseAddr)
             {
                 close(s_sdwFpgaFd);
                 return  1;
             }
			 else
			 {
                g_pvBSPDrvFpgaVirtStartAddr0 = s_pFpgaUsrSpaceBaseAddr;


			         printf("FPGA mmap Succeed !\n");
		             printf("The base address of FPGA in physical space = 0x%08x \n", (unsigned int)BSP_DRV_FPGA_BASE_ADDR);
		             printf("The base address of FPGA in usr space = 0x%08x \n", (unsigned int)s_pFpgaUsrSpaceBaseAddr);

				 return 0;
		     }
         }
		 else
		 {
			 return 3;
		 }
		
	}
    else
    {
        printf("fpga fd have been inited\r\n");
		return 0;
	}
	
}
/**************************************************************************
* �������ƣ�bsp_fpga_read_reg
* ������������FPGA�Ĵ�������
* ���ʵı���
* �޸ĵı���
* ���������dwRegOffset��FPGA�Ĵ���ƫ��
* ���������pwRegData�������Ĵ������ݵ�ָ��
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_fpga_read_reg(unsigned int dwRegOffset,T_FpgaData *ptRegData)
{
    unsigned int sdwRet = 0;

    if(s_sdwFpgaFd < 0)
    {
        sdwRet = bsp_fpga_init();
        if(sdwRet != 0)
        {
            return sdwRet;
        }
    }

    if(NULL == ptRegData)
    {
        return 4;
	}
	if(dwRegOffset > BSP_DRV_FPGA_ADDR_WIDTH)
	{
        return 5;
	}
	*ptRegData = READ_FPGA_REG(dwRegOffset);
	#if 0
	sdwRet = ioctl(s_sdwFpgaFd,FPGA_READ_REG, &dwRegOffset);
	if (sdwRet < 0)
	{
        printf("bsp_fpga_read_reg ioctl error = 0x%08x\r\n",sdwRet);
	    return BSP_ERROR_DRV_FPGA_WRITE_FRAME_IOCTL;
	}
	*ptRegData = dwRegOffset;
    #endif
    return 0;
}
/**************************************************************************
* �������ƣ�bsp_fpga_write_reg
* ����������дFPGA�Ĵ�������
* ���ʵı���
* �޸ĵı���
* ���������wRegOffset��FPGA�Ĵ���ƫ��
*           wRegData����Ҫд�������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/

unsigned int bsp_fpga_write_reg(unsigned int dwRegOffset,T_FpgaData tRegData)
{
    unsigned int sdwRet = 0;

    if(s_sdwFpgaFd < 0)
    {
        sdwRet = bsp_fpga_init();
        if(sdwRet != 0)
        {
            return sdwRet;
        }
    }
    
    if(dwRegOffset > BSP_DRV_FPGA_ADDR_WIDTH)
	{
        return 6;
	}
	
	WRITE_FPGA_REG(dwRegOffset,tRegData);
	//printf("bsp_fpga_write_frame write ok");
	#if 0
	sdwRet = ioctl(s_sdwFpgaFd,FPGA_WRITE_REG, &dwRegOffset);
	if (sdwRet < 0)
	{
        printf("bsp_fpga_write_reg ioctl error = 0x%08x\r\n",sdwRet);
	    return BSP_ERROR_DRV_FPGA_WRITE_FRAME_IOCTL;
	}
    #endif
	return 0;
}
/**************************************************************************
* �������ƣ�bsp_fpga_write_frame
* ����������дFPGA֡���ݺ���
* ���ʵı���
* �޸ĵı���
* ���������pucDataPacket��Ҫд������ݰ�ָ��
*           dwDataLen��Ҫд�����ݵĳ���
*           ucflag��д֡���ݱ�־,����ģʽ��ǻ���ģʽ(8900)
* ���������pdwWriteLen��ʵ��д�����ݵĳ���
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/

unsigned int bsp_fpga_write_frame(unsigned char*pucDataPacket,unsigned int dwDataLen, unsigned char ucflag, unsigned int *pdwWriteLen)
{
    int sdwRet = 0;
	if((NULL == pucDataPacket) || (NULL == pdwWriteLen))
    {
        return 7;
	}
	/*�����Ƿ���ȷ*/
    if (dwDataLen > FPGA_SEND_FIFO_WIDTH)
	{
        return 8;
	}
	/*�ļ��������Ƿ���ȷ*/
	if(s_sdwFpgaFd <= 0)
    {
        return 9;
	}
	/*���û��ѱ�־*/
    if(BSP_DRV_FPGA_WAKEUP_ENABLE == ucflag)
    {
        sdwRet = ioctl(s_sdwFpgaFd,FPGA_ENABLE_WAKEUP_MODE);
	}
	else if(BSP_DRV_FPGA_WAKEUP_DISABLE == ucflag)
	{
        sdwRet = ioctl(s_sdwFpgaFd,FPGA_DISABLE_WAKEUP_MODE);
	}
	else
	{
        return 10;
	}
	if (sdwRet < 0)
	{
        printf("bsp_fpga_write_frame ioctl error = 0x%08x",sdwRet);
	    return 11;
	}
	/*��������*/
	sdwRet = write(s_sdwFpgaFd,pucDataPacket,dwDataLen);
	if(sdwRet < 0)
	{
       printf("bsp_fpga_write_frame write error = 0x%08x",sdwRet);
       return 12;
	}
	else
	{
       *pdwWriteLen = sdwRet;
	   return 0;
	}
}
/**************************************************************************
* �������ƣ�bsp_fpga_read_frame
* ������������FPGA֡���ݺ���
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
unsigned int bsp_fpga_read_frame(unsigned char* pucDataPacket,unsigned int dwDataLen,unsigned int *pdwReadLen)
{
    int sdwRet = 0;
    if((NULL == pucDataPacket) || (NULL == pdwReadLen))
    {
        return 13;
	}
    if(s_sdwFpgaFd <= 0)
    {
        return 14;
	}
	sdwRet = read(s_sdwFpgaFd,pucDataPacket,dwDataLen);
	if(sdwRet < 0)
	{
		perror("\nbsp_fpga_read_frame read error:");
		return 15;
	}
	else
	{
        *pdwReadLen = sdwRet;
		return 0;
	}
}
/**************************************************************************
* �������ƣ�bsp_fpga_reset
* ����������FPGA ��λ����
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_fpga_reset(void)
{
    /*��λFPGA�ڲ�FIFO*/
	//FPGA_FIFO_RESET();
	return 0;

}
/**************************************************************************
* �������ƣ�bsp_fpga_int_enable
* ����������FPGA �ж�ʹ�ܺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/

unsigned int bsp_fpga_int_enable(void)
{
	int sdwRet = 0;
	if(s_sdwFpgaFd <= 0)
    {
        return 16;
	}
	sdwRet = ioctl(s_sdwFpgaFd,FPGA_ENABLE_INTERRUPT);
	if (sdwRet < 0)
	{
        printf("bsp_fpga_int_enable ioctl error = 0x%08x",sdwRet);
	    return 17;
	}
	return 0;
}
/**************************************************************************
* �������ƣ�bsp_fpga_int_disenable
* ����������FPGA �жϽ��ܺ���
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/

unsigned int bsp_fpga_int_disable(void)
{
    int sdwRet = 0;
	if(s_sdwFpgaFd <= 0)
    {
        return 18;
	}
	sdwRet = ioctl(s_sdwFpgaFd,FPGA_DISABLE_INTERRUPT);
	if (sdwRet < 0)
	{
        printf("bsp_fpga_int_disable ioctl error = 0x%08x",sdwRet);
	    return 29;
	}
	return 0;
}



