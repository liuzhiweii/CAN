#ifndef __COMM_LLRP__
#define __COMM_LLRP__

typedef struct tagT_Msg
{
    long        mtype;          /* Message type, used by linux */
    unsigned char        ucLinkType;     /* ��·�������� */
    unsigned char        ucLinkIndex;    /* ��·������ */
    unsigned short      wSenderTno;     /* ���Ͷ������ */
    unsigned short      wReceiverTno;   /* ���ն������ */    
    unsigned short      wMsgID;         /* ��Ϣ�¼��� */
    unsigned short      wMsgLen;        /* ��Ϣ���ȣ���������Ϣͷ��BYTEΪ��λ */
    unsigned short      wMsgSequence;   /* ��Ϣ��� */ 
    unsigned short      wSrcTno;        /* Դ����� */
    unsigned short      wDstTno;        /* Ŀ������� */
    unsigned short      wRelayMagic;    /* �м���Ϣħ���� */
    unsigned short      wPad;
}T_Msg;



#endif


