#ifndef  _MYHEA_H
#define  _MYHEA_H

#define    RUN_PC0    0X00   //000 Selects ME_RUN_PC0 configuration    
#define    RUN_PC1    0X01   //001 Selects ME_RUN_PC1 configuration
#define    RUN_PC2    0X02   //010 Selects ME_RUN_PC2 configuration
#define    RUN_PC3    0X03   //011 Selects ME_RUN_PC3 configuration
#define    RUN_PC4    0X04   //100 Selects ME_RUN_PC4 configuration    
#define    RUN_PC5    0X05   //101 Selects ME_RUN_PC5 configuration
#define    RUN_PC6    0X06   //110 Selects ME_RUN_PC6 configuration
#define    RUN_PC7    0X07   //111 Selects ME_RUN_PC7 configuration
//--------------------------------------------------------------------------------------------
#define    LP_PC0     0X00    //configuration    
#define    LP_PC1     0X01    //001 Selects configuration
#define    LP_PC2     0X02    //010 Selects ME_configuration
#define    LP_PC3     0X03    //011 Selects ME_configuration
#define    LP_PC4     0X04    //100 Selects ME_configuration    
#define    LP_PC5     0X05    //101 Selects ME_configuration
#define    LP_PC6     0X06    //110 Selects ME_configuration
#define    LP_PC7     0X07    //111 Selects ME_configuration

//****************************************************************************************
#define    PCTL4_DSPI0       4    //Control byte read/write on page 134*/
#define    PCTL5_DSPI1       5    //Control byte read/write on page 134
#define    PCTL6_DSPI2       6    //Control byte read/write on page 134
#define    PCTL16_FlexCAN0   16   //Control byte read/write on page 134
#define    PCTL17_FlexCAN1   17   //Control byte read/write on page 134
#define    PCTL18_FlexCAN2   18   //Control byte read/write on page 134
#define    PCTL19_FlexCAN3   19   //Control byte read/write on page 134
#define    PCTL20_FlexCAN4   20   //Control byte read/write on page 134
#define    PCTL21_FlexCAN5   21   //Control byte read/write on page 134
#define    PCTL32_ADC0       32   //Control byte read/write on page 134
#define    PCTL44_I2C0       44   //Control byte read/write on page 134
#define    PCTL48_LINFlex0   48   //Control byte read/write on page 134
#define    PCTL49_LINFlex1   49   //Control byte read/write on page 134
#define    PCTL50_LINFlex2   50   //Control byte read/write on page 134
#define    PCTL51_LINFlex3   51   //Control byte read/write on page 134
#define    PCTL57_CTU        57   //Control byte read/write on page 134
#define    PCTL60_CANSampler 60   //Control byte read/write on page 134
#define    PCTL68_SIUL       68   //Control byte read/write on page 134
#define    PCTL69_WKPU       69   //Control byte read/write on page 134
#define    PCTL72_eMIOS0     72   //Control byte read/write on page 134
#define    PCTL73_eMIOS1     73   //Control byte read/write on page 134
#define    PCTL91_RTC_API    91   //Control byte read/write on page 134
#define    PCTL92_PIT_RTI    92   //Control byte read/write on page 134
#define    PCTL104_CMU       104  //Control byte read/write on page 134
//------------------------------------------------------------
#define    DC0_LIN_I2C 0 //CGM_SC_DC0 System Clock Divider Configuration 0
#define    DC1_CAN_SPI 1 //CGM_SC_DC0 System Clock Divider Configuration 1
#define    DC2_MIOS_AD_CTU 2 //CGM_SC_DC0 System Clock Divider Configuration 2
//------------------------------------------------------------

