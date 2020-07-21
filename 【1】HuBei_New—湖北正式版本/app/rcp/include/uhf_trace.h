#ifndef _UHF_TRACE_H
#define _UHF_TRACE_H

#define EV_APP_FPGA_Begin	              (WORD16)0x0800
/**************************************************************************
 *                        ����                                            *
 **************************************************************************/
#define EV_RCP_FPGA_BEGIN EV_APP_FPGA_Begin

#define EV_EPC_Inventory  (EV_RCP_FPGA_BEGIN+1)
#define EV_EPC_Read       (EV_RCP_FPGA_BEGIN+2)
#define EV_EPC_Write      (EV_RCP_FPGA_BEGIN+3)
#define EV_EPC_Lock       (EV_RCP_FPGA_BEGIN+4)
#define EV_EPC_BlockLock  (EV_RCP_FPGA_BEGIN+5)
#define EV_EPC_ReadLock   (EV_RCP_FPGA_BEGIN+6)
#define EV_EPC_Kill       (EV_RCP_FPGA_BEGIN+7)
#define EV_6B_Inventory   (EV_RCP_FPGA_BEGIN+8)
#define EV_6B_Read        (EV_RCP_FPGA_BEGIN+9)
#define EV_6B_Write       (EV_RCP_FPGA_BEGIN+10)
#define EV_6B_Lock        (EV_RCP_FPGA_BEGIN+11)
#define EV_6B_LockQuery   (EV_RCP_FPGA_BEGIN+12)
#define EV_EPC_IPJQT      (EV_RCP_FPGA_BEGIN+13)
#define EV_GB_Inventory  (EV_RCP_FPGA_BEGIN+14)
#define EV_GB_Read       (EV_RCP_FPGA_BEGIN+15)
#define EV_GB_Write      (EV_RCP_FPGA_BEGIN+16)
#define EV_GB_Lock       (EV_RCP_FPGA_BEGIN+17)
#define EV_GB_Kill       (EV_RCP_FPGA_BEGIN+18)
#define EV_GB_Erase       (EV_RCP_FPGA_BEGIN+19)


#define EV_FGPA_RES_BEGIN  (EV_RCP_FPGA_BEGIN+50)

#define   EV_EPC_Inventory_RESP         (EV_FGPA_RES_BEGIN+0)
#define   EV_EPC_Inventory_Finish_RESP  (EV_FGPA_RES_BEGIN+1)
#define   EV_EPC_Read_RESP               (EV_FGPA_RES_BEGIN+2)
#define   EV_EPC_Read_Error_RESP        (EV_FGPA_RES_BEGIN+3)
#define   EV_EPC_Write_RESP             (EV_FGPA_RES_BEGIN+4)
#define   EV_EPC_Write_Error_RESP        (EV_FGPA_RES_BEGIN+5)
#define   EV_EPC_Kill_RESP              (EV_FGPA_RES_BEGIN+6)
#define   EV_EPC_Kill_Error_RESP         (EV_FGPA_RES_BEGIN+7)
#define   EV_EPC_Lock_RESP              (EV_FGPA_RES_BEGIN+8)
#define   EV_EPC_Lock_Error_RESP         (EV_FGPA_RES_BEGIN+9)
#define   EV_EPC_BlkWrt_RESP             (EV_FGPA_RES_BEGIN+10)
#define   EV_EPC_BlkWrt_Error_RESP        (EV_FGPA_RES_BEGIN+11)
#define   EV_EPC_BlkErase_RESP            (EV_FGPA_RES_BEGIN+12)
#define   EV_EPC_BlkErase_Error_RESP      (EV_FGPA_RES_BEGIN+13)
#define   EV_EPC_BlkPermaLock_RESP        (EV_FGPA_RES_BEGIN+14)
#define   EV_EPC_BlkPermaLock_Query_RESP   (EV_FGPA_RES_BEGIN+15)
#define   EV_EPC_BlkPermaLock_Error_RESP   (EV_FGPA_RES_BEGIN+16)
#define   EV_EPC_BlkReadLock_RESP          (EV_FGPA_RES_BEGIN+17)
#define   EV_EPC_BlkReadLock_Error_RESP    (EV_FGPA_RES_BEGIN+18)
#define   EV_6B_Inventory_RESP             (EV_FGPA_RES_BEGIN+19)
#define   EV_6B_Acess_RESP                (EV_FGPA_RES_BEGIN+20)
#define   EV_6B_Inventory_Finish_RESP     (EV_FGPA_RES_BEGIN+21)
#define   EV_FPGA_UNKNOW_RESP             (EV_FGPA_RES_BEGIN+22)
#define   EV_EPC_Inventory_Error_RESP     (EV_FGPA_RES_BEGIN+23)
#define   EV_GB_Inventory_RESP         (EV_FGPA_RES_BEGIN+24)
#define   EV_GB_Inventory_Finish_RESP  (EV_FGPA_RES_BEGIN+25)
#define   EV_GB_Read_RESP               (EV_FGPA_RES_BEGIN+26)
#define   EV_GB_Read_Error_RESP        (EV_FGPA_RES_BEGIN+27)
#define   EV_GB_Write_RESP             (EV_FGPA_RES_BEGIN+28)
#define   EV_GB_Write_Error_RESP        (EV_FGPA_RES_BEGIN+29)
#define   EV_GB_Kill_RESP              (EV_FGPA_RES_BEGIN+30)
#define   EV_GB_Kill_Error_RESP         (EV_FGPA_RES_BEGIN+31)
#define   EV_GB_Lock_RESP              (EV_FGPA_RES_BEGIN+32)
#define   EV_GB_Lock_Error_RESP         (EV_FGPA_RES_BEGIN+33)
#define   EV_GB_Erase_RESP                 (EV_FGPA_RES_BEGIN+34)
#define   EV_GB_Erase_Error_RESP           (EV_FGPA_RES_BEGIN+35)
#define   EV_GB_Inventory_Error_RESP     (EV_FGPA_RES_BEGIN+36)



