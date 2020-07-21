/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_vos.h
* �ļ���ʶ�� 
* ����ժҪ�� �����ϵͳ�йص�ͨ��ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2008.10.14
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_OSS_VOS_H
#define _USP_OSS_VOS_H

#define USP_OS_LINUX 1

#if USP_OS_LINUX

/**************************************************************************
 *                ������������ϵͳ�йص�ͷ�ļ�                          *
 **************************************************************************/
#ifdef _UBOOT_
#include <common.h>
#else
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <termios.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/select.h>
#include <pthread.h>
#include <setjmp.h>
#include <crypt.h>
#include <sys/prctl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <linux/rtnetlink.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>
#include <sched.h>
#include <dlfcn.h>
#include <linux/ppp_defs.h>
#include <linux/if_ppp.h>   
#include <sys/utsname.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/dir.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <pthread.h>
#endif

/**************************************************************************
 *                      �궨��                                            *
 **************************************************************************/
/* �뼶���˯�� */
#define USP_OSS_VOS_SLEEP_SEC(sec)      sleep((sec))

/* ���뼶���˯�� */
#define USP_OSS_VOS_SLEEP_MSEC(msec)    usleep((msec) * 1000)

/* ΢�뼶���˯�� */
//#define USP_OSS_VOS_SLEEP_USEC(usec)    usp_oss_vos_usleep(usec)    
#define USP_OSS_VOS_SLEEP_USEC(usec)    usleep(usec)   


/* ������̼�ͨ��ѡ�� */
#define     USP_OSS_VOS_IPC_NOWAIT          IPC_NOWAIT   /* �������أ������� */
#define     USP_OSS_VOS_IPC_FOREVERWAIT     (0)          /* ���� */

/* ���岻���õ���Ϣ���� */
#define     USP_OSS_VOS_MQ_INVALID      (0xFFFFFFFF)

/* ���岻���õĻ����� */
#define     USP_OSS_VOS_MUTEX_INVALID   (-1)

/* �����ļ����� */
#define     USP_OSS_VOS_FILE_TYPE_DIR   0
#define     USP_OSS_VOS_FILE_TYPE_FILE  1

/* ����WIFI���ܷ�ʽ */
#define     USP_OSS_VOS_WIFI_MODE_WEP   0
#define     USP_OSS_VOS_WIFI_MODE_WPA   1

/**************************************************************************
 *                      ��������                                          *
 **************************************************************************/
/* ����ϵͳ�ṩ����Ϣ���г������ݽṹ */
typedef WORD32  USP_OssVosMsgQueue;

/* ����ϵͳ�ṩ�Ļ������������ݽṹ */
typedef SWORD32 USP_OssVosMutex;

/* �ļ��а������ļ���Ϣ */
typedef struct tagT_USPOssVosDirInFo
{ 
    BYTE            aucFileName[64];       /* �ļ��� */
    BYTE            ucType;                /* �ļ����� */
    BYTE            ucPad;
    WORD16          wPad;
}T_USPOssVosDirInFo;

#endif  /* USP_OS_LINUX */

#if USP_OS_FREERTOS

/**************************************************************************
 *                ������������ϵͳ�йص�ͷ�ļ�                          *
 **************************************************************************/
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "rspdebugproxy.h"

/**************************************************************************
 *                      �궨��                                            *
 **************************************************************************/
/* �뼶���˯�� */
#define USP_OSS_VOS_SLEEP_SEC(sec)      vTaskDelay((sec) * 1000 / portTICK_RATE_MS)

/* ���뼶���˯�� */
#define USP_OSS_VOS_SLEEP_MSEC(msec)    vTaskDelay((msec) / portTICK_RATE_MS)

/* ΢�뼶���˯�� */
//#define USP_OSS_VOS_SLEEP_USEC(usec)    usp_oss_vos_usleep(usec)
#define USP_OSS_VOS_SLEEP_USEC(usec)    usleep(usec)

/* ������̼�ͨ��ѡ�� */
#define     USP_OSS_VOS_IPC_NOWAIT          portNO_DELAY   /* �������أ������� */
#define     USP_OSS_VOS_IPC_FOREVERWAIT     portMAX_DELAY  /* ���� */

/* ���岻���õ���Ϣ���� */
#define     USP_OSS_VOS_MQ_INVALID      NULL

/* ���岻���õĻ����� */
#define     USP_OSS_VOS_MUTEX_INVALID   NULL

/**************************************************************************
 *                      ��������                                          *
 **************************************************************************/
/* ����ϵͳ�ṩ����Ϣ���г������ݽṹ */
typedef xQueueHandle  USP_OssVosMsgQueue;