//******************引脚号与引脚名称***********************************
//Defines port pins vs the relevant SIU port number  for MPc5606S  144 Pins LQFP
//Defines port pins vs the relevant SIU port number  for MPc5606S  144 Pins LQFP    
#define    PCR0_PA0          0    /*100 Pins LQFP is 12 ;   144 Pins LQFP is 16   */
#define    PCR1_PA1          1    /*100 Pins LQFP is 7  ;   144 Pins LQFP is 11   */
#define    PCR2_PA2          2    /*100 Pins LQFP is 5  ;   144 Pins LQFP is 9    */
#define    PCR3_PA3          3    /*100 Pins LQFP is 68 ;   144 Pins LQFP is 90   */
#define    PCR4_PA4          4    /*100 Pins LQFP is 29 ;   144 Pins LQFP is 43   */
#define    PCR5_PA5          5    /*100 Pins LQFP is 79 ;   144 Pins LQFP is 118  */
#define    PCR6_PA6          6    /*100 Pins LQFP is 80 ;   144 Pins LQFP is 119  */
#define    PCR7_PA7          7    /*100 Pins LQFP is 71 ;   144 Pins LQFP is 104  */
#define    PCR8_PA8          8    /*100 Pins LQFP is 72 ;   144 Pins LQFP is 105  */
#define    PCR9_PA9          9    /*100 Pins LQFP is 73 ;   144 Pins LQFP is 106  */
#define    PCR10_PA10        10   /*100 Pins LQFP is 74 ;   144 Pins LQFP is 107  */
#define    PCR11_PA11        11   /*100 Pins LQFP is 75 ;   144 Pins LQFP is 108  */
#define    PCR12_PA12        12   /*100 Pins LQFP is 31 ;   144 Pins LQFP is 45   */
#define    PCR13_PA13        13   /*100 Pins LQFP is 30 ;   144 Pins LQFP is 44   */
#define    PCR14_PA14        14   /*100 Pins LQFP is 28 ;   144 Pins LQFP is 42   */
#define    PCR15_PA15        15   /*100 Pins LQFP is 27 ;   144 Pins LQFP is 40   */
                                               
#define    PCR16_PB0         16   /*100 Pins LQFP is 23 ;   144 Pins LQFP is 31   */
#define    PCR17_PB1         17   /*100 Pins LQFP is 24 ;   144 Pins LQFP is 32   */
#define    PCR18_PB2         18   /*100 Pins LQFP is 100;   144 Pins LQFP is 144  */
#define    PCR19_PB3         19   /*100 Pins LQFP is 1  ;   144 Pins LQFP is 1    */
#define    PCR20_PB4         20   /*100 Pins LQFP is 50 ;   144 Pins LQFP is 72   */
#define    PCR21_PB5         21   /*100 Pins LQFP is 53 ;   144 Pins LQFP is 75   */
#define    PCR22_PB6         22   /*100 Pins LQFP is 54 ;   144 Pins LQFP is 76   */
#define    PCR23_PB7         23   /*100 Pins LQFP is 55 ;   144 Pins LQFP is 77   */
#define    PCR24_PB8         24   /*100 Pins LQFP is 39 ;   144 Pins LQFP is 53   */
#define    PCR25_PB9         25   /*100 Pins LQFP is 38 ;   144 Pins LQFP is 52   */
#define    PCR26_PB10        26   /*100 Pins LQFP is 40 ;   144 Pins LQFP is 54   */
#define    PCR27_PB11        27   /*100 Pins LQFP is 59 ;   144 Pins LQFP is 81   */
#define    PCR28_PB12        28   /*100 Pins LQFP is 61 ;   144 Pins LQFP is 83   */
#define    PCR29_PB13        29   /*100 Pins LQFP is 63 ;   144 Pins LQFP is 85   */
#define    PCR30_PB14        30   /*100 Pins LQFP is 65 ;   144 Pins LQFP is 87   */
#define    PCR31_PB15        31   /*100 Pins LQFP is 67 ;   144 Pins LQFP is 89   */
           
#define    PCR32_PC0         32   /*100 Pins LQFP is 87 ;  144 Pins LQFP is 126   */
#define    PCR33_PC1         33   /*100 Pins LQFP is 82 ;  144 Pins LQFP is 121   */
#define    PCR34_PC2         34   /*100 Pins LQFP is 78 ;  144 Pins LQFP is 117   */
#define    PCR35_PC3         35   /*100 Pins LQFP is 77 ;  144 Pins LQFP is 116   */
#define    PCR36_PC4         36   /*100 Pins LQFP is 92 ;  144 Pins LQFP is 131   */
#define    PCR37_PC5         37   /*100 Pins LQFP is 91 ;  144 Pins LQFP is 130   */
#define    PCR38_PC6         38   /*100 Pins LQFP is 25 ;  144 Pins LQFP is 36    */
#define    PCR39_PC7         39   /*100 Pins LQFP is 26 ;  144 Pins LQFP is 37    */
#define    PCR40_PC8         40   /*100 Pins LQFP is 99 ;  144 Pins LQFP is 143   */
#define    PCR41_PC9         41   /*100 Pins LQFP is 2  ;  144 Pins LQFP is 2     */
#define    PCR42_PC10        42   /*100 Pins LQFP is 22 ;  144 Pins LQFP is 28    */
#define    PCR43_PC11        43   /*100 Pins LQFP is 21 ;  144 Pins LQFP is 27    */
#define    PCR44_PC12        44   /*100 Pins LQFP is 97 ;  144 Pins LQFP is 141   */
#define    PCR45_PC13        45   /*100 Pins LQFP is 98 ;  144 Pins LQFP is 142   */
#define    PCR46_PC14        46   /*100 Pins LQFP is 3  ;  144 Pins LQFP is 3     */
#define    PCR47_PC15        47   /*100 Pins LQFP is 4  ;  144 Pins LQFP is 4     */
           
