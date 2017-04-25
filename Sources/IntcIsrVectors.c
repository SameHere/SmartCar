/* IntcIsrVectors.c - table of ISRs for INTC in SW vector Mode */
/* Description:  Contains addresses for 310 ISR vectors */
/*			     Table address gets loaded to INTC_IACKR */
/*               Alignment: MPC551x: 2 KB after a 4KB boundary; MPC555x: 64 KB*/
/* April 22, 2004 S. Mihalik */ 
/* March 16, 2006 S. Mihalik - Modified for compile with Diab 5.3 */
/* Jun 29 2006 SM - Used pragma align instead of hard coding address */
/* Jul  5 2007 SM - alignment now done in link file; changes for MPC551x */
/* Aug 30 2007 SM - Added pragma for CodeWarrior */
/* Oct 22 2008 SM - Changed to use PIT1 ISR instead of eMIOS Ch 0 ISR */

#include "typedefs.h"

void dummy (void);

extern PIT_CH1_isr();
extern EMIOS0_CH16_17_isr();
extern EMIOS0_CH18_19_isr();
extern EMIOS0_CH20_21_isr();
extern LINFlex0_RXI_ISR();

/* Use next two lines with Diab compile */
/* #pragma section CONST ".intc_sw_isr_vector_table" */		 /* Diab compiler pragma */
/* const uint32_t IntcIsrVectorTable[] = {   */ 

