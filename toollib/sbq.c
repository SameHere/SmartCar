#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "MyHeader.h"
//#include "LEDSWITCH.h"

//***************************虚拟示波器***********************************//
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;         //是特定的校验代码段吗？？
    unsigned char i,j;
    CRC_Temp = 0xffff;                 //

    for (i=0; i<CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];
        for (j=0; j<8; j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return CRC_Temp;
}

int OutData[4]= {0};
void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0; i<4; i++)
    {

        temp[i]  = (int)OutData[i];         //way??
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0; i<4; i++)
    {
        databuf[i*2] = (unsigned char)(temp1[i]%256);  //先发低八位
        databuf[i*2+1] =(unsigned char)(temp1[i]/256); //后发高八位？?
    }



    CRC16 = CRC_CHECK(databuf,8);//求校验码？？

    databuf[8] = CRC16%256;      //把校验码拆为高低位
    databuf[9] = CRC16/256;


    for(i=0; i<10; i++)
        SCI0_SendChar(databuf[i]);    //将十个数发出
}

//在主函数里调用
//调用时直接把要显示的值赋给a、b、c、d
void sbq(int16_t a, int16_t b, int16_t c, int16_t d)
{
    OutData[0]=a;
    OutData[1]=b;
    OutData[2]=c;
    OutData[3]=d;
    OutPut_Data();
}