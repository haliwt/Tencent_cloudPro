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


//处理腾讯云下发的数据

void Receive_Data_FromCloud_Data(int type, char *str)
{

  
	uint8_t     iNameLen = 0, iValueLen = 0,iGetValue;
    char   *p_cName = 0, *p_cValue = 0,  *p_cPos = str;

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

        /* Get Value */
        p_cPos = strchr(p_cPos, ':');
		
		
       }

	   
       if(iNameLen>2){
	     	esp8266data.rx_data_success=0;
            esp8266data.getCloudValue_decade =*(p_cPos + 1);
		    esp8266data.getCloudValue_unit =*(p_cPos + 2);

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

