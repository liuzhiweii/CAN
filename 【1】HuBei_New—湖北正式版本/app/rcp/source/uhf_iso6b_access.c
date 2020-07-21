#include "usp.h"
#if (UHF_PROTOCOL_USE) &&(UHF_BOARD_TYPE !=BOARD_TYPE_RFCB2)
#include "usp_uhf_cfg.h"
#include "uhfFpgaRegDef.h"
#include "usp_uhf_iso6b.h"
#include "uhf.h"
#include "uhf_iso6b.h"
#include "usp_uhf_err.h"
#include "uhf_trace.h"
#include "uhf_dbg.h"
#include "FpgaCtl.h"
#include "uhf.h"
#include "ISO6B.h"

extern WORD16     g_awFpgaReadBuf[FPGA_BUF_BYTE_SIZE / 2];
extern T_ISO6BCtl g_tISO6BCtrl;
extern T_UhfCtrl  g_tUhfCtrl;

static T_ISO6BTagBuffer  s_tISO6BBuffer;      /*6B��ǩ�����ݻ���*/


static void uhf_ISO6BClearBuffer(void);
extern WORD32 uhf_SetInventRfPara(WORD16 wAntIndex, WORD16 wProtoType, WORD16 wForDataRate,
                           WORD16 wRcvDataRate, WORD16 wModDepth,
                           WORD16 wModType, T_USPUHFInventRfPara *ptRfPara);


/**********************************************************************
* �������ƣ� WORD32 uhf_ISO6B_Inventory_Start(T_Inventory *ptISO6BInventoryPara)
* ���������� ISO18000-6BЭ������������
* ��������� T_Inventory *ptISO6BInventoryPara: ����·�����
* ��������� ��
* �� �� ֵ��	 USP_SUCCESS  �ɹ�
								  ����: 		  ������
* ����˵����
* �޸�����		  �汾��	 �޸��� 		 �޸�����
* ------------------------------ -----------------
* 2010\11\26			V04.00.00		zdx 	  ����
**************************************************************************/
WORD32 uhf_ISO6B_Inventory_Start(T_USPUHFISO6BInventory *ptISO6BInventoryPara)
{
    WORD32 dwRetVal = USP_SUCCESS;
    T_USPUHFISO6BInventoryMask  *pISO6BInventoryMask = NULL;

    /*����һ�ν�fifo�е����ݶ���������һ֡�Ժ�wWord_HasRead�Լ���֡�Ĵӳ��ȡ�*/
    uhf_ISO6BClearBuffer();

    uhf_ISO6B_Cal_Mode_Para(ptISO6BInventoryPara->tISO6BInventoryPara.wModeIndex);

    uhf_ISO6BSetProtoPara(g_tISO6BCtrl.wFowardLinkDataRate, g_tISO6BCtrl.wReverseLinkDataRate,
                          g_tISO6BCtrl.wT1_value, g_tISO6BCtrl.wdelimeters, g_tISO6BCtrl.wModDepth);

    /* �˴�Ӧ��֧�ֶ��groupSelect����*/
    pISO6BInventoryMask = &ptISO6BInventoryPara->tISO6BInventoryPara.tISO6BInventoryMask;
    uhf_ISO6BSetInventoryPara(pISO6BInventoryMask->ucCommandType, pISO6BInventoryMask->ucAddress,
                              uhf_getInventQ(ptISO6BInventoryPara->tISO6BInventoryPara.wEstimateTag),
                              pISO6BInventoryMask->ucMask, g_tISO6BCtrl.wDataReadStartAddr,
                              pISO6BInventoryMask->aucMaskData);

    dwRetVal = uhf_SetInventRfPara(ptISO6BInventoryPara->dwAntIndex,
                                   UHF_PROTO_TYPE_ISO6B,
                                   g_tISO6BCtrl.wFowardLinkDataRate,
                                   g_tISO6BCtrl.wReverseLinkDataRate,
                                   g_tISO6BCtrl.wModDepth,
                                   Modulate_TYPE_DSB,
                                   &ptISO6BInventoryPara->tInventoryRfPara);

    if (USP_SUCCESS != dwRetVal)
    {
        return dwRetVal;
    }


    //TraceToMacMsg(EV_6B_Inventory);
    uhf_IsoStartProcAndTimer();
    return dwRetVal;
}
/**********************************************************************
* �������ƣ� WORD32 uhf_ISO6B_Inventory_Start(T_Inventory *ptISO6BInventoryPara)
* ���������� ISO18000-6BЭ��������Ƶ����
* ��������� ��
* ��������� ��
* �� �� ֵ��	 USP_SUCCESS  �ɹ�
								  ����: 		  ������
* ����˵����
* �޸�����		  �汾��	 �޸��� 		 �޸�����
* ------------------------------ -----------------
* 2010\11\26			V04.00.00		zdx 	  ����
**************************************************************************/
WORD32 uhf_ISO6B_Process_Start(WORD32 dwAntIndex)
{
    WORD32 dwRetVal = USP_SUCCESS;
    /* ����Э����� */
    uhf_ISO6BSetProtoPara(g_tISO6BCtrl.wFowardLinkDataRate, g_tISO6BCtrl.wReverseLinkDataRate,
                          g_tISO6BCtrl.wT1_value, g_tISO6BCtrl.wdelimeters, g_tISO6BCtrl.wModDepth);

    dwRetVal = uhf_SetInventRfPara(g_tUhfCtrl.wAntIndex,
                                   UHF_PROTO_TYPE_ISO6B,
                                   g_tISO6BCtrl.wFowardLinkDataRate,
                                   g_tISO6BCtrl.wReverseLinkDataRate,
                                   g_tISO6BCtrl.wModDepth,
                                   Modulate_TYPE_DSB,
                                   NULL);

    if (USP_SUCCESS != dwRetVal)
    {
        return dwRetVal;
    }

    return dwRetVal;
}


