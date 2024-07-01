#include "stm32f4xx_hal.h"

/* For Motor axis */
#define BOARD_X_EN_PORT                 GPIOI
#define BOARD_X_EN_PIN                  GPIO_PIN_8
#define BOARD_Y_EN_PORT                 GPIOI           
#define BOARD_Y_EN_PIN                  GPIO_PIN_8
#define BOARD_Z_EN_PORT                 GPIOI          
#define BOARD_Z_EN_PIN                  GPIO_PIN_8
#define BOARD_X_STEP_PORT               GPIOA
#define BOARD_X_STEP_PIN                GPIO_PIN_4
#define BOARD_Y_STEP_PORT               GPIOA           
#define BOARD_Y_STEP_PIN                GPIO_PIN_6
#define BOARD_Z_STEP_PORT               GPIOA          
#define BOARD_Z_STEP_PIN                GPIO_PIN_8
#define BOARD_X_DIR_PORT                GPIOI
#define BOARD_X_DIR_PIN                 GPIO_PIN_5
#define BOARD_Y_DIR_PORT                GPIOI           
#define BOARD_Y_DIR_PIN                 GPIO_PIN_6
#define BOARD_Z_DIR_PORT                GPIOI          
#define BOARD_Z_DIR_PIN                 GPIO_PIN_7

#define BOARD_LIMIT_X_PORT              GPIOD
#define BOARD_LIMIT_X_PIN               GPIO_PIN_4
#define BOARD_LIMIT_Y_PORT              GPIOD
#define BOARD_LIMIT_Y_PIN               GPIO_PIN_11
#define BOARD_LIMIT_Z_PORT              GPIOD
#define BOARD_LIMIT_Z_PIN               GPIO_PIN_12
#define BOARD_LIMIT_IRQ                 EXTI0_IRQn
#define BOARD_LIMIT_IRQnHANDLER         EXTI0_IRQHandler

#define BOARD_SET_TIM                   3
#define BOARD_RESET_TIM                 4

#define BOARD_LASER_TIM_PORT            1
#define BOARD_LASER_TIM_CH              TIM_CHANNEL_2
#define BOARD_LASER_PIN_AF              GPIO_AF1_TIM1
#define BOARD_LASER_OUT_PORT            GPIOB
#define BOARD_LASER_OUT_PIN             GPIO_PIN_6



#ifndef STEP_EN_PORT
#define STEP_X_EN_PORT                  BOARD_X_EN_PORT      
#define STEP_X_EN_PIN                   BOARD_X_EN_PIN   
#define STEP_Y_EN_PORT                  BOARD_Y_EN_PORT      
#define STEP_Y_EN_PIN                   BOARD_Y_EN_PIN   
#ifdef BOARD_Z_EN_PORT
#define STEP_Z_EN_PORT                  BOARD_Z_EN_PORT    
#endif  // BOARD_Z_EN_PORT
#ifdef BOARD_Z_EN_PIN
#define STEP_Z_EN_PIN                   BOARD_Z_EN_PIN  
#endif  // BOARD_Z_EN_PIN
#endif


#define MOTOR_X_DIR_PORT            BOARD_X_DIR_PORT
#define MOTOR_X_DIR_PIN             BOARD_X_DIR_PIN 
#define MOTOR_Y_DIR_PORT            BOARD_Y_DIR_PORT
#define MOTOR_Y_DIR_PIN             BOARD_Y_DIR_PIN 
#ifdef BOARD_Z_DIR_PORT
#define MOTOR_Z_DIR_PORT            BOARD_Z_DIR_PORT
#endif 
#ifdef BOARD_Z_DIR_PIN
#define MOTOR_Z_DIR_PIN             BOARD_Z_DIR_PIN
#endif

#define MOTOR_X_AXIS_PORT           BOARD_X_STEP_PORT 
#define MOTOR_X_AXIS_PIN            BOARD_X_STEP_PIN  
#define MOTOR_Y_AXIS_PORT           BOARD_Y_STEP_PORT 
#define MOTOR_Y_AXIS_PIN            BOARD_Y_STEP_PIN  
#ifdef BOARD_Z_STEP_PORT
#define MOTOR_Z_AXIS_PORT            BOARD_Z_STEP_PORT 
#endif 
#ifdef BOARD_Z_STEP_PIN
#define MOTOR_Z_AXIS_PIN             BOARD_Z_STEP_PIN  
#endif 

#define LIMIT_X_PORT                BOARD_LIMIT_X_PORT // GPIOB
#define LIMIT_X_PIN                 BOARD_LIMIT_X_PIN  // GPIO_PIN_0
#define LIMIT_Y_PORT                BOARD_LIMIT_Y_PORT // GPIOB
#define LIMIT_Y_PIN                 BOARD_LIMIT_Y_PIN  // GPIO_PIN_1
#ifdef BOARD_LIMIT_Z_PORT
#define LIMIT_Z_PORT                BOARD_LIMIT_Z_PORT
#endif // BOARD_LIMIT_Z_PORT
#ifdef BOARD_LIMIT_Z_PIN
#define LIMIT_Z_PIN                 BOARD_LIMIT_Z_PIN
#endif // BOARD_LIMIT_Z_PIN

#define LIMIT_IRQnHANDLE            BOARD_LIMIT_IRQnHANDLER
#define LIMIT_IRQn                  BOARD_LIMIT_IRQ


/* Define TIMER Freq */
#define STEP_SET_TIM_FREQ           BOARD_SET_TIM_FREQ
#define STEP_RESET_TIM_FREQ         BOARD_RESET_TIM_FREQ

/* Define Timer Port */
#define _TIM(X)                     TIM##X
#define TIM(X)                      _TIM(X)

/* Define Timer IRQn */
#define _TIM_IRQn(X)                TIM##X##_IRQn
#define TIM_IRQn(X)                 _TIM_IRQn(X)
#define _TIM_IRQHander(X)           TIM##X##_IRQHandler
#define TIM_IRQHander(X)            _TIM_IRQHander(X)

/* Define Timer Rcc */
#define _TIM_RCC(X)                 __HAL_RCC_TIM##X##_CLK_ENABLE()
#define TIM_RCC(X)                   _TIM_RCC(X)

#define SETP_SET_TIM                TIM(BOARD_SET_TIM)                     
#define SETP_RESET_TIM              TIM(BOARD_RESET_TIM)                  
#define SET_TIM_IRQn                TIM_IRQn(BOARD_SET_TIM)            
#define RESET_TIM_IRQn              TIM_IRQn(BOARD_RESET_TIM)          
#define SET_TIM_CLK_ENABLED()       TIM_RCC(BOARD_SET_TIM)            
#define RESET_TIM_CLK_ENABLED()     TIM_RCC(BOARD_RESET_TIM)          
#define STEP_SET_HANDLER            TIM_IRQHander(BOARD_SET_TIM)        
#define STEP_RESET_HANDLER          TIM_IRQHander(BOARD_RESET_TIM)     
#define STEP_SET_TIMER              STEP_SET_TIM          
#define STEP_RESET_TIMER            STEP_RESET_TIM  


#define LASER_TIM_PORT              TIM(BOARD_LASER_TIM_PORT)            
#define LASER_TIM_CH                BOARD_LASER_TIM_CH            
#define LASER_PIN_AF                BOARD_LASER_PIN_AF            
#define LASER_OUT_PORT              BOARD_LASER_OUT_PORT          
#define LASER_OUT_PIN               BOARD_LASER_OUT_PIN           
#define LASER_OUT_CLK()             TIM_RCC(BOARD_LASER_TIM_PORT) 