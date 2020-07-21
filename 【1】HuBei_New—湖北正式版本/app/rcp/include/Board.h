/************************************************************************
* ��Ȩ���� (C)2011, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� board.h
* �ļ���ʶ�� 
* ����ժҪ�� ��ͬ��Ƶ����õĺ궨���ļ�
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� zdx
* ������ڣ� 2010.12.26
* 
* �޸ļ�¼1�� RUP ����RFCB2����
*    �޸����ڣ�2011.12.1
*    �� �� �ţ�    RUP
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "uhfRfRegDef.h"
#include "RFCB.h"
#include "RFCB2.h"
#include "PURM.h"
#include "RFCB_BT.h"
#include "RFCB7.h"

#define MODULE_TYPE_DSB  (1)
#define MODULE_TYPE_PR   (3)

extern WORD32 g_dwUSPUhfVirTagWorkFlag;

#define FpgaRegWrite(wRegAddr,wData)   g_pvUSPDrvFpgaVirtStartAddr0[wRegAddr]=wData
#define FpgaRegRead(wRegAddr)          g_pvUSPDrvFpgaVirtStartAddr0[wRegAddr]


/* ȱʡ���ʣ�ʵ�ʹ��ʳ���100�Ժ��ֵ */
#define UHF_RF_MAX_FREQ_NUM                   (50)
#define UHF_RF_MIN_POWER                      (1500)

void uhfRf_initVswrAlarm(void);
void uhfRf_ReaderLKCLProc();
void uhfRf_SetForDataRate(WORD16 wForRate);

WORD32 uhfRf_BoardInit(void);

WORD32 usp_uhf_rf_setLO2Datt(WORD16 wCurPower);
WORD16 uhfRf_dealAlarmReport(WORD16 wAntIndex);
WORD32 uhfRf_SetLKCLSwitch(BYTE ucSwitch);
WORD32 uhfRf_SetRevLowNoEn(BYTE ucLowNoEn);
WORD32 uhfRf_IsAntProtected(void);
WORD32 uhfRf_ChooseRevFilter(WORD16 wLinkRate, BYTE ucSwitch);
WORD32 uhfRf_BoardReset(void);
WORD32 SetDATTtoZero(void);


#endif

