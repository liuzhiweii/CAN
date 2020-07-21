/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� DeviceCtl.c
* �ļ���ʶ�� 
* ����ժҪ�������Դ��λ����
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/

/***********************************************************
 *                                      ����ͷ�ļ�                               *
 ***********************************************************/
#include "bsp.h"

/***********************************************************
 *                                       ��������                                   *
/***********************************************************/
volatile unsigned short * g_pvBSPDrvDeviceCtlVirtStartAddr0 = NULL;
volatile unsigned short * g_pvFpgaDrvDeviceCtlVirtStartAddr0 = NULL;


#define WRITE_DEVICE_REG(OFFSET,DATA)   g_pvBSPDrvDeviceCtlVirtStartAddr0[OFFSET]=(unsigned short)DATA
#define READ_DEVICE_REG(OFFSET)         g_pvBSPDrvDeviceCtlVirtStartAddr0[OFFSET]

#define WRITE_DEVICE_FPGA_REG(OFFSET,DATA)   g_pvFpgaDrvDeviceCtlVirtStartAddr0[OFFSET]=(unsigned short)DATA
#define READ_DEVICE_FPGA_REG(OFFSET)         g_pvFpgaDrvDeviceCtlVirtStartAddr0[OFFSET]


static unsigned int s_aDevRstMaskCtl[][2]=
{
    {BSP_DRV_DEVICE_RST_USB_MASK, BSP_DRV_DEVICE_RST_USB_EN},
    {BSP_DRV_DEVICE_RST_752_MASK, BSP_DRV_DEVICE_RST_752_EN},
    {BSP_DRV_DEVICE_RST_ETH2_MASK, BSP_DRV_DEVICE_RST_ETH2_EN},
    {BSP_DRV_DEVICE_RST_ETH1_MASK, BSP_DRV_DEVICE_RST_ETH1_EN}
};


static unsigned int s_aDevPowMaskCtl[][2]=
{
    {BSP_DRV_DEVICE_POW_RF_MASK, BSP_DRV_DEVICE_POW_RF_EN},
    {BSP_DRV_DEVICE_POW_SD_MASK, BSP_DRV_DEVICE_POW_SD_EN},
    {BSP_DRV_DEVICE_POW_OLED_MASK, BSP_DRV_DEVICE_POW_OLED_EN},
    {BSP_DRV_DEVICE_POW_USB_MASK, BSP_DRV_DEVICE_POW_USB_EN}
};

static unsigned int s_aDevPowFpgaMaskCtl[][2]=
{
    {BSP_DRV_DEVICE_POW_PSAM0_MASK, BSP_DRV_DEVICE_POW_PSAM0_EN},
    {BSP_DRV_DEVICE_POW_PSAM1_MASK, BSP_DRV_DEVICE_POW_PSAM1_EN},
    {BSP_DRV_DEVICE_POW_PSAM2_MASK, BSP_DRV_DEVICE_POW_PSAM2_EN},
    {BSP_DRV_DEVICE_POW_PSAM3_MASK, BSP_DRV_DEVICE_POW_PSAM3_EN},
};

