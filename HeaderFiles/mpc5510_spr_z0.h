/**************************************************************************
 * FILE NAME: $RCSfile: mpc5510_spr_z0.h,v $ COPYRIGHT (c) Freescale 2008 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file contain all of the SPR register and bit field definitions    *
 * for MPC5510 Z0 core. There are no address assignments for the SPR      *
 * registers. Instead, the bit field structures are only assigned.        *
 * SPR register numbers are also assigned.                                *
 *========================================================================*
 * ORIGINAL AUTHOR: b04629                                        *
 * $Log: mpc5510_spr_z0.h,v $
 * Revision 1.2  2008/02/19 11:07:58  b04629
 * Replaced // comments for MISRA.
 *
 * Revision 1.1  2008/02/01 09:57:40  b04629
 * Based on mpc5516_spr_z0.h Revision 1.2.
 *
 *
 **************************************************************************/

/* >>>>NOTE! this file describes fixed special purpose registers.  */
/*           Please do not edit it directly!<<<<                   */

#ifndef _MPC5510_SPR_Z0_
#define _MPC5510_SPR_Z0_
/* This ifndef has a corresponding #endif at the bottom of this file  */
/*  so that it will only be included once.                            */

#include "typedefs.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************************/
/* Example instantiation and use:           */
/*  union CRVAL my_cr;                      */
/*  my_cr.B.CR0 = 1;                        */
/*  my_cr.R = 0x10000000                    */
/********************************************/



/****************************************************************************/
/*                   CPU REGISTERS: General Registers                       */
/****************************************************************************/
        union SPR_CRVAL { 
            vuint32_t R;
            struct {
                vuint32_t CR0:4;
                vuint32_t CR1:4;
                vuint32_t CR2:4;
                vuint32_t CR3:4;
                vuint32_t CR4:4;
                vuint32_t CR5:4;
                vuint32_t CR6:4;
                vuint32_t CR7:4;
            } B;
        };

        union SPR_LRVAL { 
            vuint32_t R;
            struct {
                vuint32_t LINKADDRESS:32;
            } B;
        };

        union SPR_CTRVAL {
            vuint32_t R;
            struct {
                vuint32_t COUNTVALUE:32;
            } B;
        };

        union SPR_XERVAL {   
            vuint32_t R;
            struct {
                vuint32_t SO:1;
                vuint32_t OV:1;
                vuint32_t CA:1;
                vuint32_t :22;
                vuint32_t BYTECNT:7;
            } B;
        };


/****************************************************************************/
/*                   CPU REGISTERS: Processor Control Registers             */
/****************************************************************************/
        union SPR_MSRVAL {  
            vuint32_t R;
            struct {
                vuint32_t :5;
                vuint32_t UCLE:1;
                vuint32_t :7;
                vuint32_t WE:1;
                vuint32_t CE:1;
                vuint32_t :1;
                vuint32_t EE:1;
                vuint32_t PR:1;
                vuint32_t FP:1;
                vuint32_t ME:1;
                vuint32_t FE0:1;
                vuint32_t :1;
                vuint32_t DE:1;
                vuint32_t FE1:1;
                vuint32_t :2;
                vuint32_t IS:1;
                vuint32_t DS:1;
                vuint32_t :2;
                vuint32_t RI:1;
                vuint32_t :1;
            } B;
        };

        union SPR_PIRVAL {   
            vuint32_t R;
            struct {
                vuint32_t :24;
                vuint32_t ID:8;
            } B;
        };

        union SPR_PVRVAL {   
            vuint32_t R;
            struct {
                vuint32_t :12;
                vuint32_t VER:4;
                vuint32_t MGBUSE:8;
                vuint32_t MJRREV:4;
                vuint32_t MGBID:4;
            } B;
        };

         union SPR_SVRVAL {   
            vuint32_t R;
            struct {
                vuint32_t SYSVER:32;
            } B;
        };

        union SPR_HID0VAL {  
            vuint32_t R;
            struct {
                vuint32_t EMCP:1;
                vuint32_t :5;
                vuint32_t BPRED:2;
                vuint32_t DOZE:1;
                vuint32_t NAP:1;
                vuint32_t SLEEP:1;
                vuint32_t :3;
                vuint32_t ICR:1;
                vuint32_t NHR:1;
                vuint32_t :1;
                vuint32_t TBEN:1;
                vuint32_t SEL_TBCLK:1;
                vuint32_t DCLREE:1;
                vuint32_t DCLRCE:1;
                vuint32_t CICLRDE:1;
                vuint32_t MCCLRDE:1;
                vuint32_t DAPUEN:1;
                vuint32_t :8;
            } B;
        };

        union SPR_HID1VAL {  
            vuint32_t R;
            struct {
                vuint32_t :16;
                vuint32_t SYSCTL:8;
                vuint32_t ATS:1;
                vuint32_t :7;
            } B;
        };

        union SPR_BUCSRVAL {  
            vuint32_t R;
            struct {
                vuint32_t :22;
                vuint32_t BBFI:1;
                vuint32_t :8;
                vuint32_t BPEN:1;
            } B;
        };        
        

