/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_eventDef.h
* �ļ���ʶ�� 
* ����ժҪ�� ��Ϣ����
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
#ifndef _USP_EVENT_DEF_H
#define _USP_EVENT_DEF_H


/*  16bit��Ϣ���붨��:                                             
  
------------+----------------------
��Ϣǰ׺    |  ��ϢӦ�÷�Χ
------------+---------------------
EV_           ��ʾ����Ϣ����
EV_           ϵͳȫ����Ϣ
EV_USP_       USPϵͳ��Ϣ
EV_USP_CLI_   ��������(CLI)�йص���Ϣ
EV_RM_        RSP �복�����������Ϣ
EV_RO_        OAM����������
EV_S_R_       SCP��RSP����Ϣ
EV_S_O_       SCP��OAM����Ϣ
EV_S_C_       SCP��COMM����Ϣ
EV_R_S_       RSP��SCP����Ϣ
EV_R_O_       RSP��OAM����Ϣ
EV_O_R_       OAM��RSP����Ϣ
EV_O_S_       OAM��SCP����Ϣ
EV_O_C_       OAM��COMM����Ϣ
EV_C_S_       COMM��SCP����Ϣ
EV_C_O_       COMM��OAM����Ϣ
EV_S_         SCPģ���ڲ�����Ϣ
EV_C_         COMMģ���ڲ�����Ϣ
EV_O_         OAMģ���ڲ�����Ϣ
EV_R_         RSPģ���ڲ�����Ϣ
EV_D_         DSRCģ���ڲ�����Ϣ
*/

/**********************************************************************
*                             ��Ϣ��ֵ                                *
**********************************************************************/

/* ϵͳ����(SCP)��Ϣ��ֵ���� */
#define EV_USP_SCP_Begin                               ((WORD16)(0xFC00)) 

/* �����н���(CLI)��Ϣ��ֵ���� */
#define EV_USP_CLI_Begin                               ((WORD16)(0xFE00)) 

/* ��־���쳣̽��(LAP)��Ϣ��ֵ���� */
#define EV_USP_LAP_Begin                               ((WORD16)(0xFF00)) 

/* ͨ��ģ��(COMM)��Ϣ��ֵ���� */
#define EV_USP_COMM_Begin                              ((WORD16)(0xFD00)) 

/* IP����ģ��(IPD)��Ϣ��ֵ���� */
#define EV_USP_COMM_IPD_Begin                          ((WORD16)(0xFB00)) 

/* ����ģ��(CMP)��Ϣ��ֵ���� */
#define EV_USP_OAM_CMP_Begin                           ((WORD16)(0xFA00))

/* �汾����ģ��(VMP)��Ϣ��ֵ���� */
#define EV_USP_OAM_VMP_Begin                           ((WORD16)(0xA800))

/* ��ϲ���(DMP)��Ϣ��ֵ����*/
#define EV_USP_OAM_DMP_Begin                          ((WORD16)(0xAC00))

/* ״̬���(AMP)��Ϣ��ֵ����*/
#define EV_USP_OSS_SMP_Begin                          ((WORD16)(0xAE00))

/* �澯����ģ��(AMP)��Ϣ��ֵ���� */
#define EV_USP_OAM_AMP_Begin                          ((WORD16)(0xF100)) 

/* MPI ģ����Ϣ��ֵ���� */
#define EV_USP_MPI_Begin                              ((WORD16)(0xF200))

/* ��չPSAM����������Ϣ��ֵ���� */
#define EV_USP_PSAM_EXT_Begin                           ((WORD16)(0xF300)) 

/* GPIO��RELAY����ģ����Ϣ��ֵ���� */
#define EV_USP_GPIO_RELAY_Begin                           ((WORD16)(0xF400))

/* ��������(MISC)��Ϣ��ֵ����*/
#define EV_USP_OSS_MISC_Begin                          ((WORD16)(0xF500))

/* �����ǩģ����Ϣ��ֵ���� */
#define EV_USP_VIR_TAG_Begin                           ((WORD16)(0xF600)) 

/**********************************************************************
*                             ��Ϣ����                                *
**********************************************************************/

