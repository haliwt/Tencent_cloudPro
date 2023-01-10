#include "run.h"
#include "wifi_fun.h"

#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "special_power.h"

#include "esp8266.h"
#include "mqtt_iot.h"


 


//static CProcess1 cprocess;
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
	  } 
      else if(cmdType_2 ==1){ //power on
         
         run_t.gPower_flag = POWER_ON;
		 run_t.gPower_On = POWER_ON;
	     
      }       
      else if(cmdType_2==2){

        run_t.gPower_On=POWER_CONNECTOR_WIFI;

      }
          
      break;
      

	  case 'H': //remember setup time timing
	      if(run_t.gPower_On==1){
				 
			
        }
	   break;

	  case 'T': //set up temperature
	  	if(run_t.gPower_On==1){
		   // mcu_dp_value_update(DPID_SETTEMP,cmdType_2); //VALUE型数据上报;
			MqttData_Publis_SetTemp(cmdType_2);
		}

	  break;

	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_On==1){

		    if(cmdType_2== 'Z'){//turn off AI
			    Buzzer_On();
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
	*Functin Name: void RunCommand_Order(void)
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void RunCommand_Order(void)
{
    
   switch(run_t.gPower_On){

	case POWER_ON:
		SetPowerOn_ForDoing();
	    Update_DHT11_Value();
	  
		 wifi_t.runCommand_order_lable = wifi_has_benn_connected;
	    run_t.gPower_On = UPDATE_TO_PANEL_DATA;
     
	break;

	case POWER_CONNECTOR_WIFI:
		
		wifi_t.runCommand_order_lable = wifi_has_benn_connected;
		run_t.gPower_On = UPDATE_TO_PANEL_DATA;

	break;


	case UPDATE_TO_PANEL_DATA:
       if(run_t.gTimer_senddata_panel >40){
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();

       	}
    break;

	case POWER_OFF:
		SetPowerOff_ForDoing();
		//wifi_t.runCommand_order_lable = wifi_disconnect;
	   //esp8266data.esp8266_login_cloud_success=0;
	   //wifi_t.has_been_login_flag = 0;
	   run_t.gPower_flag =POWER_OFF;
	break;


	

	

   
    }
	
    
  
    if(run_t.gTimer_1s>20){
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



 




    
