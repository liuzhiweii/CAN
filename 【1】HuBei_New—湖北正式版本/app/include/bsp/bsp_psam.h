/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�bsp_drvPsam.h
* �ļ���ʶ��
* ����ժҪ��PSAM ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��            �ߣ�
* ������ڣ�
************************************************************************/

#ifndef _BSP_DRV_PSAM_H_
#define _BSP_DRV_PSAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tagT_CardResetPara
{
    unsigned char   ucSockId;
    unsigned char   aucPad[3];
    unsigned short wEtu;
    unsigned short wPad;
    unsigned int dwErrCode;
    unsigned int dwAtrLen;
    unsigned char   aucAtr[32];
    unsigned int dwHistLen;
    unsigned char   aucHistory[32];
}T_CardResetPara;

typedef struct tagT_CardProcPara
{
    unsigned char   ucSockId;
    unsigned char   ucPad;
    unsigned char   ucCmdLen;
    unsigned char   ucResLen;    
    
    unsigned int dwErrCode;
    unsigned char   aucCmd[256];
    unsigned char   aucRes[256];
}T_CardProcPara;	
typedef struct tagT_CardPara
{
    unsigned short wEtu;
    unsigned short wPad;
    unsigned int dwAtrLen;
    unsigned char   aucAtr[32];

    unsigned int dwHistLen;
    unsigned char   aucHistory[32];
}T_CardPara;

/*�����붨��*/
/* rcv send char error */
#define  PHY7816_ERR_CTL_PTR            0x00000001    /*���ƽṹ��ָ��*/
#define  PHY7816_ERR_DATA_ADDR          0x00000002    /*�����ߵ�ַ��ָ��*/
#define  PHY7816_ERR_TIME_OUT           0x00000004    /*�����ַ���ʱ*/
#define  PHY7816_ERR_PARITY             0x00000008    /*�����ַ���żУ���*/
#define  PHY7816_ERR_SEND_char          0x00000010    /* �����ַ�����*/
#define  PHY7816_ERR_SIGNAL_INT         0x00000020    /* �ź��жϴ���*/


/*ATR*/
#define  PHY7816_ERR_NO_TS              0x00000100    /*û�յ�TS*/
#define  PHY7816_ERR_TS_NOT_3B          0x00000200    /*TS����3B*/
#define  PHY7816_ERR_NO_T0              0x00000400    /*û�յ�T0*/
#define  PHY7816_ERR_HISTORY            0x00000800    /*������ʷ�ֽڴ���*/

/*active */
#define PHY7816_ERR_START_ETU           0x00001000    /*��֧�ֵ�START_ETU*/
#define PHY7816_ERR_ATR                 0x00002000    /* ����ATRʧ��*/
#define PHY7816_ERR_PPS_RCV             0x00004000    /*  PPS�����У���������ʧ��*/
#define PHY7816_ERR_PPS                 0x00008000    /* PPSʧ��*/

/* apdu transcieve  */
#define  PHY7816_ERR_CMD_PARA           0x00010000    /*�����������*/
#define  PHY7816_ERR_SND_CMD_HEADER     0x00020000    /* ��������ͷʧ��*/
#define  PHY7816_ERR_RCV_INS            0X00040000    /*����INSʧ��*/
#define  PHY7816_ERR_SND_LC_DATA        0x00080000    /* ����LC���������ʧ��*/
#define  PHY7816_ERR_RCV_LE_DATA        0x00100000    /* ����LE����ķ�������ʧ��*/
#define  PHY7816_ERR_RCV_FIRST_STATUS   0x00200000    /* ���տ�Ƭ���صĵ�һ���ֽ�����ʧ��*/
#define  PHY7816_ERR_RCV_SECOND_STATUS  0x00400000    /* ���տ�Ƭ���صĵڶ����ֽ�����ʧ��*/
#define  PHY7816_ERR_SND_GETRESPONSE    0x00800000    /* ����get_responseʧ��*/

#define  PHY7816_ERR_OPEN_FILE          0x01000000    /* ��PSAM����ʧ��*/
#define  PHY7816_ERR_IOCTL              0x02000000    /*IOCTLϵͳ����ʧ��*/
#define  PHY7816_ERR_FD                 0x04000000    /* ������ļ�������*/
#define  PHY7816_MAP_FAILED             0x08000000    /* mmapϵͳ����ʧ��*/

#define  PSAM_VOLTAGE_3V3               0x1           /*��ѹ����ֵ3.3V*/
#define  PSAM_VOLTAGE_1V8               0x0           /*��ѹ����ֵ1.8V*/


#define BSP_DRV_PSAM_NR (0x4)
#define BSP_DRV_PSAM_LINUX_DEVICE "/dev/misc/drv_psam"
#define BSP_DRV_PSAM_CLKFLEQ_MAX (15)
#define BSP_DRV_PSAM_BASE_ADDR (0xc0000000)
#define BSP_DRV_PSAM_ADDR_WIDTH (0x01000000)


/**********************************************************************
* �������ƣ�bsp_psam_init
* ����������psam��ʼ������
* �����������
* �����������
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/
unsigned int bsp_psam_init (void);
/**********************************************************************
* �������ƣ�bsp_psam_active
* ����������psam�����
* ���������ucSockId:���ۺ�
* ���������
*                               ptCardPara������PSAM���ÿ�Ƭ����
*                               pdwPsamErrorCode����������ʱ�Ĵ�����

* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/
unsigned int bsp_psam_active (unsigned char ucSockId, T_CardPara *ptCardPara,unsigned int *pdwPsamErrorCode);
/**********************************************************************
* �������ƣ�bsp_psam_cmd
* ����������psam�������
* ���������ucSockId:���ۺ�
*                               ucCmdLen:�����
*                               pucCmdApdu��ָ������ָ��
* ���������
*                               pucResLen����Ƭ��Ӧ����
*                               pucResTpdu����Ƭ��Ӧ����ָ��
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_cmd(unsigned char ucSockId, unsigned char ucCmdLen, unsigned char *pucCmdApdu, unsigned char *pucResLen, unsigned char *pucResTpdu,unsigned int *pdwPsamErrorCode);
/**********************************************************************
* �������ƣ�bsp_psam_deactive
* ����������psamȥ�����
* ���������ucSockId:���ۺ�
* ���������
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_deactive(unsigned char ucSockId,unsigned int *pdwPsamErrorCode);
/**********************************************************************
* �������ƣ�bsp_psam_clkfreq_set
* �����������޸�PSAM��CLK�ķ�Ƶϵ��
* ���������ucClkDiv����Ƶϵ�� 
* ���������
*                               pdwPsamErrorCode����������ʱ�Ĵ�����
* �� �� ֵ    ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/	
unsigned int bsp_psam_clkfreq_set(unsigned char ucClkDiv);

/**********************************************************************
* �������ƣ�bsp_psam_voltage_set
* �����������޸�PSAM���Ĺ����ѹ
* ���������ucSockId:���ۺ�;��ѹֵ 
* �� �� ֵ��������
* ����˵������
* �޸�����		�汾��	�޸���		�޸�����
* ---------------------------------------------------------------------
************************************************************************/

unsigned int bsp_psam_voltage_set(unsigned char ucSockId,unsigned char ucVoltage);

#ifdef __cplusplus
}
#endif

#endif/* _BSP_DRV_PSAM_H_  */

