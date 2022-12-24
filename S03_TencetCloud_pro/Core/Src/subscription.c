#include "subscription.h"
#include <string.h>
#include <stdlib.h>
#include "cmd_link.h"
#include "run.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "usart.h"

uint8_t TCMQTTRCVPUB[256];
uint8_t Sub_Topic[128];
uint8_t Sub_Data[128];

char *pub_buf;


void Parser_Cloud_ObjectName(uint8_t name_len);

//static Subscription1 subscription;
static void SubscriptionDispatch(Subscription1 *me, unsigned const sig);
static void Parse_Rx_Cloud_Data(void);

//void Subscription_Handler(void)
//{
//	uint8_t  sig;
//	switch (run_t.wifi_subscription_lable){
//		case OPEN_SIG: 
//			sig = RUN;
//	        break;
//		case STATE_SIG : 
//			sig = STAR_SIG;  
//			break;
//		case PTC_SIG:
//			break;
//		case SONIC_SIG:
//			break;
//		case ANION_SIG:
//			break;
//		case TEMP_SIG:
//			break;
//		case FIND_SIG:
//			break;
//		case HUM_SIG:
//			break;
//		case NOWTEMP_SIG:
//			break;
//				
//	   default:  sig = CHAR_SIG;  break;
//	}
//	
//   SubscriptionDispatch(&subscription,  sig);
//}

//static void SubscriptionDispatch(Subscription1 *me, unsigned const sig) {
//   switch (me->substate__) {
//   case STANDBY:
//      switch (sig) {
//      case OPEN_SIG:
//	  	 
//         Subscription1Tran(me, START);         /* transition to "slash" */
//         break;
//      }
//   break;
//	  
//   case START:
//      switch (sig) {
//      case STATE_SIG:
//         me->commentCtr__ += 2;     /* SLASH-STAR count as comment */
//        Subscription1Tran(me, SLASH);     /* transition to "comment" */
//         break;
//      case CHAR_SIG:
//      case SLASH_SIG:
//         Subscription1Tran(me, SLASH);         /* go back to "code" */
//         break;
//      }
//   break;
//	  
//   case WORKS:
//      switch (sig) {
//      case STAR_SIG:
//         CParser1Tran(me, STAR);           /* transition to "star" */
//         break;
//      case CHAR_SIG:
//      case SLASH_SIG:
//         ++me->commentCtr__;             /* count the comment char */
//         break; 
//      }
//   break;
//	  
//   case RUN:
//      switch (sig) {
//      case STAR_SIG:
//         ++me->commentCtr__;              /* count STAR as comment */
//         break;
//      case SLASH_SIG:
//         me->commentCtr__ += 2;     /* count STAR-SLASH as comment */
//         CParser1Tran(me, CODE);           /* transition to "code" */
//         break;
//      case CHAR_SIG:
//         me->commentCtr__ += 2;         /* count STAR-? as comment */
//         CParser1Tran(me, COMMENT);        /* go back to "comment" */
//         break;
//      }
//   break;

//  case END:
//  	switch(sig){
//	case 1:

//  break;


//	}
//   }
//}
//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
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

      //subscription of topic of name
       if(esp8266data.rx_data_name_len==3 ||esp8266data.rx_data_name_len==4 || \
             esp8266data.rx_data_name_len==5 || esp8266data.rx_data_name_len==11 ){

           Parser_Cloud_ObjectName(esp8266data.rx_data_name_len);

          // Parser_Cloud_ObjectName(esp8266data.rx_data_len,esp8266data.rx_data_name_len);
        //  esp8266data.rx_data_success=0;
          //  esp8266data.getCloudValue_decade =*(p_cPos + 1);
        //  esp8266data.getCloudValue_unit =*(p_cPos + 2);

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
    switch(name_len){

     case 3: //"ptc"
       switch(num){
      
          case 0:
            if(TCMQTTRCVPUB[1]=='p')
                   num=1;
              
             break;

         case 1: //dry
                   esp8266data.getCloudValue_decade =TCMQTTRCVPUB[name_len+3];
             
            run_t.gDry= esp8266data.getCloudValue_decade;
          
            esp8266data.rx_data_success=0;
            num=0;
         break;

         

         default:
            num=0;

         break;

         }

    break;

    case 4: //"open" ,"find",
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
            else
            run_t.gFan = esp8266data.getCloudValue_decade;

         }
         esp8266data.rx_data_success=0;

         num=0;

         break;

         default:
         num=0;

         break;

         }
   
   break;

     case 5: // "sonic","state","Anion"
        switch(num){
          
              case 0:
                if(TCMQTTRCVPUB[1]=='A') //Anion-plasma
                  num=1;
               else if(TCMQTTRCVPUB[1]=='s') //state
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
   
   
   
     break;

    case 11: //"temperature"

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
   
   break;
        } 
}


 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler()
{
   uint8_t *device_massage;

    
   if(esp8266data.subscribe_flag ==1){
         device_massage = (uint8_t *)malloc(128);
     if(esp8266data.gTimer_subscription_timing > 5){
        esp8266data.gTimer_subscription_timing=0;
      

     sprintf((char *)device_massage, "AT+TCMQTTSUB=\"$thing/down/property/%s/%s\",0\r\n", PRODUCT_ID, DEVUICE_NAME);
     HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000);
    
         esp8266data.gTimer_tencent_down_1s =0;
         esp8266data.subscribe_rx_flag=1;
          esp8266data.subscribe_flag =0;
      esp8266data.subscribe_rxCloud_flag=1;
   }
      
     free(device_massage);
   }

   
   Parse_Rx_Cloud_Data();

}

