#ifndef _USP_UHF_CFG_H
#define _USP_UHF_CFG_H

/* Ant_mode_sel��0�����߹������շ������LBTģʽ��1�����߹������շ�һ��ģʽ */
#define ANT_MODE_DUAL_ANT     (0)  /* ���߹������շ������LBTģʽ */
#define ANT_MODE_SINGLE_ANT   (1)  /* ���߹������շ�һ��ģʽ           */

/*����ģ��֪ͨ��Ƶģ��Э�����͵ĺ�*/
#define UHF_RF_PROTOCOL_EPC     (0)
#define UHF_RF_PROTOCOL_ISO6B   (1)
#define UHF_RF_PROTOCOL_GB   (2)




typedef struct
{
    WORD16 wFreqChannelNum;
    WORD16 wFreqChannel[MAX_FREQ_HOP_NUM]; /*�ò�����ʾ��Ƶ��Ƶ����*/
} T_USPUHFFreqHopChannel;

/*��Ƶ����*/
typedef struct
{
    WORD16                 wFreqHopMode;     /*��Ƶ��ʽ0˳��1���2����ѭ������*/
    WORD16                 wHopTimer;        /*��Ƶʱ����,10msΪ��λ*/
    T_USPUHFFreqHopChannel tfreqChannel;       /*�ò�����ʾ��Ƶ��Ƶ����*/
} T_USPUHFFreqHopCfg;

/*ʹ�ö�Ƶ���Ƶ���������*/
typedef struct
{
    WORD16             wFreqTableID;      /*Ƶ�ʷ�ʽ*/
    WORD16             wFreqWorkType;     /*0 ��Ƶ 1 ��Ƶ*/
    WORD16             wFixFreqChannel;
    WORD16             wPad;
#ifdef SUPPORT_HOP_FREQ
    T_USPUHFFreqHopCfg tFreqHopConfig;         /*��Ƶ����*/
#endif
} T_USPUHFRfFreqCfg;

typedef struct
{
    WORD16 wHopTableID;
    WORD16 wFrequeceNum;
    WORD32 dwFrequeces[64];
}T_USPUHFFreqTableEntry;


typedef struct
{
    BYTE    ucFilterSwitch; /*0��ʾ�رն����˹��ܣ�1��ʾ�����ù���*/
    BYTE    ucFilterThreshold;
    /*�԰ٷֱ���ʽ��ʾ��ֵ��ȡֵ��Χ[1,100]*/
    WORD16  wMaxReadRSSI;    /*����ź�ǿ�ȣ�ȡֵ��Χ[1,65535]*/
} T_USPUHFRSSIFilterCtl;


typedef struct
{
    T_USPUHFRSSIFilterCtl  tRSSIFilter6C; /*6CRSSI���˿��Ʋ���*/
    T_USPUHFRSSIFilterCtl  tRSSIFilter6B; /*6BRSSI���˿��Ʋ���*/
    T_USPUHFRSSIFilterCtl  tRSSIFilterGB; /*GBRSSI���˿��Ʋ���*/
} T_USPUHFRSSIFilter;


typedef struct
{
    BYTE                ucForandRevConfig;   /*�����շ���ʽ����,0 �շ�һ�� 1 ��2  ��*/
    BYTE                ucAntennaLBT;        /*�Ƿ�֧��LBT����ģʽ,Ԥ����ʽ*/
    WORD16              wCanChkStatus;       /*�Ƿ�֧����λ���*/
    BYTE                aucAntPower[UHF_RF_ANT_POWER_COUNT];          /*���߹��ʱ�ʾ,dbm��ʾ*/
    WORD16              wRevFilterSwitch;    /*0  �ر�1 ��*/
    WORD16              wLbtDattValue;       /*4.0�汾���õĺ���ȥ����*/ 
    T_USPUHFRSSIFilter  tReadRSSIFilter;          /*RSSI ��������*/
    T_USPUHFRSSIFilter  tWriteRSSIFilter;
} T_USPUHFRfCommonCfg;


typedef enum USP_UHF_Encode_Type
{
    DATA_ENCODE_TYPE_FM0 = 0,
    DATA_ENCODE_TYPE_MILLER2 = 1,
    DATA_ENCODE_TYPE_MILLER4 = 2,
    DATA_ENCODE_TYPE_MILLER8 = 3,
} E_USPUHFDataEncodeType;

typedef enum USP_UHF_Modulate_Type
{
    Modulate_TYPE_DSB = 1,
    Modulate_TYPE_SSB = 2,
    Modulate_TYPE_PR  = 3,
} E_USPUHFModulateType;


typedef struct
{
    WORD16 wForDataRate; /*ǰ������*/
    WORD16 wRevDataRate; /*��������*/
    WORD16 wPIEValue;    /*PIE ��ֵ*/
    WORD16 wPad;
    E_USPUHFDataEncodeType   eDataEncodeType;  /*������뷽ʽ0fm0 1miller2  2mille4 3miller4*/
    E_USPUHFModulateType     eModulateType;   /*���Ʒ�ʽ0 pr 1 dsb*/
} T_USPUHFEpcUserDefPara;

typedef struct
{
    WORD16 wForDataRate; /*ǰ������*/
    WORD16 wRevDataRate; /*��������*/
    WORD16 wPIEValue;    /*PIE ��ֵ*/
    WORD16 wPad;
    E_USPUHFDataEncodeType   eDataEncodeType;  /*������뷽ʽ0fm0 1miller2  2mille4 3miller4*/
    E_USPUHFModulateType     eModulateType;   /*���Ʒ�ʽ0 pr 1 dsb*/
}T_USPUHFGBUserDefPara;


