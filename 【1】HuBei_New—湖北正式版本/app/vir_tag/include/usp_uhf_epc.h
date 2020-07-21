
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
    WORD16 wTagWordLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
	WORD16 wPad;
    BYTE   aucTagData[0];      /* �䳤 EPC ���ݵĵ�һ�����ݶ� */
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
    WORD32 dwModeIdentifier;
    E_USPUHFEpcDRValue tDRValue;
    BYTE ucEPCHAGTCConformance;
    E_USPUHFEpcMValue eMValue;
    E_USPUHFEEpcForwardLinkModulation eForwardLinkModulation;
    E_USPUHFEPCpectralMaskIndicator    eSpectralMaskIndicator;
    WORD32 dwBDRValue;
    WORD32 dwPIEValue;
    WORD32 dwMinTariValue;
    WORD32 dwMaxTariValue;
    WORD32 dwStepTariValue;
} T_USPUHFEpcRFModeTableEntry;


typedef struct
{
    WORD16            wMemBank;
    /* 00: RFU�� 01: EPC�� 10: TID�� 11: User */
    WORD16            wMaskLength;        /* Mask length (bits) */
    WORD16            wMaskBitPointer;    /* Starting Mask bit address */
    WORD16            wPad;
    EPCSelectAction   eSelectAction;
    BYTE                ucMaskData[EPC_MAX_MASK_BYTE_LEN];
    /* �䳤 Mask value,�õ�һ���ֶ� */
} T_USPUHFEpcInventoryMask;


typedef struct
{
    WORD16 wMaskNum;
    WORD16 wPad;
    T_USPUHFEpcInventoryMask tEpcInventoryMask[EPC_SETLECT_MASK_NUM];
} T_USPUHFEpcInventoryMaskList;

typedef struct
{
    WORD16 wEstimatedTagNum;
    WORD16 wSession;
    WORD16 wModeindex;
    WORD16 wPad;
    WORD32 dwRcvSenses;
    WORD32 dwUhfBitCommand;
    T_USPUHFEpcInventoryMaskList tEpcInventoryMaskList;
} T_USPUHFEpcInventoryPara;

typedef struct
{
    BYTE   ucAntennaID;            /* ���߱�� */
    BYTE   ucProtoType;           /*Э������û��*/
    WORD16 wPad;
    T_USPUHFEpcInventoryPara tEpcInventoryPara;
    T_USPUHFInventRfPara     tInventoryRfPara;
} T_USPUHFEpcInventory;


/* ����ǩӦ����Ϣ */
typedef struct
{
    WORD16 wProtoType;          /* Э������ */
    WORD16 wAntennaIndex;       /* ���߱�� */
    WORD16 wPC;               /*�ϱ�PCֵ*/
    WORD16 wCRC;                /*Ŀǰ�߼���֧�֣���0xFF*/
    WORD16 wRssi;
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD32 wQueryCollisionCount;
    WORD32 wQueryNoReplyCount;
    WORD16 wInventoryFinish;   /* ����Ƿ������־, 0��δ���� �� 1������ */
    WORD16 wResult;            /* 0 -��ȷ ���������� */
    T_USPUHFEpcTagId tTagId;     /* �䳤��ǩ���� */
} T_USPUHFEpcInventoryRes;



typedef struct
{
    BYTE   ucMB;
    BYTE   ucPad;
    WORD16 wWordPointer;
    WORD16 wWordCount;
	WORD16 wPad;
    WORD32 dwAccessPassword;
    WORD32 dwUhfBitCommand;
} T_USPUHFEpcReadPara;

typedef struct
{
    WORD32 dwAntIndex;
    T_USPUHFEpcReadPara tEpcReadPara;
    T_USPUHFEpcTagId    tTagId;
} T_USPUHFEpcRead;



/* ����ǩӦ�� */
typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;        /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;            /* ���������0���ɹ������� ��ErrorCode*/
    WORD16 wResultLength;          /* �����Ϣ���������ݳ��� ��ʧ��ʱΪ0  */
    WORD16 awReadData[0];          /* �䳤��ǩ���� */
} T_USPUHFEpcReadRes;



typedef struct
{
    BYTE   ucMB;
    BYTE   ucWriteType;   /*1 ʹ�ÿ�д 0 ��ͨд*/
    WORD16 wWordPointer;    
    WORD16 wWriteWordLen;
    WORD16 wPad;
    WORD32 dwAccessPassword;
    WORD32 dwUhfBitCommand;
    BYTE   aucData[64];  /*�䳤д������*/
} T_USPUHFEpcWritePara;

typedef struct
{  
    WORD32 dwAntIndex;
    T_USPUHFEpcWritePara tEpcWritePara;
    T_USPUHFEpcTagId     tTagId;
} T_USPUHFEpcWrite;


