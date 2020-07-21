/************************************************************************
* ��Ȩ���� (C)2011, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_pubAdt.h
* �ļ���ʶ�� 
* ����ժҪ�� �������ݽṹͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� ���ƾ�
* ������ڣ� 2011.06.18
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/

#ifndef _USP_PUB_ADT_H
#define _USP_PUB_ADT_H
/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/ 


/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/ 

typedef struct tagT_USPPubAdtListNode
{
    struct tagT_USPPubAdtListNode  *ptNext;
    struct tagT_USPPubAdtListNode  *ptPrevious;
} T_USPPubAdtListNode;

typedef struct tagT_USPPubAdtList
{
    WORD32                 dwCount;
    T_USPPubAdtListNode    tNode;
}T_USPPubAdtList;


/**************************************************************************
 *                             ������                                      *
 **************************************************************************/

/**************************************************************************
 *                           ģ��                                         *
 **************************************************************************/


/**************************************************************************
 *                         ȫ�ֱ�������                                    *
 **************************************************************************/

/**************************************************************************
 *                        ȫ�ֺ���ԭ��                                     *
 **************************************************************************/

VOID                    usp_pub_adt_listInit( T_USPPubAdtList *ptPubAdtList );
VOID                    usp_pub_adt_listInsert( T_USPPubAdtList *ptPubAdtList, T_USPPubAdtListNode *ptPrev, T_USPPubAdtListNode *ptNode );
VOID                    usp_pub_adt_listAdd( T_USPPubAdtList *ptPubAdtList, T_USPPubAdtListNode *ptNode );
VOID                    usp_pub_adt_listDelete( T_USPPubAdtList *ptPubAdtList, T_USPPubAdtListNode *ptNode );
T_USPPubAdtListNode    *usp_pub_adt_listFirst( T_USPPubAdtList *ptPubAdtList );
T_USPPubAdtListNode    *usp_pub_adt_listLast(  T_USPPubAdtList *ptPubAdtList );
T_USPPubAdtListNode    *usp_pub_adt_listGet( T_USPPubAdtList *ptPubAdtList );
T_USPPubAdtListNode    *usp_pub_adt_listNext( T_USPPubAdtListNode *ptNode );
T_USPPubAdtListNode    *usp_pub_adt_listPrevious( T_USPPubAdtListNode *ptNode );
T_USPPubAdtListNode    *usp_pub_adt_listNth( T_USPPubAdtList *ptPubAdtList, WORD32 dwNodeNum );
VOID                    usp_pub_adt_listConcat( T_USPPubAdtList *ptDstList, T_USPPubAdtList *ptAddList );
WORD32                  usp_pub_adt_listCount( T_USPPubAdtList *ptPubAdtList );


#endif  /* _USP_PUB_ADT_H */

