/*
 * TIMER1_Prog.c
 *
 * Created: 2/20/2023 1:28:06 AM
 *  Author: Mega
 */ 
#define F_CPU 16000000
#include "TIMER1_Interface.h"
#include "TIMER1_conf.h"
static void(*Timers_timer1_ovf)(void)=NULL;
static  void(*Timers_timer1_CTC)(void)=NULL;
static  void(*Timers_timer1_ICU)(void)=NULL;
/*
void timer1_init(void)
{
	#if timers_timer1_mode  == Normal
		//select modE Normal(0)
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
		//enable interrupt 
		SET_BIT(TIMSK,2);
        //ICR
        ICR1_u16=timers_timer1_ovf_VALUE;
	#elif   timers_timer1_mode  == PWM
		//select mode FAST PWM(14)
		CLR_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
		//pwm non inverting mode
		CLR_BIT(TCCR1A,6);
		SET_BIT(TCCR1A,7);
		//ICR
		ICR1_u16=timers_timer1_ovf_VALUE;
	#elif   timers_timer1_mode  == CTC
	    //select mode CTC
	    CLR_BIT(TCCR1A,0);
	    CLR_BIT(TCCR1A,1);
	    SET_BIT(TCCR1B,3);
	    CLR_BIT(TCCR1B,4);
	    //enable interrupt
	    SET_BIT(TIMSK,4);
	#endif
}
void timers_timer1_set_OCRA(u16 OCR1A_REG)
{
	OCR1A_u16=OCR1A_REG;   
	
}
void timers_timer1_start(void)
{
	//prescaller 64
	SET_BIT(TCCR1B,0);
	SET_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
	
}
void timers_timer1_stop(void)
{
	CLR_BIT(TCCR1B,0);
	CLR_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
}

*/

volatile  static u32 tick_1=0;
volatile  static u32 tick_2=0;
volatile  static u32 tick_3=0;

volatile  static u32 total_time=0;
void timers_timer1_ICU_init(void)
{
	// Select Mode = Normal Mode
	CLR_BIT(TCCR1A,0);
	CLR_BIT(TCCR1A,1);
	CLR_BIT(TCCR1B,3);
	CLR_BIT(TCCR1B,4);
	//rising edge
	SET_BIT(TCCR1B,6);
	//activate interrupt
	SET_BIT(TIMSK,5);
	// clear counter
	TCNT1=0;
	//no prescaller
	SET_BIT(TCCR1B,0);
	CLR_BIT(TCCR1B,1);
	CLR_BIT(TCCR1B,2);
}

u8 timers_timer1_ICU_get_duty(void)
{

	u32 tick_on=tick_2-tick_1;
	total_time=tick_3-tick_1;
	float duty=((float)tick_on/(float)total_time)*100;
	
	return duty ;
}
u32 timers_timer1_Icu_get_freq(void)
{
	u32 freq=(F_CPU / total_time);
	return freq;
}


void __vector_6(void)__attribute__((signal));
void __vector_6(void)
{	static u8 flag=0;
	static u16 counter_value;
	
	counter_value=ICR1  ;
	if(flag==0)
	{
		tick_1=counter_value;
		flag=1;
		CLR_BIT(TCCR1B,6);
	}
	else if (flag==1)
	{
		tick_2=counter_value;
		SET_BIT(TCCR1B,6);
		flag=2;
	}
	else if (flag==2)
	{
		tick_3=counter_value;
		CLR_BIT(TIMSK,5);
	}
}















/*

void timers_timer1_INC_set_callback(void(*ptr)(void))
{
	if (ptr != NULL)
	{
		Timers_timer1_ICU=ptr;
	}
}


void timers_timer1_ovf_set_callback(void(*ptr)(void))
{
	if (ptr!=NULL)
	{
		Timers_timer1_ovf=ptr;
	}
}
void timers_timer1_CTCA_set_callback(void(*ptr)(void))
{
	if (ptr!=NULL)
	{
		Timers_timer1_CTC=ptr;
	}
}


//ISR function for timer1 ovf
void __vector_9(void)__attribute__((signal));
void __vector_9(void)
{
	
	if (timers_timer0_ovf_counter==0)
	{
		//set preload
       	TCNT0=timers_timer0_preload;
       	//call action
       	if (Timers_timer1_ovf != NULL)
       	{
	       	Timers_timer1_ovf();
       	}
	} 
	else
	{
		static u16 counter_OVF=0;
		++counter_OVF;
		if (counter_OVF==timers_timer0_ovf_counter)
		{
			counter_OVF=0;
			//set preload
			TCNT0=timers_timer0_preload;
			//call action
			if ( Timers_timer1_ovf != NULL)
			{
				Timers_timer1_ovf();
			}
		}
	}
	
}

//ISR function for timer1 CTC A
void __vector_7(void)__attribute__((signal));
void __vector_7(void)
{
	
	if (timers_timer0_CTC_counter==0)
	{
		//call action
		if (Timers_timer1_CTC != NULL)
		{
			Timers_timer1_CTC();
		}
	}
	else
	{
		static u16 counter_CTC=0;
		++counter_CTC;
		if (counter_CTC==timers_timer0_CTC_counter)
		{
			counter_CTC=0;
			//call action
			if ( Timers_timer1_CTC != NULL)
			{
				Timers_timer1_CTC();
			}
		}
	}
	
}
*/