/**************************************************************************
* �������ƣ�uhf_ISO6BClearBuffer
* �������������ISO6B��ǩ����
* �����������
* �������: ��
* �� �� ֵ: ��
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 10/11/26    V04.00.00       zdx      ����
**************************************************************************/
static void uhf_ISO6BClearBuffer(void)
{
    s_tISO6BBuffer.wDataIndex = 0;
    s_tISO6BBuffer.wDataLen = 0;
}

/**************************************************************************
* �������ƣ�uhf_ISO6BInsertTag
* ��������������㵽��һ��EPC��ǩ���뵽������
* ���������WORD16 wRSSI         ��ǩ��RSSIֵ
                                  BYTE *pucUID           ��ǩUID��ͷָ��
* �������: ��
* �� �� ֵ:   USP_SUCCESS �ɹ�
                           ����:������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 10/11/26       V04.00.00         zdx            ����
**************************************************************************/
static WORD32  uhf_ISO6BInsertTag(WORD16 wRSSI, BYTE *pucUID)
{
    if (NULL == pucUID)
    {
        return USP_UHF_POINTER_NULL_ERROR;
    }

    if (s_tISO6BBuffer.wDataLen >= ISO6B_BUFFER_SIZE)
    {
        return USP_UHF_ISO6B_BUFFER_FULL;
    }

    s_tISO6BBuffer.tISO6BTags[s_tISO6BBuffer.wDataLen].wRSSI = wRSSI;
    memcpy(s_tISO6BBuffer.tISO6BTags[s_tISO6BBuffer.wDataLen].aucUID, pucUID, ISO_180006B_UID_BYTE_LEN);
    s_tISO6BBuffer.wDataLen++;

    return USP_SUCCESS;
}

/**************************************************************************
* �������ƣ�uhf_ISO6BRecordValidTag
* ������������У����ȷ�ı�ǩ��¼
* ���������WORD16 wRSSI         ��ǩ��RSSIֵ
                                  BYTE *pucUID           ��ǩUID��ͷָ��
* �������: ��
* �� �� ֵ:   USP_SUCCESS �ɹ�
                           ����:������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 10/12/31       V04.00.00         zdx            ����
**************************************************************************/
WORD32 uhf_ISO6BRecordValidTag(WORD16 wRSSI, BYTE *pucUID)
{
    WORD32 dwRet = USP_SUCCESS;

    if (g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tISO6BReadRssiCfg.ucTurnOnFilter == 1)
    {
        if (wRSSI < g_tUhfCtrl.atRfAntCfg[g_tUhfCtrl.wAntIndex].tISO6BReadRssiCfg.wRSSIThreshHold)
        {
            uhfDevPrint("Throw a tag!\r\n");
        }
        else
        {
            dwRet = uhf_ISO6BInsertTag(wRSSI, pucUID);
        }
    }
    else
    {
        dwRet = uhf_ISO6BInsertTag(wRSSI, pucUID);
    }

    return dwRet;
}
/**************************************************************************
* �������ƣ�uhf_ISO6BGetATag
* ������������ISO6B��ǩ�����л�ȡһ����ǩֵ
* ���������
* �������: WORD16 *pwRSSI         ��ǩ��RSSIֵ
                                  BYTE *pucUID           ��ǩUID��ͷָ��
* �� �� ֵ:   USP_SUCCESS �ɹ�
                           ����:������
* ����˵����
* �޸�����    �汾��     �޸���      �޸�����
* -----------------------------------------------
* 10/11/26       V04.00.00         zdx            ����
**************************************************************************/
WORD32 uhf_ISO6BGetATag(WORD16 *pwRSSI, BYTE *pucUID)
{
    if (NULL == pucUID || NULL == pwRSSI)
    {
        return USP_UHF_POINTER_NULL_ERROR;
    }

    if (0 == s_tISO6BBuffer.wDataLen)
    {
        return USP_UHF_ISO6B_BUFFER_EMPTY;
    }
    else
    {
        *pwRSSI = s_tISO6BBuffer.tISO6BTags[s_tISO6BBuffer.wDataIndex].wRSSI;
        memcpy(pucUID, s_tISO6BBuffer.tISO6BTags[s_tISO6BBuffer.wDataIndex].aucUID, ISO_180006B_UID_BYTE_LEN);
        s_tISO6BBuffer.wDataIndex++;
        s_tISO6BBuffer.wDataLen--;
    }

    return USP_SUCCESS;
}

/*****************************************************************************
* �������ƣ�uhf_ISO6B_Read_8_byte
* ������������ȡ��ʼλ����8���ֽڳ��ȵ�ISO��ǩ����
* �޸ı�����
* ���������WORD16 wStartAddr, BYTE ucLen, BYTE *aucRcvBuf
                               wStartAddr  ��ʼ��ȡ�ĵ�ַ
                               ucLen:��ȡ�ĳ��ȣ�����С��8�����ȡ8���ֽڣ�
                                         ֻ�ϱ���Ҫ����ֽ�
                               aucRcvBuf ��ȡ���ݴ�Ż���
* ���������
* �� �� ֵ�� USP_SUCCESS �ɹ�
                              ����:ʧ��
*****************************************************************************/
WORD32 uhf_ISO6B_Read_8_byte(WORD16 wStartAddr, BYTE ucLen, BYTE *aucRcvBuf)
{
    WORD32 dwRetVal = USP_SUCCESS;

    if (ucLen > 8 || NULL == aucRcvBuf || wStartAddr > ISO_18000_6B_MAX_TAG_SIZE - 8)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    /* ��д���ζ���ز���*/
    uhf_ISO6BSetRead8BytePara(wStartAddr);
    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_Read);

    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_READ_8BYTE, wStartAddr, ucLen,
                                          aucRcvBuf);

    return (dwRetVal);
}

