#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/prctl.h>
#include <sys/time.h> 
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include "zmq.h"
#include "zmq_utils.h"
#include "parson.h"
#include "cfg.h"
#include "logm.h"
#include "abn.h"


void *rep_socket=NULL;
void *pub_socket=NULL;
JSON_Object *root_object=NULL;    /* ������ */
JSON_Value *root_value = NULL;
char send_buf[CFG_MAX_MSG_LEN]={0};
char recv_buf[CFG_MAX_MSG_LEN]={0};
msg_func_t msg_func[]=
{
    {"cli","set",set_inner_config},
    {"cli","get",get_inner_config},
    {"cli","save",save_inner_config},
    {"restore","restore",restore_factory_defaults},
    {"reset","reset",reset_reader},
    {"comm","request",deal_comm_request_config},
    {"uhf","request",deal_uhf_request_config},
    {"rfs","request",deal_rfs_request_config},
    {"rfs_change","request",deal_rfs_change_request_config},
    {"dispatch","request",deal_dispatch_request_config},
    {"client","set",deal_client_set_config},
    {"client","get",deal_client_get_config},
    {"client_comm","set",set_client_comm_config},
    {"client_comm","get",get_client_comm_config},
    {"client_business","set",set_client_business_config},
    {"client_business","get",get_client_business_config},
    {"cmm","keepalive",deal_cmm_keepalive_msg},  
};

/*ϵͳ��λ*/
int reset_system()
{    
    int rc=0;
    /* ʹ�õ��帴λ�ӿڽ��и�λ��������û�иýӿڣ�����ò���ϵͳ��reboot���� */
    #if CPU_TYPE_ppc_8313 || CPU_TYPE_ppc_8306
    //usp_drv_ioport_init();
    //usp_drv_epld_init();   
    //g_pvUSPDrvEpldVirtStartAddr0[0x62] = 0;
    
    #else   
    CFG_DEBUG("system reboot\r\n");
    //system("reboot");    
    #endif
    
    return rc;
}

/*���̳�ʼ��*/
static void init_process()
{
    prctl(PR_SET_NAME, "cfg"); /* �������� */
    setpriority(PRIO_PROCESS, getpid(), 1); /* �������ȼ� */
    return;
}

/*�ļ��洢��jsonת��Ϊ����*/
JSON_Object *parse_file_to_object(const char *filename,JSON_Value **file_value)
{
    JSON_Object *file_object=NULL;
    JSON_Value *tmp_value=NULL;
    CFG_DEBUG(" parse_file_to_object \r\n");
 
    /* ����Ĭ������ */
    tmp_value= json_parse_file(filename); 
    if(NULL==tmp_value)
    {
       CFG_DEBUG("(parse_file_to_object) error null pointer\r\n");
	   abn_debug("null pointer %s\r\n",filename);
       return NULL;
    }
    
   if(json_value_get_type(tmp_value) == JSONObject)
   {
      file_object = json_value_get_object(tmp_value);
      *file_value = tmp_value;
   }
   else
   {
       abn_debug("error! not an object type=%d\r\n",json_value_get_type(tmp_value));
       CFG_DEBUG("(parse_file_to_object) error! not an object type=%d\r\n",json_value_get_type(tmp_value));
       json_value_free(tmp_value);
       return NULL;
   }
    
    return file_object;
}

/*ת��string���͵�json Ϊ����*/
JSON_Object *parse_string_to_object(const char *strings,JSON_Value **string_value)
{
    JSON_Object *tmp_object=NULL;
    JSON_Value *tmp_value=NULL;
    CFG_DEBUG("parse_string_to_object \r\n");
 
    /* ����Ĭ������ */
    tmp_value= json_parse_string(strings);
    *string_value = NULL;
    if(NULL==tmp_value)
    {
       CFG_DEBUG("(parse_string_to_object) error null pointer\r\n");
	   abn_debug(" null pointer error\r\n");
       return NULL;
    }

   if(json_value_get_type(tmp_value) == JSONObject)
   {
      tmp_object = json_value_get_object(tmp_value);
      *string_value = tmp_value;
   }
   else
   {
       CFG_DEBUG("error! not an object type=%d\r\n",json_value_get_type(tmp_value));
	   abn_debug("error! not an object type=%d\r\n",json_value_get_type(tmp_value));
       json_value_free(tmp_value);
       *string_value=NULL;
       return NULL;
   }
  
    return tmp_object;
}

