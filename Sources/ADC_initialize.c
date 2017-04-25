/*******************************(C) Copyright 2013 智能车团队 ***************************************/
//    文件名:     ADC_initialize.C
//    描述:       ADC初始化
//    平台:       MPC5604B_M27V
//    作者:       吕科
/****************************************************************************************************/
#include "MPC5604B_0M27V_0102.h" // Use proper include file 
#include "MyHeader.h" 
//----------------------------------------------------------------------------------------------------
//      ADC 初始化
//----------------------------------------------------------------------------------------------------
void initADC()
{
	//电感AD接口：
    SIU.PCR[PCR21_PB5].R = 0x2000;   // Use PB[5]  for ANP1  pin  此些口都为固定搭配
    SIU.PCR[PCR22_PB6].R = 0x2000;   // Use PB[6]  for ANP2  pin
    SIU.PCR[PCR23_PB7].R = 0x2000;   // Use PB[7]  for ANP3  pin
    SIU.PCR[PCR57_PD9].R = 0x2000;   // Use PD[9]  for ANP13  pin
    SIU.PCR[PCR52_PD4].R = 0x2000;   // Use PD[4]  for ANP8  pin
    SIU.PCR[PCR53_PD5].R = 0x2000;   // Use PD[5]  for ANP9  pin
    SIU.PCR[PCR54_PD6].R = 0x2000;   // Use PD[6]  for ANP10 pin
    SIU.PCR[PCR55_PD7].R = 0x2000;   // Use PD[7]  for ANP11 pin
	//预留AD口：
    SIU.PCR[PCR48_PD0].R = 0x2000;   // Use PD[9]  for ANP4 pin
    SIU.PCR[PCR49_PD1].R = 0x2000;   // Use PD[10] for ANP5 pin
    //SIU.PCR[PCR50_PD2].R = 0x2000;   // Use PD[11] for ANP6 pin
    
    ADC.MCR.R    = 0x20000000;          // Initialize ADC0 for scan mode
    ADC.CTR[0].R = 0x00000206;       // Conversion times for 32 MHz ADClock
}



