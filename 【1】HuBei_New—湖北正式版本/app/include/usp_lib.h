/************************************************************************
* ��Ȩ���� (C)2012, ����������ͨѶ�ɷ����޹�˾��
* 
* �ļ����ƣ� usp_lib.h
* �ļ���ʶ�� 
* ����ժҪ�� USP��̬��ͷ�ļ�������ʹ��USP��Ӧ�ð������ļ�����
* ����˵���� 
* ��ǰ�汾�� 
* ��    �ߣ� �߷�
* ������ڣ� 2012.02.06
* 
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ� 
* �޸ļ�¼2����
************************************************************************/
#ifndef _USP_LIB_H
#define _USP_LIB_H

#ifndef _PC_LINT
#ifdef __cplusplus
extern "C" {
#endif
#endif


/**************************************************************************
 *                       ���غ궨��                                        *
 **************************************************************************/
#define TRUE             (1)
#define FALSE            (0)

/**************************************************************************
 *                       �����ܵ������ļ�                                 *
 **************************************************************************/

/**************************************************************************
 *                       ��������ͷ�ļ�                                   *
 **************************************************************************/
#include "usp_lib_typeDef.h"
#include "usp_lib_msg.h"

/**************************************************************************
 *                       USP�������������������                          *
 **************************************************************************/
typedef struct tagT_USPLibHandle    *USPLibHandle;

/**************************************************************************
 *                       �������������ļ�                                 *
 **************************************************************************/

/**************************************************************************
 *                       USP����ϵͳ����ӿ�                              *
 **************************************************************************/
#include "usp_lib_vos.h"
#include "usp_lib_scp.h"
#//include "usp_lib_ltk.h"
//#include "usp_lib_cli.h"
//#include "usp_lib_psam.h"
//#include "usp_lib_timer.h"
//#include "usp_lib_comm.h"
//#include "usp_lib_cmp.h"
//#include "usp_lib_sysdata.h"
//#include "usp_lib_lap.h"
//#include "usp_lib_amp.h"
//#include "usp_lib_dmp.h"
//#include "usp_lib_mpi.h"
//#include "usp_lib_dsrc.h"
//#include "usp_lib_shell.h"
//#include "usp_lib_etc.h"
//#include "usp_lib_uhf.h"
//#include "usp_lib_psp.h"
//#include "usp_lib_vmp.h"
//#include "usp_lib_gpio.h"
//#include "usp_lib_misc.h"
//#include "usp_lib_led.h"
//#include "usp_lib_dsm.h"
/**************************************************************************
 *                       USP���������������                              *
 **************************************************************************/
typedef struct tagT_USPLibHandle
{
    WORD32              dwMagic;    /* У��ħ���֣�0x5A5A5A5A��*/
    #if 0
    USPLibScpHandle     scpHandle;  /* SCP������� */
    USPLibLtkHandle     ltkHandle;  /* LTK������� */
    USPLibCliHandle     cliHandle;  /* CLI������� */
    USPLibDmpHandle     dmpHandle;  /* DMP������� */
    USPLibPsamHandle    psamHandle; /* PSAM������� */
    USPLibTimerHandle   timerHandle;/* TIMER������� */
    USPLibCommHandle    commHandle; /* COMM������� */ 
    USPLibLapHandle     lapHandle;  /* LAP������� */ 
    USPLibAmpHandle     ampHandle;  /* AMP������� */ 
	USPLibCmpHandle     cmpHandle;  /* CMP������� */
    USPLibMpiHandle     mpiHandle;  /* MPI������� */
	USPLibDsrcHandle    dsrcHandle;
    USPLibUhfHandle     uhfHandle;
    USPLibPspHandle     pspHandle;
    USPLibVmpHandle     vmpHandle;
    USPLibGpioHandle    gpioHandle;
    USPLibMiscHandle    miscHandle;
    USPLibLedHandle     ledHandle;
    USPLibDsmHandle     dsmHandle;
    #endif
}T_USPLibHandle;

#ifndef _PC_LINT
#ifdef __cplusplus
}
#endif
#endif

#endif  /* _USP_LIB_H */

