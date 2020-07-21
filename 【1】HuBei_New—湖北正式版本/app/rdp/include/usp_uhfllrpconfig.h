#ifndef USP_UHFLLRPCONFIG
#define  USP_UHFLLRPCONFIG


//#define READER_ANTENNA_COUNT           4   /*�Ķ������֧�ֵ�������*/

#define READER_MAX_EVENT_NUM           9   /*EPC�����������*/
#define EPC_MAX_MASK_WORD_NUM           32   /*EPC�����������*/
#define MaxISO6BGroupSelectNum 10 /*����select�� ��ӦISO6BFilter����Ա��*/
#define   MaxEPCSelectNum                           3     /*���EPC ֧��select ��*/
#define   MaxGBSortNum                           1    /*���GB ֧��sort ��*/
#define MAX_NUM_SORT_FILTER_PERQUERY 1

/**************************���ߵ����õĶ���************************************************/
/*���ߵ���չ����*/

typedef struct  tagT_USPRdpLllrpRFReceiver
{
    unsigned short int wReceiverSensitivity;
    unsigned short int wPad;
}T_USPRdpLlrpRFReceiver;


typedef struct   tagT_USPRdpLlrpRFTransmitter
{
    unsigned short int wTransmitPower;
    unsigned short int wHopTableID;             /*ѡ��LLRP��������Ƶ��,��������Ƶ��׼*/
    unsigned short int wChannelIndex;
    unsigned short int wPad;
}T_USPRdpLlrpRFTransmitter;

typedef struct   tagT_USPRdpLlrpTargetTagMask
{
    unsigned short int wMemBank;                                                              /* 00: RFU�� 01: EPC�� 10: TID�� 11: User */
    unsigned short int wMaskLength;                                                          /* Mask length (bits) */
    unsigned short int wMaskBitPointer;         
    unsigned short int wPad;
    /* Starting Mask bit address */
    unsigned char ucMaskData[EPC_MAX_MASK_WORD_NUM * 2];   /* �䳤 Mask value,�õ�һ���ֶ� */
}T_USPRdpLlrpTargetTagMask;

typedef struct   tagT_USPRdpLlrpC1G2TagInventoryStateUnawareFilter
{
   /*LLRP_tEC1G2StateUnawareAction*/
    unsigned int  eC1G2StateUnawareAction;
}T_USPRdpLlrpC1G2TagInventoryStateUnawareFilter;

typedef struct   tagT_USPRdpLlrpC1G2RFControl
{
    unsigned short int wModeIndex;               /*���� �������е�*/
    unsigned short int wTari;                           /*����У����ݱ��� ѡ�����еı� �Ƚ���Tari 0 �� ��ָ������Ч*/
}T_USPRdpLlrpC1G2RFControl;        /*��·��¼�ͣ����Ʒ�ʽ��*/

typedef struct   tagT_USPRdpLlrpC1G2SingulationControl
{
    unsigned char        ucSession;
    unsigned char        ucPad;
    unsigned short int  wTagPopulation;
    unsigned int  dwTagTransitTime;
}T_USPRdpLlrpC1G2SingulationControl; /*���� �Ϳ�session �ɷ�ȷ��*/

typedef struct   tagT_USPRdpLlrpISO6BRFControl
{
    unsigned short int wModeIndex;
    unsigned short int wPad;
}T_USPRdpLlrpISO6BRFControl;

typedef struct  tagT_USPRdpLlrpISO6BFilter
{
    unsigned char       ucCommandType;
    unsigned char       ucAddress;
    unsigned char       ucMask;
    unsigned char       ucPad;
    unsigned short int wWordDataLen;
    unsigned short int   wPad;
    unsigned char       ucWordData[8];         /* �䳤 Mask value,from 0~8 byte in length. ������wWordDataLen�ֶξ���, MSB first */
}T_USPRdpLlrpISO6BFilter;