/*****************************************************************************
* �������ƣ�uhf_ISO6B_Read_Variable_Byte
* ������������ȡ��ʼλ����8���ֽڳ��ȵ�ISO��ǩ���W
* �޸ı�����
* ���������WORD16 wStartAddr, BYTE ucLen, BYTE *aucRcvBuf
                               wStartAddr  ��ʼ��ȡ�ĵ�ַ
                               ucLen:��ȡ�ĳ��ȣ�����С��8�����ȡ8���ֽڣ�
                                         ֻ�ϱ���Ҫ����ֽ�
                               aucRcvBuf ��ȡ���ݴ�Ż���
* ���������
* �� �� ֵ�� USP_SUCCESS �ɹ�
                              ����:ʧ��
*****************************************************************************/

WORD32  uhf_ISO6B_Read_Variable_Byte(WORD16 wStartAddr, BYTE ucLen, BYTE *aucRcvBuf)
{
    WORD32 dwRetVal = USP_SUCCESS;
    WORD16 wFpgaReadLen = (ucLen == 0) ? ucLen : (ucLen - 1);

    uhfDevPrint("ISO6B Read Varible  start\r\n");

    if (NULL == aucRcvBuf)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    uhf_ISO6BSetReadVrblPara(wStartAddr, wFpgaReadLen);
    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_Read);
    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_READ_VRBLBYTE, wStartAddr, ucLen,
                                          aucRcvBuf);
    return (dwRetVal);
}
/*****************************************************************************
* �������ƣ�uhf_ISO6B_WriteByte
* �����������������ַд��һ���ֽ�����
* �޸ı�����
* ���������
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_WriteByte(WORD16 wStartAddr, BYTE ucMask, BYTE *pucWriteData)
{
    WORD32 dwRetVal = USP_SUCCESS;

    uhfDevPrint("Write Data %02x\r\n", *pucWriteData);
    uhf_ISO6BSetWriteBytePara(wStartAddr, pucWriteData);
    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_Write);

    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_WRITE_BYTE, 0, 0, 0);

    return dwRetVal;
}

/*****************************************************************************
* �������ƣ�uhf_ISO6B_Write_4_byt
* ������������ָ����ʼ��ַд���ĸ��ֽ�����
* �޸ı�����
* ���������aucRcvBuf ��ȡ���ݴ�Ż���
* ���������
* �� �� ֵ��
*****************************************************************************/

WORD32 uhf_ISO6B_Write_4_byte(WORD16 wStartAddr, BYTE ucMask, BYTE *pucWriteData)
{
    WORD32 dwRetVal = USP_SUCCESS;

    uhfDevPrint("Write Data %02x%02x%02x%02x\r\n",
                pucWriteData[0], pucWriteData[1], pucWriteData[2], pucWriteData[3]);
    uhf_ISO6BSetWrite4BytePara(wStartAddr, pucWriteData);

    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_Write);

    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_WRITE_4BYTE, 0, 0, 0);

    return (dwRetVal);
}

/*****************************************************************************
* �������ƣ�WORD32 uhf_ISO6B_LockByte(WORD16 wStartAddr,BYTE wLen,BYTE *pucState)
* ����������ISO6B��������
* �޸ı�����
* ���������WORD16 wStartAddr   ������ǩ��ַ
                                 BYTE wLen                  �������ȣ�û�ã�Ϊ�˽������޸�ΪRetryFun�ĸ�ʽ
                                 BYTE *pucState          ���ؽ����û�ã�Ϊ�˽������޸�ΪRetryFun�ĸ�ʽ
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_LockByte(WORD16 wStartAddr, BYTE wLen,
                          /*û��*/BYTE *pucState/*û��*/)
{
    WORD32 dwRetVal = USP_SUCCESS;

    uhfDevPrint("UHF: uhf_ISO6B_LockByte : Lock Addr  %d Byte\r\n", wStartAddr);

    uhf_ISO6BSetLockBytePara(wStartAddr);
    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_Lock);

    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_LOCK, 0, 0, 0);

    return dwRetVal;
}


