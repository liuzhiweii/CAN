
#ifndef _USP_UHF_EPC_H
#define _USP_UHF_EPC_H

/***********************EPCЭ����ؽӿ�**************************/
#define EPC_BLOCK_WRITE  (1)
#define EPC_WORD_WRITE   (0)


#define EPC_MAX_MASK_WORD_LEN (32)
#define EPC_MAX_MASK_BYTE_LEN (64)

/*EPC Acees ����*/
typedef struct
{
    unsigned short int wTagWordLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
	unsigned short int wPad;
    unsigned char   aucTagData[0];      /* �䳤 EPC ���ݵĵ�һ�����ݶ� */
} T_USPUHFEpcTagId;

typedef enum
{
    EPC_Action_Select_Unselect = 0,
    EPC_Action_Select_DoNothing = 1,
    EPC_Action_DoNothing_Unselect = 2,
    EPC_Action_Unselect_DoNothing = 3,
    EPC_Action_Unselect_Select = 4,
    EPC_Action_DoNothing_Select = 5,
} EPCSelectAction;

typedef enum UHF_EEPCDRValue
{

    EPC_DRValue_DRV_8 = 0,
    EPC_DRValue_DRV_64_3 = 1,
} E_USPUHFEpcDRValue;

typedef enum LLRP_EEPCMValue
{

    EPC_MValue_MV_FM0 = 0,
    EPC_MValue_MV_2 = 1,
    EPC_MValue_MV_4 = 2,
    EPC_MValue_MV_8 = 3,
} E_USPUHFEpcMValue;


typedef enum LLRP_EpcForwardLinkModulation
{

    EPC_ForwardLinkModulation_PR_ASK = 0,
    EPC_C1G2ForwardLinkModulation_SSB_ASK = 1,
    EPC_C1G2ForwardLinkModulation_DSB_ASK = 2,
} E_USPUHFEEpcForwardLinkModulation;


typedef enum LLRP_EEPCpectralMaskIndicator
{

    EPC_SpectralMaskIndicator_Unknown = 0,
    EPC_SpectralMaskIndicator_SI = 1,
    EPC_SpectralMaskIndicator_MI = 2,
    EPC_SpectralMaskIndicator_DI = 3,
} E_USPUHFEPCpectralMaskIndicator;

typedef struct
{
    unsigned int dwModeIdentifier;
    E_USPUHFEpcDRValue tDRValue;
    unsigned char ucEPCHAGTCConformance;
    E_USPUHFEpcMValue eMValue;
    E_USPUHFEEpcForwardLinkModulation eForwardLinkModulation;
    E_USPUHFEPCpectralMaskIndicator    eSpectralMaskIndicator;
    unsigned int dwBDRValue;
    unsigned int dwPIEValue;
    unsigned int dwMinTariValue;
    unsigned int dwMaxTariValue;
    unsigned int dwStepTariValue;
} T_USPUHFEpcRFModeTableEntry;


typedef struct
{
    unsigned short int            wMemBank;
    /* 00: RFU�� 01: EPC�� 10: TID�� 11: User */
    unsigned short int            wMaskLength;        /* Mask length (bits) */
    unsigned short int            wMaskBitPointer;    /* Starting Mask bit address */
    unsigned short int            wPad;
    EPCSelectAction   eSelectAction;
    unsigned char                ucMaskData[EPC_MAX_MASK_BYTE_LEN];
    /* �䳤 Mask value,�õ�һ���ֶ� */
} T_USPUHFEpcInventoryMask;


typedef struct
{
    unsigned short int wMaskNum;
    unsigned short int wPad;
    T_USPUHFEpcInventoryMask tEpcInventoryMask[EPC_SETLECT_MASK_NUM];
} T_USPUHFEpcInventoryMaskList;

typedef struct
{
    unsigned short int wEstimatedTagNum;
    unsigned short int wSession;
    unsigned short int wModeindex;
    unsigned short int wPad;
    unsigned int dwRcvSenses;
    unsigned int dwUhfBitCommand;
    T_USPUHFEpcInventoryMaskList tEpcInventoryMaskList;
} T_USPUHFEpcInventoryPara;

typedef struct
{
    unsigned char   ucAntennaID;            /* ���߱�� */
    unsigned char   ucProtoType;           /*Э������û��*/
    unsigned short int wPad;
    T_USPUHFEpcInventoryPara tEpcInventoryPara;
    T_USPUHFInventRfPara     tInventoryRfPara;
} T_USPUHFEpcInventory;