typedef struct
{
    T_USPUHFEpcUserDefPara  tAntEpcUserDefPara[READER_ANTENNA_COUNT];
   // T_USPUHFGBUserDefPara    tAntGBUserDefPara[READER_ANTENNA_COUNT]; ��ʱ��Ҫ
    T_USPUHFRfCommonCfg     tAntRfCommonCfg[READER_ANTENNA_COUNT];
    T_USPUHFRfFreqCfg       tAntEpcRfFreqCfg[READER_ANTENNA_COUNT];
    T_USPUHFRfFreqCfg       tAntISO6BRfFreqCfg[READER_ANTENNA_COUNT];
    T_USPUHFRfFreqCfg       tAntGBRfFreqCfg[READER_ANTENNA_COUNT];
    WORD16                  wEPCModuleDepth;    /*0-80,1-90,2-100*/
    WORD16                  wISO6BModuleDepth;  /*6B�������0-18 1-100*/
    WORD16                  wGBModuleDepth;  /* GB ������� lxc */
    WORD16                  wIQConfig;          /*I Q·���ã�Ŀǰû��*/
    BYTE                    ucTurnOnLKCL;       /*й¶��������0 �ر�1��*/
    BYTE                    ucRecvDATT;         /*����DATT����*/
    WORD16                  wFreqType;          /*�Ķ����Ĺ�������800 or 900 or 800900*/
    WORD16                  wPad;
} T_USPUHFReaderCfg;


typedef struct
{
    WORD16             wTransmitPower;
#if USE_INVENTORY_RF_CONFIG 
    WORD16             wReceiverSensitivity;
    T_USPUHFRfFreqCfg  tFreqCfg;
#endif
} T_USPUHFInventRfPara;



typedef struct
{
    /*I+ I- Q+ Q-�ĳ�ʼֵ*/
    WORD16 wInitX0;
    WORD16 wInitX1;
    WORD16 wInitY0;
    WORD16 wInitY1;
     /*й¶�����Ļ���������*/
    WORD16 wIntegralMax;
    WORD16 wIntegralMin;
    /*й¶����ֹͣ����*/
    WORD16 wStopThd;
    /*й¶�����ĳ�ʼ��λ*/
    WORD16 wInitcos;   
    WORD16 wInitSin;

    WORD32 dwAdjThrHold;
    WORD16 wFineThrHold;  /*������μ�⵽��Ҫ΢�������΢��*/
    WORD16 wDacChange;

}T_USPUHFLKCLInitParas;


typedef struct
{
    /*I+ I- Q+ Q-�ĳ�ʼֵ*/
    WORD16 wX0;
    WORD16 wX1;
    WORD16 wY0;
    WORD16 wY1;
    WORD16 wInitCos;
    WORD16 wInitSin;
    WORD16 wIsCrossed;   /*�Ƿ���Ҫ����*/
    WORD16 wIsConfiged;  /*�жϵ�ǰ��Ƶ���Ƿ��Ѿ�й¶�������*/
    WORD16 wCheckFineCnt;
    WORD16 wADCidata;
    WORD16 wADCqdata;
    WORD16 wAdjCount;
}T_USPUHFLKCLParas;

typedef struct
{
    SWORD16 swPower;   /*���ʣ���dbΪ��λ����С��λ0.01db,ʹ��*100������*/
    WORD16  wVoltage;  /*��Ӧ�ĵ�ѹֵ*/
}T_USPUHFRfPowerTableEntry;

typedef struct
{
   WORD16 wGateVoltage0;     /* դѹ������0 */
   WORD16 wGateVoltage1;     /* դѹ������1 */
}T_USPUHFGateVoltageCtrl;

typedef struct
{
   WORD16  wK0;
   SWORD16 swMinK0;
   WORD16  wPR1Value;
   WORD16  wPR2Value;
   T_USPUHFRfPowerTableEntry atFwTable[RF_FORWARD_PT_NUM];
   T_USPUHFRfPowerTableEntry atBwTable[RF_BACKWARD_PT_NUM];
}T_USPUHFRfPowerTable;

typedef struct
{
   WORD32 dwVersion;
   T_USPUHFRfPowerTable tUhfRfPowerTable1;
   T_USPUHFRfPowerTable tUhfRfPowerTable2;
}T_USPUHFFlashRfData;

typedef struct 
{
    WORD16  wCalibrateK0;   /* �������ʱ��K0ֵ*/
    WORD16  wCalibratPower; /* �������ʱ�Ĺ���ֵ*/
    WORD16  wTargetPower;   
    WORD16  wDattVal;       /*DATT ˥��ֵ*/
}T_USPUHFRfPowerEntry;

typedef struct
{  
   WORD16 wResIndex;     /*��¼��ǰ�Ķ�����*/
   WORD16 wVswr;         /* פ����* 100*/
   T_USPUHFRfPowerEntry      atPowers[UHF_RF_ANT_POWER_COUNT];
   T_USPUHFRfPowerTable     *ptPowerTable;
}T_USPUHFRfAntPower;

typedef enum
{
    M_value_Type_FM0 = 0,
    M_value_Type_Miller = 1,
    M_value_Type_Miller4 = 2,
    M_value_Type_Miller8 = 3,
} EM_value_Type;

typedef enum
{
    Forward_link_Type_PR_ASK = 0,
    Forward_link_Type_SSB_ASK = 1,
    Forward_link_Type_DSB_ASK = 2,
} EForward_link_Type;

typedef struct
{
    WORD16 wantennaId;
    WORD16 wForDataRate;
    WORD16 wRevDataRate;
    EM_value_Type eM_value;
    EForward_link_Type eForward_link_modulation;
    WORD16 wPIE_Value;
} T_USPUHFRFModeCfg;

WORD32 usp_uhf_set_cfg(T_USPUHFReaderCfg *ptReaderCfg);

#endif



