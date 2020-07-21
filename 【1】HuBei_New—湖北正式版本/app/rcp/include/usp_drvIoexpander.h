/************************************************************************
* ��Ȩ���� (C)2010, ����ͨѶ����о���
* 
* �ļ����ƣ� usp_ioexpander.h
* �ļ���ʶ�� 
* ����ժҪ�� ioexpanderģ�����ӿ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾�� USP01.20.06
* ��            �ߣ� ���½�
* ������ڣ� 2010-12-30
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/


#ifndef _USP_IOEXPANDER_H_
#define _USP_IOEXPANDER_H_




/*6700  I2Cת����оƬIO���ܺ궨��*/

/*RFCB��������*/
#define USP_IOE_RFCB_TYPE_ID          0x00
#define USP_IOE_RFCB_TYPE_ID_MASK     0x0F
#define USP_IOE_RFCB_TYPE_ID_POS      0x0

/*ǰ���ʿ���*/
#define USP_IOE_TX_DATT               0x01
#define USP_IOE_TX_DATT_MASK          0xFC
#define USP_IOE_TX_DATT_POS           0x02


/*���߿������п����ź�*/
#define USP_IOE_ANT_CONTROL           0x02
#define USP_IOE_ANT_CONTROL_MASK      0x03
#define USP_IOE_ANT_CONTROL_POS       0x0


/*����DATT����*/
#define USP_IOE_RX_DATT               0x03
#define USP_IOE_RX_DATT_MASK          0xFC
#define USP_IOE_RX_DATT_POS           0x2


/*��������ʹ�ܿ���*/
#define USP_IOE_RX_LNA_EN             0x04
#define USP_IOE_RX_LNA_EN_MASK        0x02
#define USP_IOE_RX_LNA_EN_POS         0x01



/*������ѡ�񿪹�*/
#define USP_IOE_RX_LO_SEL             0x05
#define USP_IOE_RX_LO_SEL_MASK        0x01
#define USP_IOE_RX_LO_SEL_POS         0x0


/*���������ı���˥��ֵ*/
#define USP_IOE_RX_LO_DATT            0x06
#define USP_IOE_RX_LO_DATT_MASK       0xF8
#define USP_IOE_RX_LO_DATT_POS        0x03


/*���ʼ��ͨ·ѡ��*/
#define USP_IOE_PWRDET_SEL            0x07
#define USP_IOE_PWRDET_SEL_MASK       0x04
#define USP_IOE_PWRDET_SEL_POS        0x02


/*��������˲�ͨ·ѡͨ*/
#define USP_IOE_ADG704_A              0x08
#define USP_IOE_ADG704_A_MASK         0x03
#define USP_IOE_ADG704_A_POS          0x0


/*RFCB����İ汾*/
#define USP_IOE_RFCB_VER_ID           0x09
#define USP_IOE_RFCB_VER_ID_MASK      0x3F
#define USP_IOE_RFCB_VER_ID_POS       0x0


/*����ΪB��оƬ���й���*/
#define USP_IOE_ICB_FUN_ALL           0x0A
#define USP_IOE_ICB_FUN_ALL_MASK      0xFF
#define USP_IOE_ICB_FUN_ALL_POS       0x0


/*����ΪC��оƬ���й���*/
#define USP_IOE_ICC_FUN_ALL           0x0B
#define USP_IOE_ICC_FUN_ALL_MASK      0xFF
#define USP_IOE_ICC_FUN_ALL_POS       0x0


/*����ΪD��оƬ���й���*/
#define USP_IOE_ICD_FUN_ALL           0x0C
#define USP_IOE_ICD_FUN_ALL_MASK      0xFF
#define USP_IOE_ICD_FUN_ALL_POS       0x0




WORD32 usp_drv_ioexpander_init(void);
WORD32 usp_drv_ioexpander_snd(BYTE ucFID , WORD32 dwData);
WORD32 usp_drv_ioexpander_rcv (BYTE ucFID , WORD32 *dwData);


#endif


