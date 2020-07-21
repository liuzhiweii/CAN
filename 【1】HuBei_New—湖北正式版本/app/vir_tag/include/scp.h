/************************************************************************
* ��Ȩ���� (C)2009, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� scp.h
* �ļ���ʶ�� 
* ����ժҪ�� ϵͳ�����ڲ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2009.08.18
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _OSS_SCP_H
#define _OSS_SCP_H


/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/

/* ���Ź��������� */
#define SCP_WTD_TASK_NAME                   (BYTE *)"usp_wtd"

/* ������������ */
#define SCP_LED_FLASH_TASK_NAME             (BYTE *)"usp_led"

/* �������̼��Ͷ�ջdump�궨�� */
#define SCP_MAX_PROC_TERM_LOG_SIZE     	    (2 * 1024)                          /* ��¼������ֹ��Ϣ�ļ����ߴ� */
#define SCP_MAX_STACK_DUMP_LOG_SIZE         (32 * 1024)                         /* ��¼��ջdump��Ϣ�ļ����ߴ� */
#define SCP_MAX_STACK_DUMP_LOG_PROBE_DEEP   (30)                                /* ��ջ��Ϣ�����̽����� */
#if USP_CPU_TYPE_ppc_8xx || USP_CPU_TYPE_ppc_82xx
#define SCP_PROC_TERM_LOG   			    "/mnt/app/dump/ProcTermLog.txt"	    /* ��¼������ֹ��Ϣ�ļ����� */
#define SCP_PROC_TERM_LOG_BAK			    "/mnt/app/dump/ProcTermLogBak.txt"	/* ��¼������ֹ��Ϣ�����ļ����� */
#define SCP_STACK_DUMP_LOG   			    "/mnt/app/dump/StackDump.txt"	    /* ��¼��ջdump��Ϣ�ļ����� */
#define SCP_STACK_DUMP_LOG_BAK			    "/mnt/app/dump/StackDumpBak.txt"    /* ��¼��ջdump��Ϣ�����ļ����� */
#else
#define SCP_PROC_TERM_LOG   			    "/mnt/var/dump/ProcTermLog.txt"	    /* ��¼������ֹ��Ϣ�ļ����� */
#define SCP_PROC_TERM_LOG_BAK			    "/mnt/var/dump/ProcTermLogBak.txt"	/* ��¼������ֹ��Ϣ�����ļ����� */
#define SCP_STACK_DUMP_LOG   			    "/mnt/var/dump/StackDump.txt"	    /* ��¼��ջdump��Ϣ�ļ����� */
#define SCP_STACK_DUMP_LOG_BAK			    "/mnt/var/dump/StackDumpBak.txt"    /* ��¼��ջdump��Ϣ�����ļ����� */
#endif

/* ϵͳTNO�����ֵ */
#define SCP_MAX_TNO                         (255)

/* ÿ�������������һ������������� */
#define SCP_MAX_TASK_NUM                    (8)

/* �Ƿ���һ��������ƽṹ���� */
#define SCP_INVALID_TASK_CTRL_INDEX         (0xFF)

/* NTP��ʱͬ��ʱ��(����) */
#define SCP_NTP_UPDATE_TIMER                (60)

/* �ȴ��ϵ���Ӧ������Դ��� */
#define SCP_WAIT_POWERON_MAXTRY             (3)

/* ��Ӧ���������ֵ�ʧ�ܳ��Դ��� */
#define SCP_WAIT_KEEPALIVE_MAXTRY           (3)

/* ��ʱ����״̬ */
#define SCP_KEEPALIVE_STAT_LIVE             (1)
#define SCP_KEEPALIVE_STAT_DEAD             (0)

/* ϵͳ����ԭ�����ŵ��ļ� */
#if USP_CPU_TYPE_ppc_8xx || USP_CPU_TYPE_ppc_82xx
#define SCP_STARTUP_REASON_FILE             "/mnt/app/ResetStatus"
#else
#define SCP_STARTUP_REASON_FILE             "/mnt/var/ResetStatus"
#endif

/* ÿ����Ϣ���������е���Ϣ�������ռ� */
#if USP_OS_LINUX
#define SCP_MSG_BUF_MAX_SIZE                (16 * 1024)
#else
#define SCP_MSG_BUF_MAX_SIZE                (0)
#endif