/**********************************************************************
*                             ϵͳ����(SCP)��Ϣ����                   *
**********************************************************************/
#define EV_USP_SCP_PowerOn                  (EV_USP_SCP_Begin + 1)  /* �����ϵ���Ϣ */
#define EV_USP_SCP_PowerOnSuccess           (EV_USP_SCP_Begin + 2)  /* �����ϵ�ɹ���Ϣ */
#define EV_USP_SCP_PowerOnFailure           (EV_USP_SCP_Begin + 3)  /* �����ϵ�ʧ����Ϣ */ 
#define EV_USP_SCP_StopProcess              (EV_USP_SCP_Begin + 4)  /* ֹͣ���̣�����Ӧ�ý�������(exit)����Ҫ����֪ͨVMP��CMP��Ҫ�ٽ����κ�FLASH���� */
#define EV_USP_SCP_TimerWaitPowerOn         (EV_USP_SCP_Begin + 5)  /* �ȴ�Ӧ��������ϵ�Ӧ��ʱ�� */
#define EV_USP_SCP_KeepAlive                (EV_USP_SCP_Begin + 6)  /* SCP���Ӧ�������������Ϣ�����ڼ�������Ƿ��������� */
#define EV_USP_SCP_KeepAliveAck             (EV_USP_SCP_Begin + 7)  /* Ӧ��������SCP���ص�����Ӧ����Ϣ */
#define EV_USP_SCP_TimerKeepAlive           (EV_USP_SCP_Begin + 8)  /* ��Ӧ���������ֵĶ�ʱ����Ϣ */
#define EV_USP_SCP_TimerNtpUpdate           (EV_USP_SCP_Begin + 9)  /* NTPʱ��ͬ���Ķ�ʱ����Ϣ */
#define EV_USP_SCP_Reset                    (EV_USP_SCP_Begin + 10) /* ϵͳ������Ϣ */
#define EV_USP_SCP_ResetAck                 (EV_USP_SCP_Begin + 11) /* ϵͳ����Ӧ����Ϣ */
#define EV_USP_SCP_SetTime                  (EV_USP_SCP_Begin + 12) /* ����ʱ����Ϣ */
#define EV_USP_SCP_SetTimeAck               (EV_USP_SCP_Begin + 13) /* ����ʱ��Ӧ����Ϣ */
#define EV_USP_SCP_TimerIbKeepAlive         (EV_USP_SCP_Begin + 14) /* ���SCP���ֶ�ʱ����Ϣ */
#define EV_USP_SCP_IbKeepAlive              (EV_USP_SCP_Begin + 15) /* ���SCP������Ϣ */
#define EV_USP_SCP_IbKeepAliveAck           (EV_USP_SCP_Begin + 16) /* ���SCP����Ӧ����Ϣ */
#define EV_USP_SCP_PspSave                  (EV_USP_SCP_Begin + 17) /* ��ʱ��������ͳ����Ϣ */
#define EV_USP_SCP_TimerBeginRecvMsg        (EV_USP_SCP_Begin + 18) /* ������Ϣ��ʱ��ʱ����ʼ��Ϣ */

/**********************************************************************
*                             �����н���(CLI)��Ϣ����                 *
**********************************************************************/
#define EV_USP_CLI_Cmd_Request                         (EV_USP_CLI_Begin + 1)
#define EV_USP_CLI_Cmd_Response                        (EV_USP_CLI_Begin + 2)
#define EV_USP_CLI_Alarm_Inter                         (EV_USP_CLI_Begin + 3)
#define EV_USP_CLI_Close_Shell                         (EV_USP_CLI_Begin + 4)
#define EV_USP_CLI_Monitor_Ctrl                        (EV_USP_CLI_Begin + 5) 
#define EV_USP_CLI_Monitor_Ctrl_Ack                    (EV_USP_CLI_Begin + 6) 
#define EV_USP_CLI_Trace_Ctrl                          (EV_USP_CLI_Begin + 7) 
#define EV_USP_CLI_Trace_Ctrl_Ack                      (EV_USP_CLI_Begin + 8) 
 
