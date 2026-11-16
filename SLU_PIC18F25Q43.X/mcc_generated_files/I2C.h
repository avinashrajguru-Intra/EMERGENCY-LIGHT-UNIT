#ifndef _I2C_H
#define _I2C_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//IIC INTERFACE
#define SDA_IN		PORTBbits.RB1
#define SDA_OUT		LATBbits.LATB1
#define SCL         LATBbits.LATB0

// SDA  RC4
// SCL  RC3

//IIC MEMORY READ/WRITE
void I2C_NOP(void);
void _START(void);
void _STOP(void);
void MEM_WRITE(unsigned char NUM);
unsigned char MEM_READ(void);

void SDA_OUTPUT(void);
void SDA_INPUT(void);
void DELAYI2C(unsigned short lu16_cCnt);

//RTC
void SET_TIME(unsigned char hr,unsigned char min,unsigned char sec);
void SET_DATE(unsigned char DD,unsigned char MM,unsigned char YY);
void RTC_WRITE(unsigned char NUM);
unsigned char RTC_READ(unsigned char END);
void READ_RTC_TIME(void);
void READ_RTC_DATE(void);
void RTC_INT(void);







#endif //_I2C_H   
/**
 End of File
*/
