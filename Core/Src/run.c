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

		      WIFI_IC_ENABLE();
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
                run_t.gModel =2; //turn off
                Buzzer_KeySound();
            }
            else if(cmdType_2==0x04){
                run_t.gModel =1;  //turn on
                Buzzer_KeySound();
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
			   
         }
	  

	  break;

	  case 'T':
		  if(run_t.gPower_flag==POWER_ON){
             #if 1
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
		 Update_DHT11_Value();
		 HAL_Delay(200);
		 if(esp8266data.esp8266_login_cloud_success==1){
			 MqttData_Publish_SetOpen(0x01);
	         HAL_Delay(200);
	         Publish_Data_ToTencent_Initial_Data();
		 }
		 
	// cmd=0xff;  
     break;


	

    case 0x00: //power off
        Buzzer_KeySound();
        run_t.gPower_On=POWER_OFF;
        run_t.gPower_flag = POWER_OFF;
        run_t.RunCommand_Label = POWER_OFF;
         if(esp8266data.esp8266_login_cloud_success==1)  
         	MqttData_Publish_SetOpen(0x0);
           

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
		if(esp8266data.esp8266_login_cloud_success==1)
		 MqttData_Publish_SetPtc(0x01);
		 if(run_t.noBuzzer_sound_dry_flag !=1){
		     Buzzer_KeySound();
		 }
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			run_t.gDry = 0;
			 if(run_t.gPlasma ==0){ //plasma turn off flag
			  run_t.gFan_counter =0;
			   run_t.gFan_continueRun =1;

		     }
			if(esp8266data.esp8266_login_cloud_success==1)
			MqttData_Publish_SetPtc(0x0);
			if( no_buzzer_sound_dry_off !=1)
			    Buzzer_KeySound();
       break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
       		run_t.gUlransonic =1;
	   if(esp8266data.esp8266_login_cloud_success==1){
	        MqttData_Publish_SetPlasma(1) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(1); //超声波
	   	}
	    Buzzer_KeySound();
       break;

       case PLASMA_OFF:
           run_t.gPlasma=0;
           run_t.gUlransonic =0;
	   if(esp8266data.esp8266_login_cloud_success==1){
	       MqttData_Publish_SetPlasma(0) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(0); //超声波
	   	}
	    Buzzer_KeySound();
       break;

       case FAN_ON:
          run_t.set_wind_speed_value=100;
		  if(esp8266data.esp8266_login_cloud_success==1)
		  MqttData_Publis_SetFan(100);
		   Buzzer_KeySound();
       break;

       case FAN_OFF:
           run_t.set_wind_speed_value = 50;
		   if(esp8266data.esp8266_login_cloud_success==1)
		       MqttData_Publis_SetFan(50);
		    Buzzer_KeySound();
       break;

	   case WIFI_CONNECT_FAIL:

	       run_t.dp_link_wifi_fail =1;


	   break;

	   case DISPLAY_PANNEL_CONNECT_WIFI_SUCCESS:

	      
	        run_t.dp_link_wifi_fail =0;


	   break;

	    case IWDG_RECEIVE_DATA:

	     run_t.process_run_guarantee_flag=1;

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

   static uint8_t power_just_on;
    
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
		run_t.gTheFirst_powerOn=1;
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

	break;
        
    case POWER_OFF: //2
		SetPowerOff_ForDoing();
		if(esp8266data.esp8266_login_cloud_success==1){
	 	     tencent_cloud_flag = 1;
		}
		if(run_t.gTheFirst_powerOn ==0)
         	run_t.gFan_continueRun =0;
		else{
		 run_t.gFan_continueRun =1;

		}
         run_t.gFan_counter=0;
		
        
	   run_t.gPower_flag =POWER_OFF;
	   if(run_t.iwdg_the_first_falg==1){
		    run_t.RunCommand_Label =POWER_REF;
       }
        else
            run_t.RunCommand_Label=0xff;
		
      
	break;

   case UPDATE_TO_PANEL_DATA: //4
     if(run_t.gTimer_senddata_panel >30 && run_t.gPower_On==POWER_ON){ //300ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();
	      

	 }
	 if(esp8266data.esp8266_login_cloud_success==1){
	 	   if(run_t.gTimer_send_login_sucess > 11){
	 	        SendWifiData_To_Cmd(0x01) ;
	 	   	}
	   }

      if(run_t.gTimer_send_dit > 49){

          run_t.gTimer_send_dit=0;
		  Update_DHT11_Value();



	  }
	
    
    break;



	


    }
	
    if((run_t.gTimer_1s>30 && run_t.gPower_flag == POWER_ON)||power_just_on < 10){
    	power_just_on ++ ;
		run_t.gTimer_1s=0;
		Update_DHT11_Value();

     }

	

	if(run_t.gFan_continueRun ==1 && run_t.gPower_flag == POWER_OFF){
          
                if(run_t.gFan_counter < 60){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }
	  }

	 if(run_t.gPower_flag ==POWER_ON && run_t.gFan_continueRun ==1){

              if(run_t.gFan_counter < 60){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }

	 }

	 if(esp8266data.esp8266_login_cloud_success==1 && run_t.dp_link_wifi_fail ==1){
	 	     run_t.dp_link_wifi_fail=0;
	 	    SendWifiData_To_Cmd(0x01) ;
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
    static uint8_t self_power_on_flag=0;
    

	if(self_power_on_flag==0){
        self_power_on_flag ++ ;
        Buzzer_KeySound();
	    run_t.iwdg_feed_success_flag =1;
		run_t.flash_read_data =Flash_Read_Data();
		switch(run_t.flash_read_data){

	     case error: //wifi don't link to tencent cloud ,need manual operation
		      wifi_t.runCommand_order_lable = 0xff;
		      run_t.flash_write_data_flag = 0;
		 break;

		 case success: //wifi has been linked to tencent cloud,need auto link to tencent cloud
		 	//wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
			run_t.flash_write_data_flag = 1;
		  
         break;



		}
	

	switch(run_t.flash_write_data_flag){

      case 0:

      break;

      case 1:
        WIFI_IC_ENABLE();
      	//InitWifiModule();
		//Wifi_SoftAP_Config_Handler();
		//PowerOn_Self_Auto_Link_Tencent_Cloud();
		  InitWifiModule_Hardware();//InitWifiModule();
		
        SmartPhone_TryToLink_TencentCloud();
 
		if(esp8266data.esp8266_login_cloud_success==1){
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
			esp8266data.gTimer_subscription_timing=0;
			 SendWifiData_To_Cmd(0x01) ;
			Subscriber_Data_FromCloud_Handler();
		}
        else wifi_t.runCommand_order_lable =0xff;


      break;


	}
   }
    
   if(esp8266data.esp8266_login_cloud_success==1 &&  self_power_on_flag==1){
        self_power_on_flag++;
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
		esp8266data.gTimer_subscription_timing=0;
			 SendWifiData_To_Cmd(0x01) ;
   			Subscriber_Data_FromCloud_Handler();
	}



}



    
