#include "special_power.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "run.h"
#include "wifi_fun.h"
#include "esp8266.h"
#include "buzzer.h"

void (*Single_Usart_ReceiveData)(uint8_t cmd);

void SetPowerOn_ForDoing(void)
{
    
    run_t.gPower_flag = POWER_ON;
    run_t.gFan_continueRun =0;
    run_t.gPower_On=POWER_ON;
	run_t.gmt_time_flag=0;
	run_t.wifi_gPower_On = 1;


	
    if(run_t.app_timer_power_on_flag==0){
	    run_t.gFan = 1;
		run_t.gDry = 1;
		run_t.gPlasma =1;       //"杀菌"
		run_t.gUlransonic = 1; // "驱虫"
	    run_t.gFan_counter=0;

			
	    Fan_RunSpeed_Fun();//FAN_CCW_RUN();
	    PLASMA_SetHigh(); //
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	    PTC_SetHigh();
    }
	run_t.gModel =1;  //AI

	
 }

void SetPowerOff_ForDoing(void)
{
   
	run_t.gPower_flag = 0;
	run_t.gFan_continueRun =1; //the fan still run 60s
	run_t.gPower_On = POWER_OFF;
	run_t.wifi_gPower_On = 0;
	run_t.app_timer_power_on_flag=0;
 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUlransonic = 0; // "驱虫"
	//run_t.gModel =1;
	
    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	FAN_Stop();

}

void Single_Usart_RxData(void(*rxHandler)(uint8_t dat))
{

      Single_Usart_ReceiveData=  rxHandler;

}

void ActionEvent_Handler(void)
{
     

if(run_t.works_break_power_on==0) { 

    Fan_RunSpeed_Fun();
	if(run_t.gDry == 1){

	   PTC_SetHigh();

	}
	else{
		   PTC_SetLow();
	
	}
	//kill
	if(run_t.gPlasma == 1){
		
	     PLASMA_SetHigh();
	}
	else{

		PLASMA_SetLow();
	}
	//driver bug
	if(run_t.gUlransonic ==1){
	
	 
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}

	if(run_t.gPlasma ==0 && run_t.gDry==0){

        run_t.gFan_counter=0;
		run_t.gFan_continueRun=1;        

	}
		
   }


}


