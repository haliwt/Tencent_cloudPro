#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"

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






void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}
//"杀毒" 
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