static int set_eeprom_cfg_to_object()
{
    int          rc=0;
    unsigned int          len=0;
    cfg_eeprom_t    eeprom_;
    JSON_Object *cur_object=NULL;

    CFG_DEBUG("set_eeprom_cfg_to_object \r\n");
    return 0;
    /* ��ȡnvram */
     bsp_eeprom_rcv(EEPROM_IP_ADDR, sizeof(eeprom_), (unsigned char *)&eeprom_, &len);
    if (0 != rc || sizeof(eeprom_) != len)
    {   
        abn_debug("rc=%d len=%d eepromlen=%d\r\n",rc,len,sizeof(eeprom_));
        return rc;   
    }
    
    /* �����ϲ����� */
    cur_object = json_object_get_object(root_object,"eeprom");
    //json_object_set_number(cur_object,"ReaderID",eeprom_.reader_id);
    json_object_set_number(cur_object,"CommMode",eeprom_.comm_mode);
	json_object_set_number(cur_object,"IPConfig",eeprom_.ip_config);
    json_object_set_number(cur_object,"IPAddress",eeprom_.ip_addr);
    json_object_set_number(cur_object,"Netmask",eeprom_.netmask);
    json_object_set_number(cur_object,"Gateway",eeprom_.gateway);
    json_object_set_number(cur_object,"DNS",eeprom_.dns);

	json_object_set_number(cur_object,"IPAddress2",eeprom_.ip_addr2);
    json_object_set_number(cur_object,"Netmask2",eeprom_.netmask2);
    json_object_set_number(cur_object,"Gateway2",eeprom_.gateway2);
    json_object_set_number(cur_object,"DNS2",eeprom_.dns2);
    return 0;
}
static int set_object_value_to_eeprom()
{
    int          rc=0;
    unsigned int          len=0;
    cfg_eeprom_t    eeprom_;
    JSON_Object *cur_object=NULL;

    CFG_DEBUG("set_object_value_to_eeprom \r\n");
    return 0;
    /* ��ȡnvram */
    rc = bsp_eeprom_rcv(EEPROM_IP_ADDR, sizeof(eeprom_), (unsigned char *)&eeprom_, &len);
    if (0 != rc || sizeof(eeprom_) != len)
    {   
        abn_debug("rc=%d len=%d eepromlen=%d\r\n",rc,len,sizeof(eeprom_));
        return rc;   
    }
    /* �����ϲ����� */
    cur_object = json_object_get_object(root_object,"eeprom");
	eeprom_.comm_mode=(unsigned char)json_object_get_boolean(cur_object,"CommMode");
    eeprom_.ip_config=(unsigned short)json_object_get_boolean(cur_object,"IPConfig");
    eeprom_.ip_addr=(unsigned int)json_object_get_number(cur_object,"IPAddress");
    eeprom_.netmask=(unsigned int)json_object_get_number(cur_object,"Netmask");
    eeprom_.gateway=(unsigned int)json_object_get_number(cur_object,"Gateway");
    eeprom_.dns=(unsigned int)json_object_get_number(cur_object,"DNS");

    eeprom_.ip_addr2=(unsigned int)json_object_get_number(cur_object,"IPAddress2");
    eeprom_.netmask2=(unsigned int)json_object_get_number(cur_object,"Netmask2");
    eeprom_.gateway2=(unsigned int)json_object_get_number(cur_object,"Gateway2");
    eeprom_.dns2=(unsigned int)json_object_get_number(cur_object,"DNS2");
    /* д��nvram */
    rc = bsp_eeprom_rcv(EEPROM_IP_ADDR, sizeof(eeprom_), (unsigned char *)&eeprom_, &len);
    if (0 != rc || sizeof(eeprom_) != len)
    {   
        abn_debug("rc=%d len=%d eepromlen=%d\r\n",rc,len,sizeof(eeprom_));
        return rc;   
    }
    return 0;
}


