/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�ParallelPsamUsr.h
* �ļ���ʶ�� 
* ����ժҪ������PSAM����ʵ�ֺ궨���ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/

/***********************************************************
 *                ����ͷ�ļ�                               *
 ***********************************************************/

#ifndef  __PARALLEL_PSAMUSR_H
#define  __PARALLEL_PSAMUSR_H

//#include "bsp_typeDef.h"
//#include "bsp_pub.h"
//#include "drv.conf"

/*����һ���û��ռ������PSAM*/
typedef struct tagT_UsrPsamDevice
{
    unsigned char ucStatus;                   /*�豸�ļ�״̬*/
    char *ucDevName;                 /*�豸�ļ���*/
    int sdwPsamDevFd;            /*PSAM�ļ�������*/
} T_UsrPsamDevice;


/**********************************************************/
/*            Linux ioctl����                             */
/**********************************************************/

#define    BSP_PSAM_IOCTL_CMD_ACTIVE           (0x00)
#define    BSP_PSAM_IOCTL_CMD_DEACTIVE         (0x01)
#define    BSP_PSAM_IOCTL_CMD_PROCESS          (0x02)
#define    BSP_PSAM_IOCTL_CMD_CLKSET           (0x03)
#define    BSP_PSAM_IOCTL_CMD_VOLTAGESET       (0x04)
#define    BSP_PSAM_IOCTL_CMD_SETPPS_STATE     (0x05)
#define    BSP_PSAM_IOCTL_CMD_SETETU_STATE     (0x06)



#define BSP_DRV_PSAM0_LINUX_DEVICE "/dev/misc/drv_psam0"
#define BSP_DRV_PSAM1_LINUX_DEVICE "/dev/misc/drv_psam1"
#define BSP_DRV_PSAM2_LINUX_DEVICE "/dev/misc/drv_psam2"
#define BSP_DRV_PSAM3_LINUX_DEVICE "/dev/misc/drv_psam3"



#endif



