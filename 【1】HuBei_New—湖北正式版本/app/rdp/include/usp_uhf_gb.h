
#ifndef _USP_UHF_GB_H
#define _USP_UHF_GB_H

/***********************GBЭ����ؽӿ�**************************/

#define GB_MAX_MASK_WORD_LEN (32)
#define GB_MAX_MASK_BYTE_LEN (64)

/*GB Acees ����*/
typedef struct
{
    unsigned short int wTagWordLen;    /* ��ǩ���ݳ��ȣ���WORDΪ��λ��������PC�ֶ�  */
    unsigned short int wPad;
    unsigned char   aucTagData[0];      /* �䳤 GB ���ݵĵ�һ�����ݶ� */
} T_USPUHFGBTagId;

typedef enum
{
    GBSortAction_Match1b_NoMatch0b = 0,
    GBSortAction_MatchNothing_NoMatch0b = 1,
    GBSortAction_Match1b_NoMatchNothing = 2,
    GBSortAction_Match0b_NoMatch1b = 3,
} GBSortAction;

typedef enum
{
    GBSortTarget_Session_S0 = 0,
    GBSortTarget_Session_S1 = 1,
    GBSortTarget_Session_S2 = 2,
    GBSortTarget_Session_S3 = 3,
    GBSortTarget_Session_SL = 4,
} GBSortTarget;

typedef enum UHF_EGBDRValue
{

    GB_DRValue_DRV_8 = 0,
    GB_DRValue_DRV_64_3 = 1,
} E_USPUHFGBDRValue;

typedef enum LLRP_EGBMValue
{

    GB_MValue_MV_FM0 = 0,
    GB_MValue_MV_2 = 1,
    GB_MValue_MV_4 = 2,
    GB_MValue_MV_8 = 3,
} E_USPUHFGBMValue;


typedef enum LLRP_GBForwardLinkModulation
{

    GB_ForwardLinkModulation_PR_ASK = 0,
    GB_GBForwardLinkModulation_SSB_ASK = 1,
    GB_GBForwardLinkModulation_DSB_ASK = 2,
} E_USPUHFEGBForwardLinkModulation;


typedef enum LLRP_EGBpectralMaskIndicator
{

    GB_SpectralMaskIndicator_Unknown = 0,
    GB_SpectralMaskIndicator_SI = 1,
    GB_SpectralMaskIndicator_MI = 2,
    GB_SpectralMaskIndicator_DI = 3,
} E_USPUHFGBpectralMaskIndicator;

typedef struct
{
    unsigned int dwModeIdentifier;
    E_USPUHFGBMValue eMValue;  /* ������뷽ʽ FM0*/
    E_USPUHFEGBForwardLinkModulation eForwardLinkModulation; /* ���Ʒ�ʽ DSB*/
    unsigned int dwFawardValue;
    unsigned int dwBDRValue;  /* �������� *//* 320 */
    unsigned int dwPw;
    unsigned int dwTc;
} T_USPUHFGBRFModeTableEntry;

/// new GB inventory structure
typedef struct
{
    unsigned short int            wMemBank;            
    /* �¹����б�ʾ�洢��00: ��Ϣ
                         01: ���� 
                         02: ��ȫ
                         03: �û� 
    */
    unsigned short int            wMaskLength;        /* Mask length (bits) */
                                                            /* ���¹����У����ֵ��ʾ���ݳ��ȣ���ת���õ�����*/
    unsigned short int            wMaskBitPointer;    /* Starting Mask bit address */
                                                            /* ���¹����У����ֵ��ʾ����ָ�� */
    unsigned short int            wPad;
    GBSortTarget      ucSortTarget;
    GBSortAction      ucSortAction;
    unsigned char              ucMaskData[GB_MAX_MASK_BYTE_LEN];
    /* �䳤 Mask value,�õ�һ���ֶ� */
} T_USPUHFGBInventoryMask;  //T_USPUHFGBInventoryMask


typedef struct
{
    unsigned short int wMaskNum;
    unsigned short int wPad;
    T_USPUHFGBInventoryMask tGBInventoryMask[MaxGBSortNum]; //GB_SETLECT_MASK_NUM
} T_USPUHFGBInventoryMaskList;

typedef struct
{
    unsigned short int wCCN;
    unsigned short int wCIN;
    unsigned short int wSession;
    unsigned short int wModeindex;
    unsigned char ucGBQureySel;
    unsigned char ucGBQureyAction;
    unsigned short int wPad;
    T_USPUHFGBInventoryMaskList tGBInventoryMaskList;
} T_USPUHFGBInventoryPara; // T_USPUHFGBInventoryPara

typedef struct
{
    unsigned char   ucAntennaID;            /* ���߱�� */
    unsigned char   ucProtoType;           /*Э������û��*/
    unsigned short int wPad;
    T_USPUHFGBInventoryPara tGBInventoryPara;
    T_USPUHFInventRfPara     tInventoryRfPara;
} T_USPUHFGBInventory; // T_USPUHFGBInventory


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
    T_USPUHFGBTagId tTagId;     /* �䳤��ǩ���� */
} T_USPUHFGBInventoryRes; // T_USPUHFGBInventoryRes


/////// new GB read structure
typedef struct
{
    unsigned char   ucMB;   /* �¹����б�ʾ�洢�� */
    unsigned char   ucPad;
    unsigned short int wWordPointer;  /* x�¹����б�ʾ��ʼ��ַ */
    unsigned short int wWordCount;    /* �¹����б�ʾ�ֵ����� */
    unsigned short int wPad;
    unsigned int dwAccessPassword;
} T_USPUHFGBReadPara; // T_USPUHFGBReadPara

typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFGBReadPara tGBReadPara;
    T_USPUHFGBTagId    tTagId;
} T_USPUHFGBRead; //T_USPUHFGBRead



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
} T_USPUHFGBReadRes; //T_USPUHFGBReadRes


///////  new GB write Structure
typedef struct
{
    unsigned char   ucMB;              /* �¹����б�ʾ�洢�� */
    unsigned char   ucPad;   /*1 ʹ�ÿ�д 0 ��ͨд*/ /* �¹���û���� */
    unsigned short int wWordPointer;    /* ���ʵ���ʼ��ַ */
    unsigned short int wWriteWordLen; /* д�����ݳ��� */
    unsigned short int wPad;
    unsigned int dwAccessPassword;
    unsigned char   aucData[64];  /*�䳤д������*/
} T_USPUHFGBWritePara; // T_USPUHFGBWritePara

typedef struct
{  
    unsigned int dwAntIndex;
    T_USPUHFGBWritePara tGBWritePara;
    T_USPUHFGBTagId     tTagId;
} T_USPUHFGBWrite; // T_USPUHFGBWrite


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
} T_USPUHFGBWriteRes; // T_USPUHFGBWriteRes


/* ����������ض��� */
typedef struct
{
    unsigned char   ucMB;              /* �¹����б�ʾ�洢�� */
    unsigned char   ucPad;
    unsigned short int wPad;
    unsigned short int wWordPointer;    /* ���ʵ���ʼ��ַ */
    unsigned short int wEraseWordLen; /* д�����ݳ��� */
    unsigned int dwAccessPassword;
} T_USPUHFGBErasePara;

typedef struct
{  
    unsigned int dwAntIndex;
    T_USPUHFGBErasePara tGBErasePara;
    T_USPUHFGBTagId     tTagId;
} T_USPUHFGBErase;


/*������ǩӦ�� */
typedef struct
{
    unsigned short int wProtoType;
    /* Э������ 0:GEN2 ; 1:18000-6 Type A ; 2:18000-6 Type B */
    unsigned short int wAntennaIndex;         /* ���߱��0 ~ 3 */
    unsigned short int wRssi;
    unsigned short int wChannelIndex;         /*Ƶ�ʺ�*/
    unsigned short int wResult;               /* ���������0���ɹ��������������� */
    unsigned short int wResultLength;        /* �����Ϣ�����سɹ������ݳ���  */
} T_USPUHFGBEraseRes;


typedef enum E_USPGBLockDataArea   //��ucMB������ȡֵ��ͬ
{
    GB_LockDataField_Info_Area = 0,   //��Ϣ��
    GB_LockDataField_Coding_Area = 1, //������
    GB_LockDataField_Sec_Area = 2,     //��ȫ��
    GB_LockDataField_User_Area = 3,    //�û���
} E_GBLockDataArea;

typedef enum E_USPGBLockAreaCfg
{
    GB_LockCfg_Store_Area = 0,  // ���ô洢��
    GB_LockCfg_Sec_Area    = 1,  //���ð�ȫ����
}E_GBLockAreaCfg;

typedef enum E_USPGBLockActionCfg
{
    GB_LockAction_RW  = 0,  // �ɶ���д
    GB_LockAction_RO  = 1,  // �ɶ�����д
    GB_LockAction_WO  = 2,  // ���ɶ���д
    GB_LockAction_N   = 3,  // ���ɶ�����д
}E_GBLockActionCfg;


typedef struct  // lock ����͸������
{
    unsigned int dwAccessPassword;
    unsigned char  ucMB; // DataArea 
    unsigned char  ucConfig; // AreaCfg 0 or 1
    unsigned char ucAeraMode; // ActionCfg
    unsigned char ucSecureMode;  // securit mode cfg
} T_USPUHFGBLockPara;

//typedef struct
//{
//    unsigned int dwAccessPassword;
//    E_GBLockDataArea DataArea;
//    E_GBLockAreaCfg   AreaCfg;
//    E_GBLockActionCfg ActionCfg;  
//} T_USPUHFGBLockPara;


typedef struct
{   
    unsigned int dwAntIndex;
    T_USPUHFGBLockPara tGBLockPara;
    T_USPUHFGBTagId    tTagId;   /*�䳤GB��*/
} T_USPUHFGBLock;

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
} T_USPUHFGBLockRes;


typedef struct
{
    unsigned int dwKillPassword;
} T_USPUHFGBKillPara;


typedef struct
{
    unsigned int dwAntIndex;
    T_USPUHFGBKillPara tGBKillPara;
    T_USPUHFGBTagId    tTagId;   /*�䳤GB��*/
} T_USPUHFGBKill;


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
} T_USPUHFGBKillRes;



void usp_uhf_gb_inventory(T_USPUHFGBInventory*ptGBInventoryPara,T_USPUHFGBInventoryRes*ptGBTagInventoryResp);
void usp_uhf_gb_read(T_USPUHFGBRead *ptGBReadPara,T_USPUHFGBReadRes *ptGBReadResp);
void usp_uhf_gb_write(T_USPUHFGBWrite *ptGBWritePara,T_USPUHFGBWriteRes *ptGBWriteResp);
void usp_uhf_gb_lock(T_USPUHFGBLock *ptGBLockPara,T_USPUHFGBLockRes *ptGBLockResp);
void usp_uhf_gb_erase(T_USPUHFGBErase *ptGBErase,T_USPUHFGBEraseRes *ptGBEraseResp);
void usp_uhf_gb_kill(T_USPUHFGBKill *ptGBKillPara,T_USPUHFGBKillRes *ptGBKillResp);

#endif

