/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_errorDef.h
* �ļ���ʶ�� 
* ����ժҪ�� �����붨��
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2008.10.14
* 
* �޸ļ�¼1��
*    �޸����ڣ�2009.12.01
*    �� �� �ţ�V01.10.00
*    �� �� �ˣ��߷�
*    �޸����ݣ����ݴ����߲�����淶�����붨���ļ�
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_ERROR_DEF_H
#define _USP_ERROR_DEF_H


/***********************************************************
 *                     �������ֵ                          *
***********************************************************/
/* ״̬���ģ��(SMP)�������ֵ*/
#define  USP_ERR_OSS_SMP_BASE           (0xF0220000)

/* ����ģ��(PUB)�������ֵ */
#define  USP_ERR_PUB_BASE           (0xF0270000)

/* �����н���(CLI)�������ֵ */
#define  USP_ERR_OAM_CLI_BASE        (0xF0250000)

/* ϵͳ����(SCP)�������ֵ */
#define  USP_ERR_OSS_SCP_BASE        (0xF0210000)

/* ���ݴ洢ģ��(DSM)�������ֵ */
#define  USP_ERR_OSS_DSM_BASE        (0xF02E0000)

/* �ڴ����ģ��(MM)�������ֵ */
#define  USP_ERR_OSS_MM_BASE         (0xF02F0000)

/* �汾����(VMP)�������ֵ */
#define  USP_ERR_OAM_VMP_BASE        (0xF02A0000)

/* ��־���쳣̽��(LAP)�������ֵ */
#define  USP_ERR_OAM_LAP_BASE        (0xF0001000)

/* LLRP������(LTK)�������ֵ */
#define  USP_LTK_ERR_BASE        (0xF0002000)

/* �������ϵͳ(VOS)�������ֵ */
#define  USP_OSS_VOS_ERR_BASE    (0xF0003000)

/* Socketͨ��(NET)�������ֵ */
#define  USP_OSS_NET_ERR_BASE    (0xF0004000)

/* ��ʱ��(TIMER)�������ֵ */
#define  USP_OSS_TIMER_ERR_BASE  (0xF0005000)

/* ͨ��ģ��(COMM)�������ֵ */
#define  USP_COMM_ERR_BASE       (0xF0006000)

/* IP����ģ��(IPD)�������ֵ */
#define  USP_COMM_IPD_ERR_BASE   (0xF0011000)

/* ����ģ��(CMP)�������ֵ */
#define  USP_OAM_CMP_ERR_BASE    (0xF0012000)

/* ��ϲ���ģ��(DMP)�������ֵ */
#define  USP_OAM_DMP_ERR_BASE    (0xF02B0000)

/* �澯����ģ��(AMP)�������ֵ */
#define  USP_OAM_AMP_ERR_BASE    (0xF02D0000)

/* GPIO��Relayģ��(GPIO)�������ַ */
#define  USP_GPIO_ERR_BASE       (0xF0260000)

/*   FLASHϵͳ���������ʽӿ� �������ֵ    */
#define  USP_ERR_OSS_SYSDATA_START  (0xF0007000)

/* U-Boot �������ֵ*/
#define USP_ERR_UBOOT_BASE                 (0xFD000000)


/* MPI ģ��(MPI)�������ֵ */
#define  USP_MPI_ERR_BASE       (0xF0008000)

/* ��չPSAM��������������ֵ */
#define  USP_ERR_OSS_PSAM_EXT_BASE      (0xF0240000)

/* ��������������ֵ */
#define  USP_ERR_OSS_MISC_BASE          (0xF0270000)

/*   ����ƽ̨(Drv) �������ֵ    */

/*   ����ƽ̨Flashģ�� �������ֵ    */
#define USP_ERR_DRV_FLASH_START                                       (0xFF000000)
/*   ����ƽ̨��ӡ������ģ�� �������ֵ    */
#define USP_ERR_DRV_PRINTER_START                                     (0xFF001000)
/* ����ģ��������ֵ */
#define USP_ERR_DRV_UART_START                                        (0xFF002000)
/* ����������ģ��������ֵ */
#define USP_ERR_DRV_MCR_START                                         (0xFF003000)
/* 485����ģ��������ֵ */
#define USP_ERR_DRV_CTRL485_START                                     (0xFF004000)
/* �������ģ��������ֵ */
#define USP_ERR_DRV_CODEKBD_START                                     (0xFF005000)
/*   ����ƽ̨�¶ȴ�����ģ�� �������ֵ    */
#define USP_ERR_DRV_TSENSOR_START                                     (0xFF006000)
/*   ����ƽ̨Τ��ģ�� �������ֵ    */
#define USP_ERR_DRV_WG_START                                          (0xFF007000)
/* GPRSģ��������ֵ */
#define USP_ERR_DRV_GPRS_START                                        (0xFF008000)
/*   ����ƽ̨EEPROMģ�� �������ֵ    */
#define USP_ERR_DRV_EEPROM_START                                      (0xFF009000)
/*   ����ƽ̨I2Cģ�� �������ֵ    */
#define USP_ERR_DRV_I2C_START                                         (0xFF00A000)
/*   ����ƽ̨IOPORTģ�� �������ֵ    */
#define USP_ERR_DRV_IOPORT_START                                      (0xFF00B000)
/*   ����ƽ̨FPGAģ�� �������ֵ    */
#define USP_ERR_DRV_FPGA_START                                        (0xFF00C000)
/*   ����ƽ̨Epldģ�� �������ֵ    */
#define USP_ERR_DRV_EPLD_START                                        (0xFF00D000)
/*   ����ƽ̨Rtcģ�� �������ֵ    */
#define USP_ERR_DRV_RTC_START                                         (0xFF00E000)		
/*   ����ƽ̨Lcdģ�� �������ֵ    */	
#define USP_ERR_DRV_LCD_START                                         (0xFF00F000)		
/*   ����ƽ̨Modemģ�� �������ֵ    */
#define USP_ERR_DRV_MODEM_START                                       (0xFF010000)				
/*   ����ƽ̨Keyboardģ�� �������ֵ    */
#define USP_ERR_DRV_KEYBOARD_START                                    (0xFF020000)
/*   ����ƽ̨Wtdģ�� �������ֵ    */
#define USP_ERR_DRV_WTD_START                                         (0xFF030000)
/*   ����ƽ̨Ledģ�� �������ֵ    */
#define USP_ERR_DRV_LED_START                                         (0xFF040000)
/*   ����ƽ̨psamģ�� �������ֵ    */
#define USP_ERR_DRV_PSAM_START                                        (0xFF050000)
/*   ����ƽ̨gpioģ�� �������ֵ    */
#define USP_ERR_DRV_GPIO_START                                        (0xFF060000)
/*   ����ƽ̨ioexpanderģ�� �������ֵ    */
#define USP_ERR_DRV_IOE_START                                         (0xFF070000)
/*   ����ƽ̨timerģ�� �������ֵ    */
#define USP_ERR_DRV_TIMER_START                                       (0xFF080000)
/*   ����ƽ̨get system info ģ�� �������ֵ    */
#define USP_ERR_DRV_GET_SYSTEM_INFO_START                             (0xFF090000)
/*   ����ƽ̨clockgen  ģ�� �������ֵ    */
#define USP_ERR_DRV_CLOCKGEN_START                                    (0xFF0A0000)

/*   ����ƽ̨Variant SPI  ģ�� �������ֵ    */
#define USP_ERR_DRV_VARSPI_START                                      (0xFF0B0000)

/*   ����ƽ̨Relay  ģ�� �������ֵ    */
#define USP_ERR_DRV_RELAY_START                                      (0xFF0C0000)

/***********************************************************
*   ϵͳ���(SMP)������                                  *
***********************************************************/
#define USP_OSS_SMP_ERR_RECV_MSG                    ((WORD32) (USP_ERR_OSS_SMP_BASE +  1)) /* ������Ϣ���� */
#define USP_OSS_SMP_ERR_TIMERSET                    ((WORD32) (USP_ERR_OSS_SMP_BASE +  2)) /* ���ö�ʱ������*/
#define USP_OSS_SMP_ERR_UNKNOWN_MSG                 ((WORD32) (USP_ERR_OSS_SMP_BASE +  3)) /* ���յ�δ֪��Ϣ */      
#define USP_OSS_SMP_ERR_FUNC_EXECU                  ((WORD32) (USP_ERR_OSS_SMP_BASE +  4)) /* ��غ�������ʧ�� */     
#define USP_OSS_SMP_ERR_OPEN_FILE                   ((WORD32) (USP_ERR_OSS_SMP_BASE +  5)) /* ���ļ�ʧ�� */  
#define USP_OSS_SMP_ERR_MATCH_STR                   ((WORD32) (USP_ERR_OSS_SMP_BASE +  6)) /* CLI��������� */  
#define USP_OSS_SMP_ERR_SSCANF                      ((WORD32) (USP_ERR_OSS_SMP_BASE +  7)) /* CLI�������� */  
#define USP_OSS_SMP_ERR_GET_CPU_STAT                ((WORD32) (USP_ERR_OSS_SMP_BASE +  8)) /* ��ȡCPU״̬���� */  
#define USP_OSS_SMP_ERR_GET_SYSINFO                 ((WORD32) (USP_ERR_OSS_SMP_BASE +  9)) /* ��ȡϵͳ״̬���� */  
#define USP_OSS_SMP_ERR_FUNC_STATFS                 ((WORD32) (USP_ERR_OSS_SMP_BASE + 10)) /* ����statfs ���ش��� */  
#define USP_OSS_SMP_ERR_MSGCTL                      ((WORD32) (USP_ERR_OSS_SMP_BASE + 11)) /* ����msgctl ���ش��� */  
#define USP_OSS_SMP_ERR_NULL_POINTER                ((WORD32) (USP_ERR_OSS_SMP_BASE + 12)) /* ��ָ�� */
#define USP_OSS_SMP_ERR_MSG_LEN                     ((WORD32) (USP_ERR_OSS_SMP_BASE + 13)) /* ��Ϣ���ȴ��� */
#define USP_OSS_SMP_ERR_NOT_FOUND_ITEM              ((WORD32) (USP_ERR_OSS_SMP_BASE + 14)) /* �Ҳ�������� */
/***********************************************************
*   �����н���(CLI)������                                  *
***********************************************************/
#define USP_ERR_OAM_CLI_SOCKET_ACCEPT            ((WORD32) (USP_ERR_OAM_CLI_BASE + 1))  /* �����û����Ӵ��� */
#define USP_ERR_OAM_CLI_NO_RESOURCE              ((WORD32) (USP_ERR_OAM_CLI_BASE + 2))  /* û�п��õ���Դ */   
#define USP_ERR_OAM_CLI_NULL_POINTER             ((WORD32) (USP_ERR_OAM_CLI_BASE + 3))  /* ��ָ�� */   
#define USP_ERR_OAM_CLI_SELECT                   ((WORD32) (USP_ERR_OAM_CLI_BASE + 4))  /* select �������� */   
#define USP_ERR_OAM_CLI_WRITE_SOCKET             ((WORD32) (USP_ERR_OAM_CLI_BASE + 5))  /* ��ͻ���д���ݴ��� */   
#define USP_ERR_OAM_CLI_CA                       ((WORD32) (USP_ERR_OAM_CLI_BASE + 6))  /* �û�Ȩ����֤û��ͨ�� */   
#define USP_ERR_OAM_CLI_OPEN_PASSWD_FILE         ((WORD32) (USP_ERR_OAM_CLI_BASE + 7))  /* �������ļ����� */
#define USP_ERR_OAM_CLI_MATCH_QUOTATION_MARK     ((WORD32) (USP_ERR_OAM_CLI_BASE + 8))  /* ���Ų�ƥ�� */   
#define USP_ERR_OAM_CLI_PARAM_TOO_MANY           ((WORD32) (USP_ERR_OAM_CLI_BASE + 9))  /* �������� */   
#define USP_ERR_OAM_CLI_READ_PASSWD_FILE         ((WORD32) (USP_ERR_OAM_CLI_BASE + 10)) /* �������ļ����� */   
#define USP_ERR_OAM_CLI_NON_DIGATAL              ((WORD32) (USP_ERR_OAM_CLI_BASE + 11)) /* ������ */   
#define USP_ERR_OAM_CLI_NUMBER_OVERFLOW          ((WORD32) (USP_ERR_OAM_CLI_BASE + 12)) /* ��ֵ��� */  
#define USP_ERR_OAM_CLI_INIT_PRINTF_SHM          ((WORD32) (USP_ERR_OAM_CLI_BASE + 13)) /* ��ʼ����ӡ���ƹ����ڴ���� */  
#define USP_ERR_OAM_CLI_INIT_PRINTF_PIPE         ((WORD32) (USP_ERR_OAM_CLI_BASE + 14)) /* ��ʼ����ӡ��Ϣ���ݹܵ����� */  
#define USP_ERR_OAM_CLI_CMD_CHILD_SET_OVERFLOW   ((WORD32) (USP_ERR_OAM_CLI_BASE + 15)) /* ���õ������������� */  
#define USP_ERR_OAM_CLI_CMD_NODE_KEY_OVERFLOW    ((WORD32) (USP_ERR_OAM_CLI_BASE + 16)) /* ����ڵ������� */  
#define USP_ERR_OAM_CLI_CMD_PARAM_INDEX          ((WORD32) (USP_ERR_OAM_CLI_BASE + 17)) /* ��������������� */  
#define USP_ERR_OAM_CLI_INIT_INNER_PIPE          ((WORD32) (USP_ERR_OAM_CLI_BASE + 18)) /* ��ʼ��CLI�ڲ���Ϣ���ݹܵ����� */  
#define USP_ERR_OAM_CLI_BIND_SOCKET              ((WORD32) (USP_ERR_OAM_CLI_BASE + 19)) /* ��socket���� */  
#define USP_ERR_OAM_CLI_LISTEN_SOCKET            ((WORD32) (USP_ERR_OAM_CLI_BASE + 20)) /* ���ü���socket���� */  
#define USP_ERR_OAM_CLI_CREATE_SOCKET            ((WORD32) (USP_ERR_OAM_CLI_BASE + 21)) /* ����socket���� */  
#define USP_ERR_OAM_CLI_INIT_ST_SHM              ((WORD32) (USP_ERR_OAM_CLI_BASE + 22)) /* ��ʼ��������ٹ����ڴ���� */  
#define USP_ERR_OAM_CLI_SEND_DATA                ((WORD32) (USP_ERR_OAM_CLI_BASE + 23)) /* ͨ��Socket��������ʧ�� */  
#define USP_ERR_OAM_CLI_TIMEOUT_SEND             ((WORD32) (USP_ERR_OAM_CLI_BASE + 24)) /* ͨ��Socket�������ݳ�ʱ */  
#define USP_ERR_OAM_CLI_INIT_ST_PIPE             ((WORD32) (USP_ERR_OAM_CLI_BASE + 25)) /* ��ʼ�����������Ϣ���ݹܵ����� */  
#define USP_ERR_OAM_CLI_INIT_ST_BUF              ((WORD32) (USP_ERR_OAM_CLI_BASE + 26)) /* ��ʼ�����������Ϣ�洢������� */  
#define USP_ERR_OAM_CLI_LLRP_DECODE              ((WORD32) (USP_ERR_OAM_CLI_BASE + 27)) /* LLRP������� */
#define USP_ERR_OAM_CLI_LLRP_ENCODE              ((WORD32) (USP_ERR_OAM_CLI_BASE + 28)) /* LLRP������� */
#define USP_ERR_OAM_CLI_LLRP_UNKNOW_MSG          ((WORD32) (USP_ERR_OAM_CLI_BASE + 29)) /* δ֪��LLRP��Ϣ */
#define USP_ERR_OAM_CLI_LLRP_PARAMETER           ((WORD32) (USP_ERR_OAM_CLI_BASE + 30)) /* LLRP�������� */
#define USP_ERR_OAM_CLI_UNKNOW_MSG               ((WORD32) (USP_ERR_OAM_CLI_BASE + 31)) /* ��Ϣ�Ŵ��� */
#define USP_ERR_OAM_CLI_INIT_PRINTF_BUF          ((WORD32) (USP_ERR_OAM_CLI_BASE + 32)) /* ��ʼ����ӡ������Ϣ�洢������� */  
#define USP_ERR_OAM_CLI_MALLOC_HANDLE            ((WORD32) (USP_ERR_OAM_CLI_BASE + 33)) /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_ERR_OAM_CLI_ATTACH_PRINTF            ((WORD32) (USP_ERR_OAM_CLI_BASE + 34)) /* ���ӵ���ӡ���ƹ����ڴ�ʧ�� */

