
//******************************************************************************
//  MSP430G2xx3 Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  E. Chen
//  Texas Instruments, Inc
//  May 2018
//  Built with CCS Version 8.0 and IAR Embedded Workbench Version: 7.11
//****************************************************************
#include <msp430.h>
#define MAX_INPUT 4
#define SHORT_PRESS 0
#define LONG_PRESS 1
#define THRESHOLD 50000  // 50,000 �gs

unsigned int input[MAX_INPUT];
unsigned int index = 0;
unsigned int password[MAX_INPUT] = {LONG_PRESS, SHORT_PRESS, SHORT_PRESS, SHORT_PRESS};
unsigned int temp;
unsigned int press_time = 0;
unsigned int release_time = 0;

void reset_input(void);
void success(void);
void error(void);
int match(void);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    TA0CTL = TASSEL_2 + MC_2 + TACLR;	// Timer_A �ϥ�SMCLK �s��Ҧ� 
    
    P1DIR &= ~BIT3;	  		
    P1REN |= BIT3;	  // P1.3 ���s�]��INPUT,�W�Թq�� 
    P1OUT |= BIT3; 
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P1DIR |= BIT6;
    P1OUT &= ~BIT6;
    
    P1IE |= BIT3;
    P1IES |= BIT3;      //P1.3�iĲ�o���_�B���󬰰���C 
    P1IFG &= ~BIT3;
    reset_input();
    __enable_interrupt();  
    while (1)
    {
        __low_power_mode_0();  
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT3)
    {
         __delay_cycles(20000); 
        if ((P1IN & BIT3) == 0 && (P1IES & BIT3))    //�T�{�q�쬰�C�B�O���UĲ�o���_ 
        {
            press_time = TA0R;
            P1IES &= ~BIT3;   // �אּ��}Ĳ�o���_
        }
        else if((P1IN & BIT3) != 0 && !(P1IES & BIT3))
        {
            release_time = TA0R;
            unsigned int duration;

            if (release_time >= press_time)
                duration = release_time - press_time;
            else
                duration = (0xFFFF - press_time) + release_time + 1;    //�p�G�����ɶ�overflow 
            temp = duration;
    
            if (duration < THRESHOLD)
                input[index] = SHORT_PRESS;
            else
                input[index] = LONG_PRESS;

            index++;
            if (index == MAX_INPUT)   //�T�{�K�X�O�_�w�g��J���T�����
            {
                if (match())
                    success();
                else
                    error();

                reset_input();
            }
            P1IES |= BIT3;    //��^���UĲ�o���_ 
        }

        P1IFG &= ~BIT3;  //���_���� 
    }
}

void reset_input(void)
{
    unsigned int i;
    for (i = 0; i < MAX_INPUT; i++)
        input[i] = 0;
    index = 0;
}

void success(void)
{
    unsigned int i;
    for (i = 0; i < 3; i++)
    {
        P1OUT ^= BIT0;
        __delay_cycles(300000); 
    }
    P1OUT &= ~BIT0;
}

void error(void)
{
    P1OUT |= BIT6;               
    __delay_cycles(1000000);    
    P1OUT &= ~BIT6;
}

int match(void)
{
    unsigned int i;
    for (i = 0; i < MAX_INPUT; i++)
    {
        if (input[i] != password[i])
            return 0;
    }
    return 1;
}
