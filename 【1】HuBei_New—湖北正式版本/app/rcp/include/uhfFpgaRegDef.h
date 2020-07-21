/************************************************************************
* ��Ȩ���� (C)2008, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� uhfFpgaRegs.h
* �ļ���ʶ�� 
* ����ժҪ�� fpgaЭ����صļĴ��������ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� ft
* ������ڣ� 2009.03.10
* 
* �޸ļ�¼1���Զ���
*    �޸����ڣ�2012.3.8
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�ȥ�����õ�һЩ�Ĵ��������ݶ��壬����
                                 RUPƽ̨��Ҫ������޸ġ�
* �޸ļ�¼2����
************************************************************************/
#ifndef _FPGA_UHF_REG_H
#define _FPGA_UHF_REG_H
/**************************************************************************
 *                        ����                                            *
 **************************************************************************/

/**********************************************
               CPU��FPGA�ӿڼĴ���  
***********************************************/


#define reg_rb_module_type (0x0049)

/*
FPGA�߼��汾�Ĵ���
    Bit15~Bit0
*/
#define REG_FPGA_VERSION  (0x0000)

/*
�ж�״̬�Ĵ���
Bit15~bit1 ����    
Bit0        frame_indic
frame_indic��FIFO����������֡
*/
#define REG_INT_Status  (0x0001)

#define FPGA_INT_STATE_HAS_FRAME (0x1)

/*
�ж����μĴ���
Bit15~bit1����    
Bit0        frame_indic mask
frame_indic mask ��֡�����ж����� 
*/
#define REG_INT_Mask  (0x007D)

#define FPGA_INT_ENABLE   (1)
#define FPGA_INT_DISABLE  (0)

/*
�ж�ʹ�ܼĴ�������ַ��0002H
Bit15~bit1����
Bit0 int_enble
int_enble���ܵ��жϽ�ֹ��ʹ��λ
1��ʹ���ж�
0����ֹ��CPU���ж�
*/
#define REG_INT_enable  (0x0002)


/*
��д���ԼĴ���
Bit15~Bit0
CPU��д���ԣ��û��Զ���
*/
#define REG_Fpga_test  (0x0003)


/*
CPU����FIFO������֡������
Bit15~bit5����    
Bit4~Bit0        frame_cnt
frame_cnt����ǰFIFO�л����֡����
*/
#define REG_FIFO_Frame_cnt  (0x0008)

/* MACЭ���Ĵ��� */
/*
Э�����ͼĴ���
Bit15~Bit2   ����    
Bit1~Bit0        protocal type
protocal type��
00��EPC G2
01��18000-6B
10���й���׼
11������
*/
#define REG_PROTO_TYPE  (0x0004)


/*
CPU�������ͼĴ���
����    nak    blkerase    blkwrt    lock    kill    write    read    query    Select
Bit8~Bit0��1��CPU�������������BITΪ0��
*/
#define REG_CPU_CMD_type  (0x0005)

/*
CPUָʾ�Ĵ���
BIT0 ��Ч���ϵ�Ĭ��Ϊ0,�ش���fpga ������һ������
*/
#define REG_CPU_LLRP_CMD_Trig  (0x007a)

/*
CPUָʾ�Ĵ���
Bit15~bit2  ����    
Bit1    stop mac    
Bit0        cmd_indic
cmd_indic��0����Ϊ1��CPU����MAC���ʼ����
CPU��Ҫ��д�����������ͼĴ���֮��֮�����д���λ��
stop mac�� 0����Ϊ1��CPU����MAC����ֹͣ����
*/
#define REG_CPU_CMD_Trig  (0x007B)

/*
CPUָʾ�Ĵ���
�շ�����ʱDATT ��ֵ
*/
#define REG_FPGA_AGC_VALUE  (0x007E)


/*
mac�㹫��״̬�Ĵ���
Bit1   MAC busy    
Bit0  frame indic 
 
frame indic��   1��FIFO��������֡��ֻҪFIFO��֡��������0����λһֱΪ1��
MAC busy��   1��MAC״̬��BUSY ��0������
*/
#define REG_G2_mac_status  (0x0006)

/*
mac���Զ��������Ĵ���
Bit5 EPC err    
Bit4 handel error
Bit3 tag error
Bit2 crc error
Bit1 Collision
Bit0 Timeout
Timeout����ʱ�����ź�
Collision����ײ
crc error��CRCУ���
tag error����ǩ���ش�����
handel error��handel��ƥ��
EPC err�� EPC���벻ƥ��
*/
#define REG_G2_mac_selfdefine_error_code  (0x0007)

/*
CPU����FIFO����˿ڼĴ���
Bit15~bit0        cpumsg_fifo_port
cpumsg_fifo_port����ϢFIFO������˿ڼĴ�������FIFOֻ��һ������˿ڣ�����ͨ���Ĵ�����ʽ��CPU�����ʡ�
16λ��ȣ���CPU��FPGA���������߿����ͬ��
*/
#define REG_FPGA_FIFO   (0x000A)

/*
select��������Ĵ���
Bit8        Truncate    
Bit7~Bit6   MemBank    
Bit5~Bit3   action    
Bit2~Bit0   target
 
*/
#define REG_G2_Select_Para  (0x000B)


/*
select�����ַ�Ĵ���
Bit15~Bit8   EBV  BLOCK2    
Bit7~Bit0        EBV  BLOCK1
EBV  BLOCK1��   ���뿪ʼ��ַ�ĵ�1��EBV��
EBV  BLOCK2��   ���뿪ʼ��ַ�ĵ�2��EBV��
����Э�飬ÿ��EVB���а���7����ЧBIT��1����չָʾBIT������������EBV���Ա�ʾ�ĵ�ַ��Χ��0��16383��
����Ŀǰ��Ӧ��״�����õ�ַ��Χ����ʵ�ʵ�������ˣ�Ŀǰֻ����2��������ʾ�����ַ�ռ䡣
��Ҫ��ʾ����ĵ�ַ�ռ䣬��Ҫ�������ӼĴ����� 
*/
#define REG_G2_Select_Addr  (0x000C)


