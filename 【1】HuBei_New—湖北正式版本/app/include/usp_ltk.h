/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_ltk.h
* �ļ���ʶ�� 
* ����ժҪ�� LLRP���������ӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2009.05.09
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_LTK_H
#define _USP_LTK_H

#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 
#include "ltkc.h"




/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/
/* LLRP��Ϣ�Ŷ��� */
#define LTK_KEEPLIVE_MSG_TYPE                               (62)        /* keelive��Ϣ���� */
#define LTK_KEEPLIVE_ACK_MSG_TYPE                           (72)        /* keeliveӦ����Ϣ���� */

#define LTK_ENABLE_EVENTS_AND_REPORTS_MSG_TYPE              (64)        /* �����������������ϱ� */
#define LTK_DELETE_SAVED_RO_ACCESS_REPORT_MSG_TYPE          (91)        /* ��ն����������� */
#define LTK_SAVED_RO_ACCESS_REPORT_ACK_MSG_TYPE             (94)        /* �������淢��Ӧ�� */

#define LTK_READER_TRACK_CTRL_MSG_TYPE                      (44)        /* ������ٿ�����Ϣ */
#define LTK_READER_RUNINFO_CTRL_MSG_TYPE                    (43)        /* ������Ϣ�۲������Ϣ */

#define LTK_GET_READER_LOG_MSG_TYPE                         (28)        /* �����ϴ���־��Ϣ */
#define LTK_GET_READER_LOG_RESPONSE_ACK_MSG_TYPE            (95)        /* �յ���־��Ϣ��Ӧ����Ϣ */
#define LTK_GET_READER_EXCEPTION_MSG_TYPE                   (29)        /* �����ϴ��쳣��Ϣ */
#define LTK_GET_READER_EXCEPTION_RESPONSE_ACK_MSG_TYPE      (96)        /* �յ��쳣��Ϣ��Ӧ����Ϣ */

#define LTK_READER_VERSION_ACTIVATE                   (21)
#define LTK_READER_VERSION_ACTIVATE_RESPONSE          (31)
#define LTK_READER_VERSION_UNACTIVATE                 (22)
#define LTK_READER_VERSION_UNACTIVATE_RESPONSE        (32)
#define LTK_READER_VERSION_QUERY                      (23)
#define LTK_READER_VERSION_QUERY_RESPONSE             (33)
#define LTK_READER_VERSION_DOWNLOAD_REQUEST           (24)   
#define LTK_READER_VERSION_DOWNLOAD_REQUEST_RESPONSE  (34)
#define LTK_READER_VERSION_DOWNLOAD                   (25)
#define LTK_READER_VERSION_DOWNLOAD_RESPONSE          (35)
#define LTK_READER_UBOOTVERSION_QUERY                 (26)
#define LTK_READER_UBOOTVERSION_QUERY_RESPONSE        (36)
#define LTK_READER_UBOOTVERSION_UPDATE                (27)
#define LTK_READER_UBOOTVERSION_UPDATE_RESPONSE       (37)
#define LTK_READER_CUSTVERSION_QUERY                  (107)
#define LTK_READER_CUSTVERSION_QUERY_RESPONSE         (117)
#define LTK_READER_CUSTVERSION_DOWNLOAD               (106)
#define LTK_READER_CUSTVERSION_DOWNLOAD_RESPONSE      (116)
#define LTK_READER_CUSTVERSION_DELETE                 (105)
#define LTK_READER_CUSTVERSION_DELETE_RESPONSE        (115)

#define USP_LTK_ERR_BASE 0x1000
#define USP_LTK_ERR_FRAMEENCODER_CONSTRUCTOR    ((unsigned int) (USP_LTK_ERR_BASE + 1)) /* ֡�������������� */
#define USP_LTK_ERR_FRAMEENCODE                 ((unsigned int) (USP_LTK_ERR_BASE + 2)) /* ֡������� */
#define USP_LTK_ERR_XMLTEXTENCODER_CONSTRUCTOR  ((unsigned int) (USP_LTK_ERR_BASE + 3)) /* xml�ַ����������������� */
#define USP_LTK_ERR_XMLTEXTENCODE               ((unsigned int) (USP_LTK_ERR_BASE + 4)) /* xml�ַ���������� */
#define USP_LTK_ERR_BUF_OVERFLOW                ((unsigned int) (USP_LTK_ERR_BASE + 5)) /* ������������� */
#define USP_LTK_ERR_MALLOC_HANDLE               ((unsigned int) (USP_LTK_ERR_BASE + 6)) /* ��ȡ��̬�����ڴ�ʧ�� */

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/

/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/
/* ����ע���� */
extern LLRP_tSTypeRegistry     usp_ltk_tSTypeRegistry;


/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/
/* LTK��ʼ�� */
extern unsigned int usp_ltk_init(void);

/* ���ֽ�����ʽ��ʶ��llrp��Ϣ������Ӧ����Ҫ�����ݽṹ */
extern LLRP_tSMessage *usp_ltk_decodeMessage(unsigned char *pucBuf, unsigned short int wLen, LLRP_tSErrorDetails *pError);

/* �ͷ���Ϣ��ռ�õ��ڴ�ռ� */
extern void usp_ltk_freeMessage(LLRP_tSMessage *pMessage);

/* ��Ӧ�õ�LLRP��Ϣ���ݽṹ�����������ͨ����·������ֽ�����ʽ */
extern unsigned int usp_ltk_encodeMessage(unsigned char *pucBuf, unsigned short int *pwLen, LLRP_tSMessage *pMessage, LLRP_tSErrorDetails *pError);

/* ����Ϣת��Ϊxml�ַ�����ʽ */
extern unsigned int usp_ltk_messageToXMLString(unsigned char *pucBuf, unsigned short int wLen, LLRP_tSMessage *pMessage, LLRP_tSErrorDetails *pError);


#endif  /* _USP_LTK_H */

