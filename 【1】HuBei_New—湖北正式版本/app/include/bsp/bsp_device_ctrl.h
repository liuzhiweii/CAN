/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvDeviceCtl.h
* �ļ���ʶ��
* ����ժҪ�����踴λ��Դ���� ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�       
* ������ڣ�
* �޸�����:    
************************************************************************/
#ifndef _BSP_DRV_DEVICE_H_
#define _BSP_DRV_DEVICE_H_


typedef enum tagT_DevPowList
{
    DRV_DEV_RF = 0x0,
    DRV_DEV_SD,
    DRV_DEV_OLED,
    DRV_DEV_USB,
    DRV_DEV_PSAM0,
    DRV_DEV_PSAM1,
    DRV_DEV_PSAM2,
    DRV_DEV_PSAM3
}T_DevPowList;

typedef enum tagT_DevRstList
{
    DRV_DEV_RST_USB = 0x0,
    DRV_DEV_RST_752,
    DRV_DEV_RST_ETH2,
    DRV_DEV_RST_ETH1
}T_DevRstList;


#define DEVICE_POWR_ON      (1)
#define DEVICE_POWR_OFF     (0)

#define BSP_DRV_DEVICE_RST_OFFSET     (0xD6>>1)
#define BSP_DRV_DEVICE_RST_USB_MASK   (0x01)
#define BSP_DRV_DEVICE_RST_752_MASK   (0x02)
#define BSP_DRV_DEVICE_RST_ETH2_MASK  (0x04)
#define BSP_DRV_DEVICE_RST_ETH1_MASK  (0x08)

#define BSP_DRV_DEVICE_RST_USB_EN     (0x01)
#define BSP_DRV_DEVICE_RST_752_EN     (0x01)
#define BSP_DRV_DEVICE_RST_ETH2_EN    (0x00)
#define BSP_DRV_DEVICE_RST_ETH1_EN    (0x00)

#define BSP_DRV_DEVICE_POW_OFFSET     (0xD8>>1)
#define BSP_DRV_DEVICE_POW_RF_MASK    (0x01)
#define BSP_DRV_DEVICE_POW_SD_MASK    (0x02)
#define BSP_DRV_DEVICE_POW_OLED_MASK  (0x04)
#define BSP_DRV_DEVICE_POW_USB_MASK   (0x08)

#define BSP_DRV_DEVICE_POW_RF_EN      (0x01)
#define BSP_DRV_DEVICE_POW_SD_EN      (0x01)
#define BSP_DRV_DEVICE_POW_OLED_EN    (0x01)
#define BSP_DRV_DEVICE_POW_USB_EN     (0x01)



#define BSP_DRV_DEVICE_PSAM0_POW_OFFSET     (0x800fd)
#define BSP_DRV_DEVICE_POW_PSAM0_MASK       (0x01)
#define BSP_DRV_DEVICE_POW_PSAM0_EN         (0x01)

#define BSP_DRV_DEVICE_PSAM1_POW_OFFSET     (0x800fe)
#define BSP_DRV_DEVICE_POW_PSAM1_MASK       (0x01)
#define BSP_DRV_DEVICE_POW_PSAM1_EN         (0x01)

#define BSP_DRV_DEVICE_PSAM2_POW_OFFSET     (0x800ff)
#define BSP_DRV_DEVICE_POW_PSAM2_MASK       (0x01)
#define BSP_DRV_DEVICE_POW_PSAM2_EN         (0x01)

#define BSP_DRV_DEVICE_PSAM3_POW_OFFSET     (0x80100)
#define BSP_DRV_DEVICE_POW_PSAM3_MASK       (0x01)
#define BSP_DRV_DEVICE_POW_PSAM3_EN         (0x01)


/**************************************************************************
*                                              ȫ�ֱ�������                                                *
**************************************************************************/

/**************************************************************************
*                                           ȫ�ֺ���ԭ��                                                   *
**************************************************************************/
/*  ģ���ʼ���ӿ�*/
unsigned int bsp_devicectl_init(void);
unsigned int bsp_device_rst_ctl(T_DevRstList tDeviceID);
unsigned int bsp_device_power_ctl(T_DevPowList tDeviceID, unsigned int dwPowerCtl);
		
#endif/* _BSP_DRV_EPLD_H_  */