/*
select����MASK�Ĵ���
MASK word31
MASK word30
-----------
MASK word3
MASK word2
MASK word1
MASK word0
 
MASK�Ĵ����鹲32�������Դ洢���512��BIT�����롣
�üĴ�������Կ���һƬ�����Ĵ洢������ͬ�����ڴ�顣
*/
#define REG_G2_Select_Mask_Start     (0x10000)

#define REG_G2_BlockWrite_Data_Start (0x20000)


/*
query��������Ĵ���
���ౣ��    
Bit12��Bit9   Q     
Bit8          Target     
Bit7��Bit6    Session     
Bit5��Bit4    Sel    
Bit3          TRext     
Bit2��Bit1    M    
Bit0          DR
 
DR��      0: DR=8��    1: DR=64/3
M ��      00:M=1��   01: M=2��   10: M=4   11: M=8
TRext��   0: No pilot tone��    1: Use pilot tone
Sel��     00: All��    01: All��  10: ~SL��   11: SL
Session�� 00: S0��   01: S1��  10: S2��     11: S3
Target��  0��A��    1��B��
Q��       0�C15
*/
#define REG_G2_Query_Para  (0x002E)

/*
read����MemBank�Ĵ���
Bit15~Bit2
Bit1~Bit0 MemBank
MemBank��
00: Reserved 
01: EPC 
10: TID 
11: User
*/
#define REG_G2_Read_Membank  (0x002F)


/*
read�����ַ�Ĵ���
Bit15~Bit8   EBV  BLOCK2    
Bit7~Bit0        EBV  BLOCK1
EBV  BLOCK1��   ���뿪ʼ��ַ�ĵ�1��EBV��
EBV  BLOCK2��   ���뿪ʼ��ַ�ĵ�2��EBV��
����Э�飬ÿ��EVB���а���7����ЧBIT��1����չָʾBIT������������EBV���Ա�ʾ�ĵ�ַ��Χ��0��16383��
����Ŀǰ��Ӧ��״�����õ�ַ��Χ����ʵ�ʵ�������ˣ�Ŀǰֻ����2��������ʾ�����ַ�ռ䡣��Ҫ��ʾ����ĵ�ַ�ռ䣬��Ҫ�������ӼĴ�����
*/
#define REG_G2_Read_Addr  (0x0030)


/*
read������ָ����Ĵ���
����    
Bit7~Bit0        WordCount
WordCount��Ҫ��ȡ���ֵĸ��������256��
 
*/
#define REG_G2_Read_WordCount  (0x0031)

/*
write����MemBank�Ĵ���
Bit15~Bit2       Bit1~Bit0   MemBank     
MemBank��
00: Reserved 
01: EPC 
10: TID 
11: User
*/
#define REG_G2_Write_Membank  (0x0032)

/*
write�����ַ�Ĵ���
Bit15~Bit8   EBV  BLOCK2    
Bit7~Bit0        EBV  BLOCK1
EBV  BLOCK1��   ���뿪ʼ��ַ�ĵ�1��EBV��
EBV  BLOCK2��   ���뿪ʼ��ַ�ĵ�2��EBV��
����Э�飬ÿ��EVB���а���7����ЧBIT��1����չָʾBIT������������EBV���Ա�ʾ�ĵ�ַ��Χ��0��16383��
����Ŀǰ��Ӧ��״�����õ�ַ��Χ����ʵ�ʵ�������ˣ�Ŀǰֻ����2��������ʾ�����ַ�ռ䡣��Ҫ��ʾ����ĵ�ַ�ռ䣬��Ҫ�������ӼĴ�����
*/
#define REG_G2_Write_Addr  (0x0033)

/*
write�������ݼĴ���
Bit15~Bit0           Data
Data:  16BIT�����������ڵ������ݵ�д����
*/
#define REG_G2_Write_Data  (0x0034)

/*
write����д�����Ĵ�������ʱ��ʹ�øüĴ�����
����    Bit7~Bit0        WordCount
WordCount��Ҫд���ֵĸ��������256��
�üĴ�������ʹ�ö������д������������д�����֡�
*/
#define REG_G2_Write_WordCount (0x0035)

/*
lock����payload_mask�Ĵ�������ַ��0039H
����
Bit9~Bit0 payload_mask
payload_mask��Lockָ�����һ��20-bit��payload���������£�
ǰ10��payloadλΪ����λ��Mask bit������ǩ�밴��������˼�����Щλ��ֵ��
Mask=0��������Ӧ��Action���򣬲�������ǰ���������á�
Mask=1��ִ����Ӧ��Action���򣬲����ǵ�ǰ���������á�
��10λΪAction bit����ǩ�밴��������˼�����Щλ��ֵ��
Action=0��������Ӧ�洢λ�õ�������
Action=1���趨��Ӧ�洢λ��Ϊ������������������
��Action����Ĺ��ܲο�Э���б�6.39��
Lockָ���payload��ʼ��Ϊ20bit��
*/
#define REG_G2_Loc_payload_mask (0x0039)

/*
lock����payload_Action�Ĵ�������ַ��003AH
����
Bit9~Bit0 payload_Action
payload_Action���μ�payload_mask�Ĵ�����˵��
*/
#define REG_G2_Loc_payload_Action (0x003A)

/* access�������루�߰��֣��Ĵ�������ַ��003BH
Bit15~Bit0 Password_h ( high half word)
Password_h: 32λaccess����ĸ߰��֡�
*/
#define REG_G2_Access_Password_h (0x003B)

/*access�������루�Ͱ��֣��Ĵ�������ַ��003CH
Bit15~Bit0 Password_L ( low half word)
Password_L: 32λaccess����ĵͰ��֡�
*/
#define REG_G2_Access_Password_L (0x003C)


/*
blockwrite membank �Ĵ�������ַ��003DH
����
������membank��
*/
#define REG_G2_Block_Write_Membank (0x003D)


/*
blockwrite adress��ַ��003EH

payload_Action���μ�payload_mask�Ĵ�����˵��
*/
#define REG_G2_Block_Write_Adress (0x003E)


/*
blockwrite ��ַ��003EH
payload_Action���μ�payload_mask�Ĵ�����˵��
*/
#define REG_G2_Block_Write_Length (0x003F)

/*
kill�������루�߰��֣��Ĵ�������ַ��0036H
Bit15~Bit0 Password_h ( high half word)
Password_h: 32λ����ĸ߰��֡�
*/
#define REG_G2_Kill_Password_h (0x0036)

