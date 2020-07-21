#ifndef _USP_ISO6B_H
#define _USP_ISO6B_H


#define ISO6B_READLENGTH     (8)
#define ISO6B_BYTE_UNLOCKED  (0)
#define ISO6B_BYTE_LOCKED    (1)

#define ISO6B_CMD_TYPE_READ_8BYTE       (1)
#define ISO6B_CMD_TYPE_READ_VRBLBYTE    (2)
#define ISO6B_CMD_TYPE_LOCK             (3)
#define ISO6B_CMD_TYPE_WRITE_BYTE       (4)
#define ISO6B_CMD_TYPE_WRITE_4BYTE      (5)
#define ISO6B_CMD_TYPE_LOCKQRY          (6)

/* FPGA��ISO18000-6B frame struct */
/* ISO18000-6BЭ���ϱ���CPU��Ϣ�����ݽṹ
��1�������Ϣ��֡
     ֡��ʽ��֡ͷ/֡����/��Ϣ����/UID
��2������������Ӧ��Ϣ��֡
     ֡��ʽ��֡ͷ/֡����/��Ϣ����/��������/UID/���մ���״ָ̬ʾ/��ǩ���ص���Ϣֵ
��3����������Ϣ
     ֡��ʽ��֡ͷ/֡����/��Ϣ����
 */
#define  FPGA_ISO6B_FRAME_LEN         (0)  /* ֡����          */
#define  FPGA_ISO6B_FRAME_MSG_TYPE    (1)  /* ��Ϣ����        */
#define  FPGA_ISO6B_FRAME_CMD_TYPE    (2)  /*             */
#define  FPGA_ISO6B_FRAME_UID         (3)  /* UID            */
#define  FPGA_ISO6B_FRAME_RESULT      (7)  /* ���մ���״ָ̬ʾ */
#define  FPGA_ISO6B_FRAME_TAG_DATA    (8)  /* ��ǩ���ص���Ϣֵ */



#define  FPGA_ISO6B_INVENT_FRAME_LEN                       (0)  /* ֡����          */
#define  FPGA_ISO6B_INVENT_FRAME_MSG_TYPE             (1)  /* ��Ϣ����        */
#define  FPGA_ISO6B_INVENT_FRAME_UID    (2)  /* UID            */
#define  FPGA_ISO6B_INVENT_FRAME_RESULT    (6)  /*  */
#define  FPGA_ISO6B_INVENT_FRAME_DATAREAD_DATA (7)
#define  FPGA_ISO6B_INVENT_FRAME_RSSI   (11)  /*�����������RSSI��ֵ*/


/* ISO18000-6BЭ����Ϣ���ͣ�BIT1~BIT0
01�������Ϣ
10������������Ϣ
11����������Ϣ
*/
#define  FPGA_ISO6B_FRAME_CMD_TYPE_INVENT_MSG         (0x1)
#define  FPGA_ISO6B_FRAME_CMD_TYPE_ACCESS_MSG         (0x2)
#define  FPGA_ISO6B_FRAME_CMD_TYPE_INVENT_FINISH_MSG  (0x3)

/* ISO18000-6BЭ����մ���״ָ̬ʾ��BIT3~BIT0
0000���޴���
0001�����ź�
0010��ǰ���벻ƥ��
0100��CRCУ���
1000:     ��ǩ�ڲ�����
 */
#define  FPGA_ISO6B_FRAME_RESULT_NO_EEROR             (0x0)
#define  FPGA_ISO6B_FRAME_RESULT_NO_SIGNAL            (0x1)
#define  FPGA_ISO6B_FRAME_RESULT_PREAMBLE_NOT_MATCH   (0x2)
#define  FPGA_ISO6B_FRAME_RESULT_CRC_ERROR            (0x4)
#define  FPGA_ISO6B_FRAME_RESULT_TAG_ERROR              (0x8)


