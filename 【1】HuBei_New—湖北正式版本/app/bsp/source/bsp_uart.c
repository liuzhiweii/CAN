/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ� uart.c
* �ļ���ʶ�� 
* ����ժҪ���ṩ�Դ��ڷ��ʵĽӿ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� lg
* ������ڣ�2015.2.09
* 
************************************************************************/


/***********************************************************
 *                        ����ͷ�ļ�                       *
 ***********************************************************/
#include <termios.h>
#include "bsp.h"


/***********************************************************
 *                     ȫ�ֱ���                            *
***********************************************************/
/*****************************************************************************
* �������ƣ� bsp_uart_open
* �����������򿪴���
* �޸ı�����
* ��������� ucSerialNum : ���ڱ��
*                              dwMode : ���ڴ�ģʽ
* ��������� * pdwIndex : �򿪴��ڵ�����
* �� �� ֵ��    0 or ������
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/


unsigned int bsp_uart_open(unsigned char ucSerialNum, unsigned int dwMode, unsigned int * pdwIndex)
{
    unsigned int swIndex = 0;
    unsigned int dwRet = 0;
    /* ��ʱ����Ĭ������ */
    T_BSPDrvUartAttr tDefaultAttr;
    /* �жϴ��ڱ���Ƿ���ڴ������� */
    if (ucSerialNum >= 4)
    {
        return 1;
    }

    switch (ucSerialNum)
    {
        case 0:
        {
            swIndex = open("/dev/ttyS0", dwMode);
            break;
        }
        case 1:
        {
            swIndex = open("/dev/ttyS1", dwMode);
            break;
        }
        case 2:
        {
            unsigned short wData = 0;
            wData = READ_EPLD_REG(BSP_DRV_422_EN_OFFSET);
            wData |= BSP_DRV_422_EN_MASK;
            wData &=~BSP_DRV_422_DISEN_MASK;
            WRITE_EPLD_REG(BSP_DRV_422_EN_OFFSET, wData);

            wData = READ_EPLD_REG(BSP_DRV_UART_INTERRUP_OFFSET);
            wData |= BSP_DRV_UART_INTERRUP_MASK;
            WRITE_EPLD_REG(BSP_DRV_UART_INTERRUP_OFFSET, wData);
            swIndex = open("/dev/ttyS2", dwMode);
            break;
        }
        case 3:
        {
            swIndex = open("/dev/ttyS3", dwMode);
            break;
        }
        default:
        {
            return 2;
        }

    }

    if (swIndex < 0)
    {
        return 2;
    }
    *pdwIndex = swIndex;

    /* Ĭ������ֵ */
    tDefaultAttr.dwBaudRate = 115200;
    tDefaultAttr.ucDataBit = 8;
    tDefaultAttr.ucStopBit = 1;
    tDefaultAttr.ucParity = 0;
    tDefaultAttr.ucFlowControl = NO_FLOWCTRL;
    dwRet = bsp_uart_setAttr((*pdwIndex), &tDefaultAttr);
    if (0 != dwRet)
    {
        bsp_uart_close(*pdwIndex);
        return dwRet;
    }

    return 0;
}

/*****************************************************************************
* �������ƣ� bsp_uart_snd
* ���������� ���ڷ��ͺ���
* �޸ı�����
* ��������� dwIndex : ��������
*                              dwLen : ���������ݵĳ���
*                              pucSndBuf : �������ݻ�����
* ��������� * pdwSndLen : ʵ�ʷ��ͳ���
* �� �� ֵ��    0
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/
unsigned int bsp_uart_snd (unsigned int dwIndex, unsigned int dwLen, unsigned char * pucSndBuf, unsigned int * pdwSndLen)
{
     int sdwRet = 0;
    sdwRet = write(dwIndex, pucSndBuf, dwLen);
    if (sdwRet < 0)
    {
        return 3;
    }
    else
    {
        * pdwSndLen = (unsigned int)sdwRet;
    }

    return 0;
}


/*****************************************************************************
* �������ƣ� bsp_uart_rcv
* ���������� ���ڽ��պ���
* �޸ı�����
* ��������� dwIndex : ��������
*                              dwLen : �����������ݵĳ���
*                              pucRcvBuf : �������ݴ�ŵĻ�����
* ��������� * pdwRcvLen : ʵ�ʽ��ճ���
* �� �� ֵ��    0
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/
unsigned int bsp_uart_rcv (unsigned int dwIndex , unsigned int dwLen , unsigned char * pucRcvBuf, unsigned int *pdwRcvLen)
{
     int sdwRet = 0;

    /* linux��arm��Ĭ�Ϸ��������Ƿ��ģʽĬ��Ϊ������ */
    sdwRet = read(dwIndex, pucRcvBuf, dwLen);
    if (sdwRet < 0)
    {
        return 4;
    }
    else
    {
        * pdwRcvLen = (unsigned int)sdwRet;
    }
    return 0;

}