/**********************************************************************
*                             ��־���쳣̽��(LAP)��Ϣ����             *
**********************************************************************/
#define EV_USP_LAP_LogReport                                        (EV_USP_LAP_Begin + 0x01) /* ��־�ϱ���Ϣ */
#define EV_USP_LAP_AbnReport                                        (EV_USP_LAP_Begin + 0x02) /* �쳣�ϱ���Ϣ */
#define EV_USP_OAM_LAP_GetReaderLog                                 (EV_USP_LAP_Begin + 0x03) /* ��̨�����ϴ���־��Ϣ */
#define EV_USP_OAM_LAP_GetReaderLogResponse                         (EV_USP_LAP_Begin + 0x04) /* ǰ̨������־��Ϣ */
#define EV_USP_OAM_LAP_GetReaderLogResponseAck                      (EV_USP_LAP_Begin + 0x05) /* ��̨�յ���־�ϴ�Ӧ����Ϣ */
#define EV_USP_OAM_LAP_GetReaderException                           (EV_USP_LAP_Begin + 0x06) /* ��̨�����ϴ��쳣��Ϣ */
#define EV_USP_OAM_LAP_GetReaderExceptionResponse                   (EV_USP_LAP_Begin + 0x07) /* ǰ̨�����쳣��Ϣ*/
#define EV_USP_OAM_LAP_GetReaderExceptionResponseAck                (EV_USP_LAP_Begin + 0x08) /* ��̨�յ��쳣�ϴ�Ӧ����Ϣ */
#define EV_USP_OAM_LAP_WaitGetReaderLogResponseAckOverTime          (EV_USP_LAP_Begin + 0x09) /* �ȴ���̨�յ���־��ϢӦ��ACK��Ϣ�Ķ�ʱ�� */
#define EV_USP_OAM_LAP_WaitGetReaderExceptionResponseAckOverTime    (EV_USP_LAP_Begin + 0x0A) /* �ȴ���̨�յ��쳣��ϢӦ��ACK��Ϣ�Ķ�ʱ�� */
#define EV_USP_OAM_LAP_LogClear                                     (EV_USP_LAP_Begin + 0x0B) /* �����־ */
#define EV_USP_OAM_LAP_LogClear_ACK                                 (EV_USP_LAP_Begin + 0x0C) /* �����־ */
#define EV_USP_OAM_LAP_AbnClear                                     (EV_USP_LAP_Begin + 0x0D) /* ����쳣̽�� */
#define EV_USP_OAM_LAP_AbnClear_ACK                                 (EV_USP_LAP_Begin + 0x0E) /* ����쳣̽�� */
/**********************************************************************
*                             ͨ��ģ��(COMM)��Ϣ����                  *
**********************************************************************/
#define EV_USP_COMM_CreateSocketListen      (EV_USP_COMM_Begin + 1)  /* ����Socket��������˿� */
#define EV_USP_COMM_CreateSocketConnect     (EV_USP_COMM_Begin + 2)  /* ����Socket���� */
#define EV_USP_COMM_CreateLinkFd            (EV_USP_COMM_Begin + 3)  /* ���ͨ����·���ݵ������ */
#define EV_USP_COMM_DeleteSocketConnect     (EV_USP_COMM_Begin + 4)  /* ɾ��Socket���� */
#define EV_USP_COMM_LinkCloseNotice         (EV_USP_COMM_Begin + 5)  /* ��·�ر�֪ͨ */
#define EV_USP_COMM_NewLinkNotice           (EV_USP_COMM_Begin + 6)  /* �µ���·������֪ͨ��Ϣ  */
#define EV_USP_COMM_SendData                (EV_USP_COMM_Begin + 7)  /* ͨ��ͨ����·�������� */
#define EV_USP_COMM_RecvData                (EV_USP_COMM_Begin + 8)  /* ת�����ݸ�Ӧ��ģ�� */
#define EV_USP_COMM_CloseLink               (EV_USP_COMM_Begin + 9)  /* �ر���· */
#define EV_USP_COMM_LinkOpened              (EV_USP_COMM_Begin + 10) /* �����ɹ����֪ͨ��Ϣ (���͸�Ӧ��ģ��) */
#define EV_USP_COMM_LinkClosed              (EV_USP_COMM_Begin + 11) /* ���ӶϿ���֪ͨ��Ϣ (���͸�Ӧ��ģ��) */ 
#define EV_USP_COMM_CfgRequest              (EV_USP_COMM_Begin + 12) /* ��ѯ������Ϣ */ 
#define EV_USP_COMM_CfgRequestAck           (EV_USP_COMM_Begin + 13) /* ����Ӧ����Ϣ */ 
#define EV_USP_COMM_LinkCompleteOpened      (EV_USP_COMM_Begin + 14) /* ��·��ȫ�� */ 
#define EV_USP_COMM_SendReportData          (EV_USP_COMM_Begin + 15)            /* ͨ��ͨ����·���ͱ��������� */
#define EV_USP_COMM_DumpCongestionData      (EV_USP_COMM_Begin + 16)            /* ����ӵ���������� */
#define EV_USP_COMM_WriteHoldData           (EV_USP_COMM_Begin + 18)            /* ��������������� */
#define EV_USP_COMM_InterBoardKeepAlive     (EV_USP_COMM_Begin + 19)            /* ���ͨ��������Ϣ */
#define EV_USP_COMM_SetLinkForward          (EV_USP_COMM_Begin + 20)            /* ������·ת�� */
#define EV_USP_COMM_CancelLinkForward       (EV_USP_COMM_Begin + 21)            /* ȡ����·ת�� */
#define EV_USP_COMM_SendSm                  (EV_USP_COMM_Begin + 22)            /* ���Ͷ��� */
#define EV_USP_COMM_RecvSm                  (EV_USP_COMM_Begin + 23)            /* ���ն��� */
#define EV_USP_COMM_SmsSignalQualityReq     (EV_USP_COMM_Begin + 24)            /* GSM�źŲ�ѯ���� */
#define EV_USP_COMM_SmsSignalQualityAck     (EV_USP_COMM_Begin + 25)            /* GSM�źŲ�ѯӦ�� */
#define EV_USP_COMM_TimerBegin              (EV_USP_COMM_Begin + 30)            /* ��ʼ��ʱ����Ϣ */
#define EV_USP_COMM_KeepALive               (EV_USP_COMM_TimerBegin + 32 * 0)   /* KEEPALIVE��ʱ����Ϣ */
#define EV_USP_COMM_SendCongestionData      (EV_USP_COMM_TimerBegin + 32 * 1)   /* ����ӵ��������Ϣ */
#define EV_USP_COMM_SendHoldData            (EV_USP_COMM_TimerBegin + 32 * 2)   /* ���ͻ���������Ϣ */


