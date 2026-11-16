/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F25Q43
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/I2C.h"

/*
                         Main application
 */


#define MCU_FAIL          LATAbits.LATA7
#define BAT_DISCHARGE     LATCbits.LATC5
#define CHARGING_CONTROL  LATCbits.LATC4
#define CHNAGEOVER_DETECT PORTCbits.RC0
#define CHARGING_FAILURE  PORTAbits.RA2
#define WDI_FEED          LATCbits.LATC1
#define LED_GREEN         LATAbits.LATA5
#define LED_RED           LATAbits.LATA4
#define LED_UMBER         LATAbits.LATA3
#define BAT_SWITCH        LATAbits.LATA6



#define TEST_ON_TIME   420// 60 * 7 hours
#define TEST_OFF_TIME  720// 60 * 12 hours

#define LED_ON   0
#define LED_OFF  1  

#define ON   1
#define OFF  0 




volatile unsigned int LOAD_DELAY = 0;

unsigned int BAT_VOLTAGE = 0;

unsigned int DISPLAY_COUNTER = 0;

volatile unsigned int ADC_DATA = 0;

volatile unsigned char FLASH_COUNTER1 = 0;
volatile unsigned char FLASH_COUNTER2 = 0;
volatile unsigned char FLASH_COUNTER3 = 0;

volatile unsigned int DEBOUNCE_COUNTER0 = 0;
volatile unsigned int DEBOUNCE_COUNTER1 = 0;


unsigned char CHANGE_COUNTER = 0;
unsigned char SEC_COUNTER = 0;
unsigned char MIN_COUNTER = 0;
unsigned char BAT_TEST_COUNTER = 0;
unsigned char CHARGING_COUNTER = 0;
unsigned char ON_COUNTER = 0;
unsigned char OFF_COUNTER = 0;
unsigned int TOTAL_TIME = 0;
unsigned int BAT_CHECK_COUNTER = 0;


unsigned int PWM_COUNTER = 0;


_Bool CHECK_SERIAL_FLAG = 0;
_Bool READ_FLAG = 0;
_Bool SEND_DATA_FLAG = 0;
_Bool BAT_NOT_HEALTHY = 0;
_Bool BAT_TEST_FLAG = 0;
_Bool ARRAY1_ON_FLAG = 0;
_Bool ARRAY2_ON_FLAG = 0;

_Bool CHECK_BAT_FLAG = 0;
_Bool BAT_TEST_DONE_FLAG = 0;
_Bool CHARGING_ON_FLAG = 0;




volatile unsigned char RX_DATA = 0;
volatile unsigned char RX_COUNTER = 0;
volatile unsigned char RX_BUFF[20];
extern unsigned char HOUR,MINUTE,SECOUND;
extern unsigned char DATE,MONTH,YEAR;



void DELAY_MSEC(unsigned int DELAY_COUNT);
void DELAY(unsigned int COUNT);

void SERIAL_PRINT(char *HEX,char TYPE);
void UART_SEND(unsigned char DATA);
void SEND_RTC(void);
void UART_ISR(void);
void SET_NEW_RTC(void);
void INT_GPIO(void);
void START_ADC(void);
void INT_ADC(void);
void DISPLAY_INT_FOUR(unsigned int HEX);
void DISPLAY_BAT(unsigned int HEX);
void LED_INDICATION(void);
void DISPLAY_CHANGE_TIME(unsigned int HEX);
void BAT_TEST_LOGIC(void);
void BAT_CHARGING(void);

