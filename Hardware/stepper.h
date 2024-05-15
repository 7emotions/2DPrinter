 
#ifndef _MOTOR_H_
#define _MOTOR_H_
 
#include "stm32f10x.h"
 
#define MOTOR_DIR_CW GPIO_ResetBits(GPIOB,GPIO_Pin_5)        //顺时针
#define MOTOR_DIR_CCW GPIO_SetBits(GPIOB,GPIO_Pin_5)        //逆时针
#define MOTOR_ENABLE GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define MOTOR_STOP() TIM_Cmd(TIM4,DISABLE)                    //停止转动
#define MOTOR_START() TIM_Cmd(TIM4,ENABLE)                    //开始转动
 
#define MOTOR_FULL_STEP             0 //满步
#define MOTOR_HALF_STEP             1 //二分之一步
#define MOTOR_QUARTER_STEP        2      //四分之一步
#define MOTOR_EIGHTH_STEP            3 //八分之一步
#define MOTOR_SIXTEENTH_STEP    4      //十六分之一步 
 
extern uint8_t motor_dir;
 
void MOTOR_Init(void);
void MOTOR_Set_Speed(uint16_t speed);
 
#endif