
#include <msp430.h>

void uart_init()
{
	BCSCTL1 = CALBC1_8MHZ;           //DCOÊý×ÖÕñµ´Æ÷=8MHZ
	DCOCTL  = CALBC1_8MHZ;           //DCOÊý×ÖÕñµ´Æ÷=8MHZ

	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 |= UCSSEL_2;

	// set baud rate
	UCA0BR0 = 0x41;
	UCA0BR1 = 0x03;
	UCA0MCTL = 0x04;

	//P1.1 RXD, P1.2 TXD
	P1SEL |= BIT1 + BIT2;
	P1SEL2 |= BIT1 + BIT2;

	UCA0CTL1 &= ~UCSWRST;

	IE2 |= UCA0RXIE + UCA0TXIE; //Enable interrupt of RXD, TXD
}