/***********************************************************
*   ��־���쳣̽��(LAP)������                              *
***********************************************************/
#define USP_ERR_OAM_LAP_OPEN_FILE           ((WORD32) (USP_ERR_OAM_LAP_BASE +  1)) /* ���ļ�ʧ�� */
#define USP_ERR_OAM_LAP_READ_FILE           ((WORD32) (USP_ERR_OAM_LAP_BASE +  2)) /* ���ļ�ʧ�� */
#define USP_ERR_OAM_LAP_WRITE_FILE          ((WORD32) (USP_ERR_OAM_LAP_BASE +  3)) /* д�ļ�ʧ�� */
#define USP_ERR_OAM_LAP_FSEEK_FILE          ((WORD32) (USP_ERR_OAM_LAP_BASE +  4)) /* ��λ�ļ�ʧ�� */
#define USP_ERR_OAM_LAP_FTP_SEND            ((WORD32) (USP_ERR_OAM_LAP_BASE +  5)) /* ��ftp��������������ʧ�� */
#define USP_ERR_OAM_LAP_FTP_CREATE          ((WORD32) (USP_ERR_OAM_LAP_BASE +  6)) /* ��ftp����������ʧ�� */
#define USP_ERR_OAM_LAP_IN_MSG_ADAPTER      ((WORD32) (USP_ERR_OAM_LAP_BASE +  7)) /* LLRP��ʽ������� */
#define USP_ERR_OAM_LAP_OUT_MSG_ADAPTER     ((WORD32) (USP_ERR_OAM_LAP_BASE +  8)) /* LLRP��ʽ������� */
#define USP_ERR_OAM_LAP_NO_DATA             ((WORD32) (USP_ERR_OAM_LAP_BASE +  9)) /* �ļ������ڣ���������û������ */
#define USP_ERR_OAM_LAP_INVALID_MSG_LEN     ((WORD32) (USP_ERR_OAM_LAP_BASE + 10)) /* ��Ϣ���Ȳ�ƥ�� */
#define USP_ERR_OAM_LAP_INVALID_MSG         ((WORD32) (USP_ERR_OAM_LAP_BASE + 11)) /* ��Ч����Ϣ*/
#define USP_ERR_OAM_LAP_SEND_MSG            ((WORD32) (USP_ERR_OAM_LAP_BASE + 12)) /* ������Ϣʧ�� */
#define USP_ERR_OAM_LAP_RECV_MSG            ((WORD32) (USP_ERR_OAM_LAP_BASE + 13)) /* ������Ϣʧ�� */
#define USP_ERR_OAM_LAP_PARA_NULL           ((WORD32) (USP_ERR_OAM_LAP_BASE + 14)) /* ����Ϊ��ָ�� */
#define USP_ERR_OAM_LAP_MALLOC              ((WORD32) (USP_ERR_OAM_LAP_BASE + 15)) /* �����ڴ�ʧ�� */
#define USP_ERR_OAM_LAP_BUFF_LACK           ((WORD32) (USP_ERR_OAM_LAP_BASE + 16)) /* ����ռ䲻�� */
#define USP_ERR_OAM_LAP_FILE_LEN            ((WORD32) (USP_ERR_OAM_LAP_BASE + 17)) /* �ļ����ȷǷ� */
#define USP_ERR_OAM_LAP_SQNO_INVALID        ((WORD32) (USP_ERR_OAM_LAP_BASE + 18)) /* ���кŷǷ� */
#define USP_ERR_OAM_LAP_SQNO_POS            ((WORD32) (USP_ERR_OAM_LAP_BASE + 19)) /* ���и����кŵ���Ŀ���ļ��е�λ�÷Ƿ� */
#define USP_ERR_OAM_LAP_FILE_DESC           ((WORD32) (USP_ERR_OAM_LAP_BASE + 20)) /* �ļ������Ƿ� */

/***********************************************************
*   LLRP������(LTK)������                                  *
***********************************************************/
#define USP_LTK_ERR_FRAMEENCODER_CONSTRUCTOR    ((WORD32) (USP_LTK_ERR_BASE + 1)) /* ֡�������������� */
#define USP_LTK_ERR_FRAMEENCODE                 ((WORD32) (USP_LTK_ERR_BASE + 2)) /* ֡������� */
#define USP_LTK_ERR_XMLTEXTENCODER_CONSTRUCTOR  ((WORD32) (USP_LTK_ERR_BASE + 3)) /* xml�ַ����������������� */
#define USP_LTK_ERR_XMLTEXTENCODE               ((WORD32) (USP_LTK_ERR_BASE + 4)) /* xml�ַ���������� */
#define USP_LTK_ERR_BUF_OVERFLOW                ((WORD32) (USP_LTK_ERR_BASE + 5)) /* ������������� */
#define USP_LTK_ERR_MALLOC_HANDLE               ((WORD32) (USP_LTK_ERR_BASE + 6)) /* ��ȡ��̬�����ڴ�ʧ�� */

/***********************************************************
*   �������ϵͳ(VOS)������                                *
***********************************************************/
#define USP_OSS_VOS_ERR_CONFIG_COREDUMP             ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x01)) /* ����core dump���� */     
#define USP_OSS_VOS_ERR_SET_EHTERNET_IP             ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x02)) /* ������̫��IP���� */
#define USP_OSS_VOS_ERR_SET_EHTERNET_NETMASK        ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x03)) /* ������̫������������� */
#define USP_OSS_VOS_ERR_SET_EHTERNET_GATEWAY        ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x04)) /* ������̫�����ش��� */
#define USP_OSS_VOS_ERR_DELETE_ROUTE                ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x05)) /* ɾ��·�ɴ��� */     
#define USP_OSS_VOS_ERR_INVALID_TIMEZONE            ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x06)) /* �Ƿ���ʱ��ֵ */     
#define USP_OSS_VOS_ERR_SET_TIMEZONE                ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x07)) /* ����ʱ������ */     
#define USP_OSS_VOS_ERR_UPDATE_NTP                  ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x08)) /* ntpʱ��ͬ������ */     
#define USP_OSS_VOS_ERR_CARETE_MQ                   ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x09)) /* ������Ϣ���д��� */     
#define USP_OSS_VOS_ERR_CARETE_PROCESS              ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0A)) /* �������̴��� */     
#define USP_OSS_VOS_ERR_CARETE_THREAD               ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0B)) /* �����̴߳��� */     
#define USP_OSS_VOS_ERR_SEND_MSG                    ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0C)) /* ������Ϣ���� */     
#define USP_OSS_VOS_ERR_RECV_MSG                    ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0D)) /* ������Ϣ���� */     
#define USP_OSS_VOS_ERR_CARETE_MUTEX                ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0E)) /* �������������� */     
#define USP_OSS_VOS_ERR_INIT_MUTEX                  ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x0F)) /* ��ʼ������������ */     
#define USP_OSS_VOS_ERR_MUTEX_INVALID               ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x10)) /* �Ƿ��Ļ����� */ 
#define USP_OSS_VOS_ERR_OPEN_FILE                   ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x11)) /* ���ļ����� */ 
#define USP_OSS_VOS_ERR_WRITE_FILE                  ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x12)) /* д�ļ����� */ 
#define USP_OSS_VOS_ERR_NULL_POINTER                ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x13)) /* ��ָ�� */
#define USP_OSS_VOS_ERR_OPEN_DIR                    ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x14)) /* ���ļ��д��� */ 
#define USP_OSS_VOS_ERR_PROCESS_NOT_FIND            ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x15)) /* û���ҵ���Ӧ�Ľ��� */     
#define USP_OSS_VOS_ERR_DTTACH_SHM                  ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x16)) /* �Ͽ��������ڴ���������Ӵ��� */
#define USP_OSS_VOS_ERR_SYNC_FILE                   ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x17)) /* ͬ���ļ���flash���� */ 
#define USP_OSS_VOS_ERR_RENAME_FILE                 ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x18)) /* �������ļ����� */ 
#define USP_OSS_VOS_ERR_GET_MUTEX                   ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x19)) /* ��ȡ���������� */
#define USP_OSS_VOS_ERR_DESTROY_THREAD              ((WORD32) (USP_OSS_VOS_ERR_BASE + 0x1A)) /* �����߳�ʧ�� */

/***********************************************************
*   Socketͨ��ģ��(NET)������                              *
***********************************************************/
#define USP_OSS_NET_ERR_IPADDR                      ((WORD32) (USP_OSS_NET_ERR_BASE + 1)) /* IP��ַ����ȷ���� */
#define USP_OSS_NET_ERR_PROTOCOL_TYPE               ((WORD32) (USP_OSS_NET_ERR_BASE + 2)) /* Э�����ʹ��� */
#define USP_OSS_NET_ERR_CREATE_SOCKET               ((WORD32) (USP_OSS_NET_ERR_BASE + 3)) /* ����Socket�ӿڴ��� */
#define USP_OSS_NET_ERR_BIND                        ((WORD32) (USP_OSS_NET_ERR_BASE + 4)) /* bind���� */
#define USP_OSS_NET_ERR_NULL_POINTER                ((WORD32) (USP_OSS_NET_ERR_BASE + 5)) /* ��ָ�� */
#define USP_OSS_NET_ERR_CONNECT                     ((WORD32) (USP_OSS_NET_ERR_BASE + 6)) /* ����Socket����ʧ�� */
#define USP_OSS_NET_ERR_ACCEPT                      ((WORD32) (USP_OSS_NET_ERR_BASE + 7)) /* accept���� */
#define USP_OSS_NET_ERR_RECVFROM                    ((WORD32) (USP_OSS_NET_ERR_BASE + 8)) /* recvfrom���� */
#define USP_OSS_NET_ERR_SENDTO                      ((WORD32) (USP_OSS_NET_ERR_BASE + 9)) /* sendto���� */
#define USP_OSS_NET_ERR_INVALID_SOCKET_OPT          ((WORD32) (USP_OSS_NET_ERR_BASE + 10)) /* �Ƿ���Socketѡ�� */
#define USP_OSS_NET_ERR_SEND                        ((WORD32) (USP_OSS_NET_ERR_BASE + 11)) /* send���� */
#define USP_OSS_NET_ERR_RECV                        ((WORD32) (USP_OSS_NET_ERR_BASE + 12)) /* recv���� */
#define USP_OSS_NET_ERR_TIMEOUT_SEND                ((WORD32) (USP_OSS_NET_ERR_BASE + 13)) /* send��ʱ */
#define USP_OSS_NET_ERR_INVALID_LINK_TYPE           ((WORD32) (USP_OSS_NET_ERR_BASE + 14)) /* �Ƿ�����·�������� */
#define USP_OSS_NET_ERR_BIND_DEVICE                 ((WORD32) (USP_OSS_NET_ERR_BASE + 15)) /* �������豸�ӿڴ��� */
#define USP_OSS_NET_ERR_IPADDR_FORMAT               ((WORD32) (USP_OSS_NET_ERR_BASE + 16)) /* IP��ַ�ַ�����ʽ���� */
#define USP_OSS_NET_ERR_GET_SOCK_INFO               ((WORD32) (USP_OSS_NET_ERR_BASE + 17)) /* ��ȡSocket������Ϣ���� */

/***********************************************************
*   ��ʱ��(TIMER)������                                    *
***********************************************************/
#define USP_OSS_TIMER_ALLOT_BUF_FAIL                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 1))  /*û�п��еĶ�ʱ�����Է���*/  
#define USP_OSS_TIMER_BUF_ALLOTED                   ((WORD32) (USP_OSS_TIMER_ERR_BASE + 2))  /*�ö�ʱ���Ѿ�����*/  
#define USP_OSS_TIMER_BUF_CTRL_ERROR                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 3))  /*������ʱ������*/  
#define USP_OSS_TIMER_P_ERROR                       ((WORD32) (USP_OSS_TIMER_ERR_BASE + 4))  /* P����ʧ�� */   
#define USP_OSS_TIMER_BUF_INDEDX_ERROR              ((WORD32) (USP_OSS_TIMER_ERR_BASE + 5))  /*������������*/   
#define USP_OSS_TIMER_NOT_EXIST                     ((WORD32) (USP_OSS_TIMER_ERR_BASE + 6))  /*��Ӧ��ʱ��������*/   
#define USP_OSS_TIMER_GET_POINTER_FAIL              ((WORD32) (USP_OSS_TIMER_ERR_BASE + 7))  /*�ڻ�ȡȫ��ָ���ʱ�����*/   
#define USP_OSS_TIMER_ALREADY_EXISTS                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 8))  /*�Ѿ������˸ö�ʱ��*/   
#define USP_OSS_TIMER_FUNC_PARAM_PTR_NULL           ((WORD32) (USP_OSS_TIMER_ERR_BASE + 9))  /* ��������Ϊ��ָ�� */
#define USP_OSS_TIMER_CREATE_SHM_ERROR              ((WORD32) (USP_OSS_TIMER_ERR_BASE + 10)) /* ���������ڴ������� */
#define USP_OSS_TIMER_GET_SHM_ERROR                 ((WORD32) (USP_OSS_TIMER_ERR_BASE + 11)) /* ��ù����ڴ������� */
#define USP_OSS_TIMER_CTRL_SHM_ERROR                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 12)) /* ���������ڴ������� */
#define USP_OSS_TIMER_CTRL_SEM_ERROR                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 13)) /* �����ź������� */
#define USP_OSS_TIMER_CREATE_SEM_ERROR              ((WORD32) (USP_OSS_TIMER_ERR_BASE + 14)) /* �����ź������� */
#define USP_OSS_TIMER_GET_SEM_ERROR                 ((WORD32) (USP_OSS_TIMER_ERR_BASE + 15)) /* ��ȡ�ź������� */
#define USP_OSS_TIMER_SET_TIMER_ERROR               ((WORD32) (USP_OSS_TIMER_ERR_BASE + 16)) /* ���ö�ʱ������ */
#define USP_OSS_TIMER_SEM_TAKE_ERROR                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 17)) /* ��ȡ�ź������� */
#define USP_OSS_TIMER_V_ERROR                       ((WORD32) (USP_OSS_TIMER_ERR_BASE + 18)) /* V����ʧ�� */
#define USP_OSS_TIMER_SEM_GIVE_ERROR                ((WORD32) (USP_OSS_TIMER_ERR_BASE + 19)) /* �ͷ��ź������� */ 
#define USP_OSS_TIMER_SHMAT_ERROR                   ((WORD32) (USP_OSS_TIMER_ERR_BASE + 20)) /* ���ӹ����ڴ�ʧ�� */
#define USP_OSS_TIMER_MALLOC_ERROR                  ((WORD32) (USP_OSS_TIMER_ERR_BASE + 21)) /* ��̬�����ڴ�ʧ�� */
#define USP_OSS_TIMER_SENDMSG_ERROR                 ((WORD32) (USP_OSS_TIMER_ERR_BASE + 22)) /* ������Ϣʧ�� */

