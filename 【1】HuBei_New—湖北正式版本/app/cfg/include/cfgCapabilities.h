#ifndef CFG_CAPABILITIES_H
#define CFG_CAPABILITIES_H
/***********************************************************************
                     ����Ƶ��������
***********************************************************************/
#define CFG_COMM_CAPABILITY_SERIAL_PORT_NUM 1

#define   SerialAttribute   {\
               {115200,8,0,0,0},\
               {38400,8,0,0,0},\
               {19200,8,0,0,0},\
               {9600,8,0,0,0}\
                }
#define   SERIALATTRIBUTE_LEN 1

#define   CFG_ETC_SERIAL_ATTRIBUTE   {\
               {115200,8,0,0,0}, \
               {576000,8,0,0,0} \
                }
#define   ETC_SERIALATTRIBUTE_LEN 2
struct tag_reader_serialattr_t
{
    unsigned int                baud_rate;  /*������*/
    unsigned char                data_bit; /*֧�ֵ�����λ*/
    unsigned char                stop_bit;    /*֧�ֵ�ֹͣλ*/
    unsigned char                parity;         /*֧�ֵ�У��λ*/
    unsigned char                flow_control;   /*֧�ֵĿ�������ʼֵΪ�ޣ�����Ϊ0*/
}reader_serialattr_t; 

/***********************************************************************
                     ����Ƶ�������ƽ���
***********************************************************************/




/***********************************************************************
                    ETC�������ƿ�ʼ
***********************************************************************/
#define      ETC_SUPPORT_ANTENNA_NUMBER                             (1)
#define      ETC_CAN_SETANTENNA_PROPERTIES                          (0)
#define      ETC_HAS_UTC_CLOCK_CAPABILITIES                         (1)
#define      ETC_DEVICE_MANUFACTURER_NAME                           (3902)
#define      ETC_MAX_GPI_NUM                                        (7)
#define      ETC_MAX_GPO_NUM                                        (7)
#define      ETC_PROTOCAL_NUM                                       (11) 
#define      ETC_SUPPORT_UBOOT                                      (1)


#define  CFG_ETC_Frequency      {5830000,5840000} 
#define  CFG_ETC_FREQUENCY_LEN   2
#define  CFG_ETC_HOPTABLE    {\
         {0,CFG_ETC_FREQUENCY_LEN, CFG_ETC_Frequency}\
         }
#define  CFG_ETC_MODULE_DEPTH    {\
                 {0,50},\
                 {1,60},\
                 {2,70},\
                 {3,80},\
                 {4,90}\
  }
#define CFG_ETC_MODULE_DEPTH_LEN 5
  
#define          CFG_ETC_ANT_POWER_MAX        36
#define          CFG_ETC_ANT_POWER_MIN        18
#define READER_MAX_NTP_SVR_NUM              2   /*ntp server��������ø���*/
/***********************************************************************
                    ETC�������ƽ���
***********************************************************************/

#endif
