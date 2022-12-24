#include "publish.h"
#include "esp8266.h"
#include "mqtt_iot.h"
#include "run.h"

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
          

		  if(esp8266data.gTimer_publish_timing > 20){
		  	
		  	esp8266data.gTimer_publish_timing=0;
			esp8266data.gTimer_subscription_timing=0;
            esp8266data.subscribe_flag =1; //allow subscribe enable

           MqttData_Publish_State();
	    
		  
		  
		}
	    if(esp8266data.gTimer_publish_timing >  10 && esp8266data.gTimer_publish_timing < 12){
             

		    MqttData_Publis_ReadTempHum(run_t.gDht11_temperature,run_t.gDht11_humidity  );

		}

		
	}
	

   // free(device_pubmassage);
}