#define    PCR48_PD0         48   /*100 Pins LQFP is 41 ;  144 Pins LQFP is 63    */ 
#define    PCR49_PD1         49   /*100 Pins LQFP is 42 ;  144 Pins LQFP is 64    */ 
#define    PCR50_PD2         50   /*100 Pins LQFP is 43 ;  144 Pins LQFP is 65    */ 
#define    PCR51_PD3         51   /*100 Pins LQFP is 44 ;  144 Pins LQFP is 66    */ 
#define    PCR52_PD4         52   /*100 Pins LQFP is 45 ;  144 Pins LQFP is 67    */ 
#define    PCR53_PD5         53   /*100 Pins LQFP is 46 ;  144 Pins LQFP is 68    */ 
#define    PCR54_PD6         54   /*100 Pins LQFP is 47 ;  144 Pins LQFP is 69    */ 
#define    PCR55_PD7         55   /*100 Pins LQFP is 48 ;  144 Pins LQFP is 70    */ 
#define    PCR56_PD8         56   /*100 Pins LQFP is 49 ;  144 Pins LQFP is 71    */ 
#define    PCR57_PD9         57   /*100 Pins LQFP is 56 ;  144 Pins LQFP is 78    */ 
#define    PCR58_PD10        58   /*100 Pins LQFP is 57 ;  144 Pins LQFP is 79    */ 
#define    PCR59_PD11        59   /*100 Pins LQFP is 58 ;  144 Pins LQFP is 80    */ 
#define    PCR60_PD12        60   /*100 Pins LQFP is 60 ;  144 Pins LQFP is 82    */ 
#define    PCR61_PD13        61   /*100 Pins LQFP is 62 ;  144 Pins LQFP is 84    */ 
#define    PCR62_PD14        62   /*100 Pins LQFP is 64 ;  144 Pins LQFP is 86    */ 
#define    PCR63_PD15        63   /*100 Pins LQFP is 66 ;  144 Pins LQFP is 88    */ 
           
#define    PCR64_PE0         64   /*100 Pins LQFP is 6  ;  144 Pins LQFP is 10    */
#define    PCR65_PE1         65   /*100 Pins LQFP is 8  ;  144 Pins LQFP is 12    */
#define    PCR66_PE2         66   /*100 Pins LQFP is 89 ;  144 Pins LQFP is 128   */
#define    PCR67_PE3         67   /*100 Pins LQFP is 90 ;  144 Pins LQFP is 129   */
#define    PCR68_PE4         68   /*100 Pins LQFP is 93 ;  144 Pins LQFP is 132   */
#define    PCR69_PE5         69   /*100 Pins LQFP is 94 ;  144 Pins LQFP is 133   */
#define    PCR70_PE6         70   /*100 Pins LQFP is 95 ;  144 Pins LQFP is 139   */
#define    PCR71_PE7         71   /*100 Pins LQFP is 96 ;  144 Pins LQFP is 140   */
#define    PCR72_PE8         72   /*100 Pins LQFP is 9  ;  144 Pins LQFP is 13    */
#define    PCR73_PE9         73   /*100 Pins LQFP is 10 ;  144 Pins LQFP is 14    */
#define    PCR74_PE10        74   /*100 Pins LQFP is 11 ;  144 Pins LQFP is  15   */
#define    PCR75_PE11        75   /*100 Pins LQFP is 13 ;  144 Pins LQFP is  17   */
#define    PCR76_PE12        76   /*100 Pins LQFP is 76 ;  144 Pins LQFP is  109  */
#define    PCR77_PE13        77   /*                    ;  144 Pins LQFP is  103  */
#define    PCR78_PE14        78   /*                    ;  144 Pins LQFP is  112  */
#define    PCR79_PE15        79   /*                    ;  144 Pins LQFP is  113  */
//------------------------------------------------------           
#define    PCR80_PF0         80   /*  144 Pins LQFP is 55                         */ 
#define    PCR81_PF1         81   /*  144 Pins LQFP is 56                         */ 
#define    PCR82_PF2         82   /*  144 Pins LQFP is 57                         */ 
#define    PCR83_PF3         83   /*  144 Pins LQFP is 58                         */ 
#define    PCR84_PF4         84   /*  144 Pins LQFP is 59                         */ 
#define    PCR85_PF5         85   /*  144 Pins LQFP is 60                         */ 
#define    PCR86_PF6         86   /*  144 Pins LQFP is 61                         */ 
#define    PCR87_PF7         87   /*  144 Pins LQFP is 62                         */ 
#define    PCR88_PF8         88   /*  144 Pins LQFP is 34                         */ 
#define    PCR89_PF9         89   /*  144 Pins LQFP is 33                         */ 
#define    PCR90_PF10        90   /*  144 Pins LQFP is 38                         */ 
#define    PCR91_PF11        91   /*  144 Pins LQFP is 39                         */ 
#define    PCR92_PF12        92   /*  144 Pins LQFP is 35                         */ 
#define    PCR93_PF13        93   /*  144 Pins LQFP is 41                         */ 
#define    PCR94_PF14        94   /*  144 Pins LQFP is 102                        */ 
#define    PCR95_PF15        95   /*  144 Pins LQFP is 101                        */ 
                
