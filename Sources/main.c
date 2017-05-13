#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "MyHeader.h"
/*---------------------------------------------------------------------------------------*/
/*				            		第十二届电磁组		    							 */
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
float sqrt( float x );
void CircuitIdentification(int16_t *AD);
/*---------------------------------------------------------------------------------------*/
#define   servo_pwm      EMIOS_0.CH[21].CBDR.R     //Maximal value is 13333
#define   motor_pwm    	 EMIOS_0.CH[13].CBDR.R     //Maximal value is 8000
#define   servo_max     	1300//右转极限
#define   servo_min     	-1300//左转极限
#define   servo_mid     	5940
#define Error_lishudu_Max  	200
#define Error_c_lishudu_Max 200
/*========================================================================================*/

uint16_t ABS( int x,int y );
int8_t FuHao( int16_t x );
void Delay1us();
void delay_1s();

/*========================================================================================*/
struct motor_PID
{
//==========================================
    int32_t  Kp;       // 比例常数
    int32_t  Ki;       // 积分常数
    int32_t  Kd;       // 微分常数

    int32_t  iError; 		    // Error[0]
    int32_t  LastError;         // Error[-1]
    int32_t  PrevErrorl;        // Error[-2]

    int32_t  PID_DertaU;        // PID增量
    int32_t  PIDOutput;		    // PID输出
    int32_t  Last_PIDOutput;    // 上次PID输出

// 限幅：
    int32_t  Max_iError;            // 限制最大iError，防超调
    int32_t  Min_iError;			// 限制最小iError
    int32_t  Max_iIncpid;           // 限制最大PID增量
    int32_t  Min_iIncpid;			// 限制最小PID增量
    int32_t  Max_otput;             // 限制最大输出量
    int32_t  Min_otput;			    // 限制最小输出量
//==========================================
} PID;
/*************************新算法各类变量*****************************/
int ErrorRate=0;
int ErrorFit=0;int LastErrorFit=0;
int Error=0;int LastError1=0;
int ErrorP=0;int LastErrorP=0;
int ErrorVP=0;int LastErrorVP=0;
int ErrorV=0;int LastErrorV=0;
int LastError=0;
uint16_t ADC_flag = 0;
int16_t ADaverage[6] = {0};		 	    // 采样处理后的均值
int laserror=0;
uint8_t Position=0;
int Distance=1200;
uint8_t P[10]={14,13,12,11,9,7,6,5,4,4};
//uint8_t P[10]={16,15,14,13,12,9,8,7,6,5};
int Thread=400;
int thread=7;
//=====================================================================//
////////////////////////按键、蜂鸣器、指示灯初始化//////////////////////
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

#define   SAMPLING_TIME                       15     //采样次数
#define   ARR_MARKERROR_LENGTH                35


#define  port_output   0x0200                                                           //*
#define  port_input    0x0103
#define  in_put                     0
#define  out_put                    1
#define  LOW                        0
#define  HIGH                       1

struct tagParameter      //定义结构体存储各种需要调节的参数
{
    int16_t Speed_Set;  //设定速度
    int      Steer_P;       //舵机P
    int      Steer_D;       //舵机D
    int      Set_Steer_P;   //舵机基准P
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

    OLED_ShowCHinese( 40,3,8 );  //电
    OLED_ShowCHinese( 56,3,9 );  //磁
    OLED_ShowCHinese( 72,3,10 ); //组

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
        	Distance = number_test;
        	OLED_ShowString( 0,0,"Distance:",16 );
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

        if ( ok_port_in == LOW ) //参数调节完毕后跳出循环
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

    delay_ms( 100 );   //显示屏初始化需要一段延时 否则无法显示
    initI2C();         // IIC初始化
    OLED_Init();	   //初始化OLED
    OLED_Clear( 0,7 );
//--------------------------------------------------------------//
    while( flag == 0 )
    {

        decrease_port_config= port_input;   //按键设置为输入
        increase_port_config = port_input;
        ok_port_config = port_input;


        //***********待修改参数*************************//
        Show_Me_Data( PAR.Speed_Set,100,1 );
        //Show_Me_Data( thread,1,8 );
		Show_Me_Data( PAR.Steer_P,10,3 );
        Show_Me_Data( PAR.Steer_D,5,4 );
        Show_Me_Data( Thread, 25, 7 );
        
        //Show_Me_Data( Distance, 100, 5 );
        
        //Show_Me_Data( PAR.Set_Steer_P, 1, 6 );


        delay_ms( 100 );   //显示屏初始化需要一段延时 否则无法显示
        initI2C();         // IIC初始化
        OLED_Init();	   //初始化OLED
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
                delay_ms( 100 );   //显示屏初始化需要一段延时 否则无法显示
                initI2C();         // IIC初始化
                OLED_Init();	   //初始化OLED
                OLED_Clear( 0,7 );
                break;
            }
        }
    }
}

void show_Maxvalue( int16_t x1, int16_t x2, int16_t x3, int16_t x4, int16_t x5, int16_t x6  ) //输入六个变量 字符串根据需要更改
{
//   OLED_ShowString( 0,0,"M1:",16 );
    OLED_ShowNum( 8,0,x1,4,16 ); //显示数字/**/

//    OLED_ShowString( 48,0,"M2:",16 );
    OLED_ShowNum( 48,0,x2,4,16 ); //显示数字/**/

    OLED_ShowString( 0,3,"    maxvalue",16 );
    OLED_ShowNum( 88,0,x3,4,16 ); //显示数字/**/

//    OLED_ShowString( 0,6,"M4:",16 );
    OLED_ShowNum( 8,6,x4,4,16 ); //显示数字/**/

//    OLED_ShowString( 0,6,"M5:",16 );
    OLED_ShowNum( 48,6,x5,4,16 ); //显示数字/**/

//    OLED_ShowString( 0,6,"M6:",16 );
    OLED_ShowNum( 88,6,x6,4,16 ); //显示数字/**/

}

