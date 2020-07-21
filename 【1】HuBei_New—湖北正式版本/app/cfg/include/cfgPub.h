#ifndef CFG_PUB_H
#define CFG_PUB_H

#include "cfg.h"
#include "cfgCapabilities.h"
#include "parson.h"
#include "logm.h"
#include "abn.h" 


/*�Ķ���ip����*/
typedef struct tag_ipv4_t
{
    unsigned int                          ip_config;        /*�Ƿ�ʹ��dhcp�����ַ,0��ʾ����,1��ʾʹ��*/ 
    unsigned int                          ip_addr;  /*�Ķ���ip��ַ*/
    unsigned int                          netmask;  /*����*/ 
    unsigned int                          gateway; /*���ص�ַ*/
    unsigned int                          dns_addr;       /*����������������ַ*/	
}ipv4_t;

//ʱ����������ò���
typedef struct tag_ntp_server_cfg_t
{
    int                                         is_open;
    unsigned int                                ntp_server_ip[READER_MAX_NTP_SVR_NUM]; /*���Ϊ2��,��һ��0��ʾ����,��0��ǰ������Ч��*/
    char                                        tz_h;    /*ʱ��,Сʱ ��Χ-24--+24*/
    char                                        tz_m;    /*ʱ��,����,-60--+60*/
    unsigned short                              ntp_serverip_num;/*ʱ�������IP����*/
    unsigned short                              ntp_period;    /* ��NTP������ͬ����ʱ������ */
}ntp_server_cfg_t;


typedef struct tag_serial_cfg_t
{
    unsigned short                          serial_index;         /*����������*/
    unsigned short                          serial_attr_index;        /*������������*/
}serial_cfg_t;


/*��������*/
typedef struct tag_keepalive_t
{
             int                        is_open ;               /*	��ѡֵ:0 :Null-����������1 :Periodic*/
    unsigned int                        period;/*�Ժ���ǵ�ʱ��.��KeepaliveTriggerType����1ʱ,��ֵ������.*/
}keepalive_t;


void get_ntp_struct_from_object(ntp_server_cfg_t  *ntp_server_cfg);
void set_ntp_struct_from_object(ntp_server_cfg_t  *ntp_server_cfg);

int check_value_between_min_max(int value,int min,int max);
int check_object_between_min_max(JSON_Object *input_object,const char *name,int min,int max);
int check_comm_serial_config(JSON_Object *input_object);
int check_comm_keepalive_config(JSON_Object *input_object);
int check_ip_gateway_dns(unsigned int ip_addr);
int check_ip_and_netmask(unsigned int ip_addr, unsigned int netmask);
int check_eeprom_ip_config(JSON_Object *input_object);
int check_file_ntp_config(JSON_Object *input_object);



#endif