/****************************************************************************/
/*                   CPU REGISTERS: MMU                                     */
/****************************************************************************/
        union SPR_PID0VAL { 
            vuint32_t R;
            struct {
                vuint32_t :24;
                vuint32_t PID:8;
            } B;
        };

        union SPR_MMUCFGVAL { 
            vuint32_t R;
            struct {
                vuint32_t:17;
                vuint32_t NPIDS:4;
                vuint32_t PIDSIZE:5;
                vuint32_t:2;
                vuint32_t NLTBS:2;
                vuint32_t MAVN:2;
            } B;
        };

/****************************************************************************/
/*                   CPU REGISTERS: CACHE                                   */
/****************************************************************************/
        union SPR_L1CFG0VAL {    /* Read only register */
            vuint32_t R;
            struct {
                vuint32_t CARCH:2;  /* Cache Architecture; 01=Unified      */
                vuint32_t CWPA:1;   /* Cache way partitioning available =1 */
                vuint32_t CFAHA:1;  /* Cache Flush by all avail; 0=not     */
                vuint32_t CFIWA:1;  /* Cache Flush Inv by set & way avail=1 */
                vuint32_t :2;
                vuint32_t CBSIZE:2; /* Block Size 00=32 bytes         */
                vuint32_t CREPL:2;  /* Replacement Policy 10=pseudo round robin */
                vuint32_t CLA:1;    /* Line locking APU; 1=avail      */
                vuint32_t CPA:1;    /* Parity available  1=avail      */
                vuint32_t CNWAY:8;  /* Num of ways; 0x03=4way, 0x07=8way */
                vuint32_t CSIZE:11; /* Size; 0x008=8KB, 0x010=16KB,0x020=32KB */
            } B;
        };              /* Read only register */

/****************************************************************************/
/*                   CPU REGISTERS: APU                                     */
/****************************************************************************/
        
        /* NO SPE on MPC5510 */