void show_ADaverage( ) //输入六个变量 字符串根据需要更改
{

	delay_ms( 100 );     //显示屏初始化需要一段延时 否则无法显示
    initI2C();           // IIC初始化
    OLED_Init();	     //初始化OLED
    OLED_Clear( 0,7 );

	while ( 1 )
    {

        ok_port_config = port_input;

        servo_pwm = servo_mid;

        if ( ok_port_in == LOW )     //双击OK确认
        {
            while ( ok_port_in == LOW );
            break;
        }
        Sampling();

	    //OLED_ShowString( 0,0,"M1:",16 );
	    OLED_ShowNum( 8,0,ADaverage[0],4,16 ); //显示数字/**/

	    //OLED_ShowString( 48,0,"M2:",16 );
	    OLED_ShowNum( 48,0,ADaverage[1],4,16 ); //显示数字/**/

	    OLED_ShowString( 0,3,"    ADaverage",16 );
	    OLED_ShowNum( 88,0,ADaverage[2],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M4:",16 );
	    OLED_ShowNum( 8,6,ADaverage[3],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M5:",16 );
	    OLED_ShowNum( 48,6,ADaverage[4],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M6:",16 );
	    OLED_ShowNum( 88,6,ADaverage[5],4,16 ); //显示数字/**/

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
int Least_Square_Method(int n, int* data,int start) {
	int i, K=0,sumxy = 0, sumx = 0, sumy = 0, sumxx = 0, Numerator = 0, Denominator;
	for (i = 0; i < n; i++) {
		start%=ARR_MARKERROR_LENGTH;
		sumxy += (i*data[start]);
		sumx += i;
		sumy += data[start];
		sumxx += (i*i);
		start++;
	}
	Numerator  = (n*sumxy - sumx*sumy)*100;
	Denominator = n*sumxx - sumx*sumx;
	if (Denominator == 0)
		Denominator = 1;
	K = Numerator / Denominator;
	return K;
}
/*-------------------------------------------------------------------*/
inline int limit(float x,float n) {
	if(x-n>9)
		x=n+9;
	else if(n-x>9)
		x=n-9;
	return x;
}
inline int limit1(int x,int min,int max) {
	if(x<min)
		return min;
	else if(x>max)
		return max;
}
/*-------------------------------------------------------------------*/
inline int FindMax(int16_t* AD,int n) {
	int max=0,i=0;
	for(i=0;i<n;i++) {
		if(AD[i]>=max)
			max=AD[i];
	}
	return max;
}
/*-------------------------------------------------------------------*/
int dir=0;
uint16_t LostTime=0;
inline int lost(float error,float Last,int16_t *AD) {
	int min=30,max=80,i;
	uint8_t value=FindMax(AD,3);
	if(dir!=0) {
		if((value<=8&&ABS(AD[0],AD[2])<6)||(dir==1&&AD[0]>=AD[2])||(dir==-1&&AD[2]>=AD[0])) {
			error=Last;
		}
	}
	if(AD[1]<min&&dir==0) {
		if(error<0)
			dir=-1;
		else if(error>0)
			dir=1;
	} else if(AD[1]>max) {
		dir=0;	
	}
	if(error-Last>7)
		error+=7;
	else if(Last-error>7)
		error-=7;
	/*
	if(value<4) {
		if(LostTime++>1500) {
			PAR.Speed_Set=0;
		}
	} else {
		LostTime=0;
	}
	*/
	return error;
	//if((a<10||b<10)&&ABS(a,b)<6/*||AD[1]<thread*/) {

}
inline int lost1(float error,float Last,int16_t *AD) {
	int min=30,max=80,i;
	uint8_t value=FindMax(AD,3);
	if(dir!=0) {
		if((value<=8&&ABS(AD[0]+AD[3],AD[2]+AD[4])<6)||(dir==1&&AD[0]+AD[3]>=AD[2]+AD[4])||(dir==-1&&AD[2]+AD[4]>=AD[0]+AD[3])) {
			error=Last;
		}
	}
	if(AD[1]<min&&dir==0) {
		if(error<0)
			dir=-1;
		else if(error>0)
			dir=1;
	} else if(AD[1]>max) {
		dir=0;	
	}
	if(error-Last>7)
		error+=7;
	else if(Last-error>7)
		error-=7;
	/*
	if(value<4) {
		if(LostTime++>1500) {
			PAR.Speed_Set=0;
		}
	} else {
		LostTime=0;
	}
	*/
	return error;
	//if((a<10||b<10)&&ABS(a,b)<6/*||AD[1]<thread*/) {

}
/*-------------------------------------------------------------------*/
//int CalculationErrorLost(float P1,float P2,float Last,int mixLimit,int maxLimit) {
//	float error=(sqrt(P1)-sqrt(P2))/(P1+P2);
//}
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
////////////////////////////////////延时函数//////////////////////////////////////
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
int32_t Car_Distance=0;     //从发车到结束行驶的距离
uint32_t per_distance=0;	//编码器计算距离
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
    if( EMIOS_0.CH[19].CCR.B.EDPOL==0 ) //检测是否在高电平
    {
        period_yy=yy_overflow*65535+EMIOS_0.CH[19].CADR.R-jishuA;//自由计数值
        if( SIU.GPDI[PCR6_PA6].R==1 ) //脉冲有效
        {
            time_y_y=period_yy/16;	//时间的1000倍即ms
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
    distance_yy=( time_yy[0]+time_yy[1]+time_yy[2]+time_yy[3]+time_yy[4] )*34/400; //单位mm
    if( ABS( distance_yy,lastdistance_yy )>300 )distance_yy=( 9*lastdistance_yy+distance_yy )/10;

    jishuA=EMIOS_0.CH[19].CADR.R;
    yy_overflow=0;

    EMIOS_0.CH[19].CCR.B.EDPOL =~EMIOS_0.CH[19].CCR.B.EDPOL; //改变中断边沿
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

    period_sum = period_sum+count;//脉冲累加

    if( period_sum_times == smartcar_speed_time )
    {
        period = period_sum/period_sum_times;
        //B车车速计算公式：（mm/s）
        //..............频率(MHz)--线束--小齿--轮径*10---period--------大齿//
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
        

        per_distance  = smartcar_speed_1 * period/160000; //100000倍
        sum_distance  = sum_distance+per_distance;
        distance_bian = 63*distance_time/840;

        Car_Distance = 63*Distance_Count/840;

        flage_tiaosu = 1;
    }

    EMIOS0_overflow_times_Cesu = 0;

    EMIOS_0.CH[20].CSR.B.FLAG = 1;

}

/*********************************蓝牙数据传送/接收*****************************************/
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
//-------------无线通讯数据接收/发送：
uint8_t MOTE=0,gaowei_flag=0,diwei_flag=0;
int16_t DATAshou[6]= {0},R_flag=0;		//五个接收数据变
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
//*******************************ADC采样函数***************************************
/*	  单片机电感接口位置
						ANP13 31
ANP3 27		ANP2 用不了	ANP1 43
			ANP11 38	ANP10 26
			ANP8 26		ANP9 25
-----------------------------
		两排电感排布
 	 AD[0]	 AD[1]	 AD[2]
	ANP[08] ANP[9]	ANP[10]


	 AD[3]   AD[4]	 AD[5]
	ANP[2]	ANP[1]  ANP[13] 
*/
/*
1 1
2 4
3 8
8 100
9 200
10 400
11 800
13 2000

8 	0
9 	1
11 	2
10 	3
13 	4

*/

void Sampling( void )       
{
    uint16_t i=0,j=0;
    uint16_t AD_SUM=0;
    uint16_t samplearray[6][SAMPLING_TIME]= {0};		// 直接的ADC采样值
	//------------------------------------------------------------------------------------------
    ADC.NCMR[0].R = 0x00002000;     			   /* Select ANP  13 */
    ADC.MCR.B.NSTART=1;             			   /* Trigger normal conversions for ADC0 */
    for( i=0; i<SAMPLING_TIME; i++ )
    { 
        while ( ADC.CDR[13].B.VALID == 0 ) {};  	 /* Wait for last scan to complete */
        samplearray[4][i] = ADC.CDR[13].B.CDATA;  /* Read ANS0 conversion result data */
    }
    ADC.MCR.B.NSTART=0;
    
//------------------------------------------------------------------------------------------
    ADC.NCMR[0].R = 0x00000008;     			   /* Select ANP  3 */
    ADC.MCR.B.NSTART=1;             			   /* Trigger normal conversions for ADC0 */
    for( i=0; i<SAMPLING_TIME; i++ )
    {
        while ( ADC.CDR[3].B.VALID == 0 ) {};  	 /* Wait for last scan to complete */
        samplearray[3][i] = ADC.CDR[3].B.CDATA;  /* Read ANS0 conversion result data */
    }
    ADC.MCR.B.NSTART=0;
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
    ADC.NCMR[0].R = 0x000100;     			   /* Select ANP  8 */
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
        
        for( i=0; i<5; i++ )
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
    // 正反转控制：
    
    if( zkb > 0 )
    {
    
    	BEE_OUTPUT = LOW;
    
        SIU.GPDO[PCR71_PE7].R  = 1;		// 0-1正转
        SIU.GPDO[PCR40_PC8].R  = 0;

        motor_pwm = zkb;
    }
    if( zkb < 0 )
    {
    
    	BEE_OUTPUT = HIGH;
    	
        SIU.GPDO[PCR71_PE7].R = 0;		// 1-0反转
        SIU.GPDO[PCR40_PC8].R = 1;

        motor_pwm = 0-zkb;
    }
//------------------------------------
}
/*=========================================================================================*/
//********************************速度PID参数初始化********************************
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

    /////*******限幅*****////
    PID.Max_iError  =  3000;
    PID.Min_iError  = -3000;
    PID.Max_iIncpid =  6500;
    PID.Min_iIncpid = -6500;
    PID.Max_otput   =  7000;
    PID.Min_otput   = -6000;
//-----------------------------------
}

//******************速度PID调节***************************
void Speed_PID( int Current_Speed,int SetPoint )
{
    PID.iError = SetPoint-Current_Speed;  //增量计算 当前误差
    //************限制条件**************************************
    // 限制误差：
    if( PID.iError>=PID.Max_iError )
    {
        PID.iError = PID.Max_iError;
    }
    if( PID.iError<= PID.Min_iError )
    {
        PID.iError = PID.Min_iError;
    }
    //***********************************************************
    // PID_L增量计算：
    PID.PID_DertaU = ( PID.Kd*( PID.PrevErrorl+PID.iError-2*PID.LastError )
                       +PID.Kp*( PID.iError-PID.LastError )
                       +PID.Ki*PID.iError )/1000;
    //***********************************************************
    //存储误差，用于下次计算
    PID.PrevErrorl =  PID.LastError;
    PID.LastError  =  PID.iError;

    //****** ********限制判断*** **********************************
    // 限制增量：
    if( PID.PID_DertaU>= PID.Max_iIncpid )
    {
        PID.PID_DertaU= PID.Max_iIncpid;
    }

    if( PID.PID_DertaU<= PID.Min_iIncpid )
    {
        PID.PID_DertaU= PID.Min_iIncpid;
    }
    //**************************************************************
    // PID输出：
    PID.PIDOutput= PID.Last_PIDOutput+PID.PID_DertaU;

    //**************************************************************
    // 限制输出：
    if( PID.PIDOutput <= PID.Min_otput )
    {
        PID.PIDOutput= PID.Min_otput;
    }

    if( PID.PIDOutput>= PID.Max_otput )
    {
        PID.PIDOutput= PID.Max_otput;
    }
    //**************************************************************
    PID.Last_PIDOutput=PID.PIDOutput;  // PID输出存储
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
    motor_drive( PID.PIDOutput ); //驱动电机
//----------------------------------------
}

/**********************************冒泡排序***************************************************/
void Bubble_Sort( uint16_t *pList, uint8_t index )
{
	//利用冒泡排序算法，完成对数组list中的index个数进行排序。
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
                flag = 1;             //检测是否进行了交换
            }
        }
        if( flag == 0 )              //标志位为0说明已经完成排序
        	break;
    }
}

/*******************************去首尾极值并返回平均值****************************************/

/**形参变量说明：
1. pList:       实参数组的首地址
2. index:       数组里需要排序的元素个数
3. HeadCutNum : 前面极值去除个数
4. TailCutNum : 后面极值去除个数

/**/
uint16_t Extremum_Cut(uint16_t *pList, uint8_t index, uint8_t HeadCutNum, uint8_t TailCutNum )
{
	uint8_t i=0;
	uint32_t sum = 0;
	uint16_t average = 0;
	uint8_t record = index-TailCutNum;
	
	if(index < HeadCutNum+TailCutNum )     //防止指针跑飞
		return 0;
	
	for( i=HeadCutNum; i<record; i++ )
	{
		sum += pList[i];
	}
	average = sum/( record - HeadCutNum );
	return average;
}

/**************************对底噪信号采集(初始化底噪值)********************************************/

void NoiseValue_init( int16_t *Noise_Value )   
{
	int i=0,j=0;
	uint16_t arrRecordValue[6][200];
	
	delay_ms( 100 );     //显示屏初始化需要一段延时 否则无法显示
    initI2C();           // IIC初始化
    OLED_Init();	     //初始化OLED
    OLED_Clear( 0,7 );
        
    for( i=0; i<30; i++ )        //空采样30次
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

		if ( ok_port_in == LOW )     //双击OK确认
		{
		    while ( ok_port_in == LOW );
		    break;
		}        
	    //OLED_ShowString( 0,0,"M1:",16 );
	    OLED_ShowNum( 8,0,Noise_Value[0],4,16 ); //显示数字/**/

	    //OLED_ShowString( 48,0,"M2:",16 );
	    OLED_ShowNum( 48,0,Noise_Value[1],4,16 ); //显示数字/**/

	    OLED_ShowString( 0,3,"   Noise_Value",16 );
	    OLED_ShowNum( 88,0,Noise_Value[2],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M4:",16 );
	    OLED_ShowNum( 8,6,Noise_Value[3],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M5:",16 );
	    OLED_ShowNum( 48,6,Noise_Value[4],4,16 ); //显示数字/**/

	    //OLED_ShowString( 0,6,"M6:",16 );
	    OLED_ShowNum( 88,6,Noise_Value[5],4,16 ); //显示数字/**/
    
	}
	
}

/**********************************归一化*******************************************/

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

/***********************************前排找过渡点及电感最大值*************************************/
/*
	
	
*/
/********************************************************************************************/
void Find_Max( int16_t *pMax_Value, uint8_t index )
{
    //找电感最大值
    uint8_t i = 0;
    uint8_t j = 0;

    delay_ms( 100 );   //显示屏初始化需要一段延时 否则无法显示
    initI2C();         // IIC初始化
    OLED_Init();	   //初始化OLED
    OLED_Clear( 0,7 );

    ADC_flag = 0;                       //采样标志位清零
    while ( 1 )
    {
        ok_port_config = port_input;

        servo_pwm = servo_mid;

        if ( ok_port_in == LOW )     //双击OK确认
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

void Find_Transit_front( int16_t *pNormalizePT, int16_t *Max_Value, int16_t *Noise_Value )//归一化以后的过渡点值 //电感的最大值
{
    //前排找过渡点
    uint8_t TransitCount[2]={0};           //记录过渡点的个数
    uint16_t Temp[2][20]= {0};            //记录采集到的过渡点的值
    uint8_t k = 0;                       //循环标志位
    uint8_t i = 0; 
    uint8_t j = 0;
    uint16_t sum[2]= {0};                //存储过渡点值的和
    
    int16_t Transit[2]={0};             //存储滤波处理以后的过渡点的值                
    ok_port_config = port_input;
    
    Find_Max( Max_Value,6 );
    
    delay_ms( 100 );   //显示屏初始化需要一段延时 否则无法显示
    initI2C();         // IIC初始化
    OLED_Init();	   //初始化OLED
    OLED_Clear( 0,7 );
    
    while( ok_port_in == LOW );              //消除前面按键操作对后面程序的影响

    while ( k<2 )
    {
        ok_port_config = port_input;

        if ( ok_port_in == LOW )     //OK键退出查找过渡点，进入处理模式
        {
            while ( ok_port_in == LOW );
                k = 1;
        }

		for ( j = 0; j<20; j++ )    //适当增加循环次数，使运行该函数时单片机更多的时间用在寻找过渡点上
        {
            Sampling();
            
            if( ADaverage[0] >= Max_Value[0]-2 )
            {
                if( TransitCount[0] < 20 )      //记满20个值就不再查找
                {
                    Temp[0][TransitCount[0]]=ADaverage[1];
                    TransitCount[0]++;
                }
			}
			
            if( ADaverage[2] >= Max_Value[2]-2 )
            {
            	if( TransitCount[1] < 20 )     //记满20个值就不再查找
            	{
	            	Temp[1][TransitCount[1]] = ADaverage[1];
	                TransitCount[1]++;
            	}
            }
            if( TransitCount[0]==20 && TransitCount[1]==20 )  //当两个过渡点都装满以后停止查找,进入处理模式
            	k = 1;
        }
    	OLED_ShowNum( 8,0,TransitCount[0],3,16 );        //显示找到过渡点的个数
 		OLED_ShowNum( 64,0,TransitCount[1],3,16 );
        
        if( k == 1 )                                    //进入处理模式
        {
        	Bubble_Sort( Temp[0], TransitCount[0] );
        	Bubble_Sort( Temp[1], TransitCount[1] );
        	Extremum_Cut(Temp[0], TransitCount[0], 2, 2 );
        	Extremum_Cut(Temp[1], TransitCount[1], 2, 2 );
	      	Transit[0] = Extremum_Cut(Temp[0], TransitCount[0], 2, 2 );
	 		Transit[1] = Extremum_Cut(Temp[0], TransitCount[1], 2, 2 );
	 		
	 		Normalization( Transit, Noise_Value, Max_Value, pNormalizePT, 2 );     //过渡点归一化
	 		
	 		OLED_ShowNum( 8,0,pNormalizePT[0],3,16 );                               //显示数字/**/
	 		OLED_ShowNum( 64,0,pNormalizePT[1],3,16 );                               //显示数字/**/
        	OLED_ShowString( 0,3,"  PTnormalize",16 );
        	OLED_ShowString( 80,6,"NEXT",16 );
        	
        	while( ok_port_in == 1 );    //让程序停留在显示过渡点值的界面
        	delay_ms( 200 );
        	while( ok_port_in == 0 );
        	break;
        } 
    }
}

/******************************A/D滤波********************************************/
#define   ARR_FILER_AD_LENGTH          5
int16_t arrAD_Filter[3][ARR_FILER_AD_LENGTH] = {0};
int8_t  AD_Filtering_Pointer = 0;

void Filter_AD( int16_t *AD_Value )
{
	int32_t sum = 0;
	int8_t i = 0, j = 0;
	
	AD_Filtering_Pointer %= ARR_FILER_AD_LENGTH;         //使用循环队列存储
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

/*****************************Error滤波**************************************/
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
	
	Error_Filtering_Pointer %= ARR_FILER_ERROR_LENGTH;         //使用循环队列存储
	arrFilter_Error[Error_Filtering_Pointer] = nError;
	Error_Filtering_Pointer++;
		
    for( i = 0; i < ARR_FILER_ERROR_LENGTH; i ++ )
    {
		sum += arrFilter_Error[i];
    }
	ave = sum / ARR_FILER_ERROR_LENGTH;
	return ave;
}
/***************************************赛道识别*******************************************/
uint8_t CrossFlag=0;
uint8_t StraightFlag=0;
uint8_t	CircuitFlag=0;
uint8_t CurveFlag=0;
uint8_t EnterCurve=0;
void TrackIdentification(int16_t *AD) {
	if(AD[0]+AD[1]+AD[2]>330&&ABS(AD[3],AD[4])<70) {
		StraightFlag=1;
	}
	if(AD[0]+AD[2]>240&&AD[3]+AD[4]>240&&ABS(AD[3],AD[4])<40) {
		CrossFlag=1;
	}
	if(AD[1]>80&&AD[1]<180&&AD[3]>140||AD[4]>140&&ABS(AD[3],AD[4])>60) {
		EnterCurve=1;
	}
	if(AD[0]+AD[3]-AD[2]-AD[4]>80&&AD[3]+AD[4]>90&&AD[3]+AD[4]<240||(ABS(AD[3],AD[4])>50&&AD[3]+AD[4]<140)) {
		CurveFlag=0;
	}
	if(AD[0]+AD[2]>200&&AD[3]+AD[4]<90) {
		CurveFlag=0;
	}
}
/***************************************环岛识别*******************************************/
void CircuitIdentification(int16_t *AD) {
	if(CircuitFlag==0) {
		if(AD[3]<90&&AD[3]>20&&AD[4]<90&&AD[4]>20&&ABS(AD[3],AD[4])<30)
			CircuitFlag=1;
		if(AD[1]>=AD[0]&&AD[1]>=AD[2]&&AD[0]>20&&AD[1]>20&&AD[2]>20&&AD[0]<80&&AD[1]<80&&AD[2]<80&&ABS(AD[0],AD[1])<25&&ABS(AD[2],AD[1])<25)
			CircuitFlag=1;		
	} else if(CircuitFlag==1&&AD[1]>120) {
		CircuitFlag=0;	
	}
	if(CircuitFlag==1) {
		BEE_OUTPUT=HIGH;
	} else {
		BEE_OUTPUT=LOW;
	}
		
}
/***************************************十字识别*******************************************/
void CrossIdentification(int16_t *AD) {
	if(CrossFlag==0&&(AD[0]+AD[2]>240&&AD[3]+AD[4]>240&&ABS(AD[3],AD[4])<40)||(AD[3]>100&&AD[4]>100)) {
		CrossFlag=1;
	} else if(CrossFlag==1) {
		
	}
}
/***************************************位置解算*******************************************/
int Errorall=0;
uint8_t markerror_pointer = 0;
uint8_t count_markerror = 0;        //对error循环队列中元素计数
int MarkError[ARR_MARKERROR_LENGTH] = {0};
uint8_t  Max_Cross = 1;           //初始化值给1，使其开始记录上次的位置
uint16_t Max_Old = 0;         //记录上一次的最大电感位置
uint8_t Position_Old = 1;
int ErrorM=0;

void  Position_analyse_front( int16_t *PT, int16_t *Max_Value, int16_t *AD )
{

	uint8_t Max_Position=0;     //记录当前的最大电感位置
    uint8_t i=0;
    uint16_t Max_V=0;           //记录当前最大电感值
    int Error1=0;  
    int Error2=0;       
	int16_t WeightP_Mid=0;
	int16_t WeightP_P=0;
	int16_t WeightVP=0;
	int16_t WeightV=0;
	int16_t WeightFit=0;
	//Filter_AD( AD );
    
    /*--------------------------找最大电感位置--------------------------*/
    for( i=0; i<3; i++ )
    {
    	if( AD[Max_Position] < AD[i] )
    	{
    		Max_Position = i;
    	}  
    }
    Max_V = AD[Max_Position];    //不加这句话下面就无法判断丢线
    
/*-----------------------------丢线处理并给出最大电感值------------------------------*/

    if( Max_V <= 20 )
    {
    	Max_Position = Max_Old;
    }
    Max_Old = Max_Position;   //不可能一开始就丢线,所以先确定old的值
    Max_V = AD[Max_Position];
    
/*-----------------------------------防串道----------------------------------------*/
    
    if( ABS( Max_Position, Max_Cross ) >= 2 )  //检测突变，正常赛道行驶是不会出现位置跳变的(相减为1)
    {
    	Max_Position = Max_Cross;
    }
    Max_Cross = Max_Position;
    
/*-------------------------中间电感相对于中线位置解算-------------------------------*/

    if( Max_Position==0 && AD[1]<( PT[0]-1 ) )
    {
        Position=0;		    	//中间电感已经偏离0过渡点
    }
    else if( ( Max_Position==1 && ( AD[0] - AD[2] ) > 1 ) || ( Max_Position==0 && ( AD[1] >= PT[0] ) ) )
    {
        Position=1;				//中间电感在0过渡点和中线之间   
        
    }
    else if( ( Max_Position==1 && ( AD[2] - AD[0] ) > 1 ) || ( Max_Position==2 && ( AD[1] >= PT[1] ) ) )
    {
        Position=2;		    	//中间电感在1过渡点和中线之间
        
    }
    else if( Max_Position==2 && ( AD[1] <= PT[1] - 1 ) )
    {
        Position=3;		    	//中间电感已经偏离1过渡点
    }
    
/*-------------------------防位置跳变-------------------------------*/

    if( ABS( Position, Position_Old ) >= 2 )  //检测突变，正常赛道行驶是不会出现位置跳变的(相减为1)
    {
    	Position = Position_Old;
    }
    Position_Old = Position;
/*-----------------------------赛道识别----------------------------------*/
   // CircuitIdentification(AD);
/*-----------------------------偏差计算与滤波----------------------------------*/
	/*水平偏差*/
	ErrorP=lost(((sqrt((float)AD[2])-sqrt((float)AD[0]))/((float)AD[2]+(float)AD[0]+1)*Thread),(float)LastErrorP,AD);
	/*水平垂直偏差*/
	ErrorVP=lost1(limit1(((sqrt((float)AD[2]+(float)AD[4])-sqrt((float)AD[0]+(float)AD[3]))/((float)AD[2]+(float)AD[0]+1)*Thread),-400,400),(float)LastErrorVP,AD);
	/*垂直偏差*/
	ErrorV=limit1(AD[4]-AD[3],-100,100);
/*------------------------------偏差融合----------------------------------*/
	ErrorFit=((200-AD[1])*ErrorVP+2*AD[1]*ErrorP)/(200+AD[1]);
/*------------------------------偏差权值计算----------------------------------*/
    if(AD[1]>90) {
    	WeightV=430-limit1(AD[3]+AD[4],200,400);
    } else {
    	WeightV=ABS(AD[3],AD[4]);
    }
	WeightFit=10+ABS(ErrorFit,0);
	
	//Error=(WeightFit*ErrorFit+WeightV*ErrorV)/(WeightFit+WeightV);
	//Error=ErrorP;
	Error=ErrorFit;
/*-----------------------------偏移量存储---------------------------------*/
		
	
	LastErrorP=ErrorP;
	LastErrorVP=ErrorVP;
	LastErrorFit=ErrorFit;
	LastErrorV=ErrorV;
	LastError1=Error; 
	
   	markerror_pointer %= ARR_MARKERROR_LENGTH;         //使用循环队列存储Error来计算变化率
	MarkError[markerror_pointer] = Error;
    LastError = MarkError[( markerror_pointer + 1 )%ARR_MARKERROR_LENGTH ];   //宏定义 ARR_MARKERROR_LENGTH 次之前的error
    markerror_pointer++;
}
/******************************模糊控制相关变量*************************************/

int SetError[7]={-85 ,  -65 , -40,  0 , 40 ,  65 , 85};
int SetErrorRate[7]={-25 , -19 , -10 , 0 , 10 , 19 , 25};
int KPdan[7] = { 475, 400, 340, 140, 340, 400, 475 };//原来值的乘5
int KDdan[7] = { 760, 700, 640, 475, 640, 700, 760 };
//int KPdan[7] = { 95, 80, 68, 28, 68, 80, 95 };
//int KDdan[7] = { 152,140,128, 95, 128, 140,152 };
int KPrule[7][7] = { 	
	//误差变化率  0---1---2---3---4---5---6      //误差                                   
	0, 0, 0, 0, 1, 2, 3,    //0  -3
	0, 0, 0, 1, 2, 3, 3,    //1  -2 
	0, 1, 1, 2, 3, 3, 4,    //2  -1   
	1, 2, 2, 3, 4, 4, 5,    //3   0  
	2, 3, 3, 4, 5, 5, 6,    //4   1 
	3, 3, 4, 5, 6, 6, 6,    //5   2  
	3, 4, 5, 6, 6, 6, 6     //6   3  
};			//KP规则表
int KDrule[7][7] = { 	
	//误差变化率  0---1---2---3---4---5---6      //误差                                   
	0, 0, 0, 0, 1, 2, 3,    //0  -3
	0, 0, 0, 1, 2, 3, 3,    //1  -2 
	0, 1, 1, 2, 3, 3, 4,    //2  -1   
	1, 2, 2, 3, 4, 4, 5,    //3   0  
	2, 3, 3, 4, 5, 5, 6,    //4   1 
	3, 3, 4, 5, 6, 6, 6,    //5   2  
	3, 4, 5, 6, 6, 6, 6     //6   3  
};			//KP规则表
/******************************获得P值*********************************************************/
int FuzzyKP() {
	int Numerator=0,Denominator=0,i=0;
	int KPe=0,KPe_lishudu=0,KPec=0,KPec_lishudu=0;
	int Temp[4][4]={0};
	/*-----------------------------偏移量求KPe和KPe_lishudu----------------------------------*/
	if (Error >= SetError[0] && Error < SetError[6]) {
		if (Error < (SetError[0] + SetError[1]) / 2) {
			KPe = 0;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[0]) / (SetError[1] - SetError[0]);
		}else if (Error<SetError[1]){
			KPe = 0;
			KPe_lishudu = Error_lishudu_Max * (SetError[1] - Error) / (SetError[1] - SetError[0]);
		}
		else if (Error < (SetError[1] + SetError[2]) / 2) {
			KPe = 1;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[1]) / (SetError[2] - SetError[1]);
		}
		else if (Error<SetError[2]) {
			KPe = 1;
			KPe_lishudu = Error_lishudu_Max * (SetError[2] - Error) / (SetError[2] - SetError[1]);
		}
		else if (Error < (SetError[2] + SetError[3]) / 2) {
			KPe = 2;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[2]) / (SetError[3] - SetError[2]);
		}
		else if (Error<SetError[3]) {
			KPe = 2;
			KPe_lishudu = Error_lishudu_Max * (SetError[3] - Error) / (SetError[3] - SetError[2]);
		}
		else if (Error < (SetError[3] + SetError[4]) / 2) {
			KPe = 3;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[3]) / (SetError[4] - SetError[3]);
		}
		else if (Error<SetError[4]) {
			KPe = 3;
			KPe_lishudu = Error_lishudu_Max * (SetError[4] - Error) / (SetError[4] - SetError[3]);
		}
		else if (Error < (SetError[4] + SetError[5]) / 2) {
			KPe = 4;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[4]) / (SetError[5] - SetError[4]);
		}
		else if (Error<SetError[5]) {
			KPe = 4;
			KPe_lishudu = Error_lishudu_Max * (SetError[5] - Error) / (SetError[5] - SetError[4]);
		}
		else if (Error < (SetError[5] + SetError[6]) / 2) {
			KPe = 5;
			KPe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[5]) / (SetError[6] - SetError[5]);
		}
		else if (Error<SetError[6]) {
			KPe = 5;
			KPe_lishudu = Error_lishudu_Max * (SetError[6] - Error) / (SetError[6] - SetError[5]);
		}
	}else if (Error<SetError[0]) {
		KPe = 0;
		KPe_lishudu = Error_lishudu_Max;
	}else {
		KPe = 5;
		KPe_lishudu = 0;
	}

	/*-----------------------------偏移量变化率求KPec和KPec_lishudu----------------------------------*/
	if (ErrorRate >= SetErrorRate[0] && ErrorRate < SetErrorRate[6]) {
		if (ErrorRate < (SetErrorRate[0] + SetErrorRate[1]) / 2) {
			KPec = 0;
			KPec_lishudu = Error_c_lishudu_Max - Error_c_lishudu_Max * (ErrorRate - SetErrorRate[0]) / (SetErrorRate[1] - SetErrorRate[0]);
		}
		else if (ErrorRate<SetErrorRate[1]){
			KPec = 0;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[1] - ErrorRate) / (SetErrorRate[1] - SetErrorRate[0]);
		}
		else if (ErrorRate < (SetErrorRate[1] + SetErrorRate[2]) / 2) {
			KPec = 1;
		}
		else if (ErrorRate<SetErrorRate[2]) {
			KPec = 1;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[2] - ErrorRate) / (SetErrorRate[2] - SetErrorRate[1]);
		}
		else if (ErrorRate < (SetErrorRate[2] + SetErrorRate[3]) / 2) {
			KPec = 2;
		}
		else if (ErrorRate<SetErrorRate[3]) {
			KPec = 2;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[3] - ErrorRate) / (SetErrorRate[3] - SetErrorRate[2]);
		}
		else if (ErrorRate < (SetErrorRate[3] + SetErrorRate[4]) / 2) {
			KPec = 3;
		}
		else if (ErrorRate<SetErrorRate[4]) {
			KPec = 3;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[4] - ErrorRate) / (SetErrorRate[4] - SetErrorRate[3]);
		}
		else if (ErrorRate < (SetErrorRate[4] + SetErrorRate[5]) / 2) {
			KPec = 4;
		}
		else if (ErrorRate<SetErrorRate[5]) {
			KPec = 4;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[5] - ErrorRate) / (SetErrorRate[5] - SetErrorRate[4]);
		}
		else if (ErrorRate < (SetErrorRate[5] + SetErrorRate[6]) / 2) {
			KPec = 5;
		}
		else if (ErrorRate<SetErrorRate[6]) {
			KPec = 5;
			KPec_lishudu = Error_c_lishudu_Max * (SetErrorRate[6] - ErrorRate) / (SetErrorRate[6] - SetErrorRate[5]);
		}
	}
	else if (ErrorRate<SetErrorRate[0]) {
		KPec = 0;
		KPec_lishudu = Error_c_lishudu_Max;
	}
	else {
		KPec = 5;
		KPec_lishudu = 0;
	} 
	/*找规则表中的区域*/
	Temp[0][0] = KPrule[KPec][KPe];
	Temp[0][1]= KPrule[KPec][KPe+1];
	Temp[0][2] = KPrule[KPec+1][KPe];
	Temp[0][3] = KPrule[KPec+1][KPe+1];
	/*用Error找到的隶属度*/
	Temp[1][0] = KPe_lishudu;
	Temp[1][1] = Error_lishudu_Max - KPe_lishudu;
	Temp[1][2] = KPe_lishudu;
	Temp[1][3] = Error_lishudu_Max - KPe_lishudu;
	/*用Error变化率找到的隶属度*/
	Temp[2][0] = KPec_lishudu;
	Temp[2][1] = Error_c_lishudu_Max - KPec_lishudu;
	Temp[2][2] = KPec_lishudu;
	Temp[2][3] = Error_c_lishudu_Max - KPec_lishudu;
	/*确定四个点的隶属度(取2隶属度小的给1)*/
	for (i = 0; i < 4; i++) {
		if (Temp[1][i]>Temp[2][i]) {
			Temp[1][i] = Temp[2][i];
		}
	}
	/*小中取大，规则表中同等区域隶属度取大*/
	if (Temp[0][0] == Temp[0][1]) {
		if (Temp[1][0] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][0] == Temp[0][2]) {
		if (Temp[1][0] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][0] == Temp[0][3]) {
		if (Temp[1][0] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][1] == Temp[0][2]) {
		if (Temp[1][1] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][1] = 0;
	}
	if (Temp[0][1] == Temp[0][3]) {
		if (Temp[1][1] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][1] = 0;
	}
	if (Temp[0][2] == Temp[0][3]) {
		if (Temp[1][2] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][2] = 0;
	}

	for (i = 0; i < 4; i++) {
		Numerator += (KPdan[Temp[0][i]]*Temp[1][i]) ;
		Denominator += Temp[1][i];
	}
	if (Denominator < 2)
		Denominator = 1;
	return Numerator / Denominator;
}
/******************************获得D值*********************************************************/
int FuzzyKD() {
	int Numerator = 0, Denominator = 0, i = 0;
	int KDe = 0, KDe_lishudu = 0, KDec = 0, KDec_lishudu = 0;
	int Temp[4][4] = { 0 };
	/*-----------------------------偏移量求KDe和KDe_lishudu----------------------------------*/
	if (Error >= SetError[0] && Error < SetError[6]) {
		if (Error < (SetError[0] + SetError[1]) / 2) {
			KDe = 0;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[0]) / (SetError[1] - SetError[0]);
		}
		else if (Error<SetError[1]){
			KDe = 0;
			KDe_lishudu = Error_lishudu_Max * (SetError[1] - Error) / (SetError[1] - SetError[0]);
		}
		else if (Error < (SetError[1] + SetError[2]) / 2) {
			KDe = 1;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[1]) / (SetError[2] - SetError[1]);
		}
		else if (Error<SetError[2]) {
			KDe = 1;
			KDe_lishudu = Error_lishudu_Max * (SetError[2] - Error) / (SetError[2] - SetError[1]);
		}
		else if (Error < (SetError[2] + SetError[3]) / 2) {
			KDe = 2;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[2]) / (SetError[3] - SetError[2]);
		}
		else if (Error<SetError[3]) {
			KDe = 2;
			KDe_lishudu = Error_lishudu_Max * (SetError[3] - Error) / (SetError[3] - SetError[2]);
		}
		else if (Error < (SetError[3] + SetError[4]) / 2) {
			KDe = 3;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[3]) / (SetError[4] - SetError[3]);
		}
		else if (Error<SetError[4]) {
			KDe = 3;
			KDe_lishudu = Error_lishudu_Max * (SetError[4] - Error) / (SetError[4] - SetError[3]);
		}
		else if (Error < (SetError[4] + SetError[5]) / 2) {
			KDe = 4;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[4]) / (SetError[5] - SetError[4]);
		}
		else if (Error<SetError[5]) {
			KDe = 4;
			KDe_lishudu = Error_lishudu_Max * (SetError[5] - Error) / (SetError[5] - SetError[4]);
		}
		else if (Error < (SetError[5] + SetError[6]) / 2) {
			KDe = 5;
			KDe_lishudu = Error_lishudu_Max - Error_lishudu_Max * (Error - SetError[5]) / (SetError[6] - SetError[5]);
		}
		else if (Error<SetError[6]) {
			KDe = 5;
			KDe_lishudu = Error_lishudu_Max * (SetError[6] - Error) / (SetError[6] - SetError[5]);
		}
	}
	else if (Error<SetError[0]) {
		KDe = 0;
		KDe_lishudu = Error_lishudu_Max;
	}
	else {
		KDe = 5;
		KDe_lishudu = 0;
	}

	/*-----------------------------偏移量变化率求KDec和KDec_lishudu----------------------------------*/
	if (ErrorRate >= SetErrorRate[0] && ErrorRate < SetErrorRate[6]) {
		if (ErrorRate < (SetErrorRate[0] + SetErrorRate[1]) / 2) {
			KDec = 0;
			KDec_lishudu = Error_c_lishudu_Max - Error_c_lishudu_Max * (ErrorRate - SetErrorRate[0]) / (SetErrorRate[1] - SetErrorRate[0]);
		}
		else if (ErrorRate<SetErrorRate[1]){
			KDec = 0;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[1] - ErrorRate) / (SetErrorRate[1] - SetErrorRate[0]);
		}
		else if (ErrorRate < (SetErrorRate[1] + SetErrorRate[2]) / 2) {
			KDec = 1;
		}
		else if (ErrorRate<SetErrorRate[2]) {
			KDec = 1;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[2] - ErrorRate) / (SetErrorRate[2] - SetErrorRate[1]);
		}
		else if (ErrorRate < (SetErrorRate[2] + SetErrorRate[3]) / 2) {
			KDec = 2;
		}
		else if (ErrorRate<SetErrorRate[3]) {
			KDec = 2;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[3] - ErrorRate) / (SetErrorRate[3] - SetErrorRate[2]);
		}
		else if (ErrorRate < (SetErrorRate[3] + SetErrorRate[4]) / 2) {
			KDec = 3;
		}
		else if (ErrorRate<SetErrorRate[4]) {
			KDec = 3;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[4] - ErrorRate) / (SetErrorRate[4] - SetErrorRate[3]);
		}
		else if (ErrorRate < (SetErrorRate[4] + SetErrorRate[5]) / 2) {
			KDec = 4;
		}
		else if (ErrorRate<SetErrorRate[5]) {
			KDec = 4;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[5] - ErrorRate) / (SetErrorRate[5] - SetErrorRate[4]);
		}
		else if (ErrorRate < (SetErrorRate[5] + SetErrorRate[6]) / 2) {
			KDec = 5;
		}
		else if (ErrorRate<SetErrorRate[6]) {
			KDec = 5;
			KDec_lishudu = Error_c_lishudu_Max * (SetErrorRate[6] - ErrorRate) / (SetErrorRate[6] - SetErrorRate[5]);
		}
	}
	else if (ErrorRate<SetErrorRate[0]) {
		KDec = 0;
		KDec_lishudu = Error_c_lishudu_Max;
	}
	else {
		KDec = 5;
		KDec_lishudu = 0;
	}
	/*找规则表中的区域*/
	Temp[0][0] = KDrule[KDec][KDe];
	Temp[0][1] = KDrule[KDec][KDe + 1];
	Temp[0][2] = KDrule[KDec + 1][KDe];
	Temp[0][3] = KDrule[KDec + 1][KDe + 1];
	/*用Error找到的隶属度*/
	Temp[1][0] = KDe_lishudu;
	Temp[1][1] = Error_lishudu_Max - KDe_lishudu;
	Temp[1][2] = KDe_lishudu;
	Temp[1][3] = Error_lishudu_Max - KDe_lishudu;
	/*用Error变化率找到的隶属度*/
	Temp[2][0] = KDec_lishudu;
	Temp[2][1] = Error_c_lishudu_Max - KDec_lishudu;
	Temp[2][2] = KDec_lishudu;
	Temp[2][3] = Error_c_lishudu_Max - KDec_lishudu;
	/*确定四个点的隶属度(取2隶属度小的给1)*/
	for (i = 0; i < 4; i++) {
		if (Temp[1][i]>Temp[2][i]) {
			Temp[1][i] = Temp[2][i];
		}
	}
	/*小中取大，规则表中同等区域隶属度取大*/
	if (Temp[0][0] == Temp[0][1]) {
		if (Temp[1][0] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][0] == Temp[0][2]) {
		if (Temp[1][0] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][0] == Temp[0][3]) {
		if (Temp[1][0] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][0] = 0;
	}
	if (Temp[0][1] == Temp[0][2]) {
		if (Temp[1][1] > Temp[1][2])
			Temp[1][2] = 0;
		else
			Temp[1][1] = 0;
	}
	if (Temp[0][1] == Temp[0][3]) {
		if (Temp[1][1] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][1] = 0;
	}
	if (Temp[0][2] == Temp[0][3]) {
		if (Temp[1][2] > Temp[1][3])
			Temp[1][3] = 0;
		else
			Temp[1][2] = 0;
	}

	for (i = 0; i < 4; i++) {
		Numerator += (KDdan[Temp[0][i]] * Temp[1][i]);
		Denominator += Temp[1][i];
	}
	if (Denominator < 2)
		Denominator = 1;
	return Numerator / Denominator;
}

