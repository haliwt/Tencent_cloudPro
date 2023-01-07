#include "special_power.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "run.h"
#include "wifi_fun.h"
#include "esp8266.h"

void (*Single_Usart_ReceiveData)(uint8_t cmd);

void SetPowerOn_ForDoing(void)
{
    Buzzer_On();
    run_t.gFan_counter=0;
     run_t.gPower_flag = POWER_ON;
    run_t.gFan_continueRun =0;
    run_t.gPower_On=POWER_ON;
	run_t.gmt_time_flag=0;
	run_t.wifi_gPower_On = 1;

    run_t.gFan = 1;
	run_t.gDry = 1;
	run_t.gPlasma =1;       //"杀菌"
	run_t.gUlransonic = 1; // "驱虫"
	run_t.gModel =1;
    run_t.gFan_counter=0;
	
    FAN_CCW_RUN();
    PLASMA_SetHigh(); //
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetHigh();
	
    
    

}

void SetPowerOff_ForDoing(void)
{
    Buzzer_On();
	run_t.gPower_flag = 0;
	run_t.gFan_counter=0;
	run_t.gFan_continueRun =1; //the fan still run 60s
	run_t.gPower_On == POWER_OFF;
	run_t.wifi_gPower_On = 0;
 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUlransonic = 0; // "驱虫"
	run_t.gModel =0;
	
    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	FAN_Stop();

}

void Single_Usart_RxData(void(*rxHandler)(uint8_t dat))
{

      Single_Usart_ReceiveData=  rxHandler;

}
#if 0
void Updaet_DisplayPanel_Data_Handler(void)
{
   if(esp8266data.esp8266_login_cloud_success==1){
   	   SendWifiData_To_Cmd(0x01);

   	}
   else{
      SendWifiData_To_Cmd(0);

   }
   //display panel of command 
   if(esp8266data.esp8266_login_cloud_success==1){

       if(run_t.wifi_gPower_On == 1){ //power on 
	   	   if(run_t.gPower_flag == POWER_OFF){
               run_t.gPower_On = POWER_ON;
			   SendWifiCmd_To_Order(WIFI_POWER_ON);

		   }
       }
	   else{ //power off
          if(run_t.gPower_flag ==POWER_ON){

		     run_t.gPower_On = POWER_OFF;
			 SendWifiCmd_To_Order(WIFI_POWER_OFF);

		  }

	   }
	   //"ptc" 
	   if(run_t.gDry == 1){
	      SendWifiCmd_To_Order(WIFI_PTC_ON);

	   }
	   else
	   	  SendWifiCmd_To_Order(WIFI_PTC_OFF);

	   //Anion -> "灭菌"
	   if(run_t.gPlasma == 1)
	   	   SendWifiCmd_To_Order(WIFI_KILL_ON);
	   else
	   	   SendWifiCmd_To_Order(WIFI_KILL_OFF);

	   //sonic -> "驱虫"
	   if(run_t.gUlransonic ==1)
	   	   SendWifiCmd_To_Order(WIFI_SONIC_ON);
	   else
	   	   SendWifiCmd_To_Order(WIFI_SONIC_OFF);
	   //state -> model
	   if(run_t.gModel ==1)
		   SendWifiCmd_To_Order(WIFI_MODE_1);
	   else
	   	   SendWifiCmd_To_Order(WIFI_MODE_2);
	   
   }


}
#endif 