/* ���SCP��ʱ���ֵ�ʱ����(����) */
#define SCP_IB_KEEPALIVE_TIMER              (10 * 1000)

/* ���SCP���ֵ�ʧ�ܳ��Դ��� */
#define SCP_IB_WAIT_KEEPALIVE_MAXTRY        (12)

/* ����ͳ�ƶ�ʱ�����ʱ����(����) */
#define SCP_PSP_SAVE_TIMER                  (10 * 1000)

/* ���ƶ��� */
#if USP_OSS_LED_USE
#define SCP_LED_NO                          USP_OSS_LED_FLASH_NO
#define SCP_LED_STARTUPING                  USP_OSS_LED_FLASH_STARTUPING
#define SCP_LED_APP_START_ERR               USP_OSS_LED_FLASH_APP_START_ERR
#define SCP_LED_RUNNING                     USP_OSS_LED_FLASH_RUNNING
#define SCP_LED_ERR_FATAL                   USP_OSS_LED_FLASH_ERR_FATAL

#define SCP_LED_FLASH_ON(id)                usp_oss_led_on(id)
#define SCP_LED_FLASH_OFF(id)               usp_oss_led_off(id)
#else
#define SCP_LED_FLASH_ON(id)
#define SCP_LED_FLASH_OFF(id)
#endif

#if USP_OS_LINUX

/* ��ӡ�궨�� */
#define     SCP_TERMINAL_PRINT(info)                perror(info)
#define     SCP_DEBUG(...)                          usp_oss_printf(USP_OSS_SCP_TNO, USP_OSS_PRINTF_LEVEL_DEBUG, __VA_ARGS__)
#define     SCP_IS_DEBUG                            usp_oss_isPrintf(USP_OSS_SCP_TNO, USP_OSS_PRINTF_LEVEL_DEBUG)
#define     SCP_ERROR(...)                          usp_oss_printf(USP_OSS_SCP_TNO, USP_OSS_PRINTF_LEVEL_ERROR, __VA_ARGS__)
#define     SCP_INFO(...)                           usp_oss_printf(USP_OSS_SCP_TNO, USP_OSS_PRINTF_LEVEL_INFO, __VA_ARGS__)

/* ��־���쳣��¼�궨�� */
#define     SCP_LOG_START(info)                             \
            USP_LAP_START_REPORT(info)

#define     SCP_LOG_RESET(info)                             \
            USP_LAP_RESET_REPORT(info);                     \
            SCP_ERROR("SCP Log(Reset): %s \r\n", info)

#define     SCP_LOG_INFO(info)                              \
            USP_LAP_LOG_REPORT(USP_LAP_INFO, info);         \
            SCP_DEBUG("SCP Log(Info): %s \r\n", info)
            
#define     SCP_EXCEPT(exceptCode, var1, var2, var3)        \
            USP_LAP_ABN_REPORT(var1, var2, var3, USP_LAP_ABN_FILE_ID_OSS, exceptCode); \
            SCP_ERROR("%s exception: exceptCode = %u, line = %u, var1 = %u, var2 = %u, var3 = %u \r\n", __FILE__, exceptCode, __LINE__, var1, var2, var3)

#endif  /* USP_OS_LINUX */

#if USP_OS_FREERTOS

/* ��ӡ�궨�� */
#if USP_VERSION_DEBUG
#define     SCP_TERMINAL_PRINT(info)    \
do                                      \
{                                       \
    if (s_ucIsDebug)                    \
    {                                   \
        cprintf(info);                  \
    }                                   \
} while (0)

#define     SCP_DEBUG(...)              \
do                                      \
{                                       \
    if (s_ucIsDebug)                    \
    {                                   \
        cprintf(__VA_ARGS__);           \
    }                                   \
} while (0)

#define     SCP_IS_DEBUG                            TRUE   
#else
#define     SCP_TERMINAL_PRINT(info)             
#define     SCP_DEBUG(...)              
#define     SCP_IS_DEBUG                            FALSE
#endif
#define     SCP_ERROR                               SCP_DEBUG
#define     SCP_INFO                                SCP_DEBUG

