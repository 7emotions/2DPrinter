 
 
 
#include "stepper.h"
/*
引脚连接：
        PB5 - DIR                方向引脚
        PB8 - STEP TIM4_CH3        步进引脚
        PB3 - ENABLE            使能引脚
*/
uint16_t  motor_sleep;
uint8_t motor_dir=1;
void MOTOR_Init(void)
{
    //1.引脚初始化
    GPIO_InitTypeDef motor_gpio_init;
    motor_gpio_init.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_3;
    motor_gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    motor_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&motor_gpio_init);    //DIR ENABLE 通用推挽输出模式
    motor_gpio_init.GPIO_Pin = GPIO_Pin_8;
    motor_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&motor_gpio_init);    //STEP 复用推挽输出
    
    //2.定时器初始化配置
    TIM_DeInit(TIM4);
    TIM_TimeBaseInitTypeDef motor_TimeInit;
    motor_TimeInit.TIM_Prescaler = 31;                    //预分频值
    motor_TimeInit.TIM_ClockDivision = TIM_CKD_DIV1;
    motor_TimeInit.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
    motor_TimeInit.TIM_Period = 3999;                    //重装载值
    motor_TimeInit.TIM_RepetitionCounter = 0;            //重复计数值
    TIM_TimeBaseInit(TIM4,&motor_TimeInit);                //基本计数模式
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);            //配置更新中断
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);                //清除更新中断位
    
    TIM_OCInitTypeDef motor_OCInit;
    TIM_OCStructInit(&motor_OCInit);
    motor_OCInit.TIM_OCMode = TIM_OCMode_PWM1;
    motor_OCInit.TIM_OutputState = TIM_OutputState_Enable;
    motor_OCInit.TIM_Pulse = 1499;
    TIM_OC3Init(TIM4,&motor_OCInit);                    //配置通道3PWM
    //失能motor 
    MOTOR_STOP();
    MOTOR_ENABLE;
    MOTOR_DIR_CW;                                        //顺时针
    //MOTOR_DIR_CCW;
}
 
//中断服务函数
void TIM4_IRQHandler(void)
{
    if(RESET != TIM_GetITStatus(TIM4,TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);        //清除中断标志位
        motor_sleep++;
        if(motor_sleep==3200)    //改变转向
        {
            MOTOR_STOP();        //停止运转
            motor_sleep=0;
            motor_dir=!motor_dir;
            if(motor_dir)        //顺时针
            {
                MOTOR_DIR_CW;
                MOTOR_START();
            }
            else
            {
                MOTOR_DIR_CCW;
                MOTOR_START();
            }
        }
    }
}
/*
    \brief：    设置motor运转速度
    \param：    speed: 速度
                速度=(1/2M) * speed * (1.8/16)  度/秒
    \retval：    none
*/
void MOTOR_Set_Speed(uint16_t speed)
{
    TIM_SetAutoreload(TIM4,speed-1);
    TIM_SetCompare3(TIM4,speed/2-1);
}
 
//设置步进模式 （用于MS1、MS2、MS3与单片机IO连接时使用）
#if 0
void setMotorStepMod(uint8_t stepMod)
{
    switch(StepMode)
        {
            case MOTOR_FULL_STEP:
                                MOTOR_MS1(0);
                                MOTOR_MS2(0);
                                MOTOR_MS3(0);
                break;
            case MOTOR_HALF_STEP:
                                MOTOR_MS1(1);
                                MOTOR_MS2(0);
                                MOTOR_MS3(0);
                break;
            case MOTOR_QUARTER_STEP:
                                MOTOR_MS1(0);
                                MOTOR_MS2(1);
                                MOTOR_MS3(0);
                break;
            case MOTOR_EIGHTH_STEP:
                                MOTOR_MS1(1);
                                MOTOR_MS2(1);
                                MOTOR_MS3(0);
                break;
            case MOTOR_SIXTEENTH_STEP:
                                MOTOR_MS1(1);
                                MOTOR_MS2(1);
                                MOTOR_MS3(1);
                break;
            default :
                                MOTOR_MS1(1);
                                MOTOR_MS2(1);
                                MOTOR_MS3(1);
                break;
        }
}
#endif