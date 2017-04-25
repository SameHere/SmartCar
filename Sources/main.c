#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "MyHeader.h"
/*---------------------------------------------------------------------------------------*/
/*				            	2016���		    						 			 */
/* 			2016/12/16 ����:PWM1500�ȶ��� kp0=11 kp1=18 kp2=13 kd=6						 */
/*---------------------------------------------------------------------------------------*/
extern core_config();
extern enableIrq();
extern disableIrq();
extern initSTM();
extern initSCI();
extern SCI0_SendChar();
extern SCI0_SendChar_16();
extern SCI0_RecvChar();
extern initADC();
extern initPIT();
extern initEMIOS();
extern initI2C();
extern I2C_WriteChar();

void Sampling( );
void Bubble_Sort( uint16_t *pList, uint8_t index );
uint16_t Extremum_Cut(uint16_t *pList, uint8_t index, uint8_t HeadCutNum, uint8_t TailCutNum );
void motor_drive( int16_t zkb );
void Show_Me_Data( uint16_t number_test,uint8_t change,uint8_t flag );
/*---------------------------------------------------------------------------------------*/
#define   servo_pwm      EMIOS_0.CH[21].CBDR.R     //Maximal value is 13333
#define   motor_pwm    	 EMIOS_0.CH[13].CBDR.R     //Maximal value is 8000
#define   servo_max     1200//��ת����
#define   servo_min     -1200//��ת����
#define   servo_mid     5960
/*========================================================================================*/

uint16_t ABS( int x,int y );
int8_t FuHao( int16_t x );
void Delay1us();
void delay_1s();

/*========================================================================================*/
struct motor_PID
{
//==========================================
    int32_t  Kp;       // ��������
    int32_t  Ki;       // ���ֳ���
    int32_t  Kd;       // ΢�ֳ���

    int32_t  iError; 		    // Error[0]
    int32_t  LastError;         // Error[-1]
    int32_t  PrevErrorl;        // Error[-2]

    int32_t  PID_DertaU;        // PID����
    int32_t  PIDOutput;		    // PID���
    int32_t  Last_PIDOutput;    // �ϴ�PID���

// �޷���
    int32_t  Max_iError;            // �������iError��������
    int32_t  Min_iError;			// ������СiError
    int32_t  Max_iIncpid;           // �������PID����
    int32_t  Min_iIncpid;			// ������СPID����
    int32_t  Max_otput;             // ������������
    int32_t  Min_otput;			    // ������С�����
//==========================================
} PID;
/*************************���㷨�������*****************************/

int Error=0;
int LastError=0;
uint16_t ADC_flag = 0;
int16_t ADaverage[6] = {0};		 	    // ����������ľ�ֵ
int laserror=0;
uint8_t Position=0;
uint8_t P[10]={14,13,12,11,9,7,6,5,4,4};
//uint8_t P[10]={16,15,14,13,12,9,8,7,6,5};
int Thread=800;
int thread=6;
//=====================================================================//
////////////////////////��������������ָʾ�Ƴ�ʼ��//////////////////////
//====================================================================//

#define   load_port_config            SIU.PCR[PCR76_PE12].R
#define   din_port_config             SIU.PCR[PCR34_PC2].R
#define   clk_port_config             SIU.PCR[PCR5_PA5].R

#define   ok_port_config              SIU.PCR[PCR35_PC3].R
#define   increase_port_config        SIU.PCR[PCR34_PC2].R
#define   decrease_port_config        SIU.PCR[PCR5_PA5].R
 
#define   ok_port_out                 SIU.GPDO[PCR35_PC3].R
#define   increase_port_out           SIU.GPDO[PCR34_PC2].R
#define   decrease_port_out           SIU.GPDO[PCR5_PA5].R

#define   ok_port_in                  SIU.GPDI[PCR35_PC3].R
#define   increase_port_in            SIU.GPDI[PCR34_PC2].R
#define   decrease_port_in            SIU.GPDI[PCR5_PA5].R

#define   BEE_CONFIG                  SIU.PCR[PCR47_PC15].R
#define   BEE_OUTPUT                  SIU.GPDO[PCR47_PC15].R

#define   SAMPLING_TIME                       15     //��������
#define   ARR_MARKERROR_LENGTH                25


#define  port_output   0x0200                                                           //*
#define  port_input    0x0103
#define  in_put                     0
#define  out_put                    1
#define  LOW                        0
#define  HIGH                       1

struct tagParameter      //����ṹ��洢������Ҫ���ڵĲ���
{
    int16_t Speed_Set;  //�趨�ٶ�
    int      Steer_P;       //���P
    int      Steer_D;       //���D
    int      Set_Steer_P;   //�����׼P
    int16_t  Lost_Line;
    
} PAR;
void PAR_init()
{
    PAR.Speed_Set = 1500;
    PAR.Steer_P   = 15;
    PAR.Steer_D   = 15;
    PAR.Set_Steer_P   = 45;
    PAR.Lost_Line = 25;
}

void Desktop()
{
    uint8_t t;
    ok_port_config = port_input;
    OLED_ShowCHinese( 0,0,0 );
    OLED_ShowCHinese( 17,0,1 );
    OLED_ShowCHinese( 33,0,2 );
    OLED_ShowCHinese( 49,0,3 );
    OLED_ShowCHinese( 65,0,4 );
    OLED_ShowCHinese( 81,0,5 );
    OLED_ShowCHinese( 97,0,6 );
    OLED_ShowCHinese( 113,0,7 ); //huat

    OLED_ShowCHinese( 40,3,8 );  //��
    OLED_ShowCHinese( 56,3,9 );  //��
    OLED_ShowCHinese( 72,3,10 ); //��

    OLED_ShowString( 72,6,"FindMAX",16 );

    while( 1 )
    {
        if( ok_port_in == 0 )
        {
            while( ok_port_in == 0 );
            break;
        }
    }
}

