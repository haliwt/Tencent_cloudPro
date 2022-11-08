#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"


static void Delay_BuzzerUs(uint8_t t);


void FAN_CCW_RUN(void)
{
   FAN_CW_SetLow();
   FAN_CCW_SetHigh();
  
}

void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_CW_SetLow(); //brake
}


static void Delay_BuzzerUs(uint8_t t)
{

    uint16_t j;
	for(j=0;j<t;j++)
	{
	   for(int i = 0; i <100; i++)//better for(int i = 0; i < 100; i++)	 //for(int i = 0; i < 20; i++)	  
		{
			__asm("NOP");//等待1个指令周期，系统主频24M
		   
		}
	}


  

}

void Buzzer_On(void)
{
  static uint8_t i;
  #if 1
  for(i=0;i<50;i++){//
//        BUZZER_SetHigh();
//        Delay_BuzzerUs(10);//HAL_Delay(1);
//        BUZZER_SetLow() ;
		BUZZER_TOGGLE() ; 
        Delay_BuzzerUs(10);//HAL_Delay(1);
        
       
		
  }
  #endif 
 }

void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}

void SterIlization(uint8_t sel)
{
    if(sel==0){//open 
		 FAN_CCW_RUN();
		 PLASMA_SetHigh();
		 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 

	}
	else{ //close

			
		PLASMA_SetLow();
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
		//FAN_Stop();

	}



}

void Dry_Function(uint8_t sel)
{
   if(sel ==0){

  
		FAN_CCW_RUN();
		PTC_SetHigh();

   }
   else{

       PTC_SetLow();

   }

}

