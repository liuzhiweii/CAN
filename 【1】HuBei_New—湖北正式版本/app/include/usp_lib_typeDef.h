/************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_lib_typeDef.h
* �ļ���ʶ�� 
* ����ժҪ�� ���幫����������
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2012.02.06
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_TYPE_DEF_H
#define _USP_TYPE_DEF_H

/***********************************************************
 *                       �������Ͷ���                      *
 ***********************************************************/
typedef unsigned short int  WORD16;    
typedef signed short int    SWORD16;

typedef unsigned int        WORD32;
typedef signed   int        SWORD32;

typedef unsigned long long  WORD64;
typedef signed long long    SWORD64;

typedef unsigned int        BOOLEAN;

typedef float               FP32;
typedef double              FP64;

typedef unsigned char       BYTE;
typedef signed char         CHAR;

#ifndef WIN32
#define VOID                void                
#endif

#endif  /* _USP_TYPE_DEF_H */