/*****************************************************************************
* �������ƣ� bsp_uart_setAttr
* ���������� �����������Ժ���
* �޸ı�����
* ��������� dwIndex : ��������
*                              ptUartAttr : ���Ա���ָ��
* ��������� 
* �� �� ֵ��    0 or ������
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/

unsigned int bsp_uart_setAttr (unsigned int dwIndex, T_BSPDrvUartAttr * ptUartAttr)
{
    int sdwRet = 0;
    struct termios tNewTio;
    bzero(&tNewTio, sizeof(tNewTio));
    cfmakeraw(&tNewTio);

    /* ���ò����� */
    switch (ptUartAttr->dwBaudRate)
    {
        case 1000000:
        {
            cfsetispeed(&tNewTio, B1000000);
            cfsetospeed(&tNewTio, B1000000);
            break;
        }
        case 921600:
        {
            cfsetispeed(&tNewTio, B921600);
            cfsetospeed(&tNewTio, B921600);
            break;
        }
        case 576000:
        {
            cfsetispeed(&tNewTio, B576000);
            cfsetospeed(&tNewTio, B576000);
            break;
        }
        case 115200:
        {
            cfsetispeed(&tNewTio, B115200);
            cfsetospeed(&tNewTio, B115200);
            break;
        }
        case 57600:
        {
            cfsetispeed(&tNewTio, B57600);
            cfsetospeed(&tNewTio, B57600);
            break;
        }
        case 38400:
        {
            cfsetispeed(&tNewTio, B38400);
            cfsetospeed(&tNewTio, B38400);
            break;
        }
        case 19200:
        {
            cfsetispeed(&tNewTio, B19200);
            cfsetospeed(&tNewTio, B19200);
            break;
        }
        case 9600:
        {
            cfsetispeed(&tNewTio, B9600);
            cfsetospeed(&tNewTio, B9600);
            break;
        }
        default:
        {
            return 5;
        }
    }
    /* ����������Ч */
    sdwRet = tcsetattr(dwIndex, TCSANOW, &tNewTio);
    if (sdwRet != 0)
    {
        perror("set_Baut tcgetattr error!");
        return 10;
    }
    /* ˢ�»����� */
    tcflush(dwIndex, TCIOFLUSH);
    sdwRet = tcgetattr( dwIndex, &tNewTio);
    if ( sdwRet != 0)
    {
        perror("set_parity tcgetattr error!");
        return 11;
    }
    /*  ����λ���� */
    switch (ptUartAttr->ucDataBit)
    {
        case 5:
        {
            tNewTio.c_cflag &= ~CSIZE;
            tNewTio.c_cflag |= CS5;
            break;
        }
        case 6:
        {
            tNewTio.c_cflag &= ~CSIZE;
            tNewTio.c_cflag |= CS6;
            break;
        }

        case 7:
        {
            tNewTio.c_cflag &= ~CSIZE;
            tNewTio.c_cflag |= CS7;
            break;
        }
        case 8:
        {
            tNewTio.c_cflag &= ~CSIZE;
            tNewTio.c_cflag |= CS8;
            break;
        }
        default:
        {
            return 6;
        }
    }
    /* ��żУ������ */
    switch (ptUartAttr->ucParity)
    {
        case 0:
        {
            tNewTio.c_cflag &= ~PARENB;
            tNewTio.c_iflag &= ~INPCK;
            break;
        }
        case 1:
        {
            tNewTio.c_iflag |= (INPCK | ISTRIP);
            tNewTio.c_cflag |= PARENB;
            tNewTio.c_cflag &= ~PARODD;
            break;
        }
        case 2:
        {
            tNewTio.c_cflag |= PARENB;
            tNewTio.c_cflag |= PARODD;
            tNewTio.c_iflag |= (INPCK | ISTRIP);
            break;
        }
        default:
        {
            return 7;
        }
    }
    /* ֹͣλ���� */
    switch (ptUartAttr->ucStopBit)
    {
        case 1:
        {
            tNewTio.c_cflag &= ~CSTOPB;
            break;
        }
        case 2:
        {
            tNewTio.c_cflag |= CSTOPB;
            break;
        }
        default:
        {
            return 8;
        }
    }

    tNewTio.c_oflag &= ~OPOST;
    tNewTio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tNewTio.c_cc[VTIME] = 0;
    tNewTio.c_cc[VMIN] = 1;
    tNewTio.c_cflag |= (CLOCAL | CREAD);
    tNewTio.c_iflag &= ~(BRKINT | ICRNL | IXON);


    /* ���ô������� */
    switch (ptUartAttr->ucFlowControl)
    {
        case NO_FLOWCTRL:
        {
            tNewTio.c_cflag &= ~CRTSCTS;
            tNewTio.c_iflag &= ~(IXON | IXOFF | IXANY);
            break;
        }
        case HARDWARE_FLOWCTRL:
        {
            tNewTio.c_cflag |= CRTSCTS;
            break;
        }
        case SOFTWARE_FLOWCTRL:
        {
            tNewTio.c_cflag |= (IXON | IXOFF | IXANY);
            break;
        }

        default:                                                                                                                                                                           /* ȱʡ���������� */
        {
            tNewTio.c_cflag &= ~CRTSCTS;
            tNewTio.c_iflag &= ~(IXON | IXOFF | IXANY);

            break;
        }
    }

    /* ����������Ч */
    sdwRet = tcsetattr(dwIndex, TCSANOW, &tNewTio);
    if (sdwRet != 0)
    {
        return 9;
    }
    /* ˢ�»����� */
    tcflush(dwIndex, TCIOFLUSH);
    return 0;

}