#define   EV_RCP_TO_FPGA      (0)
#define   EV_FPGA_TO_RCP      (1)
#define   FPGA_ERROR_TRACE_LEN (10)


typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wCommandType;                 /*��������*/
    WORD16  wResult;                       /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                ���ɹ����򷵻�PC��EPC
                                                ʧ�ܻ��������û��PC��EPC*/

    WORD16  wPC_Data;                       /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                  /*EPC��*/
} T_EPC_Inventory_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wCommandType;                 /*��������*/
    WORD16  wResult;                       /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                ���ɹ����򷵻�PC��EPC
                                                ʧ�ܻ��������û��PC��EPC*/
} T_EPC_Inventory_Finish_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC�������ݳ��Ⱥ�����
                                              ��ʧ�ܣ�����PC��EPC������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_Inventory_Error_RESP_Trace;


typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wErrorData;                  /*����֡*/

} T_EPC_Inventory_Unkonwn_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                                         ���ɹ�������PC��EPC
                                                                         �������ݳ��Ⱥ�����
                                                                         ��ʧ�ܣ�����PC��EPC
                                                                         ������׶Ρ�������
                                                                         ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wLen_Data;                   /*�����ݵĳ���*/
    BYTE    ucReadData[0];               /*��ȡ������*/
} T_EPC_READ_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC�������ݳ��Ⱥ�����
                                              ��ʧ�ܣ�����PC��EPC������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_READ_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                      ���ɹ�������PC��EPC��д���ݳ���
                                              ��ʧ�ܣ�����PC��EPC
                                              ������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];               /*EPC��*/

    WORD16  wLen_Data;                   /*д���ݵĳ���*/
} T_EPC_WRITE_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC��д���ݳ���
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];               /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_WRITE_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];               /*EPC��*/
} T_EPC_KILL_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];               /*EPC��*/
    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_KILL_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/
} T_EPC_LOCK_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_LOCK_Error_RESP_Trace;


typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/
} T_EPC_BlkWrt_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_BlkWrt_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_BlkErase_RESP_Trace;


typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                   ���ɹ�������PC��EPC�������ݳ��ȡ�
permalockbits����
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/
} T_EPC_BlkPermaLOCK_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                                                                             ���ɹ�������PC��EPC�������ݳ��ȡ�
                                                                          permalockbits����
                                                                                                                        ��ʧ�ܣ�����PC��EPC
                                                                          ������׶Ρ�������
                                                                                                                        ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wLen_Data;                   /*�����ݵĳ���*/
    BYTE    ucLock_Data[0];             /*permallockbits����*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
}T_BlkPermaLOCK_Query_EPC_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                       ���ɹ�������PC��EPC�������ݳ��ȡ�
    permalockbits����
                                                  ��ʧ�ܣ�����PC��EPC
    ������׶Ρ�������
                                                  ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_BlkPermaLOCK_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                  ���ɹ�������PC��EPC
                                                  ��ʧ�ܣ�����PC��EPC
    ������׶Ρ�������
                                                  ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/
} T_EPC_BlkReadLOCK_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                  ���ɹ�������PC��EPC
                                                  ��ʧ�ܣ�����PC��EPC
    ������׶Ρ�������
                                                  ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wEPC_Data[0];                   /*EPC��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_EPC_BlkReadLOCK_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameHead;                 /*֡ͷ*/
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��Ϣ����*/
    BYTE   ucUID[8];                      /*UID����*/
} T_6B_Inventory_RESP_Trace;
 