void PWM_OFF(void);
void CAL_PWM(void);

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    TMR0_StartTimer();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    MCU_FAIL = 1;
    BAT_SWITCH = 1;
    DELAY(20000);
    
     PWM1_16BIT_SetSlice1Output1DutyCycleRegister(320); 
     PWM1CONbits.LD = 1;
     //PWM_COUNTER = 32000;
     
    while (1)
    {

      DELAY_MSEC(100);
      

      
      //0x0140;//320
      //0x7BC0;//31680
//      
//        PWM_COUNTER = PWM_COUNTER + 100;
//        if(PWM_COUNTER > 31600)
//        {
//           PWM_COUNTER = 0;
//        }
      

      //y = 14.457x - 156.65
      BAT_VOLTAGE = (unsigned short)((ADC_DATA + 156.65)/14.457);
        
      if(BAT_VOLTAGE < 90)
      {
       BAT_CHECK_COUNTER = BAT_CHECK_COUNTER + 1;
       if(BAT_CHECK_COUNTER > 10)//10*100 = 1Sec
       {
         BAT_SWITCH = 0; 
         DELAY_MSEC(1000);
         BAT_CHECK_COUNTER = 0;    
       }
      }
      else
      {
          BAT_CHECK_COUNTER = 0;
      }
      
      DISPLAY_COUNTER = DISPLAY_COUNTER + 1;
      if(DISPLAY_COUNTER > 100)//100*100 = 10Sec
      {
        DISPLAY_COUNTER = 0;
        SEND_DATA_FLAG = 1;
      }  
        
      if(SEND_DATA_FLAG)
      {
        SEND_DATA_FLAG = 0;
        DISPLAY_COUNTER = 0;
        READ_RTC_TIME();
        READ_RTC_DATE();
        SERIAL_PRINT("///////////////////////////////////////////////////////////////////////////",1);
        SERIAL_PRINT(" Firmware Version 2.00",1);
        SERIAL_PRINT(" RTC:  ",0);
        SEND_RTC();
        SERIAL_PRINT(" BAT : ",0);
        DISPLAY_BAT(BAT_VOLTAGE);
        
        if(ARRAY1_ON_FLAG)
        {
           SERIAL_PRINT(" Lux : 33 ",1);
        }
        else if(ARRAY2_ON_FLAG)
        {
           SERIAL_PRINT(" Lux : 22 ",1);
        }
        else
        {
           SERIAL_PRINT(" CLUSTER : OFF ",1); 
        }
        
        if(ARRAY1_ON_FLAG && !ARRAY2_ON_FLAG)
        {
           SERIAL_PRINT(" CHANGE_OVER_TIME : ",0);
           DISPLAY_CHANGE_TIME(MIN_COUNTER);
           SERIAL_PRINT(" MIN",1);
        }
        else if(!ARRAY1_ON_FLAG && ARRAY2_ON_FLAG)
        {
           SERIAL_PRINT(" CHANGE_OVER_FINISH ",1);  
        }
        
        SERIAL_PRINT(" BATTERY TEST :",0);
        if(BAT_TEST_FLAG)
        {
           SERIAL_PRINT(" ON ",1); 
        }
        else if(BAT_TEST_DONE_FLAG)
        {
           SERIAL_PRINT(" Test Done ",1); 
        }
        else
        {
            SERIAL_PRINT(" OFF ",1); 
        }
        SERIAL_PRINT("///////////////////////////////////////////////////////////////////////////",1);
        TOTAL_TIME = (HOUR*60) + MINUTE;
      }  
      
      
      //READ SERIAL
      if(CHECK_SERIAL_FLAG)
      {
          CHECK_SERIAL_FLAG = 0;
          SET_NEW_RTC();
      }
      
      
      if(CHNAGEOVER_DETECT && !ARRAY1_ON_FLAG && !ARRAY2_ON_FLAG && (BAT_VOLTAGE >= 116))//PIN 0V Means 110 Present
      {
          ON_COUNTER = ON_COUNTER + 1;
          if(ON_COUNTER > 10)
          {
            ARRAY1_ON_FLAG = 1;
            ON_COUNTER = 0;
          }
      }
      else if(ARRAY1_ON_FLAG)
      {
          CHANGE_COUNTER = CHANGE_COUNTER + 1;
          if(CHANGE_COUNTER >= 10)//1SEC
          {
              CHANGE_COUNTER = 0;
              SEC_COUNTER = SEC_COUNTER + 1;
          }
          else if(SEC_COUNTER > 60)//1 Minute
          {
              SEC_COUNTER = 0;
              MIN_COUNTER = MIN_COUNTER + 1;
          }
          //else if(MIN_COUNTER >= 120)//120 Minute
          else if(MIN_COUNTER >= 120)//2 Minute
          {
              CHANGE_COUNTER = 0;
              SEC_COUNTER = 0;
              MIN_COUNTER = 0;
              ARRAY1_ON_FLAG = 0;
              ARRAY2_ON_FLAG = 1;
          }
       }
      else 
      {
          ON_COUNTER = 0;
      }
      if((!CHNAGEOVER_DETECT && (ARRAY1_ON_FLAG || ARRAY2_ON_FLAG)) || (BAT_VOLTAGE < 105))
      {
        OFF_COUNTER = OFF_COUNTER + 1;
        if(OFF_COUNTER > 10)
        {
            PWM_OFF();
            CHANGE_COUNTER = 0;
            SEC_COUNTER = 0;
            MIN_COUNTER = 0;
            ARRAY1_ON_FLAG = 0;
            ARRAY2_ON_FLAG = 0;
            OFF_COUNTER = 0;
        }
      }
      else
      {
          OFF_COUNTER = 0;
      }
      
      
      
      
      //battery test routine
      BAT_TEST_LOGIC();
      
      //BAT CHARGING CONTROL
      BAT_CHARGING();
      
     //LUX CALCULATION
      CAL_PWM();
   
    }//WHILE
}//VOID MAIN