/*****************************************************************************
* �������ƣ�uhf_ISO6B_LockQueryByte
* ������������ȡλ�ñ�ǩ��״̬
* �޸ı�����
* ���������
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_LockQueryByte(WORD16 wStartAddr, BYTE ucLen, BYTE *pucLockStat)
{
    WORD32 dwRetVal = USP_SUCCESS;

    uhf_ISO6BSetLockQueryPara(wStartAddr);
    uhf_IsoStartProcAndTimer();

    //TraceToMacMsg(EV_6B_LockQuery);

    dwRetVal = uhf_ISO6B_Proc_OprtCmd_Rsp(ISO6B_CMD_TYPE_LOCKQRY, 0, 0, pucLockStat);

    return (dwRetVal);
}


/**********************************************************************
* �������ƣ�WORD32 uhf_ISO6B_RetryFuc(WORD16 wRetryTimes, WORD16 wStartAddr,
                                                              BYTE ucLen,BYTE *pucData, ISORetry pfRetryFun)
* ��������������retrytimeѭ���������뺯��
* ���������wRetryTimes:���Դ���
                                 wStartAddr  :��ʼ�����ĵ�ַ��
                                 ucLen :�����ĳ���
                                 pfRetryFun:���Ե��õĺ���
* �����������
* �� �� ֵ��    WORD32 : USP_SUCCESS �ɹ�
                                 ����:������
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/26      V04.00.00       zdx         ����
************************************************************************/
static WORD32 uhf_ISO6B_RetryFuc(WORD16 wRetryTimes, WORD16 wStartAddr,
                                 BYTE ucLen, BYTE *pucData, ISORetry pfRetryFun)
{
    WORD16 wTryNum = 0;
    WORD32 dwRet = USP_SUCCESS;

    for (wTryNum = 0; wTryNum <= wRetryTimes ; wTryNum++)
    {
        dwRet = pfRetryFun(wStartAddr, ucLen, pucData);

        /*����ɹ����߷���FPGA��ʱʱ�˳��������̣�ֱ�ӷ���*/
        if (USP_SUCCESS == dwRet || USP_UHF_FPGA_NO_FRAME == dwRet)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    return dwRet;
}


WORD16 ComputeWriteSuccessLen(BYTE ucMask)
{
    WORD16 wSuccessLen = 0;

    while ((ucMask & 0x1) == 0)
    {
        wSuccessLen++;
        ucMask >>= 1;
    }

    return wSuccessLen;
}
/*****************************************************************************
* �������ƣ�uhf_ISO6B_WriteData
* ������������������ʼ��ַд�����ⳤ�ȵ�����
* �޸ı�����
* ���������
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_WriteData(T_USPUHFISO6BWrite *ptISO6BWritePara,
                           T_USPUHFISO6BWriteRes *ptISO6BWriteRes)
{
    WORD32 dwRetVal = USP_SUCCESS;
    WORD16 wBeginByte = 0;					 /* white4byte ������ʼ�ֽ�*/
    WORD16 wBeforeLen;					 /* write4byte ֮ǰд��ĵ��ֽ���*/
    WORD16 wResidue;						   /* ����4�ֽ����� */
    WORD16 wParaNum;						   /* 4�ֽ�Ϊһ�εĶ��� */
    WORD16 wRetryTimes;
    WORD16 wCommandType;
    BYTE   ucByteWriteMask;
    WORD16  wSingleWriteLen = 0;
    BYTE    *pucWriteDate = NULL;
    BYTE    *pucUID = NULL;
    WORD16  wWriteLen = 0;
    WORD16  wStartAddr = 0;
    WORD16  wVerifyTimes = 0;
    WORD16  wIndex = 0;
    WORD16  wSuccessLen = 0;

    ptISO6BWriteRes->wResultLength = 0; /*��ʼ��д�ɹ��ĳ���Ϊ0*/

    if ((WORD16)ptISO6BWritePara->ucBytePtr + (WORD16)ptISO6BWritePara->ucByteCount > 256)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    wVerifyTimes = ptISO6BWritePara->tISO6BWriteCtl.ucVerifyTimes;
    wStartAddr = ptISO6BWritePara->ucBytePtr;
    wRetryTimes = ptISO6BWritePara->tISO6BWriteCtl.ucRetryTimes;
    wWriteLen = ptISO6BWritePara->ucByteCount;
    wCommandType = (wWriteLen < 4) ? ISO_6B_WRITEBYTE : ptISO6BWritePara->tISO6BWriteCtl.ucCommandType;
    pucWriteDate = ptISO6BWritePara->aucWriteData;
    pucUID = ptISO6BWritePara->tTagId.aucTagData;

    uhf_ISO6BSetUIDPara(pucUID);

    uhfDevPrint("6BWrite start %d length %d CommandType %d\r\n", wStartAddr, wWriteLen, wCommandType);
    uhfDevPrint("Retry TImes %d verity time %dWrite Data", wRetryTimes, wVerifyTimes);

    for (wIndex = 0; wIndex < wWriteLen; wIndex++)
    {
        uhfDevPrint("%02x ", pucWriteDate[wIndex]);
    }

    uhfDevPrint("\r\n");

    if (wCommandType == ISO_6B_WRITE4BYTE)
    {
        if (wStartAddr % 4 == 0)
        {
            wBeginByte = wStartAddr;
            wBeforeLen = 0;
        }
        else
        {
            wBeginByte = ((wStartAddr / 4) + 1) * 4;	/* Addr begin to execute write4byte */
            wBeforeLen = wBeginByte - wStartAddr;	 /* write byteslen before write4byte */
        }

        /* д��white4byte ����ǰ���ֽ�*/
        if (wBeforeLen > 0)
        {
            /*û��д��ɹ�*/
            ucByteWriteMask = 0xff;
            dwRetVal = uhf_ISO6B_8Byte_Verify_Write(wStartAddr, wBeforeLen,
                                                    &wSuccessLen, pucWriteDate, ucByteWriteMask,
                                                    wRetryTimes, wVerifyTimes);
            ptISO6BWriteRes->wResultLength += wSuccessLen;   /*��¼д��ɹ���byte ��*/

            if (dwRetVal != USP_SUCCESS)
            {
                uhfDevPrint("UHF : uhf_ISO6B_Write_byte failed %d times at addr %d\r\n", wRetryTimes, wStartAddr);
                return dwRetVal;
            }

            pucWriteDate += wBeforeLen;
        }

        /* write4byte ����ִ��*/
        wParaNum = (wWriteLen - wBeforeLen) / 4;
        wResidue = (wWriteLen - wBeforeLen) - (wParaNum * 4);

        if (wParaNum > 0)
        {
            uhfDevPrint("UHF : Writebyte len wParaNum %d\r\n", wParaNum);

            for (wIndex = 0; wIndex < wParaNum; wIndex += wSingleWriteLen)
            {
                ucByteWriteMask = 0xff;
                wSingleWriteLen = ((wParaNum - wIndex) < 2) ? 1 : 2;

                dwRetVal = uhf_ISO6B_4X2Byte_Verify_Write(wBeginByte + (wIndex * 4), wSingleWriteLen,
                           &wSuccessLen, pucWriteDate,
                           ucByteWriteMask, wRetryTimes,
                           wVerifyTimes);
                ptISO6BWriteRes->wResultLength += wSuccessLen * 4;   /*��¼д��ɹ���byte ��*/

                if (dwRetVal != USP_SUCCESS)
                {
                    uhfDevPrint("UHF : uhf_ISO6B_Write_byte failed %d times at addr %d\r\n", wRetryTimes, wStartAddr);
                    return dwRetVal;
                }

                pucWriteDate += wSingleWriteLen * 4;
                uhfDevPrint("UHF: uhf_ISO6B_WriteData: Write 4 bytes at %d Addr successfully !\r\n", (wBeginByte + (wIndex * 4)));

            }
        }

        /* д��white4byte �������ֽ�*/
        if (wResidue > 0)
        {
            uhfDevPrint("UHF : Writebyte len wResidue %d\r\n", wResidue);
            ucByteWriteMask = 0xff;
            dwRetVal = uhf_ISO6B_8Byte_Verify_Write((wBeginByte + wParaNum * 4), wResidue,
                                                    &wSuccessLen, pucWriteDate, ucByteWriteMask,
                                                    wRetryTimes, wVerifyTimes);
            ptISO6BWriteRes->wResultLength += wSuccessLen;   /*��¼д��ɹ���byte ��*/

            if (dwRetVal != USP_SUCCESS)
            {
                uhfDevPrint("UHF : uhf_ISO6B_Write_byte failed %d times at addr %d\r\n", wRetryTimes, wStartAddr);
                return dwRetVal;
            }
        }

    }
    else
    {
        for (wIndex = 0; wIndex < wWriteLen; wIndex += wSingleWriteLen)
        {
            ucByteWriteMask = 0xff;
            wSingleWriteLen = ((wWriteLen - wIndex) < 8) ? (wWriteLen - wIndex) : 8;

            dwRetVal = uhf_ISO6B_8Byte_Verify_Write(wStartAddr + wIndex, wSingleWriteLen,
                                                    &wSuccessLen, pucWriteDate + wIndex, ucByteWriteMask,
                                                    wRetryTimes, wVerifyTimes);
            ptISO6BWriteRes->wResultLength += wSuccessLen;   /*��¼д��ɹ���byte ��*/

            if (dwRetVal != USP_SUCCESS)
            {
                uhfDevPrint("UHF : uhf_ISO6B_Write_byte failed %d times at addr %d\r\n", wRetryTimes, wStartAddr + wIndex);
                return dwRetVal;
            }
        }
    }

    return dwRetVal;
}