/* ISO18000-6Bʹ�õĳ������� */
#define T_ISO18000_6_B_RCPTagInventoryReponse_fixed_bytes_len (14)  /* �����Ӧ��Ϣͷ�̶������ֽڳ��� */
#define T_ISO18000_6_B_RCPTagReadResponse_fixed_bytes_len      (12)  /* ����ǩ��Ӧ��Ϣͷ�̶������ֽڳ��� */
#define T_ISO18000_6_B_RCPTagWriteResponse_fixed_bytes_len     (10)  /* д��ǩ��Ӧ��Ϣͷ�̶������ֽڳ��� */

#define ISO_180006B_UID_BYTE_LEN            (8)    /* UID���ֽڳ��� */
#define ISO_180006B_READ_RESPONSE_BYTE_LEN  (8)    /* READ��Ӧ���ֽڳ��� */

#define ISO_18000_6B_BUFFER_MAX_TAG_NUM   (256)  /* UID�����ܴ�����ǩ��*/
#define ISO_18000_6B_MAX_TAG_SIZE    (256)  /* 18000_6B��ǩ���ռ� */
#define ISO_18000_6B_TAG_HEAD  (18)  /* 18000_6B��ǩ���û�����С*/
#define ISO_18000_6B_ZTE_ID       (192)   /* ���˱�ʶ*/
#define ISO_18000_6B_ID_POS  (12) /* ���˱�ʶ��iso��ǩ�е�λ��*/
#define ISO_18000_6B_USERDATA_SIZE_POS  (13) /*  �û�������Ч����*/

/*�˴���Ӧ����д��Ӧ��CommandType*/

#define ISO_6B_WRITEBYTE      (0)
#define ISO_6B_WRITE4BYTE    (1)

#define ISO6B_BUFFER_SIZE   256

/*************************************************************************
  *                      	                   �ļ��ڲ�ʹ�õ���������                               *                           *
**************************************************************************/
typedef WORD32(*ISORetry)(WORD16 , BYTE , BYTE *);


/*ISO6B ��ǩ����ṹ*/
typedef struct
{
    WORD16  wRSSI;
    WORD16  wPad;
    BYTE aucUID[ISO_180006B_UID_BYTE_LEN];
} T_ISO6BTagElement;

typedef struct
{
    WORD16 wDataIndex;
    WORD16 wDataLen;
    T_ISO6BTagElement tISO6BTags[ISO6B_BUFFER_SIZE];
} T_ISO6BTagBuffer;



typedef struct
{
    WORD32 dwModeIdentifier;
    WORD32 dwFowardLinkDataRate;
    BYTE ucDelimiters;
}
T_ISO6BUHFRFModeTableEntry;

typedef struct
{
    WORD16 wModeIndex;
    WORD16 wFowardLinkDataRate;
    WORD16 wReverseLinkDataRate;
    WORD16 wModDepth;
    WORD16 wT1_value;
    WORD16 wdelimeters;
    WORD16 wDataReadStartAddr;        
    /*ISO6B���ʱ���͵�DataRead�������ʼ��ַ*/
    WORD16 wLastTagRssi;

    BYTE   aucUIDforAccess[8];           /* ��дISO��ǩ��UIDΪ*/

    WORD16 wReadStartAddr;               /* ��ISO��ǩ��ʼ��ַ*/
    WORD16 wReadBytesLen;                /* length (in Bytes) */
    WORD16 wReadCommandType;

    
    WORD16 wWriteStartAddr;              /* дISO��ǩ��ʼ��ַ*/
    WORD16 wWriteBytesLen;               /* дISO��ǩ����*/
    WORD16 wWriteCommandType;
    BYTE   aucWriteData[256];             /* Ҫд�������*/
    BYTE   ucByteMask;                       /* д��������*/
    BYTE   ucPad;
    WORD16 wVerifyTimes;

    WORD16 wLockStartAddr;
    WORD16 wLockBytesLen;

    WORD16 wLockQueryStartAddr;
    WORD16 wLockQueryBytesLen;

    WORD16 wRetryTimes;
    WORD16 wSuccessByteLen;

} T_ISO6BCtl;


WORD32 uhf_ISO6B_Proc_Invnt_Resp(void);
WORD32 uhf_ISO6B_Proc_OprtCmd_Rsp(BYTE ucCmd, WORD16 wStartAddr, BYTE ucLen,BYTE *aucOptBuf);


#endif