/*kill�������루�Ͱ��֣��Ĵ�������ַ��0037H
Bit15~Bit0 Password_L ( low half word)
Password_L: 32λ����ĵͰ��֡�
*/
#define REG_G2_Kill_Password_L (0x0037)

/*
kill����RFU�Ĵ�������ַ��0038H
����
Bit2~Bit0 RFU
RFU�� Killָ���������RFUλ�����Ķ�����Class-1�ı�ǩͨѶʱ���Ķ�������Щλ��Ϊ0002��Class-1��ǩ�����⼸λ��
���߹��ܵı�ǩ����ʹ����Щλ����չKillָ��Ĺ��ܣ��磬ɱ��һ����ǩʹ֮����һ����������״̬��������ֱ��ɱ����
*/
#define REG_G2_Kill_RFU (0x0038)



#define REG_forward_filter_enable (0x0043)

#define REG_backward_filter_enable (0x0045)

/* ISO18000-6B��ǰ�������ͼĴ�����ISO18000_6B_forward_preamble_type��16λ����ַ��0048H
Bit15��bit2������	preamble_type
preamble_type
00��delimeter1
01��delimeter2
10��delimeter3
11��delimeter4
�������� */
#define ISO18000_6B_forward_preamble_type (0x0048)

#define ISO18000_6B_forward_delimeter1 (0x0)
#define ISO18000_6B_forward_delimeter2 (0x1)
#define ISO18000_6B_forward_delimeter3 (0x2)
#define ISO18000_6B_forward_delimeter4 (0x3)


/*
EPC G2Э���ǰ����·���ʼĴ�����G2_forlink_v��16λ
Bit15-bit9������    
bit8-bit0   G2_FORLINK_V
G2_FORLINK_V��G2��R��>T����·bit���ʣ�Bit8��bit0��G2��R��>T����·���ʳ���2��ֵ������λ����������G2��R��>T����·����Ϊ40K�����ֵΪ40��2��80��
*/
#define G2_forlink_v  (0x004C)


/*
EPC G2Э���T1ֵ�Ĵ�����G2_t1_value��16λ
G2_T1_VAL
G2_T1_VAL��G2Э���е�T1��Ӧ��ʱ��ֵ��16λ����ֵ��Ϊ���ܵȴ��������Ĳο�ֵ��������32Mʱ����Ϊ��׼�������ļ���ֵ������T1Ϊ5uS�����ֵΪ32000000��0.000005=160.
*/
#define G2_t1_value  (0x004D)


/*
EPC G2Э���T2ֵ�Ĵ�����G2_t2_value��16λ
G2_T2_VAL
G2_T2_VAL��G2Э���е�T1��Ӧ��ʱ��ֵ����ֵΪ���͵ȴ��������Ĳο�ֵ��������26Mʱ����Ϊ��׼�������ļ���ֵ������T2Ϊ5uS�����ֵΪ26000000��0.000005=130.
*/
#define G2_t2_value  (0x004E)

/*
EPC G2Э���T4ֵ�Ĵ�����G2_t4_value��16λ
G2_T4_VAL
G2_T4_VAL��G2Э���е�T4��Ӧ��ʱ��ֵ����ֵΪ���ܴ��ڼ������Ĳο�ֵ��������26Mʱ����Ϊ��׼�������ļ���ֵ������T4Ϊ5uS�����ֵΪ26000000��0.000005=130.
*/
#define G2_t4_value  (0x0050)


/*
������·״ָ̬ʾ�Ĵ�����phy_col_indicate��16λDET_SIGNAL����ַ��0051H
Bit15-bit5������
DEC_ERR
BEY_MAXPLS
LOW_MINPLS
PRM_MATCH
DET_SIGNAL
DET_SIGNAL���Ƿ��⵽�ź�ָʾ��1����⵽�źţ�0��û�м�⵽�ź�
PRM_MATCH��ǰ����ƥ��ָʾ��1��ƥ�䣻0��û��ƥ��
LOW_MINPLS�����μ��ģ���⵽������С����С��Ч�����ȴ���1������0������
BEY_MAXPLS�����μ��ģ���⵽��������������Ч�����ȴ���1������0������
DEC_ERR���������ָʾ��1������0������
*/
#define REG_reverse_link_status  (0x0051)


/*
EPC G2Э��ı�ǩ�������ͼĴ�����G2_tag_code_type��16λ
Bit15-bit2������    
bit1-bit0   G2_tag_code_type
G2_tag_code_type
00��FM0
01��Miller
��������
*/
#define G2_tag_code_type  (0x0052)

/*
EPC G2Э��ķ�����·�������ʼĴ�����G2_reverse_link_v��16λ
G2_reverse_link_v
G2_reverse_link_v��G2��readerָ����T��>R����·���ʣ�16λ��������������Ϊ160Kʱ����ֵΪ160��
*/
#define G2_reverse_link_v  (0x0056)

/*
EPC G2Э��ǰ��ǰ�������ͼĴ�����G2_forward_preamble_type��16λ
Bit15-bit10������        
Bit9-bit8,RTCal_Ta    
Bit7-bit0,TRCal_RTCal
Prm_type��ǰ���������
0����ʾǰ��������ΪR��>T��ǰ����
1����ʾǰ��������ΪR��>T��ͬ����
RTCal_Ta��RTcal��Tari�ı�ֵ��Ŀǰ�ݶ���ȡ3
TRCal_RTCal��TRcal��Tari�ı�ֵ��
*/
#define G2_forward_preamble_type  (0x0047)

/*
EPC G2Э���з�����·����ǰ����ռ�õ�ʱ��ֵ�Ĵ�����G2_reverse_preamble_t_val��16λ����ַ��0058H
G2_reverse_preamble_t_val��G2��T��>R��ǰ����ռ�õ�ʱ��ֵ����ֵΪ���ܴ��ڼ������Ĳο�ֵ��
������26Mʱ����Ϊ��׼�������ļ���ֵ������T4Ϊ5uS�����ֵΪ26000000��0.000005=130.
*/
#define G2_reverse_preamble_t_val  (0x0058)