/**********************************************************************
* �������ƣ�WORD32 uhf_ISO6B_ReadData(T_ISO6BRead *ptISO6BReadPara,
                                                                    T_ISO180006B_ReadResponse *ptISO6BReadRes)
* ������������������Ķ���������ȡ6B��ǩ������
* ���������ptISO6BReadPara :�������
                                 ptISO6BReadRes :6B��ǩ�Ķ�ȡ���
* �����������
* �� �� ֵ��    WORD32 : USP_SUCCESS �ɹ�
                                 ����:������
* ����˵������
* �޸�����      �汾��  �޸���      �޸�����
* ---------------------------------------------------------------------
* 2010/11/26      V04.00.00       zdx        �޸������������
************************************************************************/
#define ISO_READ_8BYTE_COMMAND    (0)
#define ISO_READ_VARIABLE_COMMAND (1)
WORD32 uhf_ISO6B_ReadData(T_USPUHFISO6BRead *ptISO6BReadPara,
                          T_USPUHFISO6BReadRes *ptISO6BReadRes)
{
    WORD16 wCurrentAddr = 0;                    /* ��ǰҪ��ISO�����ʼ��ַ */
    BYTE       aucReadBuf[8] = {0};            /* ���8�ֽ����ݵĻ���*/
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wRetryTimes = 0;
    WORD32 wHasNotReadLen = 0;
    WORD16 wSingleReadLen = 0;
    WORD16 wHasReadLen = 0;
    WORD16 wReadEndAddr = 0;
    WORD16 wCommandType = 0;
    WORD16 wISOReadLen = 0;
    BYTE   *pucUID = NULL;

    if ((WORD16)ptISO6BReadPara->ucBytePtr + (WORD16)ptISO6BReadPara->ucByteCount > 256)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    wRetryTimes = ptISO6BReadPara->tISO6BReadctl.ucRetryTimes;
    wCurrentAddr = ptISO6BReadPara->ucBytePtr;
    wReadEndAddr = ptISO6BReadPara->ucBytePtr + ptISO6BReadPara->ucByteCount;
    wCommandType = ptISO6BReadPara->tISO6BReadctl.ucCommandType;
    wISOReadLen  = ptISO6BReadPara->ucByteCount;

    /*ָ��д��ı�ǩ*/
    pucUID = ptISO6BReadPara->tTagId.aucTagData;
    uhf_ISO6BSetUIDPara(pucUID);

    if (wCommandType == ISO_READ_VARIABLE_COMMAND)
    {

        dwRet = uhf_ISO6B_RetryFuc(wRetryTimes, wCurrentAddr, wISOReadLen, ptISO6BReadRes->aucReadData, uhf_ISO6B_Read_Variable_Byte);

        if (USP_SUCCESS  !=  dwRet)
        {
            uhfDevPrint("ISO6B Read Varible Failed %d\r\n", dwRet);
            ptISO6BReadRes->wResultLength = 0;
            return  dwRet;
        }
    }
    else
    {
        for (; wCurrentAddr < wReadEndAddr; wCurrentAddr += ISO6B_READLENGTH)
        {
            wHasReadLen = wCurrentAddr - ptISO6BReadPara->ucBytePtr;
            wHasNotReadLen = ptISO6BReadPara->ucByteCount - wHasReadLen;
            wSingleReadLen = (wHasNotReadLen > ISO6B_READLENGTH) ? ISO6B_READLENGTH : wHasNotReadLen;
            dwRet = uhf_ISO6B_RetryFuc(wRetryTimes, wCurrentAddr,
                                       wSingleReadLen, aucReadBuf,
                                       uhf_ISO6B_Read_8_byte);

            if (USP_SUCCESS  ==  dwRet)
            {
                memcpy(&ptISO6BReadRes->aucReadData[wHasReadLen], aucReadBuf, wSingleReadLen);
            }
            else
            {
                ptISO6BReadRes->wResultLength = wHasReadLen;
                return  dwRet;
            }
        }
    }

    ptISO6BReadRes->wResultLength = ptISO6BReadPara->ucByteCount;
    return USP_SUCCESS;
}

