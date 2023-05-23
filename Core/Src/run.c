#include "run.h"
#include "wifi_fun.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "special_power.h"
#include "buzzer.h"
#include "esp8266.h"
#include "mqtt_iot.h"
#include "publish.h"
#include "flash.h"
#include "usart.h"
#include "subscription.h"


RUN_T run_t; 

static void Single_Power_ReceiveCmd(uint8_t cmd);
static void Single_Command_ReceiveCmd(uint8_t cmd); 
uint8_t tencent_cloud_flag;




/**********************************************************************
*
*Function Name:void Decode_RunCmd(void)
*Function: receive usart touchkey of command 
*Input Ref:NO
*Return Ref:NO
*
**********************************************************************/
void Decode_RunCmd(void)
{

 uint8_t cmdType_1=inputCmd[0],cmdType_2 = inputCmd[1],i;
 uint32_t temp,temp1;
    
  switch(cmdType_1){
  
      case 'P': //power on and off
        
        //   Buzzer_KeySound();
           Single_Power_ReceiveCmd(cmdType_2);  
           
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_flag==POWER_ON){
	      if(cmdType_2==1){

		     // WIFI_IC_ENABLE();
			  tencent_cloud_flag=0;
			  // wifi_link_tencent_cloud:
			  Buzzer_KeySound();	
		     // InitWifiModule_Hardware();
               esp8266data.esp8266_login_cloud_success=0;
              run_t.wifi_config_net_lable=wifi_set_restor;
              wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 
                temp =USART2->ISR;
	     		temp = USART2->RDR;
			
				
				__HAL_UART_CLEAR_OREFLAG(&huart2);
              __HAL_UART_CLEAR_NEFLAG(&huart2);
               __HAL_UART_CLEAR_FEFLAG(&huart2);

			  UART_Start_Receive_IT(&huart2,(uint8_t *)UART2_DATA.UART_DataBuf,1);
				
	
		   }
		   else if(cmdType_2==0){
                
                Buzzer_KeySound();
		   }
		   else if(cmdType_2==0x14){
                run_t.gModel =2; //Timer timing of model
                Buzzer_KeySound();
		        MqttData_Publish_SetState(0x0); //Ai model->timer_time
		        HAL_Delay(200);
            }
            else if(cmdType_2==0x04){
                run_t.gModel =1;  // AI model 
                Buzzer_KeySound();
			    MqttData_Publish_SetState(0x1); //Ai model->beijing_time
			    HAL_Delay(200);
            }
           
           
        }
       
	   break;
        
      case 'C':
           if(run_t.gPower_flag==POWER_ON){
              Single_Command_ReceiveCmd(cmdType_2); //Single_ReceiveCmd(cmdType_2); 
              
           }
     
         
      break;

	  case 'M':
	  	if(run_t.gPower_flag==POWER_ON){
           
             run_t.set_temperature_value = cmdType_2;
			 if(esp8266data.esp8266_login_cloud_success==1)
			       MqttData_Publis_SetTemp(run_t.set_temperature_value);
			 	   HAL_Delay(200);
			  
         }
	  

	  break;

	  case 'T':
		  if(run_t.gPower_flag==POWER_ON){
             #if 0
             run_t.set_temperature_value = cmdType_2;
			 if(esp8266data.esp8266_login_cloud_success==1)
				 MqttData_Publis_SetTime(run_t.set_temperature_value);
			#endif    
         }
	  

	  break;


	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_flag==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
		        run_t.buzzer_sound_flag = 1;
			   // Buzzer_KeySound();
			}
			 
		
		}
     
	    break;
 	}
    
}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Power_ReceiveCmd(uint8_t cmd)
{
  
    
    switch(cmd){

    case 0x01: // power on

	    
         Buzzer_KeySound();
         run_t.gPower_flag = POWER_ON;
		 run_t.gPower_On = POWER_ON;
         run_t.RunCommand_Label= POWER_ON;
		  run_t.gModel =1;
		 run_t.set_wind_speed_value=100;
		 Update_DHT11_Value();
		 HAL_Delay(200);
		 if(esp8266data.esp8266_login_cloud_success==1){
		 	 run_t.gUlransonic =1;
			 run_t.gPlasma =1;
		     run_t.gDry =1;
			 run_t.set_wind_speed_value=100;
             run_t.wifi_gPower_On=1;
	

			 MqttData_Publish_SetOpen(1);  
			HAL_Delay(200);
			 MqttData_Publish_Update_Data();//MqttData_Publish_SetOpen(1);  //MqttData_Publish_SetOpen(0x01);
	         HAL_Delay(200);
	         Publish_Data_ToTencent_Initial_Data();
			 HAL_Delay(200);
		 }
	   
		 
	cmd=0xff;  
     break;


	

    case 0x00: //power off
       
        Buzzer_KeySound();
        run_t.gPower_On=POWER_OFF;
        run_t.gPower_flag = POWER_OFF;
        run_t.RunCommand_Label = POWER_OFF;
		 run_t.set_wind_speed_value=10;
		 run_t.gModel =1;
		Update_DHT11_Value();
		 HAL_Delay(200);
         if(esp8266data.esp8266_login_cloud_success==1){ 
         	 run_t.gUlransonic =0;
			 run_t.gPlasma =0;
		     run_t.gDry =0;
			  run_t.set_wind_speed_value=10;
             run_t.wifi_gPower_On=0;
			MqttData_Publish_SetOpen(0);  
			HAL_Delay(200);
			MqttData_Publish_PowerOff_Ref(); 
			 HAL_Delay(200);
         }
     
    cmd = 0xff;
    break;
         
    case 0xAA: //power_on 
      if(run_t.gPower_flag == POWER_ON){
       run_t.works_break_power_on = 0;

      	}
	   
    break;

	case 0x55: //power off
	   if(run_t.gPower_flag == POWER_ON){
		PTC_SetLow();
		PLASMA_SetLow();
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off	
        run_t.gFan_continueRun=1; 
		run_t.gFan_counter=0;
		run_t.works_break_power_on = 1;
	   	}

	break;

   

    

     default:

     break;

    }

}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Command_ReceiveCmd(uint8_t cmd)
{
    static uint8_t no_buzzer_sound_dry_off;
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	        run_t.noBuzzer_sound_dry_flag =1;

       case DRY_ON:
         run_t.gDry = 1;
	      run_t.gFan_continueRun =0;
	   if(run_t.noBuzzer_sound_dry_flag !=1){
		     Buzzer_KeySound();
		 }
		if(esp8266data.esp8266_login_cloud_success==1)
		 MqttData_Publish_SetPtc(0x01);
		 HAL_Delay(200);
		 
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			run_t.gDry = 0;
			if( no_buzzer_sound_dry_off !=1)
			    Buzzer_KeySound();
			 if(run_t.gPlasma ==0){ //plasma turn off flag
			  run_t.gFan_counter =0;
			   run_t.gFan_continueRun =1;

		     }
			if(esp8266data.esp8266_login_cloud_success==1)
			MqttData_Publish_SetPtc(0x0);
			HAL_Delay(200);
			
       break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
       		run_t.gUlransonic =1;
	    Buzzer_KeySound();
	   if(esp8266data.esp8266_login_cloud_success==1){
	        MqttData_Publish_SetPlasma(1) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(1); //超声波
	        HAL_Delay(200);
	   	}
	   
       break;

       case PLASMA_OFF:
           run_t.gPlasma=0;
           run_t.gUlransonic =0;
	    Buzzer_KeySound();
	   if(esp8266data.esp8266_login_cloud_success==1){
	       MqttData_Publish_SetPlasma(0) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(0); //超声波
	        HAL_Delay(200);
	   	}
	   
       break;

	   case MODE_AI_NO_BUZZER :
	   	  run_t.gModel =1;  //AI_Works_Model 
		  MqttData_Publish_SetState(0x1); //Ai model->beijing_time
		  HAL_Delay(200);

	   break;

	   case WIFI_CONNECT_FAIL:

	       run_t.dp_link_wifi_fail =1;


	   break;

	   case DISPLAY_PANNEL_CONNECT_WIFI_SUCCESS:

	      
	        run_t.dp_link_wifi_fail =0;


	   break;




      default :
        cmd =0;

      break; 


    }



}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void SystemReset(void)
{
    
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
		

}

