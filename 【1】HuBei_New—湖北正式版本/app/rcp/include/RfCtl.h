/************************************************************************
* ��Ȩ���� (C)2011, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� rfctl.h
* �ļ���ʶ�� 
* ����ժҪ����Ƶ��������Ҫ�ĺ궨��
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� zdx
* ������ڣ� 2010.12.26
* 
* �޸ļ�¼1�� RUP �����޸�����Ӧ
*    �޸����ڣ�2011.12.1
*    �� �� �ţ�    RUP
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/

#ifndef _RFCTL_H
#define _RFCTL_H

/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
#define UHFRF_AC_WAIT_TIME              (100000)  /*�ȴ���Ƶ�ȶ�ʱ�䣬usΪ��λ*/

#define READER_ANT_INIT_POWER            2000  /*��ʼ����ֵ���������״̬�ã��ڶ��ձ������������*/
#define READER_AC_ATTENUATION_INIT       1900  /* ȱʡ˥��ֵ19 DB ��Ӧ���ʹ���20DB */

#define DEMOD_LO_SEL_PLL           (0x000)
#define DEMOD_LO_SEL_COUPLE        (0x001)

#define POWER_DETECT_CTRL_SelForward    (0x001)
#define POWER_DETECT_CTRL_SelBackward   (0x000)

#define POWER_DETECT_ADC_CLK_ON    (0x01)       /* �򿪹��ʼ��ADCʱ�� */
#define POWER_DETECT_ADC_CLK_OFF   (0x00)       /* �رչ��ʼ��ADCʱ�� */

#define LKCL_DAC_CLK_ON            (0x01)       /* ��й©����DACʱ�� */
#define LKCL_DAC_CLK_OFF           (0x00)       /* �ر�й©����DACʱ�� */

#define LKCL_SET_POWER                (3000)   

#define READER_AC_MAX_K0                 (0x6000)  /* ����K0������ֵ */
#define UHF_RF_MAX_AC_ADJUST              (10)

/* LBT״̬ */
#define LBT_STATUS_IDLE           (0)  /* ����״̬       */
#define LBT_STATUS_BUSY           (1)  /* �ŵ���ռ��״̬ */

#define  AMP_NO_NEED_SLEEP  (0)
#define  AMP_NEED_SLEEP     (1)

#define  UHF_RF_FAST_LKCL_ON  (1)
#define  UHF_RF_FAST_LKCL_OFF (0)


#define RF_AMP_ON   		(1)  			/* 1). �򿪹��ŵ�Դ */
#define RF_AMP_OFF  		(0)  			/*     �رչ��ŵ�Դ */ 

#define PLL_NO_NEED_RESET   (0)
#define PLL_NEED_RESET      (1)

#define LBT_ON           (0x2)   /* LBT���ܴ� */
#define LBT_OFF          (0x1)   /* LBT���ܹر� */

#define UHF_RF_PT_ANT14    (0)   /* ����1��Ӧ�������ݣ���������1��4���õĶ������� */
#define UHF_RF_PT_ANT23    (1)   /* ����2��Ӧ�������ݣ���������2��3���õĶ������� */
#define UHF_RF_PT_ANT3     (2)   /* ����3��Ӧ�������� */
#define UHF_RF_PT_ANT4     (3)   /* ����4��Ӧ�������� */

#define FOUR_POWER_TABLE   4     /* 4�׶������� */
#define POWER_TABLE_COUNT  0x263 /* ����EEPROM�д˵�ַ��ֵ���ж��Ķ������м��׶������� */

/*���໷������״̬��0�����ϵ�δ���е�һ�����ã�
   1 ����Ӧ�����ù���һ�����ñ��ⷢ����澯*/
#define UHF_RF_PLL_NOT_CONFIGED (0)
#define UHF_RF_PLL_CONFIGED     (1)


#define uhf_epld_reg_read(wRegAddr)         g_pvUSPDrvEpldVirtStartAddr0[wRegAddr]
#define uhf_epld_reg_write(wRegAddr,wData)  g_pvUSPDrvEpldVirtStartAddr0[wRegAddr]=(wData)

#define uhfRf_GetAmpState()  g_pvUSPDrvEpldVirtStartAddr0[EPLD_RF_AMP_CTRL_REG]


#define REV_FILTER_40       (0)
#define REV_FILTER_80_160   (1)
#define REV_FILTER_320_640  (2)
#define REV_NO_FILTER       (3)


#define LBT_switch(wAction)               FpgaRegWrite(REG_LBT_SWITCH,wAction) 

