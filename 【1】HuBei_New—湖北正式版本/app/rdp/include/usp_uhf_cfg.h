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
    unsigned short int wFreqChannelNum;
    unsigned short int wFreqChannel[MAX_FREQ_HOP_NUM]; /*�ò�����ʾ��Ƶ��Ƶ����*/
} T_USPUHFFreqHopChannel;

/*��Ƶ����*/
typedef struct
{
    unsigned short int                 wFreqHopMode;     /*��Ƶ��ʽ0˳��1���2����ѭ������*/
    unsigned short int                 wHopTimer;        /*��Ƶʱ����,10msΪ��λ*/
    T_USPUHFFreqHopChannel tfreqChannel;       /*�ò�����ʾ��Ƶ��Ƶ����*/
} T_USPUHFFreqHopCfg;

/*ʹ�ö�Ƶ���Ƶ���������*/
typedef struct
{
    unsigned short int             wFreqTableID;      /*Ƶ�ʷ�ʽ*/
    unsigned short int             wFreqWorkType;     /*0 ��Ƶ 1 ��Ƶ*/
    unsigned short int             wFixFreqChannel;
    unsigned short int             wPad;
#ifdef SUPPORT_HOP_FREQ
    T_USPUHFFreqHopCfg tFreqHopConfig;         /*��Ƶ����*/
#endif
} T_USPUHFRfFreqCfg;

typedef struct
{
    unsigned short int wHopTableID;
    unsigned short int wFrequeceNum;
    unsigned int dwFrequeces[64];
}T_USPUHFFreqTableEntry;


typedef struct
{
    unsigned char    ucFilterSwitch; /*0��ʾ�رն����˹��ܣ�1��ʾ�����ù���*/
    unsigned char    ucFilterThreshold;
    /*�԰ٷֱ���ʽ��ʾ��ֵ��ȡֵ��Χ[1,100]*/
    unsigned short int  wMaxReadRSSI;    /*����ź�ǿ�ȣ�ȡֵ��Χ[1,65535]*/
} T_USPUHFRSSIFilterCtl;


typedef struct
{
    T_USPUHFRSSIFilterCtl  tRSSIFilter6C; /*6CRSSI���˿��Ʋ���*/
    T_USPUHFRSSIFilterCtl  tRSSIFilter6B; /*6BRSSI���˿��Ʋ���*/
    T_USPUHFRSSIFilterCtl  tRSSIFilterGB; /*GBRSSI���˿��Ʋ���*/
} T_USPUHFRSSIFilter;


typedef struct
{
    unsigned char                ucForandRevConfig;   /*�����շ���ʽ����,0 �շ�һ�� 1 ��2  ��*/
    unsigned char                ucAntennaLBT;        /*�Ƿ�֧��LBT����ģʽ,Ԥ����ʽ*/
    unsigned short int              wCanChkStatus;       /*�Ƿ�֧����λ���*/
    unsigned char                aucAntPower[UHF_RF_ANT_POWER_COUNT];          /*���߹��ʱ�ʾ,dbm��ʾ*/
    unsigned short int              wRevFilterSwitch;    /*0  �ر�1 ��*/
    unsigned short int              wLbtDattValue;       /*4.0�汾���õĺ���ȥ����*/ 
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
    unsigned short int wForDataRate; /*ǰ������*/
    unsigned short int wRevDataRate; /*��������*/
    unsigned short int wPIEValue;    /*PIE ��ֵ*/
    unsigned short int wPad;
    E_USPUHFDataEncodeType   eDataEncodeType;  /*������뷽ʽ0fm0 1miller2  2mille4 3miller4*/
    E_USPUHFModulateType     eModulateType;   /*���Ʒ�ʽ0 pr 1 dsb*/
} T_USPUHFEpcUserDefPara;

typedef struct
{
    unsigned short int wForDataRate; /*ǰ������*/
    unsigned short int wRevDataRate; /*��������*/
    unsigned short int wPIEValue;    /*PIE ��ֵ*/
    unsigned short int wPad;
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
    unsigned short int                  wEPCModuleDepth;    /*0-80,1-90,2-100*/
    unsigned short int                  wISO6BModuleDepth;  /*6B�������0-18 1-100*/
    unsigned short int                  wGBModuleDepth;  /* GB ������� lxc */
    unsigned short int                  wIQConfig;          /*I Q·���ã�Ŀǰû��*/
    unsigned char                    ucTurnOnLKCL;       /*й¶��������0 �ر�1��*/
    unsigned char                    ucRecvDATT;         /*����DATT����*/
    unsigned short int                  wFreqType;          /*�Ķ����Ĺ�������800 or 900 or 800900*/
    unsigned short int                  wPad;
} T_USPUHFReaderCfg;


