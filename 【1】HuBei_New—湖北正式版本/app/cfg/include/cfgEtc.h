#ifndef  CFG_ETC_H
#define  CFG_ETC_H
#ifdef CFG_ETC
#include "cfgPub.h"

/*etc��������*/
typedef struct tag_net_cfg_t
{
    unsigned short                          port;
    unsigned short                          pad;
}net_cfg_t;
typedef struct tag_etc_config_t
{
    unsigned char                etc_version;/*0: ETC1: ������*/
    unsigned char                n4_value;
    unsigned char                bst_interval;
    unsigned char                crc_change_flag;
    unsigned char                pll_channel_id;
    unsigned char                tx_power;
    unsigned char                module_depth;
    unsigned char                pad;
    unsigned short               rsuid_change_time;
    unsigned short               pad_w;
    unsigned int                 rsu_id;
    unsigned int                 end_app_send_num;
    int                          rsuid_change_flag;
    int                          rssi_control_flag;
    int                          check_channelId_flag;
    int                          tptl_flag;
    int                          linkage_flag;
    unsigned int                 max_wait_time;
    unsigned int                 random_wait_time;
    int                          psam_force_flag;
    unsigned int                 esam_position;
    unsigned int                 icc_position;
}etc_config_t;

typedef struct tag_comm_cfg_t
{
    ipv4_t                                      reader_ip; /*�Ķ���ip*/
    keepalive_t                                 keepalive;                     /*��������*/
    net_cfg_t                                   net_cfg;/*�������濪�أ�ֻ��Թ�����·�����48M*/
    serial_cfg_t                                serial_cfg;
    ntp_server_cfg_t                            ntp_server_cfg; /*ʱ����������ò���*/
    //unsigned short                              device_id;                    /*�Ķ������豸ID*/
}comm_cfg_t;


int set_etc_struct_to_object(etc_config_t  *etc_config);
int get_etc_struct_from_object(etc_config_t  *etc_config);
int get_comm_struct_from_object(comm_cfg_t  *comm_cfg);
int set_comm_struct_from_object(comm_cfg_t  *comm_cfg);
int check_etc_business_config(JSON_Object *input_object);
int check_etc_comm_net_config(JSON_Object *input_object);
int check_etc_file_comm_config(JSON_Object *input_object);
int check_etc_file_config(JSON_Object *input_object);
int check_etc_config(JSON_Object *input_object);
#endif
#endif