/*����ģ���ʼ��*/
static int init_cfg()
{
    JSON_Value *file_value=NULL;
    CFG_DEBUG("init_cfg \r\n");
 
    /* ����Ĭ������ */
    root_object=parse_file_to_object(CFG_DEFAULT_CONFIG_FILE,&root_value);
    CHECK_NULL_POINTER_RETURN_INT(root_object);
    /* �����ļ����� */
    parse_file_to_object(CFG_CONFIG_FILE,&file_value);
    if(NULL==file_value)
    {
       CFG_DEBUG("(init_cfg)parse_file_to_object,maybe default cfg file not exist \r\n");
    }
    else
    {
       json_object_remove(root_object,"file");
       json_object_set_value(root_object,"file",file_value);
    }
    
    
    /* ����ʵ�ʵ�������Ϣ */  
    set_eeprom_cfg_to_object();
    return 0;
}


#if 0
/**************************************************************************
* �������ƣ�cfg_init_process
* ����������USPƽ̨��ʼ��������Ӳ���豸��CPU�����硢��ӡ���Ƶȵȡ�           
* ���ʵı���
* �޸ĵı���
* �����������
* �����������
* �� �� ֵ����
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 09/08/18    V1.0       �߷�        ����
**************************************************************************/
static VOID cfg_init_process(VOID)
{    
    /* ����CoreDump */
    #if USP_VERSION_DEBUG
    unsigned int      dwRet;
    dwRet = usp_oss_vos_configCoredump();
    if (USP_SUCCESS != dwRet)
    {
        SCP_EXCEPT(dwRet, 0, 0, 0);
    }
    SCP_DEBUG("(scp_initApp) config core dump ret = %u \r\n", dwRet);
    #endif

    /* ��ʼ���������̼��Ͷ�ջdump */
    SCP_DEBUG("(scp_initApp) set check zombie and stack dump \r\n");    
    scp_setZombieAndStackdump();
    
    return;
}
#endif

void set_zmq_socket_option(int type,int value)
{
    int rc=0;
    rc =zmq_setsockopt(rep_socket,type,&value,sizeof(int));
    CHECK_FAIL_NUMBER(rc,"rep_socket zmq_setsockopt fail");
}


/*������Ϣ����*/
void create_zmq_sockets()
{
    void *ctx=NULL;
    int value=0;
    int ret=0;

    ctx = zmq_init (1);
    if(NULL == ctx)
    {
        CFG_DEBUG("create context fail!\n");
		abn_debug("zmq_init error:%s\r\n",zmq_strerror(errno));
        exit(-1);
    }
    
    /*����tcp://127.0.0.1:5504��Ϣ����*/
    rep_socket = zmq_socket(ctx, ZMQ_REP);
    if(NULL == rep_socket)
    {
        CFG_DEBUG("create tcp rep_socket fail!\n");
		abn_debug("error:%s errno:%d\r\n",zmq_strerror(errno),errno);
        exit(-1);
    }

    set_zmq_socket_option(ZMQ_LINGER,0);

    /*set_zmq_socket_option(ZMQ_TCP_KEEPALIVE,0);
    set_zmq_socket_option(ZMQ_TCP_KEEPALIVE_CNT,2);
    set_zmq_socket_option(ZMQ_TCP_KEEPALIVE_IDLE,1);
    set_zmq_socket_option(ZMQ_TCP_KEEPALIVE_INTVL,1);*/

    ret = zmq_bind(rep_socket, "tcp://*:5504");
    CHECK_FAIL_NUMBER(ret,"bind tcp_rep_socket fail");

    /*����pub ��Ϣ����*/
    pub_socket = zmq_socket(ctx, ZMQ_PUB);
    if(NULL == pub_socket)
    {
        CFG_DEBUG("create pub_socket fail!\n");
		abn_debug("error:%s errno:%d\r\n",zmq_strerror(errno),errno);
        exit(-1);
    }
    
    ret = zmq_bind(pub_socket, "tcp://*:5505");
    CHECK_FAIL_NUMBER(ret,"bind pub socket fail");
    return;
}
/*���ñ����Ϣ*/
void send_config_change_msg(const char *content)
{
    int rc=0;     
    JSON_Value *tmp_value=NULL;
    JSON_Value *cur_value=NULL;
    JSON_Object *tmp_object=NULL;
 
    CFG_DEBUG("send_config_change_msg content=%s\r\n",content);
    if(NULL==content)
    {
       CFG_DEBUG("content is null\r\n");
    }
    tmp_value=json_value_init_object();
    tmp_object=tmp_value->value.object;
    json_object_set_string(tmp_object,"type","change_cfg");
    json_object_set_string(tmp_object,"action","change");
    if(NULL!=content)
    {
        json_object_set_string(tmp_object,"content",content);
    }
    memset(send_buf,0,sizeof(send_buf));
    rc=json_serialize_to_buffer(tmp_value,send_buf,CFG_MAX_MSG_LEN);
    /*�ͷ�*/
    json_value_free(tmp_value);
    CFG_DEBUG("json_serialize_to_buffer rc=%d \r\n",rc);
    rc=CFG_SEND_MSG(pub_socket,send_buf,strlen(send_buf)+1);
    CFG_DEBUG("pub_socket send over rc=%d\r\n",rc);
}

