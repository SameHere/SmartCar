#ifndef  _LEDSWITCH_H
#define  _LEDSWITCH_H


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0



#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 


#define  write_data       1                                                             //*
#define  write_command    0
//Register Address Map              REGISTER                   HEX CODE 
#define                             No_Op_addr                 0x00
#define                             Decode_Mode_addr           0x09
#define                             Intensity_addr             0x0A
#define                             Scan_Limit_addr            0x0B
#define                             Shutdown_addr              0x0C
#define                             Display_Test_addr          0x0F
                                    //REGISTER DATA            HEX CODE
#define                             Shutdown_Mode              0x00
#define                             Normal_Mode                0x01
#define                             BCD_Mode                   0xFF
#define                             nodecode_Mode              0x00
#define                             Scan_Limit                 0x07
#define                             Intensity_duty             0x00
#define                             Normal_Operation           0x00
#define                             Display_Test_Mode          0x01



void OLED_Init(void);
//void OLED_Clear(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_Display_On(void);
void OLED_Display_Off(void);
//void OLED_Clear(void);
void OLED_On(void)  ;

//void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);









#endif
