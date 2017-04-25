/*******************************(C) Copyright 2013 智能车团队 ***************************************/
//    文件名:     SCI_drive.C
//    描述:       SCI初始化,SCI收、发子程序
//    平台:       MPC5604B_M27V
//    作者:       吕科
/****************************************************************************************************/
#include "MPC5604B_0M27V_0102.h" // Use proper include file 
#include "MyHeader.h" 
//----------------------------------------------------------------------------------------------------
//      SCI0 初始化
//----------------------------------------------------------------------------------------------------
void initSCI(void)
{
    SIU.PCR[PCR18_PB2].R = 0x0403;  //Use PB[2] for lin_0 TX pin
    SIU.PCR[PCR19_PB3].R = 0x0103;  //Use PB[3] for lin_0 RX pin
//----------------------------------------------------------- 
//        LIN control register 1   (LINFLEX_X_LINCR1)        
//----------------------------------------------------------- 
    LINFLEX_0.LINCR1.B.INIT = 1;
        //Initialization Request: Set
                        
    LINFLEX_0.LINCR1.B.SLEEP = 0;
        //Sleep Request: Clear 

    LINFLEX_0.UARTCR.B.UART= 1;
        // UART Mode: Enabled 

//----------------------------------------------------------- 
//        LIN control register 2   (LINFLEX_X_LINCR2)        
//----------------------------------------------------------- 

    LINFLEX_0.LINCR2.R = 0x0000;
        // Idle on Bit Error: Disabled
        // Idle on Identifier Parity Error: Disabled


//----------------------------------------------------------- 
//        LIN integer baudrate register  (LINFLEX_X_LINIBRR)        
//----------------------------------------------------------- 
    LINFLEX_0.LINIBRR.R = 0x0022;
        // Integer Baud Rate Factor: 34

//----------------------------------------------------------- 
//        LIN fractional baudrate register  (LINFLEX_X_LINFBRR)        
//----------------------------------------------------------- 
    LINFLEX_0.LINFBRR.R = 0x000C;
        // Fractional Baud Rate Factor: 12

//----------------------------------------------------------- 
//        Identifier filter mode register  (LINFLEX_X_IFMR)         
//----------------------------------------------------------- 

    LINFLEX_0.IFMR.R = 0x0000;
        // Filters 0:identifier list mode    
        // Filters 1:identifier list mode    
        // Filters 2:identifier list mode    
        // Filters 3:identifier list mode    
        // Filters 4:identifier list mode    
        // Filters 5:identifier list mode    
        // Filters 6:identifier list mode    
        // Filters 7:identifier list mode    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR0)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[0].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR1)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[1].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR2)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[2].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR3)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[3].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR4)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[4].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR5)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[5].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR6)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[6].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//    Identifier filter control register  (LINFLEX_0_IFCR7)    
//----------------------------------------------------------- 
    LINFLEX_0.IFCR[7].R = 0x0000;
            // Data Field Length (Bytes): 1    
            // Direction: Receive    
            // Checksum: Enhanced    
            // Identifier: 0b0    

//----------------------------------------------------------- 
//        Identifier filter enable register  (LINFLEX_X_IFER)         
//----------------------------------------------------------- 

    LINFLEX_0.IFER.R = 0x0000;
        // Activate Filter 0:Disabled    
        // Activate Filter 1:Disabled    
        // Activate Filter 2:Disabled    
        // Activate Filter 3:Disabled    
        // Activate Filter 4:Disabled    
        // Activate Filter 5:Disabled    
        // Activate Filter 6:Disabled    
        // Activate Filter 7:Disabled    

//----------------------------------------------------------- 
//        UART mode control register  (LINFLEX_X_UARTCR)        
//----------------------------------------------------------- 
    LINFLEX_0.UARTCR.R = 0x0033;
        // UART Mode: Enable
        // Parity transmit/check: Disable
        // Sent parity: Even
        // Word Length in UART mode: 8 bit data (9 bit if PCE is set)
        // Transmitter Data Field Length (Bytes): 1
        // Receiver Data Field Length (Bytes): 1
        // Transmitter : Enable
        // Receiver : Enable


//----------------------------------------------------------- 
//        LIN timeout control status register  (LINFLEX_X_LINTCSR)        
//----------------------------------------------------------- 

    LINFLEX_0.LINTCSR.R = 0x0000;
        // LIN timeout mode: Disabled
        // Idle on Timeout: Disabled
        // Timeout Counter: Disabled


//----------------------------------------------------------- 
//        LIN output compare register  (LINFLEX_X_LINOCR)        
//----------------------------------------------------------- 

    LINFLEX_0.LINOCR.R = 0xFFFF;
        // Output compare Value 1: 255
        // Output compare Value 2: 255

//----------------------------------------------------------- 
//        LIN timeout control register  (LINFLEX_X_LINTOCR)        
//----------------------------------------------------------- 

    LINFLEX_0.LINTOCR.R = 0x0E2C;
        // Header Timeout (Bit Time): 44
        // Response Timeout: 14

//----------------------------------------------------------- 
//        LIN interrupt enable register  (LINFLEX_X_LINIER)        
//----------------------------------------------------------- 

    LINFLEX_0.LINIER.R = 0x0000;

        // Header Received Interrupt: Disabled    
        // Data Transmitted Interrupt: Disabled    
        // Data Reception Complete Interrupt: Disabled    
        // Data Buffer Empty Interrupt: Disabled    
        // Data Buffer Full Interrupt: Disabled    
        // Wakeup Interrupt: Disabled    
        // LIN State Interrupt: Disabled    
        // Buffer Overrun Error Interrupt: Disabled    
        // Frame Error Interrupt: Disabled    
        // Header Error Interrupt: Disabled    
        // Checksum Error Interrupt: Disabled    
        // Bit Error Interrupt: Disabled    
        // Output Compare Interrupt: Disabled    
        // Stuck at Zero Interrupt: Disabled    

//----------------------------------------------------------- 
//        LIN control register 1   (LINFLEX_X_LINCR1)        
//----------------------------------------------------------- 

    LINFLEX_0.LINCR1.R = 0x4001;
        // Initialization Request: Set 
        // Master Mode Enable: Slave
        // Receiver Buffer: Not Locked
        // Loopback Mode: Disabled
        // Self Test Mode: Disabled
        // LIN Master Break Length: 10 bits
        // Slave Mode Break Detection Threshold: 11 bits
        // Bypass Filter: Disabled
        // Automatic Wake-Up Mode: Disabled
        // Checksum field: Disabled
        // Checksum Calculation: Disabled
        // LIN Slave Automatic Resynchronization: Disabled



    LINFLEX_0.LINCR1.B.INIT = 0;
        //Initialization Request: Clear 
        
    
    LINFLEX_0.UARTSR.B.DTF = 1;              // Clear Data Transmission Completed Flag flag 


}
//----------------------------------------------------------------------------------------------------
//       SCI0 发送子程序
//----------------------------------------------------------------------------------------------------
void SCI0_SendChar(uint8_t Chr)
{
    LINFLEX_0.BDRL.B.DATA0 = Chr;           // Transmit 8 bits Data 
    while (LINFLEX_0.UARTSR.B.DTF == 0){}   // Wait for Data Transmission Completed Flag = 1
    LINFLEX_0.UARTSR.B.DTF = 1;             // Clear Data Transmission Completed Flag flag
}
//----------------------------------------------------------------------------------------------------
//       SCI0 发送子程序  两字节
//----------------------------------------------------------------------------------------------------
void SCI0_SendChar_16(uint16_t Chr)
{
    uint8_t *p;
    p=(uint8_t *)&Chr;
    LINFLEX_0.BDRL.B.DATA0 = *p;           // Transmit 8 bits Data 
    while (LINFLEX_0.UARTSR.B.DTF == 0){}   // Wait for Data Transmission Completed Flag = 1
    LINFLEX_0.UARTSR.B.DTF = 1;             // Clear Data Transmission Completed Flag flag
    
    LINFLEX_0.BDRL.B.DATA0 = *(p+1);           // Transmit 8 bits Data 
    while (LINFLEX_0.UARTSR.B.DTF == 0){}   // Wait for Data Transmission Completed Flag = 1
   LINFLEX_0.UARTSR.B.DTF = 1;             // Clear Data Transmission Completed Flag flag 
}
//----------------------------------------------------------------------------------------------------
//       SCI0 接收子程序
//----------------------------------------------------------------------------------------------------
uint8_t SCI0_RecvChar(uint8_t *Chr)
{
    if (LINFLEX_0.UARTSR.B.RMB == 0) 
    return 0;                                // Wait for Data Reception Completed Flag = 1 
    *Chr = (uint8_t) LINFLEX_0.BDRM.B.DATA4; // Read byte of Data
    LINFLEX_0.UARTSR.B.RMB = 1;              // Clear Release Message Buffer flag 
    return 1;
}


