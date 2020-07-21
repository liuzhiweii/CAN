/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� bsp_flash.h
* �ļ���ʶ�� 
* ����ժҪ�� flashģ�����ӿ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� 
* ������ڣ�
* 
************************************************************************/


#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_


#define NOR_FLASH   0         /*NOR FALSH*/
#define NAND_FLASH  1         /*NAND FALSH*/



unsigned int bsp_flash_init(void);
unsigned int bsp_flash_read(unsigned int dwFlashId, unsigned int dwOffset, unsigned int dwLen, unsigned char * pucBuf, unsigned int *pdwReadLen);
unsigned int bsp_flash_write(unsigned int dwFlashId, unsigned int dwOffset, unsigned int dwLen, unsigned char * pucBuf, unsigned int *pdwWriteLen);
unsigned int bsp_flash_erase(unsigned int dwFlashId, unsigned int dwOffset, unsigned int dwLen);
unsigned int bsp_flash_size(void);
unsigned int bsp_flash_totalsize(unsigned int dwFlashId,unsigned int *pdwSize);


#endif