/* д��ǩӦ�� */
typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;         /*Ƶ�ʺ�*/
    WORD16 wResult;               /* ���������0���ɹ��������������� */
    WORD16 wResultLength;        /* �����Ϣ�����سɹ������ݳ���  */
} T_USPUHFEpcWriteRes;

typedef struct
{
   WORD16 wDataProfile;
   WORD16 wAccessRange;
   WORD16 wPersistence;
   WORD16 wReadOrWrite;
   WORD32 dwAccessPassword;
   WORD32 dwUhfBitCommand;
}T_USPUHFEpcIPJQTPara;


typedef struct
{  
    WORD32 dwAntIndex;
    T_USPUHFEpcIPJQTPara tEpcIPJQTPara;
    T_USPUHFEpcTagId     tTagId;
} T_USPUHFEpcIPJQT;


typedef struct
{
    WORD16 wProtoType;          /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;       /* ���߱��0 ~ 3 */
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;             /*QT ����Ķ�����*/
    WORD16 wDataProfile;        /*��ǩģʽ 1��˽��ģʽ 2������ģʽ*/
    WORD16 wAccessRange;         /*�Ƿ���Short_range����1���ر�   2������*/
    WORD16 wReadOrWrite;        /*0 �Ƕ�1��д*/
    WORD16 wRssi;;
}T_USPUHFEpcIPJQTRes;


typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;         /*Ƶ�ʺ�*/
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
    WORD16 wPayLoadNum;
	WORD16 wPad;
    T_USPUHFEpcLockPayload taEpcLockPayLoad[5];
} T_USPUHFEpcLockPayloadList;

typedef struct
{
    WORD32 dwAccessPassword;
    WORD32 dwUhfBitCommand;
    T_USPUHFEpcLockPayloadList tEpcLockPayLoad;   /*���ͽ��������Ķ�����*/
} T_USPUHFEPcLockPara;


typedef struct
{   
    WORD32 dwAntIndex;
    T_USPUHFEPcLockPara tEpcLockPara;
    T_USPUHFEpcTagId    tTagId;   /*�䳤EPC��*/
} T_USPUHFEpcLock;

/* ����ǩ����Ӧ�� */
typedef struct
{
    WORD16 wProtoType;    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6
Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;               /* ���������0���ɹ��������������� */
	WORD16 wPad;
} T_USPUHFEpcLockRes;


typedef struct
{
    WORD32 dwKillPassword;
    WORD32 dwUhfBitCommand;
} T_USPUHFEpcKillPara;


typedef struct
{
    WORD32 dwAntIndex;
    T_USPUHFEpcKillPara tEpcKillPara;
    T_USPUHFEpcTagId    tTagId;   /*�䳤EPC��*/
} T_USPUHFEpcKill;


/* ɱ����ǩӦ�� */
typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;           /* ���������0���ɹ������� ������ */
    WORD16 wPad;
} T_USPUHFEpcKillRes;


typedef struct
{
    BYTE   ucMB;
    BYTE   ucReadLock;
    WORD16 wOpSpecID;
    WORD16 ucRFU;
    WORD16 wBlockPtr;
    WORD16 wBlockRange;
    WORD16 wPad;
    WORD32 dwAccessPassword;
    WORD32 dwUhfBitCommand;
    BYTE   ucMask[512];
} T_USPUHFEpcBlockLockPara;



typedef struct
{
    WORD32 dwAntIndex;
    T_USPUHFEpcBlockLockPara tEpcBlockLockPara;
    T_USPUHFEpcTagId         tTagId;
} T_USPUHFEpcBlockLock;

typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;               /* ���������0���ɹ��������������� */
    WORD16 wRssi;
    BYTE     ucReadLock;
    BYTE     ucPad;
    WORD16 wLockStateLen;   /*�ֽڳ���*/
    WORD16 wPad;
    BYTE   ucLockState[512];
} T_USPUHFEpcBlockLockRes;


typedef struct
{
    BYTE     ucMask;
    BYTE     ucPad;
    WORD16   wPad;
    WORD32   dwAccessPassword;
    WORD32   dwUhfBitCommand;
} T_USPUHFEpcReadLockPara;

/*ALEN ר�нӿڶ�����*/
typedef struct
{
    WORD32 dwAntIndex;
    T_USPUHFEpcReadLockPara   tEpcReadLockPara;
    T_USPUHFEpcTagId          tTagId;        /* ��ǩ��ʶ���� */
} T_USPUHFEpcReadLock;



typedef struct
{
    WORD16 wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    WORD16 wAntennaIndex;         /* ���߱��0 ~ 3 */
    WORD16 wRssi;
    WORD16 wChannelIndex;       /*Ƶ�ʺ�*/
    WORD16 wResult;  /*�������Ľ��*/
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