/* ����ǩӦ����Ϣ */
typedef struct
{
    unsigned short int wProtoType;          /* Э������ */
    unsigned short int wAntennaIndex;       /* ���߱�� */
    unsigned short int wPC;               /*�ϱ�PCֵ*/
    unsigned short int wCRC;                /*Ŀǰ�߼���֧�֣���0xFF*/
    unsigned short int wRssi;
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned int wQueryCollisionCount;
    unsigned int wQueryNoReplyCount;
    unsigned short int wInventoryFinish;   /* ����Ƿ������־, 0��δ���� �� 1������ */
    unsigned short int wResult;            /* 0 -��ȷ ���������� */
    T_USPUHFEpcTagId tTagId;     /* �䳤��ǩ���� */
} T_USPUHFEpcInventoryRes;



typedef struct
{
    unsigned char   ucMB;
    unsigned char   ucPad;
    unsigned short int wWordPointer;
    unsigned short int wWordCount;
	unsigned short int wPad;
    unsigned int dwAccessPassword;
    unsigned int dwUhfBitCommand;
} T_USPUHFEpcReadPara;

typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFEpcReadPara tEpcReadPara;
    T_USPUHFEpcTagId    tTagId;
} T_USPUHFEpcRead;



/* ����ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;        /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;            /* ���������0���ɹ������� ��ErrorCode*/
    unsigned short int wResultLength;          /* �����Ϣ���������ݳ��� ��ʧ��ʱΪ0  */
    unsigned short int awReadData[0];          /* �䳤��ǩ���� */
} T_USPUHFEpcReadRes;



typedef struct
{
    unsigned char   ucMB;
    unsigned char   ucWriteType;   /*1 ʹ�ÿ�д 0 ��ͨд*/
    unsigned short int wWordPointer;    
    unsigned short int wWriteWordLen;
    unsigned short int wPad;
    unsigned int dwAccessPassword;
    unsigned int dwUhfBitCommand;
    unsigned char   aucData[64];  /*�䳤д������*/
} T_USPUHFEpcWritePara;

typedef struct
{  
    unsigned int dwAntIndex;
    T_USPUHFEpcWritePara tEpcWritePara;
    T_USPUHFEpcTagId     tTagId;
} T_USPUHFEpcWrite;


/* д��ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;         /*Ƶ�ʺ�*/
    unsigned short int wResult;               /* ���������0���ɹ��������������� */
    unsigned short int wResultLength;        /* �����Ϣ�����سɹ������ݳ���  */
} T_USPUHFEpcWriteRes;

typedef struct
{
   unsigned short int wDataProfile;
   unsigned short int wAccessRange;
   unsigned short int wPersistence;
   unsigned short int wReadOrWrite;
   unsigned int dwAccessPassword;
   unsigned int dwUhfBitCommand;
}T_USPUHFEpcIPJQTPara;


typedef struct
{  
    unsigned int dwAntIndex;
    T_USPUHFEpcIPJQTPara tEpcIPJQTPara;
    T_USPUHFEpcTagId     tTagId;
} T_USPUHFEpcIPJQT;


typedef struct
{
    unsigned short int wProtoType;          /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;       /* ���߱��0 ~ 3 */
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;             /*QT ����Ķ�����*/
    unsigned short int wDataProfile;        /*��ǩģʽ 1��˽��ģʽ 2������ģʽ*/
    unsigned short int wAccessRange;         /*�Ƿ���Short_range����1���ر�   2������*/
    unsigned short int wReadOrWrite;        /*0 �Ƕ�1��д*/
    unsigned short int wRssi;;
}T_USPUHFEpcIPJQTRes;


typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;         /*Ƶ�ʺ�*/
} CommonRes;

typedef enum E_USPEpcLockPrivilege
{
    Epc_LockPrivilege_Read_Write = 0,
    Epc_LockPrivilege_Perma_Lock = 1,
    Epc_LockPrivilege_Perma_Unlock = 2,
    Epc_LockPrivilege_Unlock = 3,
} E_EpcLockPrivilege;


typedef enum E_USPEpcLockDataField
{

    Epc_LockDataField_Kill_Password = 0,
    Epc_LockDataField_Access_Password = 1,
    Epc_LockDataField_EPC_Memory = 2,
    Epc_LockDataField_TID_Memory = 3,
    Epc_LockDataField_User_Memory = 4,
} E_EpcLockDataField;


typedef struct
{
    E_EpcLockPrivilege Privilege;
    E_EpcLockDataField DataField;
} T_USPUHFEpcLockPayload;