#define    PCR96_PG0         96   /*  144 Pins LQFP is 98                         */
#define    PCR97_PG1         97   /*  144 Pins LQFP is 97                         */
#define    PCR98_PG2         98   /*  144 Pins LQFP is 8                          */
#define    PCR99_PG3         99   /*  144 Pins LQFP is 7                          */
#define    PCR100_PG4        100  /*  144 Pins LQFP is 6                          */

#define    PCR101_PG5        101  /*  144 Pins LQFP is 5                          */
#define    PCR102_PG6        102  /*  144 Pins LQFP is 30                         */
#define    PCR103_PG7        103  /*  144 Pins LQFP is 29                         */
#define    PCR104_PG8        104  /*  144 Pins LQFP is 26                         */
#define    PCR105_PG9        105  /*  144 Pins LQFP is 25                         */
#define    PCR106_PG10       106  /*  144 Pins LQFP is 114                        */
#define    PCR107_PG11       107  /*  144 Pins LQFP is 115                        */
#define    PCR108_PG12       108  /*  144 Pins LQFP is 92                         */
#define    PCR109_PG13       109  /*  144 Pins LQFP is 91                         */
#define    PCR110_PG14       110  /*  144 Pins LQFP is 110                        */
#define    PCR111_PG15       111  /*  144 Pins LQFP is 111                        */
                  
#define    PCR112_PH0        112  /*  144 Pins LQFP is 93                         */
#define    PCR113_PH1        113  /*  144 Pins LQFP is 94                         */
#define    PCR114_PH2        114  /*  144 Pins LQFP is 95                         */
#define    PCR115_PH3        115  /*  144 Pins LQFP is 96                         */
#define    PCR116_PH4        116  /*  144 Pins LQFP is 134                        */
#define    PCR117_PH5        117  /*  144 Pins LQFP is 135                        */
#define    PCR118_PH6        118  /*  144 Pins LQFP is 136                        */
#define    PCR119_PH7        119  /*  144 Pins LQFP is 137                        */
#define    PCR120_PH8        120  /*  144 Pins LQFP is 138                        */
#define    PCR121_PH9        121  /*  144 Pins LQFP is 127                        */
#define    PCR122_PH10       122  /*  144 Pins LQFP is 120                        */            

//TOTAL IS 123   中断请求
//----------------------------------------------------------------------------
/**************************************************************************************************/
#define    IRQ0_Software0    0    //0x0800 4 Software configurable flag 0 Software
#define    IRQ1_Software1    1    //0x0804 4 Software configurable flag 1 Software
#define    IRQ2_Software2    2    //0x0808 4 Software configurable flag 2 Software
#define    IRQ3_Software3    3    //0x080C 4 Software configurable flag 3 Software
#define    IRQ4_Software4    4    //0x0810 4 Software configurable flag 4 Software
#define    IRQ5_Software5    5    //0x0814 4 Software configurable flag 5 Software
#define    IRQ6_Software6    6    //0x0818 4 Software configurable flag 6 Software
#define    IRQ7_Software7    7    //0x081C 4 Software configurable flag 7 Software
  //#define IRQ8 8 0x0820 4 Reserved
