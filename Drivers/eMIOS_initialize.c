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
    EMIOS_0.MCR.B.GPRE= 0;    /* Divide 64 MHz sysclk by 0+1 = 1 for 64MHz eMIOS clk*/
    EMIOS_0.MCR.B.GPREN = 1;  /* Enable eMIOS clock */
    EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
    EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
//---------------------------------------------------------------------------------------------  
  
    /* EMIOS 0 CH 23: Modulus Up Counter, counter bus A */
    EMIOS_0.CH[23].CADR.R = 53332;     /* Period will be 53332+1 = 53333 clocks 1/300Hz */
    EMIOS_0.CH[23].CCR.B.MODE = 0x50;  /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[23].CCR.B.BSL = 0x3;    /* Use internal counter */
    EMIOS_0.CH[23].CCR.B.UCPRE = 3;    /* Set channel prescaler to divide by 3+1 */
    EMIOS_0.CH[23].CCR.B.UCPEN = 1;    /* Enable prescaler; uses default divide by 1 */
    EMIOS_0.CH[23].CCR.B.FREN = 1;     /* Freeze channel counting when in debug mode */
    
    /* EMIOS 0 CH 17: Output Pulse Width Modulation 舵机*/
    EMIOS_0.CH[17].CADR.R = 0;         /* Leading edge when channel counter bus=250*/
    EMIOS_0.CH[17].CBDR.R = 0;         /* Trailing edge when channel counter bus=500*/
    EMIOS_0.CH[17].CCR.B.BSL = 0x00;   /* Use counter bus A (default) */
    EMIOS_0.CH[17].CCR.B.EDPOL = 1;    /* Polarity-leading edge sets output/trailing clears*/
    EMIOS_0.CH[17].CCR.B.MODE = 0x60;  /* Mode is OPWM Buffered */
    SIU.PCR[PCR65_PE1].R = 0x0600;     /* MPC56xxS: Assign EMIOS_0 ch 17 to pad */
//--------------------------------------------------------------------------------------------- 
    
    /* EMIOS 0 CH 16: Modulus Up Counter, counter bus D */
    EMIOS_0.CH[16].CADR.R = 7999;      /* Period will be 7999+1 = 8000 clocks 1/8KHz */
    EMIOS_0.CH[16].CCR.B.MODE = 0x50;  /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[16].CCR.B.BSL = 0x3;    /* Use internal counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;      /* Set channel prescaler to divide by 1 */   //1
    EMIOS_0.CH[16].CCR.B.UCPEN = 1;    /* Enable prescaler; uses default divide by 1 */
    EMIOS_0.CH[16].CCR.B.FREN = 1;     /* Freeze channel counting when in debug mode */
    
    /* EMIOS 0 CH 21: Output Pulse Width Modulation 电机*/
    EMIOS_0.CH[21].CADR.R = 0;         /* Leading edge when channel counter bus=250*/
    EMIOS_0.CH[21].CBDR.R = 0;         /* Trailing edge when channel counter bus=500*/
    EMIOS_0.CH[21].CCR.B.BSL = 0x01;   /* Use counter bus D */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;    /* Polarity-leading edge sets output/trailing clears*/
    EMIOS_0.CH[21].CCR.B.MODE = 0x60;  /* Mode is OPWM Buffered */
    SIU.PCR[PCR69_PE5].R = 0x0600;     /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
 //--------------------------------------------------------------------------------------------- 
    
    /* EMIOS 0 CH 0: Modulus Up Counter, counter bus B */
    EMIOS_0.CH[0].CADR.R = 65535;      /* Period will be 65535+1 = 65536 clocks 1/300Hz */
    EMIOS_0.CH[0].CCR.B.MODE = 0x50;   /* Modulus Counter Buffered (MCB) */
    EMIOS_0.CH[0].CCR.B.BSL = 0x3;     /* Use internal counter */
    EMIOS_0.CH[0].CCR.B.UCPRE = 1;     /* Set channel prescaler to divide by 1+1 */
    EMIOS_0.CH[0].CCR.B.UCPEN = 1;     /* Enable prescaler; uses default divide by 1 */
    EMIOS_0.CH[0].CCR.B.FREN = 1;      /* Freeze channel counting when in debug mode */
    EMIOS_0.CH[0].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[141].R = 0x03;            /* EMIOS0 CH1 interrupt vector with priority 3 */
    
    /* EMIOS 0 CH 0: Input Period Measurement 车速*/
    EMIOS_0.CH[2].CCR.B.BSL = 0x01;    /* Use counter bus B */
    EMIOS_0.CH[2].CCR.B.EDPOL = 1;     /* Positive edge capture */
    EMIOS_0.CH[2].CCR.B.MODE = 0x05;   /* Input period measurement mode*/
    SIU.PCR[PCR2_PA2].R = 0x0500;      /* MPC56xxS: Assign EMIOS_0 ch 2 to pad */
    EMIOS_0.CH[2].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[142].R = 0x03;            /* EMIOS0 CH1 interrupt vector with priority 3 */
//--------------------------------------------------------------------------------------------- 
    /* EMIOS 0 CH 24: Input Period Measurement */
    EMIOS_0.CH[24].CCR.B.BSL = 0x00;    /* Use counter bus A (default) */
    EMIOS_0.CH[24].CCR.B.EDPOL = 0;     /* Negative edge capture */
    EMIOS_0.CH[24].CCR.B.MODE = 0x02;   /* Singal action input capture mode*/
    SIU.PCR[PCR60_PD12].R = 0x0900;     /* MPC56xxS: Assign EMIOS_0 ch 24 to pad */
    EMIOS_0.CH[24].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[153].R = 0x04;             /* EMIOS0 CH1 interrupt vector with priority 4 */
//--------------------------------------------------------------------------------------------- 
    /* EMIOS 0 CH 4: Input Period Measurement */
    EMIOS_0.CH[4].CCR.B.BSL = 0x00;    /* Use counter bus A (default) */
    EMIOS_0.CH[4].CCR.B.EDPOL = 0;     /* Negative edge capture */
    EMIOS_0.CH[4].CCR.B.MODE = 0x02;   /* Singal action input capture mode*/
    SIU.PSMI[14].B.PADSEL = 0x01;
    SIU.PCR[PCR28_PB12].R = 0x0500;    /* MPC56xxS: Assign EMIOS_0 ch 4 to pad */
    EMIOS_0.CH[4].CCR.B.FEN = 1;       /* Enable interrupt*/
    INTC.PSR[143].R = 0x05;            /* EMIOS0 CH1 interrupt vector with priority 4 */

    
}