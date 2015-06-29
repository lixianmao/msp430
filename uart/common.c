
#include <msp430.h>
#include "common.h"

void Delay_ms(unsigned int ms)             // 延时ms  前提是f=8MHZ
{
	unsigned int j;
	for(j=0;j<ms;j++)
		__delay_cycles(8000);
}

void Delay_us(unsigned int us)             // 延时us  前提是f=8MHZ
{
	unsigned int j;
	for(j=0;j<us;j++)
		__delay_cycles(8);
}
