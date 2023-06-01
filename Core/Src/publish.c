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
void Publish_Data_ToTencent_Initial_Data(void)
{
    if(esp8266data.esp8266_login_cloud_success==1){
          
       MqttData_Publish_Init();
	         
	}

}
	

void Publish_Data_ToTencent_Update_Data(void)
{
      MqttData_Publish_Update_Data();
  

}





