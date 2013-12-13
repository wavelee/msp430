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

volatile uint8_t newStatus = 0x00;
volatile uint8_t lastStatus = 0x00;
volatile uint8_t buf = 0x00;



void Init_Clk(void);
void Init_System(void);
void Init_Key(void);

uint8_t Read_Key1(void);

void main(void)
{
	uint16_t i ;
      Init_System();                                  // ϵͳ��ʼ��
     // _EINT();                                        // �����ж�
      //__bis_SR_register(LPM3_bits);                   //����LPM3
      P1OUT |= BIT0;
      Init_Key();
      while(1)
      {
    	 if( Read_Key1() )
    	 {
    		 P1OUT ^= BIT0;
    	 }

    	 for(i = 0;i<500;i++)
    	 {

    	 }
      }



}


/*******************************************************************************
******************** ���Ź��жϷ������*******************
********************************************************************************/

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
       P1OUT ^= BIT0;                                  // LED1��ת
}

/*************************************************
  ����(ģ��)����:void  Init_System(void)
  ����:	ϵͳ��ʼ��
  ������(ģ��)���õĺ���(ģ��)�嵥:
  ���ñ�����(ģ��)�ĺ���(ģ��)�嵥:
  �������:     void
  �������:     void
  ��������ֵ˵��:
  ʹ�õ���Դ
  ����˵��:
*************************************************/
void  Init_System(void)
{
      WDTCTL = WDTPW + WDTHOLD;                      // �ؿ��Ź�
      //Init_Clk();                                    //��ʼ��ʱ��
      WDTCTL = WDT_ADLY_1000;                         // WDT 250ms, ACLK,
      SFRIE1 |= WDTIE;                               // WDT �ж�ʹ��
      P1DIR |= BIT0;                                 // ����P1��0Ϊ���
}


/*************************************************
  ����(ģ��)����:void Init_Clk(void)
  ����:	ʱ�ӳ�ʼ��
  ������(ģ��)���õĺ���(ģ��)�嵥:
  ���ñ�����(ģ��)�ĺ���(ģ��)�嵥:
  �������:     void
  �������:     void
  ��������ֵ˵��:
  ʹ�õ���Դ
  ����˵��:
*************************************************/
void Init_Clk(void)
{
      P7SEL |= 0x03;                                   // XT1����
      P1DIR |= BIT0;                                   // P1��0���
      UCSCTL1 = DCORSEL_2;                             // ѡ�� DCO Ƶ�ʷ�Χ
      UCSCTL4 =  SELA_0 ;                              // ����t MCLK = DCOC,MCLK =DCO,ACLK=XT1
      while (SFRIFG1 & OFIFG)                          //���OFIFG,and  XT1OFFG ,DCOFFG
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
//	��������
	P1DIR|= (BIT7);
	P1OUT |= (BIT7);
	//P1REN |= BIT7;
	//P1SEL &= ~(BIT7);
	//P1DS  |= BIT7;
}



/**
 *
 */
uint8_t Read_Key1(void)
{

	buf <<= 1;
	buf |= ( (P1IN & BIT7) ? 0x01 :0x00);

	if((0x00 == buf) || (0xFF == buf))
	{
		lastStatus = newStatus;
		newStatus = buf;
	}

	if((0xFF == lastStatus) && (0x00 == newStatus))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}

}