/***********************************************************
*   ͨ��ģ��(COMM)������                                   *
***********************************************************/
#define USP_COMM_ERR_RECV_MSG                   ((WORD32) (USP_COMM_ERR_BASE + 1)) /* ������Ϣ���� */
#define USP_COMM_ERR_UNKNOW_MSG                 ((WORD32) (USP_COMM_ERR_BASE + 2)) /* δ֪����Ϣ */
#define USP_COMM_ERR_CREATE_LINK_CTRL_TASK      ((WORD32) (USP_COMM_ERR_BASE + 3)) /* ������·�����̴߳��� */
#define USP_COMM_ERR_CREATE_LINK_CTRL_SOCKET    ((WORD32) (USP_COMM_ERR_BASE + 4)) /* ������·�����̼߳���UDP/TCP�˿ڴ��� */
#define USP_COMM_ERR_CONNECT                    ((WORD32) (USP_COMM_ERR_BASE + 5)) /* ����Socket���Ӵ��� */
#define USP_COMM_ERR_SELECT                     ((WORD32) (USP_COMM_ERR_BASE + 6)) /* select���� */
#define USP_COMM_ERR_ACCEPT                     ((WORD32) (USP_COMM_ERR_BASE + 7)) /* accept���� */
#define USP_COMM_ERR_READ_LINK_CTRL_MSG         ((WORD32) (USP_COMM_ERR_BASE + 8)) /* ��ȡ��·�����߳̿�����Ϣ���� */
#define USP_COMM_ERR_CREATE_SOCKET_LISTEN       ((WORD32) (USP_COMM_ERR_BASE + 9)) /* ����Socket�����˿ڴ��� */
#define USP_COMM_ERR_FULL_LINK_CTRL             ((WORD32) (USP_COMM_ERR_BASE + 10)) /* ��·�����߳̿��ƽṹ���� */
#define USP_COMM_ERR_CONFIG_INFO_OVERLOAD       ((WORD32) (USP_COMM_ERR_BASE + 11)) /* ͨ��������Ϣ���� */
#define USP_COMM_ERR_OPEN_LINK                  ((WORD32) (USP_COMM_ERR_BASE + 12)) /* �ص�����·�������� */
#define USP_COMM_ERR_FULL_LINK_ID_CTRL          ((WORD32) (USP_COMM_ERR_BASE + 13)) /* ��·���ƽṹ���� */
#define USP_COMM_ERR_INVALID_LINK_STATUS        ((WORD32) (USP_COMM_ERR_BASE + 14)) /* �Ƿ�����·״̬ */
#define USP_COMM_ERR_INVALID_LINK_TYPE          ((WORD32) (USP_COMM_ERR_BASE + 15)) /* �Ƿ�����·�������� */
#define USP_COMM_ERR_INVALID_LINK_ID            ((WORD32) (USP_COMM_ERR_BASE + 16)) /* �Ƿ�����·��ʶ�� */
#define USP_COMM_ERR_CLOSE_LINK                 ((WORD32) (USP_COMM_ERR_BASE + 17)) /* ��·�رմ��� */
#define USP_COMM_ERR_FULL_LLRP_LINK_CTRL        ((WORD32) (USP_COMM_ERR_BASE + 18)) /* llrp��·���ƽṹ���� */
#define USP_COMM_ERR_INVALID_APP_TYPE           ((WORD32) (USP_COMM_ERR_BASE + 19)) /* �Ƿ�����·Ӧ������ */
#define USP_COMM_ERR_NULL_POINTER               ((WORD32) (USP_COMM_ERR_BASE + 20)) /* ��ָ�� */   
#define USP_COMM_ERR_INVALID_LINK               ((WORD32) (USP_COMM_ERR_BASE + 21)) /* �Ƿ�����· */
#define USP_COMM_ERR_SEND_CONN_ATTEMPT_EVENT    ((WORD32) (USP_COMM_ERR_BASE + 22)) /* ����ConnectionAttemptEventʧ�� */
#define USP_COMM_ERR_SEND_LLRP_KEEPLIVE         ((WORD32) (USP_COMM_ERR_BASE + 23)) /* ����KeepLive��Ϣʧ�� */
#define USP_COMM_ERR_LLRP_MSG_OVERLOAD          ((WORD32) (USP_COMM_ERR_BASE + 24)) /* ��������ʱ����llrp��Ϣ�ĳ��ȳ�����Ԥ��Ļ����� */
#define USP_COMM_ERR_LLRP_MSG_LEN               ((WORD32) (USP_COMM_ERR_BASE + 25)) /* llrp��Ϣ���ȴ��� */
#define USP_COMM_ERR_LLRP_ENCODE_SERIAL_FRAME   ((WORD32) (USP_COMM_ERR_BASE + 26)) /* ����llrp��������֡���� */
#define USP_COMM_ERR_LLRP_UNKNOW_MSG            ((WORD32) (USP_COMM_ERR_BASE + 27)) /* δ֪��llrp��Ϣ */
#define USP_COMM_ERR_LLRP_DECODE_SERIAL_FRAME   ((WORD32) (USP_COMM_ERR_BASE + 28)) /* ����llrp��������֡���� */
#define USP_COMM_ERR_LLRP_KEEPLIVE              ((WORD32) (USP_COMM_ERR_BASE + 29)) /* llrp��·����ʧ�� */
#define USP_COMM_ERR_UNKNOW_CLI_CMD             ((WORD32) (USP_COMM_ERR_BASE + 30)) /* δ֪��Cli���� */
#define USP_COMM_ERR_LLRP_RECV                  ((WORD32) (USP_COMM_ERR_BASE + 31)) /* llrp��·������Ϣʧ�� */
#define USP_COMM_ERR_LLRP_DECODE_MSG            ((WORD32) (USP_COMM_ERR_BASE + 32)) /* ����llrp��Ϣʧ�� */
#define USP_COMM_ERR_LLRP_MSG                   ((WORD32) (USP_COMM_ERR_BASE + 33)) /* llrp��Ϣ���ݴ��� */
#define USP_COMM_ERR_FULL_CONFIG                ((WORD32) (USP_COMM_ERR_BASE + 34)) /* ͨ��������Ϣ�ռ����� */
#define USP_COMM_ERR_LLRP_CONN_EVENT_TIMEOUT    ((WORD32) (USP_COMM_ERR_BASE + 35)) /* �ڹ涨��ʱ����û���յ��ɹ���ConnectionAttemptEvent�¼� */
#define USP_COMM_ERR_CREATE_MQ                  ((WORD32) (USP_COMM_ERR_BASE + 36)) /* ������Ϣ���д��� */
#define USP_COMM_ERR_START_COMM                 ((WORD32) (USP_COMM_ERR_BASE + 37)) /* ����ͨ��ģ����� */
#define USP_COMM_ERR_LINK_NOTICE_TYPE           ((WORD32) (USP_COMM_ERR_BASE + 38)) /* ��·֪ͨ��Ϣ���ʹ��� */
#define USP_COMM_ERR_OPEN_SERIAL_PORT           ((WORD32) (USP_COMM_ERR_BASE + 39)) /* �򿪴��ڴ��� */
#define USP_COMM_ERR_SET_ATTR_SERIAL_PORT       ((WORD32) (USP_COMM_ERR_BASE + 40)) /* ���ô������Դ��� */
#define USP_COMM_ERR_CREATE_CONN_CTRL_TASK      ((WORD32) (USP_COMM_ERR_BASE + 41)) /* �������罨���̴߳��� */
#define USP_COMM_ERR_FULL_CONN_CTRL             ((WORD32) (USP_COMM_ERR_BASE + 42)) /* ���������߳̿��ƽṹ���� */
#define USP_COMM_ERR_CREATE_COMM_TASK           ((WORD32) (USP_COMM_ERR_BASE + 43)) /* ����ͨ��ģ��������ʧ�� */
#define USP_COMM_ERR_DECODE_CRC_NOT_MATCH       ((WORD32) (USP_COMM_ERR_BASE + 44)) /* ���봮������֡ʱCRCУ�鲻ƥ�� */
#define USP_COMM_ERR_DECODE_OVERLOAD            ((WORD32) (USP_COMM_ERR_BASE + 45)) /* ���봮������֡ʱ����֡���ȳ�����Ԥ��Ļ����� */
#define USP_COMM_ERR_DECODE_FRAME_LENGTH        ((WORD32) (USP_COMM_ERR_BASE + 46)) /* ��������֡���ȴ��� */
#define USP_COMM_ERR_DECODE_FRAME_FORMAT        ((WORD32) (USP_COMM_ERR_BASE + 47)) /* ��������֡��ʽ���� */
#define USP_COMM_ERR_TIMEOUT_SEND               ((WORD32) (USP_COMM_ERR_BASE + 48)) /* �������ݳ�ʱ */
#define USP_COMM_ERR_CREATE_CONN_CTRL_MQ        ((WORD32) (USP_COMM_ERR_BASE + 49)) /* �������������߳���Ϣ���д��� */
#define USP_COMM_ERR_ETC_FULL_LINK_CTRL         ((WORD32) (USP_COMM_ERR_BASE + 50)) /* etc��·���ƽṹ���� */
#define USP_COMM_ERR_ETC_ENCODE_FRAME           ((WORD32) (USP_COMM_ERR_BASE + 51)) /* ����etc����֡���� */
#define USP_COMM_ERR_DECODE_BCC_NOT_MATCH       ((WORD32) (USP_COMM_ERR_BASE + 52)) /* ����etc����֡ʱBCCУ�鲻ƥ�� */
#define USP_COMM_ERR_ETC_DECODE_FRAME           ((WORD32) (USP_COMM_ERR_BASE + 53)) /* ����etc����֡���� */
#define USP_COMM_ERR_ETC_UNKNOW_CMDTYPE         ((WORD32) (USP_COMM_ERR_BASE + 54)) /* δ֪��etc�������� */
#define USP_COMM_ERR_MALLOC_CONGESTION_BUF      ((WORD32) (USP_COMM_ERR_BASE + 55)) /* ��ȡӵ�����ƻ���ʧ�� */
#define USP_COMM_ERR_FULL_CONGESTION_BUF        ((WORD32) (USP_COMM_ERR_BASE + 56)) /* ӵ���������� */
#define USP_COMM_ERR_INVALID_PARAM              ((WORD32) (USP_COMM_ERR_BASE + 57)) /* ������������ */
#define USP_COMM_ERR_FULL_SEND_BUF              ((WORD32) (USP_COMM_ERR_BASE + 58)) /* ͨ����·�ķ��ͻ��������� */
#define USP_COMM_ERR_INSERT_HOLD_STORAGE        ((WORD32) (USP_COMM_ERR_BASE + 59)) /* �������ݵ���������洢����ʧ�� */
#define USP_COMM_ERR_GET_HOLD_STORAGE           ((WORD32) (USP_COMM_ERR_BASE + 60)) /* ��ȡ������������ʧ�� */
#define USP_COMM_ERR_DELETE_HOLD_STORAGE        ((WORD32) (USP_COMM_ERR_BASE + 61)) /* ɾ��������������ʧ�� */
#define USP_COMM_ERR_TRUNCATE_HOLD_STORAGE      ((WORD32) (USP_COMM_ERR_BASE + 62)) /* ��ն�����������ʧ�� */
#define USP_COMM_ERR_HOLD_RECV_UNKNOW_DATA      ((WORD32) (USP_COMM_ERR_BASE + 63)) /* �������洦�������յ�δ֪�������� */
#define USP_COMM_ERR_HOLD_SEQUENCE_NOT_MATCH    ((WORD32) (USP_COMM_ERR_BASE + 64)) /* �������洦�������յ����кŲ�ƥ���Ӧ����Ϣ */
#define USP_COMM_ERR_HOLD_SEND_UNKNOW_DATA      ((WORD32) (USP_COMM_ERR_BASE + 65)) /* �������洦��������δ֪�������� */
#define USP_COMM_ERR_ZTE_MSG_MAGIC_NOT_MATCH    ((WORD32) (USP_COMM_ERR_BASE + 66)) /* ��������ʱ����zte˽����Ϣ��ħ���ֲ�ƥ�� */
#define USP_COMM_ERR_ZTE_MSG_OVERLOAD           ((WORD32) (USP_COMM_ERR_BASE + 67)) /* ��������ʱ����zte˽����Ϣ�ĳ��ȳ�����Ԥ��Ļ����� */
#define USP_COMM_ERR_ZTE_KEEPLIVE               ((WORD32) (USP_COMM_ERR_BASE + 68)) /* zte˽����·����ʧ�� */
#define USP_COMM_ERR_FULL_UHF_UDP_LINK_CTRL     ((WORD32) (USP_COMM_ERR_BASE + 69)) /* ����ƵUDP��·���ƽṹ���� */
#define USP_COMM_ERR_RECV_CLOSE_LINK_MSG        ((WORD32) (USP_COMM_ERR_BASE + 70)) /* �յ�����ر���·��Ϣ */
#define USP_COMM_ERR_CONFIG_CHANGE              ((WORD32) (USP_COMM_ERR_BASE + 71)) /* ͨ�����÷����˱仯 */
#define USP_COMM_ERR_CLOSE_OLD_LINK             ((WORD32) (USP_COMM_ERR_BASE + 72)) /* �ر�ԭ����· */
#define USP_COMM_ERR_RECV_DMP_CLOSE_MSG         ((WORD32) (USP_COMM_ERR_BASE + 73)) /* �յ���ϲ��Է�������·�ر���Ϣ */
#define USP_COMM_ERR_REJECT_NEW_LINK            ((WORD32) (USP_COMM_ERR_BASE + 74)) /* �ܾ�����· */
#define USP_COMM_ERR_SEND_LINK_CTRL_MSG         ((WORD32) (USP_COMM_ERR_BASE + 75)) /* ����·�����̷߳�����Ϣ���� */
#define USP_COMM_ERR_CREATE_LINK_CTRL_TCP       ((WORD32) (USP_COMM_ERR_BASE + 76)) /* ������·�����߳�TCP��·���� */
#define USP_COMM_ERR_CREATE_LINK_CTRL_PIPE      ((WORD32) (USP_COMM_ERR_BASE + 77)) /* ������·�����߳�PIPE��·���� */
#define USP_COMM_ERR_INTER_BOARD_MSG_OVERLOAD   ((WORD32) (USP_COMM_ERR_BASE + 78)) /* ��������ʱ���ְ����Ϣ�ĳ��ȳ�����Ԥ��Ļ����� */
#define USP_COMM_ERR_INTER_BOARD_KEEPLIVE       ((WORD32) (USP_COMM_ERR_BASE + 79)) /* ���ͨ����·����ʧ�� */
#define USP_COMM_ERR_INTER_BOARD_INVALID_TNO    ((WORD32) (USP_COMM_ERR_BASE + 80)) /* ���ͨ�Ž�����Ϣת��ʱ����Ŵ��� */     
#define USP_COMM_ERR_MPI_INVALID_HEAD           ((WORD32) (USP_COMM_ERR_BASE + 81)) /* mpiЭ����ʼͷ�ֽڴ��� */
#define USP_COMM_ERR_MPI_MSG_OVERLOAD           ((WORD32) (USP_COMM_ERR_BASE + 82)) /* ��������ʱ����mpi��Ϣ�ĳ��ȳ�����Ԥ��Ļ����� */
#define USP_COMM_ERR_MPI_MSG_CRC                ((WORD32) (USP_COMM_ERR_BASE + 83)) /* mpiЭ����ϢCRCУ����� */
#define USP_COMM_ERR_MPI_DECODE_UNCOMPLETE      ((WORD32) (USP_COMM_ERR_BASE + 84)) /* mpiЭ����Ϣ����δ��� */
#define USP_COMM_ERR_FULL_MPI_LINK_CTRL         ((WORD32) (USP_COMM_ERR_BASE + 85)) /* mpi��·���ƽṹ���� */
#define USP_COMM_ERR_MALLOC_HANDLE              ((WORD32) (USP_COMM_ERR_BASE + 86)) /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_COMM_ERR_ZTE_UNKNOW_MSG             ((WORD32) (USP_COMM_ERR_BASE + 87)) /* δ֪��zte��Ϣ */
#define USP_COMM_ERR_SMS_DECODE_FRAME           ((WORD32) (USP_COMM_ERR_BASE + 88)) /* ����sms����֡���� */
#define USP_COMM_ERR_SMS_SEND                   ((WORD32) (USP_COMM_ERR_BASE + 89)) /* ����sms����֡���� */
#define USP_COMM_ERR_SMS_RECV                   ((WORD32) (USP_COMM_ERR_BASE + 90)) /* ����sms����֡���� */
#define USP_COMM_ERR_SMS_DELE                   ((WORD32) (USP_COMM_ERR_BASE + 91)) /* ɾ������Ϣ���� */
#define USP_COMM_ERR_SMS_TEL_LEN                ((WORD32) (USP_COMM_ERR_BASE + 92)) /* �绰���볤�Ȳ���ȷ */
#define USP_COMM_ERR_SMS_PDU_TYPE               ((WORD32) (USP_COMM_ERR_BASE + 93)) /* δ֪��PDU����������� */
#define USP_COMM_ERR_SMS_CSQ                    ((WORD32) (USP_COMM_ERR_BASE + 94)) /* ��ѯ�����ź�ǿ�ȴ��� */

/***********************************************************
*   ����ģ��(CMP)������                                   *
***********************************************************/
#define   USP_OAM_CMP_GET_MSGQUE_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 1))   //��ȡ��Ϣ���г���
#define   USP_OAM_CMP_RECV_MSGQUE_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 2))   //������Ϣ����
#define   USP_OAM_CMP_PARAM_POINTER_NULL        ((WORD32) (USP_OAM_CMP_ERR_BASE + 3))   //����ָ��Ϊ��
#define   USP_OAM_CMP_INVALID_PROC_STATE        ((WORD32) (USP_OAM_CMP_ERR_BASE + 4))   //�Ƿ�����״̬
#define   USP_OAM_CMP_INVALID_MSG               ((WORD32) (USP_OAM_CMP_ERR_BASE + 5))   //��Ч����Ϣ
#define   USP_OAM_CMP_CFG_FILE_DATA_ERR         ((WORD32) (USP_OAM_CMP_ERR_BASE + 6))   //�����ļ�����������
#define   USP_OAM_CMP_FLASH_DATA_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 7))   //FLASH����������
#define   USP_OAM_CMP_EEPROM_DATA_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 8))   //EEPROM����������
#define   USP_OAM_CMP_DATA_SIZE_ERR             ((WORD32) (USP_OAM_CMP_ERR_BASE + 9))   //���ݴ�С��ƥ��
#define   USP_OAM_CMP_CRC_ERR                   ((WORD32) (USP_OAM_CMP_ERR_BASE + 10))  //crcУ�����ݲ�ƥ��
#define   USP_OAM_CMP_CFG_FILE_NONE             ((WORD32) (USP_OAM_CMP_ERR_BASE + 11))  //û�������ļ�
#define   USP_OAM_CMP_MALLOC_ERR                ((WORD32) (USP_OAM_CMP_ERR_BASE + 12))  //���䶯̬�ڴ�����
#define   USP_OAM_CMP_SEND_MSGQUE_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 13))  //������Ϣ����
#define   USP_OAM_CMP_READ_EEPROM_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 14))  //��ȡeeprom����
#define   USP_OAM_CMP_FILE_CTL_ERR              ((WORD32) (USP_OAM_CMP_ERR_BASE + 15))  //���ļ��Ķ�д��λ�Ȳ�������
#define   USP_OAM_CMP_CFG_ITEM_ERR              ((WORD32) (USP_OAM_CMP_ERR_BASE + 16))  //������������������
#define   USP_OAM_CMP_READ_FLASH_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 17))  //��ȡflash����
#define   USP_OAM_CMP_WRITE_FLASH_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 18))  //дflash����
#define   USP_OAM_CMP_WRITE_EEPROM_ERR          ((WORD32) (USP_OAM_CMP_ERR_BASE + 19))  //дeeprom����
#define   USP_OAM_CMP_OUT_MSG_ADAPTER_ERR       ((WORD32) (USP_OAM_CMP_ERR_BASE + 20))  //��һ������������
#define   USP_OAM_CMP_IN_MSG_ADAPTER_ERR        ((WORD32) (USP_OAM_CMP_ERR_BASE + 21))  //��һ�������������
#define   USP_OAM_CMP_MODULE_CONF_FILE_ERR      ((WORD32) (USP_OAM_CMP_ERR_BASE + 22))  //����ģ��������ļ�����
#define   USP_OAM_CMP_SET_DEFAULT_CFG_ERR       ((WORD32) (USP_OAM_CMP_ERR_BASE + 23))  //��һ������Ĭ�����ó���
#define   USP_OAM_CMP_INIT_EEPROM_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 24))  //eeprom��ʼ������
#define   USP_OAM_CMP_MEMORY_BEYOND_ERR         ((WORD32) (USP_OAM_CMP_ERR_BASE + 25))  //�ڴ�Խ��          
#define   USP_OAM_CMP_INIT_EPLD_ERR             ((WORD32) (USP_OAM_CMP_ERR_BASE + 26))  //��ʼ��epld����  
#define   USP_OAM_CMP_ERASE_FLASH_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 27))  //����flash����
#define   USP_OAM_CMP_CFG_DATA_TOO_LONG         ((WORD32) (USP_OAM_CMP_ERR_BASE + 28))  //�������ݹ����޷�������Ϣ���أ������޸���Ϣ�����СUSP_OAM_CMP_MAX_MSG_LEN
#define   USP_OAM_CMP_INIT_IOPORT_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 29))  //��ʼ��IOPORT����  
#define   USP_OAM_CMP_INIT_FLASH_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 30))  //��ʼ��FLASH����  
#define   USP_OAM_CMP_DEV_MODLE_NOT_MATCH_ERR   ((WORD32) (USP_OAM_CMP_ERR_BASE + 31))  //���Ͳ�ƥ��
#define   USP_OAM_CMP_PARSE_DEFAULT_CFG_ERR     ((WORD32) (USP_OAM_CMP_ERR_BASE + 32))  //����Ĭ�����ô���
#define   USP_OAM_CMP_READ_FILE_ERR             ((WORD32) (USP_OAM_CMP_ERR_BASE + 33))  //�������ļ�����
#define   USP_OAM_CMP_PARSE_FILE_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 34))  //���������ļ�����
#define   USP_OAM_CMP_PARSE_KEY_ERR             ((WORD32) (USP_OAM_CMP_ERR_BASE + 35))  //�����ؼ��ִ���
#define   USP_OAM_CMP_SEARCH_KEY_ERR            ((WORD32) (USP_OAM_CMP_ERR_BASE + 36))  //�����ؼ��ִ���
#define   USP_OAM_CMP_INVALID_TYPE_ERR          ((WORD32) (USP_OAM_CMP_ERR_BASE + 37))  //�������ʹ���
#define   USP_OAM_CMP_OPEN_FILE_ERR             ((WORD32) (USP_OAM_CMP_ERR_BASE + 38))  //�������ļ�����
#define   USP_OAM_CMP_MALLOC_HANDLE_ERR         ((WORD32) (USP_OAM_CMP_ERR_BASE + 39))  //��ȡ��̬�����ڴ�ʧ��
#define   USP_OAM_CMP_KEY_TOO_LARGE_ERR         ((WORD32) (USP_OAM_CMP_ERR_BASE + 40))  //�ؼ��ֹ���
#define   USP_OAM_CMP_STRING_TOO_LARGE_ERR      ((WORD32) (USP_OAM_CMP_ERR_BASE + 41))  //�ַ���ֵ����
#define   USP_OAM_CMP_INVALID_LINK_TYPE_ERR     ((WORD32) (USP_OAM_CMP_ERR_BASE + 42))  //�Ƿ�����·��������
#define   USP_OAM_CMP_INVALID_RS_ATTR_INDEX_ERR ((WORD32) (USP_OAM_CMP_ERR_BASE + 43))  //�Ƿ��Ĵ�����������
#define   USP_OAM_CMP_READ_SYSDATA_ERR          ((WORD32) (USP_OAM_CMP_ERR_BASE + 44))  //��ȡϵͳ����������
#define   USP_OAM_CMP_CREATE_SOCK_ERR           ((WORD32) (USP_OAM_CMP_ERR_BASE + 45))  //����socket�׽���ʧ��
#define   USP_OAM_CMP_IOCTL_ERR                 ((WORD32) (USP_OAM_CMP_ERR_BASE + 46))  //����ioctlʧ��

