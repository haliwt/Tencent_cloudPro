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


RUN_T run_t; 

static void Single_ReceiveCmd(uint8_t cmd);


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

 uint8_t cmdType_1=inputCmd[0],cmdType_2 = inputCmd[1];
    
  switch(cmdType_1){
  
      case 'P': //power on and off
        
           Buzzer_KeySound();
           Single_ReceiveCmd(cmdType_2);  
           
           cmdType_2 =0xff;
           cmdType_1=0xff;
             
          
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_flag==POWER_ON){
	      if(cmdType_2==1){
              //fast blink led for link to tencent cloud
             // WIFI_IC_ENABLE();
              esp8266data.esp8266_login_cloud_success=0;
			  // wifi_link_tencent_cloud:
			  Buzzer_KeySound();	
		     // InitWifiModule_Hardware();
               esp8266data.esp8266_login_cloud_success=0;
               run_t.wifi_config_net_lable=wifi_set_restor;
              wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 
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
           
            cmdType_2 =0xff;
        }
         cmdType_1=0xff;
	   break;
        
      case 'C':
           if(run_t.gPower_flag==POWER_ON){
               Single_ReceiveCmd(cmdType_2); 
               cmdType_2 = 0xff; 
           }
         cmdType_1=0xff;
         
      break;

	  case 'T':
	  	if(run_t.gPower_flag==POWER_ON){
              
             run_t.set_temperature_value = cmdType_2;
			 if(esp8266data.esp8266_login_cloud_success==1)
			 MqttData_Publis_SetTemp(run_t.set_temperature_value);
			   
         }
	   cmdType_1=0xff;

	  break;


	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_flag==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
			    Buzzer_KeySound();
			}
			 
			 cmdType_2=0xff;
		}
       cmdType_1 =0xff;
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
static void Single_ReceiveCmd(uint8_t cmd)
{
  
    
    switch(cmd){

    case 0x00: //power off
        run_t.gPower_On=POWER_OFF;
        run_t.gPower_flag = POWER_OFF;
        run_t.RunCommand_Label = POWER_OFF;
         if(esp8266data.esp8266_login_cloud_success==1)  
         MqttData_Publish_SetOpen(0x0);
           

    cmd = 0xff;
    break;

    case 0x01: // power on
         run_t.gPower_flag = POWER_ON;
		 run_t.gPower_On = POWER_ON;
         run_t.RunCommand_Label= POWER_ON;
		 Update_DHT11_Value();
		 HAL_Delay(200);
		 if(esp8266data.esp8266_login_cloud_success==1){
			 MqttData_Publish_SetOpen(0x01);
	         HAL_Delay(200);
	         Publish_Data_ToCloud_Handler();
		 }
		 
	 cmd=0xff;  
     break;

     default:

     break;

    
    }

   if( run_t.gPower_flag == POWER_ON){
	switch(cmd){


	 //dry key
     case 0x12: //PTC turn on
    
         run_t.gDry = 1;
         run_t.gFan_continueRun =0;
	 if(esp8266data.esp8266_login_cloud_success==1)
		 MqttData_Publish_SetPtc(0x01);

	break;

     case 0x02: //PTC turn off
		//Buzzer_KeySound();
		run_t.gDry =0;
		//Dry_Function(0) ;//
        if(run_t.gPlasma ==0){ //plasma turn off flag
			run_t.gFan_counter =0;
			run_t.gFan_continueRun =1;

		}
		if(esp8266data.esp8266_login_cloud_success==1)
			MqttData_Publish_SetPtc(0x0);

     cmd=0xff; 
       
     break;

     default:
         
     break;


    }

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
    if(run_t.gPower_flag ==POWER_ON){
		run_t.gPower_flag=0xff;
		run_t.gPower_On=POWER_ON;
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
    }

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

   static uint8_t power_just_on ;
  
   switch(run_t.RunCommand_Label){

	case POWER_ON: //1
		SetPowerOn_ForDoing();
	    run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;
		power_just_on=0;
        run_t.gTimer_1s=0;
		run_t.gTheFirst_powerOn=1;
		Update_DHT11_Value();
		HAL_Delay(200);

		if(esp8266data.esp8266_login_cloud_success==1){
	 	     SendWifiData_To_Cmd(0x01) ;
		}

	break;

   case UPDATE_TO_PANEL_DATA: //4
     if(run_t.gTimer_senddata_panel >30 && run_t.gPower_On==POWER_ON){ //300ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();
	 }

    break;



	case POWER_OFF: //2
		SetPowerOff_ForDoing();
		if(run_t.gTheFirst_powerOn ==0)
         	run_t.gFan_continueRun =0;
		else{
		 run_t.gFan_continueRun =1;

		}
         run_t.gFan_counter=0;
        
	   run_t.gPower_flag =POWER_OFF;
		
       run_t.RunCommand_Label =0xff;
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

	 if(run_t.gPlasma==0 && run_t.gDry==0 && run_t.gPower_flag ==POWER_ON && run_t.gFan_continueRun ==1){

              if(run_t.gFan_counter < 60){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }

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
		run_t.flash_read_data =Flash_Read_Data();
		Decode_Function();
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
      	InitWifiModule();
		//Wifi_SoftAP_Config_Handler();
		PowerOn_Self_Auto_Link_Tencent_Cloud();
        SmartPhone_TryToLink_TencentCloud();
      
        HAL_Delay(100);
		if(esp8266data.esp8266_login_cloud_success==1){
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;//wifi_tencent_subscription_data;
			esp8266data.gTimer_subscription_timing=0;
		}
        else wifi_t.runCommand_order_lable =0xff;

      break;


	}
   }



}



    
