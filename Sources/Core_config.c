/*******************************(C) Copyright 2013 智能车团队 ************************************
文件名:     Core_config.C
描述:       MCU核心初始化:系统工作模式,时钟,看门狗,中断向量
平台:       MPC5604B_M27V
作者:       吕科
/************************************************************************************************/
#include "MPC5604B_M27V.h"
#include "MyHeader.h"
extern uint32_t __IVPR_VALUE; /* Interrupt Vector Prefix vaue from link file*/
extern const vuint32_t IntcIsrVectorTable[];
/*----------------------------------------------------------------------------------------------*/
/*  系统时钟初始化
/*----------------------------------------------------------------------------------------------*/
void initModesAndClock(void)
{
  ME.MER.R = 0x0000001D;               /* Enable DRUN, RUN0, SAFE, RESET modes */
                                       /* Initialize PLL before turning it on: */
/* Use 1 of the next 2 lines depending on crystal frequency: */
  CGM.FMPLL_CR.R = 0x02400100;         /* 8 MHz xtal: Set PLL0 to 64 MHz */ 

        /*CGM.FMPLL_CR.R = 0x12400100;*/       /* 40 MHz xtal: Set PLL0 to 64 MHz */ 
  
  ME.RUN[0].R = 0x001F0074;            /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
  ME.RUNPC[1].R = 0x00000010;          /* Peri. Cfg. 1 settings: only run in RUN0 mode */
  
  ME.PCTL[PCTL32_ADC0].R = RUN_PC1;    /* MPC56xxB/P/S ADC 0: select ME.RUNPC[1] */
  ME.PCTL[PCTL48_LINFlex0].R = RUN_PC1;/* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] */	
  ME.PCTL[PCTL68_SIUL].R = RUN_PC1;    /* MPC56xxB/S SIUL:  select ME.RUNPC[1] */ 
  ME.PCTL[PCTL72_eMIOS0].R = RUN_PC1;  /* MPC56xxB/S EMIOS 0:  select ME.RUNPC[1] */ 
  ME.PCTL[PCTL92_PIT_RTI].R = RUN_PC1; /* PIT, RTI: select ME_RUN_PC[1] */
  
/* Mode Transition to enter RUN0 mode: */
  ME.MCTL.R = 0x40005AF0;              /* Enter RUN0 Mode & Key */
  ME.MCTL.R = 0x4000A50F;              /* Enter RUN0 Mode & Inverted Key */  
  while (ME.GS.B.S_MTRANS) {}          /* Wait for mode transition to complete */    
                                       /* Note: could wait here using timer and/or I_TC IRQ */
  while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
  
  CGM.SC_DC[0].R = 0x80;               /* MPC56xxB/S: Enable peri set 3 sysclk divided by 1 */
  CGM.SC_DC[1].R = 0x80;               /* MPC56xxB/S: Enable peri set 3 sysclk divided by 1 */
  CGM.SC_DC[2].R = 0x80;               /* MPC56xxB/S: Enable peri set 3 sysclk divided by 1 */
  
}
/*----------------------------------------------------------------------------------------------*/
/*  看门狗初始化
/*----------------------------------------------------------------------------------------------*/
void disableWatchdog(void)
{
  SWT.SR.R = 0x0000c520;     /* Write keys to clear soft lock bit */
  SWT.SR.R = 0x0000d928; 
  SWT.CR.R = 0x8000010A;     /* Clear watchdog enable (WEN) */
} 
/*----------------------------------------------------------------------------------------------*/
/*  初始化中断向量
/*----------------------------------------------------------------------------------------------*/
asm initIrqVectors(void) 
{
  lis	   r3, __IVPR_VALUE@h   /* IVPR value is passed from link file */
  ori      r3, r3, __IVPR_VALUE@l 
  mtivpr   r3									 
}
/*----------------------------------------------------------------------------------------------*/
/*  初始化中断控制器
/*----------------------------------------------------------------------------------------------*/
void initINTC(void) 
{	
  INTC.MCR.B.HVEN = 0;       /* Single core: initialize for SW vector mode */
  INTC.MCR.B.VTES = 0;       /* Single core: Use default vector table 4B offsets */
  INTC.IACKR.R = (uint32_t) &IntcIsrVectorTable[0];    /* MPC555x: INTC ISR table base */
}
/*----------------------------------------------------------------------------------------------*/
/*  中断允许
/*----------------------------------------------------------------------------------------------*/
void enableIrq(void)
{
  INTC.CPR.B.PRI = 0;       /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");         /* Enable external interrupts */
}
/*----------------------------------------------------------------------------------------------*/
/*  中断禁止
/*----------------------------------------------------------------------------------------------*/
void disableIrq(void)
{
  INTC.CPR.B.PRI = 0;       /* Single Core: Lower INTC's current priority */
  asm(" wrteei 0");         /* disable external interrupts */
}
/*----------------------------------------------------------------------------------------------*/
/************************************************************************************************/
void core_config(void)
{
  initModesAndClock();  /* MPC56xxP/B/S: Initialize mode entries, set sysclk = 64MHz*/
  disableWatchdog();    /* Disable watchdog */
  initIrqVectors();		/* Initialize exceptions: only need to load IVPR */
  initINTC();			/* Initialize INTC for software vector mode */	
}