typedef struct   tagT_USPRdpLlrpISO6BFilterList
{
    unsigned char                              ucGroupSelectNum;
    unsigned char                              ucPad[3];
    T_USPRdpLlrpISO6BFilter atUSPRdpLlrpISO6BFilter[MaxISO6BGroupSelectNum];
}T_USPRdpLlrpISO6BFilterList;

typedef struct   tagT_USPRdpLlrpISO6BSingulationControl  
{
    unsigned short int  wTagPopulation;
    unsigned short int  wPad;
    unsigned int  dwTagTransitTime;
}T_USPRdpLlrpISO6BSingulationControl;

typedef struct   tagT_USPRdpLlrpISO6BInventoryCommand
{
    T_USPRdpLlrpISO6BRFControl                tUSPRdpLlrpISO6BRFControl;   /*��·��¼�͵��Ʒ�ʽ��*/
    T_USPRdpLlrpISO6BFilterList                  tUSPRdpLlrpISO6BFilter ; /*���select����filter*/
    T_USPRdpLlrpISO6BSingulationControl    tUSPRdpLlrpISO6BSingulationControl;/*Ԥ����ǩ��*/
}T_USPRdpLlrpISO6BInventoryCommand;

typedef struct    tagT_USPRdpLlrpC1G2Filter
{
    T_USPRdpLlrpTargetTagMask tUSPRdpLlrpEPCTargetTagMask;
     /*LLRP_tEC1G2TruncateAction���unsigned int*/
    unsigned int  eC1G2TruncateAction;
    T_USPRdpLlrpC1G2TagInventoryStateUnawareFilter tUSPRdpLlrpC1G2TagInventoryStateUnawareFilterAction;
}T_USPRdpLlrpC1G2Filter; /*���룬select�������*/

typedef struct   tagT_USPRdpLlrpC1G2FilterList
{
    unsigned short int                       wFilterNum;
    unsigned short int                       wPad;
    T_USPRdpLlrpC1G2Filter atUSPRdpLlrpC1G2Filter[MaxEPCSelectNum];
}T_USPRdpLlrpC1G2FilterList;

typedef struct   tagT_USPRdpLlrpC1G2InventoryCommand
{
    unsigned short int                                            wTagInventoryStateAware;
    unsigned short int                                            wPad;
    /*��֧���Ƿ񻹴��� ����ṹ ����һ�� ����У�����*/
    T_USPRdpLlrpC1G2FilterList                 tUSPRdpLlrpC1G2Filter;                       /*select ����*/
    T_USPRdpLlrpC1G2RFControl               tUSPRdpLlrpC1G2RFControl;                /*��·���ʺ͵��Ʒ�ʽ��*/
    T_USPRdpLlrpC1G2SingulationControl  tUSPRdpLlrpC1G2SingulationControl;
}T_USPRdpLlrpC1G2InventoryCommand;

typedef struct   tagT_USPRdpLlrpGBRFControl
{
    unsigned short int wModeIndex;               /*���� �������е�*/
    unsigned short int wPad;                           
}T_USPRdpLlrpGBRFControl;        

typedef struct   tagT_USPRdpLlrpGBSingulationControl
{
    unsigned short int  wCCN;
    unsigned short int  wCIN;
    unsigned char        ucSession;
    unsigned char        ucGBQureySel;
    unsigned char        ucGBQureyAction;
    unsigned char        ucPad;
}T_USPRdpLlrpGBSingulationControl; /*���� �Ϳ�session �ɷ�ȷ��*/
typedef struct   tagT_USPRdpLlrpGBTagInventoryStateAwareFilterAction
{
    unsigned char  ucGBStateAwareTarget;
    unsigned char  ucGBStateAwareAction;
    unsigned short int wPad;
}T_USPRdpLlrpGBTagInventoryStateAwareFilterAction;

