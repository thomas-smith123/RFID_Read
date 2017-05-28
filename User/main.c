#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>
#include "bsp_lcd.h"



void IC_test ( void )
{
	char cStr [ 30 ];
  u8 ucArray_ID [ 4 ];                                                                                             //先后存放IC卡的类型和UID(IC卡序列号)
	u8 ucStatusReturn;                                                                                               //返回状态
	u8 data[16]={1,2,4,6,8,9,0,0,5,6,0,4,0,6,0,0};
  u8 data1[16];
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};

  u8 i=0,j;
  u8 sq=0x00;
  static u8 ucLineCount = 0; 
	
	
  while ( 1 )
  { 
 		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )                                    //寻卡
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );//若失败再次寻卡
		//else
			//printf("PcdRequest_MI_OK\n");		

		if ( ucStatusReturn == MI_OK  )
		{
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   //防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）
			{
			//	printf("PcdAnticoll_MI_OK\n");//防冲撞
				if( PcdSelect (ucArray_ID) == MI_OK)//选卡
				{
					
/*				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );
				
				
				printf ( "%s\r\n",cStr ); 
				

				if ( ucLineCount == 0 )
				  ILI9341_Clear ( 0, 0, 240, 271, macBACKGROUND);	
				
				ILI9341_DispString_EN ( 0, ucLineCount * 16, cStr, macBACKGROUND, macBLUE );
				
				if( PcdAuthState (0x60,0x09,KEY,ucArray_ID ) == MI_OK);			//验证	 PcdAuthState ( 0x60, 0x09, KEY, ucArray_ID )
				else 
					printf("验证失败");
					*/
				for(j=0;j<64;j++)
				{
					ucStatusReturn= PcdAuthState ( 0x60, sq, KEY, ucArray_ID );//密码验证
					if(ucStatusReturn == MI_OK)
						printf("扇区%d: ",sq);//PcdAuthState_MI_OK
					ucStatusReturn= PcdRead(sq,data1);
					for(i=0;i<16;i++)
						printf("%2x",data1[i]);
					printf("\n");
					sq++;
					if(sq==0x64)sq=0x00;
				}

				
				ucLineCount ++;
				
				if ( ucLineCount == 17 ) ucLineCount = 0;
				
			}
			}
			
		}
		
  }
	
	
}




/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	SysTick_Init ();   //滴答时钟初始化
	
	USART1_Config ();  //USART1 配置模式为 115200 8-N-1，中断接收 
	
	RC522_Init ();     //RC522模块所需外设的初始化配置
	
	LCD_Init ();         //LCD 初始化
	
	
	printf ( "WF-RC522 Test\n" );
	
	
	ILI9341_Clear ( 0,   0, 240, 320, macBACKGROUND);	
	ILI9341_Clear ( 0, 272, 240,   2, macGREEN);
	ILI9341_DispString_EN ( 0, 280, "Please put the IC card on WF-RC522 antenna area ...", macBACKGROUND, macRED );
	
	
	PcdReset ();
	M500PcdConfigISOType ( 'A' );//设置工作方式
	
	
  while ( 1 )
  {
    IC_test ();//IC卡检测	

  }
	
	
}


/****************************END OF FILE**********************/