/*������Ӧ��Ϣ*/
void send_response_msg(const char *type,int result,const char *content)
{
    int rc=0;     
    JSON_Value *tmp_value=NULL;
    JSON_Value *cur_value=NULL;
    JSON_Object *tmp_object=NULL;
 
    //CFG_DEBUG("send_response_msg content=%s\r\n",content);
    tmp_value=json_value_init_object();
    CHECK_NULL_POINTER_RETURN_VOID(tmp_value);
    tmp_object=tmp_value->value.object;
    json_object_set_string(tmp_object,"type",type);
    json_object_set_number(tmp_object,"result",result);
    if(NULL!=content)
    {
       json_object_set_string(tmp_object,"content",content);
    }
    
    memset(send_buf,0,sizeof(send_buf));
    rc=json_serialize_to_buffer(tmp_value,send_buf,CFG_MAX_MSG_LEN);
    /*�ͷ�*/
    json_value_free(tmp_value);
    CFG_DEBUG("json_serialize_to_buffer send_buf = %s \r\n", send_buf);    
    CFG_SEND_MSG(rep_socket,send_buf,strlen(send_buf)+1);
}

/*ԭʼ��Ϣ����*/
void deal_raw_msg(char *msg)
{
    /*��Ϣ̫��ʹ�ñ���������*/
    
    int loop = 0;
    const char *tpye=NULL;
    const char *action=NULL;
    const char *content=NULL;
    JSON_Object *cur_object=NULL;
    JSON_Object *content_object=NULL;
    JSON_Value *cur_value=NULL;
    CFG_DEBUG("deal_raw_msg\r\n");
    cur_object = parse_string_to_object(msg,&cur_value);
    tpye=json_object_get_string(cur_object,"type");
    action=json_object_get_string(cur_object,"action");
    CFG_DEBUG("deal_raw_msg type=%s action=%s\r\n",tpye,action);
    
    for (loop = 0; loop < sizeof(msg_func) / sizeof(msg_func_t);loop++)
    {
        if ((0==strcmp( tpye,msg_func[loop].type))
           &&(0==strcmp( action,msg_func[loop].action)))
        {
            content=json_object_get_string(cur_object,"content");
            if(NULL == content)
            {
                CFG_DEBUG("content is null \n");
            }
            msg_func[loop].msg_func(content);
            break;
        }
    }
    json_value_free(cur_value);
    /*��ջ�����*/
    memset(recv_buf, 0, sizeof(recv_buf));
    if (loop == (sizeof(msg_func) / sizeof(msg_func_t)) )
    {
        /*д���쳣*/
        CFG_DEBUG("CFG receive unsupport msg type=%s\r\n",tpye);
		send_response_msg("unsupported",-1,NULL);
    }
}

/*����������Ϣ*/
int save_config()
{
    int rc=0;
    JSON_Value *file_value=NULL;
    
    CFG_DEBUG("save_config\r\n");
    /* ���浽�ļ�*/
    file_value=json_object_get_value(root_object,"file");
    CHECK_NULL_POINTER_RETURN_INT(file_value);
    CFG_DEBUG("file_value->type=%d\r\n",file_value->type);
    rc=json_serialize_to_file(file_value,CFG_CONFIG_FILE);
    CFG_DEBUG("serialize file_value->type=%d\r\n",file_value->type);
    CHECK_NONZERO_NUMBER(rc);
    
    /*���浽eeprom*/
    rc=set_object_value_to_eeprom();
    if(0!=rc)
    {
        CFG_DEBUG("set_object_value_to_eeprom fail\r\n");
		abn_debug("set_object_value_to_eeprom fail rc=%d\r\n",rc);
        return rc;
    }
    
    /* �����ñ��֪ͨ */
    //send_config_change_msg();
    
    /* ��¼��־ */
	log_debug("save config\r\n");
    return rc;
}

