/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_scp.h
* �ļ���ʶ�� 
* ����ժҪ�� ϵͳ���ض���ӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2009.03.13
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_OSS_SCP_H
#define _USP_OSS_SCP_H

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/

/* ������Ϣ����/���պ� */
#if USP_OS_LINUX
#define MsgRcv(dwQueueID,pucDataBuf,wBufLen)  msgrcv((dwQueueID),((void *)pucDataBuf),(wBufLen),0,MSG_NOERROR)
#define MsgSend(dwQueueID,pucDataBuf,wBufLen)  msgsnd(((int)dwQueueID),((const void *)pucDataBuf),((size_t)(wBufLen)),0)
#endif

/* ���������ѡ�� */
#define     USP_OSS_SCP_ERROR_TYPE_RESET        0   /* ���� */
#define     USP_OSS_SCP_ERROR_TYPE_ALARM        1   /* �澯 */
#define     USP_OSS_SCP_ERROR_TYPE_IGNORE       2   /* ���� */

/* ����ϵͳ����ԭ���� */
#define     USP_OSS_SCP_STARTUP_NORMAL          (0) /* �������� */
#define     USP_OSS_SCP_STARTUP_TASK_DEAD       (1) /* ������������������ */

/* ������Ϣ����������� */
#define usp_scp_sendMsgBaseTno(tno, msg)   usp_oss_scp_sendTaskMsg(tno, msg, USP_OSS_IPC_FOREVERWAIT)

/* ������ٺ궨�� */
#define USP_OSS_ST_DESC_SIZE                     (24)    /* ����������Ϣ�ĳ��� */
#define USP_OSS_ST_SND                           (0)     /* �������� */
#define USP_OSS_ST_RCV                           (1)     /* �������� */

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/
/* ϵͳģ������ģ�� */
#define USP_OSS_SCP_BEGIN_MODULE_CONF_DEFINE \
static T_CliModuleConf s_atCliModuleConfUser[] = {

#define USP_OSS_SCP_MODULE_CONF(ID, NAME, DESC) \
{ID, 0, (BYTE *)NAME, (BYTE *)DESC},

#define USP_OSS_SCP_END_MODULE_CONF_DEFINE \
};

/**************************************************************************
 *                            ��������                                    *
 **************************************************************************/

/* NTP������Ϣ */
typedef struct tagT_USPOssScpNtpCfg
{
    WORD32  adwNtpServerIp[5]; /* ���Ϊ5��,��һ��0��ʾ����,��0��ǰ������Ч�� */
    CHAR    cTimeZoneH;        /* ʱ��,Сʱ,��Χ-12--+13 */
    CHAR    cTimeZoneM;        /* ʱ��,����,��Χ0--+59 */
    WORD16  wPort;             /* 0��ʾʹ��ȱʡ�˿� */
    WORD32  dwSyncPeriod;      /* ͬ������ʱ�䣬��λ���� */
}T_USPOssScpNtpCfg;

/* �ϵ���Ϣ���� */
typedef struct tagT_USPOssScpPowerOn
{    
    BYTE    ucRound;             /* ��ʾ�ڼ����ϵ� */
    BYTE    ucPad;
    WORD16  wPad;
}T_USPOssScpPowerOn;

/* �ϵ�Ӧ����Ϣ���� */
typedef struct tagT_USPOssScpPowerOnAck
{    
    BYTE    ucRound;             /* ��ʾ�ڼ����ϵ� */
    BYTE    ucPad;
    WORD16  wPad;
}T_USPOssScpPowerOnAck;

/* ����ϵͳģ��������Ϣ */
#define _USP_OSS_SCP_H_INCLUDE
#include "scp.conf"
#undef  _USP_OSS_SCP_H_INCLUDE

/* ����ϵͳ��ʼ��������������USP_SUCCESS��ʾ�ɹ�������Ϊ�����롣*/
typedef WORD32 usp_oss_scp_initEntry(VOID);

/* �����ʶ�������� */
typedef WORD16  USP_OssScpTno;

/* ����һ�������������������� */
typedef struct tagT_ScpTaskCtrl    *USP_OssScpTask;

/* ������������ */
typedef WORD32 usp_oss_scp_taskEntry(USP_OssScpTask task);

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/

/* ������ӡ���� */
#if USP_OS_LINUX && USP_OSS_PRINTF_USE