/*****************************************************************************
* �������ƣ�uhf_ISO6B_LockData
* ������������������ʼ��ַ��ס���ⳤ�ȵ�����
* �޸ı�����
* ���������
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_LockData(T_USPUHFISO6BLock *ptISO6BLockPara,
                          T_USPUHFISO6BLockRes *ptISO6BLockRes)
{
    WORD32 dwRetVal = USP_SUCCESS;
    BYTE       ucLockState = 0;
    WORD16 wRetryTimes = 0;
    WORD16 wVerifyTimes = 0;
    BYTE *pucUID = NULL;
    WORD16 wIndex = 0;
    WORD16 wEndAddr = 0;

    if (NULL == ptISO6BLockPara || NULL == ptISO6BLockRes)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    if ((WORD16)ptISO6BLockPara->ucBytePtr + (WORD16)ptISO6BLockPara->ucByteCount > 256)
    {
        return USP_UHF_RCVED_PARA_ERROR;
    }

    wVerifyTimes = ptISO6BLockPara->tISO6BLockCtl.ucVerifyTimes;
    uhfDevPrint("6BLock start Addr %d Length %d Retry Times %d\r\n",
                ptISO6BLockPara->ucBytePtr, ptISO6BLockPara->ucByteCount,
                wVerifyTimes);
    wRetryTimes = ptISO6BLockPara->tISO6BLockCtl.ucRetryTimes;
    pucUID = ptISO6BLockPara->tTagId.aucTagData;
    wEndAddr = ptISO6BLockPara->ucBytePtr + ptISO6BLockPara->ucByteCount;
    uhf_ISO6BSetUIDPara(pucUID);

    /* �Ȱ���������ѯ�����locakable ��־λ��λ*/
    for (wIndex = ptISO6BLockPara->ucBytePtr; wIndex < wEndAddr; wIndex++)
    {
        dwRetVal = uhf_ISO6B_RetryFuc(wRetryTimes, wIndex, 1,
                                      &ucLockState, uhf_ISO6B_LockQueryByte);

        if (dwRetVal != USP_SUCCESS)
        {
            UHFErrorPrint("UHF : uhf_ISO6B_Lock one byte Lock Query Failed at addr %d\r\n", wIndex);
        }
        else
        {
            if (ucLockState == ISO6B_BYTE_LOCKED)
            {
                uhfDevPrint("BYTE %d has been LOCKED\r\n", wIndex);
            }
            else
            {
                dwRetVal = uhf_ISO6B_LockByteVerify(wIndex, wRetryTimes, wVerifyTimes);

                if (dwRetVal != USP_SUCCESS)
                {
                    uhfDevPrint("BYTE  %d  LOCK or Lock Verify Failed\r\n", wIndex);
                }
            }
        }

        ptISO6BLockRes->aucLockData[wIndex - ptISO6BLockPara->ucBytePtr] = dwRetVal;

        if (dwRetVal > USP_UHF_ERROR_NEED_RESET_FGPA)
        {
            /*������������FPGA ״̬����Ҫ��λ*/
            return dwRetVal;
        }

    }

    return dwRetVal;
}
/*****************************************************************************
* �������ƣ�uhf_ISO6B_LockQueryData
* ������������������ʼ��ַ��ѯ�����ⳤ�ȵ�����
* �޸ı�����
* ���������
* ���������
* �� �� ֵ��
*****************************************************************************/
WORD32 uhf_ISO6B_LockQueryData(T_USPUHFISO6BLockQuery *ptISO6BLockQueryPara,
                               T_USPUHFISO6BLockQueryRes *ptISO6BLockQueryRes)
{
    WORD32  dwRetVal = USP_SUCCESS;
    BYTE        ucLockStat = 0;
    WORD16  wRetryTimes = 0;
    WORD16  wCurLockQueryAddr = 0;
    WORD16  wLockQueryEndAddr = 0;
    WORD16  wHasLockQueryedLen = 0;
    BYTE        *paucUID = NULL;
    //BYTE     aucReadBuffer[8];

    uhfDevPrint("Start Addr %d LockQuery Byte Len %d\r\n",
                ptISO6BLockQueryPara->ucBytePtr, ptISO6BLockQueryPara->ucByteCount);

    if ((WORD16)ptISO6BLockQueryPara->ucBytePtr + (WORD16)ptISO6BLockQueryPara->ucByteCount > 256)
    {
        uhfDevPrint("LockQuery Para error\r\n");
        return USP_UHF_RCVED_PARA_ERROR;
    }

    wCurLockQueryAddr = ptISO6BLockQueryPara->ucBytePtr;
    wRetryTimes = ptISO6BLockQueryPara->tISO6BLockQueryCtl.ucRetryTimes;
    paucUID = ptISO6BLockQueryPara->tTagId.aucTagData;
    wCurLockQueryAddr = ptISO6BLockQueryPara->ucBytePtr;
    wLockQueryEndAddr = ptISO6BLockQueryPara->ucBytePtr + ptISO6BLockQueryPara->ucByteCount;

    uhf_ISO6BSetUIDPara(paucUID);

    for (; wCurLockQueryAddr < wLockQueryEndAddr; wCurLockQueryAddr++)
    {
        wHasLockQueryedLen = wCurLockQueryAddr - ptISO6BLockQueryPara->ucBytePtr;
        dwRetVal =  uhf_ISO6B_RetryFuc(wRetryTimes, wCurLockQueryAddr, 1, &ucLockStat, uhf_ISO6B_LockQueryByte);

        if (dwRetVal == USP_SUCCESS)
        {
            ptISO6BLockQueryRes->aucLockQueryData[wHasLockQueryedLen] = ucLockStat;
            uhfDevPrint("Addr %d Lock State %d\r\n", wCurLockQueryAddr, ucLockStat);

        }
        else
        {
            if (dwRetVal > USP_UHF_ERROR_NEED_RESET_FGPA)
            {
                /*������������FPGA ״̬����Ҫ��λ*/
                memset(&ptISO6BLockQueryRes->aucLockQueryData[wHasLockQueryedLen], dwRetVal, wLockQueryEndAddr - wCurLockQueryAddr);
                return  dwRetVal;

            }
            else
            {
                ptISO6BLockQueryRes->aucLockQueryData[wHasLockQueryedLen] = dwRetVal;
                uhfDevPrint("LockQuery Addr %d ErrorCode %x\r\n", wCurLockQueryAddr, dwRetVal);
            }

        }
    }

    return dwRetVal;
}