/*�ָ���������*/
static int factory_reset()
{
    JSON_Object *file_object=NULL;
    JSON_Value *file_value=NULL;
    int rc=0;
    CFG_DEBUG("factory_reset \r\n");
 
    /* ����json value */
    json_value_free(root_value);
    CFG_DEBUG("(factory_reset) root_value=0x%x root_object=0x%x \r\n",(unsigned int)root_value,(unsigned int)root_object);
    root_value=NULL;
    root_object=NULL;
    
    root_object=parse_file_to_object(CFG_DEFAULT_CONFIG_FILE,&root_value);
    CHECK_NULL_POINTER_RETURN_INT(root_object);
    rc=save_config();
	log_debug("factory_reset\r\n");
    return rc;
}

/*��λ��Ϣ����*/
void reset_reader(const char *content)
{
    int rc=0; 
    CFG_DEBUG("reset_reader\r\n");
        
    rc=reset_system();
    log_debug("reset reader rc:%d\r\n",rc);
    /*������Ӧ*/
    send_response_msg("reset_ack",rc,NULL);
}

/*�ڲ�ģ����������*/
void set_inner_config(const char *content)
{
    int rc=0; 
    JSON_Value *input_value=NULL;
    JSON_Object *input_object=NULL;
    JSON_Value *tmp_value=NULL;
    char * tmpString=NULL;
    CFG_DEBUG("set_inner_config content=%s\r\n",content);
    
    //CHECK_NULL_POINTER_RETURN_VOID(root_object);
    if(NULL==root_object)
    {
         /*������Ӧ*/
        CFG_DEBUG("set_inner_config root_object error : null pointer\r\n");
		abn_debug("root_object is null\r\n");
        send_response_msg("set_ack",-1,NULL);
        return;
    }
    
    input_object=parse_string_to_object(content,&input_value);
    if(NULL==input_object)
    {
         /*������Ӧ*/
        CFG_DEBUG("set_inner_config input_object error : null pointer\r\n");
		abn_debug(" null pointer error\r\n");
        send_response_msg("set_ack",-1,NULL);
        return;
    }
    CFG_DEBUG("input_object->names[0]=%s type=%d\r\n",input_object->names[0],input_object->values[0]->type);
    /*�ж������Ƿ����*/
    tmp_value=json_object_dotget_value(root_object,input_object->names[0]);
    if((NULL==tmp_value)||(tmp_value->type!=input_object->values[0]->type))
    {
         /*������Ӧ*/
		abn_debug("tmp_value=0x%x\r\n",tmp_value);
        CFG_DEBUG("set_inner_config  object type is not the same error\r\n");
        if(NULL!=tmp_value)
        {
            abn_debug("%d,%d\r\n",tmp_value->type,input_object->values[0]->type);
            CFG_DEBUG("the old type is %d\r\n",tmp_value->type);
        }
		
        send_response_msg("set_ack",-1,NULL);
        return;
    }
    
    rc=json_object_dotset_value(root_object,input_object->names[0],input_object->values[0]);

    tmpString=json_serialize_to_string(root_value);
    printf("%s\r\n",tmpString);
    json_free_serialized_string(tmpString);
    
    /*������Ӧ*/
    send_response_msg("set_ack",rc,NULL);
}