/**********************************************************************
*                             IP����ģ��(IPD)��Ϣ����                  *
**********************************************************************/
#define  EV_USP_IPD_CloseLink                       (EV_USP_COMM_IPD_Begin + 1) /* �رս�������ʱTCP��· */
#define  EV_USP_IPD_KeepAliveTimer                  (EV_USP_COMM_IPD_Begin + 2) /* ��ʱ����ʱ��Ϣ */
#define  EV_USP_IPD_ReaderEventNotification         (EV_USP_COMM_IPD_Begin + 3) /* ����ReaderEventNotification�������߳���Ϣid */
#define  EV_USP_IPD_GetReaderCfgReq				    (EV_USP_COMM_IPD_Begin + 4) /* ������ģ�鷢�ͻ�ȡ����������Ϣ */
#define  EV_USP_IPD_GetReaderCfgAck				    (EV_USP_COMM_IPD_Begin + 5) /* �յ�����ģ�鷢���Ļ�ȡ����Ӧ����Ϣ */
#define  EV_USP_IPDUDP_GetReaderCfgReq				(EV_USP_COMM_IPD_Begin + 6) /* ��CMPģ�鷢�ͻ�ȡ����������Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_GetReaderCfgAck				(EV_USP_COMM_IPD_Begin + 7) /* �յ�CMPģ�鷢���Ļ�ȡ����Ӧ����Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_SetReaderCfgReq              (EV_USP_COMM_IPD_Begin + 8) /* ��CMPģ�鷢����������������Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_SetReaderCfgAck              (EV_USP_COMM_IPD_Begin + 9) /* �յ�CMPģ�鷢������������Ӧ����Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_WaitGetReaderCfgAckOverTime  (EV_USP_COMM_IPD_Begin + 10) /* �ȴ�CMPģ�鷢���Ļ�ȡ����Ӧ����Ϣ��ʱ��Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_WaitSetReaderCfgAckOverTime  (EV_USP_COMM_IPD_Begin + 11) /* �ȴ�CMPģ�鷢������������Ӧ����Ϣ��ʱ��Ϣ��UDP��ʽʹ�� */
#define  EV_USP_IPDUDP_WaitClearMsqOverTime         (EV_USP_COMM_IPD_Begin + 12) /* �ȴ������Ϣ���л�������ʱ */
#define  EV_USP_OAM_IPD_IPDiscoveryReq              (EV_USP_COMM_IPD_Begin + 13) /* IP����������Ϣ */
#define  EV_USP_OAM_IPD_IPDiscoveryAck              (EV_USP_COMM_IPD_Begin + 14) /* IP����Ӧ����Ϣ */
#define  EV_USP_OAM_IPD_SetReaderCfgReq             (EV_USP_COMM_IPD_Begin + 15) /* �����豸������Ϣ */
#define  EV_USP_OAM_IPD_SetReaderCfgAck             (EV_USP_COMM_IPD_Begin + 16) /* �����豸Ӧ����Ϣ */
#define  EV_USP_OAM_IPD_ResetReq                    (EV_USP_COMM_IPD_Begin + 17) /* ��λ�豸������Ϣ */
#define  EV_USP_OAM_IPD_ResetAck                    (EV_USP_COMM_IPD_Begin + 18) /* ��λ�豸Ӧ����Ϣ */
#define  EV_USP_OAM_IPD_WaitClearMsqOverTime        (EV_USP_COMM_IPD_Begin + 19) /* �ȴ������Ϣ���л�������ʱ */
#define  EV_USP_OAM_IPD_WaitIPDiscoveryAckOverTime  (EV_USP_COMM_IPD_Begin + 20) /* �ȴ�CMPģ�鷢���Ļ�ȡ����Ӧ����Ϣ��ʱ��Ϣ */
#define  EV_USP_OAM_IPD_WaitSetReaderCfgAckOverTime (EV_USP_COMM_IPD_Begin + 21) /* �ȴ�CMPģ�鷢������������Ӧ����Ϣ��ʱ��Ϣ */

/**********************************************************************
*                             ����ģ��(CMP)��Ϣ����                  *
**********************************************************************/
#define EV_USP_OAM_CMP_CfgParaChg               (EV_USP_OAM_CMP_Begin + 1)  /* ���ñ��֪ͨ��Ϣ */
#define EV_USP_OAM_CMP_SetCfgPara               (EV_USP_OAM_CMP_Begin + 2)  /* ��������������Ϣ */
#define EV_USP_OAM_CMP_GetCfgPara               (EV_USP_OAM_CMP_Begin + 3)  /* ��ѯ����������Ϣ */
#define EV_USP_OAM_CMP_GetCfgParaByTno          (EV_USP_OAM_CMP_Begin + 4)  /* ��������Ų�ѯ��֮��ص�����������Ϣ */
#define EV_USP_OAM_CMP_SetCfgParaAck            (EV_USP_OAM_CMP_Begin + 5)  /* ��������������Ϣ��Ӧ */
#define EV_USP_OAM_CMP_GetCfgParaAck            (EV_USP_OAM_CMP_Begin + 6)  /* ��ѯ����������Ϣ��Ӧ */
#define EV_USP_OAM_CMP_GetCfgParaByTnoAck       (EV_USP_OAM_CMP_Begin + 7)  /* ��������Ų�ѯ��֮��ص�����������Ϣ��Ӧ */
#define EV_USP_OAM_CMP_Reset                    (EV_USP_OAM_CMP_Begin + 8)  /* �����豸��Ϣ */
#define EV_USP_OAM_CMP_SetFactoryDefault        (EV_USP_OAM_CMP_Begin + 9)  /* �ָ��������� */
#define EV_USP_OAM_CMP_AMPConfigRequest         (EV_USP_OAM_CMP_Begin + 10) /* ����AMP���� */
#define EV_USP_OAM_CMP_AMPConfigRequestAck      (EV_USP_OAM_CMP_Begin + 11) /* ����AMP����Ӧ�� */
#define EV_USP_OAM_CMP_SMPConfigRequest         (EV_USP_OAM_CMP_Begin + 12) /* ����SMP���� */
#define EV_USP_OAM_CMP_SMPConfigRequestAck      (EV_USP_OAM_CMP_Begin + 13) /* ����SMP����Ӧ�� */
#define EV_USP_OAM_CMP_SaveCfgPara              (EV_USP_OAM_CMP_Begin + 14) /* ����������Ϣ */
#define EV_USP_OAM_CMP_SaveCfgParaAck           (EV_USP_OAM_CMP_Begin + 15) /* ��������Ӧ����Ϣ */
#define EV_USP_OAM_CMP_NTPConfigRequest         (EV_USP_OAM_CMP_Begin + 16) /* ����NTP���� */
#define EV_USP_OAM_CMP_NTPConfigRequestAck      (EV_USP_OAM_CMP_Begin + 17) /* ����NTP����Ӧ�� */
#define EV_USP_OAM_CMP_RDPConfigRequest         (EV_USP_OAM_CMP_Begin + 18) /* ����RDP���� */
#define EV_USP_OAM_CMP_RDPConfigRequestAck      (EV_USP_OAM_CMP_Begin + 19) /* ����RDP����Ӧ�� */
#define EV_USP_OAM_CMP_RCPConfigRequest         (EV_USP_OAM_CMP_Begin + 20) /* ����RCP���� */
#define EV_USP_OAM_CMP_RCPConfigRequestAck      (EV_USP_OAM_CMP_Begin + 21) /* ����RCP����Ӧ�� */
#define EV_USP_OAM_CMP_MPIConfigRequest         (EV_USP_OAM_CMP_Begin + 22) /* ����MPI���� */
#define EV_USP_OAM_CMP_MPIConfigRequestAck      (EV_USP_OAM_CMP_Begin + 23) /* ����MPI����Ӧ�� */
#define EV_USP_OAM_CMP_ETCConfigRequest         (EV_USP_OAM_CMP_Begin + 24) /* ����ETC���� */
#define EV_USP_OAM_CMP_ETCConfigRequestAck      (EV_USP_OAM_CMP_Begin + 25) /* ����ETC����Ӧ�� */
#define EV_USP_OAM_CMP_SetCfgIntegrated         (EV_USP_OAM_CMP_Begin + 26) /* ����ȫ������������Ϣ */
#define EV_USP_OAM_CMP_SetCfgIntegratedAck      (EV_USP_OAM_CMP_Begin + 27) /* ����ȫ������������Ϣ��Ӧ */
#define EV_USP_OAM_CMP_GetCfgIntegrated         (EV_USP_OAM_CMP_Begin + 28) /* ��ѯȫ������������Ϣ */
#define EV_USP_OAM_CMP_GetCfgIntegratedAck      (EV_USP_OAM_CMP_Begin + 29) /* ��ѯȫ������������Ϣ��Ӧ */
#define EV_USP_OAM_CMP_SetFactoryDefaultAck     (EV_USP_OAM_CMP_Begin + 30) /* �ָ���������Ӧ�� */
#define EV_USP_OAM_CMP_GPIOConfigRequest        (EV_USP_OAM_CMP_Begin + 31) /* ����GPIO���� */
#define EV_USP_OAM_CMP_GPIOConfigRequestAck     (EV_USP_OAM_CMP_Begin + 32) /* ����GPIO����Ӧ��*/

