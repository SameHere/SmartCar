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
void initADC(uint32_t ADC_mode)
{
    SIU.PCR[PCR57_PD9].R = 0x2000;  // Use PD[9] for ANP13 pin
    ADC.MCR.R = ADC_mode;            // Initialize ADC0 for scan mode
    ADC.CTR[0].R = 0x00000206;       // Conversion times for 32MHz ADClock
    ADC.NCMR[0].R = 0x00002000;     // Select ANP13 inputs for conversion
    ADC.MCR.B.NSTART=1;             /* Trigger normal conversions for ADC0 */
    //ADC.CTR[1].R = 0x00008606;       // Conversion times for 32MHz ADClock */
}



