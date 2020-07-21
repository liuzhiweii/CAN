/************************************************************************
* ��Ȩ���� (C)2009, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ�usp_drvEpld.h
* �ļ���ʶ��
* ����ժҪ��EPLD ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��V01.00.00
* ��    �ߣ�       �ຣ��
* ������ڣ�2009-10-10
* �޸�����:    2011-09-22    �޸���: cdj wuqi  
************************************************************************/
#ifndef _USP_DRV_EPLD_H_
#define _USP_DRV_EPLD_H_

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
#if USP_DRV_EPLD_PORT_WIDTH == 8
    typedef BYTE*   unsigned short *;
    typedef BYTE  T_EpldData;
#elif USP_DRV_EPLD_PORT_WIDTH == 16
    typedef WORD16* unsigned short *;
    typedef WORD16  T_EpldData;
#elif USP_DRV_EPLD_PORT_WIDTH == 32
    typedef WORD32* unsigned short *;
    typedef WORD32  T_EpldData;
#else
    typedef BYTE*   unsigned short *;
    typedef BYTE  T_EpldData;

#endif

/**************************************************************************
*                                              ȫ�ֱ�������                                                *
**************************************************************************/
/*  epld��ʼ�����ַ*/
//extern volatile unsigned short * g_pvUSPDrvEpldVirtStartAddr0;

/*��ȡEPLD�Ĵ�����ַ*/

#define EPLD_REG_ADDR(OFFSET)        ((g_pvUSPDrvEpldVirtStartAddr0) + OFFSET)
#define WRITE_EPLD_REG(OFFSET,DATA)   g_pvUSPDrvEpldVirtStartAddr0[OFFSET]=(T_EpldData)DATA
#define READ_EPLD_REG(OFFSET)         g_pvUSPDrvEpldVirtStartAddr0[OFFSET]


/**************************************************************************
*                                           ȫ�ֺ���ԭ��                                                   *
**************************************************************************/
/*  epldģ���ʼ���ӿ�*/
WORD32 usp_drv_epld_init(VOID);
WORD32 usp_drv_epld_read(WORD32 dwRegOffset, unsigned short * ptRegValue);
WORD32 usp_drv_epld_write(WORD32 dwRegOffset, T_EpldData tRegValue);

#if (USP_OS_TYPE == OS_FREERTOS)
    WORD32 usp_drv_epld_write_reg_hold(WORD32 dwRegOffset, T_EpldData tRegValue, BYTE uchold);
#endif
		
#endif/* _USP_DRV_EPLD_H_  */