WORD32 uhf_ISO6B_8Byte_Verify_Write(WORD16 wStartAddr, WORD16 wLen,
                                    WORD16 *pwWriteSuccessLen,
                                    BYTE *pucWriteData, BYTE ucByteWriteMask,
                                    WORD16 wRetryTimes, WORD16 wVerifyTimes)
{
    WORD16 wVerifyIndex = 0;
    WORD32 dwRetVal = USP_SUCCESS;
    *pwWriteSuccessLen = 0;

    do
    {
        uhfDevPrint("UHF : Writebyte len wBeforeLen %d\r\n", wLen);
        dwRetVal = uhf_ISO6B_Write_8_Byte(wStartAddr, wLen, pwWriteSuccessLen,
                                          pucWriteData, ucByteWriteMask, wRetryTimes);

        if (USP_SUCCESS == dwRetVal)
        {
            if (wVerifyTimes == 0)
            {
                break;
            }

            dwRetVal = uhf_ISO6B_Write_8_ByteVerify(wStartAddr, wLen, pucWriteData,
                                                    &ucByteWriteMask, wRetryTimes);

            if (dwRetVal == USP_SUCCESS)
            {

                if (ucByteWriteMask == 0)
                {
                    *pwWriteSuccessLen = wLen;
                    break;
                }
                else
                {
                    *pwWriteSuccessLen = ComputeWriteSuccessLen(ucByteWriteMask);
                    uhfDevPrint("UHF : uhf_ISO6B_Write_byte success but verify error!\r\n");
                    dwRetVal = USP_UHF_ISO6B_RESULT_VERIFY_ERROR;
                    continue;
                }
            }
            else
            {
                *pwWriteSuccessLen = 0;
                break;
            }

        }
        else
        {
            if (wVerifyTimes != 0)
            {
                *pwWriteSuccessLen = 0;
            }

            break;
        }
    }
    while ((++wVerifyIndex) < wVerifyTimes);

    return dwRetVal;
}


WORD32 uhf_ISO6B_4X2Byte_Verify_Write(WORD16 wStartAddr, WORD16 wLen,
                                      WORD16 *pwWriteSuccessLen,
                                      BYTE *pucWriteData, BYTE ucByteWriteMask,
                                      WORD16 wRetryTimes, WORD16 wVerifyTimes)
{
    WORD16 wVerifyIndex = 0;
    WORD32 dwRetVal = USP_SUCCESS;
    *pwWriteSuccessLen = 0;

    do
    {
        uhfDevPrint("UHF : Writebyte len wBeforeLen %d\r\n", wLen);
        dwRetVal = uhf_ISO6B_Write_4x2_Byte(wStartAddr, wLen, pwWriteSuccessLen,
                                            pucWriteData, ucByteWriteMask, wRetryTimes);

        if (USP_SUCCESS == dwRetVal)
        {
            if (wVerifyTimes == 0)
            {
                break;
            }

            dwRetVal = uhf_ISO6B_Write_4x2_ByteVerify(wStartAddr, wLen, pucWriteData,
                       &ucByteWriteMask, wRetryTimes);

            if (dwRetVal == USP_SUCCESS)
            {

                if (ucByteWriteMask == 0)
                {
                    *pwWriteSuccessLen = wLen;
                    break;
                }
                else
                {
                    *pwWriteSuccessLen = ComputeWriteSuccessLen(ucByteWriteMask);
                    uhfDevPrint("UHF : uhf_ISO6B_Write_byte success but verify error!\r\n");
                    dwRetVal = USP_UHF_ISO6B_RESULT_VERIFY_ERROR;
                    continue;
                }
            }
            else
            {
                *pwWriteSuccessLen = 0;
                break;
            }

        }
        else
        {
            /*��Ϊ0���������֤û�з��ؽ�������ó���Ϊ0*/
            if (wVerifyTimes != 0)
            {
                *pwWriteSuccessLen = 0;
            }

            break;
        }
    }
    while ((++wVerifyIndex) < wVerifyTimes);

    return dwRetVal;
}

WORD32 uhf_ISO6B_Write_8_Byte(WORD16 wStartAddr, WORD16 wLength,
                              WORD16 *pwWriteSuccessLen,
                              BYTE *pucWriteData, BYTE ucByteWriteMask,
                              WORD16 wRetryTimes)
{
    WORD16 wWriteIndex = 0;
    WORD32 dwRetVal = USP_SUCCESS;

    uhfDevPrint("enter uhf_ISO6B_Write_8_Byte fuction wLength %d ucByteWriteMask %d\r\n", wLength, ucByteWriteMask);

    for (wWriteIndex = 0; wWriteIndex < wLength; wWriteIndex++)
    {
        uhfDevPrint("wWriteIndex %d\r\n", wWriteIndex);

        if ((ucByteWriteMask & (1 << wWriteIndex)) == 0)
        {
            /*�����ǰByte�Ѿ�д��ɹ���������д�����*/
            uhfDevPrint("the location byte has been write successfully! no need to rewrite again!\r\n");
            continue;
        }

        dwRetVal = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr + wWriteIndex, 1, &pucWriteData[wWriteIndex], uhf_ISO6B_WriteByte);

        if (USP_SUCCESS == dwRetVal)
        {
            uhfDevPrint("UHF: uhf_ISO6B_WriteData: Write one byte at %d Addr successfully \r\n", (wStartAddr + wWriteIndex));
        }
        else
        {
            uhfDevPrint("UHF: uhf_ISO6B_WriteData: Write one byte at %d Addr failed \r\n", (wStartAddr + wWriteIndex));
            break;
        }
    }

    *pwWriteSuccessLen = wWriteIndex;
    return dwRetVal;
}

