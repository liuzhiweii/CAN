/************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_lib_scp.h
* �ļ���ʶ�� 
* ����ժҪ�� ϵͳ���ض�̬�����ӿ�
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
#ifndef _USP_LIB_SCP_H
#define _USP_LIB_SCP_H

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
#define USP_LIB_SUCCESS             (0)

/* ������Ƿ�Ϸ� */
#define USP_LIB_CHECK_HANDLE(handle, member) if (NULL == handle || 0x5A5A5A5A != handle->dwMagic || NULL == (handle)->member) return -1

/* ������̼�ͨ��ѡ�� */
#define USP_LIB_IPC_NOWAIT          USP_LIB_VOS_IPC_NOWAIT         /* �������أ������� */
#define USP_LIB_IPC_FOREVERWAIT     USP_LIB_VOS_IPC_FOREVERWAIT    /* ���� */

/* ������Ϣ��󳤶� */
#define USP_LIB_IPC_MSG_MAX_SIZE    (8 * 1024)

/* ��Ϣ���Ͷ��� */
#define USP_LIB_SCP_MSG_TYPE_GPIO   0       /* GPIO�����Ϣ */
#define USP_LIB_SCP_MSG_TYPE_OTHER  0xFF    /* ������Ϣ */

/* ��Ϣ������� */
#define USP_LIB_SCP_MSGSEQ(n)       (0xFFFF == (++(n)) ? ++(n) : (n))

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                            ��������                                    *
 **************************************************************************/
/* SCP������������������� */
typedef struct tagT_ScpLibHandle    *USPLibScpHandle;

/* ͨ����Ϣ������������ */
typedef struct tagT_Msg             *USPLibScpMsg;

/* �����ʶ������������ */
typedef WORD16                       USPLibScpTno;

/* ���������Ϣ��Ϣ�ṹ */
typedef struct tagT_USPLibScpAnalyseMsg
{
    WORD16  wMsgType;   /* ��Ϣ���� */
    WORD16  wPad;
}T_USPLibScpAnalyseMsg;

/* CPU�ڽ��̼�ͨ����Ϣͷ */
typedef struct tagT_USPLibScpMsg
{
    long        mtype;          /* Message type, used by linux */
    BYTE        ucLinkType;     /* ��·�������� */
    BYTE        ucLinkIndex;    /* ��·������ */
    WORD16      wSenderTno;     /* ���Ͷ������ */
    WORD16      wReceiverTno;   /* ���ն������ */    
    WORD16      wMsgID;         /* ��Ϣ�¼��� */
    WORD16      wMsgLen;        /* ��Ϣ���ȣ���������Ϣͷ��BYTEΪ��λ */
    WORD16      wMsgSequence;   /* ��Ϣ��� */ 
    WORD16      wSrcTno;        /* Դ����� */
    WORD16      wDstTno;        /* Ŀ������� */
    WORD16      wRelayMagic;    /* �м���Ϣħ���� */
    WORD16      wPad;
}T_USPLibScpMsg;

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/

/* ��USP��̬�� */
extern WORD32 usp_lib_open(USPLibHandle *pHandle, BYTE *pucDesc);

/* �ر�USP��̬�� */
extern WORD32 usp_lib_close(USPLibHandle handle);

/* ��USP��̬��(������ģ��ȫ����) */
extern WORD32 usp_lib_openAll(USPLibHandle * pHandle, BYTE * pucDesc);

/* �ر�USP��̬��(������ģ��ȫ���ر�) */
extern WORD32 usp_lib_closeAll(USPLibHandle handle);

/* ��SCPģ�� */
extern WORD32 usp_lib_scp_open(USPLibHandle handle, BYTE *pucDesc);

/* �ر�SCPģ�� */
extern WORD32 usp_lib_scp_close(USPLibHandle handle);

/* ��ȡ����� */
extern WORD32 usp_lib_scp_getTno(USPLibHandle handle, USPLibScpTno *pTno);

/* ��ָ��������ŷ�����Ϣ */
extern WORD32 usp_lib_scp_sendTaskMsg(USPLibHandle handle, USPLibScpTno tno, USPLibScpMsg msg, WORD32 dwFlag);

/* ������Ϣ������Ϣ������˳���ȡ��Ϣ�� */
extern WORD32 usp_lib_scp_recvMsg(USPLibHandle handle, USPLibScpMsg msg, WORD16 wSize, WORD32 dwFlag);

/* �����ض����͵���Ϣ */
extern WORD32 usp_lib_scp_recvMsgWithTypes(USPLibHandle handle, USPLibScpMsg msg, WORD16 wSize, WORD32 dwTimeOut, USPLibScpMsg aExpectMsg[]);

/* ��λ�Ķ��� */
extern WORD32 usp_lib_scp_reset(USPLibHandle handle);

/* �����Ķ���ʱ�� */
extern WORD32 usp_lib_scp_setTime (USPLibHandle handle, WORD32 dwTime);

/* ��ȡ�Ķ���ʱ�� */
extern WORD32 usp_lib_scp_getTime (USPLibHandle handle, WORD32 *pdwTime);

/* ������Ϣ */
extern WORD32 usp_lib_scp_analyseMsg(USPLibScpMsg msg, T_USPLibScpAnalyseMsg *ptAnalyseMsg);

#endif  /* _USP_LIB_SCP_H */

