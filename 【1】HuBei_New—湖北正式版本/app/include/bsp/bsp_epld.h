/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvEpld.h
* �ļ���ʶ��
* ����ժҪ��EPLD ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�       
* ������ڣ�
* �޸�����:    
************************************************************************/
#ifndef _BSP_DRV_EPLD_H_
#define _BSP_DRV_EPLD_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************
*                                                        ����                                                            *
**************************************************************************/


/**************************************************************************
*                                                     �궨��                                                          *
**************************************************************************/


/**************************************************************************
*                                                  ��������                                                       *
**************************************************************************/
/*  epld���������Ͷ���*/

typedef unsigned short* T_EpldAddr;
typedef unsigned short  T_EpldData;


/**************************************************************************
*                                              ȫ�ֱ�������                                                *
**************************************************************************/
/*  epld��ʼ�����ַ*/
extern volatile unsigned short * p_epld_start_addr;

/*��ȡEPLD�Ĵ�����ַ*/
#define BSP_DRV_EPLD_LINUX_DEVICE "/dev/misc/drv_epld"

#define BSP_DRV_EPLD_ADDR_WIDTH (0x400000)
#define BSP_DRV_EPLD_PORT_WIDTH (0x10)
#undef BSP_EPLD_INT_CTRL_USE


#define EPLD_REG_ADDR(OFFSET)        ((p_epld_start_addr) + OFFSET)
#define WRITE_EPLD_REG(OFFSET,DATA)   p_epld_start_addr[OFFSET]=(T_EpldData)DATA
#define READ_EPLD_REG(OFFSET)         p_epld_start_addr[OFFSET]


/**************************************************************************
*                                           ȫ�ֺ���ԭ��                                                   *
**************************************************************************/
/*  epldģ���ʼ���ӿ�*/
unsigned int bsp_epld_init(void);
unsigned int bsp_epld_read(unsigned int dwRegOffset, unsigned short * ptRegValue);
unsigned int bsp_epld_write(unsigned int dwRegOffset, T_EpldData tRegValue);

#ifdef __cplusplus
}
#endif

		
#endif/* _BSP_DRV_EPLD_H_  */