/*
EPC G2Э���з�����·����ǰ����ռ�õ�ʱ��ֵ��16λ�Ĵ�����G2_reverse_preamble_t_val_h��16λ����ַ��00b1H
G2_reverse_preamble_t_val_h
G2��T��>R��ǰ����ռ�õ�ʱ��ֵ����ֵΪ���ܴ��ڼ������Ĳο�ֵ��������32Mʱ����Ϊ��׼�������ļ���ֵ������T4Ϊ5uS�����ֵΪ32000000��0.000005=130.
G2_reverse_preamble_t_val_l���üĴ���ΪG2��T��>R��ǰ����ռ�õ�ʱ��ֵ��16λֵ
*/
#define G2_reverse_preamble_t_val_h (0x00b1)

/***********************************************************
               ��Ƶ���üĴ���
************************************************************/ 


/* PLL�����Ĵ�������,  Bit7-bit0��24λPLL���������ݸ߰�λ */
#define REG_PLL_DATA_H      (0x005F)

/* PLL�����Ĵ�������,  Bit15-bit0��24λPLL���������ݵ�16λ */
#define REG_PLL_DATA_L      (0x0060)


/* PLL���ô����Ĵ�����PLL_TRIG�� 16λ
Bit15-bit1������
bit0 PLL_TRIG
1������PLL����ģ�鿪ʼ����PLL,��ʾ�����׼�����������ݣ�Ҫȥ�߼���ʼ���͡�
0��null
*/
#define REG_PLL_TRIG      (0x0061)

#define PLL_DATA_READY_TRIG  (0x1)

/* PLL׼���ã�PLL_READY��16λ
Bit15-bit1������
bit0 PLL_RDY
1����ǰ�߼������Ѿ�������ɣ����ѿ�ʼ��һ��PLL���ݴ��͡�
0�����ڴ���PLL�������ݣ�
*/
#define REG_PLL_READY      (0x0063)

#define PLL_CFG_RDY  (1)  /* 1����ǰ�߼������Ѿ�������ɣ����ѿ�ʼ��һ��PLL���ݴ��͡� */
#define PLL_CFG_BUSY (0)  /* 0�����ڴ���PLL�������ݣ� */


/*
��Ϸ�ʽѡ��Ĵ�����COUPLE_TYPE��16λ������ַ��0066H
Bit15-bit1������
COUPLE_SW
COUPLE_SW
1��DC������·�Ͽ�
0��DC������·ͨ
*/

#define REG_COUPLE_TYPE (0x0066)

/*
��������״ָ̬ʾ�Ĵ�����ANT_STATUS��16λ
Bit15-bit4������    
bit3-bit0   ANT_STAT3    ANT_ STAT 2    ANT_ STAT 1    ANT_ STAT 0
ANT_STAT0��ANT_STAT3�ֱ��Ӧ0��1��2��3������
1����ʾ��Ӧ�����߶˿���������������
0����ʾ��Ӧ�����߶˿�û�м�⵽��������������
*/
#define REG_ANT_STATUS      (0x0067)


/*
����ͨ���������ض�ʹ�ܼĴ�����MOD_EN��16λ����ַ��0073H
Bit15-bit1������
MOD_EN
MOD_EN��
1���򿪷���ͨ��������
0���ضϷ���ͨ��������
*/
#define REG_MOD_EN      (0x0073)


/*
���ſ��ؿ��ƼĴ�����PA_EN��16λ����ַ��0074H
Bit15-bit2
bit1       PA_ENB
������ؿ��ƼĴ����������Ķ�����ǰ��ͨ���Ƿ�򿪣�
ǰ��ͨ�������Ĵ�������Ӧ��Ϊ��
1��LBT����������������ŵ�Ϊ���У�
2���м���·������д��ǰ�����������1���ڷ�ŷ�ޱ�׼Ӧ�������£��ɲ��򿪡�
*/
#define REG_RF_TX_Ctrl  (0x0074)


/*
��ֹDAC�����Ĵ�����DAC_SLEEP����ַ��007CH
Bit15-bit1������
Dac_sleep
Dac_sleep
1����ֹdac����
0��ʹ��dac����
*/
#define REG_DAC_SLEEP  (0x007c)


/*
Select����ȼĴ��� SELECT_LENTH ��ַ 000dH
Bit15-bit8 ����
Bit7-bit0 select_lenth
*/
#define REG_G2_Select_Mask_BitLen  (0x000d)


/*
�����źŵ���Ч�ź�����ֵ�Ĵ�����sam_valid_signal_val��16λ����ַ��0054H
Bit15-bit12������
sam_valid_signal_val
sam_valid_signal_val�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define REG_sam_valid_signal_val  (0x0054)


/* �½�������ֵ����ϵ���Ĵ�����signal_down_thresh_reg_factor��16λ����ַ��0055H
   Bit15-bit9������
   Bit7-bit0��Factor
    ���������޵���ֵ�Ĵ�������Χ��1-128����������Ϊ28 */

#define REG_signal_down_thresh_reg_factor  (0x0055)


/* ����������ֵ����ϵ���Ĵ�����signal_up_thresh_reg_factor��16λ����ַ��0093H
   Bit15-bit9������
   Bit7-bit0��Factor
    ���������޵���ֵ�Ĵ�������Χ��1-128����������Ϊ���½�����ͬ��ֵ */
#define REG_signal_up_thresh_reg_factor  (0x0093)




/*DAC�����ֵ�Ĵ�����DAC_DC_VALUE_REG����ַ��0087H
Bit15-bit14������
Bit13��bit0,dac_dc_value
��ֵΪDAC���Ϊ0��ƽʱ��Ӧ��DAC��14λDAC��������ֵ����ϵͳ����Ϊ2000H��1fffH����������Ϊ2000H����λĬ��ֵΪ2000h
*/ 
//#define DAC_DC_VALUE_REG 0x0087

/* �����������޿��ƼĴ�����sam_valid_signal_ctrl����ַ��0088H
Bit15-bit14������
bit0,sam_valid_signal_ctrl
Bit0:sam_valid_signal_ctrl
0����������������FPGA����ó�
1����������������CPU���ã���cpu��Ҫ����sam_valid_signal_val�Ĵ�����д�뷴����������
��λĬ����FPGA����
*/ 
//#define sam_valid_signal_ctrl 0x0088