/**********************************************************************
	*
	*Functin Name: void  RunCommand_MainBoard_Fun(void)
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void RunCommand_MainBoard_Fun(void)
{
   uint8_t i;
   static uint8_t power_just_on,send_link_times,the_first_power_off;
    
    if(run_t.buzzer_sound_flag == 1){
	 	run_t.buzzer_sound_flag = 0;
	    Buzzer_KeySound();

	 }
  
   switch(run_t.RunCommand_Label){

	case POWER_ON: //1
		SetPowerOn_ForDoing();
	    run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;
		power_just_on=0;
        run_t.gTimer_1s=0;
        run_t.gPower_repeat_times_flag =1;
		Update_DHT11_Value();
		HAL_Delay(10);

		if(esp8266data.esp8266_login_cloud_success==1){
			tencent_cloud_flag =1;
	 	    SendWifiData_To_Cmd(0x01) ;
		}
		
		if(tencent_cloud_flag ==1){

		   esp8266data.esp8266_login_cloud_success=1;
		   SendWifiData_To_Cmd(0x01) ;

		}
		if( run_t.app_timer_power_on_flag == 1){
		     run_t.app_timer_power_on_flag=0;
       
            for(i=0;i<36;i++){
		      TCMQTTRCVPUB[i]=0;
		     }

		}

	break;
        
    case POWER_OFF: //2
		SetPowerOff_ForDoing();
	    HAL_Delay(10);
		 run_t.send_link_cloud_times=0;
		if(esp8266data.esp8266_login_cloud_success==1){
	 	     tencent_cloud_flag = 1;
			 esp8266data.linking_tencent_cloud_doing =0;
	
			Update_DHT11_Value();
			HAL_Delay(200);

			//MqttData_Publish_SetOpen(0);
	       // HAL_Delay(200);

			MqttData_Publish_PowerOff_Ref(); 
			HAL_Delay(200);
		
		}
		
		if(the_first_power_off ==0){

		    the_first_power_off++;
			
        	Subscriber_Data_FromCloud_Handler();
		    HAL_Delay(200);
			run_t.RunCommand_Label = POWER_NULL;
		}
		else{
		  run_t.gFan_counter=0;
		  run_t.RunCommand_Label = FAN_CONTINUCE_RUN_ONE_MINUTE;
		  
		 }
         
	   if( run_t.app_timer_power_off_flag == 1){
		     run_t.app_timer_power_off_flag=0;
       
            for(i=0;i<36;i++){
		      TCMQTTRCVPUB[i]=0;
		     }

		}
       
      
	break;

   case UPDATE_TO_PANEL_DATA: //4
     if(run_t.gTimer_senddata_panel >30 && run_t.gPower_On==POWER_ON){ //300ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();
	      

	 }
     if(run_t.gTimer_send_dit > 49){

          run_t.gTimer_send_dit=0;
		  Update_DHT11_Value();



	  }
	 
	 if((run_t.gTimer_1s>32 && run_t.gPower_flag == POWER_ON)||power_just_on < 10){
		   power_just_on ++ ;
		   run_t.gTimer_1s=0;
		   Update_DHT11_Value();
	
	}
	
    break;

	case FAN_CONTINUCE_RUN_ONE_MINUTE:

         if(run_t.gPower_On == POWER_OFF && run_t.app_timer_power_off_flag ==0){
		  if(run_t.gFan_counter < 60){
          
                    Fan_One_Speed();
                  
            }       
           else{
		           
				   run_t.gFan_counter=0;
				   run_t.RunCommand_Label = POWER_NULL;
			      
				   FAN_Stop();
	         }
	  
         }
	break;

	case POWER_ON_FAN_CONTINUCE_RUN_ONE_MINUTE:

	    
	 if(run_t.gPower_flag ==POWER_ON && run_t.gFan_continueRun ==1){

              if(run_t.gFan_counter < 60){
          
                      Fan_RunSpeed_Fun();// FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }

	 }


	break;



   }
	
}
/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
    static uint8_t self_power_on_flag=0,send_power_off_flag=0;
    

	if(self_power_on_flag==0){
        self_power_on_flag ++ ;
		run_t.power_on_send_bejing_times=0;
        Buzzer_KeySound();

    
		InitWifiModule_Hardware();//InitWifiModule();
		HAL_Delay(1000);
        SmartPhone_TryToLink_TencentCloud();
 
		if(esp8266data.esp8266_login_cloud_success==1){
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
			esp8266data.gTimer_subscription_timing=0;
			SendWifiData_To_Cmd(0x01) ;
            HAL_Delay(50);
			
		}
   
    }
    
    
   if(esp8266data.esp8266_login_cloud_success==1 && run_t.gPower_On  !=POWER_ON ){
       
           if(send_power_off_flag==0){
            send_power_off_flag++;
		    run_t.RunCommand_Label=POWER_OFF;
			//wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
			esp8266data.gTimer_subscription_timing=0;
			SendWifiData_To_Cmd(0x01) ;
			HAL_Delay(50);
               
           }
   			
	}
   
   
   



}



    