#define SendAlarm 

/* LED���Ʋ��� */
#define ANT1_EXIST        USP_OSS_LED_FLASH_ANT1_EXIST            /* ����1��λ������ */  
#define ANT1_NOT_EXIST    USP_OSS_LED_FLASH_ANT1_NOT_EXIST        /* ����1����λ������ */
#define ANT1_WORK_ERR     USP_OSS_LED_FLASH_ANT1_WORK_ERR         /* ����1�����쳣������ʱ��2�룬֮���Զ��رգ�����Ƶ����1HZ */
#define ANT1_WORKING      USP_OSS_LED_FLASH_ANT1_WORKING          /* ����1��������������ʱ��1�룬֮���Զ��رգ�����Ƶ����5HZ */
#define ANT2_EXIST        USP_OSS_LED_FLASH_ANT2_EXIST           
#define ANT2_NOT_EXIST    USP_OSS_LED_FLASH_ANT2_NOT_EXIST       
#define ANT2_WORK_ERR     USP_OSS_LED_FLASH_ANT2_WORK_ERR        
#define ANT2_WORKING      USP_OSS_LED_FLASH_ANT2_WORKING         
#define ANT3_EXIST        USP_OSS_LED_FLASH_ANT3_EXIST           
#define ANT3_NOT_EXIST    USP_OSS_LED_FLASH_ANT3_NOT_EXIST       
#define ANT3_WORK_ERR     USP_OSS_LED_FLASH_ANT3_WORK_ERR        
#define ANT3_WORKING      USP_OSS_LED_FLASH_ANT3_WORKING        
#define ANT4_EXIST        USP_OSS_LED_FLASH_ANT4_EXIST           
#define ANT4_NOT_EXIST    USP_OSS_LED_FLASH_ANT4_NOT_EXIST       
#define ANT4_WORK_ERR     USP_OSS_LED_FLASH_ANT4_WORK_ERR        
#define ANT4_WORKING      USP_OSS_LED_FLASH_ANT4_WORKING   

#if USP_OSS_LED_USE
#define RCP_LED_FLASH_ON(id)            usp_oss_led_on(id)
#define RCP_LED_FLASH_OFF(id)           usp_oss_led_off(id)
#else
#define RCP_LED_FLASH_ON(id)
#define RCP_LED_FLASH_OFF(id)
#endif

#define DAC_DC_CHANGE_DELAY         50    /* ��FPGA 0x87�Ĵ���ֵ�޸ĺ���Ҫ��ʱ */


/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
typedef double Double;

/*����RFCB2 ��Ӧ��FPGA*/
typedef struct 
{
   WORD16 w20DBAtten;        /*20Dbʱ����Ӧ��FPGA˥��ֵ*/
   WORD16 wFullPower;        /*˥��ֵΪ0ʱ�Ĺ���ֵ*/
   WORD16 wMaxAtten;         /*����˥��ֵ��ʵֵ*100*/
   WORD16 wMinAtten;         /*��С��˥��ֵ��ʵֵ*100*/
   WORD16 wMaxPower;         /*�Ķ���֧�ֵ������ֵ*/
   WORD16 wMaxK0;            /*�ж�����K0ֵ*/
   WORD16 wRcvCompensate;    /*�������ʱ��һ����������*/
   WORD16 wClibrateWaitTime; /*���Ŵ򿪺����ʱʱ��*/
   WORD16 swForCompensatePower; /*ǰ��Ĳ�������*/
   WORD16 swRevCompensatePower; /*����Ĳ�������*/
   WORD16 wPraskDefaultLow;
   WORD16 wPraskDefaultHigh;
   WORD16 wPraskMax;
   WORD16 wPraskMin;
   WORD16 wAntOnlineThr;
   WORD16 wLKCLAutoAdjInitVal;  /*�ڵ�ѹ��У׼�����еĳ�ʼֵ*/
   WORD16 wGateVolDefault0;
   WORD16 wGateVolDefault1;
}T_RfInitPara;

