/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvWtd.h
* �ļ���ʶ��
* ����ժҪ��WTD ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�      
* ������ڣ�
************************************************************************/

#ifndef _BSP_DRV_WTD_H_
#define _BSP_DRV_WTD_H_
/**************************************************************************
*                                           ȫ�ֺ���ԭ��                                                   *
**************************************************************************/

#define BSP_DRV_WTD_HWFEED (1)
#define BSP_DRV_WTD_WMS_DATA_OFFSET (0xBC>>1)
#define BSP_DRV_WTD_WMS_MASK (0x01)
#define BSP_DRV_WTD_WMS (0x0)
#define BSP_DRV_WTD_WMS_DIR_OFFSET (0x0)

#define BSP_DRV_WTD_START (0)
#define BSP_DRV_WTD_STOP (1)
#define BSP_DRV_WTD_FEED_TYPE (1)
#define BSP_DRV_WTD_FEED (1)
/*
 * WST configuration
 */

#define BSP_DRV_WTD_WST_DIR_ADDR (0x)
#define BSP_DRV_WTD_WST_SET_ADDR (0x)
#define BSP_DRV_WTD_WST_CLR_ADDR (0x)
#define BSP_DRV_WTD_WST_MASK (0x01)
/*
 * WST configuration
 */
#define BSP_DRV_WTD_WST_DATA_OFFSET (0xB8>>1)
#define BSP_DRV_WTD_WST_DIR_OFFSET (0x0)

/*
 * WDI configuration
 */
#define BSP_DRV_WTD_WDI_DATA_OFFSET (0xC0>>1)
#define BSP_DRV_WTD_WDI_DIR_OFFSET (0x0)
#define BSP_DRV_WTD_WDI_MASK (0x01)

/*
 * WMS configuration
 */
#define BSP_DRV_WTD_HWFEED (1)
#define BSP_DRV_WTD_WMS_DATA_OFFSET (0xBC>>1)
#define BSP_DRV_WTD_WMS_MASK (0x01)
#define BSP_DRV_WTD_WMS (0x0)
#define BSP_DRV_WTD_WMS_DIR_OFFSET (0x0)


/*  ��ʼ��WTDģ��ӿ�*/
unsigned int bsp_wtd_init(void);
/*  ʹ��WTD�ӿ�*/
unsigned int bsp_wtd_enable(void);
/*  ����wtd���ʱ��ӿ�*/
unsigned int bsp_wtd_setTimer(unsigned int dwTimeMs);
/*  ����WTD �ӿ�*/
unsigned int bsp_wtd_disable(void);
/*  ι���ӿ�*/
unsigned int bsp_wtd_feed(void);
/*  Ӳ��ι���ӿ�*/
unsigned int bsp_wtd_hwFeed(void);

		
#endif/* _BSP_DRV_WTD_H_  */