#define    IRQ9_ECSM         9    //0x0824 4 Platform Flash Bank 0/1/2/3 Abort/STALL
//#define IRQ10 10 0x0828 4 Reserved
//#define IRQ11 11 0x082C 4 Reserved
//#define IRQ12 12 0x0830 4 Reserved
//#define IRQ13 13 0x0834 4 Reserved
//#define IRQ14 14 0x0838 4 Reserved
//#define IRQ15 15 0x083C 4 Reserved
//#define IRQ16 16 0x0840 4 Reserved
//#define IRQ17 17 0x0844 4 Reserved
//#define IRQ18 18 0x0848 4 Reserved
//#define IRQ19 19 0x084C 4 Reserved
//#define IRQ20 20 0x0850 4 Reserved
//#define IRQ21 21 0x0854 4 Reserved
//#define IRQ22 22 0x0858 4 Reserved
//#define IRQ23 23 0x085C 4 Reserved
//#define IRQ24 24 0x0860 4 Reserved
//#define IRQ25 25 0x0864 4 Reserved
//#define IRQ26 26 0x0868 4 Reserved
//#define IRQ27 27 0x086C 4 Reserved
#define    IRQ28_SWT         28   //0x0870 4 Timeout Software Watchdog (SWT)
//#define IRQ29 29 0x0874 4 Reserved
#define    IRQ30_STM0        30   //0x0878 4 Match on channel 0 STM
#define    IRQ31_STM1        31   //0x087C 4 Match on channel 1 STM
#define    IRQ32_STM2        32   //0x0880 4 Match on channel 2 STM
#define    IRQ33_STM3        33   //0x0884 4 Match on channel 3 STM
//#define IRQ34 34 0x0888 4 Reserved
#define    IRQ35_ECCD        35   //0x088C 4 ECC_DBD_PlatformFlash/RAM/ Platform ECC Double Bit Detection
#define    IRQ36_SCCD        36   //0x0890 4 ECC_SBC_PlatformFlash/mRAM=Platform ECC Single Bit Correction
//#define IRQ37 37 0x0894 4 Reserved
#define    IRQ38_RTC         38   //0x0898 4 RTC Real Time Counter (RTC/API)
#define    IRQ39_API         39   //0x089C 4 API Real Time Counter (RTC/API)
//#define IRQ40 40 0x08A0 4 Reserved
#define    IRQ41_SIUIRQ0     41   //0x08A4 4 SIU External IRQ_0 System Integration Unit Lite (SIUL)
#define    IRQ42_SIUIRQ1     42   //0x08A8 4 SIU External IRQ_1 System Integration Unit Lite (SIUL)
//#define IRQ43 43 0x08AC 4 Reserved
//#define IRQ44 44 0x08B0 4 Reserved
//#define IRQ45 45 0x08B4 4 Reserved
#define    IRQ46_WKUP0       46   //0x08B8 4 WakeUp_IRQ_0 WakeUp Unit (WKUP)
#define    IRQ47_WKUP1       47   //0x08BC 4 WakeUp_IRQ_1 WakeUp Unit (WKUP)
#define    IRQ48_WKUP2       48   //0x08C0 4 WakeUp_IRQ_2 WakeUp Unit (WKUP)
//#define IRQ49 49 0x08C4 4 Reserved
//#define IRQ50 50 0x08C8 4 Reserved
#define    IRQ51_MC_ME0      51   //0x08CC 4 Safe Mode Interrupt MC_ME
#define    IRQ52_MC_ME1      52   //0x08D0 4 Mode Transition Interrupt MC_ME
#define    IRQ53_MC_ME2      53   //0x08D4 4 Invalid Mode Interrupt MC_ME
#define    IRQ54_MC_ME3      54   //0x08D8 4 Invalid Mode Config MC_ME
//#define IRQ55 55 0x08DC 4 Reserved
#define    IRQ56_MC_RGM      56   //0x08E0 4 Functional and destructive reset alternate event interrupt (ipi_int)
#define    IRQ57_FXOSC       57   //0x08E4 4 FXOSC counter expired (ipi_int_osc) FXOSC
//#define IRQ58 58 0x08E8 4 Reserved
#define    IRQ59_PIT0         59   //0x08EC 4 PITimer Channel 0 Periodic Interrupt Timer (PIT)
#define    IRQ60_PIT1         60   //0x08F0 4 PITimer Channel 1 Periodic Interrupt Timer (PIT)
#define    IRQ61_PIT2         61   //0x08F4 4 PITimer Channel 2 Periodic Interrupt Timer (PIT)
#define    IRQ62_ADC_EOC      62   //0x08F8 4 ADC_EOC Analog to Digital Converter 0 (ADC0)
#define    IRQ63_ADC_ER       63   //0x08FC 4 ADC_ER Analog to Digital Converter 0 (ADC0)
#define    IRQ64_ADC_WD       64   //0x0900 4 ADC_WD Analog to Digital Converter 0 (ADC0)
#define    IRQ65_CAN0_ESR     65   //0x0904 4 FlexCAN_ESR[ERR_INT] FlexCAN 0 (CAN0)
#define    IRQ66_CAN0_ESR_BOF 66   //0x0908 4 FlexCAN_ESR_BOFF/FlexCAN_Transmit_Warning/FlexCAN_Receive_Warning=FlexCAN 0 (CAN0)
//#define IRQ67 67 0x090C 4 Reserved
#define   IRQ68_CAN0_BUF0TO3   68  //0x0910 4 FlexCAN_BUF_00_03 FlexCAN 0 (CAN0)
#define   IRQ69_CAN0_BUF4TO7   69  //0x0914 4 FlexCAN_BUF_04_07 FlexCAN 0 (CAN0)
#define   IRQ70_CAN0_BUF8TO11  70  //0x0918 4 FlexCAN_BUF_08_11 FlexCAN 0 (CAN0)
#define   IRQ71_CAN0_BUF12TO15 71  //0x091C 4 FlexCAN_BUF_12_15 FlexCAN 0 (CAN0)
#define   IRQ72_CAN0_BUF16TO31 72  //0x0920 4 FlexCAN_BUF_16_31 FlexCAN 0 (CAN0)
#define   IRQ73_CAN0_BUF32TO63 73  //0x0924 4 FlexCAN_BUF_32_63 FlexCAN 0 (CAN0)
#define   IRQ74_DSPI0_TFUF     74  //0x0928 4 DSPI_SR[TFUF]/DSPI_SR[RFOF]=DSPI_0
#define   IRQ75_DSPI0_EOQF     75  //0x092C 4 DSPI_SR[EOQF] DSPI_0
#define   IRQ76_DSPI0_TFFF     76  //0x0930 4 DSPI_SR[TFFF] DSPI_0
#define   IRQ77_DSPI0_TCF      77  //0x0934 4 DSPI_SR[TCF] DSPI_0
#define   IRQ78_DSPI0_RFDF     78  //0x0938 4 DSPI_SR[RFDF] DSPI_0
#define   IRQ79_LIN0_RX        79  //0x093C 4 LINFlex_RXI LINFlex_0
#define   IRQ80_LIN0_TXI       80  //0x0940 4 LINFlex_TXI LINFlex_0
#define   IRQ81_LIN0_ER        81  //0x0944 4 LINFlex_ERR LINFlex_0
//#define IRQ82 82 0x0948 4 Reserved
//#define IRQ83 83 0x094C 4 Reserved
//#define IRQ84 84 0x0950 4 Reserved
#define   IRQ85_CAN1_ESR       85  //0x0954 4 FlexCAN_ESR[ERR_INT] FlexCAN 1 (CAN1)
#define   IRQ86_CAN1_ESR_BO    86  //0x0958 4 FlexCAN_ESR_BOFF/FlexCAN_Transmit_Warning/FlexCAN_Receive_Warning=FlexCAN 1 (CAN1)
//#define IRQ87 87 0x095C 4 Reserved
#define   IRQ88_CAN1_BUF0TO3   88 0x0960 4 FlexCAN_BUF_00_03 FlexCAN 1 (CAN1)
#define   IRQ89_CAN1_BUF4TO7   89 0x0964 4 FlexCAN_BUF_04_07 FlexCAN 1 (CAN1)
#define   IRQ90_CAN1_BUF8TO11  90 0x0968 4 FlexCAN_BUF_08_11 FlexCAN 1 (CAN1)

#endif
