/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* chua biet ghi the nao----------------------------------------------------- */
#include "math.h"



/* Includes (khong biet dung tu nao cho ngau)------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

/*my lib-------------------------------------------------------------------------*/

/* low level drivers ------------------------------------------------------------ */
#include "i2c.h"
#include "timer.h"
#include "PWM.h"
#include "encoder.h"

/*component drivers --------------------------------------------------------------*/
#include "mpu6050.h"
#include "kalman_filter.h"


/*----------------------------------------------------------------------------------*/
#include "state_machine.h"

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