/*�ڲ�ģ���ȡ����*/
void get_inner_config(const char *content)
{
    int rc=0; 
    int loop=0;
    JSON_Value *tmp_value=NULL;
    JSON_Value *cur_value=NULL;
    JSON_Value *set_value=NULL;
    JSON_Object *tmp_object=NULL;

    char *tmp_string=NULL;
    CFG_DEBUG("get_inner_config content=%s\r\n",content);
    
    //CHECK_NULL_POINTER_RETURN_VOID(content);
    if(NULL==content)
    {
         /*������Ӧ*/
        CFG_DEBUG("get_inner_config content error : null pointer\r\n");
		abn_debug("null pointer error\r\n");
        send_response_msg("get_ack",-1,NULL);
        return;
    }
    if(NULL==root_object)
    {
         /*������Ӧ*/
        CFG_DEBUG("get_inner_config root_object error : null pointer\r\n");
		abn_debug("root_object is null\r\n");
        send_response_msg("get_ack",-1,NULL);
        return;
    }   
    cur_value=json_object_dotget_value(root_object,content);
    if(NULL==cur_value)
    {
       rc=-1;
       /*������Ӧ*/
	   abn_debug("json_object_dotget_value error\r\n");
       CFG_DEBUG("get_inner_config key error\r\n");
       send_response_msg("get_ack",-1,NULL);
       return;
    }
    CFG_DEBUG("cur_value->type=%d \r\n",cur_value->type);
    set_value=json_value_deep_copy(cur_value);
    /*������Ӧ*/   
    tmp_value=json_value_init_object();
    CHECK_NULL_POINTER_RETURN_VOID(tmp_value);
    tmp_object=tmp_value->value.object;
    CHECK_NULL_POINTER_RETURN_VOID(tmp_object);
    json_object_set_string(tmp_object,"type","get_ack");
    json_object_set_number(tmp_object,"result",rc);
    json_object_set_value(tmp_object,"value",set_value);
    memset(send_buf,0,sizeof(send_buf));
    rc=json_serialize_to_buffer(tmp_value,send_buf,CFG_MAX_MSG_LEN);
    CFG_DEBUG("json_serialize_to_buffer and send result = %u \r\n", rc);
    CFG_SEND_MSG(rep_socket,send_buf,strlen(send_buf)+1);
    /*�ͷ�*/
    json_value_free(tmp_value);
}

/*�ڲ�ģ�鱣������*/
void save_inner_config(const char *content)
{
    int rc=0; 
    int loop=0;
    JSON_Value *tmp_value=NULL;
    JSON_Object *tmp_object=NULL;
    char *tmp_string=NULL;
	if(NULL==root_object)
    {
         /*������Ӧ*/
        CFG_DEBUG("save_inner_config root_object error : null pointer\r\n");
		abn_debug("root_object is null\r\n");
        send_response_msg("save_ack",-1,NULL);
        return;
    } 
    /*����*/
    rc=save_config();
    /*�������ñ����Ϣ*/
    tmp_string=json_serialize_to_string(root_value);
    send_config_change_msg(tmp_string);
    json_free_serialized_string(tmp_string);
    /*�ظ���Ӧ*/
    send_response_msg("save_ack",rc,NULL);
}

/*�ָ���������*/
void restore_factory_defaults(const char *content)
{
    int rc=0; 
    int loop=0;
    JSON_Value *tmp_value=NULL;
    JSON_Object *tmp_object=NULL;
    
    /* �ָ����� */
    rc=factory_reset();

    /*�ظ���Ӧ*/
    send_response_msg("restore_ack",rc,NULL);
}

/*ͨ��ģ����������*/
void deal_comm_request_config(const char *content)
{
    int rc=0; 
    JSON_Value *comm_value=NULL;
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
	if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   abn_debug("root_object is null\r\n");
	   send_response_msg("comm_ack",-1,NULL);
	   return;
	}
    comm_value=json_object_dotget_value(root_object,"file.comm");
    tmp_string=json_serialize_to_string(comm_value);
    if(NULL==tmp_string)
    {
        abn_debug("null pointer comm_value=0x%x\r\n",comm_value);
        rc=-1;
    }

    /*��Ӧ*/
    send_response_msg("comm_ack",rc,tmp_string);
    
    /*�ͷ�*/
    json_free_serialized_string(tmp_string);
    return;
}

