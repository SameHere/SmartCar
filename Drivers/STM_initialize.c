/*******************************(C) Copyright 2013 智能车团队 ************************************
文件名:     STM_initialize.C
描述:       STM：系统定时模块初始化
平台:       MPC5604B_M27V
作者:       吕科
/************************************************************************************************/
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
void initSTM(void)
{
  STM.CR.B.CPS = 31;          /* Clock divide by 31+1 */
  STM.CR.B.TEN = 1;          /* Timer counter enabled */
  STM.CH[0].CCR.B.CEN = 1;   /* CH0 is enabled */
 // INTC.PSR[30].R = 0x01;     /* STM CH0 interrupt vector with priority 1 */
}