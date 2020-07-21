/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� bsp_uart.h
* �ļ���ʶ�� 
* ����ժҪ�� ����ģ�����ӿ�ͷ�ļ�
* ����˵���� 
* ��ǰ�汾��
* ��    �ߣ� 
* ������ڣ� 
************************************************************************/

#ifndef _BSP_UART_H
#define _BSP_UART_H

/**************************************************************************
 *                          �궨��                                         *
 **************************************************************************/

/* ���ص�ȡֵ */
#define  NO_FLOWCTRL    0
#define  HARDWARE_FLOWCTRL    1
#define  SOFTWARE_FLOWCTRL     2

/* ��ģʽ���ֺ�linux�ں˵Ķ���һ�� */
#define UART_NONBLOCK   0x00000800
#define UART_NOCTTY     0x00000100
#define UART_RDONLY     0x00000000
#define UART_WRONLY     0x00000001
#define UART_RDWR       0x00000002

#define BSP_DRV_422_EN_OFFSET          (0xdc>>1)
#define BSP_DRV_422_EN_MASK            (0x02)
#define BSP_DRV_422_DISEN_MASK         (0x01)

#define BSP_DRV_UART_INTERRUP_OFFSET   (0x94>>1)
#define BSP_DRV_UART_INTERRUP_MASK     (0x02)


/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
/* �������� */
typedef struct tagT_BSPDrvUartAttr
{
	unsigned int         dwBaudRate;          /*������*/
	unsigned char            ucDataBit;           /*����λ5��6��7��8 */
	unsigned char            ucStopBit;           /*ֹͣλ1 ��2  */
	unsigned char            ucParity;            /*У��λ 0 =None ��1 = Even�� 2 =Odd*/
	unsigned char            ucFlowControl;       /*������*/
}T_BSPDrvUartAttr;


typedef enum tagT_BSPDrvUartFlushMode
{
	UART_INFLUSH = 0,
	UART_OUTFLUSH,
	UART_IOFLUSH
} T_BSPDrvUartFlushMode;

/**************************************************************************
 *                            ��������                                     *
 **************************************************************************/
unsigned int  bsp_uart_open(unsigned char ucSerialNum, unsigned int dwMode, unsigned int * pdwIndex);
unsigned int  bsp_uart_snd (unsigned int dwIndex, unsigned int dwLen, unsigned char * ucSndBuf, unsigned int * pdwSndLen);
unsigned int  bsp_uart_rcv (unsigned int dwIndex , unsigned int dwLen , unsigned char * ucRcvBuf, unsigned int *pdwRcvLen);
unsigned int  bsp_uart_close(unsigned int dwIndex);
unsigned int  bsp_uart_setAttr (unsigned int dwIndex, T_BSPDrvUartAttr * ptUartAttr);
unsigned int  bsp_uart_flush (unsigned int dwIndex, T_BSPDrvUartFlushMode tMode);

#endif
