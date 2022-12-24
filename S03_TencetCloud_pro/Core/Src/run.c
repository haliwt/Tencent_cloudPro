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
 
      if(run_t.SingleMode ==1){
        Single_ReceiveCmd(run_t.Single_cmd);//Single_Usart_ReceiveData(ref);//Single_RunCmd(run_t.Single_cmd);
	     Single_RunCmd(run_t.Single_cmd);
	  
      

    }
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
          
		
		  run_t.gPower_On=0;
		  run_t.SingleMode = 0;
	
		   
      } 
       else if(cmdType_2 ==1){ //power on
       

	    
	        run_t.gPower_On=1;
		    run_t.SingleMode = 1;
         

	   }       
      
          
      break;
      
      case 'A': //AI function ->wifi ->indicate Fan
        
        if(run_t.gPower_On==1){
			
			
			if(run_t.SingleMode  ==1 ){
          
        	     Single_Usart_ReceiveData(cmdType_2);
                
            }
			
        }

      break;

	  case 'H': //remember setup time timing
	      if(run_t.gPower_On==1){
				 
			if(run_t.SingleMode  ==1 ){

			     if(cmdType_2 != 0xff){

				 	//mcu_dp_value_update(DPID_DISPTIME,cmdType_2); //VALUE型数据上报; --
			     }
				// else
                     
					// mcu_dp_value_update(DPID_DISPTIME,0); //VALUE型数据上报; --
			}
        }
	   break;

	  case 'I': // set up time timing how many ?
	  	  if(run_t.gPower_On==1){
				 
			if(run_t.SingleMode  ==1 ){
              //   wifi_t.getGreenTime = 1; //don't display GMT 
			   //  mcu_dp_value_update(DPID_SETTIME,cmdType_2); //VALUE型数据上报;
			     
			}
        }


	  break;

	  case 'T': //set up temperature
	  	if(run_t.gPower_On==1){
				 
			if(run_t.SingleMode  ==1 ){

			   // mcu_dp_value_update(DPID_SETTEMP,cmdType_2); //VALUE型数据上报;
			   MqttData_Publis_SetTemp(cmdType_2);
			
			     
			}
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
		  
			   wifi_t.wifi_sensor =1;
	
		 break;
	
		case 0x01://wifi key command turn on
			  wifi_t.wifi_sensor = 0;
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
          //  wifiUpdate_AI_Status(0);//wifi APP turn on
          //  wifiUpdate_Kill_Status(1); //update kill turn on   to smart phone APP
		//	wifiUpdate_Dry_Status(1);  //update dry turn on to smart phone APP
      
                
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
			 //  wifiUpdate_AI_Status(1);//wifi APP turn off
             //  wifiUpdate_Kill_Status(0); //update kill turn off   to smart phone APP
			 //  wifiUpdate_Dry_Status(0);  //update dry turn on off smart phone APP
			   
              

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
			  // wifiUpdate_AI_Status(0);//wifi APP turn off
              // wifiUpdate_Kill_Status(1); //update kill turn off   to smart phone APP
			  /// wifiUpdate_Dry_Status(1);  //update dry turn on off smart phone APP

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
    
    static uint8_t wifikey=0xff,retimes=0,time0=0,send_0xaa;
	uint8_t sendtemperature[4];

   if(run_t.gPower_On==0)times=0;
	
   if(run_t.gPower_On==1){

    if(run_t.sendtimes> 5 || retimes < 50) { // display humidity and temperature value
		run_t.sendtimes=0;

        retimes++;
     
       
		if(run_t.gPower_flag ==1){
			
            if(run_t.SingleMode ==1 ){// the don't be detected wifi signal
                Display_DHT11_Value(&DHT11);
            
             }
		
      }
    
      if( wifi_t.timer_wifi_send_cmd > 6){
		     wifi_t.timer_wifi_send_cmd =0;
       
      
            if(esp8266data.esp8266_login_cloud_success==1){ //has wifi 
		       
                sendtemperature[0]=wifi_t.setTimesValue;
				sendtemperature[2]=wifi_t.SetTemperatureValue;

				if(time0<2 && wifi_t.setTimesValue>0 ){

					if(sendtemperature[1] !=sendtemperature[0]){
						   
						  sendtemperature[1] =sendtemperature[0];
	                     if(run_t.SingleMode ==1){
						 // mcu_dp_value_update(DPID_SETTIME,wifi_t.setTimesValue); //VALUE型数据上报;
						//MqttData_Publis_SetTempFan(sendtemperature[1],0x32); //to tencent cloud data
						MqttData_Publis_SetTemp(sendtemperature[1]);
	                    SendWifiData_To_PanelTime(wifi_t.setTimesValue);//to displayPanle setTimesValue
						  run_t.gmt_time_flag  = 1;
						  
	                     }
					}
                }
                else{
                
                if(sendtemperature[2]> 20){
			    if(sendtemperature[3] !=sendtemperature[2]){
				      sendtemperature[3] =sendtemperature[2];
				      if(run_t.SingleMode ==1){
					  	  //mcu_dp_value_update(DPID_SETTEMP,wifi_t.SetTemperatureValue); //VALUE型数据上报;
					  	//MqttData_Publis_SetTempFan(sendtemperature[3],0x32); //to tencent cloud data
						MqttData_Publis_SetTemp(sendtemperature[1]);
						SendWifiData_To_PanelTemp(wifi_t.SetTemperatureValue);

				      }
		      
			    }
			    }
			   }
		   

		 }
        }

     #if 0
	  
      /*------------------GMT ------------------*/
      if(wifi_work_state == WIFI_CONN_CLOUD && run_t.gmt_time_flag == 0 ){
         if( wifi_t.getGreenTime !=0xff && wifi_t.getGreenTime !=0xFE ){
          wifi_t.getGreenTime =1;
           mcu_get_green_time();
	      
        }
		if(wifi_t.getGreenTime == 0xff && wifi_t.getGreenTime !=0xFE && wifi_t.getGreenTime !=0){
            
            run_t.sed_GMT_times = 1;
             wifi_t.getGreenTime =0xFE;
           // Decode_GMT(rx_wifi_data);
              
             wifi_t.real_hours = rx_wifi_data[4] + 8;
			if(wifi_t.real_hours > 24){
				wifi_t.real_hours = wifi_t.real_hours -24 ;

			}
			wifi_t.real_minutes = rx_wifi_data[5];
			wifi_t.real_seconds = rx_wifi_data[6];
         
            SendData_Real_GMT(wifi_t.real_hours ,wifi_t.real_minutes,rx_wifi_data[6]); //gmt[4]->hours, gmt[5]->minutes
		    
		}
        else{
              if(wifi_t.gTimer_gmt > 2){ //10 minute 
                wifi_t.gTimer_gmt = 0;    
                wifi_t.getGreenTime =0;
             }
            
        }
    }
  #endif 
    //检测WIFI 是否连接成功
   
	if(esp8266data.esp8266_login_cloud_success==0){//don't wifi 

	     if(wifikey != wifi_t.wifi_detect){
		 	 wifikey = wifi_t.wifi_detect;
		
		    //mcu_set_wifi_mode(0);//wifi be detector AP mode,slowly

	     }
            
		 if(wifi_t.gTimer_500ms ==0){ //to displayPanel wifi icon slowly blank ,don't wifi 
				
					  SendWifiData_To_Cmd(0x00);
			       
	                
		}
		else if(wifi_t.gTimer_500ms>0){
					 if(wifi_t.gTimer_500ms >1)wifi_t.gTimer_500ms=0;
				
					  Display_DHT11_Value(&DHT11);
					   
	                 
			}
       }
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
    
       
	
	
	 if(run_t.gFan_continueRun ==1 && (run_t.gPower_On ==1)){
          
                if(run_t.gFan_counter < 61){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }
	  }
	  if((run_t.gPower_On !=0 ) && run_t.gFan_continueRun ==0){

	      FAN_CCW_RUN();
      }
      
      
 }

   //Fan at power of function 
  if((run_t.gPower_On ==0 ) && run_t.gFan_continueRun ==1){ //Fan be stop flag :0 -Fan works 
        
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
}




    
