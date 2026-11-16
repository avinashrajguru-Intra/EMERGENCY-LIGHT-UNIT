#include <stdio.h>
#include "I2C.h"


//8 bit
unsigned char HOUR = 0,MINUTE = 0,SECOUND = 0;
unsigned char DATE = 0,MONTH = 0,YEAR = 0;


//DELAY COUNT 01-65535
void DELAYI2C(unsigned short lu16_cCnt)
{
	while(0U != lu16_cCnt)
	{
		lu16_cCnt = lu16_cCnt - 1U;
	}
}

void SDA_OUTPUT(void)
{
    TRISBbits.TRISB1 = 0;// SDA AS OUTPUT
	I2C_NOP();I2C_NOP();
}

void SDA_INPUT(void)
{
	TRISBbits.TRISB1 = 1;// SDA AS INPUT
	I2C_NOP();I2C_NOP();
}

//IIC START COMMAND
void _START(void)
{
	I2C_NOP();
	SDA_OUTPUT();
	I2C_NOP();
	SDA_OUT = 1;
	I2C_NOP();
    SCL = 1;
   	I2C_NOP();
   	SDA_OUT = 0;
   	I2C_NOP();
}

//IIC STOP COMMAND
void _STOP(void)
{
	I2C_NOP();
	SDA_OUTPUT();
	I2C_NOP();
	SDA_OUT = 0;
    I2C_NOP();
	SCL = 1;
    I2C_NOP();
	SDA_OUT = 1;
	I2C_NOP();
}

void READ_RTC_TIME(void)
{
	unsigned char DATA = 0, TEMP1 = 0,TEMP2 = 0;
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                        
	RTC_WRITE(0);
    Nop();
	_START();
	RTC_WRITE(0xD1);//READ
 	DATA = RTC_READ(1);
	_STOP(); 
	DELAYI2C(3000);
	TEMP1	= DATA & 0x70;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	SECOUND = (TEMP1*10) + TEMP2;
	
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                        
	RTC_WRITE(1);
	Nop();
	_START();
	RTC_WRITE(0xD1);//READ
 	DATA = RTC_READ(1);
	_STOP();
	DELAYI2C(3000);
	TEMP1	= DATA & 0x70;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	MINUTE = (TEMP1*10) + TEMP2;
	
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                         
	RTC_WRITE(2);
	Nop();
	_START();
	RTC_WRITE(0xD1);//READ
	DATA = RTC_READ(0);
	_STOP();
	DELAYI2C(3000);
	TEMP1	= DATA & 0x30;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	HOUR = (TEMP1*10) + TEMP2;
}

void READ_RTC_DATE(void)
{
	unsigned char DATA = 0, TEMP1 = 0,TEMP2 = 0;
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                         
	RTC_WRITE(4);
	Nop();
	_START();
	RTC_WRITE(0xD1);//READ
	DATA = RTC_READ(1);
	_STOP();
	DELAYI2C(3000);
	TEMP1	= DATA & 0x30;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	DATE = (TEMP1*10) + TEMP2;
	
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                         
	RTC_WRITE(5);
	Nop();
	_START();
	RTC_WRITE(0xD1);//READ
	DATA = RTC_READ(1);
	_STOP();
	DELAYI2C(3000);
	TEMP1	= DATA & 0x10;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	MONTH = (TEMP1*10) + TEMP2;
	
	_START();
	RTC_WRITE(0xD0);//WRITE                                                                                                                                         
	RTC_WRITE(6);
	Nop();
	_START();
	RTC_WRITE(0xD1);//READ
	DATA = RTC_READ(0);
	_STOP();
	DELAYI2C(3000);
	TEMP1	= DATA & 0xF0;
	TEMP1	= TEMP1 >> 4;
	TEMP2	= DATA & 0x0F;
	YEAR = (TEMP1*10) + TEMP2;
}

void RTC_WRITE(unsigned char NUM)
{
	unsigned char FOR_COUNT;
	
   	//OUTPUT
	SDA_OUTPUT();
	
	SCL = 0;
   	for(FOR_COUNT = 1; FOR_COUNT <= 8; FOR_COUNT++)
	{
		if(NUM & 0x80)
		{	
		    I2C_NOP();
			SDA_OUT = 1;
		} 
		else
		{
		   	I2C_NOP();			
			SDA_OUT = 0;
		}
		SCL = 0;
		I2C_NOP();
		SCL = 1;
		I2C_NOP();I2C_NOP();
		SCL = 0;
		I2C_NOP();
		NUM = (unsigned char)(NUM << 1);
	}
	SDA_OUT = 1;
	I2C_NOP();
	SCL = 0;
	I2C_NOP();
	SCL = 1;
	I2C_NOP();I2C_NOP();
	SCL = 0; 
	I2C_NOP();
}

unsigned char RTC_READ(unsigned char END)
{
	unsigned char FOR_COUNT,NUM;
	
	//INPUT
	SDA_INPUT();
   	
	for(FOR_COUNT = 1; FOR_COUNT <= 8; FOR_COUNT++)
   	{
		SCL = 0;
		I2C_NOP();I2C_NOP();
		SCL = 1;
		I2C_NOP();I2C_NOP();
		NUM = (unsigned char) (NUM << 1);
		NUM = NUM |(unsigned char)SDA_IN;
    	}
   	
	//OUTPUT
	SDA_OUTPUT();
	
	I2C_NOP();
	SDA_OUT = 0;
	I2C_NOP();
	if(END)
	{
		SCL = 1;
	}
	else
	{
		SCL = 0;
		I2C_NOP();
		SCL = 1;
		I2C_NOP();
		SDA_OUT = 1;
	}	
	I2C_NOP();
	SCL = 0;
	I2C_NOP();
	DELAYI2C(1000);
	return NUM;							   
}

void SET_RTC(unsigned char DETA,unsigned char ADD)
{
	_START();
	RTC_WRITE(0xD0);
	RTC_WRITE(ADD);
	RTC_WRITE(DETA);
	_STOP();
}

void SET_TIME(unsigned char hr,unsigned char min,unsigned char sec)
{
	unsigned char FINAL = 0,DATA = 0,TEMP1= 0,TEMP2 = 0;
	
	DATA = hr;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,2);//HOUR	
	
	DATA = min;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,1);//MINUTE
	
	DATA = sec;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,0);//SECOUND
}

void SET_DATE(unsigned char DD,unsigned char MM,unsigned char YY)
{
	unsigned char FINAL = 0,DATA = 0,TEMP1= 0,TEMP2 = 0;
	
	DATA = DD;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,4);//DATE	
	
	DATA = MM;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,5);//MONTH
	
	DATA = YY;
	TEMP1 = DATA / 10;// HIGHER BITS
	TEMP2 = DATA % 10;// LOWER BITS
	FINAL = (unsigned char)((TEMP1 << 4) + TEMP2);
	SET_RTC(FINAL,6);//YEAR
}


//DELAY NOP
void I2C_NOP(void)
{
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
}


/**
 End of File
*/