/***********************************************************
*   IP����ģ��(IPD)������                                   *
***********************************************************/
#define   USP_COMM_IPD_INVOKE_THREAD_FAIL               ((WORD32) (USP_COMM_IPD_ERR_BASE + 1)) /* �����߳�ʧ�� */
#define   USP_COMM_IPD_PARAM_POINTER_NULL               ((WORD32) (USP_COMM_IPD_ERR_BASE + 2)) /* ����ָ��Ϊ��ָ�� */
#define   USP_COMM_IPD_CREATE_SOCK_ERROR                ((WORD32) (USP_COMM_IPD_ERR_BASE + 3)) /* ����socket�׽���ʧ�� */
#define   USP_COMM_IPD_BIND_SOCK_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 4)) /* bind()ʧ�� */            
#define   USP_COMM_IPD_ADD_MULTICAST_FAIL               ((WORD32) (USP_COMM_IPD_ERR_BASE + 5)) /* ����ಥ��ʧ�� */
#define   USP_COMM_IPD_SET_MULTICAST_TTL_FAIL           ((WORD32) (USP_COMM_IPD_ERR_BASE + 6)) /* ����ttlʧ�� */            
#define   USP_COMM_IPD_SET_MULTICAST_LOOP_FAIL          ((WORD32) (USP_COMM_IPD_ERR_BASE + 7)) /* ����loopʧ�� */               
#define   USP_COMM_IPD_DE_ENCODE_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 8)) /* llrp��������� */
#define   USP_COMM_IPD_MULTICAST_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 9)) /* �ಥ���� */
#define   USP_COMM_IPD_SOCK_RECV_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 10)) /* �������ݳ��� */
#define   USP_COMM_IPD_SET_IFADDR_ERROR                 ((WORD32) (USP_COMM_IPD_ERR_BASE + 11)) /* ����½ӿڵ������� */                                
#define   USP_COMM_IPD_SET_NETMASK_ERROR                ((WORD32) (USP_COMM_IPD_ERR_BASE + 12)) /* ������������ */                            
#define   USP_COMM_IPD_CONNECT_SOCK_ERROR               ((WORD32) (USP_COMM_IPD_ERR_BASE + 13)) /* connect���� */
#define   USP_COMM_IPD_GET_IFFLAGS_ERROR                ((WORD32) (USP_COMM_IPD_ERR_BASE + 14)) /* ��ȡ�ӿ���Ϣ���� */
#define   USP_COMM_IPD_SET_IFFLAGS_ERROR                ((WORD32) (USP_COMM_IPD_ERR_BASE + 15)) /* ���ýӿ���Ϣ���� */
#define   USP_COMM_IPD_SEND_MSG_ERROR                   ((WORD32) (USP_COMM_IPD_ERR_BASE + 16)) /* ������Ϣ���� */
#define   USP_COMM_IPD_SOCK_SEND_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 17)) /* ������������ */
#define   USP_COMM_IPD_RCV_MSG_ERR                      ((WORD32) (USP_COMM_IPD_ERR_BASE + 18)) /* ������������ */
#define   USP_COMM_IPD_GET_MSGQUEUE_ERR                 ((WORD32) (USP_COMM_IPD_ERR_BASE + 19)) /* ��ȡ��Ϣ�������� */
#define   USP_COMM_IPD_INVALID_SOCKET_FD                ((WORD32) (USP_COMM_IPD_ERR_BASE + 20)) /* ��Ч���׽��� */
#define   USP_COMM_IPD_DEL_TEMP_IP_ERROR                ((WORD32) (USP_COMM_IPD_ERR_BASE + 21)) /* ɾ����ʱip���� */
#define   USP_COMM_IPD_SIGACTION_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 22)) /* ʹ��sigaction���� */
#define   USP_COMM_IPD_SELECT_ERROR                     ((WORD32) (USP_COMM_IPD_ERR_BASE + 23)) /* ʹ��select���� */ 
#define   USP_COMM_IPD_CHECKCRC_ERROR                   ((WORD32) (USP_COMM_IPD_ERR_BASE + 24)) /* CRCУ��ʧ�� */
#define   USP_COMM_IPD_WAITMSG_OVERTIME_ERROR           ((WORD32) (USP_COMM_IPD_ERR_BASE + 25)) /* �ȴ���Ϣ��ʱ���� */
#define   USP_COMM_IPD_MAGIC_ERROR                      ((WORD32) (USP_COMM_IPD_ERR_BASE + 26)) /* ħ���ִ��� */
#define   USP_COMM_IPD_CHECKWORD_ERROR                  ((WORD32) (USP_COMM_IPD_ERR_BASE + 27)) /* У���ִ��� */
#define   USP_COMM_IPD_IOCTL_ERROR                      ((WORD32) (USP_COMM_IPD_ERR_BASE + 28)) /* ioctl���� */
#define   USP_COMM_IPD_INVALID_MSG                      ((WORD32) (USP_COMM_IPD_ERR_BASE + 29)) /* �Ƿ���Ϣ */ 
#define   USP_COMM_IPD_DECODEMESSAGE_ERROR              ((WORD32) (USP_COMM_IPD_ERR_BASE + 30)) /* ����ʧ�� */
#define   USP_COMM_IPD_ENCODEMESSAGE_ERROR              ((WORD32) (USP_COMM_IPD_ERR_BASE + 31)) /* ����ʧ�� */
#define   USP_COMM_IPD_MSGLEN_ERROR                     ((WORD32) (USP_COMM_IPD_ERR_BASE + 32)) /* ��Ϣ���ȴ��� */

/***********************************************************
*   ϵͳ����(SCP)������                                    *
***********************************************************/
#define USP_OSS_SCP_ERR_INVALID_TNO             ((WORD32) (USP_ERR_OSS_SCP_BASE + 1)) /* ���������� */     
#define USP_OSS_SCP_ERR_INVALID_TASK            ((WORD32) (USP_ERR_OSS_SCP_BASE + 2)) /* ���񲻿��� */  
#define USP_OSS_SCP_ERR_POWERON                 ((WORD32) (USP_ERR_OSS_SCP_BASE + 3)) /* �ϵ�ʧ�� */  
#define USP_OSS_SCP_ERR_RECV_POWERON_TIMEOUT    ((WORD32) (USP_ERR_OSS_SCP_BASE + 4)) /* �����ϵ�Ӧ��ʱ */     
#define USP_OSS_SCP_ERR_RECV_POWERON_FAIL       ((WORD32) (USP_ERR_OSS_SCP_BASE + 5)) /* �յ��ϵ�ʧ��Ӧ�� */     
#define USP_OSS_SCP_ERR_RECV_SERVICE_MSG        ((WORD32) (USP_ERR_OSS_SCP_BASE + 6)) /* �ṩϵͳ���Ʒ���ʱ������Ϣʧ�� */ 
#define USP_OSS_SCP_ERR_RECV_UNKNOW_MSG         ((WORD32) (USP_ERR_OSS_SCP_BASE + 7)) /* �յ�δ֪����Ϣ */ 
#define USP_OSS_SCP_ERR_SEND_KEEPALIVE          ((WORD32) (USP_ERR_OSS_SCP_BASE + 8)) /* ����������Ϣ���� */  
#define USP_OSS_SCP_ERR_KEEPLIVE                ((WORD32) (USP_ERR_OSS_SCP_BASE + 9)) /* Ӧ���������� */  
#define USP_OSS_SCP_ERR_CLI_CMD                 ((WORD32) (USP_ERR_OSS_SCP_BASE + 10)) /* CLI��������� */  
#define USP_OSS_SCP_ERR_CLI_PROCESS             ((WORD32) (USP_ERR_OSS_SCP_BASE + 11)) /* CLI�������� */  
#define USP_OSS_SCP_ERR_INVALID_MQ              ((WORD32) (USP_ERR_OSS_SCP_BASE + 12)) /* ��Ϣ���в����� */  
#define USP_OSS_SCP_ERR_INVALID_POWERON_TNO     ((WORD32) (USP_ERR_OSS_SCP_BASE + 13)) /* ��Ҫ�ϵ������Ų���ȷ */  
#define USP_OSS_SCP_ERR_SET_TIME                ((WORD32) (USP_ERR_OSS_SCP_BASE + 14)) /* ����ϵͳʱ����� */  
#define USP_OSS_SCP_ERR_MALLOC_WTD_CTRL         ((WORD32) (USP_ERR_OSS_SCP_BASE + 15)) /* ��ȡ���Ź������ڴ�ʧ�� */
#define USP_OSS_SCP_ERR_INIT_COMM               ((WORD32) (USP_ERR_OSS_SCP_BASE + 16)) /* ��ʼ��ͨ����ʩʧ�� */
#define USP_OSS_SCP_ERR_NULL_POINTER            ((WORD32) (USP_ERR_OSS_SCP_BASE + 17)) /* ��ָ�� */
#define USP_OSS_SCP_ERR_OPEN_RESET_STATUS_FILE  ((WORD32) (USP_ERR_OSS_SCP_BASE + 18)) /* ������״̬��¼�ļ�ʧ�� */
#define USP_OSS_SCP_ERR_READ_RESET_STATUS_FILE  ((WORD32) (USP_ERR_OSS_SCP_BASE + 19)) /* ������״̬��¼�ļ�ʧ�� */
#define USP_OSS_SCP_ERR_WRITE_RESET_STATUS_FILE ((WORD32) (USP_ERR_OSS_SCP_BASE + 20)) /* д����״̬��¼�ļ�ʧ�� */
#define USP_OSS_SCP_ERR_LACK_OF_MEMORY          ((WORD32) (USP_ERR_OSS_SCP_BASE + 21)) /* �ڴ����ʧ�� */
#define USP_OSS_SCP_ERR_MSG_BUFF_SIZE_LARGE     ((WORD32) (USP_ERR_OSS_SCP_BASE + 22)) /* ���泤�ȴ��ڽ���buff���� */
#define USP_OSS_SCP_ERR_MSG_BUFF_SIZE_SMALL     ((WORD32) (USP_ERR_OSS_SCP_BASE + 23)) /* ���泤��С�ڽ��յ���Ϣ�ĳ��� */
#define USP_OSS_SCP_ERR_NOT_FOUND_IN_MSG_BUFF   ((WORD32) (USP_ERR_OSS_SCP_BASE + 24)) /* �ڻ�����δ�ҵ�ָ����Ϣ */
#define USP_OSS_SCP_ERR_MSG_BUFF_OVERFLOW       ((WORD32) (USP_ERR_OSS_SCP_BASE + 25)) /* �������� */
#define USP_OSS_SCP_ERR_MALLOC_HANDLE           ((WORD32) (USP_ERR_OSS_SCP_BASE + 26)) /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_OSS_SCP_ERR_INVALID_HANDLE          ((WORD32) (USP_ERR_OSS_SCP_BASE + 27)) /* �Ƿ��Ķ�̬���� */
#define USP_OSS_SCP_ERR_ATTACH_TCB              ((WORD32) (USP_ERR_OSS_SCP_BASE + 28)) /* ���ӵ�������ƹ����ڴ�ʧ�� */
#define USP_OSS_SCP_ERR_CUST_FULL               ((WORD32) (USP_ERR_OSS_SCP_BASE + 29)) /* �û��ű������ﵽ�����ֵ */
#define USP_OSS_SCP_ERR_SEND_MSG                ((WORD32) (USP_ERR_OSS_SCP_BASE + 30)) /* ������Ϣ���� */
#define USP_OSS_SCP_ERR_RECV_MSG                ((WORD32) (USP_ERR_OSS_SCP_BASE + 31)) /* ������Ϣ���� */
#define USP_OSS_SCP_ERR_RECV_MSG_TIMEOUT        ((WORD32) (USP_ERR_OSS_SCP_BASE + 32)) /* ������Ϣ��ʱ */
#define USP_OSS_SCP_ERR_IB_KEEPLIVE             ((WORD32) (USP_ERR_OSS_SCP_BASE + 33)) /* ���SCP����ʧ�� */
#define USP_OSS_SCP_ERR_GET_NTP_CFG             ((WORD32) (USP_ERR_OSS_SCP_BASE + 34)) /* ��ȡNTP����ʧ�� */
#define USP_OSS_SCP_ERR_NTP_UPDATE              ((WORD32) (USP_ERR_OSS_SCP_BASE + 35)) /* NTPͬ��ʧ�� */

/***********************************************************
*   ���ݴ洢ģ��(DSM)������                                *
***********************************************************/
#define USP_OSS_DSM_ERR_NULL_POINTER            ((WORD32) (USP_ERR_OSS_DSM_BASE + 1))  /* ��ָ�� */     
#define USP_OSS_DSM_ERR_OPEN                    ((WORD32) (USP_ERR_OSS_DSM_BASE + 2))  /* �򿪴�������ʧ�� */ 
#define USP_OSS_DSM_ERR_READ                    ((WORD32) (USP_ERR_OSS_DSM_BASE + 3))  /* ����������ʧ�� */ 
#define USP_OSS_DSM_ERR_WRITE                   ((WORD32) (USP_ERR_OSS_DSM_BASE + 4))  /* д�洢����ʧ�� */ 
#define USP_OSS_DSM_ERR_ERASE                   ((WORD32) (USP_ERR_OSS_DSM_BASE + 5))  /* �����洢����ʧ�� */ 
#define USP_OSS_DSM_ERR_INVALID_DEVICE_TYPE     ((WORD32) (USP_ERR_OSS_DSM_BASE + 6))  /* �Ƿ��Ĵ洢�豸���� */ 
#define USP_OSS_DSM_ERR_INVALID_DATA_STRUCT     ((WORD32) (USP_ERR_OSS_DSM_BASE + 7))  /* �Ƿ��Ĵ洢�ṹ���� */ 
#define USP_OSS_DSM_ERR_INVALID_DATA_SIZE       ((WORD32) (USP_ERR_OSS_DSM_BASE + 8))  /* �Ƿ������ݳ��� */ 
#define USP_OSS_DSM_ERR_NOT_OPEN                ((WORD32) (USP_ERR_OSS_DSM_BASE + 9))  /* �洢����û�г�ʼ�� */ 
#define USP_OSS_DSM_ERR_NOT_ENOUTH_SPACE        ((WORD32) (USP_ERR_OSS_DSM_BASE + 10)) /* �洢����ռ䲻�� */ 
#define USP_OSS_DSM_ERR_STORAGE_EMPTY           ((WORD32) (USP_ERR_OSS_DSM_BASE + 11)) /* �洢������û������ */ 
#define USP_OSS_DSM_ERR_BUF_TOO_SMALL           ((WORD32) (USP_ERR_OSS_DSM_BASE + 12)) /* �������ռ䲻�� */ 
#define USP_OSS_DSM_ERR_BLOCK_DESTROY           ((WORD32) (USP_ERR_OSS_DSM_BASE + 13)) /* �洢�������ݿ��� */ 
#define USP_OSS_DSM_ERR_TRUNCATE                ((WORD32) (USP_ERR_OSS_DSM_BASE + 14)) /* ��մ洢����ʧ�� */ 
#define USP_OSS_DSM_ERR_MALLOC                  ((WORD32) (USP_ERR_OSS_DSM_BASE + 15)) /* ��̬��ȡ�ڴ�ʧ�� */ 
#define USP_OSS_DSM_ERR_INVALID_KEY             ((WORD32) (USP_ERR_OSS_DSM_BASE + 16)) /* �Ƿ��Ĵ洢�����ֵ */ 
#define USP_OSS_DSM_ERR_MALLOC_HANDLE           ((WORD32) (USP_ERR_OSS_DSM_BASE + 17)) /* ��ȡ��̬�����ڴ�ʧ�� */