void DELAY_MSEC(unsigned int DELAY_COUNT)
{
	LOAD_DELAY = DELAY_COUNT;
	while(LOAD_DELAY);
}
//DELAY COUNT 01-65535
void DELAY(unsigned int COUNT)
{
	while(COUNT--);
}





void SERIAL_PRINT(char *HEX,char TYPE)
{
	while(*HEX != '\0')
		{
			UART_SEND(*HEX);
            HEX = HEX + 1;
		}
    if(TYPE == 1)
    {
      UART_SEND(0x0D);  
      UART_SEND(0x0A);  
    }
}

void UART_SEND(unsigned char DATA)
{
    UART1_Write(DATA);
    DELAY(100);
}

void SEND_RTC(void)
{
	unsigned char POS = 0;
    //HOUR
  	POS = HOUR / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = HOUR % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	UART_SEND(':');
    
    //MINUTE
	POS = MINUTE / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = MINUTE % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
    UART_SEND(':');
    
    //SECOUND
    POS = SECOUND / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = SECOUND % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
    UART_SEND(' ');
    UART_SEND(' ');
    
    //DATE
  	POS = DATE / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = DATE % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	UART_SEND('/');

    //MONTH
	POS = MONTH / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = MONTH % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	UART_SEND('/');
    
    //YEAR
    UART_SEND('2');
    UART_SEND('0');
	POS = YEAR / 10;
	POS = POS + 0x30;
	UART_SEND(POS);
	POS = YEAR % 10;
	POS = POS + 0x30;
	UART_SEND(POS);
    
    UART_SEND(0x0D);  
    UART_SEND(0x0A);     
}

void SET_NEW_RTC(void)
{
 unsigned char T1,T2,T3,T4,T5,T6;
 unsigned char TEMP1= 0,TEMP2 = 0;
 unsigned char COUNTER = 0;
 // *HHMMSS-DDMMYYYY#  
    TEMP1 = RX_BUFF[0] - 0x30;
    TEMP2 = RX_BUFF[1] - 0x30;
    T1 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    TEMP1 = RX_BUFF[2] - 0x30;
    TEMP2 = RX_BUFF[3] - 0x30;
    T2 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    TEMP1 = RX_BUFF[4] - 0x30;
    TEMP2 = RX_BUFF[5] - 0x30;
    T3 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    TEMP1 = RX_BUFF[7] - 0x30;
    TEMP2 = RX_BUFF[8] - 0x30;
    T4 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    TEMP1 = RX_BUFF[9] - 0x30;
    TEMP2 = RX_BUFF[10] - 0x30;
    T5 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    TEMP1 = RX_BUFF[13] - 0x30;
    TEMP2 = RX_BUFF[14] - 0x30;
    T6 = (unsigned char)((TEMP1 * 10) + TEMP2);
    
    if((T1 > 24) || (T2 > 60) || (T3 > 60))
    {
      SERIAL_PRINT("///////////////////////////////////////////////////////////////////////////",1); 
      SERIAL_PRINT("Invalid Time",1);
      for(COUNTER = 0;COUNTER < 15;COUNTER++)
      {
           UART_SEND(RX_BUFF[COUNTER]); 
      }   
    }
    else if((T4 > 31) || (T5 > 12) || (T6 > 99))
    {
      SERIAL_PRINT("///////////////////////////////////////////////////////////////////////////",1); 
      SERIAL_PRINT("Invalid Date",1); 
      for(COUNTER = 0;COUNTER < 15;COUNTER++)
      {
           UART_SEND(RX_BUFF[COUNTER]);
      }
    }
    else
    {
      SERIAL_PRINT(" ",1); 
      SERIAL_PRINT("Writing RTC",1);
      SET_TIME(T1,T2,T3);
      SET_DATE(T4,T5,T6);
      SEND_DATA_FLAG = 1;
    }
    
}

void DISPLAY_INT_FOUR(unsigned int HEX)
{
	unsigned int TEMP = 0;
	unsigned char POS = 0;

	TEMP = HEX;

	TEMP = TEMP % 10000;
	POS = TEMP / 1000;
	POS = 0x30 + POS;
	UART_SEND(POS);

	TEMP = TEMP % 1000;
	POS = (unsigned char)(TEMP / 100);
	POS = 0x30 + POS;
	UART_SEND(POS);

	TEMP = TEMP % 100;
	POS = (unsigned char)(TEMP / 10);
	POS = 0x30 + POS;
	UART_SEND(POS);

    POS = TEMP % 10;
	POS = 0x30 + POS;
	UART_SEND(POS);
    
    UART_SEND(0x0D);  
    UART_SEND(0x0A);
}