/**************************************************************************
* �������ƣ� bsp_uart_flush
* ����������ˢ�´��ڻ�����
* �޸ı�����
* ��������� dwIndex : ��������
*                              tMode : ˢ��ģʽ
* ��������� 
* �� �� ֵ��    0 or ������
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/

unsigned int bsp_uart_flush (unsigned int dwIndex, T_BSPDrvUartFlushMode tMode)
{
    int sdwRet = 0;
    sdwRet = tcflush(dwIndex, tMode);
    if (0 != sdwRet)
    {
        return 10;
    }

    return 0;
}

/**************************************************************************
* �������ƣ� bsp_uart_close
* �������������ڹرպ���
* �޸ı�����
* ��������� dwIndex : ��������
* ��������� 
* �� �� ֵ��    0 or ������
* ����˵����
* �޸�����   �汾��    �޸���  �޸�����
* ----------------------------------------------------
 
******************************************************/

unsigned int bsp_uart_close(unsigned int dwIndex)
{
    int swRet = 0;
        if (2 == dwIndex)
    {
        
        unsigned short wData = 0;
        wData = READ_EPLD_REG(BSP_DRV_422_EN_OFFSET);
        wData |= BSP_DRV_422_DISEN_MASK;
        wData &= ~BSP_DRV_422_EN_MASK;
        WRITE_EPLD_REG(BSP_DRV_422_EN_OFFSET, wData);
        
        wData = READ_EPLD_REG(BSP_DRV_UART_INTERRUP_OFFSET);
        wData &= ~BSP_DRV_UART_INTERRUP_MASK;
        WRITE_EPLD_REG(BSP_DRV_UART_INTERRUP_OFFSET, wData);

    }
    swRet = close(dwIndex);
    if (swRet < 0)
    {
        return 11;
    }

    return 0;

}


