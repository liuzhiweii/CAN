/************************************************************************
* ��Ȩ���� (C)2015, �Ϻ����缯�ɵ�·�������ι�˾
* 
* �ļ����ƣ�
* �ļ���ʶ��
* ����ժҪ��lcd ����ͷ�ļ�
* ����˵����
* ��ǰ�汾��
* ��    �ߣ�       
* ������ڣ�
* 
************************************************************************/

#ifndef _BSP_DRV_LCD_H_
#define _BSP_DRV_LCD_H_

unsigned int bsp_lcd_init(void);
unsigned int bsp_lcd_show_string(unsigned char ucxpos, unsigned char ucypos, unsigned char *pstr);
unsigned int bsp_lcd_ctrl(unsigned char ucCtl);
void bsp_clear_oled_row(unsigned char row, unsigned char color);
#endif