/****************************************************************************/
/*                   CPU REGISTERS: Exception Handling/Control Registers    */
/****************************************************************************/
        union SPR_SPRGVAL {  /* There are [2] entries for this tag */
            vuint32_t R;
            struct {
                vuint32_t SPRDATA:32;
            } B;
        };

       union SPR_SRR0VAL { 
            vuint32_t R;
            struct {
                vuint32_t NXTADDR:32;
            } B;
        };

       union SPR_SRR1VAL { 
            vuint32_t R;
            struct {
                vuint32_t MSRSTATE:32;
            } B;
        };

        union SPR_CSRR0VAL { 
            vuint32_t R;
            struct {
                vuint32_t CRITNXTADDR:32;
            } B;
        };

       union SPR_CSRR1VAL { 
            vuint32_t R;
            struct {
                vuint32_t CRITMSRSTATE:32;
            } B;
        };

        union SPR_DSRR0VAL { 
            vuint32_t R;
            struct {
                vuint32_t DBGNXTADDR:32;
            } B;
        };

       union SPR_DSRR1VAL { 
            vuint32_t R;
            struct {
                vuint32_t DBGMSRSTATE:32;
            } B;
        };

       union SPR_DEARVAL { 
            vuint32_t R;
            struct {
                vuint32_t DATEFADDR:16;
                vuint32_t :16;
            } B;
        };

        union SPR_ESRVAL { 
            vuint32_t R;
            struct {
                vuint32_t :4;
                vuint32_t PIL:1;
                vuint32_t PPR:1;
                vuint32_t PTR:1;
                vuint32_t FP:1;
                vuint32_t ST:1;
                vuint32_t :1;
                vuint32_t DLK:1;
                vuint32_t ILK:1;
                vuint32_t AP:1;
                vuint32_t PUO:1;
                vuint32_t BO:1;
                vuint32_t PIE:1;
                vuint32_t :8;
                vuint32_t EFP:1;
                vuint32_t :1;
                vuint32_t VLEMI:1;
                vuint32_t :3;
                vuint32_t MIF:1;
                vuint32_t XTE:1;
            } B;
        };


        union SPR_MCSRVAL {  
            vuint32_t R;
            struct {
                vuint32_t MCP:1;
                vuint32_t :1;
                vuint32_t CP_PERR:1;
                vuint32_t CPERR:1;
                vuint32_t EXCP_ERR:1;
                vuint32_t :6;
                vuint32_t NMI:1;
                vuint32_t :15;
                vuint32_t BUS_IRERR:1;
                vuint32_t BUS_DRERR:1;
                vuint32_t BUS_WRERR:1;
                vuint32_t :2;
            } B;
        };

       union SPR_IVPRVAL { 
            vuint32_t R;
            struct {
                vuint32_t VECBASE:20;
                vuint32_t :12;
            } B;
        };


/****************************************************************************/
/*                   CPU REGISTERS: DEBUG                                     */
/****************************************************************************/
        union SPR_DBCR0VAL {
            vuint32_t R;
            struct { 
                vuint32_t EDM:1;
                vuint32_t IDM:1;
                vuint32_t RST:2;
                vuint32_t ICMP:1;
                vuint32_t BRT:1;
                vuint32_t IRPT:1;
                vuint32_t TRAP:1;
                vuint32_t IAC1:1;
                vuint32_t IAC2:1;
                vuint32_t IAC3:1;
                vuint32_t IAC4:1;
                vuint32_t DAC1:2;
                vuint32_t DAC2:2;
                vuint32_t RET:1;
                vuint32_t :4;
                vuint32_t DEVT1:1;
                vuint32_t DEVT2:1;
                vuint32_t :2;
                vuint32_t CIRPT:1;
                vuint32_t CRET:1;
                vuint32_t :5;
            } B;
        };

        union SPR_DBCR1VAL { 
            vuint32_t R;
            struct {
                vuint32_t IAC1US:2;
                vuint32_t IAC1ER:2;
                vuint32_t IAC2US:2;
                vuint32_t IAC2ER:2;
                vuint32_t IAC12M:2;
                vuint32_t :6;
                vuint32_t IAC3US:2;
                vuint32_t IAC3ER:2;
                vuint32_t IAC4US:2;
                vuint32_t IAC4ER:2;
                vuint32_t IAC34M:2;
                vuint32_t :6;
            } B;
        };

        union SPR_DBCR2VAL { 
            vuint32_t R;
            struct {
                vuint32_t DAC1US:2;
                vuint32_t DAC1ER:2;
                vuint32_t DAC2US:2;
                vuint32_t DAC2ER:2;
                vuint32_t DAC12M:2;
                vuint32_t DAC1LNK:2;
                vuint32_t DAC2LNK:2;
                vuint32_t :20;
            } B;
        };

        union SPR_DBSRVAL {  
            vuint32_t R;
            struct {
                vuint32_t IDE:1;
                vuint32_t UDE:1;
                vuint32_t MRR:2;
                vuint32_t ICMP:1;
                vuint32_t BRT:1;
                vuint32_t IRPT:1;
                vuint32_t TRAP:1;
                vuint32_t IAC1:1;
                vuint32_t IAC2:1;
                vuint32_t IAC3:1;
                vuint32_t IAC4:1;
                vuint32_t DAC1R:1;
                vuint32_t DAC1W:1;
                vuint32_t DAC2R:1;
                vuint32_t DAC2W:1;
                vuint32_t RET:1;
                vuint32_t :4;
                vuint32_t DEVT1:1;
                vuint32_t DEVT2:1;
                vuint32_t :2;
                vuint32_t CIRPT:1;
                vuint32_t CRET:1;
                vuint32_t VLES:1; 
                vuint32_t :4;
            } B;
        };

        union SPR_IAC1VAL {
            vuint32_t R;
            struct {
                vuint32_t INSTADDR:30;
                vuint32_t :2;
           } B;
        };

        union SPR_IAC2VAL {
            vuint32_t R;
            struct {
                vuint32_t INSTADDR:30;
                vuint32_t :2;
           } B;
        };

        union SPR_IAC3VAL {
            vuint32_t R;
            struct {
                vuint32_t INSTADDR:30;
                vuint32_t :2;
           } B;
        };

        union SPR_IAC4VAL {
            vuint32_t R;
            struct {
                vuint32_t INSTADDR:30;
                vuint32_t :2;
           } B;
        };


        union SPR_DAC1VAL {
            vuint32_t R;
            struct {
                vuint32_t DATTADDR:32;
           } B;
        };

        union SPR_DAC2VAL {
            vuint32_t R;
            struct {
                vuint32_t DATTADDR:32;
           } B;
        };



