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

    static uint8_t tm0 ,tm1,tm2;
    if(htim->Instance==TIM3){
		
	   tm0 ++ ;
       run_t.gTimer_senddata_panel++;
	 if(tm0 > 99){//100ms *10 = 1000ms =1s
        tm0 =0;
        tm2++;
    
       run_t.gTimer_1s ++;
       esp8266data.gTimer_publish_dht11++;
	   esp8266data.gTimer_publish_timing++;
	   esp8266data.gTimer_subscription_timing++;
	  
	   if(run_t.gFan_continueRun ==1){
	   	   tm1++;
           run_t.gFan_counter++;
		   if(tm1 > 60){ //1 minute
		   	  tm1=0;
	          run_t.gTimer_60s =1;
              
		   }
	   }
      if(tm2 > 59){ // 1minute 
         tm2=0;
       
      
      }

	 }
 	}
 }
