/*******************************(C) Copyright 2013 智能车团队 ***********************************
文件名:     eMIOS_initialize.C
描述:       eMIOS初始化
平台:       MPC5604B_M27V
作者:       吕科
/************************************************************************************************/
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "MyHeader.h"
void initEMIOS(void)
{
    EMIOS_0.MCR.B.GPRE= 3;              /* Divide 64 MHz sysclk by 3+1 = 4 for 16MHz eMIOS clk*/
    EMIOS_0.MCR.B.GPREN = 1;            /* Enable eMIOS clock */
    EMIOS_0.MCR.B.GTBE = 1;             /* Enable global time base */
    EMIOS_0.MCR.B.FRZ = 1;              /* Enable stopping channels when in debug mode */

    /* EMIOS 0 CH 8: Modulus Up Counter, counter bus C */
    EMIOS_0.CH[8].CADR.R      = 7999;   /* Period will be 7999+1 = 8000 clocks 2KHz */
    EMIOS_0.CH[8].CCR.B.MODE  = 0x50;   /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[8].CCR.B.BSL   = 0x3;    /* Use internal counter */
    EMIOS_0.CH[8].CCR.B.UCPRE = 0;      /* Set channel prescaler to divide by 1 */   //1
    EMIOS_0.CH[8].CCR.B.UCPEN = 1;      /* Enable prescaler; uses default divide by 1 */
    EMIOS_0.CH[8].CCR.B.FREN  = 1;      /* Freeze channel counting when in debug mode */
    
    /* EMIOS 0 CH 13: Output Pulse Width Modulation 电机*/
    EMIOS_0.CH[13].CADR.R = 0;          /* Leading edge when channel counter bus=250*/
    EMIOS_0.CH[13].CBDR.R = 0;          /* Trailing edge when channel counter bus=500*/
    EMIOS_0.CH[13].CCR.B.BSL = 0x01;    /* Use counter bus D */
    EMIOS_0.CH[13].CCR.B.EDPOL = 1;     /* Polarity-leading edge sets output/trailing clears*/
    EMIOS_0.CH[13].CCR.B.MODE = 0x60;   /* Mode is OPWM Buffered */
    SIU.PSMI[21].B.PADSEL = 0x00;       /* 引脚复用配置 */
    SIU.PCR[PCR45_PC13].R = 0x0600;     /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
    
//舵机  S_D5 ---- 300HZ
   /* EMIOS 0 CH 23: Modulus Up Counter, counter bus A */
    EMIOS_0.CH[23].CADR.R = 13333;      /* Period will be 49999+1=50000 clocks    80Hz */
    EMIOS_0.CH[23].CCR.B.MODE = 0x50;   /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[23].CCR.B.BSL = 0x3;     /* Use internal counter */
    EMIOS_0.CH[23].CCR.B.UCPRE = 3;     /* Set channel prescaler to divide by 3+1==>4MHZ */
    EMIOS_0.CH[23].CCR.B.UCPEN = 1;     /* Enable prescaler; uses default divide by 1 */
    EMIOS_0.CH[23].CCR.B.FREN = 1;      /* Freeze channel counting when in debug mode */

 	/* EMIOS 0 CH 21: Output Pulse Width Modulation 舵机*/
    EMIOS_0.CH[21].CADR.R = 0;          /* Leading edge when channel counter bus=0*/
    EMIOS_0.CH[21].CBDR.R = 0;          /* Trailing edge when channel counter bus=0*/
    EMIOS_0.CH[21].CCR.B.BSL = 0x00;    /* Use counter bus A */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;     /* Polarity-leading edge sets output/trailing clears*/
    EMIOS_0.CH[21].CCR.B.MODE = 0x60;   /* Mode is OPWM Buffered */
    SIU.PCR[PCR69_PE5].R = 0x0600;      /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */

	/* EMIOS 0 CH 16: Modulus Up Counter, counter bus D */
    EMIOS_0.CH[16].CADR.R = 65535;      /* Period will be 65535+1 = 65536 clocks 1/300Hz */
    EMIOS_0.CH[16].CCR.B.MODE = 0x50;   /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[16].CCR.B.BSL = 0x3;     /* Use internal counter */
    EMIOS_0.CH[16].CCR.B.UCPRE = 0;     /* Set channel prescaler to divide by 0+1 */
    EMIOS_0.CH[16].CCR.B.UCPEN = 1;     /* Enable prescaler; uses default divide by 1 ==> 16/1=16 MHz */
    EMIOS_0.CH[16].CCR.B.FREN = 1;      /* Freeze channel counting when in debug mode */
    EMIOS_0.CH[16].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[149].R = 0x03;             /* EMIOS0 CH1 interrupt vector with priority 3 */ 
 
   /* EMIOS 0 CH 19: Input Period Measurement 车速1*/
    EMIOS_0.CH[19].CCR.B.BSL = 0x01;    /* Use counter bus D */
    EMIOS_0.CH[19].CCR.B.EDPOL = 1;     /* Negative edge capture */
    EMIOS_0.CH[19].CCR.B.MODE = 0x02;   /* Input period measurement mode*/
    SIU.PCR[PCR67_PE3].R = 0x0500;      /* MPC56xxS: Assign EMIOS_0 ch 5 to pad */ 
    EMIOS_0.CH[19].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[150].R = 0x03;             /* EMIOS0 CH5 interrupt vector with priority 3 */
  
   /* EMIOS 0 CH 20: Input Period Measurement 车速2*/
    EMIOS_0.CH[20].CCR.B.BSL = 0x01;    /* Use counter bus D */
    EMIOS_0.CH[20].CCR.B.EDPOL = 1;     /* Positive edge capture */
    EMIOS_0.CH[20].CCR.B.MODE = 0x02;   /* Input period measurement mode*/
    SIU.PCR[PCR68_PE4].R = 0x0500;      /* MPC56xxS: Assign EMIOS_0 ch 6 to pad */ 
    EMIOS_0.CH[20].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[151].R = 0x03;             /* EMIOS0 CH6 interrupt vector with priority 3 */

}