void Show_Me_Data( uint16_t number_test,uint8_t change,uint8_t flag )
{
    OLED_ShowChar( 0,6,'-',16 );
    OLED_ShowChar( 56,6,'+',16 );
    OLED_ShowString( 96,6,"OK",16 );
//----------------------------------------------//

    while ( 1 )
    {
        if ( decrease_port_in == LOW )
        {
            while ( decrease_port_in == LOW );
            number_test-=change;
        }
        if ( increase_port_in == LOW )
        {
            while ( increase_port_in == LOW );
            number_test+=change;
        }

        switch( flag )
        {

        case  1:
            PAR.Speed_Set = number_test;
            OLED_ShowString( 0,0,"Speed_Set:",16 );
            break;
        case  3:
            PAR.Steer_P = number_test;
            OLED_ShowString( 0,0,"Steer_P:",16 );
            break;
        case  4:
            PAR.Steer_D = number_test;
            OLED_ShowString( 0,0,"Steer_D:",16 );
            break;
        case  5:
        	PAR.Lost_Line = number_test;
        	OLED_ShowString( 0,0,"LostLine:",16 );
        	break;
        case  6:
        	PAR.Set_Steer_P = number_test;
        	OLED_ShowString( 0,0,"Set_Steer_P:",16 );
        	break;
        case  7:
        	Thread = number_test;
        	OLED_ShowString( 0,0,"Thread:",16 );
        	break;
        case  8:
        	thread = number_test;
        	OLED_ShowString( 0,0,"Thread:",16 );
        	break;

        default:
            break;
        }

        OLED_ShowNum( 20,3,number_test,5,16 );

        if ( ok_port_in == LOW ) //����������Ϻ�����ѭ��
        {
            while( ok_port_in == LOW );
            OLED_Clear( 0,7 );
            break;
        }
    }
}

void xianshi( void )
{
    uint8_t flag = 0;

    delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();         // IIC��ʼ��
    OLED_Init();	   //��ʼ��OLED
    OLED_Clear( 0,7 );
//--------------------------------------------------------------//
    while( flag == 0 )
    {

        decrease_port_config= port_input;   //��������Ϊ����
        increase_port_config = port_input;
        ok_port_config = port_input;


        //***********���޸Ĳ���*************************//
        Show_Me_Data( PAR.Speed_Set,50,1 );
        Show_Me_Data( Thread, 25, 7 );
        Show_Me_Data( thread,1,8 );

        Show_Me_Data( PAR.Steer_D,1,4 );
        
        //Show_Me_Data( PAR.Lost_Line, 2, 5 );
        
        //Show_Me_Data( PAR.Set_Steer_P, 1, 6 );


        delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
        initI2C();         // IIC��ʼ��
        OLED_Init();	   //��ʼ��OLED
        OLED_Clear( 0,7 );
        OLED_ShowString( 80,6,"NEXT",16 );
        OLED_ShowString( 0,6,"BACK",16 );
        while( 1 )
        {
            if( ok_port_in == LOW )
            {
                while( ok_port_in == LOW );
                flag =1;
                break;
            }
            if( decrease_port_in == LOW )
            {
                while( decrease_port_in == LOW );
                delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
                initI2C();         // IIC��ʼ��
                OLED_Init();	   //��ʼ��OLED
                OLED_Clear( 0,7 );
                break;
            }
        }
    }
}

void show_Maxvalue( int16_t x1, int16_t x2, int16_t x3, int16_t x4, int16_t x5, int16_t x6  ) //������������ �ַ���������Ҫ����
{
//   OLED_ShowString( 0,0,"M1:",16 );
    OLED_ShowNum( 8,0,x1,4,16 ); //��ʾ����/**/

//    OLED_ShowString( 48,0,"M2:",16 );
    OLED_ShowNum( 48,0,x2,4,16 ); //��ʾ����/**/

    OLED_ShowString( 0,3,"    maxvalue",16 );
    OLED_ShowNum( 88,0,x3,4,16 ); //��ʾ����/**/

//    OLED_ShowString( 0,6,"M4:",16 );
    OLED_ShowNum( 8,6,x4,4,16 ); //��ʾ����/**/

//    OLED_ShowString( 0,6,"M5:",16 );
    OLED_ShowNum( 48,6,x5,4,16 ); //��ʾ����/**/

//    OLED_ShowString( 0,6,"M6:",16 );
    OLED_ShowNum( 88,6,x6,4,16 ); //��ʾ����/**/

}