/* ��־���쳣��¼�궨�� */
#define     SCP_LOG_START(info)

#define     SCP_LOG_RESET(info)                         \
            SCP_ERROR("SCP Log(Reset): %s \r\n", info)

#define     SCP_EXCEPT(exceptCode, var1, var2, var3)    \
            SCP_ERROR("%s exception: exceptCode = %u, line = %u, var1 = %u, var2 = %u, var3 = %u \r\n", __FILE__, exceptCode, __LINE__, var1, var2, var3)

#endif  /* USP_OS_FREERTOS */

/**************************************************************************
 *                            ��������                                    *
 **************************************************************************/
/* ��Ϣ�������ݽṹ */
typedef struct tagT_ScpMsgBuff
{
    WORD32                  dwSpace;             /* ʣ��ռ� */
    T_USPPubAdtList         tListMsgBuff;        /* ���ݿ����� */
}T_ScpMsgBuff;
        
/* ��Ϣ����ڵ����ݽṹ*/
typedef struct tagT_ScpMsgBlock
{
    T_USPPubAdtListNode     tNode;
    BYTE				    *pucBlock;           /* ָ�򻺴�����ʼ��ַ */
    WORD16                  wBlockLength;        /* ����鳤�� */
    WORD16                  wPad;
}T_ScpMsgBlock;
        
/* ����ϵͳ��ʼ��������Ŀ */
typedef struct tagT_ScpAppInitItem
{
    usp_oss_scp_initEntry   *initEntry;    /* ��ʼ������ */
    BYTE                    ucErrorType;   /* �����Ĵ���ʽ */
}T_ScpAppInitItem;

/* һ�������������ݽṹ */
typedef struct tagT_ScpTaskConfig
{
    char                    *pcName;        /* һ���������� */
    usp_oss_scp_taskEntry   *taskEntry;     /* ��������ں��� */
    WORD16                  wStackDepth;    /* һ������Ķ�ջ��С(��λ: �ֽ�) */
    SWORD16                 swPriority;     /* һ����������ȼ� */
    USP_OssScpTno           tno;            /* �����ʶ */   
    WORD16                  wMaxMsgNum;     /* ������Ϣ�����а�������Ϣ������ */
    WORD16                  wMsgItemSize;   /* ��Ϣ�����а����ĵ�����Ϣ��С */
    WORD16                  wMsgQueueKey;   /* ��Ϣ���еļ�ֵ */
    BYTE                    ucErrorType;    /* �����Ĵ����� */
    BYTE                    ucIsStopMsg;    /* �Ƿ���Ҫ����ֹͣ��Ϣ */
    BYTE                    ucIsKeepAlive;  /* �Ƿ���Ҫ��ʱ���� */
}T_ScpTaskConfig;

/* �������������ݽṹ */
typedef struct tagT_ScpMainTaskConfig
{
    char                    *pcName;                            /* ���������� */
    SWORD16                 swPriority;                         /* ����������ȼ� */
	WORD32                  dwDevModel;                         /* ���õĻ��� */
    T_ScpTaskConfig         atScpTaskConfig[SCP_MAX_TASK_NUM];  /* һ������������Ϣ */
}T_ScpMainTaskConfig;

/* ������������ݽṹ */
typedef struct tagT_ScpMainTaskCtrl
{ 
    T_ScpMainTaskConfig         *ptScpMainTaskConfig;  /* ָ�����������ýṹ */
    USP_OssVosPid               pid;                   /* ϵͳ����ı�ʶ�� */
    BYTE                        ucTaskCtrl;            /* ��һ��һ�����������Ϣ���� */
    BYTE                        ucTaskNum;             /* ������һ��������� */
    BYTE                        ucErrorType;           /* ������������������������Ĵ���ʽ */                  
    BYTE                        ucIsValid;             /* �����Ƿ���� */
    BYTE                        ucRealTaskNum;         /* ��������ʵ������� */
    BYTE                        ucPad;
    WORD16                      wPad;        
}T_ScpMainTaskCtrl;

