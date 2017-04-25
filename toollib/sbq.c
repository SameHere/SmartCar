#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "MyHeader.h"
//#include "LEDSWITCH.h"

//***************************����ʾ����***********************************//
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;         //���ض���У�������𣿣�
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
        databuf[i*2] = (unsigned char)(temp1[i]%256);  //�ȷ��Ͱ�λ
        databuf[i*2+1] =(unsigned char)(temp1[i]/256); //�󷢸߰�λ��?
    }



    CRC16 = CRC_CHECK(databuf,8);//��У���룿��

    databuf[8] = CRC16%256;      //��У�����Ϊ�ߵ�λ
    databuf[9] = CRC16/256;


    for(i=0; i<10; i++)
        SCI0_SendChar(databuf[i]);    //��ʮ��������
}

//�������������
//����ʱֱ�Ӱ�Ҫ��ʾ��ֵ����a��b��c��d
void sbq(int16_t a, int16_t b, int16_t c, int16_t d)
{
    OutData[0]=a;
    OutData[1]=b;
    OutData[2]=c;
    OutData[3]=d;
    OutPut_Data();
}