/**********************************************************************
*                             �汾����ģ��(VMP)��Ϣ����               *
**********************************************************************/
#define EV_USP_OAM_VMP_VerDownload                      (EV_USP_OAM_VMP_Begin + 1)  /* �汾���� */
#define EV_USP_OAM_VMP_VerDownloadAck                   (EV_USP_OAM_VMP_Begin + 2)  /* �汾����Ӧ�� */
#define EV_USP_OAM_VMP_VerDownloadQuery                 (EV_USP_OAM_VMP_Begin + 3)  /* �������ذ汾��Ϣ */
#define EV_USP_OAM_VMP_VerDownloadQueryAck              (EV_USP_OAM_VMP_Begin + 4)  /* �������ذ汾��ϢӦ�� */
#define EV_USP_OAM_VMP_VerActive                        (EV_USP_OAM_VMP_Begin + 5)  /* �汾���� */
#define EV_USP_OAM_VMP_VerActiveAck                     (EV_USP_OAM_VMP_Begin + 6)  /* �汾����Ӧ�� */
#define EV_USP_OAM_VMP_VerUnActive                      (EV_USP_OAM_VMP_Begin + 7)  /* �汾������ */
#define EV_USP_OAM_VMP_VerUnActiveAck                   (EV_USP_OAM_VMP_Begin + 8)  /* �汾������Ӧ�� */
#define EV_USP_OAM_VMP_VerDelete                        (EV_USP_OAM_VMP_Begin + 9)  /* �汾ɾ�� */
#define EV_USP_OAM_VMP_VerDeleteAck                     (EV_USP_OAM_VMP_Begin + 10) /* �汾ɾ��Ӧ�� */
#define EV_USP_OAM_VMP_VerQuery                         (EV_USP_OAM_VMP_Begin + 11)  /* �汾��ѯ */
#define EV_USP_OAM_VMP_VerQueryAck                      (EV_USP_OAM_VMP_Begin + 12) /* �汾��ѯӦ�� */
#define EV_USP_OAM_VMP_BootDownload                     (EV_USP_OAM_VMP_Begin + 13) /* boot���� */
#define EV_USP_OAM_VMP_BootDownloadAck                  (EV_USP_OAM_VMP_Begin + 14) /* boot����Ӧ�� */
#define EV_USP_OAM_VMP_BootQuery                        (EV_USP_OAM_VMP_Begin + 15) /* boot��ѯ */
#define EV_USP_OAM_VMP_BootQueryAck                     (EV_USP_OAM_VMP_Begin + 16) /* boot��ѯӦ�� */
#define EV_USP_OAM_VMP_SubDevBootDownload               (EV_USP_OAM_VMP_Begin + 17) /* ���豸boot���� */
#define EV_USP_OAM_VMP_SubDevBootDownloadAck            (EV_USP_OAM_VMP_Begin + 18) /* ���豸boot����Ӧ�� */
#define EV_USP_OAM_VMP_SubDevBootQuery                  (EV_USP_OAM_VMP_Begin + 19) /* ���豸boot��ѯ */
#define EV_USP_OAM_VMP_SubDevBootQueryAck               (EV_USP_OAM_VMP_Begin + 20) /* ���豸boot��ѯӦ�� */
#define EV_USP_OAM_VMP_VerUpdateStart                   (EV_USP_OAM_VMP_Begin + 21) /* �汾���¿�ʼ */
#define EV_USP_OAM_VMP_VerUpdateStartAck                (EV_USP_OAM_VMP_Begin + 22) /* �汾���¿�ʼӦ�� */
#define EV_USP_OAM_VMP_VerUpdateData                    (EV_USP_OAM_VMP_Begin + 23) /* �汾�������� */
#define EV_USP_OAM_VMP_VerUpdateDataAck                 (EV_USP_OAM_VMP_Begin + 24) /* �汾��������Ӧ�� */
#define EV_USP_OAM_VMP_VerUpdateEnd                     (EV_USP_OAM_VMP_Begin + 25) /* �汾���½��� */
#define EV_USP_OAM_VMP_VerUpdateEndAck                  (EV_USP_OAM_VMP_Begin + 26) /* �汾���½���Ӧ�� */
#define EV_USP_OAM_VMP_SubDevBootQueryTimeout           (EV_USP_OAM_VMP_Begin + 27) /* ���豸boot��ѯ��ʱ */
#define EV_USP_OAM_VMP_SubDevBootDownloadTimeout        (EV_USP_OAM_VMP_Begin + 28) /* ���豸boot������ʱ */
#define EV_USP_OAM_VMP_LMT_Begin                        (EV_USP_OAM_VMP_Begin + 50)     /* LMT��ص���ʼ��Ϣ�� */
#define EV_USP_OAM_VMP_LMT_VerDownload                  (EV_USP_OAM_VMP_LMT_Begin + 1)  /* �汾���� */ 
#define EV_USP_OAM_VMP_LMT_VerDownloadAck               (EV_USP_OAM_VMP_LMT_Begin + 2)  /* �汾������Ӧ */
#define EV_USP_OAM_VMP_LMT_VersionDownloadRequest       (EV_USP_OAM_VMP_LMT_Begin + 3)  /* ����汾������Ϣ */
#define EV_USP_OAM_VMP_LMT_VersionDownloadRequestAck    (EV_USP_OAM_VMP_LMT_Begin + 4)  /* ����汾������Ϣ��Ӧ */
#define EV_USP_OAM_VMP_LMT_VerActivate                  (EV_USP_OAM_VMP_LMT_Begin + 5)  /* �汾���� */ 
#define EV_USP_OAM_VMP_LMT_VerActivateAck               (EV_USP_OAM_VMP_LMT_Begin + 6)  /* �汾������Ӧ */ 
#define EV_USP_OAM_VMP_LMT_VerUnActivate                (EV_USP_OAM_VMP_LMT_Begin + 7)  /* �汾ȥ���� */ 
#define EV_USP_OAM_VMP_LMT_VerUnActivateAck             (EV_USP_OAM_VMP_LMT_Begin + 8)  /* �汾ȥ������Ӧ */
#define EV_USP_OAM_VMP_LMT_VersionQuery                 (EV_USP_OAM_VMP_LMT_Begin + 9)  /* �汾��ѯ */ 
#define EV_USP_OAM_VMP_LMT_VersionQueryAck              (EV_USP_OAM_VMP_LMT_Begin + 10) /* �汾��ѯ��Ӧ */
#define EV_USP_OAM_VMP_LMT_UbootVersionQueryRequest     (EV_USP_OAM_VMP_LMT_Begin + 11) /* uboot�汾��ѯ */
#define EV_USP_OAM_VMP_LMT_UbootVersionQueryRequestAck  (EV_USP_OAM_VMP_LMT_Begin + 12) /* uboot�汾��ѯ��Ӧ */
#define EV_USP_OAM_VMP_LMT_UBootUpdateRequest           (EV_USP_OAM_VMP_LMT_Begin + 13) /* uboot�汾���� */
#define EV_USP_OAM_VMP_LMT_UBootUpdateRequestAck        (EV_USP_OAM_VMP_LMT_Begin + 14) /* uboot�汾������Ӧ */

