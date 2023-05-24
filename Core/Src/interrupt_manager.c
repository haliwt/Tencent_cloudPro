#include "interrupt_manager.h"
#include "run.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "cmd_link.h"
#include "usart.h"
#include "fan.h"
#include "subscription.h"
#include "buzzer.h"


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2){

		if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_ORE)!=RESET){

             __HAL_UART_CLEAR_OREFLAG(&huart2);

		}
		__HAL_UNLOCK(&huart2);


	}
	if(huart->Instance==USART1){
	
			if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE)!=RESET){
	
				 __HAL_UART_CLEAR_OREFLAG(&huart1);
	
			}
			__HAL_UNLOCK(&huart1);
	
	
		}




}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
        strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);

#if 1
		 
		   	  
               if(strstr((const char*)esp8266data.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              	//	esp8266data.soft_ap_config_success=1;
					esp8266data.esp8266_login_cloud_success=1;
	                esp8266data.linking_tencent_cloud_doing=0;
					wifi_t.soft_ap_config_flag=0;
               	}
                else if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
	              esp8266data.esp8266_login_cloud_success=1;
	              esp8266data.linking_tencent_cloud_doing=0;
				  run_t.auto_link_cloud_flag=0xff;
				  wifi_t.wifi_reconnect_read_flag = 0;
                   wifi_t.soft_ap_config_flag=0;
			  }
              else{
           
           
                   UART2_DATA.UART_Flag = 0;
                   UART2_DATA.UART_Cnt=0;
                  
              }
		 
         
#endif        
            
}


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t state=0;
   
    //wifi usart2
//    if(huart->Instance==USART2)
//    {
//           
//     if(USART2->ISR & UART_FLAG_RXFNE){
//
//	  UART2_DATA.UART_DataBuf[0] = USART2 ->RDR;
//      
//		  
//	
//	
//	      if(esp8266data.linking_tencent_cloud_doing ==1){
//
//			UART2_DATA.UART_Data[UART2_DATA.UART_Cnt] = UART2_DATA.UART_DataBuf[0];
//			UART2_DATA.UART_Cnt++;
//
//			if(*UART2_DATA.UART_DataBuf==0X0A) // 0x0A = "\n"
//			{
//				UART2_DATA.UART_Flag = 1;
//				Wifi_Rx_InputInfo_Handler();
//				UART2_DATA.UART_Cnt=0;
//			}
//
//	      } 
//		  else{
//
//		        
//
//		        if(wifi_t.get_rx_beijing_time_flag==1){
//					UART2_DATA.UART_Data[UART2_DATA.UART_Cnt] = UART2_DATA.UART_DataBuf[0];
//					UART2_DATA.UART_Cnt++;
//
//				}
//				else
//				Subscribe_Rx_Interrupt_Handler();
//	      }
//     	}
//	   
//     // HAL_UART_Receive_IT(&huart2,UART2_DATA.UART_DataBuf,1);
//	}

	
	if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
        //test_counter_usat1++;
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 'T')  //hex :54 - "T" -fixed
				state=1; //=1
		
			break;
		case 1: //#1
             if(inputBuf[0] == 'K')  //hex :4B - "K" -fixed
				state=2; //=1
			else
			   state =0;
			break;
            
        case 2:
             inputCmd[0]= inputBuf[0];
             state = 3;
        
        break;
        
        case 3:
            inputCmd[1]= inputBuf[0];
            run_t.decodeFlag =1;
            state = 0;
        
        break;
	
		default:
			state=0;
			run_t.decodeFlag =0;
		}
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
	 }
    
  
 }

/***********************************************************************************
	*
	*Function Name: void USART2_Interrupt_Rx_Handler(void)
	*
	*
	*
	*
***********************************************************************************/
void USART2_Interrupt_Rx_Handler(void)
{
	if(USART2->ISR & UART_FLAG_RXFNE){

	  UART2_DATA.UART_DataBuf[0] = USART2 ->RDR;
      
		  
	
	
	      if(esp8266data.linking_tencent_cloud_doing ==1 && UART2_DATA.UART_Flag ==0){

			UART2_DATA.UART_Data[UART2_DATA.UART_Cnt] = UART2_DATA.UART_DataBuf[0];
			UART2_DATA.UART_Cnt++;

			if(*UART2_DATA.UART_DataBuf==0X0A) // 0x0A = "\n"
			{
				UART2_DATA.UART_Flag = 1;
				//strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);
				Wifi_Rx_InputInfo_Handler();
			
			}

	      } 
		  else{

		        

		        if(wifi_t.get_rx_beijing_time_flag==1){
					UART2_DATA.UART_Data[UART2_DATA.UART_Cnt] = UART2_DATA.UART_DataBuf[0];
					UART2_DATA.UART_Cnt++;

				}
				else
				Subscribe_Rx_Interrupt_Handler();
	      }
     	}


}
/***********************************************************************************
	*
	*Function Name: void USART2_Interrupt_Rx_Handler(void)
	*
	*
	*
	*
***********************************************************************************/
static void USART2_Parse_Fun(void)
{
     if(UART2_DATA.UART_Flag == 1){

               if(strstr((const char*)esp8266data.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              	//	esp8266data.soft_ap_config_success=1;
					esp8266data.esp8266_login_cloud_success=1;
	                esp8266data.linking_tencent_cloud_doing=0;
					wifi_t.soft_ap_config_flag=0;
               	}
                if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
                   	esp8266data.esp8266_login_cloud_success=1;
	                esp8266data.linking_tencent_cloud_doing=0;
					wifi_t.soft_ap_config_flag=0;
               }

			}
            else{
				  if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
	              esp8266data.esp8266_login_cloud_success=1;
	              esp8266data.linking_tencent_cloud_doing=0;
				  run_t.auto_link_cloud_flag=0xff;
				  wifi_t.wifi_reconnect_read_flag = 0;
			  }
           
           }
         UART2_DATA.UART_Flag = 0;
         UART2_DATA.UART_Cnt=0;
		 

}


/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t tm0,tm1;
    if(htim->Instance==TIM3){
		
	   tm0 ++ ;
       run_t.gTimer_senddata_panel++;
	 if(tm0 > 99){//100ms *10 = 1000ms =1s
        tm0 =0;
		tm1++;
        run_t.gTimer_1s ++;
       esp8266data.gTimer_publish_dht11++;
	   esp8266data.gTimer_publish_timing++;
	   esp8266data.gTimer_subscription_timing++;

	    wifi_t.gTimer_get_beijing_time++;

	   wifi_t.gTimer_beijing_time++;
	
	  
	 
	   wifi_t.gTimer_get_beij_times++;
	
	   run_t.gTimer_usart_error++;
	   wifi_t.gTimer_reconnect_wifi++;
	   run_t.gTimer_usart2_error++;

	   run_t.gTimer_to_publish_updata++;
	   run_t.gTimer_send_dit++;
	   wifi_t.gTimer_reconnect_wifi_order++;
	   wifi_t.gTimer_power_off++;
	  
	   if(run_t.gFan_continueRun ==1){
           run_t.gFan_counter++;
		 
	   }
	 
          
	   
      

	 }
 	}
 }
