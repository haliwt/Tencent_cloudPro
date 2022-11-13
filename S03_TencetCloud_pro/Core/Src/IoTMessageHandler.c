#include "loTMSG.h"
#include <string.h>
#include <stdlib.h>
#include "cmd_link.h"
#include "run.h"
#include "gpio.h"
#include "json_parser.h"
#define LOT_Debug 0

uint8_t TCMQTTRCVPUB[256];
uint8_t Sub_Topic[128];
uint8_t Sub_Data[128];

char *pub_buf;


void Parser_Cloud_ObjectName(uint8_t name_len);


//处理腾讯云下发的数据
/*******************************************************************************
	**
	*Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
	*Function: receive data from tencent cloud
	*Input Ref: module , str ->data
	*Return Ref:NO
	*
********************************************************************************/
void Receive_Data_FromCloud_Data(int type, char *str)
{

  
	uint8_t   iNameLen = 0;
    char  *p_cName = 0, *p_cPos = str;

       esp8266data.rx_data_len=strlen(str);

	   if (type == JSOBJECT) {
        /* Get Key */
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        p_cName = ++p_cPos;
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        iNameLen = p_cPos - p_cName;
		esp8266data.rx_data_name_len=iNameLen;

        /* Get Value */
        p_cPos = strchr(p_cPos, ':');
		
		
       }

	   
       if(esp8266data.rx_data_name_len==3 ||esp8266data.rx_data_name_len==4 || \
	   	esp8266data.rx_data_name_len==5 || esp8266data.rx_data_name_len==11 ){

	    Parser_Cloud_ObjectName(esp8266data.rx_data_name_len);

	       // Parser_Cloud_ObjectName(esp8266data.rx_data_len,esp8266data.rx_data_name_len);
	     //	esp8266data.rx_data_success=0;
          //  esp8266data.getCloudValue_decade =*(p_cPos + 1);
		  ///  esp8266data.getCloudValue_unit =*(p_cPos + 2);

       	}
	
    

	   

}



/*******************************************************************************
	**
	*Function Name:void Parser_Cloud_ObjectName(void)
	*Function: dy
	*Input Ref: 
	*Return Ref:NO
	*
********************************************************************************/
void Parser_Cloud_ObjectName(uint8_t name_len)
{
    static uint8_t num,temp;
	uint8_t fan_hundred;

    if(name_len==3){

	   switch(num){
		
			 case 0:
				if(TCMQTTRCVPUB[1]=='p')
                   num=1;
              
				 break;

			case 1: //power on or off
                   esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3];
			    
            run_t.gDry= esp8266data.getCloudValue_decade;
          
				esp8266data.rx_data_success=0;
				num=0;
			break;

			

			default:
				num=0;

			break;

			}

         }




	if(name_len ==4){

	
        switch(num){
			 
				  case 0:
					 if(TCMQTTRCVPUB[1]=='o')
						num=1;
					else if(TCMQTTRCVPUB[1]=='f')
						num=2;
					   
					 break;
	  
				 case 1: //open
	                  esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3];
					 run_t.gPower_flag=esp8266data.getCloudValue_decade;
                
                      
                    
					 esp8266data.rx_data_success=0;
					 num=0;
				 break;
	  
				 case 2: //fan
                 
                 
					 esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3] -'30';
                     esp8266data.getCloudValue_unit =TCMQTTRCVPUB[name_len+4] -'30';
				     fan_hundred = TCMQTTRCVPUB[name_len+5] -'30';

					 if(fan_hundred ==0) run_t.gFan =100;
					 else{
					 
				      if((esp8266data.getCloudValue_decade >=0 && esp8266data.getCloudValue_decade <=9)
					  	   &&(esp8266data.getCloudValue_unit>=0 && esp8266data.getCloudValue_unit<=9)){

					 
                 
	                    temp = esp8266data.getCloudValue_decade*10;
	                 
	                    run_t.gFan=temp + esp8266data.getCloudValue_unit;
					

					


					 }
					 
					 	}
					esp8266data.rx_data_success=0;
                 
					num=0;
	  
				 break;
	  
				 default:
					 num=0;
	  
				 break;
	  
				 }
	
	
	
      }


	 if(name_len ==5){

	
        switch(num){
			 
				  case 0:
					 if(TCMQTTRCVPUB[1]=='A') //Anion
						num=1;
					else if(TCMQTTRCVPUB[1]=='s')
						num=2;
					   
					 break;
	  
				 case 1: //
	                 run_t.gPlasma= esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3];
					esp8266data.rx_data_success=0;
					num=0;
					
				 break;
	  
				 case 2:
				 	 if(TCMQTTRCVPUB[2]=='t') //state
						num=3;
					else if(TCMQTTRCVPUB[2]=='o')  //sonic
						num=4;
					else
						 num=0;
					
	  
				 break;

				 case 3:
                     esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3];
				 	 run_t.gModel=  esp8266data.getCloudValue_decade;
					 esp8266data.rx_data_success=0;
					 num=0;

				 break;

				 case 4:

				     
                     esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+4];
                     run_t.gUlransonic = esp8266data.getCloudValue_decade;
					esp8266data.rx_data_success=0;
					num=0;

				 break;
	  
				 default:
					 num=0;
	  
				 break;
	  
				 }
	
	
	
      }

	

	
      
	if(name_len==11){
	
		   switch(num){
			
				 case 0:
					if(TCMQTTRCVPUB[1]=='t')
					   num=1;
				  
					 break;
	
				case 1: //power on or off

				     esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3]-'30'; 
                     esp8266data.getCloudValue_unit =TCMQTTRCVPUB[name_len+4]-'30'; 

					 temp = esp8266data.getCloudValue_decade *10;
				    
	
					run_t.gTemperature= temp + esp8266data.getCloudValue_unit ;
					
					esp8266data.rx_data_success=0;
					num=0;
				break;
	
				
	
				default:
					num=0;
	
				break;
	
				}
	
			 }
	







		
    


}