/**********************************************************************
*                            ��ϲ���ģ��(DMP)��Ϣ����        *
**********************************************************************/
#define EV_USP_OAM_DMP_HostRequest            (EV_USP_OAM_DMP_Begin + 1) /* �ⲿģ�鷢����ϲ���������Ϣ*/
#define EV_USP_OAM_DMP_HostResponse           (EV_USP_OAM_DMP_Begin + 2) /* �ⲿģ�鷢����ϲ���Ӧ����Ϣ */
#define EV_USP_OAM_DMP_Request                (EV_USP_OAM_DMP_Begin + 3) /* �����ڲ�ģ�鷢����ϲ���������Ϣ */
#define EV_USP_OAM_DMP_Response               (EV_USP_OAM_DMP_Begin + 4) /* �����ڲ�ģ�鷢����ϲ���Ӧ����Ϣ */
#define EV_USP_OAM_DMP_AppRequest             (EV_USP_OAM_DMP_Begin + 5) /* ��ϲ���ģ�鷢�;���ҵ��Ӧ��ģ����ϲ���������Ϣ */
#define EV_USP_OAM_DMP_AppResponse            (EV_USP_OAM_DMP_Begin + 6) /* ��ϲ���ģ�鷢�;���ҵ��Ӧ��ģ����ϲ���Ӧ����Ϣ*/
#define EV_USP_OAM_DMP_WaitAppResponse        (EV_USP_OAM_DMP_Begin + 7) /* ������ϲ���ģ��ȴ�ҵ��Ӧ��ģ��Ӧ��ʱ�����Ϣ */
#define EV_USP_OAM_DMP_CLOSEREQUEST           (EV_USP_OAM_DMP_Begin + 8) /* DMP������������Ϣ */
#define EV_USP_OAM_DMP_CLOSERESPONSE          (EV_USP_OAM_DMP_Begin + 9) /* DMP������Ӧ����Ϣ */
#define EV_USP_OAM_DMP_OPENREQUEST            (EV_USP_OAM_DMP_Begin +10) /* DMP������������Ϣ */
#define EV_USP_OAM_DMP_OPENRESPONSE           (EV_USP_OAM_DMP_Begin +11) /* DMP������Ӧ����Ϣ */