/* CPU����FPGA���з����������޲��ԼĴ�����cpu_trig_noise_test_reg����ַ��008aH
Bit15-bit14������
bit0, cpu_trig_noise_test
Bit0: cpu_trig_noise_test
CPUÿ�������λд��0��Ȼ����д��1������FPGA���з����������޲��� 
*/
#define cpu_trig_noise_test_reg 0x008a

/* FPGA���з����������޲���ʱ�򿪹��ź���ӳ�ֵ��16λ��noise_test_delay_val_h����ַ��008bH
Bit15-bitt0, noise_test_delay_val_h
��ֵΪFPGA���������ķ�����������ֵ
FPGA���з����������޲���ʱ����Ҫ���ȴ򿪹��ţ�Ȼ���ӳ�һ��ʱ����ٿ�ʼ��ADC����Ĳ������ݽ������޲��������ӳ���26MHzʱ������������Ҫ�ӳ�10s������ӳٵļ���ֵӦ������Ϊ10��26000000�� F7F4900H����λĬ���ӳ�ֵΪ1s��
�üĴ���ֵΪ���ӳټ���ֵ�ĸ�16λ����λĬ��ֵΪ18cH
*/
#define noise_test_delay_val_h 0x008b

/* FPGA���з����������޲���ʱ�򿪹��ź���ӳ�ֵ��16λ��noise_test_delay_val_l����ַ��008cH
Bit15-bitt0, noise_test_delay_val_l
��ֵΪFPGA���������ķ�����������ֵ
FPGA���з����������޲���ʱ����Ҫ���ȴ򿪹��ţ�Ȼ���ӳ�һ��ʱ����ٿ�ʼ��ADC����Ĳ������ݽ������޲��������ӳ���26MHzʱ����������
��Ҫ�ӳ�10s������ӳٵļ���ֵӦ������Ϊ10��26000000�� F7F4900H���üĴ���ֵΪ���ӳٵĵ�16λ����λĬ��ֵΪba80H
*/
#define noise_test_delay_val_l 0x008c


/* FPGA���з����������޲���ʱͳ�ƵĲ���������noise_test_sampoint_cnt����ַ��008dH
Bit15-bitt0, noise_test_sampoint_cnt
��ֵΪFPGA���������ķ�����������ֵʱͳ�ƵĲ���������FPGA�����������޲�����Ҫ��һ�������Ĳ�����������ͳ�ƣ��ҳ�������Сֵ�����Ϊ���Գ�������������ֵ����������Ϊ4000��FA0H����λĬ��ֵΪFA1H��
*/
#define noise_test_sampoint_cnt 0x008d

/*FPGA���з����������޲��������޵���ֵ�� noise_test_adj_val����ַ��008eH
Bit15-bitt0, noise_test_adj_val
ͨ��ʵ�ʲ��Է��֣�FPGA��������������ֵfpga_sam_valid_signal_val��Ҫ���е�����һ��Ϊ���ϻ��ȥһ��ֵ����ֵ��CPUͨ��noise_test_adj_valд�롣��λĬ��Ϊ140H��
*/
#define noise_test_adj_val 0x008e



/* �����������޲�������ϵ����fpga_noise_adj_factor����ַ��0090H
Bit15-bit14������
bit0,sam_valid_signal_ctrl
Bit0:sam_valid_signal_ctrl
0��������������ֵΪfpga_sam_valid_signal_val��noise_test_adj_val
1��������������ֵΪfpga_sam_valid_signal_val��noise_test_adj_val
��λĬ��Ϊ0��
����Ҫ��
�ı�G2_forward_preamble_type�Ĵ���ֵʱ�����볬ǰ�ڴ�����ǩ���ʵ��ź�2uS��
ASK����ʱ��module_depth������ȼĴ���ֵ���ã�Ŀǰ����DAC������ƽ��Ӧ2000H��DAC����ߵ�ƽ������ز���ӦֵΪ0000H���������ȼ���Ϊ80��ʱ��module_depthֵ�������£�
1fffH��(1fffH-0000H)*80%
�򿪹��ŵ�ǰ��������
����DAC_SLEEP�Ĵ�����ʹ��DAC��������������MOD_EN�Ĵ�����ʹ�ܵ���������������ANT_SEL�Ĵ�����ѡ�����е�һ�����ߣ���������ѡ�������������״̬�����ã�����ѡ��û���������ߵĶ˿ڣ�������Ҫ��������LBT_SWITCH���ر�LBT���ڣ�Ŀǰ���߼�����û�и��������ơ���������������������������PA_EN�Ĵ�����ʹ�ܹ��ţ����ŲŻ�򿪣���Ƶ���й������������ʹPA_EN�Ĵ�������Ϊʹ�ܹ��ţ�����Ҳ����򿪡�
*/
#define fpga_noise_adj_factor  0x0090


#define fpga_ant0_noise_thresh_val_i 0x009c

