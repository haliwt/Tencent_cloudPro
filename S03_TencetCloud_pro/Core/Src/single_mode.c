#include "single_mode.h"
#include "wifi_fun.h"
#include "run.h"
#include "special_power.h"

void Single_RunCmd(uint8_t sig)
{
  // Ai_Fun(sig);
}


void Single_Mode(void)
{
   uint8_t ref;
   if(run_t.globe_setPriority==1){
      if(run_t.SingleMode ==1){
        Single_Usart_ReceiveData(ref);//Single_RunCmd(run_t.Single_cmd);
	     Single_RunCmd(run_t.Single_cmd);
	  
      }

}
}