typedef struct    tagT_USPRdpLlrpGBFilter
{
    T_USPRdpLlrpTargetTagMask tUSPRdpLlrpGBTargetTagMask;
    T_USPRdpLlrpGBTagInventoryStateAwareFilterAction tUSPRdpLlrpGBTagInventoryStateAwareFilterAction;
}T_USPRdpLlrpGBFilter; /*���룬select�������*/

typedef struct   tagT_USPRdpLlrpGBFilterList
{
    unsigned short int                       wFilterNum;
    unsigned short int                       wPad;
    T_USPRdpLlrpGBFilter atUSPRdpLlrpGBFilter[MAX_NUM_SORT_FILTER_PERQUERY];
}T_USPRdpLlrpGBFilterList;

typedef struct   tagT_USPRdpLlrpGBInventoryCommand
{
    /*��֧���Ƿ񻹴��� ����ṹ ����һ�� ����У�����*/
    T_USPRdpLlrpGBFilterList                 tUSPRdpLlrpGBFilter;                       /*select ����*/
    T_USPRdpLlrpGBRFControl               tUSPRdpLlrpGBRFControl;                /*��·���ʺ͵��Ʒ�ʽ��*/
    T_USPRdpLlrpGBSingulationControl  tUSPRdpLlrpGBSingulationControl;
}T_USPRdpLlrpGBInventoryCommand;

typedef struct   tagT_USPRdpLlrpAirProtocolInventoryCommand
{
    T_USPRdpLlrpC1G2InventoryCommand  tUSPRdpLlrpC1G2InventoryCommand;
    T_USPRdpLlrpISO6BInventoryCommand tUSPRdpLlrpISO6BInventoryCommand;
    T_USPRdpLlrpGBInventoryCommand tUSPRdpLlrpGBInventoryCommand;
}T_USPRdpLlrpAirProtocolInventoryCommand;

typedef struct   tagT_USPRdpLlrpAntennaConfiguration
{
    unsigned char                                                          ucAntID;    /* ���߱�� */
    unsigned char                                                          ucPad[3];        
    //T_USPRdpLlrpRFReceiver                             tUSPRdpLlrpRFReceiverSettings;
    T_USPRdpLlrpRFTransmitter                         tUSPRdpLlrpRFTransmitterSettings;          /*�Ƿ������ṹ���һ���ṹ*/
    T_USPRdpLlrpAirProtocolInventoryCommand tUSPRdpLlrpAirProtocolInventoryCommandSettings;   
}T_USPRdpLlrpAntennaConfiguration;


typedef struct tagT_USPRdpLlrpReaderEventNotificationSpec
{
          /*LLRP_tENotificationEventType���unsigned int*/
    unsigned int          tNotificationEventType;   /*�¼���,�и����л���index-value��������ʾ*/
    unsigned int                                      bNotificationState;/*֪ͨ��״̬*/
}T_USPRdpLlrpReaderEventNotificationSpec;

 /*�¼��ϱ�֪ͨ������*/
  typedef struct tagT_USPRdpLlrpAntennaProperties
{
    unsigned short int             wAntennaID;      /*���߱��*/   
    signed short int           swAntennaGain;      /*��������*/ 
    unsigned int            bAntennaConnected;    /*�����Ƿ�����False = δ����.True = ����*/
}T_USPRdpLlrpAntennaProperties;

typedef struct  tagT_USPRdpLlrpTagReportContentSelector
{
    unsigned int                  bEnableROSpecID;
    unsigned int                  bEnableSpecIndex;
    unsigned int                  bEnableInventoryParameterSpecID;
    unsigned int                  bEnableAntennaID;
    unsigned int                  bEnableChannelIndex;
    unsigned int                  bEnablePeakRSSI;
    unsigned int                  bEnableFirstSeenTimestamp;
    unsigned int                  bEnableLastSeenTimestamp;
    unsigned int                  bEnableTagSeenCount;
    unsigned int                  bEnableAccessSpecID;
    unsigned int                  bEnablePC;
    unsigned int                  bEnableCRC;

}T_USPRdpLlrpTagReportContentSelector;

