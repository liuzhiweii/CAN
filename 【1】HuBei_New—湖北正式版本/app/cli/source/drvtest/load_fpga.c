#include <stdio.h>
#include "bsp.h"
#include "drvtest.h"

#define EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET      0x92   /*���ƼĴ���ƫ��*/
#define EPLD_FPGA_DOWNLOAD_DATA_REG_OFFSET     0x94   /*���ݼĴ���ƫ��*/
#define FPGA_DELAY_US                          16      /*��ʱ����*/
#define FPGA_CONFIG_DONE_TIME_OUT              1000     /*CONFIGDONE��ʱʱ��*/
#define EPLD_FPGA_RESET                        0xa2

#define NCONFIG_BIT_MASK                       0x1     /*config��������*/
#define NSTATUS_BIT_MASK                       0x2     /*nstatus��������*/
#define CONFDONE_BIT_MASK                      0x4     /*configdone��������*/
#define INITDONE_BIT_MASK                      0x8     /*initdone��������*/

#define PIN_HIGH                               0x1     /*����ߵ�ƽ*/
#define PIN_LOW                                0x0     /*����͵�ƽ*/

#define VER_LOAD_BUF_LEN                       0x200000  
#define FPGA_DOWNLOAD_START  		           0x10	   /* ����ָʾFPGA����ֶ����صĿ�ʼ��־ */
#define FPGA_DOWNLOAD_CLOSE    		           0x01	   /* ����ָʾFPGA����ֶ����صĽ�����־ */


#define WRITE_FPGA_DOWNLOAD_REG(OFFSET,BITOFFSET,DATA) ((DATA > 0) ? (p_epld_start_addr[OFFSET]|=BITOFFSET) : (p_epld_start_addr[OFFSET]&=~BITOFFSET))
#define READ_FPGA_DOWNLOAD_REG(OFFSET,BITOFFSET) (((p_epld_start_addr[OFFSET] & BITOFFSET) > 0) ? 0x1 : 0x0)

#define FPGA_DOWNLOAD_ERROR_CONFIG_LOW         0x1     /*config��������ʱ��������*/
#define FPGA_DOWNLOAD_ERROR_CONFIG_HIGH        0x2     /*config��������ʱ��������*/
#define FPGA_DOWNLOAD_ERROR_CONFIG_DONE        0x3     /*config_done��������ʱ��������*/



static void os_usDelay(unsigned int us)
{
    volatile unsigned int i = 0;
    for (i = 0;i < (FPGA_DELAY_US * us); i++);
    return;
}

static void initFpgaLoadAddr(void)
{
    unsigned short dwRet;

    dwRet = bsp_epld_init();
    if (0 != dwRet)
    {
        printf("init epld fail!\r\n");
        return dwRet;
    }
}

static unsigned int drv_fpga_download(unsigned char * buff, int len)
{
    unsigned int dwTimeOut = 0;
    unsigned int i = 0;
    
    printf("FPGA version download start\r\n");
    WRITE_FPGA_DOWNLOAD_REG(EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET,NCONFIG_BIT_MASK,PIN_LOW);

    #if 1
    dwTimeOut = FPGA_CONFIG_DONE_TIME_OUT;


    while ((READ_FPGA_DOWNLOAD_REG(EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET,CONFDONE_BIT_MASK) != 0)\ 
			    && (dwTimeOut > 0))
    {   
        //os_usDelay(1);
        dwTimeOut--;
    }
    if (dwTimeOut == 0)
    {
        printf("set config pin low error!\r\n");
	    return FPGA_DOWNLOAD_ERROR_CONFIG_LOW;
    }
    #endif
    os_usDelay(1);
    /* CONFIG д�ߵ�ƽ */
    WRITE_FPGA_DOWNLOAD_REG(EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET,NCONFIG_BIT_MASK,PIN_HIGH);

    #if 1
    /* ���ʱ��, STATUSҲ���ɸߵ�ƽ */
    dwTimeOut = FPGA_CONFIG_DONE_TIME_OUT;
    while ((READ_FPGA_DOWNLOAD_REG(EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET,NSTATUS_BIT_MASK) != 1) && (dwTimeOut > 0))
    {
        os_usDelay(1);
        dwTimeOut--;
    }
    if (dwTimeOut == 0)
    {
        printf("set config pin high error!\r\n");
	    return FPGA_DOWNLOAD_ERROR_CONFIG_HIGH;
    }
    #endif
    os_usDelay(5);
    /* ѭ����д����(�ֽ�)+������ʱ(1us) */
    for (i = 0;i < len;i++)
    {

        p_epld_start_addr[EPLD_FPGA_DOWNLOAD_DATA_REG_OFFSET] = buff[i];
        if (i % 10000 == 0)
        {
            printf(".");
        }
        os_usDelay(5);
    }
    //������������Ҫ�ڲ���2��clkʱ��
    p_epld_start_addr[EPLD_FPGA_DOWNLOAD_DATA_REG_OFFSET] = 0x0;
    printf("\r\nFPGA version download end\r\n");
    /* FPGA���ݱ����Դ�CRC����Ϣ, ���յ�����Ҫ�������Ժ�, CONFIG_DONE���žͻ��ɸߵ�ƽ */
    printf("CONFIG_DONE %x\r\n",p_epld_start_addr[EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET]);

    #if 0
    dwTimeOut = FPGA_CONFIG_DONE_TIME_OUT;
    while ((READ_FPGA_DOWNLOAD_REG(EPLD_FPGA_DOWNLOAD_CTL_REG_OFFSET,CONFDONE_BIT_MASK) != 1) && (dwTimeOut > 0))
    {
	    os_usDelay(2000);
        dwTimeOut--;
    }
    if (dwTimeOut == 0)
    {
        printf("CONFIG_DONE error when timeout!\r\n");
        return FPGA_DOWNLOAD_ERROR_CONFIG_DONE;
    }

    #endif

    return 0;
}

unsigned int drvtest_loadfpag(char *pFpgaFilePath)
{
    FILE *pf = NULL;
    unsigned char  aucfpgaBuf[VER_LOAD_BUF_LEN];
    unsigned int dwRet = 0;
	unsigned int iReadSize = 0;
	unsigned short tReadVale = 0;
	unsigned short tWriteValue = 0x55aa;
	unsigned short dwRegOffset = 0;
	T_FpgaData tFpgaTemp = 0;

    (void)initFpgaLoadAddr();

  
	//printf("drvtest_LoadFpag run...,file path=%s\r\n", pFpgaFilePath);
    pf = fopen((const char *)pFpgaFilePath,"rb");
    if(NULL == pf)
    {
        printf("Can't open FPGA Version file\r\n");
        return 0x1;
    }
    printf("FPGA Version file is successfully found!\n");
    iReadSize = fread(aucfpgaBuf,sizeof(unsigned char),VER_LOAD_BUF_LEN,pf);
    if(iReadSize < 0)
    {
        printf("Read Fpga File error\r\n");
        return 0x2;
    }
    printf("Read FPGA bin file successfully, ReadSize is %d \r\n", iReadSize);
    
    dwRet = drv_fpga_download(aucfpgaBuf,iReadSize);
    if (0 != dwRet)
    {
        printf("drv_fpga_download error!\r\n");
        return dwRet;
    }

    //bsp_epld_write(0x144, 0x00);
    os_usDelay(1000);
    //bsp_epld_write(0x144, 0x01);

    return 0;
}

