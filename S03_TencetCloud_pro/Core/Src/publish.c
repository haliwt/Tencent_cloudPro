#include "publish.h"
#include "esp8266.h"
#include "mqtt_iot.h"
#include "run.h"
#include "dht11.h"

/*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Publish_Data_ToCloud_Handler(void)
{
    
   if(esp8266data.esp8266_login_cloud_success==1){
          
       switch(esp8266data.send_tencent_cloud_data_lable){

        case tencent_cloud_init:
			 MqttData_Publish_Init();
	         
			 esp8266data.send_tencent_cloud_data_lable=tencent_cloud_real_times;
	    break;

		case tencent_cloud_real_times:
		     Update_Dht11_Totencent_Value();
			
        break;


		
       }


	}
	

   // free(device_pubmassage);
}





