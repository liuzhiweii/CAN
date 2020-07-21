#ifndef UHF_RF_H
#define UHF_RF_H


#define RF_POWER_STATE_ON     (0)
#define RF_POWER_STATE_OFF    (1)


#define RF_ANT_INPOS     (1)
#define RF_ANT_NOT_INPOS (0)     


#define  POWER_TABLE_LEN    19
#define  RF_POWER_TABLE 	{15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33}

/* ����״̬ */
#define  ANTENNA_STATUS_OK      (1)   /* ���� */
#define  ANTENNA_STATUS_ERROR   (0)   /* �쳣 */
#define  ANTENNA_STATUS_UNKNOWN (2)   /* ״̬δ֪,���Ķ�����֧������״̬��ѯ */


/* ��Ƶ��Ԫ״̬ */
#define  RF_STATUS_OK      (0)   /* ���� */
#define  RF_STATUS_ERROR   (1)   /* �쳣 */

#define UHF_RF_START_TX     (1)   /* ������Ƶ���ʷ��ͣ�  */
#define UHF_RF_STOP_TX      (0)   /* �ر���Ƶ���ʷ��� */

#define  NEED_RESET_POWER  (1)
#define  NO_NEED_RESET_POWER  (0)


/* ��Ƶ��ԪPLL״̬ */
#define  RF_PLL_STATUS_OK      (1)   /* ���� */
#define  RF_PLL_STATUS_ERROR   (0)   /* �쳣 */

//��Ƶ���߶�Ƶģʽ
#define FIXED_FREQ                (0) /*��Ƶģʽ*/
#define FREQ_HOP                  (1) /*��Ƶģʽ*/

//��Ƶģʽ
#define FREQ_HOP_SCAN_CHANNEL  (0)           /* ���*/
#define FREQ_HOP_SCAN_RANDOM_CHANNEL  (1)  /* ���*/
#define FREQ_HOP_SCAN_STEP_CHANNEL  (2)  /* ����ѭ������*/


#define PLL_TYPE_800M         (0x0)
#define PLL_TYPE_900M         (0x1)
#define PLL_TYPE_800900M      (0x3)


/*Ӳ���ڶ���֧�������շ�����,1,3�Լ�2,4�ֱ�Ϊһ��,
  ͬʱ����֧��һ�Է���,��������Ϊ�շ�һ��*/
typedef struct
{
    WORD16 wAnt_mode;           /* 0�����߹������շ������LBTģʽ��1�����߹������շ�һ��ģʽ */
    WORD16 wAgcDatt;
    BYTE   wRcvOrSnd;          /*�������ߺ�,����շ�һ�����뷢�����ߺ���ͬ*/
    BYTE   ucAntennaLBT;          /*�Ƿ�֧��LBT����ģʽ*/
    BYTE   ucSupportCheckStatus;  /*�Ƿ�֧����λ���*/
    BYTE   ucAntStause;           /*�����Ƿ���λ*/
    BYTE   aucAntPower[UHF_RF_ANT_POWER_COUNT];
    BYTE   ucRevFilterSwitch;
    BYTE   ucPad;
    WORD16 wInventPower;
} T_AntennaCfg;

/* ��Ƶģ��������ݽṹ */
typedef struct
{
    WORD16 wConfigAntNum;
    WORD16 wLKCLSwitch;
    WORD16 wRcvDatt;
    WORD16 wLowNoEn;
    WORD16 wAntIndex;         /* ��ǰ�������� */
    WORD16 wFreqType;          /*800M 900M���໷*/
    WORD16 wWorkChannel;
    WORD16 wMaxFreqTable;
    WORD16 wProtocolType;
    WORD16 wPad;
    WORD32 dwWorkFreq;         /* ��ǰ����Ƶ��, ��λ��kHz,  */
    WORD32 dwPllWorkFreq;      /* ���໷Ŀǰ������Ƶ��ֵ*/
    T_AntennaCfg    atRfAntCfg[READER_ANTENNA_COUNT + 1];
} T_RfCtrl;

#define RF_CFG_NOT_EXIST (0)
#define RF_CFG_EXIST (1)
typedef struct
{
   WORD16 wAntIndex;
   WORD16 wForDataRate;/*ǰ������40 80 160 */
   WORD16 wRevDataRate;
   WORD16 wModulateType; /*���Ʒ�ʽ01 DSB  11 PR*/
   WORD16 wModulateDepth;
   WORD16 wProtocolType;
   WORD16 wPad;
   WORD16 wRfCfgExist;
   T_USPUHFInventRfPara tRfCfg;
}T_UhfInventRfConfigPara;


typedef struct 
{
  WORD16 wResult;  /*0 �ɹ�*/
  WORD16 wChannelNo;
  WORD16 wAntIndex;
  WORD16 wPad;
}T_UhfInventRfConfigRes;

   
typedef struct
{
   WORD16 wAntIndex;
   BYTE   aucPower[UHF_RF_ANT_POWER_COUNT]; 
   WORD16 wRevFilterSwitch;
   WORD16 wAntMode;
   WORD16 wAntLBT;
   WORD16 wAgcDatt;
   T_USPUHFRfFreqCfg tFreqCtrl;
}T_UhfRfAntConfigPara;

typedef struct
{
   WORD16 wConfigAntNum;
   WORD16 wRcvDatt;
   WORD16 wLKCLSwitch;
   WORD16 wLowNoEn;
   T_UhfRfAntConfigPara tAntConfigPara[READER_ANTENNA_COUNT]; 
}T_UhfRfConfigPara;


void uhfRf_SetFreqCfg(T_USPUHFRfFreqCfg *ptFreqCfg, WORD16 wAntIndex,
                                                   WORD16  wProtocolType);
WORD16 uhfRf_FreqHopCtrl(WORD16 wAntIndex,WORD16 wProtoType);
WORD32 uhfRf_CalRFPara(T_UhfInventRfConfigPara *ptInventRfConfigPara);
void   uhfRf_FreqHopInit(void);
T_USPUHFFreqTableEntry *uhfRf_GetHopTable(WORD16 wHopTableId);
WORD32 uhfRf_Init();
WORD32 uhfRf_Reset();
void uhfRf_SetCfgs(T_USPUHFReaderCfg *ptRfConfigPara);
void uhfRf_CreateDmpResMsg(T_Msg *ptRcvMsg,T_Msg *ptSndMsg);


#endif