/* һ������������ݽṹ */
typedef struct tagT_ScpTaskCtrl
{ 
    T_ScpTaskConfig             *ptScpTaskConfig;      /* ָ��һ���������ýṹ */
    USP_OssIpcMsgQueue          msgQueue;              /* ��Ϣ���� */
    WORD16                      wRunStatus;            /* ����ĵ�ǰ״̬ */
    BYTE                        ucIsValid;             /* �����Ƿ���� */
    BYTE                        ucKeepAliveStat;       /* ��ʱ����״̬ */
    T_ScpMsgBuff                tScpMsgBuff;           /* ��Ϣ���� */
}T_ScpTaskCtrl;

/* TCB�������ݽṹ */
typedef struct tagT_ScpTcbCtrl
{ 
    WORD32                      dwTnoCtrlPos;
    WORD32                      dwMainTaskCtrlPos;
    WORD32                      dwTaskCtrlPos;    
    WORD32                      dwCustAllocPos;
    USP_OssIpcMutex             custMutex;
    WORD16                      wCustAllocCur;
    WORD16                      wCustTaskMax;
}T_ScpTcbCtrl;

/* SCP��̬�������������������� */
typedef struct tagT_ScpLibHandle
{
    USPLibScpTno                localTno;                               /* �û��ű������ */
    USPLibScpTno                scpTno;                                 /* SCPģ������� */
    WORD16		                wMsgSequence;                           /* ��Ϣ��� */
    WORD16                      wPad;
    BYTE                        aucMsgBuf[USP_LIB_IPC_MSG_MAX_SIZE];    /* ��Ϣ�շ������� */
    T_ScpTcbCtrl                *ptTcbCtrl;                             /* TCB�������ݽṹ */
    BYTE                        *pucTnoCtrl;                            /* TNO��������һ���������ṹ��Ӧ��ϵ */
    T_ScpTaskCtrl               *ptTaskCtrl;                            /* һ���������ṹͷָ�� */
    BYTE                        *pucCustAlloc;
}T_ScpLibHandle;

/* ���SCP������ϢӦ�� */
typedef struct tagT_ScpIbKeepAliveAck
{
    WORD32                      dwTime;                 /* ��ǰʱ�� */
}T_ScpIbKeepAliveAck;

/**************************************************************************
 *                             ������                                      *
 **************************************************************************/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/
/* ������ʼ����ں��� */
#define SCP_DECLEAR_INIT_ENTRY(NAME) \
extern usp_oss_scp_initEntry NAME;

/* ��ʼ����ϵͳ��ʼ����Ϣ */
#define SCP_BEGIN_APPINIT_DEFINE \
static T_ScpAppInitItem s_atScpAppInitItem[] = {

/* �Զ���ϵͳ��ʼ�� */
#define SCP_IMPLEMENT_APPINIT(ENTRY, ERROR_TYPE) \
{ENTRY, ERROR_TYPE},

/* ��������ϵͳ��ʼ����Ϣ */
#define  SCP_END_APPINIT_DEFINE \
{NULL, 0} \
};

/* ����������ں��� */
#define SCP_DECLEAR_TASK_ENTRY(NAME) \
extern usp_oss_scp_taskEntry NAME;

/* ����һ������������Ϣ */
#define SCP_DECLEAR_UPPERTASK(NAME) \
static T_ScpTaskConfig NAME[];

/* ��ʼ�������� */
#define SCP_BEGIN_TASK_DEFINE \
static T_ScpMainTaskConfig s_atScpMainTaskConfig[] = {

/* ������������ */
#define SCP_END_TASK_DEFINE \
};

/* ���õ�һ���ϵ���� */
#define SCP_FIRST_POWERON_TNO(TNO_SET...) \
static WORD32 s_adwScpFirstPowerOnTno[] = {0xFFFFFFFF, ## TNO_SET, 0xFFFFFFFF};

/* ���õڶ����ϵ���� */
#define SCP_SECOND_POWERON_TNO(TNO_SET...) \
static WORD32 s_adwScpSecondPowerOnTno[] = {0xFFFFFFFF, ## TNO_SET, 0xFFFFFFFF};

/* ���õ������ϵ���� */
#define SCP_THIRD_POWERON_TNO(TNO_SET...) \
static WORD32 s_adwScpThirdPowerOnTno[] = {0xFFFFFFFF, ## TNO_SET, 0xFFFFFFFF};

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
 
/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/


#endif  /* _OSS_SCP_H */