/* Use pragma next two lines with CodeWarrior compile */
#pragma section data_type ".intc_sw_isr_vector_table" ".intc_sw_isr_vector_table" data_mode=far_abs  
  uint32_t IntcIsrVectorTable[] = { 	 	 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 00 - 04 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 05 - 09 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 10 - 14 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 15 - 19 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 20 - 24 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 25 - 29 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 30 - 34 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 35 - 39 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 40 - 44 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 45 - 49 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 50 - 54 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 55 - 59 */
 /* Use the next line for MPC551x or MPC563x: */
 /* (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy,*/ /* ISRs 60 - 64 */ 
 /* Use the next line for MPC56xB, MPC56xxP, MPC56xS, where PIT1 vector number is 60: */
 (uint32_t)&PIT_CH1_isr, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 60 - 64 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 65 - 69 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 70 - 74 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&LINFlex0_RXI_ISR, /* ISRs 75 - 79 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 80 - 84 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 85 - 89 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 90 - 94 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 95 - 99 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 100 - 104 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 105 - 109 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 110 - 114 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 115 - 119 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 120 - 124 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 125 - 129 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 130 - 134 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 135 - 139 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 140 - 144 */ 
 /* Use the next line for MPC563x or MPC56xxB, MPC56xxP, MPC56xxS: */
 /* (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&Pit1ISR,*/ /* ISRs 145 - 149 */
 /* Use the next line for MPC551x, where PIT1 vector number is 149: */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&EMIOS0_CH16_17_isr, /* ISRs 145 - 149 */
 (uint32_t)&EMIOS0_CH18_19_isr, (uint32_t)&EMIOS0_CH20_21_isr, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 150 - 154 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 155 - 159 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 160 - 164 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 155 - 169 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 170 - 174 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 175 - 179 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 180 - 184 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 185 - 189 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 190 - 194 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 195 - 199 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 200 - 204 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 205 - 209 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 210 - 214 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 215 - 219 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 220 - 224 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 225 - 229 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 230 - 234 */ 
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, /* ISRs 235 - 239 */
 (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy, (uint32_t)&dummy  /* ISRs 240 - 244 */ 

/* For MPC563x, continue vectors and add Pit1ISR address for INTC vector number 302 */
 };
//&software_ISR0        &software_ISR1         &software_ISR2       &software_ISR3       &software_ISR4              /* ISRs 00 - 04 */
//&software_ISR5        &software_ISR6         &software_ISR7       &reserved            &Platform_Flash_Bank_ISR    /* ISRs 05 - 09 */ 
//&reserved             &reserved              &reserved            &reserved            &reserved                   /* ISRs 10 - 14 */
//&reserved             &reserved              &reserved            &reserved            &reserved                   /* ISRs 15 - 19 */
//&reserved             &reserved              &reserved            &reserved            &reserved                   /* ISRs 20 - 24 */
//&reserved             &reserved              &reserved            &SWT_timeout_ISR     &reserved                   /* ISRs 25 - 29 */
//&STM_CH0_ISR          &STM_CH1_ISR           &STM_CH2_ISR         &STM_CH3_ISR         &reserved                   /* ISRs 30 - 34 */
//&ECC_DBD_Platform_ISR &ECC_SBC_Platform_ISR  &reserved            &RTC_RTC_ISR         &RTC_API_ISR                /* ISRs 35 - 39 */
//&reserved             &SIU_EIRQ0_ISR         &SIU_EIRQ1_ISR       &reserved            &reserved                   /* ISRs 40 - 44 */
//&reserved             &wakeup_IRQ0_ISR       &wakeup_IRQ1_ISR     &wakeup_IRQ1_ISR     &reserved                   /* ISRs 45 - 49 */
//&reserved             &safe_Mode_ISR         &mode_transition_ISR &invalid_mode_ISR    &invalid_mode_config_ISR    /* ISRs 50 - 54 */
//&reserved             &ipi_int_ISR           &ipi_int_osc_ISR     &reserved            &PIT_CH0_ISR                /* ISRs 55 - 59 */
                                                                      
//&PIT_CH1_ISR             &PIT_CH2_ISR              &ADC_EOC_ISR             &ADC_ER_ISR               &ADC_WD                     /* ISRs 60 - 64 */ 
//&FlexCAN0_ESR_ISR        &FlexCAN0_Warning_ISR     &reserved                &FlexCAN0_BUF_00_03_ISR   &FlexCAN0_BUF_04_07_ISR     /* ISRs 65 - 69 */
//&FlexCAN0_BUF_08_11_ISR  &FlexCAN0_BUF_12_15_ISR   &FlexCAN0_BUF_16_31_ISR  &FlexCAN0_BUF_32_63_ISR   &DSPI0_SR_TFUF_RFOF_ISR     /* ISRs 70 - 74 */ 
//&DSPI0_SR_EOQF_ISR       &DSPI0_SR_TFFF_ISR        &DSPI0_SR_TCF_ISR        &DSPI0_SR_RFDF_ISR        &LINFlex0_RXI_ISR           /* ISRs 75 - 79 */
//&LINFlex0_TXI_ISR        &LINFlex0_ERR_ISR         &reserved                &reserved                 &reserved                   /* ISRs 80 - 84 */ 
//&FlexCAN1_ESR_ISR        &FlexCAN1_Warning_ISR     &reserved                &FlexCAN1_BUF_00_03_ISR   &FlexCAN1_BUF_04_07_ISR     /* ISRs 85 - 89 */
//&FlexCAN1_BUF_08_11_ISR  &FlexCAN1_BUF_12_15_ISR   &FlexCAN1_BUF_16_31_ISR  &FlexCAN1_BUF_32_63_ISR   &DSPI1_SR_TFUF_RFOF_ISR     /* ISRs 90 - 94 */ 
//&DSPI1_SR_EOQF_ISR       &DSPI1_SR_TFFF_ISR        &DSPI1_SR_TCF_ISR        &DSPI1_SR_RFDF_ISR        &LINFlex1_RXI_ISR           /* ISRs 95 - 99 */
//&LINFlex1_TXI_ISR        &LINFlex1_ERR_ISR         &reserved                &reserved                 &reserved                   /* ISRs 100 - 104 */ 
//&FlexCAN2_ESR_ISR        &FlexCAN2_Warning_ISR     &reserved                &FlexCAN2_BUF_00_03_ISR   &FlexCAN2_BUF_04_07_ISR     /* ISRs 105 - 109 */
//&FlexCAN2_BUF_08_11_ISR  &FlexCAN2_BUF_12_15_ISR   &FlexCAN2_BUF_16_31_ISR  &FlexCAN2_BUF_32_63_ISR   &DSPI2_SR_TFUF_RFOF_ISR     /* ISRs 110 - 114 */ 
//&DSPI2_SR_EOQF_ISR       &DSPI2_SR_TFFF_ISR        &DSPI2_SR_TCF_ISR        &DSPI2_SR_RFDF_ISR        &LINFlex2_RXI_ISR           /* ISRs 115 - 119 */
//&LINFlex2_TXI_ISR        &LINFlex2_ERR_ISR         &LINFlex3_RXI_ISR        &LINFlex3_TXI_ISR         &LINFlex3_ERR_ISR           /* ISRs 120 - 124 */ 
//&I2C_SR_ISR              &reserved                 &PIT_CH3_ISR             &PIT_CH4_ISR              &PIT_CH5_ISR                /* ISRs 125 - 129 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 130 - 134 */ 
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 135 - 139 */
//&reserved                &EMIOS0_GFR_F0_F1_ISR     &EMIOS0_GFR_F2_F3_ISR    &EMIOS0_GFR_F4_F5_ISR     &EMIOS0_GFR_F6_F7_ISR       /* ISRs 140 - 144 */                                                                       
                                                                      
//&EMIOS0_GFR_F8_F9_ISR    &EMIOS0_GFR_F10_F11_ISR   &EMIOS0_GFR_F12_F13_ISR  &EMIOS0_GFR_F14_F15_ISR   &EMIOS0_GFR_F16_F17_ISR     /* ISRs 145 - 149 */
//&EMIOS0_GFR_F18_F19_ISR  &EMIOS0_GFR_F20_F21_ISR   &EMIOS0_GFR_F22_F23_ISR  &EMIOS0_GFR_F24_F25_ISR   &EMIOS0_GFR_F26_F27_ISR     /* ISRs 150 - 154 */
//&reserved                &reserved                 &EMIOS1_GFR_F0_F1_ISR    &EMIOS1_GFR_F2_F3_ISR     &EMIOS1_GFR_F4_F5_ISR       /* ISRs 155 - 159 */
//&EMIOS1_GFR_F6_F7_ISR    &EMIOS1_GFR_F8_F9_ISR     &EMIOS1_GFR_F10_F11_ISR  &EMIOS1_GFR_F12_F13_ISR   &EMIOS1_GFR_F14_F15_ISR     /* ISRs 160 - 164 */
//&EMIOS1_GFR_F16_F17_ISR  &EMIOS1_GFR_F18_F19_ISR   &EMIOS1_GFR_F20_F21_ISR  &EMIOS1_GFR_F22_F23_ISR   &EMIOS1_GFR_F24_F25_ISR     /* ISRs 155 - 169 */
//&EMIOS1_GFR_F26_F27_ISR  &reserved                 &reserved                &FlexCAN3_ESR_ISR         &FlexCAN3_Warning_ISR       /* ISRs 170 - 174 */
//&reserved                &FlexCAN3_BUF_00_03_ISR   &FlexCAN3_BUF_04_07_ISR  &FlexCAN3_BUF_08_11_ISR   &FlexCAN3_BUF_12_15_ISR     /* ISRs 175 - 179 */
//&FlexCAN3_BUF_16_31_ISR  &FlexCAN3_BUF_32_63_ISR   &reserved                &reserved                 &reserved                   /* ISRs 180 - 184 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 185 - 189 */
//&FlexCAN4_ESR_ISR        &FlexCAN4_Warning_ISR     &reserved                &FlexCAN4_BUF_00_03_ISR   &FlexCAN4_BUF_04_07_ISR     /* ISRs 190 - 194 */
//&FlexCAN4_BUF_08_11_ISR  &FlexCAN4_BUF_12_15_ISR   &FlexCAN4_BUF_16_31_ISR  &FlexCAN4_BUF_32_63_ISR   &reserved                   /* ISRs 195 - 199 */
//&reserved                &reserved                 &FlexCAN5_ESR_ISR        &FlexCAN5_Warning_ISR     &reserved                   /* ISRs 200 - 204 */
//&FlexCAN5_BUF_00_03_ISR  &FlexCAN5_BUF_04_07_ISR   &FlexCAN5_BUF_08_11_ISR  &FlexCAN5_BUF_12_15_ISR   &FlexCAN5_BUF_16_31_ISR     /* ISRs 205 - 209 */
//&FlexCAN5_BUF_32_63_ISR  &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 210 - 214 */ 
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 215 - 219 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 220 - 224 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 225 - 229 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 230 - 234 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 235 - 239 */
//&reserved                &reserved                 &reserved                &reserved                 &reserved                   /* ISRs 240 - 244 */

void dummy (void)
 {
     while (1) {};    /* Wait forever or for watchdog timeout */ 
 }