/**********************************************************************
*                            ״̬���(SMP)��Ϣ����                   *
**********************************************************************/
#define EV_USP_SMP_MonitorTimerId            (EV_USP_OSS_SMP_Begin + 1) /* ������ڶ�ʱ����Ϣ */
#define EV_USP_SMP_ConfigAckTimeId           (EV_USP_OSS_SMP_Begin + 2) /* �ȴ����ó�ʱ��Ϣ*/

/**********************************************************************
*                            �澯Դ����ǰ̨AMP�澯��Ϣ����                   *
**********************************************************************/
#define EV_USP_OAM_AMP_AlarmReport              (EV_USP_OAM_AMP_Begin + 1)//ǰ̨�澯ģ�鷢�͸���̨�ĸ澯��Ϣ
#define EV_USP_OAM_AMP_AlarmRestoreReport       (EV_USP_OAM_AMP_Begin + 2)//ǰ̨�澯ģ�鷢�͸���̨�ĸ澯�ָ���Ϣ
#define EV_USP_OAM_AMP_InformReport             (EV_USP_OAM_AMP_Begin + 3)//ǰ̨�澯ģ�鷢�͸���̨�ĸ澯֪ͨ��Ϣ
#define EV_USP_OAM_AMP_REPORT_ALL_ALARM         (EV_USP_OAM_AMP_Begin + 4)//���澯���е����и澯���͸���̨
#define EV_B_O_ALARM                            EV_USP_OAM_AMP_AlarmReport         /* �澯Դ�ϱ��澯 */
#define EV_B_O_ALARMRESTORE                     EV_USP_OAM_AMP_AlarmRestoreReport  /* �澯Դ�ϱ��澯�ָ� */
#define EV_B_O_INFORM                           EV_USP_OAM_AMP_InformReport        /* �澯Դ�ϱ��澯֪ͨ */

/**********************************************************************
*                            MPI ģ����Ϣ����                    *
**********************************************************************/
#define EV_USP_MPI_OPENRF                       (EV_USP_MPI_Begin + 1)/* ��̬����Э��ģ�����Ƶ*/
#define EV_USP_MPI_OPENRFAck                    (EV_USP_MPI_Begin + 2)/* Э��ģ����̬�����ƵӦ��*/
#define EV_USP_MPI_CLOSERF                      (EV_USP_MPI_Begin + 3)/* ��̬����Э��ģ��ر���Ƶ*/
#define EV_USP_MPI_CLOSERFAck                   (EV_USP_MPI_Begin + 4)/* Э��ģ����̬��ر���ƵӦ��*/
#define EV_USP_MPI_RB_OpenRf                    (EV_USP_MPI_Begin + 7)/* MPI Э���ڲ�����Ƶ*/
#define EV_USP_MPI_RB_OpenRfAck	                (EV_USP_MPI_Begin + 8)/* MPI Э���ڲ�����ƵӦ��*/
#define EV_USP_MPI_RB_CloseRf                   (EV_USP_MPI_Begin + 9)/* MPI Э���ڲ��ر���Ƶ*/
#define EV_USP_MPI_RB_CloseRfAck                (EV_USP_MPI_Begin +10)/* MPI Э���ڲ��ر���ƵӦ��*/
#define EV_USP_MPI_RF_CmpAckOverTime            (EV_USP_MPI_Begin +11)/* MPI ��Ƶ����������Ӧ��ʱ*/
#define EV_USP_MPI_RF_ResetCC1101Time           (EV_USP_MPI_Begin +12)/* MPI ��Ƶ�帴λCC1101 ��ʱ*/
#define EV_USP_MPI_RB_NoticeShellTno            (EV_USP_MPI_Begin +13)/* ��֪��Ƶ��ű������*/
#define EV_USP_MPI_RF_ReportObuInfo             (EV_USP_MPI_Begin +14)/* ��Ƶ���ϱ�OBU��Ϣ*/

/**********************************************************************
*                             ��չPSAM����������Ϣ����                *
**********************************************************************/
#define EV_USP_PSAM_EXT_Detect                  (EV_USP_PSAM_EXT_Begin + 1)  /* PSAM̽��������Ϣ */
#define EV_USP_PSAM_EXT_DetectAck               (EV_USP_PSAM_EXT_Begin + 2)  /* PSAM̽��Ӧ����Ϣ */
#define EV_USP_PSAM_EXT_Cmd                     (EV_USP_PSAM_EXT_Begin + 3)  /* PSAM�����������Ϣ */
#define EV_USP_PSAM_EXT_CmdAck                  (EV_USP_PSAM_EXT_Begin + 4)  /* PSAM�����Ӧ����Ϣ */
#define EV_USP_PSAM_EXT_Ec02Check               (EV_USP_PSAM_EXT_Begin + 5)  /* EC02��λ��ⶨʱ����Ϣ */
#define EV_USP_PSAM_EXT_Active                  (EV_USP_PSAM_EXT_Begin + 7)  /* PSAM���������������Ϣ */
#define EV_USP_PSAM_EXT_ActiveAck               (EV_USP_PSAM_EXT_Begin + 8)  /* PSAM���������Ӧ����Ϣ */