typedef struct
{
    unsigned short int wPayLoadNum;
	unsigned short int wPad;
    T_USPUHFEpcLockPayload taEpcLockPayLoad[5];
} T_USPUHFEpcLockPayloadList;

typedef struct
{
    unsigned int dwAccessPassword;
    unsigned int dwUhfBitCommand;
    T_USPUHFEpcLockPayloadList tEpcLockPayLoad;   /*���ͽ��������Ķ�����*/
} T_USPUHFEPcLockPara;


typedef struct
{   
    unsigned int dwAntIndex;
    T_USPUHFEPcLockPara tEpcLockPara;
    T_USPUHFEpcTagId    tTagId;   /*�䳤EPC��*/
} T_USPUHFEpcLock;

/* ����ǩ����Ӧ�� */
typedef struct
{
    unsigned short int wProtoType;    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6
Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;               /* ���������0���ɹ��������������� */
	unsigned short int wPad;
} T_USPUHFEpcLockRes;


typedef struct
{
    unsigned int dwKillPassword;
    unsigned int dwUhfBitCommand;
} T_USPUHFEpcKillPara;


typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFEpcKillPara tEpcKillPara;
    T_USPUHFEpcTagId    tTagId;   /*�䳤EPC��*/
} T_USPUHFEpcKill;


/* ɱ����ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;           /* ���������0���ɹ������� ������ */
    unsigned short int wPad;
} T_USPUHFEpcKillRes;


typedef struct
{
    unsigned char   ucMB;
    unsigned char   ucReadLock;
    unsigned short int wOpSpecID;
    unsigned short int ucRFU;
    unsigned short int wBlockPtr;
    unsigned short int wBlockRange;
    unsigned short int wPad;
    unsigned int dwAccessPassword;
    unsigned int dwUhfBitCommand;
    unsigned char   ucMask[512];
} T_USPUHFEpcBlockLockPara;



typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFEpcBlockLockPara tEpcBlockLockPara;
    T_USPUHFEpcTagId         tTagId;
} T_USPUHFEpcBlockLock;

typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;               /* ���������0���ɹ��������������� */
    unsigned short int wRssi;
    unsigned char     ucReadLock;
    unsigned char     ucPad;
    unsigned short int wLockStateLen;   /*�ֽڳ���*/
    unsigned short int wPad;
    unsigned char   ucLockState[512];
} T_USPUHFEpcBlockLockRes;


typedef struct
{
    unsigned char     ucMask;
    unsigned char     ucPad;
    unsigned short int   wPad;
    unsigned int   dwAccessPassword;
    unsigned int   dwUhfBitCommand;
} T_USPUHFEpcReadLockPara;

/*ALEN ר�нӿڶ�����*/
typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFEpcReadLockPara   tEpcReadLockPara;
    T_USPUHFEpcTagId          tTagId;        /* ��ǩ��ʶ���� */
} T_USPUHFEpcReadLock;



typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;       /*Ƶ�ʺ�*/
    unsigned short int wResult;  /*�������Ľ��*/
} T_USPUHFEpcReadLockRes;


void usp_uhf_epc_inventory(T_USPUHFEpcInventory*ptEpcInventoryPara,
                                  T_USPUHFEpcInventoryRes*ptEpcTagInventoryResp);
void usp_uhf_epc_read(T_USPUHFEpcRead *ptEpcReadPara,T_USPUHFEpcReadRes *ptEPCReadResp);
void usp_uhf_epc_write(T_USPUHFEpcWrite *ptEpcWritePara,T_USPUHFEpcWriteRes *ptEPCWriteResp);
void usp_uhf_epc_lock(T_USPUHFEpcLock *ptEpcLockPara,T_USPUHFEpcLockRes *ptEPCLockResp);
void usp_uhf_epc_kill(T_USPUHFEpcKill *ptEpcKillPara,T_USPUHFEpcKillRes *ptEPCKillResp);
void usp_uhf_epc_blocklock(T_USPUHFEpcBlockLock *ptEpcBlockLockPara,
                                    T_USPUHFEpcBlockLockRes *ptEPCBlockLockResp);
void usp_uhf_epc_readlock(T_USPUHFEpcReadLock *ptEpcReadLock,
                                   T_USPUHFEpcReadLockRes *ptEPCReadLockResp);
void usp_uhf_epc_query(T_USPUHFEpcInventoryRes*ptEpcTagInventoryResp);
void usp_uhf_epc_ipjqt(T_USPUHFEpcIPJQT *ptEpcIPJQT,
                              T_USPUHFEpcIPJQTRes *ptEpcIPJQTResp);



#endif