#define usp_oss_printf(ucModule, ucLevel, ...)              usp_cli_printf(ucModule, ucLevel, __VA_ARGS__)
#define usp_oss_isPrintf(ucModule, ucLevel)                 usp_cli_isPrintf(ucModule, ucLevel)

#else

#define usp_oss_printf(ucModule, ucLevel, ...)  
#define usp_oss_isPrintf(ucModule, ucLevel)                 FALSE

#endif  /* USP_OS_LINUX && USP_OSS_PRINTF_USE */

/* ����������ٺ��� */
#if USP_OS_LINUX  && USP_OSS_TRACE_USE && USP_OSS_ST_USE
#define usp_oss_trace(ucType, pucDesc, ptMsg)               usp_cli_trace(ucType, pucDesc, ptMsg)
#define usp_oss_isTraceByTno(wTno)                          usp_cli_isTraceByTno(wTno)
#define usp_oss_isTraceById(wId)                            usp_cli_isTraceById(wId)
#else
#define usp_oss_trace(ucType, pucDesc, ptMsg)  
#define usp_oss_isTraceByTno(wTno)                          FALSE
#define usp_oss_isTraceById(wId)                            FALSE
#endif  /* USP_OS_LINUX && USP_OSS_TRACE_USE */

/* �ֲ�ʽ�ṹ�ж�����ŵĲ�ֺͺϲ� */
#define usp_oss_scp_getSubDevID(wTno)                       (BYTE)(((WORD16)(wTno) & 0xFFC0) >> 6)
#define usp_oss_scp_getSubTno(wTno)                         (BYTE)((WORD16)(wTno) & 0x003F)
#define usp_oss_scp_getTno(subDevID, subTno)                (WORD16)(((WORD16)(subDevID) << 6) | (WORD16)(subTno))
#define usp_oss_scp_getMinTno(subDevID)                     usp_oss_scp_getTno((subDevID), 0)
#define usp_oss_scp_getMaxTno(subDevID)                     usp_oss_scp_getTno((subDevID), 0x003F)

/* ��Ϣ������� */
#define USP_OSS_SCP_MSGSEQ(n)                               (0xFFFF == (++(n)) ? ++(n) : (n))

/* ������Ϣ��ָ�������� */
extern WORD32 usp_oss_scp_sendTaskMsg(USP_OssScpTno tno, VOID *msg, WORD32 dwFlag);

/* ��ָ�������������Ϣ */
extern WORD32 usp_oss_scp_recvTaskMsg(USP_OssScpTno tno, VOID *msg, WORD16 wSize, WORD32 dwFlag);

/* �����ض����͵���Ϣ */
extern WORD32 usp_oss_scp_recvTaskMsgWithTypes(USP_OssScpTno tno, VOID *msg, WORD16 wSize, WORD32 dwTimeOut, T_Msg *aptExpectMsg[]);

/* ��ʾ��Ϣ�����е����� */
extern WORD32 usp_oss_scp_showTaskMsgBuff( USP_OssScpTno tno );

/* �õ���������Ϣ������ */
extern WORD32 usp_oss_scp_countTaskMsgBuff( USP_OssScpTno tno, WORD32 * pdwMsgCount );

/* ����Ӧ������ĵ�ǰ״̬ */
extern VOID usp_oss_scp_setTaskStatus(USP_OssScpTask task, WORD16 wStatus);

/* ��ȡӦ������ĵ�ǰ״̬ */
extern WORD16 usp_oss_scp_getTaskStatus(USP_OssScpTask task);

/* ��������Ӧ����Ϣ */
extern WORD32 usp_oss_scp_keepAliveAck(USP_OssScpTno tno);

/* ��ȡ��������ص���Ϣ���о�� */
//extern WORD32 usp_oss_scp_getMsgQueueHandle(USP_OssScpTno tno, USP_OssIpcMsgQueue *mq);

/* ֹͣι���ӿ� */
extern WORD32 usp_scp_stopFeedWatchDog(VOID);

/* ϵͳ���Ʒ���������ں��� */
extern WORD32 usp_scp_serviceEntry(USP_OssScpTask task);

/* ����ϵͳ����ԭ�� */
extern WORD32 usp_oss_scp_setStartupReason(BYTE ucWhy);

/* ��ȡϵͳ����ԭ�� */
extern WORD32 usp_oss_scp_getStartupReason(BYTE *pucWhy);

/* ��ʼ������ͳ����Ϣ */
extern WORD32 usp_oss_scp_pspInit(VOID);

#endif  /* _USP_OSS_SCP_H */


