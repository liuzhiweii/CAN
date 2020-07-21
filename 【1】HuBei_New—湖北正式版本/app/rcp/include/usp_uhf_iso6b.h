
#ifndef _USP_UHF_ISO6B_H
#define _USP_UHF_ISO6B_H

/***********************ISO6BЭ����ؽӿ�**************************/
#define ISO6B_RES_COMM        \
    WORD16 wProtoType; \
    WORD16 wAntennaIndex;   \
    WORD16 wChannelIndex; \
    WORD16 wRssi;

/*���ĸ������Ӧ6B����ֵ��ǰ�ĸ���������֤�����
   ���ؽṹ����ǿ��ת�����˽ṹ�����ڷ���ʱ��һ��
   �����ĺ�������*/
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
} T_USPUHFISO6BResComm;

typedef struct
{
    BYTE   ucCommandType;
    BYTE   ucAddress;
    BYTE   ucMask;
    BYTE   ucMaskLen;
    BYTE   aucMaskData[8];
    /* �䳤 Mask value,from 0~8 byte in length. ������wWordDataLen�ֶξ���, MSB first */
} T_USPUHFISO6BInventoryMask;

typedef struct
{
    WORD16 wModeIndex;
    WORD16 wEstimateTag;
    T_USPUHFISO6BInventoryMask tISO6BInventoryMask;
} T_USPUHFISO6BInventoryPara;

typedef struct
{
    WORD32 dwAntIndex;
    T_USPUHFISO6BInventoryPara tISO6BInventoryPara;
    T_USPUHFInventRfPara          tInventoryRfPara;
} T_USPUHFISO6BInventory;


typedef struct
{
    WORD16 wTagDataLen;
	WORD16 wPad;
    /* �䳤 UID ���ݵĵ�һ�����ݶΣ�������wUID_bits_Len
                                                                  �ֶξ���,Ŀǰ����Ϊ64bit   */
    BYTE aucTagData[8];
} T_USPUHFISO6BTagId;


/* ����ǩӦ����Ϣ */
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
    WORD16 wInventoryFinish;
    /* ����Ƿ������־, 0��δ������1��������*/
    WORD16 wResult;      /* ����Ϣ�еı�ǩ��Ŀ�����ߴ�����  */
	
    T_USPUHFISO6BTagId tTagId;  /* �䳤��ǩ���� */

} T_USPUHFISO6BInventoryRes;

typedef struct
{
    BYTE ucCommandType;
    BYTE ucRetryTimes;
    WORD16 wPad;
} T_USPUHFISO6BReadCtl;

typedef struct
{   
    WORD32 dwAntIndex;
    BYTE ucBytePtr;
    BYTE ucByteCount;
    WORD16 wPad;
    T_USPUHFISO6BReadCtl tISO6BReadctl;
    T_USPUHFISO6BTagId   tTagId;
} T_USPUHFISO6BRead;


/*ISO ����ǩӦ�� */
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
    WORD16 wResult;              /* ���������0���ɹ�������Error Code */
    WORD16 wResultLength;        /* ��ȡ���ȣ�����ʱ��0 */
    BYTE   aucReadData[0];          /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BReadRes;



typedef struct
{
    BYTE ucCommandType;
    BYTE ucRetryTimes;
    BYTE ucVerifyTimes;
    BYTE ucPad;
} T_USPUHFISO6BWriteCtl;

typedef struct
{
    WORD32 dwAntIndex;
    BYTE ucBytePtr;
    BYTE ucByteCount;
	WORD16 wPad;
    BYTE aucWriteData[256];  /*Ҫд�������*/
    T_USPUHFISO6BWriteCtl tISO6BWriteCtl;
    T_USPUHFISO6BTagId    tTagId;
} T_USPUHFISO6BWrite;



/* д��ǩ������Ӧ */
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
    WORD16 wResultLength;          /* ��ȡ���ȣ�����ʱ��0 */
    WORD16 wResult;                    /* ���������0���ɹ�������Error Code */
} T_USPUHFISO6BWriteRes;


typedef struct
{
    BYTE ucRetryTimes;
    BYTE ucVerifyTimes;
    WORD16 wPad;
} T_USPUHFISO6BLockCtl;

typedef struct
{
    WORD32 dwAntIndex;
    BYTE ucBytePtr;
    BYTE ucByteCount;
	WORD16 wPad;
    T_USPUHFISO6BLockCtl tISO6BLockCtl;
    T_USPUHFISO6BTagId   tTagId;
} T_USPUHFISO6BLock;

/* ����ǩӦ�� */
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
    WORD16 wResultLength;          /* ��ȡ���ȣ�����ʱ��0 */
    WORD16 wResult;                     /* ���������0���ɹ�������Error Code*/
    BYTE   aucLockData[0];        /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BLockRes;


typedef struct
{
    BYTE ucRetryTimes;
    BYTE ucPad;
    WORD16 wPad;
} T_USPUHFISO6BLockQueryCtl;

typedef struct
{
    WORD32 dwAntIndex;
    BYTE ucBytePtr;
    BYTE ucByteCount;
	WORD16 wPad;
    T_USPUHFISO6BLockQueryCtl tISO6BLockQueryCtl;
    T_USPUHFISO6BTagId        tTagId;
} T_USPUHFISO6BLockQuery;


/* ����ѯӦ�� */
typedef struct
{
    WORD16 wProtoType;      
    WORD16 wAntennaIndex;         
    WORD16 wChannelIndex; 
    WORD16 wRssi;
    WORD16 wResult;         /* ���������0���ɹ�������Error Code */
    WORD16 wResultLength;         /* �����Ϣ���� */
    BYTE   aucLockQueryData[0];   /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BLockQueryRes;


WORD32 usp_uhf_iso6b_inventory(T_USPUHFISO6BInventory*ptISO6BInventoryPara,
                                T_USPUHFISO6BInventoryRes *ptISO6BInventoryRes);
WORD32 usp_uhf_iso6b_read(T_USPUHFISO6BRead *ptISO6BReadPara,
                                T_USPUHFISO6BReadRes *ptISO6BReadRes);
WORD32 usp_uhf_iso6b_lockquery(T_USPUHFISO6BLockQuery *ptISO6BLockQueryPara,
                                   T_USPUHFISO6BLockQueryRes *ptISO6BLockQueryRes);
WORD32 usp_uhf_iso6b_write(T_USPUHFISO6BWrite *ptISO6BWritePara,
                                       T_USPUHFISO6BWriteRes *ptISO6BWriteRes);
WORD32 usp_uhf_iso6b_gettag(T_USPUHFISO6BInventoryRes *ptISO6BInventoryRes);
WORD32 usp_uhf_iso6b_lock(T_USPUHFISO6BLock *ptISO6BLockPara,
                                      T_USPUHFISO6BLockRes *ptISO6BLockRes);



#endif