/***********************************************************
*   TFTP������                                             *
***********************************************************/
#define USP_ERR_PUB_TFTP_BEGIN                  ((WORD32) (USP_ERR_PUB_BASE + 1))
#define USP_ERR_PUB_TFTP_OPTION                 ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 1))   /* ����ѡ����� */
#define USP_ERR_PUB_TFTP_SOCKET                 ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 2))   /* Socket���� */
#define USP_ERR_PUB_TFTP_BIND                   ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 3))   /* Bind���� */
#define USP_ERR_PUB_TFTP_SENDTO                 ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 4))   /* �������ݴ��� */
#define USP_ERR_PUB_TFTP_RECV                   ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 5))   /* �������ݴ��� */
#define USP_ERR_PUB_TFTP_BUF_TOO_SMALL          ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 6))   /* ���ջ�����̫С */
#define USP_ERR_PUB_TFTP_NO_LINK                ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 7))   /* ��û�н���tftp��· */
#define USP_ERR_PUB_TFTP_TIMEOUT                ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 8))   /* ���ճ�ʱ */
#define USP_ERR_PUB_UFTP_OPTION                 ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 9))   /* ����ѡ����� */
#define USP_ERR_PUB_UFTP_CONNECT                ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 10))  /* Connect���� */
#define USP_ERR_PUB_UFTP_SEND                   ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 11))  /* �������ݴ��� */
#define USP_ERR_PUB_UFTP_RECV                   ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 12))  /* �������ݴ��� */
#define USP_ERR_PUB_UFTP_BUF_TOO_SMALL          ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 13))  /* ���ջ�����̫С */
#define USP_ERR_PUB_UFTP_NO_LINK                ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 14))  /* ��û�н���uftp��· */
#define USP_ERR_PUB_UFTP_TIMEOUT                ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 15))  /* ���ճ�ʱ */
#define USP_ERR_PUB_UFTP_DATA                   ((WORD32) (USP_ERR_PUB_TFTP_BEGIN + 16))  /* ���ݴ��� */

/***********************************************************
*   PSP������                                              *
***********************************************************/
#define USP_ERR_PUB_PSP_BEGIN                   ((WORD32) (USP_ERR_PUB_BASE + 50))
#define USP_ERR_PUB_PSP_MALLOC_INFO             ((WORD32) (USP_ERR_PUB_PSP_BEGIN + 1))    /* ��ȡͳ����Ϣ�ڴ�ʧ�� */
#define USP_ERR_PUB_PSP_MALLOC_HANDLE           ((WORD32) (USP_ERR_PUB_PSP_BEGIN + 2))    /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_ERR_PUB_PSP_ATTACH_INFO             ((WORD32) (USP_ERR_PUB_PSP_BEGIN + 3))    /* ���ӵ�ͳ����Ϣ�����ڴ�ʧ�� */
#define USP_ERR_PUB_PSP_INVALID_ID              ((WORD32) (USP_ERR_PUB_PSP_BEGIN + 4))    /* �Ƿ���ͳ����Ϣ��� */

/***********************************************************
*   �ڴ����ģ��(MM)������                                 *
***********************************************************/
#define USP_OSS_MM_ERR_STATIC_MALLOC            ((WORD32) (USP_ERR_OSS_MM_BASE + 1))  /* ���ܻ�ȡ�ڴ�ռ� */     
#define USP_OSS_MM_ERR_NULL_POINTER             ((WORD32) (USP_ERR_OSS_MM_BASE + 2))  /* ��ָ�� */
#define USP_OSS_MM_ERR_ZERO_SIZE                ((WORD32) (USP_ERR_OSS_MM_BASE + 3))  /* �ռ�Ϊ�� */
#define USP_OSS_MM_ERR_FIFO_FULL                ((WORD32) (USP_ERR_OSS_MM_BASE + 4))  /* FIFO���� */
#define USP_OSS_MM_ERR_FIFO_EMPTY               ((WORD32) (USP_ERR_OSS_MM_BASE + 5))  /* FIFOΪ�� */ 
#define USP_OSS_MM_ERR_MUTEX_INVALID            ((WORD32) (USP_ERR_OSS_MM_BASE + 6))  /* ������������ */ 
#define USP_OSS_MM_ERR_FIFO_ITEM_INVALID        ((WORD32) (USP_ERR_OSS_MM_BASE + 7))  /* FIFO��Ŀ������ */ 


/***********************************************************
*   ��ϲ���ģ��(DMP)������                                   *
***********************************************************/
#define USP_OAM_DMP_ERR_INVALID_PROC_STATE                ((WORD32)(USP_OAM_DMP_ERR_BASE + 1)) /* �Ƿ�������״̬ */
#define USP_OAM_DMP_ERR_INVALID_PARA                      ((WORD32)(USP_OAM_DMP_ERR_BASE + 2)) /* �Ƿ��Ĳ���*/
#define USP_OAM_DMP_ERR_UNKNOWN_MSG                       ((WORD32)(USP_OAM_DMP_ERR_BASE + 3)) /* δ֪����Ϣ */
#define USP_OAM_DMP_ERR_NULL_POINTER                      ((WORD32)(USP_OAM_DMP_ERR_BASE + 4)) /* ��ָ�� */
#define USP_OAM_DMP_ERR_OVERFLOW_TESTNUM                  ((WORD32)(USP_OAM_DMP_ERR_BASE + 5)) /* ���������ϲ�������� */
#define USP_OAM_DMP_ERR_INVALID_TESTID                    ((WORD32)(USP_OAM_DMP_ERR_BASE + 6)) /* �Ƿ�����ϲ�����ID  */
#define USP_OAM_DMP_ERR_SENDMSG_FAIL                      ((WORD32)(USP_OAM_DMP_ERR_BASE + 8)) /* ������Ϣʧ�� */
#define USP_OAM_DMP_ERR_RCVMSG_FAIL                       ((WORD32)(USP_OAM_DMP_ERR_BASE + 9)) /* ������Ϣʧ�� */
#define USP_OAM_DMP_ERR_INVALID_MSGID                     ((WORD32)(USP_OAM_DMP_ERR_BASE + 10)) /* �Ƿ�����ϢID  */
#define USP_OAM_DMP_ERR_ACK_ERR                           ((WORD32)(USP_OAM_DMP_ERR_BASE + 11)) /* �����Ӧ���� */
#define USP_OAM_DMP_ERR_INVALID_TNO                       ((WORD32)(USP_OAM_DMP_ERR_BASE + 12)) /* ���������� */
#define USP_OAM_DMP_ERR_DATA_OVERFLOW                     ((WORD32)(USP_OAM_DMP_ERR_BASE + 13)) /* ������� */
#define USP_OAM_DMP_ERR_IN_MSG_ADAPTER_ERR                ((WORD32)(USP_OAM_DMP_ERR_BASE + 14)) /* ������Ϣ���亯��ת��ʧ�� */
#define USP_OAM_DMP_ERR_OUT_MSG_ADAPTER_ERR               ((WORD32)(USP_OAM_DMP_ERR_BASE + 15)) /* �����Ϣ���亯��ת��ʧ�� */
#define USP_OAM_DMP_ERR_INCLICMDPARAERROR                 ((WORD32)(USP_OAM_DMP_ERR_BASE + 16)) /* �������������ת��ʧ�� */
#define USP_OAM_DMP_ERR_OUTCLICMDPARAERROR                ((WORD32)(USP_OAM_DMP_ERR_BASE + 17)) /* �������������ת��ʧ�� */
#define USP_OAM_DMP_ERR_WAITAPPRESPONSEOVERTIME           ((WORD32)(USP_OAM_DMP_ERR_BASE + 18)) /* �ȴ�ҵ��Ӧ��ģ�鳬ʱ */
#define USP_OAM_DMP_ERR_RECVUNEXPECTEDMSG                 ((WORD32)(USP_OAM_DMP_ERR_BASE + 19)) /* ���յ��˲��ڴ�����Ϣ */
#define USP_OAM_DMP_ERR_READEEPROMERROR                   ((WORD32)(USP_OAM_DMP_ERR_BASE + 20)) /* ��EEPROM���� */
#define USP_OAM_DMP_ERR_EEPROMRWTESTERROR                 ((WORD32)(USP_OAM_DMP_ERR_BASE + 21)) /* EEPROM��д���Գ��� */
#define USP_OAM_DMP_ERR_WRITEEEPROMERROR                  ((WORD32)(USP_OAM_DMP_ERR_BASE + 22)) /* дEEPROM���� */
#define USP_OAM_DMP_ERR_GPIOSETINPUTERROR                 ((WORD32)(USP_OAM_DMP_ERR_BASE + 23)) /* GPIO���ó��� */
#define USP_OAM_DMP_ERR_INVALIDADDRERROR                  ((WORD32)(USP_OAM_DMP_ERR_BASE + 24)) /* �Ƿ��ĵ�ַ */
#define USP_OAM_DMP_ERR_PARAMETERERROR                    ((WORD32)(USP_OAM_DMP_ERR_BASE + 25)) /* �������� */
#define USP_OAM_DMP_ERR_NOTUSPTESTID                      ((WORD32)(USP_OAM_DMP_ERR_BASE + 26)) /* ��ƽ̨������ */
#define USP_OAM_DMP_ERR_NON_DIGATAL                       ((WORD32) (USP_OAM_DMP_ERR_BASE + 27)) /* ������ */   
#define USP_OAM_DMP_ERR_MALLOC_HANDLE                     ((WORD32) (USP_OAM_DMP_ERR_BASE + 28)) /* ��ȡ��̬�����ڴ�ʧ�� */ 
#define USP_OAM_DMP_ERR_NON_DEVMODLE                      ((WORD32) (USP_OAM_DMP_ERR_BASE + 29)) /* û����֮ƥ��Ļ��� */
#define USP_OAM_DMP_ERR_CLINOTSUPPORT                     ((WORD32) (USP_OAM_DMP_ERR_BASE + 28)) /* �����в�֧�ָ���ϲ����� */ 
/***********************************************************
*                  �澯����ģ��(AMP)������                          *
***********************************************************/
/*AMPģ�鷵�ظ���̨�Ĵ�����*/
#define USP_OAM_AMP_ERR_NULL_POINTER                    (USP_OAM_AMP_ERR_BASE + 0)/*����ָ��Ϊ��*/
#define USP_OAM_AMP_ERR_IN_MSG_ADAPTER_ERR              (USP_OAM_AMP_ERR_BASE + 1)/*��һ�������������*/
#define USP_OAM_AMP_ERR_OUT_MSG_ADAPTER_ERR             (USP_OAM_AMP_ERR_BASE + 2)/*��һ������������*/
#define USP_OAM_AMP_ERR_UNKNOWN_MSG                     (USP_OAM_AMP_ERR_BASE + 3)/*���յ�δ֪��Ϣ*/
#define USP_OAM_AMP_ERR_DEAL_DELETE_MSG                 (USP_OAM_AMP_ERR_BASE + 4)/*����ɾ���澯����*/
#define USP_OAM_AMP_ERR_DEAL_SYNC_MSG                   (USP_OAM_AMP_ERR_BASE + 5)/*����ͬ���澯����*/
#define USP_OAM_AMP_ERR_SEND_MSG                        (USP_OAM_AMP_ERR_BASE + 6)/*������Ϣ����*/
#define USP_OAM_AMP_ERR_RECV_MSG                        (USP_OAM_AMP_ERR_BASE + 7)/*������Ϣ����*/
#define USP_OAM_AMP_ERR_AID_NUM                         (USP_OAM_AMP_ERR_BASE + 7)/*�����aidֵ*/
#define USP_OAM_AMP_ERR_SAVE_PACKAGE                    (USP_OAM_AMP_ERR_BASE + 8)/*����ͬ�����ݰ�����*/
#define USP_OAM_AMP_ERR_SAVE_ALL_PACKAGE                (USP_OAM_AMP_ERR_BASE + 9)/*���ղ�������ͬ�����ݰ�*/
#define USP_OAM_AMP_ERR_DEAL_BCK_MSG                    (USP_OAM_AMP_ERR_BASE + 10)/*�����̨��Ϣ����*/
#define USP_OAM_AMP_ERR_DEAL_SCRALARM_MSG               (USP_OAM_AMP_ERR_BASE + 11)/*����澯Դ��Ϣ����*/
#define USP_OAM_AMP_ERR_MALLOC_SPACE                    (USP_OAM_AMP_ERR_BASE + 12)/*����ռ�ʧ��*/
#define USP_OAM_AMP_ERR_REALLOCATE_AID                  (USP_OAM_AMP_ERR_BASE + 13)/*���·���AIDʧ��*/
#define USP_OAM_AMP_ERR_DEAL_FULL_PROC                  (USP_OAM_AMP_ERR_BASE + 14)/*����澯��������*/
#define USP_OAM_AMP_ERR_RECV_SRC_MSG                    (USP_OAM_AMP_ERR_BASE + 15)/*ǰ̨���յ�����ĸ澯Դ��Ϣ*/
#define USP_OAM_AMP_ERR_POOL_OVERFLOW                   (USP_OAM_AMP_ERR_BASE + 16)/*�澯�����*/
#define USP_OAM_AMP_ERR_POOL_PUT                        (USP_OAM_AMP_ERR_BASE + 17)/*�澯�ز������*/
#define USP_OAM_AMP_ERR_POOL_MISS_FULL_ALARM            (USP_OAM_AMP_ERR_BASE + 18)/*��ʧ�澯�����澯*/
#define USP_OAM_AMP_ERR_REPEAT_RESTORE                  (USP_OAM_AMP_ERR_BASE + 19)/*�ظ��ϱ��澯�ָ�*/
#define USP_OAM_AMP_ERR_RECV_BCK_INVALID_MSG            (USP_OAM_AMP_ERR_BASE + 20)/*���յ���̨��Ч��Ϣ*/
#define USP_OAM_AMP_ERR_ALARM_POOL_EMPTY                (USP_OAM_AMP_ERR_BASE + 21)/*ǰ̨�澯�ؿ�*/
#define USP_OAM_AMP_ERR_MSG_LEN_OVERFLOW                (USP_OAM_AMP_ERR_BASE + 22)/*�澯��Ϣ�������*/
#define USP_OAM_AMP_ERR_INVALID_PARAM                   (USP_OAM_AMP_ERR_BASE + 23)/*��Ч����*/


/***********************************************************                   
*                   U-Boot ������                                  *
***********************************************************/
#define USP_ERR_UBOOT_FPGA_LOAD_TIMEOUT           ((WORD32) (USP_ERR_UBOOT_BASE + 1))
#define USP_ERR_UBOOT_RAM_TEST_PARAM                 ((WORD32) (USP_ERR_UBOOT_BASE + 2))
#define USP_ERR_UBOOT_ILLEGAL_PARAM                    ((WORD32) (USP_ERR_UBOOT_BASE + 3))
#define USP_ERR_UBOOT_FLASH_TEST_PARAM              ((WORD32) (USP_ERR_UBOOT_BASE + 4))
#define USP_ERR_UBOOT_FLASH_TEST_OUT_OF_BANK  ((WORD32) (USP_ERR_UBOOT_BASE + 5))
#define USP_ERR_UBOOT_FLASH_TEST                            ((WORD32) (USP_ERR_UBOOT_BASE + 6))
#define USP_ERR_UBOOT_EEPROM_READ                        ((WORD32) (USP_ERR_UBOOT_BASE + 7))
#define USP_ERR_UBOOT_EEPROM_WRITE                      ((WORD32) (USP_ERR_UBOOT_BASE + 8))
#define USP_ERR_UBOOT_FTRUN_LEN                             ((WORD32) (USP_ERR_UBOOT_BASE + 9))
#define USP_ERR_UBOOT_FTRUN_TYPE                           ((WORD32) (USP_ERR_UBOOT_BASE + 10))
#define USP_ERR_UBOOT_RAM_TEST                               ((WORD32) (USP_ERR_UBOOT_BASE + 11))
#define USP_ERR_UBOOT_PL_INIT_EEPROM                    ((WORD32) (USP_ERR_UBOOT_BASE + 12))
#define USP_ERR_UBOOT_IP_CONFIG_PARAM                 ((WORD32) (USP_ERR_UBOOT_BASE + 13))
#define USP_ERR_UBOOT_FPGA_LD_TIMEOUT                 ((WORD32) (USP_ERR_UBOOT_BASE + 14))


/***********************************************************                   
*                   ����ƽ̨(Drv) ������                                  *
***********************************************************/

