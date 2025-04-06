/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_1602.h"
#include "esp8266.h"
#include "ds18b20.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int temp_ban = 23;
int water_ban= 2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	bool fire_status = false;
	uint16_t adcx = 0;
	float adcy;
	char data_light[10]={0},data_show[10],data_tmp[4];
	  float num_d;;
	char send_wifi_data[30];
	static int _count_data=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
		LCD_INIT();
		DS_Init();

		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
		
		firelay_control(false);
		water_control(false);
		start_esp8266();
//		beep_control(true);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_ADC_Start(&hadc1);   
		HAL_ADC_PollForConversion(&hadc1,10); 
		adcx = (uint16_t)HAL_ADC_GetValue(&hadc1);  
		adcy = (float)adcx*3.3/4096;             
		sprintf(data_light,"Stage:%.3f",adcy);
		lcd1602_show_string(0,1,data_light);
		
		num_d = Get_DS_Temperature();	
		sprintf(data_show,"temp:%0.2f",num_d);	
		lcd1602_show_string(0,0,data_show);
		
		sprintf(data_tmp,"%02d",temp_ban);
		lcd1602_show_string(12,0,data_tmp);
		
				sprintf(data_tmp,"%02d",water_ban);
		lcd1602_show_string(12,1,data_tmp);
		
		_count_data++;
		if(_count_data/500)
		{
						sprintf(send_wifi_data,"temp:%0.2f water:%0.3f\r\n",num_d,adcy);
		send_wifi(send_wifi_data,30);
		}

		
		handle_esp8266();
		
		if(num_d>temp_ban)  //温度大于阈值了
		{
				firelay_control(false);
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,5);
		}
		else
		{
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,25);
		}
		
		if(adcy<water_ban)
		{
				beep_control(true);
		}
		else
		{
				beep_control(false);
		}
		
     if(botton==KEY1)
		 {
			   HAL_Delay(100);
			   botton=UNPRESS;
				fire_status=!fire_status;
				firelay_control(fire_status);

		 }
		 
		 if(botton==KEY2)
		 {
			   HAL_Delay(100);
			   botton=UNPRESS;
					water_control(true);
			 __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,25);
			 			   HAL_Delay(300);
					water_control(false);
		 }
		 
		 if(botton==KEY3)
		 {
			   HAL_Delay(100);
			   botton=UNPRESS;
				temp_ban--;
			  if(temp_ban<0)
					temp_ban=99;
		 }
		 
		 
		 	if(botton==KEY4)
		 {
			   HAL_Delay(100);
			   botton=UNPRESS;
				 temp_ban++;
			   if(temp_ban>99)
					 temp_ban=0;
		 }
		 


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