/**********************************************************************
*                            GPIO��RELAY����ģ����Ϣ��ֵ����          *
**********************************************************************/
#define EV_USP_GPIO_Config                     (EV_USP_GPIO_RELAY_Begin + 0)  /* GPIOģ������*/
#define EV_USP_GPIO_ConfigAck                  (EV_USP_GPIO_RELAY_Begin + 1)  /* GPIOģ������Ӧ��*/
#define EV_USP_GPIO_InterruptReport            (EV_USP_GPIO_RELAY_Begin + 2)  /* GPIOģ���ж��ϱ�*/
#define EV_USP_GPIO_IOStatusReq                (EV_USP_GPIO_RELAY_Begin + 3)  /* GPIOģ���ж��ϱ�*/
#define EV_USP_GPIO_IOStatusReport             (EV_USP_GPIO_RELAY_Begin + 4)  /* GPIOģ��״̬�ϱ� */
#define EV_USP_GPIO_SetOut                     (EV_USP_GPIO_RELAY_Begin + 5)  /* GPIOģ��״̬��� */
#define EV_USP_GPIO_SetOutAck                  (EV_USP_GPIO_RELAY_Begin + 6)  /* GPIOģ��״̬���Ӧ�� */
#define EV_USP_RELAY_SetOut                    (EV_USP_GPIO_RELAY_Begin + 7)  /* GPIOģ��̵������ */
#define EV_USP_RELAY_SetOutAck                 (EV_USP_GPIO_RELAY_Begin + 8)  /* GPIOģ��̵������Ӧ�� */
#define EV_USP_GPIO_ConfigReq                  (EV_USP_GPIO_RELAY_Begin + 9)  /* GPIOģ������������Ϣ*/
#define EV_USP_GPIO_SetGpiReport               (EV_USP_GPIO_RELAY_Begin + 10) /* GPIOģ��GPI״̬�������� */
#define EV_USP_GPIO_SetGpiReportAck            (EV_USP_GPIO_RELAY_Begin + 11) /* GPIOģ��GPI״̬��������Ӧ�� */
#define EV_USP_GPIO_TimerInitiative            (EV_USP_GPIO_RELAY_Begin + 12) /* �����ϱ���ʱ����Ϣ */


/**********************************************************************
*                             ����(MISC)��Ϣ����                      *
**********************************************************************/
#define EV_USP_OSS_MISC_BuzzerOn                (EV_USP_OSS_MISC_Begin + 1)  /* �򿪷����� */
#define EV_USP_OSS_MISC_BuzzerOff               (EV_USP_OSS_MISC_Begin + 2)  /* �رշ����� */
#define EV_USP_OSS_MISC_BuzzerInterval          (EV_USP_OSS_MISC_Begin + 3)  /* ������� */
#define EV_USP_OSS_MISC_SetWiegand              (EV_USP_OSS_MISC_Begin + 4)  /* ����Τ������ */
#define EV_USP_OSS_MISC_GetWiegand              (EV_USP_OSS_MISC_Begin + 5)  /* ��ȡΤ������ */
#define EV_USP_OSS_MISC_SendWiegandReport       (EV_USP_OSS_MISC_Begin + 6)  /* ͨ��Τ�����ͱ��� */
#define EV_USP_OSS_MISC_SwitchGpioAndWiegand    (EV_USP_OSS_MISC_Begin + 7)  /* Τ����GPIO���л� */

/***********************************************************************
*                �����ǩģ����Ϣ����
************************************************************************/
#define EV_USP_VIR_TAG_Config6cTag              (EV_USP_VIR_TAG_Begin + 1)  /*����6c��ǩ*/
#define EV_USP_VIR_TAG_Config6cTagResponse      (EV_USP_VIR_TAG_Begin + 2)  /* ����6c��ǩ������� */
#define EV_USP_VIR_TAG_Query6cTag               (EV_USP_VIR_TAG_Begin + 3)
#define EV_USP_VIR_TAG_Query6cTagResponse       (EV_USP_VIR_TAG_Begin + 4)
#define EV_USP_VIR_TAG_Config6bTag              (EV_USP_VIR_TAG_Begin + 5) 
#define EV_USP_VIR_TAG_Config6bTagResponse      (EV_USP_VIR_TAG_Begin + 6)
#define EV_USP_VIR_TAG_Query6bTag               (EV_USP_VIR_TAG_Begin + 7)
#define EV_USP_VIR_TAG_Query6bTagResponse       (EV_USP_VIR_TAG_Begin + 8)
#define EV_USP_VIR_TAG_OperateTag               (EV_USP_VIR_TAG_Begin + 9)
#define EV_USP_VIR_TAG_StartCmd                 (EV_USP_VIR_TAG_Begin + 10)
#define EV_USP_VIR_TAG_StartLlrpCmd             (EV_USP_VIR_TAG_Begin + 11)
#define EV_USP_VIR_TAG_StopCmd                  (EV_USP_VIR_TAG_Begin + 12)
#define EV_USP_VIR_TAG_Remove6cTagAll           (EV_USP_VIR_TAG_Begin + 13)
#define EV_USP_VIR_TAG_Remove6bTagAll           (EV_USP_VIR_TAG_Begin + 14)
#define EV_USP_VIR_TAG_StartWork                (EV_USP_VIR_TAG_Begin + 15)
#define EV_USP_VIR_TAG_Delete6cTag              (EV_USP_VIR_TAG_Begin + 16)
#define EV_USP_VIR_TAG_Delete6cTagResponse      (EV_USP_VIR_TAG_Begin + 17)
#define EV_USP_VIR_TAG_Delete6bTag              (EV_USP_VIR_TAG_Begin + 18)
#define EV_USP_VIR_TAG_Delete6bTagResponse      (EV_USP_VIR_TAG_Begin + 19)

#endif  /* _USP_EVENT_DEF_H */

