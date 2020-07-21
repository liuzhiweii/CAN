/************************************************************************
* ��Ȩ���� (C)2009, ����ͨѶ����о���
* 
* �ļ����ƣ� usp_tsensor.h
* �ļ���ʶ�� 
* ����ժҪ�� �¶ȴ�����ģ�����ӿ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� USP01.10.00
* ��    �ߣ� �ܻ۾�
* ������ڣ� 2009-12-11
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/


#ifndef _USP_TSENSOR_H_
#define _USP_TSENSOR_H_

/*����ID �Ŷ���--begin*/

/*��6700,first tsensorλ�����ְ���*/
#define FIRST_TSENSOR_LOCAL_TEMP               0
#define FIRST_TSENSOR_REMOTE_TEMP              1
#define FIRST_TSENSOR_LOCAL_TEMP_LOW           2
#define FIRST_TSENSOR_LOCAL_TEMP_HIGH          3
#define FIRST_TSENSOR_REMOTE_TEMP_LOW          4
#define FIRST_TSENSOR_REMOTE_TEMP_HIGH         5
#define FIRST_TSENSOR_ALARM_STATUS             6

/*��Ƶ���ϵ��¶ȴ�����*/
#define SECOND_TSENSOR_LOCAL_TEMP              7
#define SECOND_TSENSOR_REMOTE_TEMP             8
#define SECOND_TSENSOR_LOCAL_TEMP_LOW          9
#define SECOND_TSENSOR_LOCAL_TEMP_HIGH         10
#define SECOND_TSENSOR_REMOTE_TEMP_LOW         11
#define SECOND_TSENSOR_REMOTE_TEMP_HIGH        12
#define SECOND_TSENSOR_ALARM_STATUS            13
#define SECOND_TSENSOR_CLEAR_INT              14

/*����ID �Ŷ���--end*/



WORD32 usp_drv_tsensor_init(VOID);
WORD32 usp_drv_tsensor_get_temp_input(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_remote_temp_input(BYTE * pucValue);
WORD32 usp_drv_tsensor_set_temp_high(BYTE ucValue);
WORD32 usp_drv_tsensor_set_temp_low(BYTE ucValue);
WORD32 usp_drv_tsensor_set_remote_temp_high(BYTE ucValue);
WORD32 usp_drv_tsensor_set_remote_temp_low(BYTE ucValue);
WORD32 usp_drv_tsensor_get_temp_high(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_temp_low(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_remote_temp_high(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_remote_temp_low(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_alarm_status(BYTE * pucValue);
WORD32 usp_drv_tsensor_get_temperature(BYTE ucFunID,BYTE * pucValue);
WORD32 usp_drv_tsensor_set_temperature(BYTE ucFunID,BYTE  ucValue);

#endif