static unsigned int s_aDevPowFpgaOffestCtl[]=
{
    BSP_DRV_DEVICE_PSAM0_POW_OFFSET,
    BSP_DRV_DEVICE_PSAM1_POW_OFFSET,
    BSP_DRV_DEVICE_PSAM2_POW_OFFSET,
    BSP_DRV_DEVICE_PSAM3_POW_OFFSET
};
/**************************************************************************
* �������ƣ�unsigned int bsp_devicectl_init
* �����������豸����ģ���ʼ������
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_devicectl_init(void)
{
    g_pvBSPDrvDeviceCtlVirtStartAddr0 = p_epld_start_addr;
    g_pvFpgaDrvDeviceCtlVirtStartAddr0 = g_pvBSPDrvFpgaVirtStartAddr0;
    return 0;
}

/**************************************************************************
* �������ƣ�unsigned int bsp_device_rst_ctl
* �����������豸��λ����
* ���ʵı���
* �޸ĵı���
* ���������tDeviceID:�豸��
*                      
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_device_rst_ctl(T_DevRstList tDeviceID)
{
    unsigned short tDeviceReg = 0;

    tDeviceReg = READ_DEVICE_REG(BSP_DRV_DEVICE_RST_OFFSET);

    if (tDeviceID > DRV_DEV_RST_ETH1)
    {
        return 1;
    }
    
    if (s_aDevRstMaskCtl[tDeviceID][1]) 
    {
        tDeviceReg |= s_aDevRstMaskCtl[tDeviceID][0];
        WRITE_DEVICE_REG(BSP_DRV_DEVICE_RST_OFFSET, tDeviceReg);
        tDeviceReg &= ~s_aDevRstMaskCtl[tDeviceID][0];
        WRITE_DEVICE_REG(BSP_DRV_DEVICE_RST_OFFSET, tDeviceReg);
    }
    else
    {
        tDeviceReg &= ~s_aDevRstMaskCtl[tDeviceID][0];
        WRITE_DEVICE_REG(BSP_DRV_DEVICE_RST_OFFSET, tDeviceReg);
        tDeviceReg |= s_aDevRstMaskCtl[tDeviceID][0];
        WRITE_DEVICE_REG(BSP_DRV_DEVICE_RST_OFFSET, tDeviceReg);
    }
    return 0;
}

/**************************************************************************
* �������ƣ�unsigned int bsp_device_power_ctl
* �����������豸�ϵ����
* ���ʵı���
* �޸ĵı���
* ���������tDeviceID:�豸�ţ�dwPowerCtl:0 off 1:on
*                      
* �����������
* �� �� ֵ    ��0: �ɹ�������Ϊʧ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
**************************************************************************/
unsigned int bsp_device_power_ctl(T_DevPowList tDeviceID, unsigned int dwPowerCtl)
{
    unsigned short tDeviceReg = 0;
    unsigned short tFpgaData = 0;
    unsigned char ucID = 0;


    if (tDeviceID > DRV_DEV_PSAM3)
    {
        return 2;
    }    

    //ͨ��FPGA����
    if (tDeviceID > DRV_DEV_USB)
    {
        ucID = tDeviceID - DRV_DEV_USB - 1;
        tFpgaData = READ_DEVICE_FPGA_REG(s_aDevPowFpgaOffestCtl[ucID]);
        
        if (dwPowerCtl)
        {
            if (s_aDevPowFpgaMaskCtl[ucID][1]) 
            {
                tFpgaData |= s_aDevPowFpgaMaskCtl[ucID][0];
                WRITE_DEVICE_FPGA_REG(s_aDevPowFpgaOffestCtl[ucID], tFpgaData);
            }
            else
            {
                tFpgaData &= ~s_aDevPowFpgaMaskCtl[ucID][0];
                WRITE_DEVICE_FPGA_REG(s_aDevPowFpgaOffestCtl[ucID], tFpgaData);
            }
        }
        else
        {
            if (s_aDevPowFpgaMaskCtl[ucID][1]) 
            {
                tFpgaData &= ~s_aDevPowFpgaMaskCtl[ucID][0];
                WRITE_DEVICE_FPGA_REG(s_aDevPowFpgaOffestCtl[ucID], tFpgaData);
            }
            else
            {
                tFpgaData |= s_aDevPowFpgaMaskCtl[ucID][0];
                WRITE_DEVICE_FPGA_REG(s_aDevPowFpgaOffestCtl[ucID], tFpgaData);
            }
        
        }

    }
    //ͨ��EPLD����
    else
    {
        tDeviceReg = READ_DEVICE_REG(BSP_DRV_DEVICE_POW_OFFSET);
        
        if (dwPowerCtl)
        {
            if (s_aDevPowMaskCtl[tDeviceID][1]) 
            {
                tDeviceReg |= s_aDevPowMaskCtl[tDeviceID][0];
                WRITE_DEVICE_REG(BSP_DRV_DEVICE_POW_OFFSET, tDeviceReg);
            }
            else
            {
                tDeviceReg &= ~s_aDevPowMaskCtl[tDeviceID][0];
                WRITE_DEVICE_REG(BSP_DRV_DEVICE_POW_OFFSET, tDeviceReg);
            }
        }
        else
        {
            if (s_aDevPowMaskCtl[tDeviceID][1]) 
            {
                tDeviceReg &= ~s_aDevPowMaskCtl[tDeviceID][0];
                WRITE_DEVICE_REG(BSP_DRV_DEVICE_POW_OFFSET, tDeviceReg);
            }
            else
            {
                tDeviceReg |= s_aDevPowMaskCtl[tDeviceID][0];
                WRITE_DEVICE_REG(BSP_DRV_DEVICE_POW_OFFSET, tDeviceReg);
            }
        
        }

    }


    return 0;
}


