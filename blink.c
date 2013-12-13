//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

void Init_Clk(void);
void Init_System(void);
void Init_Key(void);

uint8_t Read_Key1(void);
uint8_t Read_Key2(void);

void main(void)
{
	uint16_t i ;
      Init_System();                                  // 系统初始化
     // _EINT();                                        // 开总中断
      //__bis_SR_register(LPM3_bits);                   //进入LPM3
      P1OUT |= BIT0;
      P8OUT |= BIT1;
      Init_Key();
      while(1)
      {
    	 if( Read_Key1() )
    	 {
    		 P1OUT ^= BIT0;
    	 }
    	 if(Read_Key2())
    	 {
    		 P8OUT ^= BIT1;
    	 }

      }

}


/*******************************************************************************
******************** 看门狗中断服务程序*******************
********************************************************************************/

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
       P1OUT ^= BIT0;                                  // LED1翻转
}

/*************************************************
  函数(模块)名称:void  Init_System(void)
  功能:	系统初始化
  本函数(模块)调用的函数(模块)清单:
  调用本函数(模块)的函数(模块)清单:
  输入参数:     void
  输出参数:     void
  函数返回值说明:
  使用的资源
  其它说明:
*************************************************/
void  Init_System(void)
{
      WDTCTL = WDTPW + WDTHOLD;                      // 关看门狗
      //Init_Clk();                                    //初始化时钟
      WDTCTL = WDT_ADLY_1000;                         // WDT 250ms, ACLK,
      SFRIE1 |= WDTIE;                               // WDT 中断使能
      P1DIR |= BIT0;                                 // 设置P1。0为输出
      P8DIR |= BIT1;                                 // 设置P2。7为输出,LED2

}


/*************************************************
  函数(模块)名称:void Init_Clk(void)
  功能:	时钟初始化
  本函数(模块)调用的函数(模块)清单:
  调用本函数(模块)的函数(模块)清单:
  输入参数:     void
  输出参数:     void
  函数返回值说明:
  使用的资源
  其它说明:
*************************************************/
void Init_Clk(void)
{
      P7SEL |= 0x03;                                   // XT1起振
      P1DIR |= BIT0;                                   // P1。0输出
      UCSCTL1 = DCORSEL_2;                             // 选择 DCO 频率范围
      UCSCTL4 =  SELA_0 ;                              // 设置t MCLK = DCOC,MCLK =DCO,ACLK=XT1
      while (SFRIFG1 & OFIFG)                          //清除OFIFG,and  XT1OFFG ,DCOFFG
      {
           UCSCTL7 &= ~(  XT1LFOFFG + DCOFFG);
           SFRIFG1 &= ~OFIFG;
      }

}

/**
 *
 */
void Init_Key(void)
{
//	输入上拉
	P1DIR|= (BIT7);
	P1OUT |= (BIT7);

	//P1REN |= BIT7;
	//P1SEL &= ~(BIT7);
	P1DS  |= BIT7;
	P2DIR|= (BIT2);
	P2OUT |= (BIT2);
}



/**
 *
 */
uint8_t Read_Key1(void)
{
	static  uint16_t buf = 0x0000;
	static uint16_t newStatus = 0x0000;
	static  uint16_t lastStatus = 0x0000;


	buf <<= 1;
	buf |= ( (P1IN & BIT7) ? 0x01 :0x00);

	if((0x0000 == buf) || (0xFFFF == buf))
	{
		lastStatus = newStatus;
		newStatus = buf;
	}

	if((0x0000 == lastStatus) && (0xFFFF == newStatus))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}

}


uint8_t Read_Key2(void)
{
	static  uint16_t buf = 0x0000;
	static uint16_t newStatus = 0x0000;
	static  uint16_t lastStatus = 0x0000;


	buf <<= 1;
	buf |= ( (P2IN & BIT2) ? 0x01 :0x00);

	if((0x0000 == buf) || (0xFFFF == buf))
	{
		lastStatus = newStatus;
		newStatus = buf;
	}

	if((0x0000 == lastStatus) && (0xFFFF == newStatus))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}

}