typedef struct
{
    unsigned short int             wTransmitPower;
    unsigned short int             wReceiverSensitivity;
    T_USPUHFRfFreqCfg  tFreqCfg;
} T_USPUHFInventRfPara;



typedef struct
{
    /*I+ I- Q+ Q-�ĳ�ʼֵ*/
    unsigned short int wInitX0;
    unsigned short int wInitX1;
    unsigned short int wInitY0;
    unsigned short int wInitY1;
     /*й¶�����Ļ���������*/
    unsigned short int wIntegralMax;
    unsigned short int wIntegralMin;
    /*й¶����ֹͣ����*/
    unsigned short int wStopThd;
    /*й¶�����ĳ�ʼ��λ*/
    unsigned short int wInitcos;   
    unsigned short int wInitSin;

    unsigned int dwAdjThrHold;
    unsigned short int wFineThrHold;  /*������μ�⵽��Ҫ΢�������΢��*/
    unsigned short int wDacChange;

}T_USPUHFLKCLInitParas;


typedef struct
{
    /*I+ I- Q+ Q-�ĳ�ʼֵ*/
    unsigned short int wX0;
    unsigned short int wX1;
    unsigned short int wY0;
    unsigned short int wY1;
    unsigned short int wInitCos;
    unsigned short int wInitSin;
    unsigned short int wIsCrossed;   /*�Ƿ���Ҫ����*/
    unsigned short int wIsConfiged;  /*�жϵ�ǰ��Ƶ���Ƿ��Ѿ�й¶�������*/
    unsigned short int wCheckFineCnt;
    unsigned short int wADCidata;
    unsigned short int wADCqdata;
    unsigned short int wAdjCount;
}T_USPUHFLKCLParas;

typedef struct
{
    signed short int swPower;   /*���ʣ���dbΪ��λ����С��λ0.01db,ʹ��*100������*/
    unsigned short int  wVoltage;  /*��Ӧ�ĵ�ѹֵ*/
}T_USPUHFRfPowerTableEntry;

typedef struct
{
   unsigned short int wGateVoltage0;     /* դѹ������0 */
   unsigned short int wGateVoltage1;     /* դѹ������1 */
}T_USPUHFGateVoltageCtrl;

typedef struct
{
   unsigned short int  wK0;
   signed short int swMinK0;
   unsigned short int  wPR1Value;
   unsigned short int  wPR2Value;
   T_USPUHFRfPowerTableEntry atFwTable[RF_FORWARD_PT_NUM];
   T_USPUHFRfPowerTableEntry atBwTable[RF_BACKWARD_PT_NUM];
}T_USPUHFRfPowerTable;

typedef struct
{
   unsigned int dwVersion;
   T_USPUHFRfPowerTable tUhfRfPowerTable1;
   T_USPUHFRfPowerTable tUhfRfPowerTable2;
}T_USPUHFFlashRfData;

typedef struct 
{
    unsigned short int  wCalibrateK0;   /* �������ʱ��K0ֵ*/
    unsigned short int  wCalibratPower; /* �������ʱ�Ĺ���ֵ*/
    unsigned short int  wTargetPower;   
    unsigned short int  wDattVal;       /*DATT ˥��ֵ*/
}T_USPUHFRfPowerEntry;

typedef struct
{  
   unsigned short int wResIndex;     /*��¼��ǰ�Ķ�����*/
   unsigned short int wVswr;         /* פ����* 100*/
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
    unsigned short int wantennaId;
    unsigned short int wForDataRate;
    unsigned short int wRevDataRate;
    EM_value_Type eM_value;
    EForward_link_Type eForward_link_modulation;
    unsigned short int wPIE_Value;
} T_USPUHFRFModeCfg;

unsigned int usp_uhf_set_cfg(T_USPUHFReaderCfg *ptReaderCfg);

#endif