/*FPGA���Ե�����0��Ӧ��I·��������ֵ��fpga_ant0_noise_thresh_val_q��16λ����ַ��009dH
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant0_noise_thresh_val_q 0x009d

/*FPGA���Ե�����1��Ӧ��I·��������ֵ��fpga_ant1_noise_thresh_val_i��16λ����ַ��009eH
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant1_noise_thresh_val_i 0x009e


/*FPGA���Ե�����1��Ӧ��I·��������ֵ��fpga_ant1_noise_thresh_val_q��16λ����ַ��009fH
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant1_noise_thresh_val_q 0x009f

/*FPGA���Ե�����2��Ӧ��I·��������ֵ��fpga_ant2_noise_thresh_val_i��16λ����ַ��00a0H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant2_noise_thresh_val_i 0x00a0

/*FPGA���Ե�����2��Ӧ��I·��������ֵ��fpga_ant2_noise_thresh_val_q��16λ����ַ��00a1H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant2_noise_thresh_val_q 0x00a1

/*
FPGA���Ե�����3��Ӧ��I·��������ֵ��fpga_ant3_noise_thresh_val_i��16λ����ַ��00a2H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant3_noise_thresh_val_i    0x00a2

/* FPGA���Ե�����3��Ӧ��I·��������ֵ��fpga_ant3_noise_thresh_val_q��16λ����ַ��00a3H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_ant3_noise_thresh_val_q 0x00a3

/* ����fpga���Ե�����ֵ�������0��Ӧ��I·��������ֵ��fpga_adj_ant0_noise_thresh_val_i��16λ����ַ��00a4H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant0_noise_thresh_val_i 0x00a4


/* ����fpga���Ե�����ֵ�������0��Ӧ��I·��������ֵ��fpga_ adj_ant0_noise_thresh_val_q��16λ����ַ��00a5H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant0_noise_thresh_val_q 0x00a5

/* ����fpga���Ե�����ֵ�������1��Ӧ��I·��������ֵ��fpga_ adj_ant1_noise_thresh_val_i��16λ����ַ��00a6H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant1_noise_thresh_val_i 0x00a6

/* ����fpga���Ե�����ֵ�������1��Ӧ��I·��������ֵ��fpga_ adj_ant1_noise_thresh_val_q��16λ����ַ��00a7H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant1_noise_thresh_val_q 0x00a7

/* ����fpga���Ե�����ֵ�������2��Ӧ��I·��������ֵ��fpga_ adj_ant2_noise_thresh_val_i��16λ����ַ��00a8H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant2_noise_thresh_val_i 0x00a8

/* ����fpga���Ե�����ֵ�������2��Ӧ��I·��������ֵ��fpga_ adj_ant2_noise_thresh_val_q��16λ����ַ��00a9H
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant2_noise_thresh_val_q 0x00a9

/* ����fpga���Ե�����ֵ�������3��Ӧ��I·��������ֵ��fpga_ adj_ant3_noise_thresh_val_i��16λ����ַ��00aaH
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant3_noise_thresh_val_i 0x00aa

/*����fpga���Ե�����ֵ�������3��Ӧ��I·��������ֵ��fpga_ adj_ant3_noise_thresh_val_q��16λ����ַ��00abH
sam_valid_signal_val
ant0_noise_thresh_val_i�������������ֵ����Сֵ���һ����ڸ�ֵʱ����Ϊ����Ч�ź�
*/
#define fpga_adj_ant3_noise_thresh_val_q 0x00ab

/* �����������޲���״̬�Ĵ�����fpga_noise_test_status����ַ��00acH
Bit15-bit14������
bit0,noise_test_end
Bit0 noise_test_end
0�����ڽ����������޲�������ʱ��������ǩ
1�������������޲�����ɣ���������ǩ
��λĬ��Ϊ0��
*/
#define fpga_noise_test_status 0x00ac

/* G2����д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ��16λ��G2_write_rcv_delay_val_h��16λ����ַ��00adH
Bit15��bit0��G2_write_rcv_delay_val_h
��ֵΪG2Э���д���д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ��16λ��G2����д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ������26Mʱ����Ϊ��׼�������ļ���ֵ������T2Ϊ5uS�����ֵΪ26000000��0.000005=130. ��ֵĬ��Ϊ20ms��Ӧ��ʱ��ֵ��
*/
#define G2_write_rcv_delay_val_h 0x00ad

/* G2����д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ��16λ��G2_write_rcv_delay_val_l��16λ����ַ��00aeH
Bit15��bit0��G2_write_rcv_delay_val_l
��ֵΪG2Э���д���д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ��16λ��G2����д��ǩ�����ĵ�����Ľ��ܴ��ڳ�ʱ�������ӳ�ʱ��ֵ������26Mʱ����Ϊ��׼�������ļ���ֵ������T2Ϊ5uS�����ֵΪ26000000��0.000005=130.��ֵĬ��Ϊ20ms��Ӧ��ʱ��ֵ��
*/
#define G2_write_rcv_delay_val_l 0x00ae

#define REG_fir_sig_down_thresh_factor  (0x00AF)
#define REG_fir_sig_up_thresh_factor  (0x00B0)


/*
DAC����ߵ�ƽֵ�Ĵ�����DAC_HIGH_VALUE_REG����ַ��0086H
Bit15-bit14������
Bit13��bit0,dac_high_value
��ֵΪDAC����ز�ʱ��Ӧ��DAC��14λDAC��������ֵ��һ������Ϊ3fffH��0000H����������Ϊ3fffh
*/
#define REG_DAC_HIGH_VALUE_REG  (0x0086)

#define REG_FPGA_SAM_VALID_SIGNAL_CONTROL (0x0088)
#define REG_FPGA_SAM_VALID_SIGNAL_VAL     (0x0089)

/* 18000-6B ����ģʽ�Ĵ�������ַ��0012H
����	Bit1~Bit0        18000B_cmd_mode
18000B_cmd_mode��
01�����ģʽ
10��ֱ���������ģʽ
*/
#define REG_18000B_cmd_mode (0x0012)

#define ISO18000B_cmd_mode_invent        (0x1)
#define ISO18000B_cmd_mode_direct_access (0x2)
#define ISO18000B_cmd_mode_noinit_invent   (0x3)


/* 18000-6B ѡ���������ͼĴ�������ַ��0013H
����	Bit7~Bit0        18000B_selectcmd_typ
18000B_selectcmd_typ����Э��涨����������ͬ

*/
#define     GROUP_SELECT_EQ     0x00
#define     GROUP_SELECT_NE     0x01
#define     GROUP_SELECT_GT     0x02
#define     GROUP_SELECT_LT     0x03
#define     GROUP_UNSELECT_EQ   0x04
#define     GROUP_UNSELECT_NE   0x05
#define     GROUP_UNSELECT_GT   0x06
#define     GROUP_UNSELECT_LT   0x07

#define GROUP_SELECT_EQ_FLAGS   0x17
#define GROUP_SELECT_NE_FLAGS   0x18
#define GROUP_UNSELECT_EQ_FLAGS 0x19
#define GROUP_UNSELECT_NE_FLAGS 0x1A

#define REG_18000B_selectcmd_type (0x0013)

#define ISO18000B_selectcmd_type_GROUP_SELECT_EQ  (0x0)