/* ��Ƶ���� */
typedef struct
{
    WORD16 wFreqWorkType;         /*��Ƶ������Ƶ*/
    WORD16 wWorkChannel;          /* ʵ��ʹ�õĵ�ǰ�����ŵ�*/
    WORD16 wFreqTableID;          /*��ǰ���õ���Ƶ��*/
    WORD16 wFixFreqChannel;       /* ��ƵƵ����*/
    WORD32 dwLbtEnable;
    WORD32 dwMaxLbtCheckTimes;
#ifdef SUPPORT_HOP_FREQ
    WORD16 wFreqHopMode;         /* ������Ƶ */
    WORD16 wHopChannelNum;       /* ��̨����Ƶ����Ŀ,�����ظ�Ƶ����*/
    WORD16 wHopChannelNo[MAX_FREQ_HOP_NUM];          /* Ƶ����,�����ڿ�����ͬƵ����*/
    WORD16 wHopChannel;            /*��ǰ����Ƶ������*/
    WORD16 wPad;
    WORD16 wHopTimer;           /* ��Ƶ��� */
    struct timeval tHopClock;       /*��Ƶʱ���ʱ��,������wFreqHopTimerʱ��tHopClock��¼Ϊ��ǰʱ�䲢��Ƶ*/
#endif
} T_UhfRfFreqHopCtrl;

/*ÿ������������Ƶ�ʿ��ƽṹ��6B��6c�Ƿֿ���*/
typedef struct tagT_UhfRfAntFreqHopCtrl
{
   T_UhfRfFreqHopCtrl  tEpcFreqHopCtrl;
   T_UhfRfFreqHopCtrl  tIso6BFreqHopCtrl;
   T_UhfRfFreqHopCtrl  tGBFreqHopCtrl;
}T_UhfRfAntFreqHopCtrl;



typedef struct
{
    BYTE ucADR38;
    BYTE ucADR39;
    BYTE ucADR3A;
    BYTE ucADR3B;
    BYTE ucADR3C;
    BYTE ucPad;
    WORD16 wPad;
} T_I2CValue;

typedef struct
{
    BYTE ucRFCBType;
    BYTE ucFwDatt;
    BYTE ucAntIndex;
    BYTE ucRwDatt;
    BYTE ucLowNoEn;
    BYTE ucRwCryseCho;   /*������ѡ��*/
    BYTE ucLo2Datt;
    BYTE ucLinkCho;
    BYTE ucRwFilter;
    BYTE ucRFCBVer;
    BYTE ucLKCLSwitch;
    BYTE ucPad;
} T_I2CFunValue;

/* ����״̬���ƽṹ�� */
typedef struct tagT_AntStatus
{
    WORD16 wAnt_Exist;        /* ������λ */ 
    WORD16 wAnt_Not_Exist;    /* ���߲���λ */
    WORD16 wAnt_Work_Err;     /* ���߹����쳣 */
    WORD16 wAnt_Working;      /* �������ڹ��� */
} T_AntStatus;

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
/**************************************************************************
 *                         ȫ�ֺ���                                    *
 **************************************************************************/
void    usp_uhf_rf_txSet(WORD16 dwRfTxAction);
void    SendAntAbnoramlAlarm(WORD16 wAntIndex, WORD32 dwAlarmType);
WORD32  uhfRf_SetLKCLPower(BYTE ucAntIndex);
void    uhfRf_RefreshPower();
void    usp_uhf_rf_setModulateDepth(WORD32 dwModulateDepth);
void    uhfRf_SetLKCLPowerPara(BYTE ucAntIndex);
void    usp_uhf_rf_writeCtrlWord(WORD16 wCrtlWord);
WORD32  usp_uhf_rf_getAntStatus(WORD16 wAntIndex);
WORD32  uhfRf_ConvChannelToFreq(WORD16 wChannelNo, WORD16 wHopTableID);
WORD32  uhfRf_startAllAction(void);
void    uhfRf_InitCtrlStruct();
WORD32 usp_uhf_rf_setPLLFreq(WORD32 dwRfFreq);
WORD16 uhfRf_getPRDacHigh(WORD16 wAntIndex);
WORD16 uhfRf_getPRDacLow(WORD16 wAntIndex);
WORD16 uhfRf_getPRMinK0(WORD16 wAntIndex);
T_UhfRfFreqHopCtrl* uhfRf_getFreqHopCtrl(WORD16 wAntIndex,WORD16 wProtocolType);
WORD16 uhfRf_getFreqWorkChannel(WORD16 wAntIndex,WORD16 wProtocolType);
WORD16 uhfRf_getFreqTableId(WORD16 wAntIndex,WORD16 wProtocolType);
void   uhfRf_warmPA(WORD16 wPTarget,WORD16 wAntIndex);
WORD32 ACPowerSetProc();
WORD16 usp_uhf_rf_queryAntVswr(WORD16 wAntIndex);
WORD16 uhfRf_CheckPLLStatus(WORD16 *pwPLLConfigStatus);
WORD32 usp_uhf_rf_autoCalibrate(BYTE ucAntIndex, WORD16 wPTarget);


#endif


