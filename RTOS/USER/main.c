#include "LED.h"
#include "delay.h"
#include "usart.h"
#include "i2c_lcd.h"
#include "timer_drive.h"

/////////////////////////OS///////////////////////////////////

#define START_TASK_PRIO      			10 
#define START_STK_SIZE  				64
OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata);	
 			   
#define LED1_TASK_PRIO       			3
#define LED1_STK_SIZE  					64
OS_STK LED1_TASK_STK[LED1_STK_SIZE];

void led1_task(void *pdata);


#define SHOW_TASK_PRIO    			5 
#define SHOW_STK_SIZE  		 		64
OS_STK SHOW_TASK_STK[SHOW_STK_SIZE];

void show_task(void *pdata);


#define SERVO_TASK_PRIO       			8
#define SERVO_STK_SIZE  		    		64
OS_STK SERVO_TASK_STK[SERVO_STK_SIZE];

void servo_task(void *pdata);


#define MAIN_TASK_PRIO       		6
#define MAIN_STK_SIZE  					64
 OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];//////////////////// 

void main_task(void *pdata);

////////////////////////////////////////////////////////////
OS_EVENT * msg_key;	
 

uint16_t count ;
uint16_t flagin=0, flagout=0;
uint32_t i=0,n=1,h=0,m=0,t=0,k=0;
uint32_t  hh,mm,ss;
char a[16]; 
char str_percent[2],numberOfCar[5];

 int main(void)
 {	
	 
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 delay_init();	
	 
	 LED_Init();
	 USARTx_Init(USART1, Pins_PA9PA10, 115200);
	 I2C_LCD_Configuration();                            
	 OSInit();   
 	 OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );
	 OSStart();
 }

 void start_task(void *pdata){
	 
	 OS_CPU_SR cpu_sr=0;
	  pdata = pdata; 
	 
	 msg_key=OSMboxCreate((void*)0);	
	 OS_ENTER_CRITICAL();//	
	 
	 OSTaskCreate(led1_task,(void *)0,(OS_STK *)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO );
	 OSTaskCreate(show_task,(void *)0,(OS_STK*)&SHOW_TASK_STK[SHOW_STK_SIZE-1],SHOW_TASK_PRIO);
	 OSTaskCreate(servo_task,(void *)0,(OS_STK *)&SERVO_TASK_STK[SERVO_STK_SIZE-1],SERVO_TASK_PRIO );
	 OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);
	  
	 OSTaskSuspend(START_TASK_PRIO);	
	 OS_EXIT_CRITICAL();//
	 
 }
 


void led1_task(void *pdata)
{	  
	while(1)
	{
		if(m%2==1)
				LED1=0,
				delay_ms(130);
		else
				LED1=1,
				delay_ms(130);
	};
}

void servo_task(void *pdata)
{	  
	count = 0;
	servo_180_deg(PA,8, 90);
	while(1)
	{
			if(IRin == 0 && flagin==0){
				if(count<5){flagin=1;
					if(flagout==0)
							{servo_180_deg(PA,8, 0); count++;}}
				else{count=5;}
	}

			if(IRout == 0 && flagout==0){
					if(count>0){flagout=1;
				if(flagin==0)
						{servo_180_deg(PA,8, 0);count--;}}
					else {count=0;}
			}
			
			if(flagin==1 && flagout==1){
					delay_ms(1000);
						servo_180_deg(PA,8, 90);
							flagin=0, flagout=0;
				}
			delay_ms(500);
	};
}

void main_task(void *pdata)
{	
	
 	char buffer[100];
 	while(1)
	{		
		if (USART_Gets(USART1, buffer, sizeof(buffer))){				
			h=0;m=0;t=0;i=0;
			if(buffer[i+1]==':'){
		h=buffer[i]-48;
				i=i+2;
			}
			else 
			{h=(buffer[i]-48)*10+ buffer[i+1] -48;i=i+3;}		
				if(buffer[i+1]==':'){
						m=buffer[i]-48;
								i=i+2;}
				else 
				{m=(buffer[i]-48)*10+ buffer[i+1] -48;i=i+3;}		
					sprintf(a,"Time: %d:%d",h,m);
					delay_ms(200);
			}
			else{
				delay_ms(700);
			}
		}
}	

void show_task(void *pdata)
{
	lcd_init ();
	while(1)
	{
		Delete_LCD();	
		sprintf (numberOfCar, "%d car", count);
		if(count<=0){
		lcd_send_string(a);
		lcd_Control_Write(0xc0);
		lcd_send_string("EMPTY: ");
		lcd_send_string(numberOfCar);
		}
		else if(count>=5){
		lcd_send_string(a);
		lcd_Control_Write(0xc0);
		lcd_send_string("Full: ");
		lcd_send_string(numberOfCar);
		}
		else {
		lcd_send_string(a);
		lcd_Control_Write(0xc0);
		lcd_send_string("Available: ");
		lcd_send_string(numberOfCar);
		}
		delay_ms(200);
	}									 
}