typedef struct tagT_USPRdpLlrpROReportConfig
{
     /*LLRP_tEROReportTriggerType���unsigned int*/
    unsigned int                              eROReportTrigger;
    unsigned short int                                                            wThresholdOfReportNum;               
    unsigned short int                                                            wPad;
    T_USPRdpLlrpTagReportContentSelector               tRdpLlrpTagReportContentSelector;
}T_USPRdpLlrpROReportConfig;
/*Accesss ����������ֻ������*/
typedef struct tagT_USPRdpLlrpAccessReportConifg
{
    /*LLRP_tEROReportTriggerType���unsigned int*/
    unsigned int  eAccessReportTriggerType; /* 0    Whenever ROReport is generated for the RO that    triggered the execution of this AccessSpec.   1    End of AccessSpec */
}T_USPRdpLlrpAccessReportConifg;

typedef struct tagT_USPRdpLlrpEventsAndReports
{
   unsigned int  bHoldEventsAndReportsUponReconnect ;  /*False��ʾ�����汨�����¼�,True���ʾ����.*/
}T_USPRdpLlrpEventsAndReports;


/* �Ķ�������6B6C���书�� */


/*ʹ�ö�Ƶ���Ƶ���������*/
typedef struct  tagT_USPRdpLlrpRfFreqCfg
{
    T_USPUHFRfFreqCfg      tUSPUHFRfFreqCfg6B;
    T_USPUHFRfFreqCfg      tUSPUHFRfFreqCfg6C;
    T_USPUHFRfFreqCfg      tUSPUHFRfFreqCfgGB;
} T_USPRdpLlrpRfFreqCfg;
typedef struct  tagT_USPRdpLlrpTransmitPower
{
    unsigned short int wTransmitPower6B;
    unsigned short int wTransmitPower6C;
    unsigned short int wTransmitPowerGB;
}T_USPRdpLlrpTransmitPower;

/*���ߵ���չ����*/
typedef struct tagT_USPRdpLlrpAntennaExtConfiguration
{
    unsigned short int                               wAntennaID ;                                 /*�����ֵΪ0,���ʾ������Ӧ������������*/
     /*LLRP_tEForandRevConfigType���unsigned int*/
    unsigned int eForandRevConfigType;            /*��ѡֵ:0 :ֻ����������1 :ֻ����������2 :�շ�һ��*/
    T_USPRdpLlrpRfFreqCfg      tUSPRdpLlrpRfFreqCfg;   /*ʹ�ö�Ƶ���Ƶ���������*/
    T_USPRdpLlrpTransmitPower   tTransmitPower; 
}T_USPRdpLlrpAntennaExtConfiguration;

typedef struct tagT_USPRdpLlrpAntSwitchDelayCfg
{
    unsigned char        ucTurnOnDelay; /*0��ʾ�ر������л���ʱ��1��ʾ����*/
    unsigned char        ucPad;
    unsigned short int   wDelayTime; /*��ʱʱ�䣬��λms��ȡֵ��Χ[10,1000]*/
}T_USPRdpLlrpAntSwitchDelayCfg;

typedef struct tagT_USPRdpLlrpTagFilterCfg
{
    unsigned char              ucTurnOnOPFilter;//0��ʾ�رղ������ˣ�1��ʾ����
    unsigned char              ucTurnOnRFFilter;//0��ʾ�ر��ϱ����ˣ�1��ʾ����
    unsigned char              ucOPKeepaliveTime;//�������˱���ʱ�䣬ȡֵ��Χ[1,255]
    unsigned char              ucOPPollTime;//����������ѯʱ�䣬�̶�Ϊ20
    unsigned char              ucRFKeepaliveTime;//�ϱ����˱���ʱ�䣬ȡֵ��Χ[1,255]
    unsigned char              ucRFPollTime;//�ϱ�������ѯʱ�䣬�̶�Ϊ20
    unsigned short int         wPad;
}T_USPRdpLlrpTagFilterCfg;

