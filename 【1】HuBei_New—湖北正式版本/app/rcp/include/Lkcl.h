#ifndef LKCL_H
#define LKCL_H

/*һ����������м������������*/
#define UHF_RF_MAX_LKCL_TABLE_LEN  (3) 

typedef  struct tag_initPhase
{
    SWORD16 swCos;
    SWORD16 swSin;
}T_InitPhase;


/*��������׼��������Ƶ�������������±�����Ƶ����*/
typedef struct
{
  WORD16 wHopTableID;
  WORD16 wTryIndex;
  T_USPUHFLKCLParas tAntLKCLPara[UHF_RF_MAX_FREQ_NUM];
}T_SingleFreqTableLKCLPara;

typedef struct
{
  WORD16 wLKCLTableLen;
  WORD16 wLKCLIndex;
  T_SingleFreqTableLKCLPara tSingleFreqTableLKCLPara[UHF_RF_MAX_LKCL_TABLE_LEN];
}T_SingleAntLKCLPara;

#define FPGA_RFADJ_CANNOT_STOP       (100)

void uhfRf_SetLKCLInitPara(void);
void uhfRf_VAutoAdjust(void);
WORD32 uhfRf_FineLKCL(BYTE ucAntIndex, T_UhfRfFreqHopCtrl *ptFreqHopCtrl);
WORD32 uhfRf_FPGALKCLProc(T_USPUHFLKCLInitParas *ptInintLKCLAdj,
                                   T_USPUHFLKCLParas *ptLKCLSetPara,
                                   WORD16 *pwTryindex);

#endif
