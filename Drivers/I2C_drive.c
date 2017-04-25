/*******************************(C) Copyright 2013 ���ܳ��Ŷ� ***************************************/
//    �ļ���:     I2C.C
//    ����:       I2C��ʼ��,I2C�ա����ӳ���
//    ƽ̨:       MPC5605B
//    ����:       ����
/****************************************************************************************************/
#include "MPC5605B.h" // Use proper include file 
#include "MyHeader.h"
#define  SCL_out    SIU.GPDO[PCR11_PA11].R
#define  SDA_out    SIU.GPDO[PCR10_PA10].R
#define  SCL_in     SIU.GPDI[PCR11_PA11].R
#define  SDA_in     SIU.GPDI[PCR10_PA10].R
#define  SCL_port_direction    SIU.PCR[PCR11_PA11].R
#define  SDA_port_direction    SIU.PCR[PCR10_PA10].R
#define  input      0x0103
#define  output     0x0200
#define  master_ack     0x01
#define  no_master_ack     0x00
//----------------------------------------------------------------------------------------------------
//      I2C ��ʼ��
//----------------------------------------------------------------------------------------------------
void initI2C(void)
{
    SCL_port_direction = output;
    SDA_port_direction = output;
     
}
//----------------------------------------------------------------------------------------------------
//       I2C ��ʼ�ź�
//----------------------------------------------------------------------------------------------------
void I2C_start(void)
{
    uint8_t i;
    
    SCL_out=0;
    for(i=0;i<15;i++){}
    SDA_out=1;
    for(i=0;i<15;i++){}
    
    SCL_out=1;
	for(i=0;i<15;i++){}
	SDA_out=0;
	for(i=0;i<15;i++){}
}
//----------------------------------------------------------------------------------------------------
//       I2C ֹͣ�ź�
//----------------------------------------------------------------------------------------------------
void I2C_stop(void)
{
    uint8_t i;
    
    SCL_out=0;
    for(i=0;i<15;i++){}
    SDA_out=0;
    for(i=0;i<15;i++){}
    
    SCL_out=1;
	for(i=0;i<15;i++){}
	SDA_out=1;
	for(i=0;i<15;i++){}
}

//----------------------------------------------------------------------------------------------------
//       I2C �����ֽ�
//----------------------------------------------------------------------------------------------------
uint8_t I2C_send_byte(uint8_t byte)
{
    uint8_t i,j,k;
    
	for(k=0;k<8;k++)
	{
		SCL_out=0;
		if((byte<<k) & 0x80) 
		    SDA_out=1;
		else 
		    SDA_out=0;
		for(i=0;i<15;i++){}
		SCL_out=1;
		for(i=0;i<15;i++){}
	}
	SCL_out=0;
	for(i=0;i<15;i++){}
	
	SDA_port_direction = input;
	for(i=0;i<15;i++){}
	SCL_out=1;
	for(i=0;i<15;i++){}
	j = SDA_in;
	SCL_out=0;
    for(i=0;i<15;i++){}
    SDA_port_direction = output;
    SDA_out=0;
    for(i=0;i<15;i++){}
    if(j==0)
        return 1;
    else
        return 0;
}
//----------------------------------------------------------------------------------------------------
//       I2C �����ֽ�
//----------------------------------------------------------------------------------------------------
uint8_t I2C_read_byte(uint8_t ack)
{
	uint8_t i,j=0,k;
	SCL_out=0;
	SDA_port_direction = input;
	for(i=0;i<15;i++){}
	for(k=0;k<8;k++)
	{
		SCL_out=0;
		for(i=0;i<15;i++){}
		SCL_out=1;
		for(i=0;i<15;i++){}
		j<<=1;
		if(SDA_in==1)
		    j+=1;
	}
	SCL_out=0;
	SDA_port_direction = output;
	SDA_out=0;
	for(i=0;i<15;i++){}
	
	if(ack==1)
	{
		SCL_out=1;
        for(i=0;i<15;i++){}
        SCL_out=0;
        for(i=0;i<15;i++){}
	}

	return j;

}

//----------------------------------------------------------------------------------------------------
//       I2C д�����ӳ���
//----------------------------------------------------------------------------------------------------

void I2C_WriteChar(uint8_t slave_address,uint8_t reg_address,uint8_t data)
{
    I2C_start();
    I2C_send_byte(slave_address);
    I2C_send_byte(reg_address);
    I2C_send_byte(data);
    I2C_stop();
}
//----------------------------------------------------------------------------------------------------
//       I2C �������ӳ���
//----------------------------------------------------------------------------------------------------
uint8_t I2C_ReadWord(uint8_t slave_address,uint8_t reg_address,uint16_t *data,uint8_t sequence)
{
    I2C_start();
    I2C_send_byte(slave_address);
    I2C_send_byte(reg_address);
    I2C_start();
    if(I2C_send_byte(slave_address+1))
    {
        if(sequence==0)
        {
        	*data = I2C_read_byte(master_ack)*256;
            *data += I2C_read_byte(no_master_ack);
        }
        else
        {
        	*data = I2C_read_byte(master_ack);
            *data += I2C_read_byte(no_master_ack)*256;
        }
        
        I2C_stop();
        return 1;
    }
    I2C_stop();
    return 0;
}