/*uhfģ����������*/
void deal_uhf_request_config(const char *content)
{
    int rc=0; 
    JSON_Value *rfs_value=NULL;
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
    if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   abn_debug("root_object is null\r\n");
	   send_response_msg("uhf_ack",-1,NULL);
	   return;
	}
    rfs_value=json_object_dotget_value(root_object,"file.uhf");
    tmp_string=json_serialize_to_string(rfs_value);
    //CFG_DEBUG("tmp_string:%s\n",tmp_string);
    if(NULL==tmp_string)
    {
        CFG_DEBUG("(deal_uhf_request_config)error tmp_string is null\r\n");
		abn_debug(" null pointer error\r\n");
        rc=-1;
    }

    /*��Ӧ*/
    send_response_msg("uhf_ack",rc,tmp_string);
    /*�ͷ�*/
    json_free_serialized_string(tmp_string);
    return;
}
/*rfsģ����������*/
void deal_rfs_request_config(const char *content)
{
    int rc=0; 
    JSON_Value *rfs_value=NULL;
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
    if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   send_response_msg("rfs_ack",-1,NULL);
	   abn_debug("root_object is null\r\n");
	   return;
	}
    rfs_value=json_object_dotget_value(root_object,"file.rfs");
    tmp_string=json_serialize_to_string(rfs_value);

    if(NULL==tmp_string)
    {
        CFG_DEBUG("(deal_rfs_request_config)error tmp_string is null\r\n");
		abn_debug(" null pointer error\r\n");
        rc=-1;
    }

    /*��Ӧ*/
    send_response_msg("rfs_ack",rc,tmp_string);

    /*�ͷ�*/
    json_free_serialized_string(tmp_string);
    return;
}
/*rfsģ����������*/
void deal_rfs_change_request_config(const char *content)
{
    int rc=0; 
    JSON_Value *rfs_value=NULL;
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
    if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   send_response_msg("rfs_ack",-1,NULL);
	   abn_debug("root_object is null\r\n");
	   return;
	}
    rfs_value=json_object_dotget_value(root_object,"file.rfs");
    tmp_string=json_serialize_to_string(rfs_value);

    if(NULL==tmp_string)
    {
        CFG_DEBUG("(deal_rfs_request_config)error tmp_string is null\r\n");
		abn_debug(" null pointer error\r\n");
        rc=-1;
    }

    /*��Ӧ*/
    send_response_msg("rfs_change_ack",rc,tmp_string);

    /*�ͷ�*/
    json_free_serialized_string(tmp_string);
    return;
}

/*dispatchģ����������*/
void deal_dispatch_request_config(const char *content)
{
    int rc=0; 
    JSON_Value *rfs_value=NULL;
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
    if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   send_response_msg("dispatch_ack",-1,NULL);
	   abn_debug("root_object is null\r\n");
	   return;
	}
    rfs_value=json_object_dotget_value(root_object,"file");
    tmp_string=json_serialize_to_string(rfs_value);

    if(NULL==tmp_string)
    {
        CFG_DEBUG("(deal_dispatch_request_config)error tmp_string is null\r\n");
		abn_debug(" null pointer error\r\n");
        rc=-1;
    }

    CFG_DEBUG("(deal_dispatch_request_config):%s\r\n", tmp_string);
    /*��Ӧ*/
    send_response_msg("dispatch_ack",rc,tmp_string);

    /*�ͷ�*/
    json_free_serialized_string(tmp_string);
    return;
}


/*client set ����������Ϣ*/
void deal_client_get_config(const char *content)
{
    int rc=0; 
    char *tmp_string=NULL;
    
    /*��ȡroot������comm*/
	if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   abn_debug("root_object is null\r\n");
	   send_response_msg("client_get_ack",-1,NULL);
	   return;
	}
    tmp_string=json_serialize_to_string(root_value);

    if(NULL==tmp_string)
    {
        abn_debug(" null pointer error\r\n");
        rc=-1;
    }

    /*������Ϣ��Ӧ*/
    send_response_msg("client_get_ack",rc,tmp_string);

    /*�ͷ�*/
    json_free_serialized_string(tmp_string); 
    return;
}

/*client get ����������Ϣ*/
void deal_client_set_config(const char *content)
{
    int rc=0; 
    JSON_Value *input_value=NULL;
    JSON_Object *input_object=NULL;
    char *tmp_string=NULL;
    
    CFG_DEBUG("deal_client_set_config\r\n");
    
    //CHECK_NULL_POINTER_RETURN_VOID(content);
    if(NULL==root_object)
	{
	   CFG_DEBUG("root_object is NULL\r\n");
	   abn_debug("root_object is null\r\n");
	   send_response_msg("client_set_ack",-1,NULL);
	   return;
	}
    if(0!=json_validate(input_value,root_value))
    {
       CFG_DEBUG("(deal_client_set_config) error content is null\r\n");
       rc =-1;
	   abn_debug(" json_validate error\r\n");
       send_response_msg("client_set_ack",rc,NULL);
       return;
    }
    input_object=parse_string_to_object(content,&input_value);
    if(0!=json_validate(input_value,root_value))
    {
       CFG_DEBUG("(deal_client_set_config) error the object is not the same\r\n");
       rc =-1;
	   abn_debug(" json_validate error\r\n");
       send_response_msg("client_set_ack",rc,NULL);
       return;
    }
    /*����У��*/
    rc=CHECK_CFG(input_object);
    if(rc!=0)
    {
       CFG_DEBUG("(deal_client_set_config)CHECK_CFG  error\r\n");
	   abn_debug(" CHECK_CFG error rc=%d\r\n",rc);
       send_response_msg("client_set_ack",rc,NULL);
       return;
    }

    /*����root*/
    json_value_free(root_value);
    root_value=input_value;
    root_object=root_value->value.object;
    
    /*������Ӧ*/
    send_response_msg("client_set_ack",rc,NULL);

    /*�������ñ��*/
    send_config_change_msg(content);
    return;
}

