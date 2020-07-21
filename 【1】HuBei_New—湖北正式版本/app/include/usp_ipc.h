/************************************************************************
* ��Ȩ���� (C)2009, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_ipc.h
* �ļ���ʶ�� 
* ����ժҪ�� ���̼�ͨ�Ŷ���ӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2009.04.07
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_OSS_IPC_H
#define _USP_OSS_IPC_H

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/

/* �����õ���Ϣ���� */
#define     USP_OSS_IPC_MQ_INVALID          USP_OSS_VOS_MQ_INVALID

/* �����õĻ����� */
#define     USP_OSS_IPC_MUTEX_INVALID       USP_OSS_VOS_MUTEX_INVALID

/* �Ƿ��ļ�ֵ */
#define     USP_OSS_IPC_KEY_INVALID         (0xFFFF)

/* ������̼�ͨ��ѡ�� */
#define     USP_OSS_IPC_NOWAIT              (0)            /* �������أ������� */
#define     USP_OSS_IPC_FOREVERWAIT         (0xFFFFFFFF)   /* ���� */

/* ���廥���������� */
#define     USP_OSS_IPC_CREATE_MUTEX(pMutex, key)   usp_oss_vos_createMutex(pMutex, key)
#define     USP_OSS_IPC_TAKE_MUTEX(mutex)           usp_oss_vos_takeMutex(mutex, USP_OSS_VOS_IPC_FOREVERWAIT)
#define     USP_OSS_IPC_GIVE_MUTEX(mutex)           usp_oss_vos_giveMutex(mutex)

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
/* ������Ϣ���г������ݽṹ */
typedef     USP_OssVosMsgQueue      USP_OssIpcMsgQueue;

/* ���廥�����������ݽṹ */
typedef     USP_OssVosMutex         USP_OssIpcMutex;

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
/* ������Ϣ���� */
extern WORD32 usp_oss_ipc_createMq(USP_OssIpcMsgQueue *pmq, WORD16 wMsgNum, WORD16 wItemSize, WORD16 wMsgQueueKey);

/* ������Ϣ */
extern WORD32 usp_oss_ipc_sendMsg(USP_OssIpcMsgQueue mq, VOID *msg, WORD32 dwFlag);

/* ������Ϣ */
extern WORD32 usp_oss_ipc_recvMsg(USP_OssIpcMsgQueue mq, VOID *msg, WORD16 wSize, WORD32 dwFlag);


#endif  /* _USP_OSS_IPC_H */

