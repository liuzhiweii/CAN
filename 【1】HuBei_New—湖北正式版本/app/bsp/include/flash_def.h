/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� flash.h
* �ļ���ʶ�� 
* ����ժҪ��ģ���ڲ�ͷ�ļ�
* ����˵����  
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/
#ifndef _FLASH_H_
#define _FLASH_H_

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
//#define FLASH_DEBUG
struct mtd_partition {
	char *name;			/* identifier string */
	unsigned int size;			/* partition size */
	unsigned int offset;		/* offset within the master MTD space */
	unsigned int mask_flags;		/* master MTD flags to mask out for this partition */
};

typedef struct tagT_BSPDrvFlashMtd
{
	char acMtdDevName[16];   /*MTD�豸��*/
	unsigned int dwMtdOffset;      /*MTD�豸��ַƫ��*/
	unsigned int dwMtdSize;        /*MTD��С*/
    char acMtdName[64];      /*MTD����*/
} T_BSPDrvFlashMtd;



#define SYSTEM_MTD_MAX         24

#define BSP_DRV_NOR_SECTOR_SIZE         0x20000
#define BSP_DRV_NAND_SECTOR_SIZE       0x20000
#endif
