#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor.h"
#include "pid.h"

void SystemClock_Config(void);
volatile int32_t encA;
volatile int32_t encB;

pid_type pidA;
pid_type pidB;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM9_Init();
  MX_I2C1_Init();
	MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  //MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1|TIM_CHANNEL_2);
	/* Do not use OR bitwise operator - TIM_CHANNEL_1|TIM_CHANNEL_2 */
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	/*_____________ Init value _____________*/
	pidA.setPoint = 8000;
	pidA.kP = 100;
	pidA.kI = 1000;
	pidA.kD = 0;
	pidB.setPoint = 8000;
	pidB.kP = 100;
	pidB.kI = 1000;
  while (1){
		speed_run(pidA.output,pidB.output);
		if(HAL_GPIO_ReadPin(U_BTN_GPIO_Port,U_BTN_Pin)==GPIO_PIN_RESET){
			pidA.setPoint = -10000;
			pidB.setPoint = -10000;
		}
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
