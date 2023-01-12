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


RUN_T run_t; 

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
        
        
       if(cmdType_2 == 0x00){ //power off
          
	       run_t.gPower_On=POWER_OFF;
            run_t.gPower_flag = POWER_OFF;
	  } 
      else if(cmdType_2 ==1){ //power on
         
         run_t.gPower_flag = POWER_ON;
		 run_t.gPower_On = POWER_ON;
	     
      }       
          
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_flag==POWER_ON){
	      if(cmdType_2==1){
			  wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;	 
		   }
		   else if(cmdType_2==0){
               	wifi_t.runCommand_order_lable= wifi_has_been_connected;
		   }
		   else if(cmdType_2==0x14){
                run_t.gModel =2; //turn off
            }
            else if(cmdType_2==0x04){
                run_t.gModel =1;  //turn on
            }
        }
	   break;


	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_flag==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
			    Buzzer_KeySound();
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
   static uint8_t stop_fan_flag;
   switch(run_t.gPower_On){

	case POWER_ON:
		SetPowerOn_ForDoing();
	    Update_DHT11_Value();
	    run_t.gPower_On = UPDATE_TO_PANEL_DATA;
        run_t.gTimer_1s=0;
		if(esp8266data.esp8266_login_cloud_success==1){
	 	     SendWifiData_To_Cmd(0x01) ;
		}
		stop_fan_flag=0;
	break;

   case UPDATE_TO_PANEL_DATA:
       if(run_t.gTimer_senddata_panel >40){ //400ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();

     }
    break;

	case POWER_OFF:
		SetPowerOff_ForDoing();
        if(run_t.gFan_continueRun==1 && stop_fan_flag==0){
            stop_fan_flag++;
            run_t.gFan_counter=0;
        }
	   run_t.gPower_flag =POWER_OFF;
       run_t.gPower_On=0xff;
	break;

    }
	
    if(run_t.gTimer_1s>7 ){
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
	  if((run_t.gPower_On !=POWER_OFF ) && run_t.gFan_continueRun ==0){

	      FAN_CCW_RUN();
      }

      
      
 }



 




    