/***********************************舵机控制************************************/
int32_t Steer_output;
inline void Servo_PD(int16_t *AD ) {
	int max = servo_mid+servo_max;
	int min = servo_mid+servo_min;
	/*
	int temp=0;
	temp=AD[1]/20;
	PAR.Steer_P=P[temp];
    */
    //ErrorRate=Least_Square_Method(20, MarkError,markerror_pointer+30);
    //PAR.Steer_P=FuzzyKP();
    //PAR.Steer_D=FuzzyKD();
    Steer_output =  ( PAR.Steer_P*Error + PAR.Steer_D*( Error - LastError ) );
    Steer_output = servo_mid + Steer_output/10;
    /*
    if(CrossFlag==1) {
    	Steer_output=2*ErrorP;
    } else if(AD[1]<90) {
    	Steer_output=Steer_output;
    } else {
    	Steer_output=Steer_output/5;
    }
    */
    if( Steer_output > max )   
    	Steer_output = max;
    if( Steer_output < min )   
    	Steer_output = min;
    servo_pwm = Steer_output;
}
//********************停车检测*************************//
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



uint8_t flag=0,ju=0,ju1=0;
/*****************************调试中选择LED是否显示**************************************/
void DisplaySwitch(int16_t *AD) {
	if ( ok_port_in == LOW ) {
        	flag++;
        	flag=flag%2;
    }    
    if(flag==0) {
       	if(ju==0) {
       		OLED_Init();
       		delay_ms( 1000 );
       		ju=1;
       		ju1==0;
        }
       	if(markerror_pointer%ARR_MARKERROR_LENGTH==0) {
       		OLED_ShowNum( 0,0,AD[0],4,16 );
    		OLED_ShowNum( 40,0,AD[1],4,16 );
       		OLED_ShowNum( 80,0,AD[2],4,16 );
       		OLED_ShowNum( 40,3,ABS(Error,0),4,16 );
      		OLED_ShowNum( 40,6,ABS(ErrorM,0),4,16 );  	
       	}        		
    } else {
    	if(ju1==0) {
       		OLED_Clear(0,7);
    		delay_ms( 1000 );
       		ju=0;
       		ju1=1;    		
    	}
    }
}
/*****************************数值提前设置***************************************/
void Dubug_Mode(int16_t *Max_Valu, int16_t *Noise_Value, int16_t *NormalizePT )
{
	Max_Valu[0]=210;Max_Valu[1]=210;Max_Valu[2]=210;Max_Valu[3]=190;Max_Valu[4]=190;
	Noise_Value[0]=51;Noise_Value[1]=69;Noise_Value[2]=71;Noise_Value[3]=41;Noise_Value[4]=43;
	NormalizePT[0]=51;NormalizePT[1]=53;
	PAR.Speed_Set = 2600;
	PAR.Steer_P = 320;
	PAR.Steer_D = 340;
}
/*******************************主函数*******************************************/
void main ( void )
{
	int16_t NormalizePT[2]={0};
	int16_t AD[6]={0};
	int16_t Noise_Value[6]={0};         //底噪值
	int16_t Max_Value[6]  ={0};
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
    

	//Dubug_Mode(Max_Value,Noise_Value,NormalizePT);
    
    BEE_CONFIG = port_output;
    BEE_OUTPUT = LOW;

    delay_ms( 1000 );   //显示屏初始化需要一段延时 否则无法显示
    initI2C();         // IIC初始化
    OLED_Init();	   //初始化OLED
    OLED_Clear( 0,7 );
    Desktop();
    
    PAR_init();      
    NoiseValue_init( Noise_Value );	
    Find_Transit_front( NormalizePT, Max_Value, Noise_Value );  
    
    xianshi();
    
    OLED_Clear( 0,7 );
    Car_Distance=0;

    while ( 1 ) {
    	Sampling( );
		Normalization( ADaverage, Noise_Value, Max_Value, AD, 6 );
        Position_analyse_front( NormalizePT, Max_Value, AD ); 
        Servo_PD(AD);
        if( flage_tiaosu ) {	
	    	Speed_PID( smartcar_speed, PAR.Speed_Set );
	    	flage_tiaosu = 0;
		}
		/*
		if(markerror_pointer%ARR_MARKERROR_LENGTH==0) {
			
			OLED_ShowNum( 40,0,ABS(ErrorP,0),4,16 );
			OLED_ShowNum( 40,4,ABS(ErrorVP,0),4,16 );
			
			OLED_ShowNum( 20,0,AD[3],4,16 );	
       		OLED_ShowNum( 60,0,AD[4],4,16 );
       		OLED_ShowNum( 0,4,AD[0],4,16 );
			OLED_ShowNum( 40,4,AD[1],4,16 );
			OLED_ShowNum( 80,4,AD[2],4,16 );
       		
       	}
       	*/
        //DisplaySwitch(AD);
        //sbq( ABS(Error,0), Position, Speed, smartcar_speed );
    	//StopCar();     
    }	
}
//无论如何要打败电院~！