/* 18000-6B �����������ͼĴ�������ַ��0014H
����	Bit7~Bit0        18000B_accesscmd_typ
18000B_accesscmd_typ����Э��涨����������ͬ
0B�� Recommended   DATA_READ
0C�� Mandatory     READ
0D�� Recommended   WRITE
0E�� Recommended   WRITE_MULTIPLE
0F�� Recommended   LOCK
11�� Recommended   QUERY_LOCK
12�� Recommended   READ_VERIFY
1B�� Recommended   WRITE4BYTE
1C�� Recommended   WRITE4BYTE_MULTIPLE
1D�� Recommended   READ_VERIFY_4BYTE
*/
#define REG_18000B_accesscmd_type (0x0014)

#define ISO18000B_accesscmd_type_READ (0x000C)
#define ISO18000B_accesscmd_type_Variable_READ (0x0051)

/* add by mk */
#define ISO18000B_accesscmd_type_Write4Bytes (0x001B)
#define ISO18000B_accesscmd_type_WriteByte (0x000D)
#define ISO18000B_accesscmd_type_Lock (0x000F)
#define ISO18000B_accesscmd_type_LockQuery (0x0011) 
/* 18000-6Bѡ�������ַ�����Ĵ�������ַ��0015H
����	Bit7~Bit0        18000B_sel_address
18000B_sel_address����ǩ�Ӹõ�ַ��ʼ����ȡ֮��Ĵ洢�����Ķ������͵�WORD_DATA���бȽ�
 */
#define REG_18000B_sel_address (0x0015)

/* 18000-6Bѡ��������������Ĵ�������ַ��0016H
����	Bit7~Bit0        18000B_sel_mask
18000B_sel_mask���μ���׼��8.2.7.7.3 Data comparison for selection command on flags
Bit 7 (MSB) is set Consider D7 and FLAG7 for comparison
Bit 6 is set Consider D6 and FLAG6 for comparison
Bit 5 is set Consider D5 and FLAG5 for comparison
Bit 4 is set Consider D4 and FLAG4 for comparison
Bit 3 is set Consider D3 and FLAG3 for comparison
Bit 2 is set Consider D2 and FLAG2 for comparison
Bit 1 is set Consider D1 and FLAG1 for comparison
Bit 0 (LSB) is set Consider D0 and FLAG0 for comparison
Bit 7 (MSB) is cleared Ignore D7 and FLAG7 for comparison
Bit 6 is cleared Ignore D6 and FLAG6 for comparison
Bit 5 is cleared Ignore D5 and FLAG5 for comparison
Bit 4 is cleared Ignore D4 and FLAG4 for comparison
Bit 3 is cleared Ignore D3 and FLAG3 for comparison
Bit 2 is cleared Ignore D2 and FLAG2 for comparison
Bit 1 is cleared Ignore D1 and FLAG1 for comparison
Bit 0 (LSB) is cleared Ignore D0 and FLAG0 for comparison
 */
#define REG_18000B_sel_mask (0x0016)


/* 18000-6Bѡ������WORD_DATA�Ĵ���0����ַ��0017H
Bit15~Bit0           18000B_sel_worddata_0
*/
#define REG_18000B_sel_worddata_0 (0x0017)

/* 18000-6Bѡ������WORD_DATA�Ĵ���1����ַ��0018H
Bit15~Bit0           18000B_sel_worddata_1
*/
#define REG_18000B_sel_worddata_1 (0x0018)

/* 18000-6Bѡ������WORD_DATA�Ĵ���2����ַ��0019H
Bit15~Bit0           18000B_sel_worddata_2
*/
#define REG_18000B_sel_worddata_2 (0x0019)

/* 18000-6Bѡ������WORD_DATA�Ĵ���3����ַ��001AH
Bit15~Bit0           18000B_sel_worddata_3
*/
#define REG_18000B_sel_worddata_3 (0x001A)

/* 18000-6B FLAGSѡ������WORD_DATA�Ĵ���3����ַ��001BH
Bit15~Bit0           18000B_sel_worddata_3
*/
#define REG_18000B_sel_flags_Byte_data (0x001B)


/* 18000-6B��������UID�Ĵ���0����ַ��001CH
Bit15~Bit0          18000B_UID_reg_0 */
#define REG_18000B_access_UID_0 (0x001C)

/* 18000-6B��������UID�Ĵ���1����ַ��001DH
Bit15~Bit0          18000B_UID_reg_1 */
#define REG_18000B_access_UID_1 (0x001D)

/* 18000-6B��������UID�Ĵ���2����ַ��001EH
Bit15~Bit0          18000B_UID_reg_2 */
#define REG_18000B_access_UID_2 (0x001E)

/* 18000-6B��������UID�Ĵ���3����ַ��001FH
Bit15~Bit0          18000B_UID_reg_3 */
#define REG_18000B_access_UID_3 (0x001F)

/* 18000-6B���������ַ�����Ĵ�������ַ��0020H
����	Bit7~Bit0        18000B_Address
18000B_Address����ǩ�Ӹõ�ַ��ʼ���ж�д���� */
#define REG_18000B_access_Address (0x0020) 

/* 18000-6B��������д���ݼĴ���0����ַ��0021H */
#define REG_18000B_Read_Variable_Len  (0x0021)

/* add by mk */
/* 18000-6B��������д���ݼĴ���0����ַ��0021H */
#define REG_18000B_write_data_0  (0x0021)

/* 18000-6B��������д���ݼĴ���1����ַ��0022H */
#define REG_18000B_write_data_1  (0x0022)

/* 18000-6B��������д���ݼĴ���2����ַ��0023H */
//#define REG_18000B_write_data_2  (0x0023)

/* 18000-6B��������д���ݼĴ���3����ַ��0024H */
//#define REG_18000B_write_data_3  (0x0024)

/* 18000-6Bд������������Ĵ�������ַ��0025H */
#define REG_18000B_write_bytemask  (0x0023) 
/* add by mk for 6B write */
/* 18000-6B д��������Ƿ�������*/
#define REG_18000B_write_continue  (0x0024)



