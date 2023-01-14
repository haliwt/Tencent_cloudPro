#include "buzzer.h"
#include "delay.h"


void Buzzer_KeySound(void)
{

  unsigned int m=200;//80
	while(m--){

    HAL_GPIO_TogglePin(BUZZER_GPIO,BUZZER_PIN);//BEEP=!BEEP;
    //delay_us(600);
    delay_us(500);//460//__delay_us(300);;//__delay_us(800);//delayUS(300);
   
    }
       
}



