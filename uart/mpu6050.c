/*
 * mpu6050.c
 *
 *  Created on: 2015-6-25
 *      Author: Administrator
 */
#include"mpu6050.h"
uchar err;
//end
void Delay5us()
{
	_delay_cycles(5);
}

void Delay5ms()
{
	_delay_cycles(5000);
}

void start()
{
	SCL_OUT;
	SDA_OUT;
	SDA_H;
	SCL_H;
	Delay5us();
	SDA_L;
	Delay5us();
	SCL_L;
}

void stop()
{
	SCL_OUT;
	SDA_OUT;
	SDA_L;
	SCL_H;
	Delay5us();
	SDA_H;
	Delay5us();
	SCL_L;
}

void mack()
{
	SDA_OUT;
	SCL_OUT;
	SDA_L;
	SCL_H;
	Delay5us();
	SCL_L;
	SDA_H;
}

void mnack()
{
	SDA_OUT;
	SCL_OUT;
	SDA_H;
	Delay5us();
	SCL_H;
	Delay5us();
	SCL_L;
	SDA_L;
}

void cack()
{
	SDA_IN;
	SCL_OUT;
	SCL_L;
	_delay_cycles(2);
	SCL_H;
	err = 0;
	if(SDA_DATA)
		err = 1;
	SCL_L;
	SDA_OUT;
}

void write1byte(uchar byte1)
{
	unsigned char i = 8;
	SDA_OUT;
	SCL_OUT;
	while(i--)
	{
		Delay5us();
		Delay5us();
		if(byte1 & 0x80)
		{SDA_H;}
		else
		{SDA_L;}
		Delay5us();
		Delay5us();
		SCL_H;
		Delay5us();
		Delay5us();
		SCL_L;
		byte1 <<= 1;
	}
}

uchar read1byte()
{
	unsigned char i;
	unsigned char ddata = 0;
	signed char temp;
	SCL_OUT;
	SDA_IN;
	for(i=0;i<8;i++)
	{
		ddata <<= 1;
		Delay5us();
		Delay5us();
		SCL_H;
		temp = SDA_DATA;
		if(temp)
			ddata ++;
		Delay5us();
		Delay5us();
		SCL_L;
	}
	return ddata;
}

void Single_Write_MPU6050(uchar REG_Address, uchar REG_data)
{
	start();
	write1byte(SlaveAddress);
	cack();
	write1byte(REG_Address);
	cack();
	write1byte(REG_data);
	cack();
	stop();
}

uchar Single_Read_MPU6050(uchar REG_Address)
{
	uchar REG_data;
	start();
	write1byte(SlaveAddress);
	cack();
	write1byte(REG_Address);
	cack();
	start();
	write1byte(SlaveAddress+1);
	cack();
	REG_data=read1byte();
	mnack();
	stop();
	return REG_data;
}


void MPU6050_Init()
{
	Single_Write_MPU6050(PWR_MGMT_1,0x00);
	Single_Write_MPU6050(SMPLRT_DIV, 0x07);		//设定输出速率为25HZ
	Single_Write_MPU6050(CONFIG, 0x06);		    //禁止自动休眠，工作在测量模式
	Single_Write_MPU6050(GYRO_CONFIG, 0x18);		//使能DATA_READY中断
	Single_Write_MPU6050(ACCEL_CONFIG, 0x01);

}
int Get16Bit (uchar address)
{
	uchar ho,lo;
	ho = Single_Read_MPU6050(address);
	lo = Single_Read_MPU6050(address+1);
	return (ho<<8)+lo;
}
int GetAccelX ()
{
	return Get16Bit(ACCEL_XOUT_H);
}
int GetAccelY ()
{
	return Get16Bit(ACCEL_YOUT_H);
}
int GetAccelZ ()
{
	return Get16Bit(ACCEL_ZOUT_H);
}

// X/Y/Z-Axis Angular velocity
int GetAnguX ()
{
	return Get16Bit(GYRO_XOUT_H);
}
int GetAnguY ()
{
	return Get16Bit(GYRO_YOUT_H);
}
int GetAnguZ ()
{
	return Get16Bit(GYRO_ZOUT_H);
}