/*
Ӳ����������Epld��ƫ�Ƶ�ַ0x0184��ȡ��ֵ
 
�������EPLD�ϵ��汾�Ĵ���ֵΪ01����ѡ��FPGA�Ĵ���00BA=0��
�������EPLD�ϵ��汾�Ĵ���ֵΪ00����ѡ��FPGA�Ĵ���00BA=1��
*/
#define REG_FPGA_PATCH_VERSION    (0x00ba) 
/*
��ǩ���ٹ��ܵļĴ���
1��	������Ƶ�����ڼĴ��� Doppler_shift_timer����ַ��00BBH
2��	������Ƶ����Ч���ڴ����Ĵ��� Doppler_shift_times����ַ��00BCH
*/
#define REG_DOPPLER_SHIFT_TIMER    (0x00BB)
#define REG_DOPPLER_SGIFT_TIMES    (0x00BC)

/* Added  by gaoyu for DATTControl, 2007��8��17�� 10:01:17 */
/*
������������˥��ֵ�Ĵ��� 
Bit4 - Bit0Ϊ rx_lo2_datt�Ĵ���
�ɵ��ڷ�ΧΪ0dbm - 15.5dbm 
*/
#define FPGA_TESTVERSION_CTRL  (0x00c5)

#define FPGA_RSSI_SWITCH    (0x00d4)



/*2010.1.26 cj add*/
/*
block lock����MemBank�Ĵ�����ַ005b
Bit15~Bit3 	BIT2:read/lock		Bit1~Bit0   MemBank     
MemBank��
00: Reserved 
01: EPC 
10: TID 
11: User
READ/LOCK:
0:read
1:lock
 
*/
#define REG_G2_Block_Lock_MemBank  (0x005b)


/*	Blockpermalock����EVB��ַ�Ĵ�������ַ��005CH
Bit15~Bit8   EBV  BLOCK2	Bit7~Bit0        EBV  BLOCK1
��16����Ϊ��λEBV  BLOCK1��   ���뿪ʼ��ַ�ĵ�1��EBV��
EBV  BLOCK2��   ���뿪ʼ��ַ�ĵ�2��EBV��
*/
#define REG_G2_Blcokpermalock_EVB (0x005c)


/*
Blockpermalock����BLOCKRANGE������Χ�Ĵ�������ַ��005DH
����	Bit7~Bit0        Blockrange
Blockrange����������Χ�����256��16blocks��
Blockpermalock�������ݼĴ�������ַ�ռ�Ϊ20000H��2FFFFH��
�ܹ�128K����ַΪ20000Hʵ��ռ��512�ֽ�(256����)��
*/

#define REG_G2_Blockpermalock_Blockrange (0x005d)

/*Blockreadlock��������������ALIEN��ǩ���Ŀ鷶Χ�Ĵ�������ַ��005eH
BIT15~BIT8:����	Bit7~Bit0        blockcnt
*/


#define REG_G2_Blockreadlock_mask (0x005e)
/*2010.1.26add end*/

/* FPGA ��EPCЭ���ж�д�����Դ����Ĵ���*/
#define REG_FPGA_WRITE_READ_RETRY_TIME  (0x00d0)

/* FPGA ��ISO Qֵ���üĴ���*/
#define REG_FPGA_ISO6B_INVENTQ  (0x0076)



/* IPJ ���õĶ�д��״̬ģʽ*/
#define REG_I_QT_WR_PER  (0x0089)

/* IPJָ��ľ���*/
#define REG_I_QT_PAYLOAD  (0x008a)

////////////////////////
/// GB Reg Def
////////////////////////

#define ACCESS_KILL      0x10//0b00010000 
#define ACCESS_LOCK     0x32//0b00110010
#define ACCESS_READ     0x76//0b01110110
#define ACCESS_WRITE   0x98//0b10011000

#define REG_GB_Sort_Para  (0xC001)
#define REG_GB_Sort_Cmd_Ptr  (0xC002)
#define REG_GB_Sort_Cmd_BitLen  (0xC003)

#define REG_GB_Query_Para  (0xC004)

#define REG_GB_Read_Membank  (0xC005)
#define REG_GB_Read_Addr  (0xC006)
#define REG_GB_Read_WordCount  (0xC007)

#define REG_GB_Write_Membank  (0xC008)
#define REG_GB_Write_Addr  (0xC009)
#define REG_GB_Write_WordCount (0xC00A)

/* LOCK ������� */
#define REG_GB_Lock_Para   (0xC00B)

/*
  access ���ʴ洢������
  bit0-bit5 �洢��
  bit6-bit9 �������
*/
#define REG_GB_Access_Membank (0xC00C)

#define REG_GB_Access_Password_L (0xC00D)
#define REG_GB_Access_Password_H (0xC011)

/* ERASE ������� */

#define REG_GB_Erase_Membank        (0xC00E)
#define REG_GB_Erase_Addr               (0xC00F)
#define REG_GB_Erase_WordCount     (0xC010)

/*
select/sort����MASK�Ĵ���
MASK word31
MASK word30
-----------
MASK word3
MASK word2
MASK word1
MASK word0
 
MASK�Ĵ����鹲32�������Դ洢���512��BIT�����롣
�üĴ�������Կ���һƬ�����Ĵ洢������ͬ�����ڴ�顣
*/
#define REG_GB_Select_Mask_Start     (0x10000)

#define REG_GB_BlockWrite_Data_Start (0x20000)
/* FPGA ��EPCЭ���ж�д�����Դ����Ĵ���*/
#define REG_GB_FPGA_WRITE_READ_RETRY_TIME  (0x00d0)

/*
kill�������루�߰��֣��Ĵ�������ַ��0036H
Bit15~Bit0 Password_h ( high half word)
Password_h: 32λ����ĸ߰��֡�
*/
#define REG_GB_Kill_Password_H (0x0036)

/*kill�������루�Ͱ��֣��Ĵ�������ַ��0037H
Bit15~Bit0 Password_L ( low half word)
Password_L: 32λ����ĵͰ��֡�
*/
#define REG_GB_Kill_Password_L (0x0037)

/*
kill����RFU�Ĵ�������ַ��0038H
����
Bit2~Bit0 RFU
RFU�� Killָ���������RFUλ�����Ķ�����Class-1�ı�ǩͨѶʱ���Ķ�������Щλ��Ϊ0002��Class-1��ǩ�����⼸λ��
���߹��ܵı�ǩ����ʹ����Щλ����չKillָ��Ĺ��ܣ��磬ɱ��һ����ǩʹ֮����һ����������״̬��������ֱ��ɱ����
*/
#define REG_GB_Kill_RFU (0x0038)

#endif