WORD32 uhf_ISO6B_Write_8_ByteVerify(WORD16 wStartAddr, WORD16 wLength,
                                    BYTE *pucWriteData, BYTE *pByteWriteMask,
                                    WORD16 wRetryTimes)
{
    BYTE ucReadData[8];
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wReadIndex = 0;

    /*��֤��������Ϊ0ʱֱ�ӷ��سɹ�*/
    if (0 == wRetryTimes)
    {
        *pByteWriteMask = 0;
        return  USP_SUCCESS;
    }

    uhfDevPrint("enter uhf_ISO6B_Write_8_ByteVerify fuction wLength %d\r\n", wLength);
    dwRet = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr, 1, ucReadData, uhf_ISO6B_Read_8_byte);

    if (USP_SUCCESS == dwRet)
    {
        uhfDevPrint("uhf_ISO6B_Write_4x2_ByteVerify read success\r\n");
        *pByteWriteMask = 0;

        uhfDevPrint("uhf_ISO6B_Write_8_ByteVerify Read Data0x");

        for (wReadIndex = 0; wReadIndex < wLength; wReadIndex++)
        {
            uhfDevPrint(" %02x", ucReadData[wReadIndex]);

            if (ucReadData[wReadIndex] != pucWriteData[wReadIndex])
            {
                *pByteWriteMask |= (1 << wReadIndex);
            }
        }

        uhfDevPrint("\r\n");
    }
    else
    {
        uhfDevPrint("uhf_ISO6B_Write_4x2_ByteVerify read failed\r\n");
    }

    return dwRet;
}
WORD32 uhf_ISO6B_Write_4x2_Byte(WORD16 wStartAddr, WORD16 wLength,
                                WORD16 *pwWriteSuccessLen,
                                BYTE *pucWriteData, BYTE ucByteWriteMask,
                                WORD16 wRetryTimes)
{

    WORD16 wWriteIndex = 0;
    WORD32 dwRetVal = USP_SUCCESS;

    uhfDevPrint("enter uhf_ISO6B_Write_4x2_Byte fuction\r\n");

    for (wWriteIndex = 0; wWriteIndex < wLength; wWriteIndex++)
    {
        if ((ucByteWriteMask & (1 << wWriteIndex)) == 0)
        {
            /*�����ǰByte�Ѿ�д��ɹ���������д�����*/
            uhfDevPrint("the location 4byte has been write successfully! no need to rewrite again!\r\n");
            continue;
        }

        dwRetVal = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr + (wWriteIndex * 4), 1, &pucWriteData[wWriteIndex * 4], uhf_ISO6B_Write_4_byte);

        if (USP_SUCCESS == dwRetVal)
        {
            uhfDevPrint("Iso Write Data Success at %d\r\n", wStartAddr + (wWriteIndex * 4));
        }
        else
        {
            uhfDevPrint("Iso Write Data Failed at %d\r\n", wStartAddr + (wWriteIndex * 4));
            break;
        }
    }

    *pwWriteSuccessLen = wWriteIndex;
    return dwRetVal;
}

WORD32 uhf_ISO6B_Write_4x2_ByteVerify(WORD16 wStartAddr, WORD16 wLength,
                                      BYTE *pucWriteData, BYTE *pByteWriteMask, WORD16 wRetryTimes)
{
    BYTE ucReadData[8] = {0};
    WORD32 dwRet = USP_SUCCESS;
    WORD16 wReadIndex = 0;

    uhfDevPrint("enter uhf_ISO6B_Write_4x2_ByteVerify fuction\r\n");

    if (0 == wRetryTimes)
    {
        *pByteWriteMask = 0;
        return  USP_SUCCESS;
    }

    dwRet = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr, 1, ucReadData, uhf_ISO6B_Read_8_byte);

    if (USP_SUCCESS == dwRet)
    {
        uhfDevPrint("uhf_ISO6B_Write_4x2_ByteVerify read success\r\n");
        *pByteWriteMask = 0;

        uhfDevPrint("uhf_ISO6B_Write_4x2_ByteVerify Read Data");

        for (wReadIndex = 0; wReadIndex < wLength; wReadIndex++)
        {

            uhfDevPrint(" %02x %02x %02x %02x",
                        ucReadData[wReadIndex], ucReadData[wReadIndex + 1],
                        ucReadData[wReadIndex + 2], ucReadData[wReadIndex + 3]);
            uhfDevPrint("\r\n");

            if (memcmp(&ucReadData[4 * wReadIndex], &pucWriteData[4 * wReadIndex], 4) != 0)
            {
                *pByteWriteMask |= (1 << wReadIndex);
            }
        }
    }
    else
    {
        uhfDevPrint("uhf_ISO6B_Write_4x2_ByteVerify read failed\r\n");
    }

    return dwRet;

}
WORD32 uhf_ISO6B_LockByteVerify(WORD16 wStartAddr, WORD16 wRetryTimes, WORD16 wVerifyTimes)
{
    WORD32 dwRetVal = USP_SUCCESS;
    BYTE  ucLockState = 0;
    WORD16  wVerifyIndex = 0;

    uhfDevPrint("enter uhf_ISO6B_LockByteVerify fuction\r\n");

    do
    {
        dwRetVal = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr, 1, &ucLockState, uhf_ISO6B_LockByte);

        if (USP_SUCCESS != dwRetVal)
        {
            uhfDevPrint("Lock Byte %d  error\r\n", wStartAddr);
            break;
        }
        else
        {
            if (wVerifyTimes == 0)
            {
                uhfDevPrint("Lock no need to verify\r\n");
                break;
            }

            dwRetVal = uhf_ISO6B_RetryFuc(wRetryTimes, wStartAddr, 1, &ucLockState, uhf_ISO6B_LockQueryByte);

            if (USP_SUCCESS != dwRetVal)
            {
                uhfDevPrint("Lock Byte %d Query Error \r\n", wStartAddr);
                break;
            }
            else
            {
                if (ISO6B_BYTE_LOCKED == ucLockState)
                {
                    uhfDevPrint("ISO6B_Lock Verify Locked\r\n");
                    break;
                }
                else
                {
                    uhfDevPrint("ISO6B_Lock Verify UnLock\r\n");
                    dwRetVal = USP_UHF_ISO6B_RESULT_VERIFY_ERROR;
                }
            }

        }
    }
    while ((++wVerifyIndex) < wVerifyTimes);

    return dwRetVal;
}

#endif
