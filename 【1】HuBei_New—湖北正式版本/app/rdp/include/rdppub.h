/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� RdpPub.h
* �ļ���ʶ�� 
* ����ժҪ��RDP����ģ�鹫��ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lzy
* ������ڣ� 2012.03.19
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _RDP_PUB_H
#define _RDP_PUB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/prctl.h>
#include <sys/time.h> 
#include <sys/resource.h>
#include <unistd.h>
#include <assert.h>

#include "rdpprotocolpub.h"
#include "uhfllrpRule.h"
#include "uhfllrpDispatch.h"
#include "uhfllrpReport.h"
#include "uhfllrpEpc.h"
#include "uhfllrpISO6B.h"
#include "uhfllrpGB.h"






#define USP_LIB_SCP_MSGSEQ(n)       (0xFFFF == (++(n)) ? ++(n) : (n))


#define CHECK_FAIL_NUMBER(p,s)                             \
do\
{\
    if((p)<0)                                 \
    {                                             \
       printf("%s file:%s line:%u\r\n",s,__FILE__,__LINE__);\
       exit(-1);                                 \
    }\
}while(0)

#define CHECK_NULL_POINTER_RETURN_VOID(p)\
		do\
		{\
			if(NULL==(p))								  \
			{											  \
			   RdpLlrpDbgPrint("null pointer error file:%s line:%u\r\n",__FILE__,__LINE__);\
			   return ; 								\
			}\
		}while(0)




#endif  /* _RDP_PUB_H */

