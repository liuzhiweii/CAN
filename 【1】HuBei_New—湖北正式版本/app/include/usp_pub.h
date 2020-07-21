/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_pub.h
* �ļ���ʶ�� 
* ����ժҪ�� ���幫������(��Ҫ���¹滮)
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2009.03.10
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_PUB_H
#define _USP_PUB_H

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
#define USP_SUCCESS         (0)
#define ERROR               (-1)

/* �����ӡ��Ϣ���� */
#define	USP_OSS_PRINTF_LEVEL_ALERT 	    ((BYTE)(0x02))	/* �쳣��Ϣ */
#define	USP_OSS_PRINTF_LEVEL_ERROR 		((BYTE)(0x04))	/* ������Ϣ */
#define	USP_OSS_PRINTF_LEVEL_WARNING 	((BYTE)(0x08))	/* �澯��Ϣ */
#define	USP_OSS_PRINTF_LEVEL_NOTICE 	((BYTE)(0x10))	/* ��Ҫ��ʾ��Ϣ */
#define	USP_OSS_PRINTF_LEVEL_INFO 		((BYTE)(0x20))	/* һ����ʾ��Ϣ */
#define	USP_OSS_PRINTF_LEVEL_DEBUG 		((BYTE)(0x40))	/* ������Ϣ */

/* ����Linux����Ϣ���С��������͹����ڴ��ֵ���� */
#define USP_OSS_TIMER_MUTEX_KEY          (1000)  /* ��ʱ��ģ�飬��ʱ�����滥���ź������� */
#define USP_OSS_SYSDATA_MUTEX_KEY        (9100)  /* ϵͳ�����������ź��� */
#define USP_DRV_I2C_MUTEX1_KEY           (9101)  /* I2C��д������ */
#define USP_DRV_I2C_MUTEX2_KEY           (9102)  /* I2C��д������ */
#define USP_OSS_ST_MUTEX_KEY             (9103)  /* ��������ڴ�������� */
#define USP_OSS_PRINTF_MUTEX_KEY         (9104)  /* ��ӡ���ƻ����� */
#define USP_OSS_LED_MUTEX_KEY            (9105)  /* ���ƹ������� */
#define USP_OSS_SCP_CUST_MUTEX_KEY       (9106)  /* �û��Զ�������������� */
#define USP_OSS_MPI_RF_CHECK_MUTEX_KEY   (9107)  /* ��������Ƶ��⻥���� */
#define USP_OSS_GPIO_MUTEX_KEY           (9108)  /* GPIO�������� */
#define USP_VIR_TAG_MUTEX_KEY            (9109)  /* �����ǩ������ */

#define USP_OSS_SCP_TCB_SHM_KEY          (9200)  /* ������ƿ鹲���ڴ��ֵ */
#define USP_OSS_SCP_WTD_CTRL_SHM_KEY     (9201)  /* ���Ź����ƹ����ڴ��ֵ */
#define USP_OSS_PRINTF_CTRL_SHM_KEY      (9202)  /* ��ӡ���ƹ����ڴ��ֵ */
#define USP_OSS_ST_CTRL_SHM_KEY          (9203)  /* ������ٿ�����Ϣ�����ڴ��ֵ */
#define USP_OSS_ST_BUF_SHM_KEY           (9204)  /* ������ٴ洢�����ڴ��ֵ */
#define USP_OSS_PRINTF_BUF_SHM_KEY       (9205)  /* ��ӡ���ƴ洢�����ڴ��ֵ */
#define USP_IDTE_SHM_KEY                 (9206)  /* IDTE�����ڴ��ֵ */
#define USP_OSS_LED_SHM_KEY              (9207)  /* ���ƹ������ڴ��ֵ */
#define USP_PUB_PSP_SHM_KEY              (9208)  /* ����ͳ�ƹ����ڴ��ֵ */
#define USP_OSS_TIMER_SHM_KEY            (1500)  /* ��ʱ��ģ�鹲���ڴ��ֵ */
#define USP_VIR_TAG_SHM_KEY              (9209)  /* �����ǩģ�鹲���ڴ�*/
#define USP_VIR_TAG_SHM1_KEY             (9210)  /* �����ǩģ�鹲���ڴ�*/