/*cmmģ��ı�����Ϣ*/
void deal_cmm_keepalive_msg(const char *content)
{
    int rc=0;     
    JSON_Value *tmp_value=NULL;
    JSON_Object *tmp_object=NULL;
 
    CFG_DEBUG("deal_cmm_keepalive_msg content=%s\r\n",content);
    
    tmp_value=json_value_init_object();
    tmp_object=tmp_value->value.object;
    json_object_set_string(tmp_object,"type","cfg");
    json_object_set_string(tmp_object,"action","keepalive_ack");
   
    memset(send_buf,0,sizeof(send_buf));
    rc=json_serialize_to_buffer(tmp_value,send_buf,CFG_MAX_MSG_LEN);
    /*�ͷ�*/
    json_value_free(tmp_value);
    CFG_DEBUG("json_serialize_to_buffer send_buf = %s \r\n", send_buf);
    CFG_SEND_MSG(rep_socket,send_buf,strlen(send_buf)+1);
}


void get_client_comm_config(const char *content)
{
    return;
}


void set_client_comm_config(const char *content)
{
    return; 
}
void get_client_business_config(const char *content)
{
    return;
}


void set_client_business_config(const char *content)
{
    return; 
}


#if 1
int main(int argc, char *argv[])
{
    //���ý��̵����ƺ����ȼ�
    //��ʼ���źŴ��������Խ����˳����д���
    //zmq��ʼ��������rep��pub�׽���
    //cfgģ���ʼ�����������ļ���ȡ������Ϣ
    //ͨ��zmq_poll������Ϣ��������Ϣ
    int item_num;
    int rc;

    printf("cfg start \n");
    init_logm("cfg");
    init_process();
    create_zmq_sockets();
    rc=init_cfg();
    if(0!=rc)
    {
        printf("init_cfg error rc=%d\r\n",rc);
		abn_debug("init_cfg error\r\n");
        /*��¼�쳣*/
        exit(-1);
    }
    while(1)
    {

       rc=zmq_recv(rep_socket,recv_buf,CFG_MAX_MSG_LEN,0);
       if(-1==rc)
       {
          CFG_DEBUG("zmq_recv error :%s\r\n",zmq_strerror(errno));
		  abn_debug("zmq_recv error :%s\r\n",zmq_strerror(errno));
          continue;
       }
       deal_raw_msg(recv_buf);
	   log_debug("recv a cfg msg\r\n");
    }
    
    return 0;
 }
#endif
 #if 0
 int main(int argc, char *argv[])
 {
     zmq_pollitem_t  items[1];
     int item_num;
     int rc;
     
     init_process();
     create_zmq_sockets();
     init_cfg();

     memset(items,0,sizeof(items));
     items[0].socket = rep_socket;
     items[0].events = ZMQ_POLLIN;
     
     while(1)
     {
         rc = zmq_poll(items, 1, -1);
         if(rc<0)
         {
            printf("zmq_poll return , return %d %d,%s\n", rc,errno,zmq_strerror(errno));
            continue;
         }
         if(items[0].revents & ZMQ_POLLIN)
         {
             rc=zmq_recv(rep_socket,recv_buf,CFG_MAX_MSG_LEN,0);
             if(-1==rc)
             {
                printf("zmq_recv rep_socket error errno=%d\r\n",errno);
                continue;
             }
             deal_raw_msg(recv_buf);            
         }
         else
         {
             printf("pull error \r\n");
         }
     }
 }
#endif