/***********************************************************
*                   ����ƽ̨Epldģ�������                            *
***********************************************************/
#define USP_ERROR_DRV_EPLD_READY                        (USP_ERR_DRV_EPLD_START+0)
#define USP_ERROR_DRV_EPLD_OPEN                         (USP_ERR_DRV_EPLD_START+1)
#define USP_ERROR_DRV_EPLD_MMAP                         (USP_ERR_DRV_EPLD_START+2)
#define USP_ERROR_DRV_EPLD_RWWIDTH                      (USP_ERR_DRV_EPLD_START+3)
#define USP_ERROR_DRV_EPLD_NULL_POINTER                 (USP_ERR_DRV_EPLD_START+4)

/***********************************************************
*                   ����ƽ̨Fpgaģ�������                            *
***********************************************************/
#define USP_ERROR_DRV_FPGA_INIT_FD                     (USP_ERR_DRV_FPGA_START+0)
#define USP_ERROR_DRV_FPGA_INIT_OPEN                   (USP_ERR_DRV_FPGA_START+1)
#define USP_ERROR_DRV_FPGA_INIT_MMAP_EPLD              (USP_ERR_DRV_FPGA_START+2)
#define USP_ERROR_DRV_FPGA_INIT_MMAP_FPGA              (USP_ERR_DRV_FPGA_START+3)
#define USP_ERROR_DRV_FPGA_READ_REG_DATA               (USP_ERR_DRV_FPGA_START+4)
#define USP_ERROR_DRV_FPGA_READ_REG_OFFSET             (USP_ERR_DRV_FPGA_START+5)
#define USP_ERROR_DRV_FPGA_WRITE_REG_OFFSET            (USP_ERR_DRV_FPGA_START+6)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_PARA            (USP_ERR_DRV_FPGA_START+7)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_LEN             (USP_ERR_DRV_FPGA_START+8)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_FLAG            (USP_ERR_DRV_FPGA_START+9)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_FD              (USP_ERR_DRV_FPGA_START+10)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_IOCTL           (USP_ERR_DRV_FPGA_START+11)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_WRITE           (USP_ERR_DRV_FPGA_START+12)
#define USP_ERROR_DRV_FPGA_READ_FRAME_PARA             (USP_ERR_DRV_FPGA_START+13)
#define USP_ERROR_DRV_FPGA_READ_FRAME_FD               (USP_ERR_DRV_FPGA_START+14)
#define USP_ERROR_DRV_FPGA_WRITE_FRAME_READ            (USP_ERR_DRV_FPGA_START+15)
#define USP_ERROR_DRV_FPGA_INT_EN_FD                   (USP_ERR_DRV_FPGA_START+16)
#define USP_ERROR_DRV_FPGA_INT_EN_IOCTL                (USP_ERR_DRV_FPGA_START+17)
#define USP_ERROR_DRV_FPGA_INT_DISEN_FD                (USP_ERR_DRV_FPGA_START+18)
#define USP_ERROR_DRV_FPGA_INT_DISEN_IOCTL             (USP_ERR_DRV_FPGA_START+19)
#define USP_ERROR_DRV_FPGA_INIT_IOCTL_TOG              (USP_ERR_DRV_FPGA_START+20)
#define USP_ERROR_DRV_FPGA_INIT_IOCTL_RESET            (USP_ERR_DRV_FPGA_START+21)
#define USP_ERROR_DRV_FPGA_1101_SEND_BROADCAST_TIMEOUT (USP_ERR_DRV_FPGA_START+22)
#define USP_ERROR_DRV_FPGA_1101_SEND_OBUID_TIMEOUT     (USP_ERR_DRV_FPGA_START+23)


/***********************************************************
*                   ����ƽ̨Wtdģ�������                   *
***********************************************************/
#define USP_ERROR_DRV_WTD_PARAM                               (USP_ERR_DRV_WTD_START+0)

/***********************************************************
*                   ����ƽ̨LEDģ�������                   *
***********************************************************/
#define USP_ERROR_DRV_LED_NUMBER                              (USP_ERR_DRV_LED_START+0)
#define USP_ERROR_DRV_LED_STATE                                 (USP_ERR_DRV_LED_START+1)

/***********************************************************
*                   ����ƽ̨Psamģ�������                            *
***********************************************************/
#define USP_ERR_DRV_PSAM_INIT_OPEN                          (USP_ERR_DRV_PSAM_START+0)
#define USP_ERR_DRV_PSAM_INIT_DONE                          (USP_ERR_DRV_PSAM_START+1)
#define USP_ERR_DRV_PSAM_ACTIVE_ID                          (USP_ERR_DRV_PSAM_START+2)
#define USP_ERR_DRV_PSAM_ACTIVE_PARA                        (USP_ERR_DRV_PSAM_START+3)
#define USP_ERR_DRV_PSAM_ACTIVE_FD                          (USP_ERR_DRV_PSAM_START+4)
#define USP_ERR_DRV_PSAM_ACTIVE_IOCTL                       (USP_ERR_DRV_PSAM_START+5)
#define USP_ERR_DRV_PSAM_CMD_ID                             (USP_ERR_DRV_PSAM_START+6)
#define USP_ERR_DRV_PSAM_CMD_FD                             (USP_ERR_DRV_PSAM_START+7)
#define USP_ERR_DRV_PSAM_CMD_PARA                           (USP_ERR_DRV_PSAM_START+8)
#define USP_ERR_DRV_PSAM_CMD_LEN                            (USP_ERR_DRV_PSAM_START+9)
#define USP_ERR_DRV_PSAM_CMD_IOCTL                          (USP_ERR_DRV_PSAM_START+10)
#define USP_ERR_DRV_PSAM_DEACTIVE_ID                        (USP_ERR_DRV_PSAM_START+11)
#define USP_ERR_DRV_PSAM_DEACTIVE_PARA                      (USP_ERR_DRV_PSAM_START+12)
#define USP_ERR_DRV_PSAM_DEACTIVE_FD                        (USP_ERR_DRV_PSAM_START+13)
#define USP_ERR_DRV_PSAM_DEACTIVE_IOCTL                     (USP_ERR_DRV_PSAM_START+14)
#define USP_ERR_DRV_PSAM_CLKFREQ_FD                         (USP_ERR_DRV_PSAM_START+15)
#define USP_ERR_DRV_PSAM_CLKFREQ_DATA                       (USP_ERR_DRV_PSAM_START+16)
#define USP_ERR_DRV_PSAM_CLKSET_IOCTL                       (USP_ERR_DRV_PSAM_START+17)
#define USP_ERR_DRV_PSAM_VOLTAGE_FD                         (USP_ERR_DRV_PSAM_START+18)
#define USP_ERR_DRV_PSAM_VOLTAGE_DATA                       (USP_ERR_DRV_PSAM_START+19)
#define USP_ERR_DRV_PSAM_VOLTAGE_IOCTL                      (USP_ERR_DRV_PSAM_START+20)
#define USP_ERR_DRV_PSAM_MALLOC_HANDLE                      (USP_ERR_DRV_PSAM_START+21)
#define USP_ERR_DRV_PSAM_CLKFREQ_ID                         (USP_ERR_DRV_PSAM_START+22)
#define USP_ERR_DRV_PSAM_VOLTAGE_ID                         (USP_ERR_DRV_PSAM_START+23)
#define USP_ERR_DRV_PSAM_CLKFREQ_MMAP                       (USP_ERR_DRV_PSAM_START+24)
#define USP_ERR_DRV_PSAM_DETECT_PARA                        (USP_ERR_DRV_PSAM_START+25)

/***********************************************************
*                   ����ƽ̨gpio ģ�������                            *
***********************************************************/
#define USP_ERROR_DRV_GPIO_READY                               (USP_ERR_DRV_GPIO_START+0)
#define USP_ERROR_DRV_GPIO_OPEN                                 (USP_ERR_DRV_GPIO_START+1)
#define USP_ERROR_DRV_GPIO_MMAP                                (USP_ERR_DRV_GPIO_START+2)
#define USP_ERROR_DRV_GPIO_RWWIDTH                         (USP_ERR_DRV_GPIO_START+3)
#define USP_ERROR_DRV_GPIO_PARAM                               (USP_ERR_DRV_GPIO_START+4)
#define USP_ERROR_DRV_GPIO_READ                                  (USP_ERR_DRV_GPIO_START+5)
#define USP_ERROR_DRV_GPIO_WRITE                                (USP_ERR_DRV_GPIO_START+6)

/***********************************************************
*                   ����ƽ̨Rtcģ�������                            *
***********************************************************/
#define USP_ERR_DRV_RTC_BUSINI                                             (USP_ERR_DRV_RTC_START+0)//��ʼ��i2c����ʧ��
#define USP_ERR_DRV_RTC_WRITEADDR                                     (USP_ERR_DRV_RTC_START+1)
#define USP_ERR_DRV_RTC_READCH                                           (USP_ERR_DRV_RTC_START+2)
#define USP_ERR_DRV_RTC_WRITECH                                         (USP_ERR_DRV_RTC_START+3)
#define USP_ERR_DRV_RTC_READOSF                                         (USP_ERR_DRV_RTC_START+4)
#define USP_ERR_DRV_RTC_WRITEOSF                                       (USP_ERR_DRV_RTC_START+5)
#define USP_ERR_DRV_RTC_READSQW                                        (USP_ERR_DRV_RTC_START+6)
#define USP_ERR_DRV_RTC_WRITESQW                                      (USP_ERR_DRV_RTC_START+7)
#define USP_ERR_DRV_RTC_READOUT                                         (USP_ERR_DRV_RTC_START+8)
#define USP_ERR_DRV_RTC_WRITEOUT                                       (USP_ERR_DRV_RTC_START+9)
#define USP_ERR_DRV_RTC_READTIME                                       (USP_ERR_DRV_RTC_START+10)
#define USP_ERR_DRV_RTC_WRITETIME                                     (USP_ERR_DRV_RTC_START+11)	
#define USP_ERR_DRV_RTC_READHOURFORMAT                         (USP_ERR_DRV_RTC_START+12)
#define USP_ERR_DRV_RTC_WRITEHOURFORMAT                       (USP_ERR_DRV_RTC_START+13)
#define USP_ERR_DRV_RTC_CH_EXCEP                                       (USP_ERR_DRV_RTC_START+14)
#define USP_ERR_DRV_RTC_OSF_STOP                                       (USP_ERR_DRV_RTC_START+15)
#define USP_ERR_DRV_RTC_TIME_INVALID                                (USP_ERR_DRV_RTC_START+16)
#define USP_ERR_DRV_RTC_READ_PARAM                                (USP_ERR_DRV_RTC_START+17)


/***********************************************************
*                   ����ƽ̨I2cģ�������                   *
***********************************************************/
#define USP_ERR_DRV_I2C_PIN                                (USP_ERR_DRV_I2C_START+1)
#define USP_ERR_DRV_I2C_FREE                               (USP_ERR_DRV_I2C_START+2)
#define USP_ERR_DRV_I2C_ACK                                (USP_ERR_DRV_I2C_START+3)
#define USP_ERR_DRV_I2C_SNDBUF                             (USP_ERR_DRV_I2C_START+4)
#define USP_ERR_DRV_I2C_RESET                              (USP_ERR_DRV_I2C_START+5)
#define USP_ERR_DRV_I2C_SEMCREATE			               (USP_ERR_DRV_I2C_START+6)
#define USP_ERR_DRV_I2C_SEM_INIT			               (USP_ERR_DRV_I2C_START+7)
#define USP_ERR_DRV_I2C_SEM_P			                   (USP_ERR_DRV_I2C_START+8)
#define USP_ERR_DRV_I2C_SEM_V			                   (USP_ERR_DRV_I2C_START+9)



/***********************************************************
*                   ����ƽ̨Ioportģ�������                            *
***********************************************************/
#define USP_ERROR_DRV_IOPORT_READY                               (USP_ERR_DRV_IOPORT_START+0)
#define USP_ERROR_DRV_IOPORT_OPEN                                 (USP_ERR_DRV_IOPORT_START+1)
#define USP_ERROR_DRV_IOPORT_MMAP                                (USP_ERR_DRV_IOPORT_START+2)
#define USP_ERROR_DRV_IOPORT_RWWIDTH                         (USP_ERR_DRV_IOPORT_START+3)

/***********************************************************
*                   ����ƽ̨Lcdģ�������                            *
***********************************************************/
#define USP_ERROR_DRV_LCD_PLDBASEADDR                            (USP_ERR_DRV_LCD_START+0)
#define USP_ERROR_DRV_LCD_PARAM                                        (USP_ERR_DRV_LCD_START+1)
#define USP_ERROR_DRV_LCD_IORWWIDTH                               (USP_ERR_DRV_LCD_START+2)

/***********************************************************
*                   ����ƽ̨Modemģ�������                       *
***********************************************************/
#define USP_ERR_DRV_MODEM_INIT_MODEM                            (USP_ERR_DRV_MODEM_START+0)
#define USP_ERR_DRV_MODEM_INIT_UART                               (USP_ERR_DRV_MODEM_START+1)
#define USP_ERR_DRV_MODEM_DIAL                                         (USP_ERR_DRV_MODEM_START+2)
#define USP_ERR_DRV_MODEM_DIALOFF                                   (USP_ERR_DRV_MODEM_START+3)
#define USP_ERR_DRV_MODEM_CLRRECVBUF                             (USP_ERR_DRV_MODEM_START+4)/*  ������ջ�����ʧ��*/
#define USP_ERR_DRV_MODEM_CLRSNDBUF                               (USP_ERR_DRV_MODEM_START+5)/*  ������ͻ�����ʧ��*/
#define USP_ERR_DRV_MODEM_READ                                         (USP_ERR_DRV_MODEM_START+6)/*  ��ʧ��*/
#define USP_ERR_DRV_MODEM_WRITE                                       (USP_ERR_DRV_MODEM_START+7)/*  дʧ��*/
#define USP_ERR_DRV_MODEM_RELEASE                                   (USP_ERR_DRV_MODEM_START+8)/*  ������ջ�����ʧ��*/
#define USP_ERR_DRV_MODEM_PARAM                                      (USP_ERR_DRV_MODEM_START+9)/*  ������ջ�����ʧ��*/
#define USP_ERR_DRV_MODEM_READY                                       (USP_ERR_DRV_MODEM_START+10)


/***********************************************************
*                   ����ƽ̨Keyboardģ�������                   *
***********************************************************/
#define USP_ERROR_DRV_KEYBOARD_OPEN                             (USP_ERR_DRV_KEYBOARD_START+0)
#define USP_ERROR_DRV_KEYBOARD_CLOSE                           (USP_ERR_DRV_KEYBOARD_START+1)
#define USP_ERROR_DRV_KEYBOARD_READ                             (USP_ERR_DRV_KEYBOARD_START+2)
#define USP_ERROR_DRV_KEYBOARD_READY                           (USP_ERR_DRV_KEYBOARD_START+3)

/***********************************************************
*                   ����ƽ̨EEPROMģ�������                   *
***********************************************************/
#define USP_ERR_DRV_EEPROM_INIT_FIRST          (USP_ERR_DRV_EEPROM_START+1)
#define USP_ERR_DRV_EEPROM_INIT_SECOND         (USP_ERR_DRV_EEPROM_START+2)
#define USP_ERR_DRV_EEPROM_INIT_ALL            (USP_ERR_DRV_EEPROM_START+3)
#define USP_ERR_DRV_EEPROM_SND_PARAM           (USP_ERR_DRV_EEPROM_START+4)
#define USP_ERR_DRV_EEPROM_RCV_PARAM           (USP_ERR_DRV_EEPROM_START+5)
#define USP_ERR_DRV_EEPROM_OFFSET              (USP_ERR_DRV_EEPROM_START+6)
#define USP_ERR_DRV_EEPROM_RCV_DATA            (USP_ERR_DRV_EEPROM_START+7)
#define USP_ERR_DRV_EEPROM_RCV_PARAM_SECOND    (USP_ERR_DRV_EEPROM_START+8)
#define USP_ERR_DRV_EEPROM_SND_PARAM_SECOND    (USP_ERR_DRV_EEPROM_START+9)
#define USP_ERR_DRV_EEPROM_SND_DATA            (USP_ERR_DRV_EEPROM_START+10)
#define USP_ERR_DRV_EEPROM_SND_WRITE_PAGE            (USP_ERR_DRV_EEPROM_START+11)


/***********************************************************
*                   ����ƽ̨FLASHģ�������                   *
***********************************************************/
#define USP_ERR_DRV_FLASH_OFFSET                                         (USP_ERR_DRV_FLASH_START+1)
#define USP_ERR_DRV_FLASH_MTDINDEX                                    (USP_ERR_DRV_FLASH_START+2)
#define USP_ERR_DRV_FLASH_MTDOPEN                                      (USP_ERR_DRV_FLASH_START+3)
#define USP_ERR_DRV_FLASH_READ                                            (USP_ERR_DRV_FLASH_START+4)
#define USP_ERR_DRV_FLASH_WRITE                                          (USP_ERR_DRV_FLASH_START+5)
#define USP_ERR_DRV_FLASH_ERASEMALLOC                              (USP_ERR_DRV_FLASH_START+6)
#define USP_ERR_DRV_FLASH_ERASE                                          (USP_ERR_DRV_FLASH_START+7)
#define USP_ERR_DRV_FLASH_NULL_POINTER                                   (USP_ERR_DRV_FLASH_START+8)
#define USP_ERR_DRV_FLASH_OPEN_FILE                                      (USP_ERR_DRV_FLASH_START+9)
#define USP_ERR_DRV_FLASH_READ_FILE                                      (USP_ERR_DRV_FLASH_START+10)
#define USP_ERR_DRV_FLASH_TYPE                                     (USP_ERR_DRV_FLASH_START+11)