#define USP_OAM_CLI_MSG_KEY              (9001)  /* �����н���(CLI)��Ϣ���йؼ��� */
#define USP_OAM_LAP_MSG_KEY              (9002)  /* ��־���쳣̽��(LAP)��Ϣ���йؼ��� */
#define USP_COMM_MSG_KEY                 (9003)  /* ͨ��ģ��(COMM)��Ϣ���йؼ��� */
#define USP_OSS_SCP_MSG_KEY              (9004)  /* ϵͳ����(SCP)��Ϣ���йؼ��� */
#define USP_COMM_IPD_MSG_KEY             (9005)  /* ip���ֹ���ʹ�� */
#define USP_OAM_CMP_MSG_KEY              (9006)  /* ���ù���(CMP)��Ϣ���йؼ��� */
#define USP_COMM_HOLD_MSG_KEY            (9007)  /* �������洦��ģ��(COMM_HOLD)��Ϣ���йؼ��� */
#define USP_OSS_ST_MSG_KEY               (9008)  /* �������ģ��(ST)��Ϣ���йؼ��� */
#define USP_OAM_VMP_MSG_KEY              (9009)  /* �汾����(VMP)��Ϣ���йؼ��� */
#define USP_OAM_DMP_MSG_KEY              (9010)  /* ��ϲ���(DMP)��Ϣ���йؼ��� */
#define USP_OSS_SMP_MSG_KEY	             (9011)  /* ״̬���(SMP)��Ϣ���йؼ��� */
#define USP_IDTE_SUB_MSG_KEY             (9012)  /* ���ɲ��Թ���(IDTE)����������Ϣ���йؼ��� */
#define USP_IDTE_CASE_MSG_KEY            (9013)  /* ���ɲ��Թ���(IDTE)����������Ϣ���йؼ��� */
#define USP_OAM_AMP_MSG_KEY              (9014)  /* �澯����(AMP)��Ϣ���йؼ��� */
#define USP_UHF_PROTOCOL_KEY             (9015)  /* ����ƵЭ�鴦����Ϣ���йؼ��� */
#define USP_UHF_RF_KEY                   (9016)  /* ����Ƶ��Ƶ������Ϣ���йؼ��� */
#define USP_ETC_PROTOCOL_KEY             (9017)  /* ETC Э�鴦����Ϣ���йؼ��� */
#define USP_ETC_RF_KEY                   (9018)  /* ETC ��Ƶ������Ϣ���йؼ��� */
#define USP_UHF_RDP_RULE_KEY             (9019)  /* UHF �����·���Ϣ���йؼ��� */
#define USP_UHF_RDP_REPORT_KEY           (9020)  /* UHF ���洦����Ϣ���йؼ��� */
#define USP_UHF_RDP_LLRP_KEY             (9021)  /* UHF ��������Ϣ���йؼ��� */
#define USP_MPI_PROTOCOL_KEY             (9022)  /* MPI Э�鴦����Ϣ���йؼ��� */
#define USP_MPI_RF_KEY      	         (9023)  /* MPI ��Ƶ������Ϣ���йؼ��� */
#define USP_OSS_PSAM_EXT_MSG_KEY         (9024)  /* ��չPSAM����������Ϣ���йؼ��� */
#define USP_OSS_GPIO_MSG_KEY             (9025)  /* GPIO����������Ϣ���йؼ��� */
#define USP_OSS_MISC_MSG_KEY             (9026)  /* �����������Ϣ���йؼ��� */
#define USP_VIR_TAG_MSG_KEY              (9027)  /* �����ǩ������Ϣ���йؼ��� */
#define USP_COMM_SMS_MSG_KEY             (9028)  /* ����Ϣģ����Ϣ���йؼ��� */

#define USP_CUST_MSG_KEY_START           (9051)  /* �û��Զ���������Ϣ������ʼ�ؼ��� */

/* ������Ϣ��ʽ */
#define USP_PUB_MSG_FORMAT_LLRP               (0)     /* LLRP��Ϣ��ʽ */
#define USP_PUB_MSG_FORMAT_STRUCT             (1)     /* �ṹ������Ϣ��ʽ */

/* �м���Ϣħ���� */
#define USP_PUB_MSG_RELAY                     (0x5A5A)

/* ǰ̨�ڲ�ʹ�õ��豸���� */
#define USP_PUB_DEV_MODEL_COMMON              (0xFFFFFFFF)     /* ͨ�� */
#define USP_PUB_DEV_MODEL_READER_UHF          (0x1)            /* ����Ƶ�Ķ��� */
#define USP_PUB_DEV_MODEL_READER_ETC          (0x2)            /* ETC�Ķ��� */
#define USP_PUB_DEV_MODEL_READER_MPI          (0x4)            /* �������Ķ��� */
#define USP_PUB_DEV_MODEL_HANDSET_UHF         (0x8)            /* ����Ƶ�ֳֻ� */
#define USP_PUB_DEV_MODEL_HANDSET_ETC         (0x10)           /* ETC�ֳֻ� */
#define USP_PUB_DEV_MODEL_UNKNOW              (0x80000000)     /* δ֪ */

/* ���ܽӿ����� */
#define USP_PUB_OAM_IF_TYPE_LLRP              0                /* LLRPЭ�� */
#define USP_PUB_OAM_IF_TYPE_ZTE               1                /* ZTEЭ�� */

/* ����ṹ�У�ĳ����ǰ�ĳ��� */
#define usp_pub_getStructFixedLen(structType, member)   /*lint -save -e413 */ (WORD16)((WORD32)((BYTE *)&(((structType *)0)->member))) /*lint -restore */

/* �ָ��������õĲ������� */
#define USP_PUB_SET_DEFAULT_ITEM        "set_default"

