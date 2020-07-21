
#ifndef _USP_UHF_ISO6B_H
#define _USP_UHF_ISO6B_H

/***********************ISO6BЭ����ؽӿ�**************************/
#define ISO6B_RES_COMM        \
    unsigned short int wProtoType; \
    unsigned short int wAntennaIndex;   \
    unsigned short int wChannelIndex; \
    unsigned short int wRssi;

/*���ĸ������Ӧ6B����ֵ��ǰ�ĸ���������֤�����
   ���ؽṹ����ǿ��ת�����˽ṹ�����ڷ���ʱ��һ��
   �����ĺ�������*/
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
} T_USPUHFISO6BResComm;

typedef struct
{
    unsigned char   ucCommandType;
    unsigned char   ucAddress;
    unsigned char   ucMask;
    unsigned char   ucMaskLen;
    unsigned char   aucMaskData[8];
    /* �䳤 Mask value,from 0~8 byte in length. ������wWordDataLen�ֶξ���, MSB first */
} T_USPUHFISO6BInventoryMask;

typedef struct
{
    unsigned short int wModeIndex;
    unsigned short int wEstimateTag;
    T_USPUHFISO6BInventoryMask tISO6BInventoryMask;
} T_USPUHFISO6BInventoryPara;

typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFISO6BInventoryPara tISO6BInventoryPara;
    T_USPUHFInventRfPara          tInventoryRfPara;
} T_USPUHFISO6BInventory;


typedef struct
{
    unsigned short int wTagDataLen;
	unsigned short int wPad;
    /* �䳤 UID ���ݵĵ�һ�����ݶΣ�������wUID_bits_Len
                                                                  �ֶξ���,Ŀǰ����Ϊ64bit   */
    unsigned char aucTagData[8];
} T_USPUHFISO6BTagId;


/* ����ǩӦ����Ϣ */
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
    unsigned short int wInventoryFinish;
    /* ����Ƿ������־, 0��δ������1��������*/
    unsigned short int wResult;      /* ����Ϣ�еı�ǩ��Ŀ�����ߴ�����  */
	
    T_USPUHFISO6BTagId tTagId;  /* �䳤��ǩ���� */

} T_USPUHFISO6BInventoryRes;

typedef struct
{
    unsigned char ucCommandType;
    unsigned char ucRetryTimes;
    unsigned short int wPad;
} T_USPUHFISO6BReadCtl;

typedef struct
{   
    unsigned int dwAntIndex;
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
    unsigned short int wPad;
    T_USPUHFISO6BReadCtl tISO6BReadctl;
    T_USPUHFISO6BTagId   tTagId;
} T_USPUHFISO6BRead;


/*ISO ����ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
    unsigned short int wResult;              /* ���������0���ɹ�������Error Code */
    unsigned short int wResultLength;        /* ��ȡ���ȣ�����ʱ��0 */
    unsigned char   aucReadData[0];          /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BReadRes;



typedef struct
{
    unsigned char ucCommandType;
    unsigned char ucRetryTimes;
    unsigned char ucVerifyTimes;
    unsigned char ucPad;
} T_USPUHFISO6BWriteCtl;

typedef struct
{
    unsigned int dwAntIndex;
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
	unsigned short int wPad;
    unsigned char aucWriteData[256];  /*Ҫд�������*/
    T_USPUHFISO6BWriteCtl tISO6BWriteCtl;
    T_USPUHFISO6BTagId    tTagId;
} T_USPUHFISO6BWrite;



/* д��ǩ������Ӧ */
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
    unsigned short int wResultLength;          /* ��ȡ���ȣ�����ʱ��0 */
    unsigned short int wResult;                    /* ���������0���ɹ�������Error Code */
} T_USPUHFISO6BWriteRes;


typedef struct
{
    unsigned char ucRetryTimes;
    unsigned char ucVerifyTimes;
    unsigned short int wPad;
} T_USPUHFISO6BLockCtl;

typedef struct
{
    unsigned int dwAntIndex;
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
	unsigned short int wPad;
    T_USPUHFISO6BLockCtl tISO6BLockCtl;
    T_USPUHFISO6BTagId   tTagId;
} T_USPUHFISO6BLock;

/* ����ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
    unsigned short int wResultLength;          /* ��ȡ���ȣ�����ʱ��0 */
    unsigned short int wResult;                     /* ���������0���ɹ�������Error Code*/
    unsigned char   aucLockData[0];        /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BLockRes;


typedef struct
{
    unsigned char ucRetryTimes;
    unsigned char ucPad;
    unsigned short int wPad;
} T_USPUHFISO6BLockQueryCtl;

typedef struct
{
    unsigned int dwAntIndex;
    unsigned char ucBytePtr;
    unsigned char ucByteCount;
	unsigned short int wPad;
    T_USPUHFISO6BLockQueryCtl tISO6BLockQueryCtl;
    T_USPUHFISO6BTagId        tTagId;
} T_USPUHFISO6BLockQuery;


/* ����ѯӦ�� */
typedef struct
{
    unsigned short int wProtoType;      
    unsigned short int wAntennaIndex;         
    unsigned short int wChannelIndex; 
    unsigned short int wRssi;
    unsigned short int wResult;         /* ���������0���ɹ�������Error Code */
    unsigned short int wResultLength;         /* �����Ϣ���� */
    unsigned char   aucLockQueryData[0];   /* �䳤��ǩ���ݵĵ�һ�������ֶ� */
} T_USPUHFISO6BLockQueryRes;


unsigned int usp_uhf_iso6b_inventory(T_USPUHFISO6BInventory*ptISO6BInventoryPara,
                                T_USPUHFISO6BInventoryRes *ptISO6BInventoryRes);
unsigned int usp_uhf_iso6b_read(T_USPUHFISO6BRead *ptISO6BReadPara,
                                T_USPUHFISO6BReadRes *ptISO6BReadRes);
unsigned int usp_uhf_iso6b_lockquery(T_USPUHFISO6BLockQuery *ptISO6BLockQueryPara,
                                   T_USPUHFISO6BLockQueryRes *ptISO6BLockQueryRes);
unsigned int usp_uhf_iso6b_write(T_USPUHFISO6BWrite *ptISO6BWritePara,
                                       T_USPUHFISO6BWriteRes *ptISO6BWriteRes);
unsigned int usp_uhf_iso6b_gettag(T_USPUHFISO6BInventoryRes *ptISO6BInventoryRes);
unsigned int usp_uhf_iso6b_lock(T_USPUHFISO6BLock *ptISO6BLockPara,
                                      T_USPUHFISO6BLockRes *ptISO6BLockRes);



#endif

