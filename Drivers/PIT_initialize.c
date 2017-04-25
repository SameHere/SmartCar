/*******************************(C) Copyright 2013 智能车团队 ***************************************/
//    文件名:     PIT_initialize.C
//    描述:       PIT初始化
//    平台:       MPC5604B_M27V
//    作者:       吕科
/****************************************************************************************************/
#include "MPC5604B_0M27V_0102.h" // Use proper include file 
#include "MyHeader.h" 
//----------------------------------------------------------------------------------------------------
//      PIT 初始化
//----------------------------------------------------------------------------------------------------
void initPIT(void)
{                                  /* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
  PIT.PITMCR.R = 0x00000001;       /* Enable PIT and configure timers to stop in debug mode */
  PIT.CH[1].LDVAL.R = 320000;//64000;       /* PIT1 timeout = 64000 sysclks x 1sec/64M sysclks = 1 msec */
  PIT.CH[1].TCTRL.R = 0x000000003; /* Enable PIT1 interrupt and make PIT active to count */ 
  //INTC.PSR[60].R = 0x02;           /* PIT 1 interrupt vector with priority 1 */
}