/* CPU���Ͷ��� */
#define USP_CPU_MPC852        (0)
#define USP_CPU_MPC8241       (1)
#define USP_CPU_MPC8306       (2)
#define USP_CPU_MPC8313       (3)
#define USP_CPU_LPC2138       (4)
#define USP_CPU_LPC2148       (5)
#define USP_CPU_LPC2468       (6)

/* LINUX�ں˰汾���� */
#define USP_OS_LINUX_15       (0) /*linux 2.6.15�汾*/
#define USP_OS_LINUX_38       (1) /*linux 2.6.38�汾*/


/* Ӳ�����Ͷ��� */
#define USP_HD_EC00_BOARD0    (0) /*EC00�ϰ�*/
#define USP_HD_6601_BOARD0    (1) /*6601�ϰ�*/
#define USP_HD_8801_BOARD0    (2) /*8801�ϰ�*/
#define USP_HD_8700_BOARD0    (3) /*8700�ϰ�*/
#define USP_HD_8900_BOARD0    (4) /*8900B   ��*/
#define USP_HD_8900_BOARD1    (5) /*8900RUP ��*/
#define USP_HD_6700_BOARD0    (6) /*6700�ϰ�*/
#define USP_HD_6700_BOARD1    (7) /*6700С�İ�*/
#define USP_HD_6700_BOARD2_D  (8) /*6700RUP�����ְ�*/
#define USP_HD_6700_BOARD2_R  (9) /*6700RUP����Ƶ��*/
#define USP_HD_8700_BOARD1_R  (10)/*8700RUP����Ƶ��*/

/*OS���Ͷ��� */
#define OS_LINUX              (0) /*LINUX*/
#define OS_FREERTOS           (1) /*FREERTOS*/
#define OS_UBOOT              (2) /*UBOOT*/

/*T_IOType ���Ͷ��� */
#define DRV_EPLD              (0) /*EPLD*/
#define DRV_IOPORT            (1) /*IOPORT*/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
/* CPU�ڽ��̼�ͨ����Ϣͷ */
typedef struct tagT_Msg
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
}T_Msg;

/* ϵͳ������Ϣ */
typedef struct tagT_USPPubConfig
{
    WORD16      wDevID;             /* �豸�� */
    BYTE        ucIsSetDefault;     /* �Ƿ�ָ��������� */
    BYTE        aucSerialNum[9];    /* �������к� */
    BYTE        aucMacAddress[6];   /* MAC��ַ */
    BYTE        ucBigEndian;        /* ����ֽ���? */
    BYTE        ucIsDistributed;    /* �Ƿ��Ƿֲ�ʽϵͳ */
    BYTE        ucIsDebug;          /* �Ƿ��ǵ��԰汾 */
    BYTE        ucOamIfType;        /* ���ܽӿ����� */    
    BYTE        aucPad[2];
    WORD16      wDevType;           /* �豸�ͺ� */
    WORD16      wDevSpec;           /* �豸��� */
    WORD32      dwDevModel;         /* ǰ̨�ڲ�ʹ�õ��豸���� */
    WORD32      dwVersion;          /* �汾�� */
    WORD32      dwNet1Addr;         /* �ڶ���������IP��ַ */
}T_USPPubConfig;

/* ����ͳ����Ϣ���ݽṹ */
typedef struct tagT_USPPubStatisticsConf
{
    char        *pcName;        /* ���� */
    WORD32      dwDevModel;     /* ���õĻ��� */
}T_USPPubStatisticsConf;

typedef union tagT_USPPubStatisticsInfo
{
    WORD32      adwVal[64];     /* ͳ��ֵ */
}T_USPPubStatisticsInfo;

/* I2C, SPIģ��ɹ��� */
#if 0
typedef volatile enum tagT_IOType
{
	    DRV_EPLD = 0,   /* epld�ܽ�ģ�� */
	    DRV_IOPORT		/* io��ģ�� */
}T_IOType;
#endif
typedef BYTE T_IOType ;

typedef struct tagT_USPDrvIO
{
    WORD32 data_mask;   /* ���ݼĴ������� */
    WORD32 dir_mask;    /* ����Ĵ������� */
    WORD32 data_reg;    /* ���ݼĴ��� */
    WORD32 dir_reg;     /* ����Ĵ��� */
}T_USPDrvIO;

/* ������� ���������ֿ��Թ��� */
typedef enum tagT_DrvIODirc
{
	    DRV_OUT=0x1,
	    DRV_IN=0
} T_DrvIODirc;



/* IO�źţ��������ֿ��Թ��� */




/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/* ϵͳ������Ϣ */
extern T_USPPubConfig           g_tUSPPubConfig;

/* ����ͳ��������Ϣ */
extern T_USPPubStatisticsConf   g_atUSPPubStatisticsConf[];

/* ����ͳ����Ϣ */
extern T_USPPubStatisticsInfo  *g_ptUSPPubStatisticsInfo;

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
WORD32 usp_pub_countCRC32(BYTE * pucData, WORD32 dwLen);
WORD16 usp_pub_countCRC16(BYTE *pucData, WORD16 wLength, WORD16 wPreCRC16);
extern WORD32 usp_pub_init(VOID);
#endif  /*_USP_PUB_H */

