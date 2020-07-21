/************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_lib_vos.h
* �ļ���ʶ�� 
* ����ժҪ�� �������ϵͳ��̬�����ӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2012.02.06
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_LIB_VOS_H
#define _USP_LIB_VOS_H

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
#include <linux/ppp_defs.h>
#include <linux/if_ppp.h>   
#include <sys/utsname.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/dir.h>

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
/* ������̼�ͨ��ѡ�� */
#define     USP_LIB_VOS_IPC_NOWAIT          IPC_NOWAIT   /* �������أ������� */
#define     USP_LIB_VOS_IPC_FOREVERWAIT     (0)          /* ���� */

/* ���岻���õ���Ϣ���� */
#define     USP_LIB_VOS_MQ_INVALID          (0xFFFFFFFF)

/* ���岻���õĻ����� */
#define     USP_LIB_VOS_MUTEX_INVALID       (-1)

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                            ��������                                    *
 **************************************************************************/
/* ����ϵͳ�ṩ����Ϣ���г������ݽṹ */
typedef WORD32  USPLibVosMsgQueue;

/* ����ϵͳ�ṩ�Ļ������������ݽṹ */
typedef SWORD32 USPLibVosMutex;

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/

/* ���������ڴ� */
extern VOID *usp_lib_vos_createShm(WORD16 wKey, WORD32 dwSize);

/* ���ӵ��Ѿ����ڵĹ����ڴ� */
extern VOID *usp_lib_vos_attachShm(WORD16 wKey);

/* �Ͽ��������ڴ������ */
extern WORD32 usp_lib_vos_dttachShm(VOID *ptr);

/* ������Ϣ���� */
extern WORD32 usp_lib_vos_createMq(USPLibVosMsgQueue *pmq, WORD16 wMsgNum, WORD16 wItemSize, WORD16 wMsgQueueKey);

/* ������Ϣ���� */
extern WORD32 usp_lib_vos_destroyMq(USPLibVosMsgQueue mq);

/* ���������� */
extern WORD32 usp_lib_vos_createMutex(USPLibVosMutex *pMutex, WORD16 wMutexKey);

/* ��ȡ���еĻ������������ */
extern WORD32 usp_lib_vos_getMutex(USPLibVosMutex *pMutex, WORD16 wMutexKey);

/* ������P���� */
extern WORD32 usp_lib_vos_takeMutex(USPLibVosMutex mutex, WORD32 dwFlag);

/* ������V���� */
extern WORD32 usp_lib_vos_giveMutex(USPLibVosMutex mutex);

/* ������Ϣ */
extern WORD32 usp_lib_vos_sendMsg(USPLibVosMsgQueue mq, VOID *msg, WORD16 wMsgLen, WORD32 dwFlag);

/* ������Ϣ */
extern WORD32 usp_lib_vos_recvMsg(USPLibVosMsgQueue mq, VOID *msg, WORD16 wSize, WORD32 dwFlag);

#endif  /* _USP_LIB_VOS_H */