typedef struct
{
    WORD16  wFrameHead;                 /*֡ͷ*/
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��Ϣ����*/

    BYTE    ucUID[8];                      /*UIDΪ64λ */

    WORD16  wRec_Err_State;            /*���մ���״ָ̬ʾ*/
    BYTE    ucRec_Data[8];
    /*��ǩ���ص���Ϣֵ����Ϣ�����ǿɱ�ģ����8���ֽ�*/

} T_6B_Access_RESP_Trace;

typedef struct
{
    WORD16  wFrameHead;                 /*֡ͷ*/
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��Ϣ����*/
} T_6B_Inventory_Finish_RESP_Trace;

///////////////////
/// GB RESP_TRACE
///////////////////

typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wCommandType;                 /*��������*/
    WORD16  wResult;                       /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                ���ɹ����򷵻�PC��EPC
                                                ʧ�ܻ��������û��PC��EPC*/

    WORD16  wPC_Data;                       /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                  /*GB��*/
} T_GB_Inventory_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wCommandType;                 /*��������*/
    WORD16  wResult;                       /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                ���ɹ����򷵻�PC��EPC
                                                ʧ�ܻ��������û��PC��EPC*/
} T_GB_Inventory_Finish_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC�������ݳ��Ⱥ�����
                                              ��ʧ�ܣ�����PC��EPC������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_Inventory_Error_RESP_Trace;


typedef struct
{
    WORD16  wFrameLength;                 /*֡����*/
    WORD16  wErrorData;                  /*����֡*/

} T_GB_Inventory_Unkonwn_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                                         ���ɹ�������PC��EPC
                                                                         �������ݳ��Ⱥ�����
                                                                         ��ʧ�ܣ�����PC��EPC
                                                                         ������׶Ρ�������
                                                                         ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/

    WORD16  wLen_Data;                   /*�����ݵĳ���*/
    BYTE    ucReadData[0];               /*��ȡ������*/
} T_GB_READ_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC�������ݳ��Ⱥ�����
                                              ��ʧ�ܣ�����PC��EPC������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_READ_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                                      ���ɹ�������PC��EPC��д���ݳ���
                                              ��ʧ�ܣ�����PC��EPC
                                              ������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];               /*GB��*/

    WORD16  wLen_Data;                   /*д���ݵĳ���*/
} T_GB_WRITE_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC��д���ݳ���
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];               /*GB��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_WRITE_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ*/
    WORD16  wGB_Data[0];               /*GB��*/
} T_GB_KILL_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];               /*GB��*/
    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_KILL_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/
} T_GB_LOCK_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_LOCK_Error_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/
} T_GB_ERASE_RESP_Trace;

typedef struct
{
    WORD16  wFrameLength;               /*֡����*/
    WORD16  wCommandType;               /*��������*/
    WORD16  wResult;                     /*ִ�н����0��ʧ��  1���ɹ�  2������
                                              ���ɹ�������PC��EPC
                                              ��ʧ�ܣ�����PC��EPC
������׶Ρ�������
                                              ����������ֱ�ӽ���*/

    WORD16  wPC_Data;                    /*PCֵ ��value>> 11) & 0x1F */
    WORD16  wGB_Data[0];                   /*GB��*/

    WORD16  wErr_State;                  /*����׶�,��ʾ״̬���ĵ�ǰ״̬*/
    WORD16  wErr_Code_Prt;               /*�Զ���Ĵ�����*/
    WORD16  wErr_Code_Std;               /*��ǩ���صĴ�����*/
} T_GB_ERASE_Error_RESP_Trace;


typedef struct
{
    WORD16  wDataLen;                    /*PCֵ*/
    WORD16  wFpgaData[0];                   /*EPC��*/
} T_Fpga_Unkonwn_RESP_Trace;


#endif