void loTMessageHandler(void)
{
    char *pub_buf;
    unsigned int i = 0;

    if (TCMQTTRCVPUB != 0)
    {
        //分离出Topic
        pub_buf = strstr((const char *)TCMQTTRCVPUB, "$thing/down/property/EHQB1P53IH/");
        if (pub_buf != NULL)
        {
            while(*pub_buf != '}') //(*pub_buf != '\"')
            {
                Sub_Topic[i] = *pub_buf++;
                i++;
            }
            i = 0;
#if LOT_Debug
            printf("Topic=%s\r\n", Sub_Topic);
#endif
        }
        //分离出数据
        pub_buf = strstr((const char *)TCMQTTRCVPUB, "params");
        if (pub_buf != NULL)
        {
            while (*pub_buf != '}')
            {
                Sub_Data[i] = *pub_buf++;
                i++;
            }
            //去掉结尾的双引号
            Sub_Data[i - 2] = '\0';
#if LOT_Debug
            printf("Data=%s\r\n", Sub_Data);
#endif

            i = 0;
        }
    }

    memset(TCMQTTRCVPUB, 0, sizeof(TCMQTTRCVPUB));
}

void cJsonMessageHandler(uint8_t *cJsonDATA)
{
    if (cJsonDATA != NULL)
    {
        cJSON *_JsonRoot = cJSON_Parse((const char *)cJsonDATA);
        if (_JsonRoot == NULL)
            goto __cJSON_Delete;

        cJSON *cJSON_params = cJSON_GetObjectItem(_JsonRoot, "params");
        if (cJSON_params == NULL)
            goto __cJSON_Delete;

		cJSON *cJSON_anion = cJSON_GetObjectItem(cJSON_params, "anion"); //power

		cJSON *cJSON_open = cJSON_GetObjectItem(cJSON_params, "open"); //power
		cJSON *cJSON_sonic = cJSON_GetObjectItem(cJSON_params, "sonic"); //ultrasonic
		cJSON *cJSON_fan =  cJSON_GetObjectItem(cJSON_params,"find");//fan
#if LOT_Debug
        printf("led1=%d\r\n", cJSON_led1->valueint);

#endif

		if (cJSON_anion->valueint == 1)
			 run_t.gPlasma =1;
		else 
		  run_t.gPlasma=0;

		if (cJSON_open->valueint == 1)
           run_t.gPower_flag = 1;// HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        else
           run_t.gPower_flag =0;// HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

		if(cJSON_sonic->valueint==1) //ultrasonic
		  run_t.gUlransonic =1;
		else 
		  run_t.gUlransonic =0;

		if(cJSON_fan->valueint==1)
		 run_t.gFan =1;
		else
		  run_t.gFan=0;
		  
		   
		
    __cJSON_Delete:
        cJSON_Delete(_JsonRoot);
        memset(Sub_Data, 0, sizeof(Sub_Data));
    }
}

