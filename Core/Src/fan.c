#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"




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

void Fan_One_Speed(void)
{
    static uint16_t fan_speed;
	fan_speed ++ ;
	//FAN_CW_SetLow();

	if(fan_speed < 80){
	  FAN_CW_SetLow();
	   FAN_CCW_SetHigh(); //Run fan
    }
    else if(fan_speed >79  && fan_speed <100){
      FAN_CW_SetHigh();;
	  FAN_CCW_SetLow(); //brake

	}
    else if(fan_speed > 99 && fan_speed < 179){
		 FAN_CW_SetLow();
		 FAN_CCW_SetHigh(); //Run fan
		
	}
	else{
		fan_speed =0;
	}

}

void Fan_Two_Speed(void)
{
    static uint16_t fan_speed;
	fan_speed ++ ;
	//FAN_CW_SetLow();
	#if 1
	
	if(fan_speed < 90){
	   FAN_CW_SetLow();
	   FAN_CCW_SetHigh(); //Run fan
    }
   else if(fan_speed >89  && fan_speed <100){
      FAN_CW_SetHigh();
	  FAN_CCW_SetHigh(); //Run fan

	}
    else if(fan_speed > 99 && fan_speed < 189 ){
		FAN_CW_SetLow();
		FAN_CCW_SetHigh(); //Run fan
		
	}
	else{
	   fan_speed =0;


	}
   #endif 

}

void Fan_Full_Speed(void)
{
     FAN_CCW_RUN();

}

void Fan_Slowly_Speed(void)
{
    static uint16_t fan_speed;
	fan_speed ++ ;
	FAN_CW_SetLow();
	
	if(fan_speed < 100){
	   FAN_CCW_SetHigh(); //Run fan
    }

	if(fan_speed > 99  && fan_speed < 103){
  
	  FAN_CCW_SetLow(); //brake
	  FAN_CCW_SetHigh(); //Run fan
	  FAN_CCW_SetLow(); //brake

	}

	if(fan_speed > 102){
		 FAN_CCW_SetHigh(); //Run fan
		fan_speed =0;
	 }
	
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

void Fan_RunSpeed_Fun(void)
{
         if(run_t.set_wind_speed_value < 33){
              Fan_One_Speed();
		 }
		 else if(run_t.set_wind_speed_value > 32 && run_t.set_wind_speed_value < 67 ){

             Fan_Two_Speed();

		 }
		 else
		 	FAN_CCW_RUN();


}