/* ����ϵͳ�ṩ�Ļ������������ݽṹ */
typedef xQueueHandle  USP_OssVosMutex;

#endif  /* USP_OS_FREERTOS */

/**************************************************************************
 *                      ��������                                          *
 **************************************************************************/
/* ������ϵͳ�еı�ʶ������ */
typedef WORD32  USP_OssVosPid;

/* �߳���ϵͳ�еı�ʶ������ */
typedef WORD32  USP_OssVosTid;

/* ���幤����ں��� */
typedef VOID USP_OssVosWorkEntry(VOID *arg);

/**************************************************************************
 *                      ��װ��ȫ�ֺ���ԭ��                                *
 **************************************************************************/

/* ������Ϣ���� */
extern WORD32 usp_oss_vos_createMq(USP_OssVosMsgQueue *pmq, WORD16 wMsgNum, WORD16 wItemSize, WORD16 wMsgQueueKey);

/* ������Ϣ */
extern WORD32 usp_oss_vos_sendMsg(USP_OssVosMsgQueue mq, VOID *msg, WORD16 wMsgLen, WORD32 dwFlag);

/* ������Ϣ */
extern WORD32 usp_oss_vos_recvMsg(USP_OssVosMsgQueue mq, VOID *msg, WORD16 wSize, WORD32 dwFlag);

/* �������� */
extern WORD32 usp_oss_vos_createProcess(USP_OssVosWorkEntry *entry, VOID *arg, BYTE *pucName, SWORD16 swPriority, USP_OssVosPid *pid);

/* �����߳� */
extern WORD32 usp_oss_vos_createThread(USP_OssVosWorkEntry *entry, VOID *arg, BYTE *pucName, WORD16 wStackDepth, SWORD16 swPriority, USP_OssVosTid *tid);

/* �����߳� */
extern WORD32 usp_oss_vos_destroyThread(USP_OssVosTid tid);

/* ���ý������� */
extern VOID usp_oss_vos_setProcessName(BYTE *pucName);

/* ��������ϵͳ */
extern VOID usp_oss_vos_reset(VOID);

/* ����Coredump */
extern WORD32 usp_oss_vos_configCoredump(VOID);

/* ���빲���ڴ� */
extern VOID *usp_oss_vos_malloc(WORD16 wKey, WORD32 dwSize);

/* ���ò���ϵͳʱ�� */
extern WORD32 usp_oss_vos_setTimezone(CHAR cTimeZoneH, CHAR cTimeZoneM);

/* NTPʱ��ͬ�� */
extern WORD32 usp_oss_vos_updateNtp(WORD32 dwNtpServerIp);

/* ��ʼ���豸��ͨ����ʩ */
extern WORD32 usp_oss_vos_initComm(VOID);

/* ΢�뼶����ӳٺ��� */
extern VOID usp_oss_vos_usleep(WORD32 dwUsec);

/* ���������� */
extern WORD32 usp_oss_vos_createMutex(USP_OssVosMutex *pMutex, WORD16 wMutexKey);

/* ������P���� */
extern WORD32 usp_oss_vos_takeMutex(USP_OssVosMutex mutex, WORD32 dwFlag);

/* ������V���� */
extern WORD32 usp_oss_vos_giveMutex(USP_OssVosMutex mutex);

/* ���ݽ������Ʋ��ҽ��̺� */
extern WORD32 usp_oss_vos_getpid(BYTE *pucPName, BYTE *pucPID);

/* ����GPRS���� */
extern WORD32 usp_oss_vos_startGprsPPP(BYTE *pucAPN);

/* �Ͽ�GPRS������· */
extern WORD32 usp_oss_vos_stopGprsPPP(VOID);

/* ��ѯGPRS������·״̬ */
extern WORD32 usp_oss_vos_statusGprsPPP(BYTE *pucStatus);

/* ��ȡ�ļ��а������ļ���Ϣ */
extern WORD32 usp_oss_vos_readDir(BYTE *pucDir, T_USPOssVosDirInFo *ptUSPOssVosDirInFo, WORD16 *pwNum);

/* ԭ��д�ļ� */
extern WORD32 usp_oss_vos_fileAtomicWrite(BYTE *pucFileName, BYTE *pucBuf, WORD32 dwLen);

/* ����WIFI */
extern WORD32 usp_oss_vos_WifiStart(BYTE ucMode, BYTE *pucEssId, BYTE *pucKey);

/* ֹͣWIFI */
extern WORD32 usp_oss_vos_WifiStop(VOID);

#endif  /* _USP_OSS_VOS_H */