/***********************************************************
*                   ����ƽ̨PRINTERCTLģ�������                   *
***********************************************************/

#define USP_ERR_DRV_PRINTER_NOTCOMPLETED                       (USP_ERR_DRV_PRINTER_START+1)
#define USP_ERR_DRV_PRINTER_LACKPAPER                               (USP_ERR_DRV_PRINTER_START+2)
#define USP_ERR_DRV_PRINTER_OVERHEAT                                 (USP_ERR_DRV_PRINTER_START+3)
#define USP_ERR_DRV_PRINTER_WRONGCMD                               (USP_ERR_DRV_PRINTER_START+4)
#define USP_ERR_DRV_PRINTER_CMD_SND                                   (USP_ERR_DRV_PRINTER_START+5)
#define USP_ERR_DRV_PRINTER_CMD_PARA                                 (USP_ERR_DRV_PRINTER_START+6)
#define USP_ERR_DRV_PRINTERCTL_BUF_NULL                             (USP_ERR_DRV_PRINTER_START+7)
#define USP_ERR_DRV_PRINTERCTL_BUF_LEN                               (USP_ERR_DRV_PRINTER_START+8)

/***********************************************************
*                   ����ƽ̨����ģ�������                   *
***********************************************************/

/* ���ڱ�Ŵ��ڴ������� */
#define  USP_ERR_DRV_UART_NUM                                                 (USP_ERR_DRV_UART_START+1)
/* ���Ѿ��򿪵Ĵ��� */
#define  USP_ERR_DRV_UART_EXIST                                                 (USP_ERR_DRV_UART_START+2)
/* δ֪ԭ���ʧ�� */
#define USP_ERR_DRV_UART_OPEN                                                 (USP_ERR_DRV_UART_START+3)
/* ��֧�ֵĲ����� */
#define USP_ERR_DRV_UART_BAUDRATE                                                 (USP_ERR_DRV_UART_START+4)
/* ��֧�ֵ�����λ */
#define USP_ERR_DRV_UART_DATABIT                                                 (USP_ERR_DRV_UART_START+5)
/* ��֧�ֵ���ż���� */
#define USP_ERR_DRV_UART_PARITY                                                 (USP_ERR_DRV_UART_START+6)
/* ��֧�ֵ�ֹͣλ */
#define USP_ERR_DRV_UART_STOPBIT                                                 (USP_ERR_DRV_UART_START+7)

/* ������������ȷ */
#define USP_ERR_DRV_UART_INDEX                                                 (USP_ERR_DRV_UART_START+8)

/* ˢ�´��ڻ��������� */
#define USP_ERR_DRV_UART_FLUSH                                                 (USP_ERR_DRV_UART_START+9)

/* ����Ĵ����жϺŴ��� */
#define USP_DRV_UART_IRQNUMBER_ERR                                                 (USP_ERR_DRV_UART_START+0x0A)

/* �رմ��ڴ��� */
#define USP_DRV_UART_CLOSE_ERR                                                 (USP_ERR_DRV_UART_START+0x0B)
/* ���մ������ݴ��� */
#define USP_DRV_UART_READ_ERR                                                 (USP_ERR_DRV_UART_START+0x0C)
/* ���ô������Դ���  */
#define USP_DRV_UART_SETATTR_ERR                                                  (USP_ERR_DRV_UART_START+0x0D)
/* ���ڷ������ݴ��� */
#define USP_DRV_UART_WRITE_ERR                                                 (USP_ERR_DRV_UART_START+0x0E)

/***********************************************************
*                   ����ƽ̨����������ģ�������                   *
***********************************************************/
#define USP_ERR_DRV_MCR_OPEN                                                   (USP_ERR_DRV_MCR_START+0x01)
#define USP_ERR_DRV_MCR_PARA                                                   (USP_ERR_DRV_MCR_START+0x02)
#define USP_ERR_DRV_MCR_READ                                                   (USP_ERR_DRV_MCR_START+0x03)
#define USP_ERR_DRV_MCR_DATA_INVALID                                   (USP_ERR_DRV_MCR_START+0x04)
#define USP_ERR_DRV_MCR_PARITY                                               (USP_ERR_DRV_MCR_START+0x05)
#define USP_ERR_DRV_MCR_CLOSE                                                 (USP_ERR_DRV_MCR_START+0x06)

/***********************************************************
*                   ����ƽ̨485����ģ�������                   *
***********************************************************/
#define USP_ERR_DRV_CTRL485_PARAM                                                (USP_ERR_DRV_CTRL485_START+0x01)


/***********************************************************
*                   ����ƽ̨�������ģ�������                   *
***********************************************************/
#define USP_ERR_DRV_CODEKBD_CMD_SND                                (USP_ERR_DRV_CODEKBD_START+0x01)
#define USP_ERR_DRV_CODEKBD_RES_RCV        				 (USP_ERR_DRV_CODEKBD_START+0x02)
#define USP_ERR_DRV_CODEKBD_CMD_PARA     				 (USP_ERR_DRV_CODEKBD_START+0x03)
#define USP_ERR_DRV_CODEKBD_TIMEOUT                                 (USP_ERR_DRV_CODEKBD_START+0x04)
#define USP_ERR_DRV_CODEKBD_TMKINVALID                           (USP_ERR_DRV_CODEKBD_START+0x05)
#define USP_ERR_DRV_CODEKBD_WPINVALID                             (USP_ERR_DRV_CODEKBD_START+0x06)
#define USP_ERR_DRV_CODEKBD_CMDTMKINVALID                    (USP_ERR_DRV_CODEKBD_START+0x07)
#define USP_ERR_DRV_CODEKBD_BATTERY                                 (USP_ERR_DRV_CODEKBD_START+0x08)
#define USP_ERR_DRV_CODEKBD_CMDBATTERY                          (USP_ERR_DRV_CODEKBD_START+0x09)
#define USP_ERR_DRV_CODEKBD_CMDINVALID                           (USP_ERR_DRV_CODEKBD_START+0x0A)
#define USP_ERR_DRV_CODEKBD_UNKNOWN                               (USP_ERR_DRV_CODEKBD_START+0x0B)
#define USP_ERR_DRV_CODEKBD_PWD_RCV                                (USP_ERR_DRV_CODEKBD_START+0x0C)
#define USP_ERR_DRV_CODEKBD_PWD_CANEL                            (USP_ERR_DRV_CODEKBD_START+0x0D)
#define USP_ERR_DRV_CODEKBD_ACTIVEWPCMD_SND               (USP_ERR_DRV_CODEKBD_START+0x0E)
#define USP_ERR_DRV_CODEKBD_ACTIVEWPRES_RCV                (USP_ERR_DRV_CODEKBD_START+0x0F)
#define USP_ERR_DRV_CODEKBD_SETALGPARACMD_SND            (USP_ERR_DRV_CODEKBD_START+0x10)
#define USP_ERR_DRV_CODEKBD_SETALGPARARES_RCV             (USP_ERR_DRV_CODEKBD_START+0x11)
#define USP_ERR_DRV_CODEKBD_SWITCHBZCMD_SND                (USP_ERR_DRV_CODEKBD_START+0x12)
#define USP_ERR_DRV_CODEKBD_SWITCHBZRES_RCV                 (USP_ERR_DRV_CODEKBD_START+0x13)
#define USP_ERR_DRV_CODEKBD_ENCRYPTCMD_SND                   (USP_ERR_DRV_CODEKBD_START+0x14)
#define USP_ERR_DRV_CODEKBD_ENCRYPTRES_RCV                   (USP_ERR_DRV_CODEKBD_START+0x15)
#define USP_ERR_DRV_CODEKBD_GETPINCMD_SND                     (USP_ERR_DRV_CODEKBD_START+0x16)
#define USP_ERR_DRV_CODEKBD_GETPINRES_RCV                      (USP_ERR_DRV_CODEKBD_START+0x17)
#define USP_ERR_DRV_CODEKBD_DECRYPTCMD_SND                   (USP_ERR_DRV_CODEKBD_START+0x18)
#define USP_ERR_DRV_CODEKBD_DECRYPTRES_RCV                   (USP_ERR_DRV_CODEKBD_START+0x19)
#define USP_ERR_DRV_CODEKBD_EXIT                                         (USP_ERR_DRV_CODEKBD_START+0x1A)
#define USP_ERR_DRV_CODEKBD_UART_TIMEOUT                       (USP_ERR_DRV_CODEKBD_START+0x1B)

/***********************************************************
*                   ����ƽ̨SYSDATAģ�������             *
***********************************************************/
#define USP_ERROR_OSS_SYSDATA_PARAERR                          (USP_ERR_OSS_SYSDATA_START+0)
#define USP_ERROR_OSS_SYSDATA_FLASH_READ                       (USP_ERR_OSS_SYSDATA_START+1)
#define USP_ERROR_OSS_SYSDATA_FLASH_WRITE                      (USP_ERR_OSS_SYSDATA_START+2)
#define USP_ERROR_OSS_SYSDATA_FLASH_ERASE                      (USP_ERR_OSS_SYSDATA_START+3)
#define USP_ERROR_OSS_SYSDATA_FAILED                           (USP_ERR_OSS_SYSDATA_START+4)
#define USP_ERROR_OSS_SYSDATA_FULL                             (USP_ERR_OSS_SYSDATA_START+5)
#define USP_ERROR_OSS_SYSDATA_INIT_MUTEX                       (USP_ERR_OSS_SYSDATA_START+6)

/***********************************************************
*   �汾����(VMP)������                                    *
***********************************************************/
#define USP_ERR_OAM_VMP_LOAD_APP_INFO            ((WORD32) (USP_ERR_OAM_VMP_BASE + 1))  /* ��ȡӦ�ó�����Ϣ���� */
#define USP_ERR_OAM_VMP_APP_VER_INCORRENT        ((WORD32) (USP_ERR_OAM_VMP_BASE + 2))  /* Ӧ�ó���汾����ȷ */
#define USP_ERR_OAM_VMP_APP_RUN_INFO             ((WORD32) (USP_ERR_OAM_VMP_BASE + 3))  /* û�п������е�Ӧ�ó���汾 */
#define USP_ERR_OAM_VMP_RECV_MSG                 ((WORD32) (USP_ERR_OAM_VMP_BASE + 4))  /* ������Ϣ���� */
#define USP_ERR_OAM_VMP_POWERON_ROUND            ((WORD32) (USP_ERR_OAM_VMP_BASE + 5))  /* �ϵ���Ϣ���ִκŲ���ȷ */
#define USP_ERR_OAM_VMP_STATUS                   ((WORD32) (USP_ERR_OAM_VMP_BASE + 6))  /* VMP����״̬���� */
#define USP_ERR_OAM_VMP_NULL_POINTER             ((WORD32) (USP_ERR_OAM_VMP_BASE + 7))  /* ��ָ�� */
#define USP_ERR_OAM_VMP_VER_CTRL_INFO_DESTROY    ((WORD32) (USP_ERR_OAM_VMP_BASE + 8))  /* �汾������Ϣ�� */
#define USP_ERR_OAM_VMP_UNKNOW_STORE_TYPE        ((WORD32) (USP_ERR_OAM_VMP_BASE + 9))  /* �汾�洢���ʹ��� */
#define USP_ERR_OAM_VMP_NO_RUN_VERSION           ((WORD32) (USP_ERR_OAM_VMP_BASE + 10)) /* û�п������еİ汾 */
#define USP_ERR_OAM_VMP_SET_VER_CTRL_INFO        ((WORD32) (USP_ERR_OAM_VMP_BASE + 11)) /* ���°汾������Ϣʧ�� */
#define USP_ERR_OAM_VMP_UNKNOW_MSG               ((WORD32) (USP_ERR_OAM_VMP_BASE + 12)) /* ��Ϣ�Ŵ��� */
#define USP_ERR_OAM_VMP_LLRP_DECODE              ((WORD32) (USP_ERR_OAM_VMP_BASE + 13)) /* LLRP������� */
#define USP_ERR_OAM_VMP_LLRP_ENCODE              ((WORD32) (USP_ERR_OAM_VMP_BASE + 14)) /* LLRP������� */
#define USP_ERR_OAM_VMP_LLRP_UNKNOW_MSG          ((WORD32) (USP_ERR_OAM_VMP_BASE + 15)) /* δ֪��LLRP��Ϣ */
#define USP_ERR_OAM_VMP_INVALID_MAIN_INDEX       ((WORD32) (USP_ERR_OAM_VMP_BASE + 16)) /* �Ƿ��İ汾��� */
#define USP_ERR_OAM_VMP_INVALID_PARAM            ((WORD32) (USP_ERR_OAM_VMP_BASE + 17)) /* �Ƿ��Ĳ��� */
#define USP_ERR_OAM_VMP_FTP_OPEN                 ((WORD32) (USP_ERR_OAM_VMP_BASE + 18)) /* ��FTP����ʧ�� */
#define USP_ERR_OAM_VMP_FTP_RECV                 ((WORD32) (USP_ERR_OAM_VMP_BASE + 19)) /* ����FTP����ʧ�� */
#define USP_ERR_OAM_VMP_FILE_OPEN                ((WORD32) (USP_ERR_OAM_VMP_BASE + 20)) /* ���ļ�ʧ�� */
#define USP_ERR_OAM_VMP_FILE_WRITE               ((WORD32) (USP_ERR_OAM_VMP_BASE + 21)) /* д�ļ�ʧ�� */
#define USP_ERR_OAM_VMP_FLASH_WRITE              ((WORD32) (USP_ERR_OAM_VMP_BASE + 22)) /* дFLASHʧ�� */
#define USP_ERR_OAM_VMP_LMT_UNKNOW_MSG           ((WORD32) (USP_ERR_OAM_VMP_BASE + 23)) /* δ֪��LMT��Ϣ */
#define USP_ERR_OAM_VMP_FPGA_LOAD                ((WORD32) (USP_ERR_OAM_VMP_BASE + 24)) /* ����FPGAʧ�� */
#define USP_ERR_OAM_VMP_READ_VERSION             ((WORD32) (USP_ERR_OAM_VMP_BASE + 25)) /* ���汾�ļ�ʧ�� */
#define USP_ERR_OAM_VMP_VERSION_DESTROY          ((WORD32) (USP_ERR_OAM_VMP_BASE + 26)) /* �汾�� */
#define USP_ERR_OAM_VMP_OPEN_UBOOT               ((WORD32) (USP_ERR_OAM_VMP_BASE + 27)) /* ��Uboot�������� */
#define USP_ERR_OAM_VMP_READ_UBOOT               ((WORD32) (USP_ERR_OAM_VMP_BASE + 28)) /* ��Uboot�������� */
#define USP_ERR_OAM_VMP_WRITE_UBOOT              ((WORD32) (USP_ERR_OAM_VMP_BASE + 29)) /* дUboot�������� */
#define USP_ERR_OAM_VMP_UBOOT_INFO               ((WORD32) (USP_ERR_OAM_VMP_BASE + 30)) /* Uboot�汾��Ϣ��ʽ���� */
#define USP_ERR_OAM_VMP_UNKNOW_CLI_CMD           ((WORD32) (USP_ERR_OAM_VMP_BASE + 31)) /* δ֪��Cli���� */
#define USP_ERR_OAM_VMP_BUF_TOO_SMALL            ((WORD32) (USP_ERR_OAM_VMP_BASE + 32)) /* ���ݻ�����̫С */
#define USP_ERR_OAM_VMP_STRING_FORMAT            ((WORD32) (USP_ERR_OAM_VMP_BASE + 33)) /* �ַ�����ʽ���� */
#define USP_ERR_OAM_VMP_NOT_SUPPORT_UPDATE_BOOT  ((WORD32) (USP_ERR_OAM_VMP_BASE + 34)) /* ��֧��BOOT���� */
#define USP_ERR_OAM_VMP_VERSION_INFO_STRING      ((WORD32) (USP_ERR_OAM_VMP_BASE + 35)) /* �汾��Ϣ�ַ�����ʽ���� */
#define USP_ERR_OAM_VMP_CRC_CHECK                ((WORD32) (USP_ERR_OAM_VMP_BASE + 36)) /* CRCУ����� */
#define USP_ERR_OAM_VMP_LOAD_CUST                ((WORD32) (USP_ERR_OAM_VMP_BASE + 37)) /* �����û��Զ���汾ʧ�� */
#define USP_ERR_OAM_VMP_CUST_NOT_EXIST           ((WORD32) (USP_ERR_OAM_VMP_BASE + 38)) /* �û��Զ���汾������ */
#define USP_ERR_OAM_VMP_VERSION_IS_RUN           ((WORD32) (USP_ERR_OAM_VMP_BASE + 39)) /* �汾�������� */
#define USP_ERR_OAM_VMP_VERSION_NOT_RUN          ((WORD32) (USP_ERR_OAM_VMP_BASE + 40)) /* �汾û������ */
#define USP_ERR_OAM_VMP_CLI_PARAM                ((WORD32) (USP_ERR_OAM_VMP_BASE + 41)) /* �����в������� */
#define USP_ERR_OAM_VMP_LLRP_PARAM               ((WORD32) (USP_ERR_OAM_VMP_BASE + 42)) /* LLRP�������� */
#define USP_ERR_OAM_VMP_FILE_SIZE                ((WORD32) (USP_ERR_OAM_VMP_BASE + 43)) /* �汾�ļ��ߴ���� */
#define USP_ERR_OAM_VMP_DATA_SIZE                ((WORD32) (USP_ERR_OAM_VMP_BASE + 44)) /* �汾����֡�ߴ���� */
#define USP_ERR_OAM_VMP_NOT_UPDATE_STAT          ((WORD32) (USP_ERR_OAM_VMP_BASE + 45)) /* û�д��ڰ汾����״̬ */
#define USP_ERR_OAM_VMP_FRAME_INDEX              ((WORD32) (USP_ERR_OAM_VMP_BASE + 46)) /* ����֡��Ų���ȷ */
#define USP_ERR_OAM_VMP_FRAME_UNKNOW_MSG         ((WORD32) (USP_ERR_OAM_VMP_BASE + 47)) /* δ֪����Ϣ */
#define USP_ERR_OAM_VMP_LOAD_VERSION             ((WORD32) (USP_ERR_OAM_VMP_BASE + 48)) /* �汾����ʧ�� */
#define USP_ERR_OAM_VMP_TFTP_OPEN                ((WORD32) (USP_ERR_OAM_VMP_BASE + 49)) /* ��TFTP����ʧ�� */
#define USP_ERR_OAM_VMP_SIZE_ZERO                ((WORD32) (USP_ERR_OAM_VMP_BASE + 50)) /* �汾��СΪ�� */
#define USP_ERR_OAM_VMP_NO_VER_CTRL_INFO         ((WORD32) (USP_ERR_OAM_VMP_BASE + 51)) /* û���ҵ��汾������Ϣ */
#define USP_ERR_OAM_VMP_ERASE_VER_CTRL_INFO      ((WORD32) (USP_ERR_OAM_VMP_BASE + 52)) /* �����汾������Ϣ���� */
#define USP_ERR_OAM_VMP_READ_VER_CTRL_INFO       ((WORD32) (USP_ERR_OAM_VMP_BASE + 53)) /* ���汾������Ϣ���� */
#define USP_ERR_OAM_VMP_WRITE_VER_CTRL_INFO      ((WORD32) (USP_ERR_OAM_VMP_BASE + 54)) /* д�汾������Ϣ���� */
#define USP_ERR_OAM_VMP_LOAD_SUB_DEV_VER         ((WORD32) (USP_ERR_OAM_VMP_BASE + 55)) /* �����Ӱ�汾ʧ�� */
#define USP_ERR_OAM_VMP_MANAGER_SUB_DEV_VER      ((WORD32) (USP_ERR_OAM_VMP_BASE + 56)) /* �Ӱ�汾����ʧ�� */
#define USP_ERR_OAM_VMP_FPGA_SIZE                ((WORD32) (USP_ERR_OAM_VMP_BASE + 57)) /* FPGA�ļ���С���� */
#define USP_ERR_OAM_VMP_FPGA_NAME                ((WORD32) (USP_ERR_OAM_VMP_BASE + 58)) /* FPGA�ļ������� */
#define USP_ERR_OAM_VMP_RECV_MSG_TIMEOUT         ((WORD32) (USP_ERR_OAM_VMP_BASE + 59)) /* ������Ϣ��ʱ */
#define USP_ERR_OAM_VMP_UNKNOW_DEV_MODLE         ((WORD32) (USP_ERR_OAM_VMP_BASE + 60)) /* δ֪���豸���� */
#define USP_ERR_OAM_VMP_ZTE_UNKNOW_MSG           ((WORD32) (USP_ERR_OAM_VMP_BASE + 61)) /* δ֪��ZTE˽����Ϣ */
#define USP_ERR_OAM_VMP_MALLOC_HANDLE            ((WORD32) (USP_ERR_OAM_VMP_BASE + 62)) /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_ERR_OAM_VMP_UNKNOW_VER_TYPE          ((WORD32) (USP_ERR_OAM_VMP_BASE + 63)) /* δ֪�İ汾���� */
#define USP_ERR_OAM_VMP_SSCANF                   ((WORD32) (USP_ERR_OAM_VMP_BASE + 64)) /* �����ַ������� */

