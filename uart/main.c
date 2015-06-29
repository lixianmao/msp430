#include <msp430g2553.h>
#include "mpu6050.h"
#include "uart.h"

int data[3] = {0, 0, 0};
float speed[3]={0, 0, 0};
int count = 0;
float gxmax = 0;

int thresholdAx = 12000;
int isAxBig = 0;
int thresholdAz = -5000;
int isAzBig = 0;

double fabs(double f);

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop Watchdog Timer
	MPU6050_Init();
	uart_init();
	while(1)
	{
		data[0] = GetAccelX();
		data[1] = GetAccelZ();
		data[2] = GetAnguX();

		//speed[0] = (float)(data[0] * 1.0 / 32768 * 2);
		//speed[1] = (float)(data[1] * 1.0 / 32768 * 2);
		speed[2] = (float)(data[2] * 1.0 / 32768 * 250 + 0.465);
		//__no_operation();

		if(count < 50){
			if(fabs(speed[2]) > fabs(gxmax))
				gxmax = speed[2];
			count ++;
		}
		else
		{
			if(gxmax > 8)
			{
				UCA0TXBUF = 0x01;
				isAxBig = 0;
				isAzBig = 0;
			}
			else if (gxmax < -8)
			{
				UCA0TXBUF =0x02;
				isAxBig = 0;
				isAzBig = 0;
			}
			else if((data[1] > thresholdAz) && !isAzBig)
			{
				UCA0TXBUF =0x04;
				isAzBig = 1;
				isAxBig = 0;
			}
			else if((data[0] > thresholdAx)&&!isAxBig)
			{
				UCA0TXBUF =0x03;
				isAxBig = 1;
				isAzBig = 0;
			}

			else
			{
				UCA0TXBUF =0x00;
				isAxBig = 0;
				isAzBig = 0;
			}
			__no_operation();
			count = 0;
			gxmax = 0;

		}
	}
}


double fabs(double f)
{
	if(f > 0)
	{
		return f;
	}
	else
	{
		return 0 - f;
	}
}
/**
 * TXD interrupt
 */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	IE2 &= ~UCA0TXIE;	//关闭写中断，使主程序能进入循环
}

/**
 * RXD interrupt
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	//无论什么命令都回传
	while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character

}
