#ifndef __PUBLISH_H_
#define __PUBLISH_H_
#include "main.h"

enum pub_Signal {                   /* enumeration for CParser signals */
  tencent_cloud_init,
  tencent_cloud_real_times,
};





void Publish_Data_ToTencent_Initial_Data(void);
void Publish_Data_ToTencent_Update_Data(void);


#endif 