typedef struct tagT_USPRdpLlrpPerAntUDPCfg
{
    unsigned int        dwReaderIP; /*�߽����������Ķ���IP */
    unsigned char              ucAntIndex; /*�߽����ߺţ�1-4*/
    unsigned char              ucLinkNo;//��·�ţ��̶�Ϊ5��6
    unsigned short int        wPort;//�˿ںţ��̶�Ϊ8088
}T_USPRdpLlrpPerAntUDPCfg;

typedef struct tagT_USPRdpLlrpAntCoopUDPCfg
{
    unsigned char                  ucAntNum; /*�ٽ�����������ȡֵΪ1 2*/
    unsigned char                  ucT1;//�̶�Ϊ20ms
    unsigned char                  ucN1;//�̶�Ϊ2
    unsigned char                  ucPad;//����ֽڣ���֤����
    unsigned short int             wT2;//�̶�Ϊ1000ms
    unsigned short int             wPad;
    T_USPRdpLlrpPerAntUDPCfg  atPerAntUDPCfg [2]; /*Udp��ip����*/
}T_USPRdpLlrpAntCoopUDPCfg; 

typedef struct tagT_USPRdpLlrpAntCoopCfg
{
    unsigned char                       ucTurnOnAntCoop; /*�����������أ�0��ʾ�رոù��ܣ�1��ʾUDP������ʽ��2��ʾGPIO���� ��ʽ���ݲ�֧�֣�*/
    unsigned char                       ucPad;
    unsigned short int                  wPad;
    T_USPRdpLlrpAntCoopUDPCfg    tAntCoopUDPCfg; /*UDP��������*/
}T_USPRdpLlrpAntCoopCfg;

typedef struct tagT_USPRdpLlrpRSSISortCfg
{
    unsigned char        ucTurnOnSort; /*0��ʾ�ر������ܣ�1��ʾ����*/
    unsigned char        ucPad;
    unsigned short int   wSortNodeNum;/*�̶�Ϊ5���궨��*/
    unsigned short int   wOTTime; /*����ʱʱ�䣬��λΪs��ȡֵ��Χ[1,65535]��Ĭ��ֵΪ30*/
    unsigned short int   wpad;
} T_USPRdpLlrpRSSISortCfg;

typedef struct tagT_USPRdpLlrpAdvancedConfig
{
    T_USPRdpLlrpTagFilterCfg                                   tTagFilterCfg; 
}T_USPRdpAdvancedConfig;

 typedef struct tagT_RdpConfig
{
    T_USPRdpLlrpReaderEventNotificationSpec     tUSPRdpLlrpReaderEventNotificationSpec[READER_MAX_EVENT_NUM];/*�¼��ϱ�֪ͨ������READER_MAX_EVENT_NUM = 9*/ 
    T_USPRdpLlrpAntennaProperties                    tUSPLlrpRdpAntennaProperties[READER_ANTENNA_COUNT];/*һ����������  READER_ANTENNA_COUNT = 4*/
    T_USPRdpLlrpAntennaConfiguration               tUSPRdpLlrpAntennaConfiguration[READER_ANTENNA_COUNT];/*һ�����ߵ�����*/ 
    T_USPRdpLlrpROReportConfig                       tROReportSpec;                       /*ro��������*/
    T_USPRdpLlrpAccessReportConifg                 tAccessReportSpec ;                  /*ACCESS��������*/
    T_USPRdpLlrpEventsAndReports                    tEventsAndReports;                    /*�������汨������*/    
    T_USPRdpLlrpAntennaExtConfiguration           trAntennaExtConfiguration[READER_ANTENNA_COUNT];      /*���ߵ���չ����*/     
    T_USPRdpAdvancedConfig                        tUSPRdpAdvancedConfig;
}T_RdpConfig;
 


#endif
