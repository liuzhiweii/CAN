#ifndef _USP_GB_H
#define _USP_GB_H

#define MAX_GB_MASK_WORD_LEN     (256)
#define MAX_GB_WRITE_WORD_LEN    (256)

/* 16bits��WORD���� */
#define mem_words_cpy(dst, src, word_len)  memcpy((dst), (src), ((word_len)*2))

/**************************************************************************/
typedef struct
{
    BYTE   ucMB;
    BYTE   ucPad;
    WORD16 wMaskBitLength;        /* Mask length (bits) */
    WORD16 wMaskBitPointer;       /* Starting Mask bit address */
    WORD16 awMaskData[MAX_GB_MASK_WORD_LEN];         /* �䳤 Mask value,�õ�һ���ֶ� */
    //WORD16 awMaskData[0];
} T_GBMacSelectMask;

typedef struct
{
    WORD16 wMemBank;             
    WORD16 wPad;
    WORD16 wReadWordsLen;         /* length (in words) */
    WORD16 wWordPointer;          /* Starting word address */
} T_GBMacRead;


typedef struct
{
    WORD16 wMemBank;             
    WORD16 wWriteWordsLen;          /* length (in words) */
    WORD16 wWordPointer;          /* Starting word address */
    WORD16 awWriteData[MAX_GB_WRITE_WORD_LEN];       /* �䳤���� */
    //WORD16 awWriteData[0]; 
} T_GBMacWrite;

typedef struct
{
    WORD16 wMemBank;              
    WORD16 wEraseWordsLen;          /* length (in words) */
    WORD16 wWordPointer;          /* Starting word address */
    WORD16 wPad;
} T_GBMacErase;


typedef struct
{
    WORD16 ucMemBank;             
    WORD16 ucReadLock;
    WORD16 wPad;
    WORD16 wBlockPtr;
    WORD16 wBlockRange;
    WORD16 ucMask[1]; /*�䳤����*/
} T_GBMacBlockLock;

typedef struct
{
    WORD16 wMask;
    WORD16 wPad;
} T_GBMacReadLock;

typedef struct
{
    WORD32 dwPwd;
} T_GBMacAccessPwd;

typedef struct
{
    WORD32 dwLockPayLoad;
} T_GBMacLock;

typedef struct
{
    WORD32 dwKillPwd;

} T_GBMacKill;


/* ����Э��Query����������� lxc */
typedef struct
{
    BYTE ucM;                            //����ѡ��miller or FM bit 0-bit 1
    BYTE ucReLinkRateDivisor;   //������·�������� bit 2 - bit 5
    BYTE ucTRext;                      //ǰ���ź�ָʾ bit 6
    BYTE ucTarget;                     //Ŀ�� , �̵��־bit 7
    BYTE ucSession;                   //�Ự bit8 -bit9
    BYTE ucCondition;                // ����bit10 - bit 13
    BYTE ucReserved;                //���� bit14 - bit15
    BYTE ucPad;
} T_uhf_GB_Query;

/////lxc sort
/* �¹���Э��sort����������� */
typedef struct
{
    BYTE ucMemBank;   //�洢�� bit0-bit5
    BYTE ucTarget;        //Ŀ��bit6-bit9
    BYTE ucRule;           //����bit10-bit11
    BYTE ucReserved;
} T_uhf_GB_Sort;


typedef struct
{
    WORD16 wModeIndex;
    WORD16 RTDataRate;            /* R=>T�������� */
    WORD16 TRDataRate;            /* T=>R�������� */
    WORD16 RealTRDataRate;     /* T=>Rʵ����ʹ�õ��������� */
    WORD16 TRLinkRate;             /* T=>R������·���� */
    float Tari;                        /* �Ķ�������ǩR=>T���źŴ���������0�ı�׼ʱ���� */
    float RTcal;                      /* Interrogator-to-Tag calibration symbol */
    float TRcal;                      /* Tag-to-Interrogator calibration symbol */
    WORD16 wRTcal_div_Tari;
    WORD16 wTRcal_div_halfTari;
    WORD16 wTpri;
    WORD16 T1min;
    WORD16 T1max;
    WORD16 T2min;
    WORD16 T2max;
    WORD16 T4min;
    WORD16 wTc;

    WORD16 wLastTagRssi;

    WORD16 TRCodeType;            /*T=>R����ģʽ����FM0���������Miller���ز���������*/
    WORD16 RTModulateType;      /* R=>T����ģʽ */
    WORD16 wRTModDepth;

    WORD16 PIE_data_1_len;      /*  data-1��" 110 "����" 1110 "��ʾ*/
    WORD16 Session;                   /* ��ǰ��ǩ��Session */


    WORD16 Command;                /* ��ǰ��������� */
    WORD16 MemBank;                 /* ��ǰ���ʵĴ洢�� */
    WORD16 Sel;
    WORD16 Target;
    WORD16 Truncate;
    WORD16 Action;

    BYTE ucGBUserDataLoad;      /*Rcp�ĸ��Ӳ���˵����*/
    /*0��û�и��Ӳ���*/
    /*1���������ñ�־*/
    /*������δ�������*/

    BYTE ucPad;

    WORD32 dwDefaultInventQ;

    T_uhf_GB_Sort Select;

    T_uhf_GB_Query Query;

    T_GBMacSelectMask tGBSelectMask;

    T_GBMacRead tGBRead;

    T_GBMacWrite tGBWrite;

    T_GBMacErase tGBErase;

    T_GBMacAccessPwd tGBAccess;

    T_GBMacLock tGBLock;

    T_GBMacKill tGBKill;

} T_GBCtl;

/* GB Э��ʱ��������� */
typedef struct
{
    WORD16 RTDataRate;             /* R=>T�������� */ //160
    WORD16 TRDataRate;             /* T=>R�������� */ //320
    WORD16 RealTRDataRate;       /* T=>Rʵ����ʹ�õ��������� */
    WORD16 wTc;
    WORD16 wTpri;
    WORD16 dwK;                    /* ������������ */
    WORD16 M;                            /* M: Number of subcarrier cycles per symbol��ȡֵΪ1��2��4��8 */
    WORD16 T1min;
    WORD16 T2min;
    WORD16 T4min;
} T_GB_TimesPara;



void uhf_ProtoHwInit(void);


WORD32 uhf_GB_Invetory_Resp(T_USPUHFGBInventoryRes *ptGBTagInventoryResp);
WORD32 uhf_GB_Read_Resp(T_USPUHFGBReadRes *ptGBReadResp);
WORD32 uhf_GB_Write_Resp(T_USPUHFGBWriteRes *ptGBWriteResp);
WORD32 uhf_GB_Lock_Resp(T_USPUHFGBLockRes *ptGBLockResp);
WORD32 uhf_GB_Erase_Resp(T_USPUHFGBEraseRes *ptGBEraseResp);
WORD32 uhf_GB_Kill_Resp(T_USPUHFGBKillRes *ptGBKillResp);




#endif