void show_ADaverage( ) //������������ �ַ���������Ҫ����
{

	delay_ms( 100 );     //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();           // IIC��ʼ��
    OLED_Init();	     //��ʼ��OLED
    OLED_Clear( 0,7 );

	while ( 1 )
    {

        ok_port_config = port_input;

        servo_pwm = servo_mid;

        if ( ok_port_in == LOW )     //˫��OKȷ��
        {
            while ( ok_port_in == LOW );
            break;
        }
        Sampling();

	    //OLED_ShowString( 0,0,"M1:",16 );
	    OLED_ShowNum( 8,0,ADaverage[0],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 48,0,"M2:",16 );
	    OLED_ShowNum( 48,0,ADaverage[1],4,16 ); //��ʾ����/**/

	    OLED_ShowString( 0,3,"    ADaverage",16 );
	    OLED_ShowNum( 88,0,ADaverage[2],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M4:",16 );
	    OLED_ShowNum( 8,6,ADaverage[3],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M5:",16 );
	    OLED_ShowNum( 48,6,ADaverage[4],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M6:",16 );
	    OLED_ShowNum( 88,6,ADaverage[5],4,16 ); //��ʾ����/**/

    }
}


/*-------------------------------------------------------------------*/
inline uint16_t ABS( int x,int y )
{
    if( x>y )
        return( x-y );
    else
        return( y-x );
}
float ABS_f( float x,float y )
{
    if( x>y ) return( x-y );
    else	return( y-x );
}
/*-------------------------------------------------------------------*/
inline int limit(float x,float n) {
	if(x-n>4)
		x=n+4;
	else if(n-x>4)
		x=n-4;
	
	if(x>150)
		return 150;
	else if(x<-150)
		return -150;
	else
		return x;
}
/*-------------------------------------------------------------------*/
inline int lost(int a,int b,int error,int16_t *AD) {
	if((a<10||b<10)&&ABS(a,b)<5||AD[1]<thread) {
		if(Position==0)
			return -150;
		else if(Position==3)
			return 150;
	}else	
		return error;
}
/*-------------------------------------------------------------------*/
int8_t FuHao( int16_t x )
{
    if( x>0 )
        return 1;
    if( x<0 )
        return -1;
}
/*----------------------------------------------------------------------------------------*/

float sqrt( float x )
{
    float xhalf ;
    float num ;
    int i ;
    float xx=x;

    if ( x<0 )
        x=-x;
    xhalf = 0.5l * x;
    num = x;
    i = *( int * )& x;

    i = 0x5f375a86 - ( i >> 1 );
    x = *( float * ) & i;
    x = x * ( 1.5f - xhalf*x*x );
    x = x * ( 1.5f - xhalf*x*x );
    x = x * ( 1.5f - xhalf*x*x );
    if ( xx>0 )
        return num*x;
    else if ( xx==0 )
        return 0;
    else if ( xx<0 )
        return -num*x;
}
////////////////////////////////////��ʱ����//////////////////////////////////////
void Delay1us( void )
{
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
    \
    __asm( nop );
    __asm( nop );
    __asm( nop );
    __asm( nop );
}

void delay_1s( void )
{
    uint16_t i=0,f=0;
    for( i=0; i<888; i++ )
    {
        for( f=0; f<666; f++ )
        {
            Delay1us();
        }
    }
}

/*----------------------------------------------------------------------------------------*/
void PIT_CH1_isr( void )
{
    SIU.GPDO[PCR38_PC6].R=~SIU.GPDO[PCR38_PC6].R;
    PIT.CH[1].TFLG.B.TIF = 1;
}
/*---------------------------------------------------------------------------------------*/
uint32_t yy_overflow=0;
uint32_t EMIOS0_overflow_times=0;
uint16_t EMIOS0_overflow_times_Dis=0;
int32_t distance_time=0;
int32_t Distance_Count=0;
int32_t per_distance_time=0;
int32_t distance_bian=0;
int32_t Car_Distance=0;     //�ӷ�����������ʻ�ľ���
uint32_t per_distance=0;	//�������������
uint32_t sum_distance=0;
//-----------------------------------
uint8_t flage_tiaosu = 0;
uint8_t EMIOS0_overflow_times_Cesu=0;
//-----------------------------------
int16_t smartcar_speed=0;
//-----------------------------------
uint16_t count_now=0;
uint16_t count_last=0;
uint32_t count;
uint32_t period;
//------------------------------------
uint8_t  period_sum_times = 1;
int16_t smartcar_speed_0=0;
int16_t smartcar_speed_1=0;
int32_t smartcar_speed_temp;
uint16_t smartcar_speed_time = 0;
uint32_t period_sum = 0;
// EMIOS0_CH1_0_isr interrupt used for counting times,and make conditions for speed musure.
void EMIOS0_CH16_17_isr( void )    //overflow interrupt
{
    EMIOS0_overflow_times++;
    yy_overflow++;
    EMIOS0_overflow_times_Cesu++;
    if( EMIOS0_overflow_times_Cesu>10 )
    {
        EMIOS0_overflow_times_Cesu = 0;
        smartcar_speed = 0;
        smartcar_speed_time = 0;
        period_sum_times = 1;
        period_sum=0;
        smartcar_speed_0=0;
        smartcar_speed_temp=0;
        flage_tiaosu=1;
    }

    EMIOS_0.CH[16].CSR.B.FLAG = 1;
}
//---------------------------------------------------------------------------
int32_t time_yy[5]= {0},time_y_y,yi=0;
int32_t jishuA=0,distance_yy=0,period_yy=0;
int32_t lastdistance_yy=0;
void EMIOS0_CH18_19_isr( void )
{
    if( EMIOS_0.CH[19].CCR.B.EDPOL==0 ) //����Ƿ��ڸߵ�ƽ
    {
        period_yy=yy_overflow*65535+EMIOS_0.CH[19].CADR.R-jishuA;//���ɼ���ֵ
        if( SIU.GPDI[PCR6_PA6].R==1 ) //������Ч
        {
            time_y_y=period_yy/16;	//ʱ���1000����ms
            if( time_y_y>9000 )
                time_yy[4]=time_yy[4];
            else if( time_y_y<=9000&&time_y_y>0 )
                time_yy[4]=time_y_y;
        }
    }
    for( yi=0; yi<4; yi++ )
    {
        time_yy[yi]=time_yy[yi+1];
    }

    if( ABS( time_yy[4],time_yy[3] )>300 )time_yy[4]=( 9*time_yy[3]+time_yy[4] )/10;

    lastdistance_yy=distance_yy;
    distance_yy=( time_yy[0]+time_yy[1]+time_yy[2]+time_yy[3]+time_yy[4] )*34/400; //��λmm
    if( ABS( distance_yy,lastdistance_yy )>300 )distance_yy=( 9*lastdistance_yy+distance_yy )/10;

    jishuA=EMIOS_0.CH[19].CADR.R;
    yy_overflow=0;

    EMIOS_0.CH[19].CCR.B.EDPOL =~EMIOS_0.CH[19].CCR.B.EDPOL; //�ı��жϱ���
    EMIOS_0.CH[19].CSR.B.FLAG = 1;
}
//---------------------------------------------------------------------------
void EMIOS0_CH20_21_isr( void )
{

//-------------------------------------------------------------------
    smartcar_speed_time++;
    distance_time++;
    Distance_Count++;
//	SIU.GPDO[PCR38_PC6].R = !SIU.GPDO[PCR38_PC6].R;

	SIU.PCR[PCR66_PE2].R = port_input;
	
    	if(SIU.GPDI[PCR66_PE2].R==0)
        {
        	distance_time=distance_time-2;
        	Distance_Count=Distance_Count-2;
        }

    count_last = count_now;
    count_now  = EMIOS_0.CH[20].CADR.R;

    if( count_last<50 && EMIOS0_overflow_times_Cesu>0 )
    {
        EMIOS0_overflow_times_Cesu-=1;

    }

    if( count_now<count_last && EMIOS0_overflow_times_Cesu==0 )
    {
        EMIOS0_overflow_times_Cesu+=1;

    }

    count = count_now+( EMIOS0_overflow_times_Cesu*65535 )-count_last;
    if( count<370 )
    {
        count = 370;
    }

    period_sum = period_sum+count;//�����ۼ�

    if( period_sum_times == smartcar_speed_time )
    {
        period = period_sum/period_sum_times;
        //B�����ټ��㹫ʽ����mm/s��
        //..............Ƶ��(MHz)--����--С��--�־�*10---period--------���//
        smartcar_speed_temp = 1600/100*40*560*3142/period/102       /2;
        //----------------------------------------------------
        if( ABS( smartcar_speed_temp,smartcar_speed_0 )>1500 )
        {
            smartcar_speed_temp = smartcar_speed_0;
        }

        smartcar_speed_temp =( smartcar_speed_1 + smartcar_speed_0 + smartcar_speed_temp )/3;

        smartcar_speed_1 = smartcar_speed_0;
        smartcar_speed_0 = smartcar_speed_temp;

        smartcar_speed_time = 0;
        period_sum = 0;
        period_sum_times = 1 + smartcar_speed_temp/100;
        

        smartcar_speed = smartcar_speed_temp;
        if(SIU.GPDI[PCR66_PE2].R==1)
        	smartcar_speed =  - smartcar_speed;/**/
        

        per_distance  = smartcar_speed_1 * period/160000; //100000��
        sum_distance  = sum_distance+per_distance;
        distance_bian = 63*distance_time/840;

        Car_Distance = 63*Distance_Count/840;

        flage_tiaosu = 1;
    }

    EMIOS0_overflow_times_Cesu = 0;

    EMIOS_0.CH[20].CSR.B.FLAG = 1;

}

/*********************************�������ݴ���/����*****************************************/
uint16_t last_gao[10]= {0},last_di[10]= {0};
uint16_t gao_wei=0,di_wei=0;
void DATA_send( int16_t SHU, uint8_t i )
{
    gao_wei = SHU/100;
    di_wei  = SHU%100;
    if( gao_wei>99 )
        gao_wei = last_gao[i];
    if( di_wei>99 )
        di_wei  = last_di[i];
    last_gao[i] = gao_wei;
    last_di[i]  = di_wei;
    if( i==1 )
        SCI0_SendChar( 0xaa );
    if( i==2 )
        SCI0_SendChar( 0xbb );
    if( i==3 )
        SCI0_SendChar( 0xcc );
    if( i==4 )
        SCI0_SendChar( 0xdd );
    if( i==5 )
        SCI0_SendChar( 0xee );
    //----------------------
    SCI0_SendChar( gao_wei );
    SCI0_SendChar( di_wei );
    gao_wei=0;
    di_wei=0;
}
//-------------����ͨѶ���ݽ���/���ͣ�
uint8_t MOTE=0,gaowei_flag=0,diwei_flag=0;
int16_t DATAshou[6]= {0},R_flag=0;		//����������ݱ�
uint8_t ia,aa,bb;
void LINFlex0_RXI_ISR()
{
    MOTE=( uint8_t ) LINFLEX_0.BDRM.B.DATA4; // Read byte of Data
    LINFLEX_0.UARTSR.B.RMB = 1;              // Clear Release Message Buffer flag
    if( MOTE==0xaa )
    {
        R_flag=1;
        gaowei_flag=1;
        ia=0;
    }
    if( MOTE==0xbb )
    {
        R_flag=2;
        gaowei_flag=1;
        ia=0;
    }
    if( MOTE==0xcc )
    {
        R_flag=3;
        gaowei_flag=1;
        ia=0;
    }
    if( MOTE==0xdd )
    {
        R_flag=4;
        gaowei_flag=1;
        ia=0;
    }
    if( MOTE==0xee )
    {
        R_flag=5;
        gaowei_flag=1;
        ia=0;
    }

    ia++;
    if( ia==2&&gaowei_flag==1 )
    {
        gaowei_flag=0;
        aa=MOTE;
        diwei_flag=1;
    }
    if( ia==3&&diwei_flag==1 )
    {
        if( aa<0xaa )
            DATAshou[R_flag]= MOTE+aa*100;
        diwei_flag=0;
        ia=0;
    }
    if( ia>=3 ) ia=0;

    SIU.GPDO[PCR38_PC6].R=~SIU.GPDO[PCR38_PC6].R;
    LINFLEX_0.UARTSR.B.DRF = 1;   /* Clear Data Reception Complete Flag flag */
}
//*******************************ADC��������***************************************
/*	  ��Ƭ����нӿ�λ��
						ANP13
ANP3		ANP2		ANP1
			ANP11		ANP10
			ANP8		ANP9
-----------------------------
		���ŵ���Ų�
 	 AD[0]	 AD[1]	 AD[2]
	ANP[08] ANP[9]	ANP[10]


	 AD[3]   AD[4]	 AD[5]
	ANP[2]	ANP[1]  ANP[13]
*/


void Sampling( void )       
{
    uint16_t i=0,j=0;
    uint16_t AD_SUM=0;
    uint16_t samplearray[6][SAMPLING_TIME]= {0};		// ֱ�ӵ�ADC����ֵ

//------------------------------------------------------------------------------------------
    ADC.NCMR[0].R = 0x00000400;     			   /* Select ANP  10 */
    ADC.MCR.B.NSTART=1;             			   /* Trigger normal conversions for ADC0 */
    for( i=0; i<SAMPLING_TIME; i++ )
    {
        while ( ADC.CDR[10].B.VALID == 0 ) {};  	 /* Wait for last scan to complete */
        samplearray[2][i] = ADC.CDR[10].B.CDATA;  /* Read ANS0 conversion result data */
    }
    ADC.MCR.B.NSTART=0;
//------------------------------------------------------------------------------------------
    ADC.NCMR[0].R = 0x00000200;     			   /* Select ANP  9 */
    ADC.MCR.B.NSTART=1;             			   /* Trigger normal conversions for ADC0 */
    for( i=0; i<SAMPLING_TIME; i++ )
    {
        while ( ADC.CDR[9].B.VALID == 0 ) {};  	 /* Wait for last scan to complete */
        samplearray[1][i] = ADC.CDR[9].B.CDATA;  /* Read ANS0 conversion result data */
    }
    ADC.MCR.B.NSTART=0;
//------------------------------------------------------------------------------------------
    ADC.NCMR[0].R = 0x00000100;     			   /* Select ANP  8 */
    ADC.MCR.B.NSTART=1;             			   /* Trigger normal conversions for ADC0 */
    for( i=0; i<SAMPLING_TIME; i++ )
    {
        while ( ADC.CDR[8].B.VALID == 0 ) {};  	 /* Wait for last scan to complete */
        samplearray[0][i] = ADC.CDR[8].B.CDATA;  /* Read ANS0 conversion result data */
    }
    ADC.MCR.B.NSTART=0;                            /* finish the current chain conversion and stops the operation.*/

//------------------------------------------------------------------------------------------
    ADC_flag++;
    if( ADC_flag >= 20 )
    {
        ADC_flag = 25;
        
        for( i=0; i<3; i++ )
        {
        	Bubble_Sort( samplearray[i], SAMPLING_TIME );
        	ADaverage[i] = samplearray[i][SAMPLING_TIME/2];
//        	ADaverage[i] = Extremum_Cut( samplearray[i], SAMPLING_TIME, 4, 4 );
        }
    }
//------------------------------------------
}

void motor_init()
{
    SIU.PCR[PCR71_PE7].R  = 0x0203;
    SIU.PCR[PCR40_PC8].R  = 0x0203;

    SIU.GPDO[PCR71_PE7].R = 1;
    SIU.GPDO[PCR40_PC8].R = 0;
    motor_pwm = 0;
}
/*-------------------------------------------------------------*/
void motor_drive( int16_t zkb )
{
    SIU.PCR[PCR71_PE7].R = 0x0200;  //GPIO[70] is output
    SIU.PCR[PCR40_PC8].R = 0x0200;  //GPIO[71] is output
//**************************************************************
    // ����ת���ƣ�
    
    if( zkb > 0 )
    {
    
    	BEE_OUTPUT = LOW;
    
        SIU.GPDO[PCR71_PE7].R  = 1;		// 0-1��ת
        SIU.GPDO[PCR40_PC8].R  = 0;

        motor_pwm = zkb;
    }
    if( zkb < 0 )
    {
    
    	BEE_OUTPUT = HIGH;
    	
        SIU.GPDO[PCR71_PE7].R = 0;		// 1-0��ת
        SIU.GPDO[PCR40_PC8].R = 1;

        motor_pwm = 0-zkb;
    }
//------------------------------------
}
/*=========================================================================================*/
//********************************�ٶ�PID������ʼ��********************************
void PID_init()
{
    PID.Kp = 5600;
    PID.Ki = 350;
    PID.Kd = 230;

    PID.iError     = 0;
    PID.LastError  = 0;
    PID.PrevErrorl = 0;

    PID.PID_DertaU     = 0;
    PID.PIDOutput      = 0;
    PID.Last_PIDOutput = 0;

    /////*******�޷�*****////
    PID.Max_iError  =  3000;
    PID.Min_iError  = -3000;
    PID.Max_iIncpid =  6500;
    PID.Min_iIncpid = -6500;
    PID.Max_otput   =  7000;
    PID.Min_otput   = -6000;
//-----------------------------------
}

//******************�ٶ�PID����***************************
void Speed_PID( int Current_Speed,int SetPoint )
{
    PID.iError = SetPoint-Current_Speed;  //�������� ��ǰ���
    //************��������**************************************
    // ������
    if( PID.iError>=PID.Max_iError )
    {
        PID.iError = PID.Max_iError;
    }
    if( PID.iError<= PID.Min_iError )
    {
        PID.iError = PID.Min_iError;
    }
    //***********************************************************
    // PID_L�������㣺
    PID.PID_DertaU = ( PID.Kd*( PID.PrevErrorl+PID.iError-2*PID.LastError )
                       +PID.Kp*( PID.iError-PID.LastError )
                       +PID.Ki*PID.iError )/1000;
    //***********************************************************
    //�洢�������´μ���
    PID.PrevErrorl =  PID.LastError;
    PID.LastError  =  PID.iError;

    //****** ********�����ж�*** **********************************
    // ����������
    if( PID.PID_DertaU>= PID.Max_iIncpid )
    {
        PID.PID_DertaU= PID.Max_iIncpid;
    }

    if( PID.PID_DertaU<= PID.Min_iIncpid )
    {
        PID.PID_DertaU= PID.Min_iIncpid;
    }
    //**************************************************************
    // PID�����
    PID.PIDOutput= PID.Last_PIDOutput+PID.PID_DertaU;

    //**************************************************************
    // ���������
    if( PID.PIDOutput <= PID.Min_otput )
    {
        PID.PIDOutput= PID.Min_otput;
    }

    if( PID.PIDOutput>= PID.Max_otput )
    {
        PID.PIDOutput= PID.Max_otput;
    }
    //**************************************************************
    PID.Last_PIDOutput=PID.PIDOutput;  // PID����洢
    //**************************************************************
/*
    if( ABS( SetPoint,0 )<200 )
    {
        if( ABS( Current_Speed,0 )>SetPoint && ABS( Current_Speed,0 )<150 )
        {
            PID.PIDOutput=0-2*SetPoint;
            motor_drive( PID.PIDOutput );
        }
    }
*/
    motor_drive( PID.PIDOutput ); //�������
//----------------------------------------
}

/**********************************ð������***************************************************/
void Bubble_Sort( uint16_t *pList, uint8_t index )
{
	//����ð�������㷨����ɶ�����list�е�index������������
    uint8_t i=0, j=0, flag=0;
    uint16_t temp=0;
    
    for(i=1; i<index; i++)
    {
    	flag = 0;
        for(j=0; j<index-i; j++)
        {
            if(pList[j]>pList[j+1])
            {
                temp = pList[j];
                pList[j] = pList[j+1];
                pList[j+1] = temp;
                flag = 1;             //����Ƿ�����˽���
            }
        }
        if( flag == 0 )              //��־λΪ0˵���Ѿ��������
        	break;
    }
}

/*******************************ȥ��β��ֵ������ƽ��ֵ****************************************/

/**�βα���˵����
1. pList:       ʵ��������׵�ַ
2. index:       ��������Ҫ�����Ԫ�ظ���
3. HeadCutNum : ǰ�漫ֵȥ������
4. TailCutNum : ���漫ֵȥ������

/**/
uint16_t Extremum_Cut(uint16_t *pList, uint8_t index, uint8_t HeadCutNum, uint8_t TailCutNum )
{
	uint8_t i=0;
	uint32_t sum = 0;
	uint16_t average = 0;
	uint8_t record = index-TailCutNum;
	
	if(index < HeadCutNum+TailCutNum )     //��ָֹ���ܷ�
		return 0;
	
	for( i=HeadCutNum; i<record; i++ )
	{
		sum += pList[i];
	}
	average = sum/( record - HeadCutNum );
	return average;
}

/**************************�Ե����źŲɼ�(��ʼ������ֵ)********************************************/

void NoiseValue_init( int16_t *Noise_Value )   
{
	int i=0,j=0;
	uint16_t arrRecordValue[6][200];
	
	delay_ms( 100 );     //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();           // IIC��ʼ��
    OLED_Init();	     //��ʼ��OLED
    OLED_Clear( 0,7 );
        
    for( i=0; i<30; i++ )        //�ղ���30��
    {
    	Sampling( );
    }
	for( i=0; i<200;i++ )
	{
		Sampling( );
		for( j=0; j<6; j++)
		{
			arrRecordValue[j][i] = ADaverage[j];
		}
	}
	for( i=0; i<6; i++ )
	{
		Bubble_Sort( arrRecordValue[i],200);
	}
	for( i=0; i<6; i++)
	{
		Noise_Value[i] = Extremum_Cut( arrRecordValue[i],200,20,20);
	}
	while ( 1 )
	{

		ok_port_config = port_input;

		servo_pwm = servo_mid;

		if ( ok_port_in == LOW )     //˫��OKȷ��
		{
		    while ( ok_port_in == LOW );
		    break;
		}        
	    //OLED_ShowString( 0,0,"M1:",16 );
	    OLED_ShowNum( 8,0,Noise_Value[0],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 48,0,"M2:",16 );
	    OLED_ShowNum( 48,0,Noise_Value[1],4,16 ); //��ʾ����/**/

	    OLED_ShowString( 0,3,"   Noise_Value",16 );
	    OLED_ShowNum( 88,0,Noise_Value[2],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M4:",16 );
	    OLED_ShowNum( 8,6,Noise_Value[3],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M5:",16 );
	    OLED_ShowNum( 48,6,Noise_Value[4],4,16 ); //��ʾ����/**/

	    //OLED_ShowString( 0,6,"M6:",16 );
	    OLED_ShowNum( 88,6,Noise_Value[5],4,16 ); //��ʾ����/**/
    
	}
	
}

/**********************************��һ��*******************************************/

void Normalization( int16_t *pNormValue, int16_t *pMin_Value, int16_t *pMax_Value, int16_t *pResult, uint8_t index )
{
	uint8_t i = 0;
	float TempTran[6]= {0};
    
    for( i=0; i<index; i++ )
    {
        TempTran[i]=( float )( pNormValue[i]-pMin_Value[i] )/( float )( pMax_Value[i]-pMin_Value[i] );
        if( TempTran[i] <= 0 )
            TempTran[i] = 0.00;
        if( TempTran[i] >= 1 )
            TempTran[i] = 1.0;
        pResult[i]=( uint16_t )( 200*TempTran[i] );
    }
}

/***********************************ǰ���ҹ��ɵ㼰������ֵ*************************************/
/*
	
	
*/
/********************************************************************************************/
void Find_Max( int16_t *pMax_Value, uint8_t index )
{
    //�ҵ�����ֵ
    uint8_t i = 0;
    uint8_t j = 0;

    delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();         // IIC��ʼ��
    OLED_Init();	   //��ʼ��OLED
    OLED_Clear( 0,7 );

    ADC_flag = 0;                       //������־λ����
    while ( 1 )
    {
        ok_port_config = port_input;

        servo_pwm = servo_mid;

        if ( ok_port_in == LOW )     //˫��OKȷ��
        {
            while ( ok_port_in == LOW );
           	break;
        }

        for ( j = 0; j<20; j++ )
        {
            Sampling( );
            for ( i = 0; i<index; i++ )
            {
                if( ADaverage[i] > pMax_Value[i] )
                {
                    pMax_Value[i] = ADaverage[i];
                }
            }
        }
        show_Maxvalue( pMax_Value[0],pMax_Value[1],pMax_Value[2],pMax_Value[3],pMax_Value[4],pMax_Value[5] );
    }
}

void Find_Transit_front( int16_t *pNormalizePT, int16_t *Max_Value, int16_t *Noise_Value )//��һ���Ժ�Ĺ��ɵ�ֵ //��е����ֵ
{
    //ǰ���ҹ��ɵ�
    uint8_t TransitCount[2]={0};           //��¼���ɵ�ĸ���
    uint16_t Temp[2][20]= {0};            //��¼�ɼ����Ĺ��ɵ��ֵ
    uint8_t k = 0;                       //ѭ����־λ
    uint8_t i = 0; 
    uint8_t j = 0;
    uint16_t sum[2]= {0};                //�洢���ɵ�ֵ�ĺ�
    
    int16_t Transit[2]={0};             //�洢�˲������Ժ�Ĺ��ɵ��ֵ                
    ok_port_config = port_input;
    
    Find_Max( Max_Value,6 );
    
    delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();         // IIC��ʼ��
    OLED_Init();	   //��ʼ��OLED
    OLED_Clear( 0,7 );
    
    while( ok_port_in == LOW );              //����ǰ�水�������Ժ�������Ӱ��

    while ( k<2 )
    {
        ok_port_config = port_input;

        if ( ok_port_in == LOW )     //OK���˳����ҹ��ɵ㣬���봦��ģʽ
        {
            while ( ok_port_in == LOW );
                k = 1;
        }

		for ( j = 0; j<20; j++ )    //�ʵ�����ѭ��������ʹ���иú���ʱ��Ƭ�������ʱ������Ѱ�ҹ��ɵ���
        {
            Sampling();
            
            if( ADaverage[0] >= Max_Value[0]-2 )
            {
                if( TransitCount[0] < 20 )      //����20��ֵ�Ͳ��ٲ���
                {
                    Temp[0][TransitCount[0]]=ADaverage[1];
                    TransitCount[0]++;
                }
			}
			
            if( ADaverage[2] >= Max_Value[2]-2 )
            {
            	if( TransitCount[1] < 20 )     //����20��ֵ�Ͳ��ٲ���
            	{
	            	Temp[1][TransitCount[1]] = ADaverage[1];
	                TransitCount[1]++;
            	}
            }
            if( TransitCount[0]==20 && TransitCount[1]==20 )  //���������ɵ㶼װ���Ժ�ֹͣ����,���봦��ģʽ
            	k = 1;
        }
    	OLED_ShowNum( 8,0,TransitCount[0],3,16 );        //��ʾ�ҵ����ɵ�ĸ���
 		OLED_ShowNum( 64,0,TransitCount[1],3,16 );
        
        if( k == 1 )                                    //���봦��ģʽ
        {
        	Bubble_Sort( Temp[0], TransitCount[0] );
        	Bubble_Sort( Temp[1], TransitCount[1] );
        	Extremum_Cut(Temp[0], TransitCount[0], 2, 2 );
        	Extremum_Cut(Temp[1], TransitCount[1], 2, 2 );
	      	Transit[0] = Extremum_Cut(Temp[0], TransitCount[0], 2, 2 );
	 		Transit[1] = Extremum_Cut(Temp[0], TransitCount[1], 2, 2 );
	 		
	 		Normalization( Transit, Noise_Value, Max_Value, pNormalizePT, 2 );     //���ɵ��һ��
	 		
	 		OLED_ShowNum( 8,0,pNormalizePT[0],3,16 );                               //��ʾ����/**/
	 		OLED_ShowNum( 64,0,pNormalizePT[1],3,16 );                               //��ʾ����/**/
        	OLED_ShowString( 0,3,"  PTnormalize",16 );
        	OLED_ShowString( 80,6,"NEXT",16 );
        	
        	while( ok_port_in == 1 );    //�ó���ͣ������ʾ���ɵ�ֵ�Ľ���
        	delay_ms( 200 );
        	while( ok_port_in == 0 );
        	break;
        } 
    }
}

/******************************A/D�˲�********************************************/
#define   ARR_FILER_AD_LENGTH          5
int16_t arrAD_Filter[3][ARR_FILER_AD_LENGTH] = {0};
int8_t  AD_Filtering_Pointer = 0;

void Filter_AD( int16_t *AD_Value )
{
	int32_t sum = 0;
	int8_t i = 0, j = 0;
	
	AD_Filtering_Pointer %= ARR_FILER_AD_LENGTH;         //ʹ��ѭ�����д洢
	for( i=0; i<3; i++ )
	{
		arrAD_Filter[i][AD_Filtering_Pointer] = AD_Value[i];
	}
	AD_Filtering_Pointer ++;
	
    for( i=0; i<3; i++ )
    {
    	sum = 0;
	   	for( j = 0; j < ARR_FILER_AD_LENGTH; j ++ )
	    {
			sum += arrAD_Filter[i][j];
	    }
    	AD_Value[i] = sum / ARR_FILER_AD_LENGTH;
    }
}

/*****************************Error�˲�**************************************/
#define   ARR_FILER_ERROR_LENGTH              5
#define    Limite_Error                       50 
int8_t Error_Filtering_Pointer = 0;
int arrFilter_Error[ARR_FILER_ERROR_LENGTH] = {0};
int Limit_Error = 0;


int Filter_Error( int nError )
{
	int32_t sum = 0;
	int16_t ave = 0;
	int8_t  i   = 0;
	
	if( nError >= Limite_Error )
	{
		nError = Limite_Error + sqrt( nError - Limite_Error );
	}
	
	Error_Filtering_Pointer %= ARR_FILER_ERROR_LENGTH;         //ʹ��ѭ�����д洢
	arrFilter_Error[Error_Filtering_Pointer] = nError;
	Error_Filtering_Pointer++;
		
    for( i = 0; i < ARR_FILER_ERROR_LENGTH; i ++ )
    {
		sum += arrFilter_Error[i];
    }
	ave = sum / ARR_FILER_ERROR_LENGTH;
	return ave;
}

/***************************************λ�ý���*******************************************/
int Errorall=0;
uint8_t markerror_pointer = 0;
uint8_t count_markerror = 0;        //��errorѭ��������Ԫ�ؼ���
int MarkError[ARR_MARKERROR_LENGTH] = {0};
uint8_t  Max_Cross = 1;           //��ʼ��ֵ��1��ʹ�俪ʼ��¼�ϴε�λ��
uint16_t Max_Old = 0;         //��¼��һ�ε������λ��
uint8_t Position_Old = 1;
void  Position_analyse_front( int16_t *PT, int16_t *Max_Value, int16_t *AD )
{

	uint8_t Max_Position=0;     //��¼��ǰ�������λ��
    uint8_t i=0;
    uint16_t Max_V=0;           //��¼��ǰ�����ֵ
    int Error1=0;  
    int Error2=0;       

	//Filter_AD( AD );
    
    /*--------------------------�������λ��--------------------------*/
    for( i=0; i<3; i++ )
    {
    	if( AD[Max_Position] < AD[i] )
    	{
    		Max_Position = i;
    	}  
    }
    Max_V = AD[Max_Position];    //������仰������޷��ж϶���
    
/*-----------------------------���ߴ��������������ֵ------------------------------*/

    if( Max_V <= 20 )
    {
    	Max_Position = Max_Old;
    }
    Max_Old = Max_Position;   //������һ��ʼ�Ͷ���,������ȷ��old��ֵ
    Max_V = AD[Max_Position];
    
/*-----------------------------------������----------------------------------------*/
    
    if( ABS( Max_Position, Max_Cross ) >= 2 )  //���ͻ�䣬����������ʻ�ǲ������λ�������(���Ϊ1)
    {
    	Max_Position = Max_Cross;
    }
    Max_Cross = Max_Position;
    
/*-------------------------�м������������λ�ý���-------------------------------*/

    if( Max_Position==0 && AD[1]<( PT[0]-1 ) )
    {
        Position=0;		    	//�м����Ѿ�ƫ��0���ɵ�
    }
    else if( ( Max_Position==1 && ( AD[0] - AD[2] ) > 1 ) || ( Max_Position==0 && ( AD[1] >= PT[0] ) ) )
    {
        Position=1;				//�м�����0���ɵ������֮��   
        
    }
    else if( ( Max_Position==1 && ( AD[2] - AD[0] ) > 1 ) || ( Max_Position==2 && ( AD[1] >= PT[1] ) ) )
    {
        Position=2;		    	//�м�����1���ɵ������֮��
        
    }
    else if( Max_Position==2 && ( AD[1] <= PT[1] - 1 ) )
    {
        Position=3;		    	//�м����Ѿ�ƫ��1���ɵ�
    }
    
/*-------------------------��λ������-------------------------------*/

    if( ABS( Position, Position_Old ) >= 2 )  //���ͻ�䣬����������ʻ�ǲ������λ�������(���Ϊ1)
    {
    	Position = Position_Old;
    }
    Position_Old = Position;
    
/*-----------------------------����ƫ����----------------------------------*/
	Error1=lost(AD[0],AD[2],limit((sqrt((float)AD[2])-sqrt((float)AD[0]))/((float)AD[2]+(float)AD[0]+1)*Thread,(float)laserror),AD);
	laserror=Error1;
    Error =  Error1;
/************************************Error�洢************************************************/
   	markerror_pointer %= ARR_MARKERROR_LENGTH;         //ʹ��ѭ�����д洢Error������仯��
	MarkError[markerror_pointer] = Error;
    LastError = MarkError[( markerror_pointer + 1 )%ARR_MARKERROR_LENGTH ];   //�궨�� ARR_MARKERROR_LENGTH ��֮ǰ��error
    markerror_pointer++;
    for(i=0;i<25;i++) {
    	Errorall+=MarkError[i];
    }
    Errorall/=10;
}

/***********************************�������************************************/
uint16_t Steer_output;
void Servo_PD( int16_t *AD ) {
	int max = servo_mid+servo_max;
	int min = servo_mid+servo_min;
	int temp=0;
	temp=AD[1]/20;
	PAR.Steer_P=P[temp];
    Steer_output = servo_mid + ( PAR.Steer_P*Error + PAR.Steer_D*( Error - LastError ) );
    if( Steer_output > max )   
    	Steer_output = max;
    if( Steer_output < min )   
    	Steer_output = min;
    servo_pwm = Steer_output;
}
/**********************************����servo_pwm��ֵ����ƫ�����Ĺ�ϵ***********************************/

void Debug_Servo_pwm( int16_t *Max_Value, int16_t *AD, int16_t *NormalizePT )
{
	delay_ms( 100 );      //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();            // IIC��ʼ��
    OLED_Init();	      //��ʼ��OLED
    OLED_Clear( 0,7 );
	
    OLED_ShowString( 8, 6,"-",16 );
    OLED_ShowString( 48,6,"+",16 );	
    
    servo_pwm = servo_mid;
    
    while( 1 )
    {
        decrease_port_config= port_input;   //��������Ϊ����
        increase_port_config = port_input;
        ok_port_config = port_input;
        
        Sampling( ); 
        
        OLED_ShowNum( 32,3,servo_pwm,5,16 );
        if( decrease_port_in == LOW )
        {
            while( decrease_port_in == LOW )
            {
            	if( ok_port_in == LOW )
        		{
        			delay_ms( 100 );
        			while( ok_port_in == LOW );
        			servo_pwm -= 100;
        		}
            }
           	servo_pwm -= 10;                 //�� - ÿ�μ�10
        }
        
        if( increase_port_in == LOW )
        {
            while( increase_port_in == LOW )
            {
            	if( ok_port_in == LOW )
        		{
        			delay_ms( 100 );
        			while( ok_port_in == LOW );
        			servo_pwm += 100;
        		}
            }
           	servo_pwm += 10;                //�� + ÿ�μ�10
        }
        
        if( ok_port_in == LOW )               //�Ȱ�OK�ٰ�+��ΪΪ����100��ֻ��OK��������
        {
        	while( ok_port_in == LOW )
        	{
        		if( increase_port_in == LOW )
        		{
        			delay_ms( 100 );
        			while( increase_port_in == LOW );
//        			sbq( ADaverage[0], ADaverage[1], ADaverage[2], servo_pwm );	
        		}
        		
        		if( decrease_port_in == LOW )
        		{
        			delay_ms( 100 );
        			while( decrease_port_in == LOW );
//        			sbq( ADaverage[0], ADaverage[1], ADaverage[2], servo_pwm );
        		}
        	}
        	sbq( ADaverage[0], ADaverage[1], ADaverage[2], servo_pwm );
        }
    }
}

//********************ͣ�����*************************//
#define   PARK_CAR   SIU.GPDI[PCR50_PD2].R
#define   PARK_CONFIGURE  SIU.PCR[PCR50_PD2].R
uint8_t StopCarCount = 0;
void StopCar()
{
	PARK_CONFIGURE = port_input;
	if( PARK_CAR == LOW )
	{
//		BEE_OUTPUT = HIGH;
	    while( PARK_CAR == LOW );
	    StopCarCount++;
//	    BEE_OUTPUT = LOW;	
	}
	if( flage_tiaosu )
	{
		if( StopCarCount == 2 )
		{
			Speed_PID( smartcar_speed, 0 );
		}
		flage_tiaosu = 0;
	}
}


void Dubug_Mode(int16_t *Max_Valu, int16_t *Noise_Value, int16_t *NormalizePT )
{
	Max_Valu[0]=233;Max_Valu[1]=228;Max_Valu[2]=232;
	Noise_Value[0]=25;Noise_Value[1]=23;Noise_Value[2]=25;
	NormalizePT[0]=51;NormalizePT[1]=53;
	PAR.Speed_Set = 2600;
	PAR.Steer_D = 18;
	PAR.Steer_P = 10;
}

/******************************************************************************/
void main ( void )
{
	int16_t NormalizePT[2]={0};
	int16_t AD[6]={0};
	int16_t Noise_Value[6]={0};         //����ֵ
	int16_t Max_Value[6]  ={0};
	int Speed = 0;
	int temp=0;
    core_config();
    disableIrq();
    initSCI();
    initSTM();
    initPIT();
    initEMIOS();
    initADC();                      // Initialize ADC0 for scan mode
    motor_init();
    PID_init();
    enableIrq();
    

   	Dubug_Mode(Max_Value,Noise_Value,NormalizePT);
    
    BEE_CONFIG = port_output;
    BEE_OUTPUT = LOW;

    delay_ms( 1000 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();         // IIC��ʼ��
    OLED_Init();	   //��ʼ��OLED
    OLED_Clear( 0,7 );
    Desktop();
    /*
    PAR_init();      
    NoiseValue_init( Noise_Value );	
    Find_Transit_front( NormalizePT, Max_Value, Noise_Value );  
    */
    xianshi();
   	delay_ms( 100 );   //��ʾ����ʼ����Ҫһ����ʱ �����޷���ʾ
    initI2C();         // IIC��ʼ��
    OLED_Init();	   //��ʼ��OLED
    OLED_Clear( 0,7 );

    while ( 1 )
    {
    	Sampling( );
		Normalization( ADaverage, Noise_Value, Max_Value, AD, 6 );
        Position_analyse_front( NormalizePT, Max_Value, AD ); 
        temp=ABS(Error,0);
        Servo_PD( AD );
/*
        if(markerror_pointer%ARR_MARKERROR_LENGTH==0) {
        	OLED_ShowNum( 0,0,AD[0],4,16 );
        	OLED_ShowNum( 40,0,AD[1],4,16 );
        	OLED_ShowNum( 80,0,AD[2],4,16 );
        	
        	OLED_ShowNum( 40,3,ABS(Error,0),4,16 );
        	OLED_ShowNum( 40,6,ABS(Errorall,0),4,16 );  	
        }
*/         
        if( StopCarCount != 2 )
        {
	        if( flage_tiaosu )
	        {	
	        	if(temp > 40) {
	        		temp /= 20;
	        		Speed = PAR.Speed_Set-100*temp;
	        	}else 
	        		Speed = PAR.Speed_Set;
	        	Speed_PID( smartcar_speed, Speed );
	            flage_tiaosu = 0;
	        }
        }

        sbq( ABS(Error,0), Position, Speed, smartcar_speed );
    StopCar(); 
    
    }
	
}


