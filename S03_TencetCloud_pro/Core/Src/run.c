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
uint8_t times;
 
uint8_t timeArray[7];




/**********************************************************************
*
*Functin Name: void Initial_Ref(void)
*Function : be check key of value 
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Initial_Ref(void)
{
  
  run_t.gPlasma=0;
  run_t.gDry =0;
  run_t.gAi = AIENABLE;
  run_t.gFan_counter=0;
 

}

void Single_RunCmd(uint8_t sig)
{
   Ai_Fun(sig);
}

void Single_Mode(void)
{
   uint8_t ref;
 
   
        Single_ReceiveCmd(run_t.Single_cmd);//Single_Usart_ReceiveData(ref);//Single_RunCmd(run_t.Single_cmd);
	     Single_RunCmd(run_t.Single_cmd);
	  
      

    
}


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
       run_t.gPower_On=POWER_ON;
	   
      }       
      else if(cmdType_2==2){

        run_t.gPower_On=POWER_CONNECTOR_WIFI;

      }
          
      break;
      
      case 'A': //AI function ->wifi ->indicate Fan
        
        if(run_t.gPower_On==1){
			
		  Single_ReceiveCmd(cmdType_2);
            
		}

      break;

	  case 'H': //remember setup time timing
	      if(run_t.gPower_On==1){
				 
			
        }
	   break;

	  case 'I': // set up time timing how many ?
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
void Single_ReceiveCmd(uint8_t cmd)
{
   
	switch(cmd){
	
		 case 0x11: //wifi key command turn off
		  
			  
	
		 break;
	
		case 0x01://wifi key command turn on
			 
		break;
	
		//AI key
		case 0x08: //AI key command turn on
		
			 run_t.Single_cmd = 0x08;
			 run_t.globe_setPriority =1;
		   
	
		break;
	
		case 0x18: //AI turn off
		  
			   run_t.Single_cmd = 0x18;
			   run_t.globe_setPriority =1;
		 
		 break;
	
		//dry key
		case 0x02:
		   
			 run_t.Single_cmd = 0x02;
			 run_t.globe_setPriority =1;
		   
		break;
	
		case 0x12:
		  
			 run_t.Single_cmd = 0x12;
			 run_t.globe_setPriority =1;
		  
		break;
	
		//kill key
	
		case 0x04:
		  
			 run_t.Single_cmd = 0x04;
			 run_t.globe_setPriority =1;
		
		break;
	
		case 0x14:
		   
			 run_t.Single_cmd = 0x14;
			 run_t.globe_setPriority =1;
		 
		break;
	
		case 0x88:
				 run_t.Single_cmd = 0x88; //turn on plasma and dry ->set up temperature value
				 run_t.globe_setPriority =1;
	
	
		break;
	
		case 0x87:
				run_t.Single_cmd = 0x87;  //tunr off plasma and dry machine ->set up temperature value 
				run_t.globe_setPriority =1;
		break;
	
		default:
			
			run_t.Single_cmd = 0;
		break;

}

}
/**********************************************************************
	*
	*Functin Name: void RunCommand_Mode(unit8_t sig)
	*Function : wifi functio AI 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void AI_Function(uint8_t sig)
{
   static uint8_t ai_off=0xff,ai_on=0xff, settemp=0xff,settemp_off= 0xff;
   static uint8_t dry_on =0xff, dry_off = 0xff,ster_on=0xff,ster_off=0xff;

	switch(sig){
   

     //wifi function   
     case 0x04: //kill turn on
         if(run_t.globe_sub_flag != kill){
       
         if(ster_on !=run_t.kill_key && (run_t.gAi ==AIDISABLE )){
	   	    ster_on = run_t.kill_key;
           
            

			  run_t.globe_sub_flag = kill;
              run_t.kill_key_off++;
			   
			   run_t.dry_key++;
			   run_t.dry_key_off++;
			   	
			   run_t.ai_key++;
			   run_t.ai_key_off++;

			   run_t.wifi_key_off++;
			   run_t.wifi_key++;
			  
			  Buzzer_On();
	   
		
	       run_t.gPlasma =0;
	
		   run_t.gFan_continueRun =0;
		   
		 
	       SterIlization(0); //turn on
	       //SendWifiCmd_To_Order(0x04);
		   MqttData_Publish_State();
		   
            }
	   
        }
     break;
         
    case 0x14: //kill turn off
        
          if(run_t.globe_sub_flag != notkill){
          if((ster_off !=run_t.kill_key_off && (run_t.gAi == AIDISABLE ))){
               ster_off = run_t.kill_key_off;
		
			  run_t.globe_sub_flag = notkill;
			   
			   run_t.kill_key++;
			  
			   
			   run_t.dry_key++;
			   run_t.dry_key_off++;
			  
			   	
			   run_t.ai_key++;
			   run_t.ai_key_off++;
			   
			   run_t.wifi_key_off++;
			   run_t.wifi_key++;
			   
	    	 
			  
			     Buzzer_On();
       
            run_t.gPlasma =1; //turn off plasma 
			//wifiUpdate_Kill_Status(0);
		
           SterIlization(1); //turn off kill function
			 if( run_t.gDry ==1){
			 
				  run_t.gFan_counter =0;
				 run_t.gFan_continueRun =1;
			 }
    
               SendWifiCmd_To_Order(0x14);
            }
        }
    
    break;


    case 0x02: //dry turn 0n
        if(run_t.globe_sub_flag != dry){
        if((dry_on != run_t.dry_key && (run_t.gAi == AIDISABLE ))){
			      dry_on = run_t.dry_key;

			  
				run_t.globe_sub_flag = dry;

		            run_t.dry_key_off++;
			
				   run_t.kill_key++;
				   run_t.kill_key_off++;
				   
				   run_t.ai_key++;
				   run_t.ai_key_off++;

				   run_t.wifi_key_off++;
			       run_t.wifi_key++;

             	
			   Buzzer_On();
			  
            
             run_t.gDry = 0;
          
	
			 run_t.gFan_continueRun =0;
			// wifiUpdate_Dry_Status(1);
			 Dry_Function(0);

			  SendWifiCmd_To_Order(0x02);
			 
         }
             
        }
    break;
         
    case 0x12 : //dry turn off
          if(run_t.globe_sub_flag !=notdry){
          if((dry_off != run_t.dry_key_off && (run_t.gAi ==AIDISABLE ))){
			  dry_off = run_t.dry_key_off;
			  

			  run_t.globe_sub_flag = notdry;
			        run_t.dry_key++;
			  
		           run_t.kill_key++;
				   run_t.kill_key_off++;
				   
				   run_t.ai_key++;
				   run_t.ai_key_off++;

				   run_t.wifi_key_off++;
			       run_t.wifi_key++;
			   

			    Buzzer_On();
			  
        
             run_t.gDry =1;
		 	
			//wifiUpdate_Dry_Status(0);
		    Dry_Function(1) ;//Display_Function_OnOff();
		    
             if(run_t.gPlasma ==1){ //plasma turn off flag

                
				 run_t.gFan_counter =0;
				 run_t.gFan_continueRun =1;

             }
		     SendWifiCmd_To_Order(0x12);
           }
          }
    break;

	case 0x08: //wifi AI and Single AI-> AI tunr ON

	    if(ai_on != run_t.ai_key){
		      ai_on = run_t.ai_key;
		      
		       run_t.gAi = AIENABLE;
              // wifi_t.wifi_itemAi=AIENABLE;
            
			   run_t.ai_key_off++;

		       run_t.kill_key++;
			   run_t.kill_key_off++;
			   
			   run_t.dry_key++;
			   run_t.dry_key_off++;

			   run_t.wifi_key_off++;
			   run_t.wifi_key++;
			  
             //  wifi_t.wifi_itemAi=0;
			 
				 
		
		   Buzzer_On();
	      
		   run_t.gFan_continueRun =0;
         
            run_t.gPlasma =0;
            run_t.gDry =0;
  
                
                FAN_CCW_RUN();
                PLASMA_SetHigh(); //
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
                PTC_SetHigh();
			    SendWifiCmd_To_Order(0x08);
             
				
		}
		
	  break;

	 case 0x18: //wifi AI and Signal AI turn off
	      if(ai_off != run_t.ai_key_off){
		      ai_off = run_t.ai_key_off;
		            run_t.ai_key++;

		           run_t.gAi = AIDISABLE;
                 //  wifi_t.wifi_itemAi=AIDISABLE;
              
		           run_t.kill_key++;
				   run_t.kill_key_off++;
				   run_t.dry_key++;
				   run_t.dry_key_off++;

				   run_t.wifi_key_off++;
				   run_t.wifi_key++;

				  Buzzer_On();
				// wifiUpdate_AI_Status(1);
				 SendWifiCmd_To_Order(0x18);

		  }

	 break;

	 case 0x87: //set up temperature  value auto shut off plasma and dry machine 
	       if(settemp != run_t.set_temperature_off){
		   	   settemp = run_t.set_temperature_off;
                 run_t.set_temperature_on++;
			   
			   run_t.gFan_continueRun =1;
			   run_t.gFan_counter = 0;

               Buzzer_On(); 
		
			   
              

			   PLASMA_SetLow(); //
			   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
			   PTC_SetLow();
			   FAN_Stop();
			  

			   
			   
	     }
	 break;

	 case 0x88: // setu up temperature value auto turn on plasma and dry machine 
          if(settemp_off != run_t.set_temperature_on){
		   	   settemp_off = run_t.set_temperature_on;
                 run_t.set_temperature_off++;

				   run_t.gFan_continueRun =0;

			    Buzzer_On(); 
		

				FAN_CCW_RUN();
                PLASMA_SetHigh(); //
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
                PTC_SetHigh();
			  
	 
          }
	 break;
           
   

	default:
     sig = 0;

	break;
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
	    Update_DHT11_Value(&DHT11);
	     wifi_t.wifi_connector_tencent_cloud=1;
	    run_t.gPower_On = 0x0A;
     
	break;

	case POWER_CONNECTOR_WIFI:
		wifi_t.wifi_connector_tencent_cloud=1;
		run_t.gPower_On = 0x0A;

	break;

	case POWER_OFF:
		SetPowerOff_ForDoing();
		
	break;

	

   
    }
	
    
  
    if(run_t.gTimer_1s>10){
		run_t.gTimer_1s=0;
				
		Update_DHT11_Value(&DHT11);
					   
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

#if 0
	   //has wifi 
		if((esp8266data.esp8266_login_cloud_success==1)   && (run_t.gTimer_send_0xaa > 10 || send_0xaa < 4 )){
		   run_t.gTimer_send_0xaa=0;
		   wifi_t.wifi_detect++;
		   send_0xaa++;
		   
			SendWifiData_To_Cmd(0xaa); 
			if(send_0xaa > 50){
			  send_0xaa =0 ;
			//wifiDisplayTemperature_Humidity();//to tencent cloud data
			 MqttData_Publis_ReadTempHum(run_t.gDht11_temperature,run_t.gDht11_humidity);
		   
		   }
		   
		}
	   
		  


   //Fan at power of function 
  if((run_t.gPower_flag == 0 ) && run_t.gFan_continueRun ==1){ //Fan be stop flag :0 -Fan works 
        
         if(run_t.gFan_counter < 61){
         
             FAN_CCW_RUN();
         }        
       
        if(run_t.gFan_counter >= 60){ //60s
        
	     run_t.gFan_counter=0;
	  
         run_t.gFan_continueRun++;
		  FAN_Stop();
	   }
   }

}
  #endif 





    