/*****************************************************/
/* Define instances of modules                       */
/*  with special register numbers                    */
/*****************************************************/

/* The CR register does not have an SPR#  */
/* The GPR registers do not have an SPR#  */
/* The MSR register does not have an SPR# */

#define SPR_LR          8
#define SPR_CTR         9
#define SPR_XER         1

#define SPR_PIR       286
#define SPR_PVR       287
#define SPR_SVR      1023
#define SPR_HID0     1008
#define SPR_HID1     1009

#define SPR_PID0       48
#define SPR_MMUCFG   1015

#define SPR_L1CFG0    515

#define SPR_SPRG0     272
#define SPR_SPRG1     273
#define SPR_SRR0       26
#define SPR_SRR1       27
#define SPR_CSRR0      58
#define SPR_CSRR1      59
#define SPR_DSRR0     574
#define SPR_DSRR1     575
#define SPR_ESR        62
#define SPR_MCSR      572
#define SPR_DEAR       61
#define SPR_IVPR       63

#define SPR_DBCR0     308
#define SPR_DBCR1     309
#define SPR_DBCR2     310
#define SPR_DBSR      304
#define SPR_IAC1      312
#define SPR_IAC2      313
#define SPR_IAC3      314
#define SPR_IAC4      315
#define SPR_DAC1      316
#define SPR_DAC2      317

#define SPR_BUCSR    1013 



#ifdef  __cplusplus
}
#endif

#endif  /* ends inclusion of #ifndef __MPC5510_SPR_Z0_ for one instantiation */

/*********************************************************************
 *
 * Copyright:
 *	Freescale Semiconductor, INC. All Rights Reserved.
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Freescale
 *  Semiconductor, Inc. This software is provided on an "AS IS"
 *  basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, Freescale
 *  Semiconductor DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED,
 *  INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
 *  PARTICULAR PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH
 *  REGARD TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF)
 *  AND ANY ACCOMPANYING WRITTEN MATERIALS.
 *
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL Freescale Semiconductor BE LIABLE FOR ANY DAMAGES WHATSOEVER
 *  (INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 *  BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER
 *  PECUNIARY LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.
 *
 *  Freescale Semiconductor assumes no responsibility for the
 *  maintenance and support of this software
 *
 ********************************************************************/