void DISPLAY_BAT(unsigned int HEX)
{
	unsigned int TEMP = 0;
	unsigned char POS = 0;

	TEMP = HEX;

	TEMP = TEMP % 1000;
	POS = (unsigned char)(TEMP / 100);
	POS = 0x30 + POS;
	UART_SEND(POS);

	TEMP = TEMP % 100;
	POS = (unsigned char)(TEMP / 10);
	POS = 0x30 + POS;
	UART_SEND(POS);
    UART_SEND('.');

    POS = TEMP % 10;
	POS = 0x30 + POS;
	UART_SEND(POS);
    UART_SEND('V');
    
    UART_SEND(0x0D);  
    UART_SEND(0x0A);
}

void DISPLAY_CHANGE_TIME(unsigned int HEX)
{
	unsigned int TEMP = 0;
	unsigned char POS = 0;

	TEMP = HEX;

	TEMP = TEMP % 1000;
	POS = (unsigned char)(TEMP / 100);
	POS = 0x30 + POS;
	UART_SEND(POS);

	TEMP = TEMP % 100;
	POS = (unsigned char)(TEMP / 10);
	POS = 0x30 + POS;
	UART_SEND(POS);

    POS = TEMP % 10;
	POS = 0x30 + POS;
	UART_SEND(POS);
}


void UART_ISR(void)
{
  if(RX_DATA == '#')
  {
     READ_FLAG = 0;
     CHECK_SERIAL_FLAG = 1;
  }
  else if(RX_DATA == '*')
  {
     RX_COUNTER = 0;
     READ_FLAG = 1;
  }
  else if(READ_FLAG)
  {
   RX_BUFF[RX_COUNTER] = RX_DATA;
   RX_COUNTER = RX_COUNTER + 1;
  }
  RX_DATA = 0;
}


void LED_INDICATION(void)
{
    //LED_RED
    
    if((CHNAGEOVER_DETECT == 1) || (CHARGING_FAILURE == 0))
    {
       DEBOUNCE_COUNTER0 = DEBOUNCE_COUNTER0 + 1;
        if(DEBOUNCE_COUNTER0 > 2000)
        {
            DEBOUNCE_COUNTER0 = 0;
            if(BAT_VOLTAGE > 90)
            {
                LED_RED = LED_ON; 
            }
        }
    }
    else if(BAT_NOT_HEALTHY)
    {
        FLASH_COUNTER1 = FLASH_COUNTER1 + 1;
        if(FLASH_COUNTER1 > 150)
        {
          LED_RED ^= 1;
          FLASH_COUNTER1 = 0;
        }
    }
    else
    {
       LED_RED = LED_OFF;
       DEBOUNCE_COUNTER0 = 0;
    }
    
    //LED_GREEN
    if(BAT_TEST_FLAG)
    {
        FLASH_COUNTER2 = FLASH_COUNTER2 + 1;
        if(FLASH_COUNTER2 > 150)
        {
          LED_GREEN ^= 1;
          FLASH_COUNTER2 = 0;
        }  
    }
    else if(BAT_VOLTAGE > 90 && BAT_VOLTAGE < 180) //9V
    {
            LED_GREEN = LED_ON;  
    }
    else
    {
       LED_GREEN = LED_OFF; 
    }
    
     //LED_UMBER
    if((BAT_VOLTAGE > 180) || (BAT_VOLTAGE < 80))
    {
            LED_UMBER = 0;
            LED_GREEN = LED_OFF;
            LED_RED = LED_OFF;
        
    }
    else if((BAT_VOLTAGE < 105) && (BAT_VOLTAGE > 90))//10.5V
    {
        FLASH_COUNTER3 = FLASH_COUNTER3 + 1;
        if(FLASH_COUNTER3 > 150)
        {
          LED_UMBER ^= 1;
          FLASH_COUNTER3 = 0;
        }  
    }
    else
    {
        LED_UMBER = 0; 
        DEBOUNCE_COUNTER1 = 0;
    }  
}