static void Parse_Rx_Cloud_Data(void)
{
    

   if(  esp8266data.subscribe_rxCloud_flag==1){

    
        Receive_Data_FromCloud_Data(JSOBJECT,TCMQTTRCVPUB);
      
    
    }
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_IntHandler(void)
{
    switch(esp8266data.rx_data_state)
      {
      case 0:  //#0

            
         if(UART2_DATA.UART_DataBuf[0] == '"')  //hex :54 - "T" -fixed
            esp8266data.rx_data_state=1; //=1
          else{
               esp8266data.rx_counter=0;
            
            }
         break;

      case 1:
      
         if(UART2_DATA.UART_DataBuf[0] == 'p')  //hex :54 - "T" -fixed
            esp8266data.rx_data_state=2; //=1
          else{
               esp8266data.rx_counter=0;
            
            }
            
         break;
      case 2: //#1
             if(UART2_DATA.UART_DataBuf[0] == 'a')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=3; //=1
         else{
            esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
         break;
            
        case 3:
            if(UART2_DATA.UART_DataBuf[0] == 'r')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=4; //=1
         else{
           esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
        
        break;
        
        case 4:
            if(UART2_DATA.UART_DataBuf[0] == 'a')  //hex :4B - "K" -fixed
            esp8266data.rx_data_state=5; //=1
         else{
            esp8266data.rx_data_state =0;
             esp8266data.rx_counter=0;
         }
        
        break;

      case 5:
       if(UART2_DATA.UART_DataBuf[0] == 'm')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=6; //=1
         else{
           esp8266data.rx_data_state=0;
            esp8266data.rx_counter=0;
         }
            
      break;

      
      case 6:
       if(UART2_DATA.UART_DataBuf[0] == 's')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=7; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;

      case 7:
       if(UART2_DATA.UART_DataBuf[0] == '"')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=8; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;

        case 8:
       if(UART2_DATA.UART_DataBuf[0] == ':')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=9; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;


      case 9:
       if(UART2_DATA.UART_DataBuf[0] == '{')  //hex :4B - "K" -fixed
         esp8266data.rx_data_state=10; //=1
         else{
           esp8266data.rx_data_state =0;
            esp8266data.rx_counter=0;
         }
            
      break;

      case 10:
        
         
         UART2_DATA.UART_Data[esp8266data.rx_counter] = UART2_DATA.UART_DataBuf[0];
            esp8266data.rx_counter++ ;
                 
         if(UART2_DATA.UART_DataBuf[0]=='}') // 0x0A = "\n"
         {
            
             esp8266data.rx_data_success++;
            if(esp8266data.rx_data_success==1){
               esp8266data.rx_data_success++;
                strcpy(TCMQTTRCVPUB, UART2_DATA.UART_Data);
            }
            esp8266data.rx_data_state=0;
            esp8266data.rx_counter=0;

           break;
               
         }
         else{

            esp8266data.rx_data_state=10; //=1

         }
       
            
      break;



        default:
         
         
       break;
      }
 

}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
    
          strcpy((char *)esp8266data.data, (const char *)UART2_DATA.UART_Data);
          esp8266data.data_size = UART2_DATA.UART_Cnt;
    
            if(strstr((const char*)esp8266data.data,"+TCMQTTCONN:OK")){
              esp8266data.esp8266_login_cloud_success=1;
           esp8266data.subscribe_flag =1;
            esp8266data.rx_link_cloud_flag=0;
           
           }
         UART2_DATA.UART_Flag = 0;
           UART2_DATA.UART_Cnt=0;
         
        
            
}

