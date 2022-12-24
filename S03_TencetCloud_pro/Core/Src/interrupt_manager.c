#include "interrupt_manager.h"
#include "run.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "cmd_link.h"
#include "usart.h"






/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t tm0 ,tm1,tm2,tm3;
    if(htim->Instance==TIM3){
	   tm0 ++ ;
     tm2++;
     if(tm2 > 49){
       tm2 =0;
      

     }
	 if(tm0 == 100){//100ms *10 = 1000ms =1s
        tm0 =0;
        tm3++;
    
    
	    run_t.sendtimes++;
       run_t.gTimer_wifi_1s++;
	    wifi_t.timer_wifi_send_cmd++;
	   esp8266data.esp8266_timer_1s++;
	   run_t.gTimer_send_0xaa++;
	   
	   esp8266data.esp8266_timer_link_1s++;
	   esp8266data.gTimer_subscription_timing++;
	   esp8266data.gTimer_tencent_down_1s++;
       esp8266data.esp8266_timer_dynamic_1s++;
	   esp8266data.gTimer_publish_timing++;
	   
	  
	   if(run_t.gFan_continueRun ==1){
	   	   tm1++;
           run_t.gFan_counter++;
		   if(tm1 > 60){
		   	  tm1=0;
	          run_t.gTimer_60s =1;
              
		   }
	   }
      if(tm3 > 59){ // 1minute 
         tm3=0;
       
      
      }

	 }
 }
 }