void BAT_TEST_LOGIC(void)
{ 
    if((DATE == 1 || DATE == 16) && !BAT_TEST_DONE_FLAG)
    {
        
        if(!BAT_TEST_FLAG  && !CHNAGEOVER_DETECT && BAT_VOLTAGE > 127 && !BAT_NOT_HEALTHY && ! BAT_TEST_DONE_FLAG)
        {
                if((TOTAL_TIME >= TEST_ON_TIME) && (TOTAL_TIME < TEST_OFF_TIME))
                {
                    BAT_TEST_FLAG = 1;
                    BAT_DISCHARGE = 1;
                    BAT_TEST_COUNTER = 0;
                    CHECK_BAT_FLAG = 1;
                    CHARGING_CONTROL = OFF;
                } 
        } 
        else if((!BAT_TEST_FLAG  && !CHNAGEOVER_DETECT && BAT_VOLTAGE < 127 && !BAT_NOT_HEALTHY && ! BAT_TEST_DONE_FLAG) && (TOTAL_TIME >= TEST_ON_TIME) && (TOTAL_TIME < TEST_OFF_TIME))
        {
           BAT_TEST_DONE_FLAG = 1; 
        }
        else if(BAT_TEST_FLAG)
        {
           if(CHNAGEOVER_DETECT || (TOTAL_TIME >= TEST_OFF_TIME) || (BAT_VOLTAGE < 105))
           {
               BAT_TEST_FLAG = 0;
               BAT_DISCHARGE = 0;
               BAT_TEST_COUNTER = 0;
               if(BAT_VOLTAGE < 105)
               {
                   BAT_TEST_DONE_FLAG  = 1;
               }
           }
           if(CHECK_BAT_FLAG)
           {
                BAT_TEST_COUNTER = BAT_TEST_COUNTER + 1;
                if(BAT_TEST_COUNTER > 50)//5 SEC TEST AS PER 6.22
                {
                    if(BAT_VOLTAGE < 122)
                    {
                         BAT_TEST_FLAG = 0;
                         BAT_DISCHARGE = 0;
                         BAT_NOT_HEALTHY = 1;
                    }
                    BAT_TEST_COUNTER = 0;
                    CHECK_BAT_FLAG = 0;
                }
           }
           else
           {  
              BAT_TEST_COUNTER = 0; 
           }
        }
    }
    else if((DATE == 2 || DATE == 17) && BAT_TEST_DONE_FLAG)
    {
       BAT_TEST_DONE_FLAG = 0; 
    }
}

void BAT_CHARGING(void)
{
    if((BAT_TEST_FLAG || BAT_VOLTAGE >= 144) && CHARGING_ON_FLAG)
    {
        CHARGING_COUNTER = CHARGING_COUNTER + 1;
        if(CHARGING_COUNTER >= 70)
        {
            CHARGING_CONTROL = OFF;
            CHARGING_COUNTER = 0;
            CHARGING_ON_FLAG = 0;
        }
    }
    else if((BAT_VOLTAGE < 135) && !CHARGING_ON_FLAG && !BAT_TEST_FLAG)
    {
        CHARGING_COUNTER = CHARGING_COUNTER + 1;
        if(CHARGING_COUNTER >= 70)
        {
            CHARGING_CONTROL = ON;
            CHARGING_COUNTER = 0;
            CHARGING_ON_FLAG = 1;
        }
    }
    else
    {    
      CHARGING_COUNTER = 0;  
    }

//    if(BAT_TEST_FLAG)
//    {
//      CHARGING_CONTROL = OFF;
//    }
//    else
//    {    
//      CHARGING_CONTROL = ON;  
//    }
}

void PWM_OFF(void)
{
    PWM_COUNTER = 0;
}


void CAL_PWM(void)
{
 
    float TEST1 = 0;
    float TEST2 = 0;
    float TEST3 = 0;
    float TEST4 = 0;
    
if(ARRAY1_ON_FLAG)
 {
  //y = 9.3071x2 - 3043.6x + 266646
    TEST1 = BAT_VOLTAGE * BAT_VOLTAGE;
    TEST2 = (float)(TEST1 * 9.3071);
    TEST3 = (float)(BAT_VOLTAGE * 3043.6);
    TEST4 = TEST2 - TEST3 + 266646;
 }
 else if(ARRAY2_ON_FLAG)
 {
   // y = 9.6098x2 - 2975.9x + 243590
    TEST1 = BAT_VOLTAGE * BAT_VOLTAGE;
    TEST2 = (float)(TEST1 * 9.6098);
    TEST3 = (float)(BAT_VOLTAGE * 2975.9);
    TEST4 = TEST2 - TEST3 + 243590;
        
 }
    PWM_COUNTER = TEST4;
}

/**
 End of File
*/