/***********************************************************
*                   ����ƽ̨�¶ȴ�����ģ�������                   *
***********************************************************/
#define USP_ERR_DRV_TSENSOR_INIT_FIRST      (USP_ERR_DRV_TSENSOR_START+1)
#define USP_ERR_DRV_TSENSOR_INIT_SECOND     (USP_ERR_DRV_TSENSOR_START+2)
#define USP_ERR_DRV_TSENSOR_INIT_ALL        (USP_ERR_DRV_TSENSOR_START+3)
#define USP_ERR_DRV_TSENSOR_SND_PARAM       (USP_ERR_DRV_TSENSOR_START+4)
#define USP_ERR_DRV_TSENSOR_RCV_PARAM       (USP_ERR_DRV_TSENSOR_START+5)
#define USP_ERR_DRV_TSENSOR_OFFSET          (USP_ERR_DRV_TSENSOR_START+6)
#define USP_ERR_DRV_TSENSOR_POR_STATE       (USP_ERR_DRV_TSENSOR_START+7)
#define USP_ERR_DRV_TSENSOR_CONFIG          (USP_ERR_DRV_TSENSOR_START+8)
#define USP_ERR_DRV_TSENSOR_FUN_NUM         (USP_ERR_DRV_TSENSOR_START+9)
#define USP_ERR_DRV_TSENSOR_POR_CONFIG      (USP_ERR_DRV_TSENSOR_START+10)
#define USP_ERR_DRV_TSENSOR_POR_CONVER      (USP_ERR_DRV_TSENSOR_START+11)
#define USP_ERR_DRV_TSENSOR_CONVER          (USP_ERR_DRV_TSENSOR_START+12)
/***********************************************************
*                   ����ƽ̨Τ��ģ�������                   *
***********************************************************/
#define USP_ERR_DRV_WG_NUM                                        (USP_ERR_DRV_WG_START+1)
#define USP_ERR_DRV_WG_OPEN                                      (USP_ERR_DRV_WG_START+2)
#define USP_ERR_DRV_WG_INDEX                                    (USP_ERR_DRV_WG_START+3)
#define USP_ERR_DRV_WG_IOCTL                                    (USP_ERR_DRV_WG_START+4)
#define USP_ERR_DRV_WG_READ                                      (USP_ERR_DRV_WG_START+5)
#define USP_ERR_DRV_WG_CLOSE                                    (USP_ERR_DRV_WG_START+6)

/***********************************************************
*                   ����ƽ̨GPRSģ�������                   *
***********************************************************/
#define USP_ERR_DRV_GPRS_PARA                                   (USP_ERR_DRV_GPRS_START+0x01)
#define USP_ERR_DRV_GPRS_CMD        				            (USP_ERR_DRV_GPRS_START+0x02)
#define USP_ERR_DRV_GPRS_RES_NOTCOMPLETE     			        (USP_ERR_DRV_GPRS_START+0x03)
#define USP_ERR_DRV_GPRS_RES_PARA                               (USP_ERR_DRV_GPRS_START+0x04)
#define USP_ERR_DRV_GPRS_BUF_FULL                               (USP_ERR_DRV_GPRS_START+0x05)
#define USP_ERR_DRV_GPRS_TIMEOUT                                (USP_ERR_DRV_GPRS_START+0x06)
#define USP_ERR_DRV_GPRS_CTRL_POWER                             (USP_ERR_DRV_GPRS_START+0x07)
#define USP_ERR_DRV_GPRS_CTRL_ONOFF                             (USP_ERR_DRV_GPRS_START+0x08)


/***********************************************************
*                   ����ƽ̨ioexpanderģ�������                   *
***********************************************************/
#define USP_ERR_DRV_IOE_INIT               (USP_ERR_DRV_IOE_START + 0x1)  /*��ʼ������*/
#define USP_ERR_DRV_IOE_SND_ADDR           (USP_ERR_DRV_IOE_START + 0x2)  /*���ͺ�����ȡ��ַ����*/
#define USP_ERR_DRV_IOE_SND_START          (USP_ERR_DRV_IOE_START + 0x3)  /*���ͺ���������ʼλ����*/
#define USP_ERR_DRV_IOE_SND_ACK            (USP_ERR_DRV_IOE_START + 0x4)  /*���ͺ����ȴ�Ӧ��λ����*/
#define USP_ERR_DRV_IOE_RCV_ADDR           (USP_ERR_DRV_IOE_START + 0x5)  /*���պ�����ȡ��ַ����*/
#define USP_ERR_DRV_IOE_RCV_START          (USP_ERR_DRV_IOE_START + 0x6)  /*���պ���������ʼλ����*/
#define USP_ERR_DRV_IOE_RCV_ACK            (USP_ERR_DRV_IOE_START + 0x7)  /*���պ����ȴ�Ӧ��λ����*/
/***********************************************************
*			����ƽ̨timerģ�������		    *
***********************************************************/
#define USP_ERR_DRV_TIMER_INIT_FD          (USP_ERR_DRV_TIMER_START + 0x1)  /*��ʼ���������ļ�����������*/
#define USP_ERR_DRV_TIMER_INIT_OPEN        (USP_ERR_DRV_TIMER_START + 0x2)  /*��ʼ��������open��������*/
#define USP_ERR_DRV_TIMER_INIT_MMAP        (USP_ERR_DRV_TIMER_START + 0x3)  /*��ʼ��������mmap��������*/
#define USP_ERR_DRV_TIMER_CONTROL_PARA     (USP_ERR_DRV_TIMER_START + 0x4)  /*���ƺ�����������*/
/***********************************************************
*			����ƽ̨GET_SYSTEM_INFOģ�������  		    * 
***********************************************************/
#define USP_ERROR_DRV_GET_SYSTEM_INFO_OPEN (USP_ERR_DRV_GET_SYSTEM_INFO_START + 0x1)  /*��ʼ���������ļ���ʧ��*/
#define USP_ERROR_DRV_GET_JIFFIES          (USP_ERR_DRV_GET_SYSTEM_INFO_START + 0x2)  /*��ȡJIFFIESʧ��*/
#define USP_ERROR_DRV_GET_HZ               (USP_ERR_DRV_GET_SYSTEM_INFO_START + 0x3)  /*��ȡHZʧ��*/
#define USP_ERROR_DRV_PAR_ERROR            (USP_ERR_DRV_GET_SYSTEM_INFO_START + 0x4)  /*��δ���*/
/***********************************************************
*			����ƽ̨clockgen ģ�������  		    * 
***********************************************************/
#define USP_ERROR_DRV_CLOCKGEN_CONFIG_TYPE  (USP_ERR_DRV_CLOCKGEN_START + 0x1)  /**/
#define USP_ERROR_DRV_CLOCKGEN_WRITE_REG    (USP_ERR_DRV_CLOCKGEN_START + 0x2)
#define USP_ERROR_DRV_CLOCKGEN_WRITE        (USP_ERR_DRV_CLOCKGEN_START + 0x3)
#define USP_ERROR_DRV_CLOCKGEN_READ_REG     (USP_ERR_DRV_CLOCKGEN_START + 0x4)
#define USP_ERROR_DRV_CLOCKGEN_CLOCK        (USP_ERR_DRV_CLOCKGEN_START + 0x5)
#define USP_ERROR_DRV_CLOCKGEN_UNLOCK       (USP_ERR_DRV_CLOCKGEN_START + 0x6)

/***********************************************************
*			����ƽ̨Variant SPI ģ�������  		    * 
***********************************************************/
#define USP_ERR_DRV_VARSPI_PARAM            (USP_ERR_DRV_VARSPI_START + 0x1)
#define USP_ERR_DRV_VARSPI_TIMEOUT          (USP_ERR_DRV_VARSPI_START + 0x2)
#define USP_ERR_DRV_VARSPI_OPERATION        (USP_ERR_DRV_VARSPI_START + 0x3)
#define USP_ERR_DRV_VARSPI_FUNC_ID          (USP_ERR_DRV_VARSPI_START + 0x4)

/***********************************************************
*			����ƽ̨Relay ģ�������  		    * 
***********************************************************/
#define USP_ERR_DRV_RELAY_PARAM             (USP_ERR_DRV_RELAY_START + 0x1)

/***********************************************************
*			 		MPI ģ�������						          *
***********************************************************/
#define USP_ERR_MPI_MALLOC_HANDLE			(USP_MPI_ERR_BASE + 1)	/* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_ERR_MPI_INVALID_TNO             (USP_MPI_ERR_BASE + 2)	/* ��ȡ�ű������ʧ�� */
#define USP_ERR_MPI_RFTEST          		(USP_MPI_ERR_BASE + 3)	/* 433M �źż��ʧ�� */
#define USP_ERR_MPI_CCTEST          		(USP_MPI_ERR_BASE + 4)	/* CC1101 ���ʧ�� */
#define USP_ERR_MPI_SENDMSGTORF				(USP_MPI_ERR_BASE + 5)	/* MPI ���ְ���������Ƶ��������Ϣʧ�� */
#define USP_ERR_MPI_RECVMSGFROMRF           (USP_MPI_ERR_BASE + 6)	/* MPI ���ְ��������Ƶ����������Ϣʧ�� */
#define USP_ERR_MPI_SENDMSGACK				(USP_MPI_ERR_BASE + 7)	/* MPI ���ְ�������ű�����Ϣʧ�� */
#define USP_ERR_MPI_RFBSPINIT				(USP_MPI_ERR_BASE + 8)	/* ��Ƶ��Ӳ����ʼ��ʧ�� */
#define USP_ERR_MPI_POWEROUTRANGE			(USP_MPI_ERR_BASE + 9)	/* ��Ƶ���ʳ���Χ*/
#define USP_ERR_MPI_AUTOCALIBRATE			(USP_MPI_ERR_BASE + 10)	/* ��Ƶ������У׼ʧ�� */
#define USP_ERR_MPI_READSYSINFODATA			(USP_MPI_ERR_BASE + 11)	/* ��ȡ��Ƶ��������ʧ�� */
#define USP_ERR_MPI_BSPCCINIT          		(USP_MPI_ERR_BASE + 12)	/* CC1101 ��ʼ��ʧ�� */
#define USP_ERR_MPI_RXCC_RX_OVERTIME        (USP_MPI_ERR_BASE + 13)	/* ������оƬ��������֡ʧ�� */
#define USP_ERR_MPI_DECCC_RX_OVERTIME       (USP_MPI_ERR_BASE + 14)	/* ��У׼оƬ��������֡ʧ�� */
#define USP_ERR_MPI_INVALID_RETURNINFO		(USP_MPI_ERR_BASE + 15)	/* ��Ч�ķ�����Ϣ */
#define USP_ERR_MPI_RFHARDWARE              (USP_MPI_ERR_BASE + 16)	/* ��ƵӲ������ */
#define USP_ERR_MPI_LIB_OPENRF              (USP_MPI_ERR_BASE + 17)	/* ��̬�����Ƶʧ�� */
#define MPI_SHELL_ERROR_PSAM_ACTIVE         (USP_MPI_ERR_BASE + 18)	/* ��̬�⼤��PSAM ��ʧ�� */

/***********************************************************
*   ��չPSAM�������������                                 *
***********************************************************/
#define USP_OSS_PSAM_EXT_ERR_UNKNOW_MSG     ((WORD32) (USP_ERR_OSS_PSAM_EXT_BASE + 1)) /* δ֪����Ϣ */
#define USP_OSS_PSAM_EXT_ERR_CMD_ACK        ((WORD32) (USP_ERR_OSS_PSAM_EXT_BASE + 2)) /* �����PSAM�������Ӧ����Ϣ */
#define USP_OSS_PSAM_EXT_ERR_KEEP_ALIVE     ((WORD32) (USP_ERR_OSS_PSAM_EXT_BASE + 3)) /* �����������Ϣ */
#define USP_OSS_PSAM_EXT_ERR_ACTIVE_ACK     ((WORD32) (USP_ERR_OSS_PSAM_EXT_BASE + 4)) /* �����PSAM�������Ӧ����Ϣ */


/***********************************************************
*   GPIO��Relayģ�����������                                 *
***********************************************************/
#define  USP_GPIO_ERR_DECODE_MSG            ((WORD32) (USP_GPIO_ERR_BASE + 1)) /* ����ʧ��*/
#define  USP_GPIO_ERR_SEND_MSG              ((WORD32) (USP_GPIO_ERR_BASE + 2)) /* ������Ϣʧ��*/
#define  USP_GPIO_UNKOWN_MSG                ((WORD32) (USP_GPIO_ERR_BASE + 3)) /* δ֪��Ϣ*/
#define  USP_GPIO_ERR_INVALID_PARA          ((WORD32) (USP_GPIO_ERR_BASE + 4)) /* �Ƿ��Ĳ���*/
#define  USP_GPIO_ERR_MALLOC_HANDLE         ((WORD32) (USP_GPIO_ERR_BASE + 5)) /* ��ȡ��̬�����ڴ�ʧ�� */ 
#define  USP_GPIO_ERR_INVALID_TNO           ((WORD32) (USP_GPIO_ERR_BASE + 6)) /* ���������� */
#define  USP_GPIO_ERR_INTERRUPT_PORT        ((WORD32) (USP_GPIO_ERR_BASE + 7)) /* ������ж϶˿ں�*/

/***********************************************************
*   �������������                                         *
***********************************************************/
#define USP_ERR_OSS_MISC_MALLOC_HANDLE      ((WORD32) (USP_ERR_OSS_MISC_BASE + 1)) /* ��ȡ��̬�����ڴ�ʧ�� */
#define USP_ERR_OSS_MISC_INVALID_PARAM      ((WORD32) (USP_ERR_OSS_MISC_BASE + 2)) /* �Ƿ��Ĳ��� */


#endif  /* _USP_ERROR_DEF_H */


